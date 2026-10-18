#include "TAlgorithmState.h"
#include <limits>

// Definition of the global constant
const int INFINITY_INT_VALUE = std::numeric_limits<int>::max();

TAlgorithmState::TAlgorithmState()
  : _isVisited(false),
  _tentativeDistance(INFINITY_INT_VALUE),
  _previousVertex(nullptr) {}

bool TAlgorithmState::GetIsVisited() const {
  return _isVisited;
}

int TAlgorithmState::GetTentativeDistance() const {
  return _tentativeDistance;
}

TVertex<TLocation*>* TAlgorithmState::GetPreviousVertex() const {
  return _previousVertex;
}

void TAlgorithmState::SetIsVisited(bool aIsVisited) {
  _isVisited = aIsVisited;
}

void TAlgorithmState::SetTentativeDistance(int aDistance) {
  _tentativeDistance = aDistance;
}

void TAlgorithmState::SetPreviousVertex(TVertex<TLocation*>* aPrevious) {
  _previousVertex = aPrevious;
}