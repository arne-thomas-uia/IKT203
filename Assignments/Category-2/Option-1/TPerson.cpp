#pragma once
#include "TPerson.h"
#include <format>
#include <iostream>
#include <string>

TPerson::TPerson(std::string aFirstName, std::string aLastName, EPersonStatusType aStatus, int aCabinSize)
	: FirstName(aFirstName), LastName(aLastName), Status(aStatus), CabinSize(aCabinSize) {}

void TPerson::PrintOut() const {
	std::string statusStr = (Status == EPersonStatusType::Employee) ? "Employee" : "Guest";
	std::cout << std::format("[{}] {} {}, Cabin Size: {}", statusStr, LastName, FirstName, CabinSize) << std::endl;
}
