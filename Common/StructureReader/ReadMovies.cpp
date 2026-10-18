#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../StructureReader.h"
#include "GetRecordCount.h"
#include "../DebugUtils.h"

void ReadMoviesFromFile(FMovieRead aOnMovieRead)
{
	if (MovieFilePath.empty()) {
		debugText(Debug::ELevel::Error, "ReadMovies: Filename is empty!");
		return;
	}
	std::ifstream file(MovieFilePath);
	if (!file.is_open())
	{
		debugText(Debug::ELevel::Error, "ReadMovies: Could not open file '{}'", MovieFilePath);
		return;
	}
	std::string line;
	int totalCount = 0;
	int currentIndex = 0;
	bool keepReading = true;
	// --- 1. Read the header line ---
	if (std::getline(file, line))
	{
		// Use our shared helper to get the count
		totalCount = GetRecordCount(line);
	}
	// --- 2. Loop through the rest of the file (the data lines) ---
	while (keepReading && std::getline(file, line))
	{
		if (line.empty()) continue; // Skip empty lines
		std::istringstream lineStream(line);
		std::string title, director, genre;
		unsigned short year;
		float rating;
		// Parse the five semicolon-separated fields
		// Title;Director;Year;Genre;Rating
		if (std::getline(lineStream, title, ';') &&
			std::getline(lineStream, director, ';') &&
			lineStream >> year && lineStream.ignore(1) && // Read year and skip semicolon
			std::getline(lineStream, genre, ';') &&
			lineStream >> rating) // Last one reads to end of line
		{
			if (aOnMovieRead)
			{
				// Call the callback with all parameters
				if (!aOnMovieRead(currentIndex, totalCount, title, director, year, genre, rating))
				{
					keepReading = false;
				}
				currentIndex++;
			}
		}
	}
	file.close();
	debugText(Debug::ELevel::Information, "ReadMovies: Successfully finished parsing movies.");
}