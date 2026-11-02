#pragma once
#include <string>

enum class EPersonStatusType {
	Employee,
	Guest
};

struct TPerson {
	std::string FirstName;
	std::string LastName;
	EPersonStatusType Status;
	int CabinSize;

	TPerson(std::string aFirstName, std::string aLastName, EPersonStatusType aStatus, int aCabinSize);

	void PrintOut() const;
};
