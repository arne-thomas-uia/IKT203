#include <iostream>
#include "Task-1.h"
#include "TemplateMastery.hpp"

void runStep1() {
  std::cout << "--- 1a) Testing PrintTypeSize ---\n";
  int testInt = 42;
  double testDouble = 3.14;
  TStudent testStruct{ 95 };

  PrintTypeSize(testInt);     // Expect 4
  PrintTypeSize(testDouble);  // Expect 8
  PrintTypeSize(testStruct);  // Expect 4 (depending on padding/architecture)

  std::cout << "\n--- 1b) Testing GetMax ---\n";
  int maxInt = GetMax(10, 20);
  std::cout << "Max Int: " << maxInt << "\n";

  TStudent studentA{ 75 };
  TStudent studentB{ 90 };
  TStudent topStudent = GetMax(studentA, studentB); // Uses overloaded >
  std::cout << "Top Student Grade: " << topStudent.grade << "\n";

  std::cout << "\n--- 1c) Testing TDataPack (Variadic) ---\n";
  // Creates a tuple-like struct holding an int, a double, and a char
  TDataPack<int, double, char> myData(100, 42.5, 'X');
  std::cout << "DataPack Value 1 (int): " << myData.value << "\n";
  std::cout << "DataPack Value 2 (double): " << myData.rest.value << "\n";
  std::cout << "DataPack Value 3 (char): " << myData.rest.rest.value << "\n";

  std::cout << "\n--- 1d) Testing TStaticArray ---\n";
  TStaticArray<int, 5> intArray;
  intArray.Populate(10);
  std::cout << "Array contents: ";
  intArray.PrintContents();

  std::cout << "\n--- 1e) Testing PrintContainer (Template-Template) ---\n";
  // Passing the TStaticArray blueprint into PrintContainer
  PrintContainer(intArray);
}