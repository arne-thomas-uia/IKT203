#pragma once
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string
#include <functional> // For std::function
#include "../DebugUtils.h"
#include "TCollection.hpp"
#include "TNode.hpp"


// Type defined function for data factory
template <typename T, typename TArgs>
using FDataFactory = std::function<T(TArgs)>;

// Type defined functions for TLinkedList
template <typename T>
using FCheckNode = std::function<bool(const std::remove_pointer_t<T>*, const std::remove_pointer_t<T>*)>;

template <typename T>
using FVisitNode = std::function<void(const std::remove_pointer_t<T>*, int)>;

template <typename T>
using FSearchNode = std::function<bool(const std::remove_pointer_t<T>*, const void*)>;

/// <summary>
/// A template-based doubly-linked list collection that stores elements of type T with support for custom data factories, searching, and iteration.
/// </summary>
/// <typeparam name="T">The type of elements stored in the linked list.</typeparam>

template <typename T>
class TLinkedList : public TCollection<T> {
private:
	TNode<T>* head; // Pointer to the dummy head node
	TNode<T>* tail; // Pointer to the tail node
	int size;       // Current size of the list
	bool isDataOwner; // Indicates if the list owns the data and should delete it

	void InternAppend(T);
	void InternPrepend(T);

	T SafeGet(int aIndex) const;

public:
	// Constructor
	TLinkedList();
	TLinkedList(bool);
	// Destructor
	~TLinkedList();

	// Core Linked List Operations
	template <typename TArgs>
	T Append(FDataFactory<T, TArgs>, TArgs);

	template <typename TObject, typename TArgs>
	T Append(TObject* instance, T(TObject::* method)(TArgs), TArgs aArgs);

	void Append(T);

	template <typename TArgs>
	T Prepend(FDataFactory<T, TArgs>, TArgs);

	template <typename TObject, typename TArgs>
	T Prepend(TObject* instance, T(TObject::* method)(TArgs), TArgs aArgs);

	void Prepend(T aValue);

	T GetAtIndex(int aIndex) const;
	void Remove(const T aValue);

	// Finding and Checking Operations
	bool Contains(const T aValue) const;
	T Search(const T aValue, FCheckNode<T> checkNode = nullptr) const;

	template <typename TObject>
	T Search(const T aValue, TObject* instance, bool (TObject::* method)(const std::remove_pointer_t<T>*, const std::remove_pointer_t<T>*)) const;

	// Loop Operations
	void ForEach(FVisitNode<T> aVisitNode) const;

	template <typename TObject>
	void ForEach(TObject* instance, void (TObject::* method)(const std::remove_pointer_t<T>*, int)) const;

	T SearchFor(FSearchNode<T> aSearchNode, const void* aCriteria) const;

	template <typename TObject>
	T SearchFor(TObject* instance, bool (TObject::* method)(const std::remove_pointer_t<T>*, const void*), const void* aCriteria) const;

	void FindAll(FSearchNode<T> aSearchNode, const void* aCriteria, TLinkedList<T>& aOutList) const;

	template <typename TObject>
	void FindAll(TObject* instance, bool (TObject::* method)(const std::remove_pointer_t<T>*, const void*), const void* aCriteria, TLinkedList<T>& aOutList) const;

	// Helper Functions
	int GetSize() const;
	bool IsEmpty() const;
	void Clear();
	void Reverse();
};

// --- Method Implementations ---

// Default Constructor: Initializes the dummy head node and tail, assumes not owning data by default

template <typename T>
TLinkedList<T>::TLinkedList() : TLinkedList(false) {} // Delegate to the main constructor with false for data ownership

// Constructor: Initializes the dummy head node and tail
template <typename T>
TLinkedList<T>::TLinkedList(bool aIsDataOwner) : size(0) {
	static_assert(std::is_pointer<T>::value, "TLinkedList: T must be a pointer type.");
	isDataOwner = aIsDataOwner;
	head = new TNode<T>(nullptr); // Create a dummy head node
	tail = head;                 // Initially, tail is the same as head
}

// Destructor: Deletes all nodes in the list
template <typename T>
TLinkedList<T>::~TLinkedList() {
	TNode<T>* current = head;
	while (current != nullptr) {
		TNode<T>* nextNode = current->GetNext();
		if (isDataOwner && current->GetData() != nullptr) {
			delete current->GetData(); // Delete the data if the list owns it
		}
		delete current; // Free the current node
		current = nextNode; // Move to the next node
	}
}

