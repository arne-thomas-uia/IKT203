#include <iostream>
#include "Task-1.h"
#include "TheBridge.hpp"

// --- 3c) Concrete Callbacks ---
void DoubleInt(int& val) {
  val *= 2;
}

void CapitalizeChar(char& val) {
  // Standard ASCII shift to capitalize lowercase letters
  if (val >= 'a' && val <= 'z') {
    val -= 32;
  }
}

// A quick helper function for printing arrays
template <typename T>
void PrintArray(T* arr, int size) {
  for (int i = 0; i < size; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << "\n";
}

// --- 3d) Testing the Bridge ---
void runStep3() {
  std::cout << "--- 3) Callbacks + Templates (In-Place Modification) ---\n";

  // Testing with Integers
  int intArr[] = { 1, 2, 3, 4, 5 };
  int intSize = sizeof(intArr) / sizeof(intArr[0]);

  std::cout << "Integers before: ";
  PrintArray(intArr, intSize);

  ProcessArray(intArr, intSize, &DoubleInt);

  std::cout << "Integers after:  ";
  PrintArray(intArr, intSize);

  // Testing with Characters
  char charArr[] = { 'a', 'g', 'd', 'e', 'r' };
  int charSize = sizeof(charArr) / sizeof(charArr[0]);

  std::cout << "\nCharacters before: ";
  PrintArray(charArr, charSize);

  ProcessArray(charArr, charSize, &CapitalizeChar);

  std::cout << "Characters after:  ";
  PrintArray(charArr, charSize);
}