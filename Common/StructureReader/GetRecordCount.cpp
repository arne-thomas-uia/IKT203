#include "GetRecordCount.h"

int GetRecordCount(const std::string& aHeaderLine)
{
	size_t recordPos = aHeaderLine.find("records:=");
	if (recordPos == std::string::npos)
	{
		return -1; // No record count found
	}

	size_t countStart = recordPos + 9; // Length of "records:="

	// Find the end bracket ']' or a potential semicolon ';'
	size_t countEnd = aHeaderLine.find_first_of("];", countStart);
	if (countEnd == std::string::npos)
	{
		return -1; // Malformed header
	}

	std::string countStr = aHeaderLine.substr(countStart, countEnd - countStart);
	try
	{
		// stoi = string to integer
		return std::stoi(countStr);
	}
	catch (const std::exception&)
	{
		return -1; // Malformed number
	}
}