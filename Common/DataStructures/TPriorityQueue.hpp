#pragma once
#include "TCollection.hpp"
#include "../DebugUtils.h"
#include <stdexcept>
#include <functional>
#include <type_traits>

// Enum to dictate the heap's behavioral rule
enum class EHeapType {
  Min,
  Max
};

// Delegate for comparing two data objects. 
// Should return:
// < 0 if node1 < node2
//   0 if node1 == node2
// > 0 if node1 > node2
template <typename T>
using FCompareElements = std::function<int(const T, const T)>;

template <typename T>
class TPriorityQueue : public TCollection<T> {
private:
  T* _heapArray;                  // Array of pointers
  int _capacity;                  // Maximum allowed size
  int _size;                      // Current number of elements
  EHeapType _heapType;            // Min or Max heap
  bool _isDataOwner;              // Memory ownership flag
  FCompareElements<T> _comparator; // The injected comparison logic

  // --- Mathematical Index Helpers O(1) ---
  int GetParentIndex(int aIndex) const;
  int GetLeftChildIndex(int aIndex) const;
  int GetRightChildIndex(int aIndex) const;

  // --- Core Heap Operations ---
  void Swap(int aIndexOne, int aIndexTwo);
  void HeapifyUp(int aIndex);
  void HeapifyDown(int aIndex);

  // Evaluates if the element at indexOne has higher priority than indexTwo
  bool HasHigherPriority(int aIndexOne, int aIndexTwo) const;

public:
  TPriorityQueue(int aCapacity, EHeapType aHeapType, FCompareElements<T> aComparator, bool aIsDataOwner = false);
  ~TPriorityQueue() override;

  // --- Memory Protection (Rule of Three) ---
  // Explicitly delete copy constructor and assignment operator to prevent shallow copy double-free crashes.
  TPriorityQueue(const TPriorityQueue&) = delete;
  TPriorityQueue& operator=(const TPriorityQueue&) = delete;

  // --- TCollection Implementation ---
  int GetSize() const override;
  bool IsEmpty() const override;
  void Clear() override;

  // --- Priority Queue Specifics ---
  void Enqueue(T aItem);
  T Dequeue();
  T Peek() const;
};

// --- Implementations ---

template <typename T>
TPriorityQueue<T>::TPriorityQueue(int aCapacity, EHeapType aHeapType, FCompareElements<T> aComparator, bool aIsDataOwner)
  : _capacity(aCapacity), _size(0), _heapType(aHeapType), _comparator(aComparator), _isDataOwner(aIsDataOwner) {

  // Gentlemen's Rule: Enforce pointer types strictly at compile time.
  static_assert(std::is_pointer<T>::value, "TPriorityQueue: T must be a pointer type.");

  if (aCapacity <= 0) {
    throw std::invalid_argument("TPriorityQueue: Capacity must be greater than zero.");
  }
  if (_comparator == nullptr) {
    throw std::invalid_argument("TPriorityQueue: A comparator delegate must be provided.");
  }

  _heapArray = new T[_capacity];
}

template <typename T>
TPriorityQueue<T>::~TPriorityQueue() {
  Clear();
  delete[] _heapArray; // Free the array itself
}

template <typename T>
int TPriorityQueue<T>::GetSize() const {
  return _size;
}

template <typename T>
bool TPriorityQueue<T>::IsEmpty() const {
  return _size == 0;
}

template <typename T>
void TPriorityQueue<T>::Clear() {
  if (_isDataOwner) {
    for (int i = 0; i < _size; i++) {
      delete _heapArray[i];
    }
  }
  _size = 0;
}

template <typename T>
int TPriorityQueue<T>::GetParentIndex(int aIndex) const {
  return (aIndex - 1) / 2;
}

template <typename T>
int TPriorityQueue<T>::GetLeftChildIndex(int aIndex) const {
  return (2 * aIndex) + 1;
}

template <typename T>
int TPriorityQueue<T>::GetRightChildIndex(int aIndex) const {
  return (2 * aIndex) + 2;
}

template <typename T>
void TPriorityQueue<T>::Swap(int aIndexOne, int aIndexTwo) {
  T temp = _heapArray[aIndexOne];
  _heapArray[aIndexOne] = _heapArray[aIndexTwo];
  _heapArray[aIndexTwo] = temp;
}

template <typename T>
bool TPriorityQueue<T>::HasHigherPriority(int aIndexOne, int aIndexTwo) const {
  int comparisonResult = _comparator(_heapArray[aIndexOne], _heapArray[aIndexTwo]);

  if (_heapType == EHeapType::Max) {
    return comparisonResult > 0; // In a Max heap, the larger element has priority
  }
  else {
    return comparisonResult < 0; // In a Min heap, the smaller element has priority
  }
}

template <typename T>
void TPriorityQueue<T>::HeapifyUp(int aIndex) {
  int currentIndex = aIndex;
  int parentIndex = GetParentIndex(currentIndex);

  // Bubble up as long as we are not the root and we have higher priority than our parent
  while (currentIndex > 0 && HasHigherPriority(currentIndex, parentIndex)) {
    Swap(currentIndex, parentIndex);
    currentIndex = parentIndex;
    parentIndex = GetParentIndex(currentIndex);
  }
}

template <typename T>
void TPriorityQueue<T>::HeapifyDown(int aIndex) {
  int currentIndex = aIndex;

  while (GetLeftChildIndex(currentIndex) < _size) {
    int highestPriorityChildIndex = GetLeftChildIndex(currentIndex);
    int rightChildIndex = GetRightChildIndex(currentIndex);

    // Check if a right child exists and if it has higher priority than the left child
    if (rightChildIndex < _size && HasHigherPriority(rightChildIndex, highestPriorityChildIndex)) {
      highestPriorityChildIndex = rightChildIndex;
    }

    // If the current node already has higher priority than both children, the heap is valid
    if (HasHigherPriority(currentIndex, highestPriorityChildIndex)) {
      break;
    }

    Swap(currentIndex, highestPriorityChildIndex);
    currentIndex = highestPriorityChildIndex;
  }
}

template <typename T>
void TPriorityQueue<T>::Enqueue(T aItem) {
  if (_size >= _capacity) {
    throw std::out_of_range("TPriorityQueue: Cannot Enqueue, the heap has reached its maximum capacity.");
  }

  _heapArray[_size] = aItem;
  _size++; // Increment first so the heap knows it has a new member
  HeapifyUp(_size - 1);
}

template <typename T>
T TPriorityQueue<T>::Dequeue() {
  if (IsEmpty()) {
    throw std::out_of_range("TPriorityQueue: Cannot Dequeue, the heap is currently empty.");
  }

  T highestPriorityItem = _heapArray[0];

  // Move the last element to the root and shrink the size
  _heapArray[0] = _heapArray[_size - 1];
  _size--;

  // Restore the heap property
  HeapifyDown(0);

  return highestPriorityItem;
}

template <typename T>
T TPriorityQueue<T>::Peek() const {
  if (IsEmpty()) {
    Debug::TLogger::Log(Debug::ELevel::Warning, "TPriorityQueue: Peek called on an empty heap.");
    return nullptr;
  }
  return _heapArray[0];
}