// Submission-01.cpp : Defines the entry point for the application.
//

#include <iostream> 


// printNaturalNumbers
static void PrintNaturalNumbers(int aN)
{
	if (aN <= 0) return; // Base case: if n is less than or equal to 0, do nothing
	PrintNaturalNumbers(aN - 1); // Recursive call with n-1
	std::cout << aN << " "; // Print the number after the recursive call to achieve ascending order
}

// factorial function
static int CalculateFactorial(int aN)
{
	if (aN <= 1) return 1; // Base case: factorial of 0 or 1 is 1
	return aN * CalculateFactorial(aN - 1); // Recursive call
}

// power function using exponentiation by squaring
// This method is more efficient than the naive approach, reducing the time complexity from O(n) to O(log n).
static int Power(int aBase, int aExponent)
{
	if (aExponent == 0) return 1; // Base case: any number to the power of 0 is 1
	if (aExponent < 0) return 1 / Power(aBase, -aExponent); // Handle negative exponents
	if (aExponent % 2 == 0) // If exponent is even
	{
		int halfPower = Power(aBase, aExponent / 2);
		return halfPower * halfPower; // (x^(n/2))^2
	}
	else // If exponent is odd
	{
		return aBase * Power(aBase, aExponent - 1); // x * x^(n-1)
	}
}

// Fibonacci function
// Note: This naive recursive solution is inefficient because it recalculates the same Fibonacci numbers multiple times, leading to an exponential time complexity of O(2^n).
// An improvement could be made by using memoization or an iterative approach to store previously calculated values, reducing the time complexity to O(n).
static int Fibonacci(int aN)
{
	if (aN <= 0) return 0; // Base case: Fibonacci(0) = 0
	if (aN == 1) return 1; // Base case: Fibonacci(1) = 1
	int a = Fibonacci(aN - 1);
	int b = Fibonacci(aN - 2);
	std::cout << a << " + " << b << " = " << (a + b) << std::endl; // Print the sum of the two preceding numbers
	return a + b; // Recursive call
}

// Count occurrences of a character in a string
// This function counts how many times a specific character appears in a given string using recursion.
static int CountOccurrences(const char* aS, char aC)
{
	if (*aS == '\0') return 0; // Base case: end of string
	return (*aS == aC ? 1 : 0) + CountOccurrences(aS + 1, aC); // Check current character and recurse for the rest of the string
}

// Find the largest element in an array using binary recursion
// This function divides the array into two halves, finds the largest element in each half recursively, and then returns the larger of the two.
static int FindLargestElement(int arr[], int size)
{
	if (size == 1) return arr[0]; // Base case: only one element
	int mid = size / 2;
	int leftMax = FindLargestElement(arr, mid); // Find max in left half
	int rightMax = FindLargestElement(arr + mid, size - mid); // Find max in right half
	return (leftMax > rightMax) ? leftMax : rightMax; // Return the larger of the two
}

// Traverse and print characters in the ASCII table from aStart to aEnd using recursion
// This function prints characters in ascending order during the building phase of the recursion and in descending order during the unwinding phase.
static void TraverseAsciiTable(char aStart, char aEnd)
{
	if (aStart > aEnd) return; // Base case: if start exceeds end, do nothing
	std::cout << aStart << " "; // Print before the recursive call
	TraverseAsciiTable(aStart + 1, aEnd); // Recursive call with next character
	std::cout << aStart << " "; // Print after the recursive call
}

int main()
{
	std::cout << "--- Submission 2: Fundamental Recursion ---" << std::endl;
	std::cout << std::endl << "Part 1: Linear Recursion - Your First Steps" << std::endl;
	PrintNaturalNumbers(5);
	std::cout << std::endl << "------------------------------------------------" << std::endl;

	std::cout << std::endl << "Factorial of 5: " << CalculateFactorial(5);
	std::cout << std::endl << "------------------------------------------------" << std::endl;

	std::cout << std::endl << "Part 2: Multiple & Binary Recursion - Diving Deeper" << std::endl;
	std::cout << std::endl << "2^10: " << Power(2, 10);
	std::cout << std::endl << "------------------------------------------------" << std::endl;

	std::cout << std::endl << "4th Fibonacci number: " << std::endl << Fibonacci(4);
	std::cout << std::endl << "------------------------------------------------" << std::endl;

	std::cout << std::endl << "Occurrences of 'l' in 'Hello, World!': " << CountOccurrences("Hello, World!", 'l');
	std::cout << std::endl << "------------------------------------------------" << std::endl;

	int* arr = new int[20];
	// Fill array with random numbers from 0 to 999
	for (int i = 0; i < 20; ++i) arr[i] = rand() % 999;
	std::cout << std::endl << "Part 3: Advanced Binary Recursion" << std::endl;
	// Print first 20 elements of the array
	for (int i = 0; i < 20; ++i) std::cout << arr[i] << " ";

	std::cout << std::endl << "Largest element in array: " << FindLargestElement(arr, 20);
	std::cout << std::endl << "------------------------------------------------" << std::endl;
	
	std::cout << std::endl << "Traverse ASCII table from 'A' to 'Z':" << std::endl;
	TraverseAsciiTable('A', 'Z');

	/*
	Note: The output reflects the building and unwinding of the call stack.
	*/
	
	std::cout << std::endl;
	std::cout << std::endl << "---------- Press any key to exit -----------------------------" << std::endl;
	std::cin.get(); // Wait for user input before exiting
	return 0;
}
