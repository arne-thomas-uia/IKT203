#pragma once
#include "TVertex.hpp"
#include <limits>
#include <type_traits>

// Primitive constant for infinite distance per the naming conventions
const int INFINITY_INT_VALUE = std::numeric_limits<int>::max();

template <typename T>
class TAlgorithmState {
private:
  bool _isVisited;
  int _tentativeDistance;
  TVertex<T>* _previousVertex; // Pointer to the parent vertex for path reconstruction

public:
  TAlgorithmState()
    : _isVisited(false),
    _tentativeDistance(INFINITY_INT_VALUE),
    _previousVertex(nullptr) {

    // Gentlemen's Rule: Enforce pointer types strictly at compile time.
    static_assert(std::is_pointer<T>::value, "TAlgorithmState: T must be a pointer type.");
  }

  // --- Getters ---
  bool GetIsVisited() const { return _isVisited; }
  int GetTentativeDistance() const { return _tentativeDistance; }
  TVertex<T>* GetPreviousVertex() const { return _previousVertex; }

  // --- Setters ---
  void SetIsVisited(bool aIsVisited) { _isVisited = aIsVisited; }
  void SetTentativeDistance(int aDistance) { _tentativeDistance = aDistance; }
  void SetPreviousVertex(TVertex<T>* aPrevious) { _previousVertex = aPrevious; }
};