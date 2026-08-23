#include <iostream>
#include "Task-1.h"
#include "GlobalCallbacks.h"

void runStep2() {
  std::cout << "--- 2a) Basic Assignment & Invocation ---\n";
  // Declare and initialize the function pointer
  int (*mathPtr)(int, int) = &Add;

  // Explicit dereference (Old C-style)
  std::cout << "Explicit call: " << (*mathPtr)(5, 3) << "\n";
  // Implicit dereference (Modern C++ style)
  std::cout << "Implicit call: " << mathPtr(10, 7) << "\n";

  std::cout << "\n--- 2b) Making It Readable with Type Aliases ---\n";
  GreetPtr myGreeter = &Greet;
  std::cout << myGreeter("Master Yoda") << "\n";

  std::cout << "\n--- 2c) Passing Functions as Arguments (Callbacks) ---\n";
  int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  int size = sizeof(numbers) / sizeof(numbers[0]);

  std::cout << "Evens: ";
  FilterAndPrint(numbers, size, &IsEven);

  std::cout << "Odds: ";
  FilterAndPrint(numbers, size, &IsOdd);

  std::cout << "\n--- 2d) Array of Function Pointers ---\n";
  // Create the array of function pointers (the "Jump Table")
  using FAction = void(*)();
  FAction actions[] = { &Play, &Pause, &Quit };

  // Iterate and execute dynamically
  for (int i = 0; i < 3; ++i) {
    actions[i]();
  }
}