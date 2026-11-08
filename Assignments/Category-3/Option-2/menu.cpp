#include "menu.h"
#include <iostream>
#include <limits>
#include <cstdlib>

void TMenu::PrintHeader(const std::string& aTitle) {
	std::cout << "\n======================================================\n";
	std::cout << "  " << aTitle << "\n";
	std::cout << "======================================================\n";
}

void TMenu::PrintMessage(const std::string& aMessage) {
	std::cout << " " << aMessage << std::endl;
}

void TMenu::PrintSuccess(const std::string& aMessage) {
	std::cout << " [SUCCESS] " << aMessage << std::endl;
}

void TMenu::PrintError(const std::string& aErrorMessage) {
	std::cout << " [ERROR] " << aErrorMessage << std::endl;
}

std::string TMenu::ReadString(const std::string& aPrompt) {
	std::string userInput;
	std::cout << " " << aPrompt << " ";
	std::getline(std::cin, userInput);
	return userInput;
}

double TMenu::ReadDouble(const std::string& aPrompt) {
	double userInput = 0.0;
	while (true) {
		std::cout << " " << aPrompt << " ";
		if (std::cin >> userInput) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return userInput;
		}

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		PrintError("Invalid input. Please enter a valid decimal number.");
	}
}

void TMenu::PrintCalculatorInstructions() {
	std::cout << "\n --- Interpreted Calculator Commands (Pedagogical Mode) ---\n";
	std::cout << "  1. Evaluate Math: Type 'calc [expression]' or '[expression]' (e.g., '(x + 5) * 3')\n";
	std::cout << "  2. Set Variable:  Type 'set [name] = [value]' or '[name] = [value]' (e.g., 'set x = 10')\n";
	std::cout << "  3. List Vars:     Type 'list' (Displays all variables stored in the AVL Symbol Table)\n";
	std::cout << "  4. Exit:          Type 'exit' or 'quit'\n";
	std::cout << " ------------------------------------------------------------\n";
}

void TMenu::ClearScreen() {
#if defined(_WIN32) || defined(_WIN64)
	std::system("cls");
#else
	std::system("clear");
#endif
}

void TMenu::WaitForAnyKey(const std::string& aPrompt) {
	std::cout << "\n " << aPrompt << " ";
	std::cin.get();
}