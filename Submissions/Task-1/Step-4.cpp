#include <iostream>
#include "Task-1.h"
#include "ContextPointers.hpp"

// --- 4e) Putting it all together ---
void runStep4() {
  std::cout << "--- 4) OOP Template Class & Context Pointers ---\n";

  // 1. Instantiate the stateful object
  TDataAggregator<int> myAggregator;

  // 2. Create the array
  int numbers[] = { 10, 20, 30, 40, 50 };
  int size = sizeof(numbers) / sizeof(numbers[0]);

  std::cout << "Initial Array: ";
  for (int i = 0; i < size; ++i) std::cout << numbers[i] << " ";
  std::cout << "\nInitial State Total: " << myAggregator.GetTotal() << "\n\n";

  // 3. Execute the generic processor!
  // Pass the static wrapper as the callback, and the object's address (&myAggregator) as the context.
  ProcessWithContext(numbers, size, &TDataAggregator<int>::StaticCallback, &myAggregator);

  std::cout << "Modified Array (Doubled): ";
  for (int i = 0; i < size; ++i) std::cout << numbers[i] << " ";

  // 4. Verify the state was successfully updated across the void* boundary
  std::cout << "\nFinal State Total (Sum of originals): " << myAggregator.GetTotal() << "\n";
}