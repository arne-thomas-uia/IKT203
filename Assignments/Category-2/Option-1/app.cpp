// Option 1 (Standard): Cruise Ship Manifest

#include "StructureReader.h"
#include "TPerson.h"
#include <cstdlib> // For rand()
#include "DataStructure.h"
#include "DebugUtils.h"
#include <format>
#include <iostream>
#include <string>

// Global lists strictly for the callback injection
static TLinkedList<TPerson*>* guestList = nullptr;
static TLinkedList<TPerson*>* employeeList = nullptr;

// Formats console output explicitly
template<typename... Args>
void PrintMessage(const std::string& aMessage, const Args&... aArgs) {
	std::cout << std::vformat(aMessage, std::make_format_args(aArgs...)) << std::endl;
}

// Data loading callback
static bool PersonReadCallback(const int aIndex, const int /*aTotalCount*/, const std::string& aFirstName, const std::string& aLastName) {
	// Random cabin size between 1 and 4
	int cabinSize = (std::rand() % 4) + 1;

	EPersonStatusType status = (aIndex < 1500) ? EPersonStatusType::Employee : EPersonStatusType::Guest;
	TPerson* newPerson = new TPerson(aFirstName, aLastName, status, cabinSize);

	if (status == EPersonStatusType::Employee) {
		employeeList->Append(newPerson);
	}
	else {
		guestList->Append(newPerson);
	}
	return true; // Continue reading
}

// Utility to extract TLinkedList to a bare-metal array for processing
static TPerson** ConvertListToArray(TLinkedList<TPerson*>* aList, int& aOutSize) {
	aOutSize = aList->GetSize();
	TPerson** arr = new TPerson * [aOutSize];
	for (int i = 0; i < aOutSize; i++) {
		arr[i] = aList->GetAtIndex(i);
	}
	return arr;
}

int RunApp(int /*argc*/, char* /*argv*/[]) {
	Debug::TLogger::SetLevel(Debug::ELevel::Information);
	std::srand(12345); // Seed for reproducible cabin sizes

	PrintMessage("--- Initializing Cruise Ship Manifest ---");

	// Instantiate lists as data owners (true) to prevent memory leaks
	guestList = new TLinkedList<TPerson*>(true);
	employeeList = new TLinkedList<TPerson*>(true);

	Debug::LogInfo("Loading personnel from file...");
	ReadNamesFromFile(PersonReadCallback);
	PrintMessage("Loaded {} Employees and {} Guests.", employeeList->GetSize(), guestList->GetSize());

	// =========================================================================================
	// TASK 1: MERGE SORT (Alphabetical Manifests)
	// =========================================================================================
	PrintMessage("\n--- TASK 1: Merge Sorting Manifests Alphabetically ---");

	// Alphabetical Comparator (Last Name, then First Name)
	auto alphaCompare = [](const TPerson* a, const TPerson* b) -> int {
		if (a->LastName != b->LastName) {
			return a->LastName.compare(b->LastName);
		}
		return a->FirstName.compare(b->FirstName);
		};

	// Turn on verbose logging for the sort
	Debug::TLogger::SetLevel(Debug::ELevel::Verbose);


	// Sort Guests
	int guestCount = 0;
	TPerson** guestArrayAlpha = ConvertListToArray(guestList, guestCount);
	TAlgorithm<TPerson*>::MergeSort(guestArrayAlpha, guestCount, alphaCompare);

	// Sort Employees
	int employeeCount = 0;
	TPerson** employeeArrayAlpha = ConvertListToArray(employeeList, employeeCount);
	TAlgorithm<TPerson*>::MergeSort(employeeArrayAlpha, employeeCount, alphaCompare);

	PrintMessage("First 3 Sorted Guests:");
	for (int i = 0; i < 3; i++) guestArrayAlpha[i]->PrintOut();

	//Turn off verbose logging for the next tasks
	Debug::TLogger::SetLevel(Debug::ELevel::Information);

	// =========================================================================================
	// TASK 2: QUICK SORT (Cabin Grouping)
	// =========================================================================================
	PrintMessage("\n--- TASK 2: Quick Sorting Guests by Cabin Size ---");

	// We copy the pointers into a new array to preserve the Alphabetical array for Task 3
	TPerson** guestArrayCabin = new TPerson * [guestCount];
	for (int i = 0; i < guestCount; i++) guestArrayCabin[i] = guestArrayAlpha[i];

	// Cabin Comparator (1. CabinSize Ascending, 2. Last Name Alphabetical)
	auto cabinCompare = [](const TPerson* a, const TPerson* b) -> int {
		if (a->CabinSize != b->CabinSize) {
			return a->CabinSize - b->CabinSize;
		}
		return a->LastName.compare(b->LastName);
		};
	TAlgorithm<TPerson*>::QuickSort(guestArrayCabin, guestCount, cabinCompare);

	PrintMessage("First 5 Guests Assigned to Cabins:");
	for (int i = 0; i < 5; i++) guestArrayCabin[i]->PrintOut();

	// =========================================================================================
	// TASK 3: BINARY SEARCH (Passenger Lookup)
	// =========================================================================================
	PrintMessage("\n--- TASK 3: Binary Search for Passenger ---");

	// We search the Alphabetical array we preserved from Task 1
	std::string searchLast = "Wcuna"; // Example target
	std::string searchFirst = "Aaron";

	// Turn on verbose logging for the search
	Debug::TLogger::SetLevel(Debug::ELevel::Verbose);

	// Dummy target to pass into BinarySearch
	TPerson targetSearch("", searchLast, EPersonStatusType::Guest, 0);

	// Note: We adjust the lambda slightly to strictly search by LastName for the demo,
	// or create a strict First/Last comparator matching our target struct.
	auto searchCompare = [](const TPerson* a, const TPerson* b) -> int {
		return a->LastName.compare(b->LastName);
		};

	int foundIndex = TAlgorithm<TPerson*>::BinarySearch(guestArrayAlpha, guestCount, searchCompare, &targetSearch);

	if (foundIndex != -1) {
		PrintMessage("Passenger Found at alphabetical index {}:", foundIndex);
		guestArrayAlpha[foundIndex]->PrintOut();
	}
	else {
		PrintMessage("Passenger '{}' not found.", searchLast);
	}

	// =========================================================================================
	// MEMORY CLEANUP
	// =========================================================================================
	// Delete the raw pointer arrays. 
	// Do NOT delete the actual TPerson objects inside them here, because `guestList` and 
	// `employeeList` are the true owners (`isDataOwner = true`) and will delete them.
	delete[] guestArrayAlpha;
	delete[] employeeArrayAlpha;
	delete[] guestArrayCabin;

	delete guestList;
	delete employeeList;

	PrintMessage("\n=== Cruise Ship Manifest Complete ===");
	return 0;
}