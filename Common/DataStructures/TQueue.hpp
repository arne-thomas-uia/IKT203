#pragma once
#include "TCollection.hpp"
#include "TLinkedList.hpp"
#include "DebugUtils.h"
#include <stdexcept>

template <typename T>
class TQueue : public TCollection<T> {
private:
  TLinkedList<T> list; // The internal engine doing the heavy lifting

public:
  // Explicitly tell the list NOT to own the data!
  TQueue() : list(false) {}
  ~TQueue() override = default;

  // --- TCollection Implementation ---
  int GetSize() const override;
  bool IsEmpty() const override;
  void Clear() override;

  // --- Queue Specific Rules (FIFO) ---
  void Enqueue(T item);
  T Dequeue();
  T Peek() const;
};

// --- Implementations ---

template <typename T>
int TQueue<T>::GetSize() const {
  return list.GetSize();
}

template <typename T>
bool TQueue<T>::IsEmpty() const {
  return list.IsEmpty();
}

template <typename T>
void TQueue<T>::Clear() {
  list.Clear();
}

template <typename T>
void TQueue<T>::Enqueue(T item) {
  // Line up at the back!
  list.Append(item);
}

template <typename T>
T TQueue<T>::Peek() const {
  if (IsEmpty()) {
    Debug::TLogger::Log(Debug::ELevel::Warning, "Peek called on an empty queue");
    return nullptr;
  }
  // Look at the front of the line
  return list.GetAtIndex(0);
}

template <typename T>
T TQueue<T>::Dequeue() {
  if (IsEmpty()) {
    Debug::TLogger::Log(Debug::ELevel::Warning, "Dequeue called on an empty queue");
    return nullptr;
  }

  // Serve the front of the line, then remove them
  T item = list.GetAtIndex(0);
  list.Remove(item);
  return item;
}