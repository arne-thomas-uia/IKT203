#pragma once
#include <string>

// --- 2a) Basic Assignment ---
int Add(int a, int b);

// --- 2b) Type Aliases ---
std::string Greet(std::string name);
using GreetPtr = std::string(*)(std::string); // Modern C++ alias

// --- 2c) Passing Functions as Arguments ---
bool IsEven(int val);
bool IsOdd(int val);

// Using an alias for the predicate to keep the function signature clean
using FPredicate = bool(*)(int);
void FilterAndPrint(int* arr, int size, FPredicate filter);

// --- 2d) Array of Function Pointers ---
void Play();
void Pause();
void Quit();