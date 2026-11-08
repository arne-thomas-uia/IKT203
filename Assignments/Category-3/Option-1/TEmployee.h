#pragma once
#include <string>

enum class EPersonStatusType {
	Employee,
	Guest
};

struct TEmployee {
	int EmployeeID;
	std::string FirstName;
	std::string LastName;

	TEmployee(std::string aFirstName, std::string aLastName, int aEmployeeID);

	void PrintOut() const;
};
