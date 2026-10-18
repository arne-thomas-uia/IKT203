#include "app.h"
#include <iostream>
#include <string>
#include <atomic>
#include "DebugUtils.h"
#include "DataStructure.h"
#include "MiscUtils.h"
#include "StructureReader.h"

struct TLocation {
  std::string Category;
};

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

	PrintMessage("--- Initializing E-Sport Fiber Network (Prim's MST) ---");

  TGraph<TLocation*> esportNetwork(false, true);

  TNodeReader nodeReader{};
  nodeReader.Network = &esportNetwork;

  TEdgeReader edgeReader{};
  edgeReader.Network = &esportNetwork;

  Debug::LogInfo("Loading network structure from file: {}", GraphEsportArenasFilePath);
  // Using the exact function signature provided in StructureReader.h
  ReadGraphFromFile(GraphEsportArenasFilePath, nodeReader, edgeReader);

  std::string startNodeName = "Main Stage";

  TNetworkBuilder<TLocation*> networkBuilder;
  Debug::LogInfo("Calculating optimal physical layout from '{}'", startNodeName);

  try {
    TLinkedList<TEdge<TLocation*>*>* minimumSpanningTree = networkBuilder.GeneratePhysicalLayout(&esportNetwork, startNodeName);

    int totalFiberCost = 0;
    Debug::LogInfo("Total Fiber Cost: {}", totalFiberCost);

		PrintMessage("\n=== Approved Physical Fiber Layout ===");

    TEdgePrinter edgePrinter;
    edgePrinter.TotalCostTracker = &totalFiberCost;

    minimumSpanningTree->ForEach(edgePrinter);

		PrintMessage("\n======================================");
    PrintMessage("Total Network Infrastructure Cost: {}", totalFiberCost);

    delete minimumSpanningTree;

  }
  catch (const std::exception& e) {
		Debug::LogError("CRITICAL ROUTING ERROR: {}", e.what());
    return 1;
  }


  return 0;
}