#pragma once
#include <string>

// Gentlemen's Rule: Intent-revealing data structure for the Symbol Table
struct TVariable {
	std::string Name;
	double Value;

	TVariable(const std::string& aName, double aValue)
		: Name(aName), Value(aValue) {}
};