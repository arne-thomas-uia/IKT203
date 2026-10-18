#include "TNetworkBuilder.h"
#include <stdexcept>

// --- Helper Methods ---

TAlgorithmState* TNetworkBuilder::GetOrCreateState(THashTable<TAlgorithmState*>& aStateTable, TVertex<TLocation*>* aVertex) const {
  std::string vertexName = aVertex->GetName();
  TAlgorithmState* state = aStateTable.Search(vertexName);

  if (state == nullptr) {
    state = new TAlgorithmState();
    aStateTable.Insert(vertexName, state);
  }
  return state;
}

int TNetworkBuilder::CompareEdges(const TEdge<TLocation*>* aLeft, const TEdge<TLocation*>* aRight) {
  return aLeft->GetWeight() - aRight->GetWeight();
}

// --- Functor Implementations ---

void TNetworkBuilder::TInitialEdgeSeeder::operator()(const TEdge<TLocation*>* aEdge, int /*aIndex*/) const {
  EdgeQueue->Enqueue(const_cast<TEdge<TLocation*>*>(aEdge));
}

void TNetworkBuilder::TEdgeExpander::operator()(const TEdge<TLocation*>* aEdge, int /*aIndex*/) const {
  TVertex<TLocation*>* nextDest = aEdge->GetDestination();
  TAlgorithmState* nextState = Builder->GetOrCreateState(*StateTable, nextDest);

  if (!nextState->GetIsVisited()) {
    EdgeQueue->Enqueue(const_cast<TEdge<TLocation*>*>(aEdge));
  }
}

void TNetworkBuilder::TVertexInitializer::operator()(const TVertex<TLocation*>* aVertex, int /*aIndex*/) const {
  int index = *CurrentIndex;

  Vertices[index] = const_cast<TVertex<TLocation*>*>(aVertex);
  Distances[index] = INFINITY_INT_VALUE;
  Previous[index] = nullptr;

  // T must be a pointer per the Gentlemen's Rules, so we allocate the int on the heap
  IndexTable->Insert(aVertex->GetName(), new int(index));

  (*CurrentIndex)++;
}

void TNetworkBuilder::TRelaxationFunctor::operator()(const TEdge<TLocation*>* aEdge, int /*aIndex*/) const {
  int* sourceIndexPtr = IndexTable->Search(aEdge->GetSource()->GetName());
  int* destIndexPtr = IndexTable->Search(aEdge->GetDestination()->GetName());

  if (sourceIndexPtr == nullptr || destIndexPtr == nullptr) return;

  int u = *sourceIndexPtr;
  int v = *destIndexPtr;
  int weight = aEdge->GetWeight();

  // Array-based relaxation
  if (Distances[u] != INFINITY_INT_VALUE && (Distances[u] + weight < Distances[v])) {
    Distances[v] = Distances[u] + weight;
    Previous[v] = aEdge->GetSource();
    *HasChanged = true;
  }
}

// --- Main Algorithm Implementations ---

TLinkedList<TEdge<TLocation*>*>* TNetworkBuilder::GeneratePhysicalLayout(TGraph<TLocation*>* aGraph, std::string aStartHubName) const {
  if (aGraph == nullptr) throw std::invalid_argument("Graph is null.");
  TVertex<TLocation*>* startVertex = aGraph->GetVertex(aStartHubName);
  if (startVertex == nullptr) throw std::invalid_argument("Start hub not found.");

  TLinkedList<TEdge<TLocation*>*>* minimumSpanningTree = new TLinkedList<TEdge<TLocation*>*>(false);
  THashTable<TAlgorithmState*> stateTable(true);
  TPriorityQueue<TEdge<TLocation*>*> edgeQueue(10000, EHeapType::Min, CompareEdges, false);

  TAlgorithmState* startState = GetOrCreateState(stateTable, startVertex);
  startState->SetIsVisited(true);

  TInitialEdgeSeeder seeder{ &edgeQueue };
  startVertex->GetIncidentEdges().ForEach(seeder);

  TEdgeExpander expander{ &edgeQueue, &stateTable, this };

  while (!edgeQueue.IsEmpty()) {
    TEdge<TLocation*>* currentEdge = edgeQueue.Dequeue();
    TVertex<TLocation*>* destinationVertex = currentEdge->GetDestination();

    TAlgorithmState* destState = GetOrCreateState(stateTable, destinationVertex);

    if (destState->GetIsVisited()) continue;

    destState->SetIsVisited(true);
    minimumSpanningTree->Append(currentEdge);

    destinationVertex->GetIncidentEdges().ForEach(expander);
  }

  return minimumSpanningTree;
}

TRoutingResult* TNetworkBuilder::GenerateBoosterRouting(TGraph<TLocation*>* aGraph, std::string aStartHubName) const {
  if (aGraph == nullptr) throw std::invalid_argument("Graph is null.");
  TVertex<TLocation*>* startVertex = aGraph->GetVertex(aStartHubName);
  if (startVertex == nullptr) throw std::invalid_argument("Start hub not found.");

  int totalVertices = aGraph->GetVertexCount();

  // 1. Initialize the bare-metal arrays
  TRoutingResult* result = new TRoutingResult();
  result->TotalVertices = totalVertices;
  result->HasNegativeCycle = false;
	result->CorruptedNodeIndex = -1;
  result->Distances = new int[totalVertices];
  result->Previous = new TVertex<TLocation*>*[totalVertices];
  result->Vertices = new TVertex<TLocation*>*[totalVertices];

  // Temporary lookup table to strictly map String -> Int without mutating TVertex. 
  // 'true' ensures the dynamically allocated ints are deleted automatically.
  THashTable<int*> indexTable(true);
  int currentIndex = 0;

  TVertexInitializer initializer{ &indexTable, result->Distances, result->Previous, result->Vertices, &currentIndex };
  aGraph->GetMasterVertexList().ForEach(initializer);

  // Set the start node distance to 0
  int* startIndexPtr = indexTable.Search(aStartHubName);
  if (startIndexPtr != nullptr) {
    result->Distances[*startIndexPtr] = 0;
  }

  // 2. The Optimized V-1 Passes
  int passCount = 0;
  bool changed = false;
  TRelaxationFunctor relaxer{ &indexTable, result->Distances, result->Previous, &changed };

  do {
    changed = false;
    passCount++;

    aGraph->GetMasterEdgeList().ForEach(relaxer);

  } while (changed && passCount < totalVertices);

  // 3. The Negative Cycle Flag
  if (changed) {
    result->HasNegativeCycle = true;
    result->CorruptedNodeIndex = -1;
    aGraph->GetMasterEdgeList().ForEach([&](const TEdge<TLocation*>* aEdge, int) {
      int* sourceIndexPtr = indexTable.Search(aEdge->GetSource()->GetName());
      int* destIndexPtr = indexTable.Search(aEdge->GetDestination()->GetName());
      if (!sourceIndexPtr || !destIndexPtr) return;

      int u = *sourceIndexPtr;
      int v = *destIndexPtr;
      int weight = aEdge->GetWeight();

      if (result->Distances[u] != INFINITY_INT_VALUE && (result->Distances[u] + weight < result->Distances[v])) {
        result->HasNegativeCycle = true;
        result->CorruptedNodeIndex = v; // Save the exact node that is trapped in the loop
      }
      });
  }

  // indexTable goes out of scope and cleans up its integer pointers.
  // The flat arrays are returned intact for mapping.
  return result;
}