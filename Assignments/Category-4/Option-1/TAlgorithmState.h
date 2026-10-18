#pragma once
#include "DataStructure.h"
#include "TLocation.h"

// Extern declaration so it can be shared without multiple definition errors
extern const int INFINITY_INT_VALUE;

class TAlgorithmState {
private:
  bool _isVisited;
  int _tentativeDistance;
  TVertex<TLocation*>* _previousVertex;

public:
  TAlgorithmState();

  // --- Getters ---
  bool GetIsVisited() const;
  int GetTentativeDistance() const;
  TVertex<TLocation*>* GetPreviousVertex() const;

  // --- Setters ---
  void SetIsVisited(bool aIsVisited);
  void SetTentativeDistance(int aDistance);
  void SetPreviousVertex(TVertex<TLocation*>* aPrevious);
};