template <typename T>
void TLinkedList<T>::InternAppend(T aData) {
	TNode<T>* newNode = new TNode<T>(aData);
	newNode->SetPrev(tail); // Set the prev pointer of the new node
	tail->SetNext(newNode); // Update the next pointer of the current tail
	tail = newNode;         // Update the tail to the new node
	size++;
}

template <typename T>
void TLinkedList<T>::InternPrepend(T aData) {
	TNode<T>* newNode = new TNode<T>(aData);
	newNode->SetNext(head->GetNext());
	if (head->GetNext() != nullptr) {
		head->GetNext()->SetPrev(newNode); // Update the prev pointer of the first node
	}
	head->SetNext(newNode);
	newNode->SetPrev(head); // Set the prev pointer of the new node
	if (tail == head) {
		tail = newNode;
	}
	size++;
}

template <typename T>
T TLinkedList<T>::SafeGet(int aIndex) const {
	if (aIndex < 0 || aIndex >= size) {
		debugText(Debug::ELevel::Error, "TLinkedList::SafeGet - Index {} is out of bounds (Size: {})", aIndex, size);
		return nullptr; // Return nullptr if index is out of bounds
	}
	TNode<T>* current;
	if (aIndex < size / 2) {
		current = head->GetNext();
		for (int i = 0; i < aIndex; i++) {
			current = current->GetNext();
		}
	}
	else {
		current = tail;
		for (int i = size - 1; i > aIndex; i--) {
			current = current->GetPrev();
		}
	}
	return current->GetData(); // Return the data directly
}


// Append(): Adds a new node with the given factory and returns the new node's data
template <typename T>
template <typename TArgs>
T TLinkedList<T>::Append(FDataFactory<T, TArgs> aDataFactory, TArgs aArgs) {
	if (aDataFactory == nullptr) {
		return nullptr; // Return nullptr if no factory is provided
	}
	T newData = aDataFactory(aArgs);
	InternAppend(newData);
	return newData;
}

template <typename T>
void TLinkedList<T>::Append(T aValue) {
	InternAppend(aValue);
}

// Prepend(): Adds a new node with the given factory to the beginning of the list
template <typename T>
template <typename TArgs>
T TLinkedList<T>::Prepend(FDataFactory<T, TArgs> aDataFactory, TArgs aArgs) {
	if (aDataFactory == nullptr) {
		return nullptr; // Return nullptr if no factory is provided
	}
	T newData = aDataFactory(aArgs);
	InternPrepend(newData);
	return newData;
}

template <typename T>
void TLinkedList<T>::Prepend(T aValue) {
	InternPrepend(aValue);
}

// GetAtIndex(): Returns the value at the specified index
template <typename T>
T TLinkedList<T>::GetAtIndex(int aIndex) const {
	return SafeGet(aIndex);
}

// Remove(): Removes the first node with the given value
template <typename T>
void TLinkedList<T>::Remove(const T aValue) {
	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		if (current->GetData() == aValue) {
			TNode<T>* nodeToDelete = current;
			if (current->GetPrev() != nullptr) {
				current->GetPrev()->SetNext(current->GetNext());
			}
			if (current->GetNext() != nullptr) {
				current->GetNext()->SetPrev(current->GetPrev());
			}
			if (nodeToDelete == tail) {
				tail = current->GetPrev();
			}

			if (isDataOwner && nodeToDelete->GetData() != nullptr) {
				delete nodeToDelete->GetData(); // Delete the data if the list owns it
			}
			delete nodeToDelete;
			size--;
			return;
		}
		current = current->GetNext();
	}
}

// Contains(): Checks if the list contains the given value
template <typename T>
bool TLinkedList<T>::Contains(const T aValue) const {
	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		if (current->GetData() == aValue) {
			return true;
		}
		current = current->GetNext();
	}
	return false;
}

// Search(): Searches for a node with the given value using an optional check function
template <typename T>
T TLinkedList<T>::Search(const T aValue, FCheckNode<T> checkNode) const {
	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		if (checkNode == nullptr) {
			if (current->GetData() == aValue) {
				return current->GetData();
			}
		}
		else {
			if (checkNode(current->GetData(), aValue)) {
				return current->GetData();
			}
		}
		current = current->GetNext();
	}
	return nullptr;
}

// ForEach(): Applies a function to each node in the list
template <typename T>
void TLinkedList<T>::ForEach(FVisitNode<T> aVisitNode) const {
	if (aVisitNode == nullptr) {
		return; // Return if no visit function is provided
	}
	TNode<T>* current = head->GetNext();
	int index = 0;
	while (current != nullptr) {
		aVisitNode(static_cast<const T>(current->GetData()), index); // Cast to const T
		current = current->GetNext();
		index++;
	}
}

