#include "app.h"
#include <iostream>
#include <string>
#include <atomic>
#include "DebugUtils.h"
#include "DataStructure.h"
#include "MiscUtils.h"
#include "StructureReader.h"
#include "TLocation.h"
#include "TNetworkBuilder.h"

// Functor matching FNodeRead: bool(const int, const int, const std::string&)
struct TNodeReader {
  TGraph<TLocation*>* Network;

  bool operator()(const int aIndex, const int aTotalCount, const std::string& aNodeName) {
    Debug::LogInfo("Read Node: {} ({} of {})", aNodeName, aIndex + 1, aTotalCount);
    TLocation* locationPayload = new TLocation{ "Arena/Hub" };
    Network->InsertVertex(aNodeName, locationPayload);
    return true; // Continue reading
  }
};

// Functor matching FEdgeRead: bool(const int, const int, const std::string&, const std::string&, float)
struct TEdgeReader {
  TGraph<TLocation*>* Network;

  bool operator()(const int /*aIndex*/, const int /*aTotalCount*/, const std::string& aSourceName, const std::string& aDestinationName, float aWeight) {
    Debug::LogInfo("Read Edge: {} --> {} (Weight: {})", aSourceName, aDestinationName, aWeight);
    // Cast the parsed float to int to match the TEdge weight requirement
    Network->InsertEdge(aSourceName, aDestinationName, static_cast<int>(aWeight));
    return true; // Continue reading
  }
};

template<typename... Args>
void PrintMessage(const std::string& aMessage, const Args&... aArgs) {
  std::cout << std::vformat(aMessage, std::make_format_args(aArgs...)) << std::endl;
}

// Functor for printing and tallying the final MST edges
struct TEdgePrinter {
  int* TotalCostTracker;

  void operator()(const TEdge<TLocation*>* aEdge, int /*aIndex*/) {
    PrintMessage("Connect: {} <---> {} (Cost: {})", aEdge->GetSource()->GetName(), aEdge->GetDestination()->GetName(), aEdge->GetWeight());
    *TotalCostTracker += aEdge->GetWeight();
  }
};

int RunApp(int /*argc*/, char* /*argv*/[]) {
  Debug::TLogger::SetLevel(Debug::ELevel::Verbose);
  std::string startNodeName = "Main Stage";
  TNetworkBuilder networkBuilder;

  // =========================================================================================
  // PART 1: PHYSICAL FIBER LAYOUT (Prim's Algorithm - Undirected Graph)
  // =========================================================================================
  PrintMessage("--- Initializing E-Sport Fiber Network (Prim's MST) ---");

  // false = Undirected (cables go both ways), true = Graph owns the TLocation payload
  TGraph<TLocation*> esportNetwork(false, true);

  TNodeReader nodeReader{ &esportNetwork };
  TEdgeReader edgeReader{ &esportNetwork };

  Debug::LogInfo("Loading network structure from file: {}", GraphEsportArenasFilePath);
  ReadGraphFromFile(GraphEsportArenasFilePath, nodeReader, edgeReader);

  Debug::LogInfo("Calculating optimal physical layout from '{}'", startNodeName);

  try {
    TLinkedList<TEdge<TLocation*>*>* minimumSpanningTree = networkBuilder.GeneratePhysicalLayout(&esportNetwork, startNodeName);

    int totalFiberCost = 0;
    PrintMessage("\n=== Approved Physical Fiber Layout ===");

    TEdgePrinter edgePrinter{ &totalFiberCost };
    minimumSpanningTree->ForEach(edgePrinter);

    PrintMessage("\n======================================");
    PrintMessage("Total Network Infrastructure Cost: {}", totalFiberCost);

    delete minimumSpanningTree; // Clean up the returned list
  }
  catch (const std::exception& e) {
    Debug::LogError("CRITICAL ROUTING ERROR (PRIM): {}", e.what());
    return 1;
  }

  // =========================================================================================
  // PART 2: EXPERIMENTAL BOOSTER NETWORK (Bellman-Ford - Directed Graph)
  // =========================================================================================
  PrintMessage("\n--- Initializing Experimental Booster Network (Bellman-Ford) ---");

  // true = Directed (boosters are one-way), true = Graph owns the TLocation payload
  TGraph<TLocation*> boosterNetwork(true, true);

  TNodeReader boosterNodeReader{ &boosterNetwork };
  TEdgeReader boosterEdgeReader{ &boosterNetwork };

  Debug::LogInfo("Loading booster network structure from file: {}", GraphEsportBoostersFilePath);
  ReadGraphFromFile(GraphEsportBoostersFilePath, boosterNodeReader, boosterEdgeReader);

  try {
    std::string boosterStartNode = "Austin Core";
    Debug::LogInfo("Calculating booster routing paths from '{}'", boosterStartNode);

    TRoutingResult* routingResult = networkBuilder.GenerateBoosterRouting(&boosterNetwork, boosterStartNode);

    if (routingResult->HasNegativeCycle) {
      Debug::LogError("ROUTING LOOP DETECTED: Negative cycle found in the booster network.");
      PrintMessage("\nCRITICAL FAILURE: Network loop detected in booster configuration!");
      PrintMessage("ABORTING CONNECTION TO PREVENT CASCADING FAILURE.");
      PrintMessage("\n--- EXTRACING CORRUPTED ROUTE TRACE ---");

      bool* visitedNodes = new bool[routingResult->TotalVertices] {false};

      // Start the trace exactly at the node trapped in the infinite glitch
      int traceIndex = routingResult->CorruptedNodeIndex;

      // Trace backward through the 'Previous' array until we hit a loop
      while (traceIndex < routingResult->TotalVertices) {
        if (visitedNodes[traceIndex]) {
          PrintMessage("  -> LOOP DETECTED AT: {}", routingResult->Vertices[traceIndex]->GetName());
          break; // We found the start of the infinite loop
        }

        visitedNodes[traceIndex] = true;
        TVertex<TLocation*>* currentNode = routingResult->Vertices[traceIndex];
        TVertex<TLocation*>* prevNode = routingResult->Previous[traceIndex];

        if (prevNode != nullptr) {
          PrintMessage("  Node: {} (Routed from: {})", currentNode->GetName(), prevNode->GetName());

          // Find the integer index of the prevNode to continue the trace
          for (int i = 0; i < routingResult->TotalVertices; i++) {
            if (routingResult->Vertices[i] == prevNode) {
              traceIndex = i;
              break;
            }
          }
        }
        else {
					PrintMessage("  Node: {} (No previous node, end of trace)", currentNode->GetName());
          break; // Reached the start or an unrouted node
        }
      }
      delete[] visitedNodes;

    }
    else {
      PrintMessage("\n=== Approved Booster Routing Paths ===");
      PrintMessage("SUCCESS: Booster network paths calculated. No infinite loops detected.");

      for (int i = 0; i < routingResult->TotalVertices; ++i) {
        if (routingResult->Distances[i] != INFINITY_INT_VALUE) {
          PrintMessage("Hub: {} | Distance: {}",
            routingResult->Vertices[i]->GetName(),
            routingResult->Distances[i]);
        }
      }
    }

    delete routingResult;
  }
  catch (const std::exception& e) {
    Debug::LogError("CRITICAL ROUTING ERROR (Bellman-Ford): {}", e.what());
    return 1;
  }

  return 0;
}