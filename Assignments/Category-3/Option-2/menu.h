#pragma once
#include <string>

class TMenu {
public:
	// Gentlemen's Rule: Prevent instantiation of a purely static utility class
	TMenu() = delete;

	// --- Output Display ---
	static void PrintHeader(const std::string& aTitle);
	static void PrintMessage(const std::string& aMessage);
	static void PrintSuccess(const std::string& aMessage);
	static void PrintError(const std::string& aErrorMessage);
	static void PrintCalculatorInstructions();

	// --- Input Handling ---
	static std::string ReadString(const std::string& aPrompt);
	static double ReadDouble(const std::string& aPrompt);
	static void ClearScreen();
	static void WaitForAnyKey(const std::string& aPrompt);
};