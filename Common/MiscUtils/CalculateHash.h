#pragma once
#include <string>

// A standard Polynomial Rolling Hash function for strings
// Maps any string to a bucket index from 0 to (bucket_count - 1)
unsigned int CalculateHash(const std::string&);