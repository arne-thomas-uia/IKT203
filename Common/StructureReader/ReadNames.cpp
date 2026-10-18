#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../StructureReader.h"
#include "GetRecordCount.h"
#include "../DebugUtils.h"


void ReadNamesFromFile(FNameRead aOnNameRead)
{
	if (NameFilePath.empty()) {
		debugText(Debug::ELevel::Error, "ReadNames: Filename is empty!");
		return;
	}

	std::ifstream file(NameFilePath);
	if (!file.is_open())
	{
		debugText(Debug::ELevel::Error, "ReadNames: Could not open file '{}'", NameFilePath);
		return;
	}

	std::string line;
	int totalCount = 0;
	int currentIndex = 0;
	bool keepReading = true;
	int fileLineNumber = 0;
	int skippedLines = 0;

	// --- 1. Read the header line ---
	if (std::getline(file, line))
	{
		++fileLineNumber;
		// Use our shared helper to get the count
		totalCount = GetRecordCount(line);
	}

	auto isOnlyWhitespace = [](const std::string& s) {
		return s.find_first_not_of(" \t\r\n") == std::string::npos;
		};

	// --- 2. Loop through the rest of the file ---
	while (keepReading && std::getline(file, line))
	{
		++fileLineNumber;

		// Skip empty or whitespace-only lines
		if (line.empty() || isOnlyWhitespace(line))
		{
			++skippedLines;
			// log the skipped line number for debugging
			debugText(Debug::ELevel::Verbose, "ReadNames: Skipping empty/whitespace line {}", fileLineNumber);
			continue;
		}

		std::istringstream nameStream(line);
		std::string firstName, lastName;

		// Parse "FirstName LastName"
		if (nameStream >> firstName >> lastName)
		{
			if (aOnNameRead)
			{
				// Call the callback with all parameters
				if (!aOnNameRead(currentIndex, totalCount, firstName, lastName))
				{
					keepReading = false;
				}
				currentIndex++;
			}
		}
		else
		{
			// Log malformed line so you can inspect the input
			debugText(Debug::ELevel::Warning, "ReadNames: Could not parse name on line {}: '{}'", fileLineNumber, line);
			++skippedLines;
		}
	}

	file.close();

	if (skippedLines > 0)
	{
		debugText(Debug::ELevel::Information, "ReadNames: Skipped {} malformed/empty line(s) while reading names.", skippedLines);
	}

	// Error checking, if keepReading == true, and currentIndex < totalCount, we may have read fewer names than expected.
	if (keepReading && currentIndex < totalCount)
	{
		debugText(Debug::ELevel::Warning, "ReadNames: Expected to read {} names, but only read {}.", totalCount, currentIndex);
	}

	debugText(Debug::ELevel::Information, "ReadNames: Successfully finished parsing names.");
}