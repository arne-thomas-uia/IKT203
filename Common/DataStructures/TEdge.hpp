#pragma once
#include <string>
#include <type_traits>
#include "TLinkedList.hpp"

// Forward declaration
template <typename T>
class TVertex;

template <typename T>
class TEdge {
private:
  TVertex<T>* _source;
  TVertex<T>* _destination;
  int _weight;

public:
  TEdge(TVertex<T>* aSource, TVertex<T>* aDestination, int aWeight)
    : _source(aSource), _destination(aDestination), _weight(aWeight) {}

  TVertex<T>* GetSource() const { return _source; }
  TVertex<T>* GetDestination() const { return _destination; }
  int GetWeight() const { return _weight; }
};

