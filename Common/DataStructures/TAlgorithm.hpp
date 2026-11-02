#pragma once
#include "TLinkedList.hpp"
#include <functional>

template <typename T>
using FCompare = std::function<int(const std::remove_pointer_t<T>*, const std::remove_pointer_t<T>*)>;

template <typename T>
class TAlgorithm {
private:
	// --- Merge Sort Helpers ---
	static void Merge(T* aArray, int aLeft, int aMid, int aRight, FCompare<T> aCompare) {
		int n1 = aMid - aLeft + 1;
		int n2 = aRight - aMid;

		Debug::LogVerbose("Merge: Combining sub-arrays [Left Size: {}] and [Right Size: {}]", n1, n2);

		T* leftArray = new T[n1];
		T* rightArray = new T[n2];

		for (int i = 0; i < n1; i++) leftArray[i] = aArray[aLeft + i];
		for (int j = 0; j < n2; j++) rightArray[j] = aArray[aMid + 1 + j];

		int i = 0, j = 0, k = aLeft;
		while (i < n1 && j < n2) {
			if (aCompare(leftArray[i], rightArray[j]) <= 0) {
				aArray[k] = leftArray[i];
				i++;
			}
			else {
				aArray[k] = rightArray[j];
				j++;
			}
			k++;
		}

		while (i < n1) aArray[k++] = leftArray[i++];
		while (j < n2) aArray[k++] = rightArray[j++];

		delete[] leftArray;
		delete[] rightArray;
	}

	static void MergeSortInternal(T* aArray, int aLeft, int aRight, FCompare<T> aCompare) {
		if (aLeft >= aRight) {
			Debug::LogVerbose("MergeSort: Base case reached with indices [Left: {}, Right: {}]", aLeft, aRight);
			return;
		}

		int mid = aLeft + (aRight - aLeft) / 2;

		Debug::LogVerbose("MergeSort: Dividing array at indices [Left: {}, Mid: {}, Right: {}]", aLeft, mid, aRight);
		MergeSortInternal(aArray, aLeft, mid, aCompare);

		Debug::LogVerbose("MergeSort: Left half sorted, now sorting right half [Mid+1: {}, Right: {}]", mid + 1, aRight);
		MergeSortInternal(aArray, mid + 1, aRight, aCompare);

		Debug::LogVerbose("MergeSort: Merging sorted halves [Left: {}, Mid: {}, Right: {}]", aLeft, mid, aRight);
		Merge(aArray, aLeft, mid, aRight, aCompare);
	}

	// --- Quick Sort Helpers ---
	static int Partition(T* aArray, int aLow, int aHigh, FCompare<T> aCompare) {
		T pivot = aArray[aHigh];
		int i = aLow - 1;

		Debug::LogVerbose("Partition: Pivot selected at index {}", aHigh);

		for (int j = aLow; j < aHigh; j++) {
			if (aCompare(aArray[j], pivot) <= 0) {
				i++;
				T temp = aArray[i];
				aArray[i] = aArray[j];
				aArray[j] = temp;
				Debug::LogVerbose("Partition: Swapped indices {} and {}", i, j);
			}
		}
		T temp = aArray[i + 1];
		aArray[i + 1] = aArray[aHigh];
		aArray[aHigh] = temp;
		return i + 1;
	}

	static void QuickSortInternal(T* aArray, int aLow, int aHigh, FCompare<T> aCompare) {
		if (aLow < aHigh) {
			int partitionIndex = Partition(aArray, aLow, aHigh, aCompare);
			QuickSortInternal(aArray, aLow, partitionIndex - 1, aCompare);
			QuickSortInternal(aArray, partitionIndex + 1, aHigh, aCompare);
		}
	}

public:
	// Static class, prevent instantiation
	TAlgorithm() = delete;

	// --- Public API ---

	// Merge Sort for Arrays
	static void MergeSort(T* aArray, int aSize, FCompare<T> aCompare) {
		if (aArray == nullptr || aSize <= 1) {
			Debug::LogWarning("MergeSort: Invalid array or empty size ({}) provided.", aSize);
			return;
		}
		Debug::LogInfo("MergeSort: Initiating sort on array of size {}.", aSize);
		MergeSortInternal(aArray, 0, aSize - 1, aCompare);
		Debug::LogInfo("MergeSort: Sorting completed successfully.");
	}

	// Quick Sort for Arrays
	static void QuickSort(T* aArray, int aSize, FCompare<T> aCompare) {
		if (aArray == nullptr || aSize <= 0) {
			Debug::LogWarning("QuickSort: Invalid array or empty size ({}) provided.", aSize);
			return;
		}
		Debug::LogInfo("QuickSort: Initiating sort on array of size {}.", aSize);
		QuickSortInternal(aArray, 0, aSize - 1, aCompare);
		Debug::LogInfo("QuickSort: Sorting completed successfully.");
	}

	// Binary Search for Arrays (Returns index, or -1 if not found)
	static int BinarySearch(T* aArray, int aSize, FCompare<T> aCompare, T aTarget) {
		if (aArray == nullptr || aSize <= 0) {
			Debug::LogError("BinarySearch: Invalid array or empty size ({}) provided.", aSize);
			return -1;
		}

		Debug::LogInfo("BinarySearch: Initiating search on array of size {}.", aSize);
		int left = 0;
		int right = aSize - 1;

		while (left <= right) {
			int mid = left + (right - left) / 2;
			Debug::LogVerbose("BinarySearch: Inspecting bounds [Left: {}, Right: {}] -> Midpoint: {}", left, right, mid);

			int cmpResult = aCompare(aArray[mid], aTarget);

			if (cmpResult == 0) {
				Debug::LogInfo("BinarySearch: Target successfully located at index {}.", mid);
				return mid;
			}
			if (cmpResult < 0) {
				left = mid + 1;
			}
			else {
				right = mid - 1;
			}
		}

		Debug::LogWarning("BinarySearch: Target not found in array.");
		return -1;
	}
};