// Reverse(): Reverses the order of the nodes in the list
template <typename T>
void TLinkedList<T>::Reverse() {
	TNode<T>* current = head->GetNext();
	TNode<T>* temp = nullptr; // Temporary pointer for swapping

	// Handle an empty or single-node list
	if (current == nullptr || current->GetNext() == nullptr) {
		return;
	}

	// Iterate and swap next and prev pointers
	while (current != nullptr) {
		temp = current->GetPrev(); // Store prev pointer
		current->SetPrev(current->GetNext()); // Set prev to next
		current->SetNext(temp); // Set next to temp (which holds original prev)
		current = current->GetPrev(); // Move to the original next node
	}

	// Update head and tail
	TNode<T>* newHeadNext = tail; // The original tail becomes the new head->next
	tail = head->GetNext(); // The original first node is now the tail
	tail->SetNext(nullptr); // Ensure the new tail has no next pointer
	head->SetNext(newHeadNext); // Set the head's next to the new first node
}

template <typename T>
int TLinkedList<T>::GetSize() const {
	return size;
}

template <typename T>
bool TLinkedList<T>::IsEmpty() const {
	return size == 0;
}

template <typename T>
T TLinkedList<T>::SearchFor(FSearchNode<T> aSearchNode, const void* aCriteria) const {
	if (aSearchNode == nullptr) {
		return nullptr; // Safety check
	}

	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		// We pass the raw data and the generic criteria pointer
		if (aSearchNode(current->GetData(), aCriteria)) {
			return current->GetData(); // Return the first match we find
		}
		current = current->GetNext();
	}

	return nullptr; // No match found
}

template <typename T>
void TLinkedList<T>::FindAll(FSearchNode<T> aSearchNode, const void* aCriteria, TLinkedList<T>& aOutList) const {
	if (aSearchNode == nullptr) {
		return;
	}

	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		// If the search criteria matches, append the data to our output list
		if (aSearchNode(current->GetData(), aCriteria)) {
			aOutList.Append(current->GetData());
		}
		current = current->GetNext();
	}
}

template <typename T>
void TLinkedList<T>::Clear() {
	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		TNode<T>* nextNode = current->GetNext();
		if (isDataOwner && current->GetData() != nullptr) {
			delete current->GetData(); // Delete the data if the list owns it
		}
		delete current; // Free the current node
		current = nextNode; // Move to the next node
	}
	head->SetNext(nullptr); // Reset the list to empty state
	tail = head; // Reset tail to dummy head
	size = 0; // Reset size
}

template <typename T>
template <typename TObject, typename TArgs>
T TLinkedList<T>::Append(TObject* instance, T(TObject::* method)(TArgs), TArgs aArgs) {
	return Append<TArgs>([instance, method](TArgs args) {
		return (instance->*method)(args);
		}, aArgs);
}

template <typename T>
template <typename TObject, typename TArgs>
T TLinkedList<T>::Prepend(TObject* instance, T(TObject::* method)(TArgs), TArgs aArgs) {
	return Prepend<TArgs>([instance, method](TArgs args) {
		return (instance->*method)(args);
		}, aArgs);
}

template <typename T>
template <typename TObject>
T TLinkedList<T>::Search(const T aValue, TObject* instance, bool (TObject::* method)(const std::remove_pointer_t<T>*, const std::remove_pointer_t<T>*)) const {
	return Search(aValue, [instance, method](const std::remove_pointer_t<T>* node1, const std::remove_pointer_t<T>* node2) {
		return (instance->*method)(node1, node2);
		});
}

template <typename T>
template <typename TObject>
void TLinkedList<T>::ForEach(TObject* instance, void (TObject::* method)(const std::remove_pointer_t<T>*, int)) const {
	ForEach([instance, method](const std::remove_pointer_t<T>* node, int index) {
		(instance->*method)(node, index);
		});
}

template <typename T>
template <typename TObject>
T TLinkedList<T>::SearchFor(TObject* instance, bool (TObject::* method)(const std::remove_pointer_t<T>*, const void*), const void* aCriteria) const {
	return SearchFor([instance, method](const std::remove_pointer_t<T>* node, const void* criteria) {
		return (instance->*method)(node, criteria);
		}, aCriteria);
}

template <typename T>
template <typename TObject>
void TLinkedList<T>::FindAll(TObject* instance, bool (TObject::* method)(const std::remove_pointer_t<T>*, const void*), const void* aCriteria, TLinkedList<T>& aOutList) const {
	FindAll([instance, method](const std::remove_pointer_t<T>* node, const void* criteria) {
		return (instance->*method)(node, criteria);
		}, aCriteria, aOutList);
}

// --- End of TLinkedList class ---