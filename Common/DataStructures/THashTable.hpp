#pragma once
#include <string>
#include <stdexcept>
#include <type_traits>
#include "TLinkedList.hpp"
#include "MiscUtils.h"

// A data structure to wrap the string key and the generic pointer value.
template <typename T>
struct TKeyValuePair {
  std::string Key;
  T Value;

  TKeyValuePair(std::string aKey, T aValue) : Key(aKey), Value(aValue) {}
};

template <typename T>
class THashTable : public TCollection<T> {
private:
  int _capacity;
  int _size;
  bool _isDataOwner; // Determines if the table owns the actual T pointers

  // An array of POINTERS to custom linked lists for collision chaining
  TLinkedList<TKeyValuePair<T>*>** _buckets;

  // Delegated Named Functions to replace lambdas[cite: 10]
  static bool EvaluateKeyMatch(const TKeyValuePair<T>* aPair, const void* aSearchKey);
  static void DeleteValue(const TKeyValuePair<T>* aPair, int aIndex);

public:
  THashTable(bool aIsDataOwner = false);
  ~THashTable() override;

  // --- TCollection Implementation ---
  int GetSize() const override;
  bool IsEmpty() const override;
  void Clear() override;

  // --- Hash Table Operations ---
  void Insert(std::string aKey, T aValue);
  T Search(std::string aKey) const;
  void Remove(std::string aKey);
};

// --- Implementations ---

template <typename T>
bool THashTable<T>::EvaluateKeyMatch(const TKeyValuePair<T>* aPair, const void* aSearchKey) {
  const std::string* targetKey = static_cast<const std::string*>(aSearchKey);
  return aPair->Key == *targetKey;
}

template <typename T>
void THashTable<T>::DeleteValue(const TKeyValuePair<T>* aPair, int /*aIndex*/) {
  delete aPair->Value;
}

template <typename T>
THashTable<T>::THashTable(bool aIsDataOwner)
  : _capacity(1009), _size(0), _isDataOwner(aIsDataOwner) {

  // Gentlemen's Rule: Enforce pointer types strictly at compile time.
  static_assert(std::is_pointer<T>::value, "THashTable: T must be a pointer type.");

  // Allocate an array of pointers
  _buckets = new TLinkedList<TKeyValuePair<T>*>*[_capacity];
  for (int i = 0; i < _capacity; i++) {
    // Instantiate each list on the heap to avoid shallow-copy destruction
    _buckets[i] = new TLinkedList<TKeyValuePair<T>*>(true);
  }
}

template <typename T>
THashTable<T>::~THashTable() {
  Clear();
  // Delete the individual list objects
  for (int i = 0; i < _capacity; i++) {
    delete _buckets[i];
  }
  // Delete the array container itself
  delete[] _buckets;
}

template <typename T>
int THashTable<T>::GetSize() const {
  return _size;
}

template <typename T>
bool THashTable<T>::IsEmpty() const {
  return _size == 0;
}

template <typename T>
void THashTable<T>::Clear() {
  for (int i = 0; i < _capacity; i++) {
    if (_isDataOwner) {
      _buckets[i]->ForEach(DeleteValue);
    }
    _buckets[i]->Clear();
  }
  _size = 0;
}

template <typename T>
void THashTable<T>::Insert(std::string aKey, T aValue) {
  unsigned int hash = CalculateHash(aKey);
  int bucketIndex = hash % _capacity;

  TKeyValuePair<T>* existingPair = _buckets[bucketIndex]->SearchFor(EvaluateKeyMatch, &aKey);

  if (existingPair != nullptr) {
    if (_isDataOwner) {
      delete existingPair->Value;
    }
    existingPair->Value = aValue;
  }
  else {
    TKeyValuePair<T>* newPair = new TKeyValuePair<T>(aKey, aValue);
    _buckets[bucketIndex]->Append(newPair);
    _size++;
  }
}

template <typename T>
T THashTable<T>::Search(std::string aKey) const {
  unsigned int hash = CalculateHash(aKey);
  int bucketIndex = hash % _capacity;

  TKeyValuePair<T>* foundPair = _buckets[bucketIndex]->SearchFor(EvaluateKeyMatch, &aKey);

  if (foundPair != nullptr) {
    return foundPair->Value;
  }

  return nullptr;
}

template <typename T>
void THashTable<T>::Remove(std::string aKey) {
  unsigned int hash = CalculateHash(aKey);
  int bucketIndex = hash % _capacity;

  TKeyValuePair<T>* foundPair = _buckets[bucketIndex]->SearchFor(EvaluateKeyMatch, &aKey);

  if (foundPair != nullptr) {
    if (_isDataOwner) {
      delete foundPair->Value;
    }
    _buckets[bucketIndex]->Remove(foundPair);
    _size--;
  }
}