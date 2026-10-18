#pragma once
#include <string>
#include <functional>
#include <stdexcept>
#include "THashTable.hpp"
#include "TVertex.hpp"
#include "TEdge.hpp"

template <typename T>
class TGraph {
private:
  THashTable<TVertex<T>*> _vertexMap; // Maps Name -> TVertex pointer in O(1)
  TLinkedList<TEdge<T>*> _masterEdgeList; // The "Edge List" for memory cleanup and Bellman-Ford
  TLinkedList<TVertex<T>*> _masterVertexList; // The "Vertex List" for memory cleanup
  bool _isDirected;
  bool _isDataOwner; // Determines if TGraph deletes the T payload inside TVertex

  // Delegated named function for edge deletion
  static void DeleteEdge(const TEdge<T>* aEdge, int /*aIndex*/) {
    TEdge<T>* mutableEdge = const_cast<TEdge<T>*>(aEdge);
    delete mutableEdge;
  }

  // Functor for vertex and payload deletion (allows us to pass state without lambdas)
  struct TVertexDeleter {
    bool IsDataOwner;

    void operator()(const TVertex<T>* aVertex, int /*aIndex*/) const {
      if (aVertex == nullptr) return;

      // Explicitly cast away constness because our explicit intent is destruction
      TVertex<T>* mutableVertex = const_cast<TVertex<T>*>(aVertex);

      if (IsDataOwner && mutableVertex->GetPayload() != nullptr) {
        delete mutableVertex->GetPayload();
      }
      delete mutableVertex;
    }
  };

public:
  TGraph(bool aIsDirected, bool aIsDataOwner = false)
    : _vertexMap(false), // The hash table must NOT own the data; TGraph owns it.
    _masterEdgeList(false),
    _masterVertexList(false),
    _isDirected(aIsDirected),
    _isDataOwner(aIsDataOwner) {

    static_assert(std::is_pointer<T>::value, "TGraph: T must be a pointer type.");
  }

  // Rule of Three protections
  TGraph(const TGraph&) = delete;
  TGraph& operator=(const TGraph&) = delete;

  ~TGraph() {
    // 1. Delete all TEdge pointers manually
    _masterEdgeList.ForEach(DeleteEdge);

    // 2. Delete all TVertex pointers (and their payloads if owned)
    TVertexDeleter vertexDeleter{ _isDataOwner };
    _masterVertexList.ForEach(vertexDeleter);
  }

  void InsertVertex(std::string aName, T aPayload) {
    if (_vertexMap.Search(aName) != nullptr) {
      return; // Prevent duplicates
    }
    TVertex<T>* newVertex = new TVertex<T>(aName, aPayload);
    _vertexMap.Insert(aName, newVertex);
    _masterVertexList.Append(newVertex);
  }

  void InsertEdge(std::string aSourceName, std::string aDestinationName, int aWeight) {
    TVertex<T>* sourceVertex = _vertexMap.Search(aSourceName);
    TVertex<T>* destVertex = _vertexMap.Search(aDestinationName);

    if (sourceVertex == nullptr || destVertex == nullptr) {
      throw std::invalid_argument("TGraph: Cannot create edge. Vertex not found.");
    }

    TEdge<T>* newEdge = new TEdge<T>(sourceVertex, destVertex, aWeight);
    sourceVertex->AddEdge(newEdge);
    _masterEdgeList.Append(newEdge);

    if (!_isDirected) {
      TEdge<T>* reverseEdge = new TEdge<T>(destVertex, sourceVertex, aWeight);
      destVertex->AddEdge(reverseEdge);
      _masterEdgeList.Append(reverseEdge);
    }
  }

  TVertex<T>* GetVertex(std::string aName) const {
    return _vertexMap.Search(aName);
  }

  int GetVertexCount() const {
    return _masterVertexList.GetSize();
  }

  TLinkedList<TEdge<T>*>& GetMasterEdgeList() {
    return _masterEdgeList;
  }

  TLinkedList<TVertex<T>*>& GetMasterVertexList() {
    return _masterVertexList;
  }
};