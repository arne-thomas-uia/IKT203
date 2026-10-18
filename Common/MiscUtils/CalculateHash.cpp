#include <iostream>
#include <string>

// A standard Polynomial Rolling Hash function for strings
// Maps any string to a bucket index from 0 to (bucket_count - 1)
unsigned int CalculateHash(const std::string& aKey) {
  const unsigned int PRIME = 31;
  unsigned int hashValue = 0;

  for (char c : aKey) {
    hashValue = hashValue * PRIME + c;
  }

  return hashValue;
}