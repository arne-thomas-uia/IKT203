#pragma once
#include "TEmployee.h"
#include <format>
#include <iostream>
#include <string>

TEmployee::TEmployee(std::string aFirstName, std::string aLastName, int aEmployeeID)
	: FirstName(aFirstName), LastName(aLastName), EmployeeID(aEmployeeID) {}

void TEmployee::PrintOut() const {
	std::cout << std::format("Employee ID: {}, {} {}", EmployeeID, LastName, FirstName) << std::endl;
}
