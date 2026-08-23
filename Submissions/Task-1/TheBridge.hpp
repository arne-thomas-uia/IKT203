#pragma once

// --- 3a) Generic Type Alias ---
// This alias defines a pointer to a function that takes a reference to T.
template <typename T>
using FActionFunc = void(*)(T&);

// --- 3b) The Generic Processor ---
// An algorithm that processes ANY array using ANY matching callback.
template <typename T>
void ProcessArray(T* arr, int size, FActionFunc<T> action) {
  for (int i = 0; i < size; ++i) {
    action(arr[i]); // Modifies the original data via reference
  }
}