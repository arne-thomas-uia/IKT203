#pragma once
#include "TCollection.hpp"
#include "TLinkedList.hpp"
#include "DebugUtils.h"
#include <stdexcept>

template <typename T>
class TStack : public TCollection<T> {
private:
	TLinkedList<T> list; // The internal engine doing the heavy lifting

public:
	TStack() = default;
	~TStack() override = default;

	// --- TCollection Implementation ---
	int GetSize() const override;
	bool IsEmpty() const override;
	void Clear() override;

	// --- Stack Specific Rules (LIFO) ---
	void Push(T item);
	T Pop();
	T Peek() const;
};

template <typename T>
int TStack<T>::GetSize() const {
	return list.GetSize();
}

template <typename T>
bool TStack<T>::IsEmpty() const {
	return list.IsEmpty();
}

template <typename T>
void TStack<T>::Clear() {
	list.Clear();
}

template <typename T>
void TStack<T>::Push(T item) {
	list.Prepend(item);
}

template <typename T>
T TStack<T>::Peek() const {
	if (IsEmpty()) {
		Debug::TLogger::Log(Debug::ELevel::Warning, "Peek called on an empty stack");
		return nullptr; // This structure always uses pointers, so nullptr is a safe return value for empty stack
	}
	return list.GetAtIndex(0);
}

template <typename T>
T TStack<T>::Pop() {
	if (IsEmpty()) {
		Debug::TLogger::Log(Debug::ELevel::Warning, "Pop called on an empty stack");
		return nullptr; // This structure always uses pointers, so nullptr is a safe return value for empty stack
	}
	T item = list.GetAtIndex(0);
	list.Remove(item);
	return item;
}