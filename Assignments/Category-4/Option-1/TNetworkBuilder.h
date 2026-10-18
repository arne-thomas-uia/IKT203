#pragma once
#include <string>
#include "DataStructure.h"
#include "TAlgorithmState.h"
#include "TLocation.h"


// The Data-Oriented bare-metal return structure
struct TRoutingResult {
  int* Distances;
  TVertex<TLocation*>** Previous;
  TVertex<TLocation*>** Vertices; // Maps the integer index back to the vertex
  int TotalVertices;
  bool HasNegativeCycle;
  int CorruptedNodeIndex;

  // The struct owns the ARRAYS, but not the TVertex pointers inside them.
  ~TRoutingResult() {
    delete[] Distances;
    delete[] Previous;
    delete[] Vertices;
  }
};

class TNetworkBuilder {
private:
  TAlgorithmState* GetOrCreateState(THashTable<TAlgorithmState*>& aStateTable, TVertex<TLocation*>* aVertex) const;

  static int CompareEdges(const TEdge<TLocation*>* aLeft, const TEdge<TLocation*>* aRight);

  // --- Functors for Prim's Algorithm ---
  struct TInitialEdgeSeeder {
    TPriorityQueue<TEdge<TLocation*>*>* EdgeQueue;
    void operator()(const TEdge<TLocation*>* aEdge, int aIndex) const;
  };

  struct TEdgeExpander {
    TPriorityQueue<TEdge<TLocation*>*>* EdgeQueue;
    THashTable<TAlgorithmState*>* StateTable;
    const TNetworkBuilder* Builder;
    void operator()(const TEdge<TLocation*>* aEdge, int aIndex) const;
  };

  // --- Functors for Bellman-Ford (Bare-Metal Arrays) ---

  // Initializes the flat arrays and builds the String -> Int index map
  struct TVertexInitializer {
    THashTable<int*>* IndexTable;
    int* Distances;
    TVertex<TLocation*>** Previous;
    TVertex<TLocation*>** Vertices;
    int* CurrentIndex;

    void operator()(const TVertex<TLocation*>* aVertex, int aIndex) const;
  };

  // Relaxes edges using strict O(1) array indexing
  struct TRelaxationFunctor {
    THashTable<int*>* IndexTable;
    int* Distances;
    TVertex<TLocation*>** Previous;
    bool* HasChanged;

    void operator()(const TEdge<TLocation*>* aEdge, int aIndex) const;
  };

public:
  TNetworkBuilder() = default;
  ~TNetworkBuilder() = default;

  TNetworkBuilder(const TNetworkBuilder&) = delete;
  TNetworkBuilder& operator=(const TNetworkBuilder&) = delete;

  TLinkedList<TEdge<TLocation*>*>* GeneratePhysicalLayout(TGraph<TLocation*>* aGraph, std::string aStartHubName) const;
  TRoutingResult* GenerateBoosterRouting(TGraph<TLocation*>* aGraph, std::string aStartHubName) const;
};