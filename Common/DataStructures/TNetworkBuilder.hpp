#pragma once
#include <string>
#include <stdexcept>
#include "TGraph.hpp"
#include "TPriorityQueue.hpp"
#include "THashTable.hpp"
#include "TAlgorithmState.hpp"
#include "TLinkedList.hpp"

template <typename T>
class TNetworkBuilder {
private:
  // Helper method to safely get or create state to strictly enforce DRY
  TAlgorithmState<T>* GetOrCreateState(THashTable<TAlgorithmState<T>*>& aStateTable, TVertex<T>* aVertex) const {
    std::string vertexName = aVertex->GetName();
    TAlgorithmState<T>* state = aStateTable.Search(vertexName);

    if (state == nullptr) {
      state = new TAlgorithmState<T>();
      aStateTable.Insert(vertexName, state);
    }
    return state;
  }

public:
  TNetworkBuilder() = default;
  ~TNetworkBuilder() = default;

  // Rule of Three protections
  TNetworkBuilder(const TNetworkBuilder&) = delete;
  TNetworkBuilder& operator=(const TNetworkBuilder&) = delete;

  // Executes Prim's Algorithm to find the Minimum Spanning Tree
  // Returns a dynamically allocated TLinkedList containing the edges that form the MST.
  // The caller is responsible for deleting the returned list (but NOT the edges inside it).
  TLinkedList<TEdge<T>*>* GeneratePhysicalLayout(TGraph<T>* aGraph, std::string aStartHubName) const {
    if (aGraph == nullptr) {
      throw std::invalid_argument("TNetworkBuilder: Cannot generate layout from a null graph.");
    }

    TVertex<T>* startVertex = aGraph->GetVertex(aStartHubName);
    if (startVertex == nullptr) {
      throw std::invalid_argument("TNetworkBuilder: Start hub not found in the network.");
    }

    // 1. The Output Container: A list of edges forming the MST. (Does NOT own the edge pointers)
    TLinkedList<TEdge<T>*>* minimumSpanningTree = new TLinkedList<TEdge<T>*>(false);

    // 2. The Algorithm State Table: Maps Vertex Name -> TAlgorithmState pointer.
    // Instantiated with 'true' so the THashTable explicitly owns and deletes the states.
    THashTable<TAlgorithmState<T>*> stateTable(true);

    // 3. The Priority Queue (Min-Heap): Prioritizes edges by lowest weight.
    auto edgeComparator = [](const TEdge<T>* aLeft, const TEdge<T>* aRight) {
      return aLeft->GetWeight() - aRight->GetWeight();
      };

    // Assuming a maximum safe capacity for edges in the PQ
    TPriorityQueue<TEdge<T>*> edgeQueue(10000, EHeapType::Min, edgeComparator, false);

    // --- Algorithm Initialization ---
    TAlgorithmState<T>* startState = GetOrCreateState(stateTable, startVertex);
    startState->SetIsVisited(true);

    // Seed the PQ with all edges incident to the start vertex
    startVertex->GetIncidentEdges().ForEach([&edgeQueue](const TEdge<T>* aEdge, int /*aIndex*/) {
      edgeQueue.Enqueue(const_cast<TEdge<T>*>(aEdge));
      });

    // --- Algorithm Execution (Lazy Prim's) ---
    while (!edgeQueue.IsEmpty()) {
      TEdge<T>* currentEdge = edgeQueue.Dequeue();
      TVertex<T>* destinationVertex = currentEdge->GetDestination();

      TAlgorithmState<T>* destState = GetOrCreateState(stateTable, destinationVertex);

      // If the destination is already in the MST, this edge is obsolete (a cycle). Skip it.
      if (destState->GetIsVisited()) {
        continue;
      }

      // Otherwise, we have found a valid edge for our MST
      destState->SetIsVisited(true);
      minimumSpanningTree->Append(currentEdge);

      // Add all incident edges of the newly visited vertex to the PQ
      destinationVertex->GetIncidentEdges().ForEach([&edgeQueue, &stateTable, this](const TEdge<T>* aEdge, int /*aIndex*/) {
        TVertex<T>* nextDest = aEdge->GetDestination();
        TAlgorithmState<T>* nextState = this->GetOrCreateState(stateTable, nextDest);

        // Only enqueue edges that lead to unvisited vertices
        if (!nextState->GetIsVisited()) {
          edgeQueue.Enqueue(const_cast<TEdge<T>*>(aEdge));
        }
        });
    }

    // The stateTable goes out of scope here and automatically deletes all TAlgorithmState pointers.
    return minimumSpanningTree;
  }
};
