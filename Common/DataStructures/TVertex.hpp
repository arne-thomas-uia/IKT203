#pragma once
#include <string>
#include <type_traits>
#include "TLinkedList.hpp"

// Forward declaration
template <typename T>
class TEdge;

template <typename T>
class TVertex {
private:
  std::string _name;
  T _payload; // e.g., A pointer to a specific THub or TArena object
  TLinkedList<TEdge<T>*> _incidentEdges;

public:
  TVertex(std::string aName, T aPayload) : _name(aName), _payload(aPayload), _incidentEdges(false) {
    // Enforce pointer semantics per the Gentlemen's Rules
    static_assert(std::is_pointer<T>::value, "TVertex: T must be a pointer type.");
  }

  std::string GetName() const { return _name; }
  T GetPayload() const { return _payload; }
  TLinkedList<TEdge<T>*>& GetIncidentEdges() { return _incidentEdges; }

  void AddEdge(TEdge<T>* aEdge) {
    _incidentEdges.Append(aEdge);
  }
};