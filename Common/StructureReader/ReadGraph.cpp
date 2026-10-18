#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../StructureReader.h"
#include "GetRecordCount.h"
#include "../DebugUtils.h"

// --- Enum for the parser's state ---
enum class EParseState
{
	NONE,
	NODES,
	EDGES
};

void ReadGraphFromFile(const std::string& aFilename, FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead)
{
	if (aFilename.empty()) {
		debugText(Debug::ELevel::Error, "ReadGraph: Filename is empty!");
		return;
	}

	std::ifstream file(aFilename);
	if (!file.is_open())
	{
		debugText(Debug::ELevel::Error, "ReadGraph: Could not open file '{}'", aFilename);
		return;
	}

	std::string line;
	EParseState currentState = EParseState::NONE;
	int totalCount = 0;
	int currentIndex = 0;
	bool keepReading = true;

	while (keepReading && std::getline(file, line))
	{
		if (line.empty()) continue;

		if (line[0] == '[')
		{
			// --- 2. USE THE SHARED FUNCTION ---
			totalCount = GetRecordCount(line);
			currentIndex = 0;

			if (line.find("[NODES") != std::string::npos)
			{
				currentState = EParseState::NODES;
				continue;
			}
			else if (line.find("[EDGES") != std::string::npos)
			{
				currentState = EParseState::EDGES;
				continue;
			}
			// If it's a comment or other header, reset state and count
			currentState = EParseState::NONE;
			totalCount = 0;
			continue;
		}

		// Process data based on the current state
		switch (currentState)
		{
		case EParseState::NODES:
			if (aOnNodeRead)
			{
				if (!aOnNodeRead(currentIndex, totalCount, line))
				{
					debugText(Debug::ELevel::Warning, "ReadGraph: Node reading halted by callback at index {}", currentIndex);
					keepReading = false;
				}
				currentIndex++;
			}
			break;

		case EParseState::EDGES:
		{
			std::istringstream edgeStream(line);
			std::string fromNode, toNode, weightStr;

			if (std::getline(edgeStream, fromNode, ';') &&
				std::getline(edgeStream, toNode, ';') &&
				std::getline(edgeStream, weightStr))
			{
				float weight = 0.0f;
				bool parsed = true;
				try
				{
					// Use std::stof (string to float) for weight
					weight = std::stof(weightStr);
				}
				catch (const std::exception&)
				{
					// Failed to parse float, skip this line
					parsed = false;
				}

				// Called outside the try block so callback exceptions are not swallowed
				if (parsed && aOnEdgeRead)
				{
					if (!aOnEdgeRead(currentIndex, totalCount, fromNode, toNode, weight))
					{
						debugText(Debug::ELevel::Warning, "ReadGraph: Edge reading halted by callback at index {}", currentIndex);
						keepReading = false;
					}
					currentIndex++;
				}
			}
			break;
		}
		case EParseState::NONE:
		default:
			break;
		}
	}
	file.close();
	if (keepReading)
	{
		debugText(Debug::ELevel::Information, "ReadGraph: Successfully finished parsing graph.");
	}
}