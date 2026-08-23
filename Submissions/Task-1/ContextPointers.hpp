#pragma once

// --- 4d) The C-Style API ---
// Alias for a function that takes a reference to data, and a void context pointer
template <typename T>
using FContextFunc = void(*)(T&, void*);

// The generic processor that loops through and passes the context along
template <typename T>
void ProcessWithContext(T* arr, int size, FContextFunc<T> callback, void* context) {
  for (int i = 0; i < size; ++i) {
    callback(arr[i], context);
  }
}

// --- 4a) The Stateful Template Class ---
template <typename T>
class TDataAggregator {
private:
  T stateTotal;

public:
  TDataAggregator() : stateTotal(0) {} // Initialize state to 0

  T GetTotal() const {
    return stateTotal;
  }

  // --- 4b) The Internal Action ---
  // Requires a 'this' pointer to access stateTotal! Cannot be a standard callback.
  void Accumulate(T& val) {
    stateTotal += val;
    val *= 2;
  }

  // --- 4c) The Static Wrapper ---
  // Has NO 'this' pointer, acts like a global function, perfectly matches FContextFunc.
  static void StaticCallback(T& val, void* context) {
    // 1. Cast the generic void* back into our specific object type
    TDataAggregator<T>* instance = static_cast<TDataAggregator<T>*>(context);

    // 2. Safely call the internal method using the recovered instance pointer
    if (instance != nullptr) {
      instance->Accumulate(val);
    }
  }
};