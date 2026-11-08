// Category 3: Trees
#include "DataStructure.h"
#include "DebugUtils.h"
#include "StructureReader.h"
#include "TEmployee.h"
#include <format>
#include <iostream>
#include <string>

// Pointer to the RBT, populated via file reader callback
static TRBTree<TEmployee*>* employeeTree = nullptr;

template<typename... Args>
void PrintMessage(const std::string& aMessage, const Args&... aArgs) {
	std::cout << std::vformat(aMessage, std::make_format_args(aArgs...)) << std::endl;
}

// Callback for loading 200 employees
static bool EmployeeReadCallback(const int aIndex, const int /*aTotalCount*/, const std::string& aFirstName, const std::string& aLastName) {
	if (aIndex >= 200) return false; // Only load the first 200 per the assignment

	// Generate a mock unique ID (using index to ensure uniqueness for the test)
	int employeeID = 1000 + aIndex;
	TEmployee* newEmployee = new TEmployee(aFirstName, aLastName, employeeID);

	employeeTree->Insert(newEmployee);
	return true;
}

int RunApp(int /*argc*/, char* /*argv*/[]) {
	Debug::TLogger::SetLevel(Debug::ELevel::Verbose);
	PrintMessage("--- Initializing Category 3: Red-Black Tree ---");

	// Define the strict comparison logic (by EmployeeID)
	auto compareEmployees = [](const TEmployee* a, const TEmployee* b) -> int {
		return a->EmployeeID - b->EmployeeID;
		};

	// Instantiate the RBT as the absolute memory owner (true)
	employeeTree = new TRBTree<TEmployee*>(compareEmployees, true);

	Debug::LogInfo("Loading top 200 employees into the Red-Black Tree...");
	ReadNamesFromFile(EmployeeReadCallback);

	PrintMessage("\n=== In-Order Traversal (Should be sorted by EmployeeID) ===");
	employeeTree->TraverseInOrder([](TEmployee* emp) {
		emp->PrintOut();
		});

	PrintMessage("\n=== Searching for EmployeeID: 1042 ===");

	// Create a dummy employee strictly for searching (ID is the only thing that matters to the comparator)
	TEmployee searchTarget("", "", 1042);
	TEmployee* foundEmployee = employeeTree->Search(&searchTarget);

	if (foundEmployee != nullptr) {
		PrintMessage("FOUND: ");
		foundEmployee->PrintOut();
	}
	else {
		PrintMessage("Employee 1042 not found.");
	}

	employeeTree->Delete(foundEmployee);
	foundEmployee = nullptr; // Try to find it again after deletion
	foundEmployee = employeeTree->Search(&searchTarget);
	if (foundEmployee == nullptr) {
		PrintMessage("Employee 1042 successfully deleted and no longer found in the tree.");
	}
	else {
		PrintMessage("Error: Employee 1042 still found after deletion.");
	}

	// The RBT automatically deletes all 200 TEmployee objects upon destruction
	delete employeeTree;

	PrintMessage("\n=== Tree Operations Complete ===");
	return 0;
}