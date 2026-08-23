#include "GlobalCallbacks.h"
#include <iostream>

// --- 2a ---
int Add(int a, int b) {
  return a + b;
}

// --- 2b ---
std::string Greet(std::string name) {
  return "Hello, " + name;
}

// --- 2c ---
bool IsEven(int val) {
  return val % 2 == 0;
}

bool IsOdd(int val) {
  return val % 2 != 0;
}

void FilterAndPrint(int* arr, int size, FPredicate filter) {
  for (int i = 0; i < size; ++i) {
    if (filter(arr[i])) {
      std::cout << arr[i] << " ";
    }
  }
  std::cout << "\n";
}

// --- 2d ---
void Play() {
  std::cout << "Playing game...\n";
}

void Pause() {
  std::cout << "Game paused.\n";
}

void Quit() {
  std::cout << "Quitting game.\n";
}