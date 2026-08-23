#include <iostream> 
#include "Task-1.h"

int main()
{
	std::cout << "--- Running Step 1 Tests ---\n";
	runStep1();
	
	std::cout << "\n--- Running Step 2 Tests ---\n";
	runStep2();

	std::cout << "\n--- Running Step 3 Tests ---\n";
	runStep3();

	std::cout << "\n--- Running Step 4 Tests ---\n";
	runStep4();

	// wait for user input before closing (useful for some IDEs)
	std::cout << "\nPress Enter to exit...";
	std::cin.get();
	return 0;
}
