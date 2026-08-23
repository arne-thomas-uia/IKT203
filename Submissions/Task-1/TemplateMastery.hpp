#pragma once
#include <iostream>

// ==========================================
// 1a) PrintTypeSize
// ==========================================
template <typename T>
void PrintTypeSize(T v) {
  std::cout << "Size of type: " << sizeof(v) << " bytes\n";
}

// ==========================================
// 1b) GetMax
// ==========================================
template <typename T>
T GetMax(T a, T b) {
  return (a > b) ? a : b;
}

// Custom struct to test GetMax
struct TStudent {
  int grade;

  // The Twist: Overloading the > operator so GetMax can compile
  bool operator>(const TStudent& other) const {
    return this->grade > other.grade;
  }
};

// ==========================================
// 1c) Variadic Template Struct (TDataPack)
// ==========================================
// Base case for the recursive struct
template <typename... Args>
struct TDataPack {};

// Recursive case: Holds one value, and inherits/contains the rest
template <typename T, typename... Rest>
struct TDataPack<T, Rest...> {
  T value;
  TDataPack<Rest...> rest;

  // Constructor to initialize the pack
  TDataPack(T v, Rest... r) : value(v), rest(r...) {}
};

// ==========================================
// 1d) Template Array (TStaticArray)
// ==========================================
template <typename T, int Size>
class TStaticArray {
private:
  T data[Size]; // Fixed size array on the stack

public:
  void Populate(T startValue) {
    for (int i = 0; i < Size; ++i) {
      data[i] = startValue + static_cast<T>(i);
    }
  }

  void SetElement(int index, T value) {
    if (index >= 0 && index < Size) {
      data[index] = value;
    }
  }

  void PrintContents() const {
    for (int i = 0; i < Size; ++i) {
      std::cout << data[i] << " ";
    }
    std::cout << "\n";
  }
};

// ==========================================
// 1e) Template-Template Parameters
// ==========================================
// This function strictly expects a template class that takes <typename, int>
template <template <typename, int> class Container, typename T, int N>
void PrintContainer(const Container<T, N>& container) {
  std::cout << "Printing a container of size " << N << ":\n";
  container.PrintContents();
}