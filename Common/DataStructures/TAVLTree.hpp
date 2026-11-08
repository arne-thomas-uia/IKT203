#pragma once
#include "TRotatingTree.hpp"
#include <algorithm>

template <typename T>
class TAVLTree : public TRotatingTree<T, TAVLTree<T>> {
private:
	// --- Height & Balance Utilities ---
	int GetHeight(TTreeNode<T>* aNode) const {
		if (aNode == nullptr) return 0;
		return aNode->GetMetadata();
	}

	int GetBalanceFactor(TTreeNode<T>* aNode) const {
		if (aNode == nullptr) return 0;
		return GetHeight(aNode->GetLeft()) - GetHeight(aNode->GetRight());
	}

	void UpdateHeight(TTreeNode<T>* aNode) {
		if (aNode != nullptr) {
			int leftHeight = GetHeight(aNode->GetLeft());
			int rightHeight = GetHeight(aNode->GetRight());
			aNode->SetMetadata(1 + std::max(leftHeight, rightHeight));
		}
	}

	// --- Rotation Wrappers (To fix heights after structural rotation) ---
	void PerformRotateLeft(TTreeNode<T>* aNode) {
		Debug::LogVerbose(" -> [AVL] Performing Left Rotation...");
		this->RotateLeft(aNode);
		// After a left rotation, aNode drops down, and its right child moves up.
		UpdateHeight(aNode);
		UpdateHeight(aNode->GetParent());
	}

	void PerformRotateRight(TTreeNode<T>* aNode) {
		Debug::LogVerbose(" -> [AVL] Performing Right Rotation...");
		this->RotateRight(aNode);
		// After a right rotation, aNode drops down, and its left child moves up.
		UpdateHeight(aNode);
		UpdateHeight(aNode->GetParent());
	}

public:
	TAVLTree(FCompareNodes<T> aCompare, bool aIsDataOwner)
		: TRotatingTree<T, TAVLTree<T>>(aCompare, aIsDataOwner) {}

	// --- CRTP Hook: Executed immediately after TBSTree places the new node ---
	void OnAfterInsert(TTreeNode<T>* aNode) {
		// New nodes are inserted as leaves with a height of 1
		Debug::LogVerbose("TAVLTree: New node inserted. Recalculating heights and balancing AVL tree...");

		aNode->SetMetadata(1);

		TTreeNode<T>* current = aNode->GetParent();

		// Walk back up to the root, updating heights and checking balances
		while (current != nullptr) {
			UpdateHeight(current);

			int balance = GetBalanceFactor(current);
			Debug::LogVerbose(std::format(" -> [AVL] Node balance factor: {}", balance));

			// Case 1: Left Heavy (LL or LR)
			if (balance > 1) {
				if (GetBalanceFactor(current->GetLeft()) < 0) {
					// Left-Right Case
					Debug::LogVerbose(" -> [AVL] Left-Right (LR) imbalance detected.");
					PerformRotateLeft(current->GetLeft());
					PerformRotateRight(current);
				}
				else {
					// Left-Left Case
					Debug::LogVerbose(" -> [AVL] Left-Left (LL) imbalance detected.");
					PerformRotateRight(current);
				}
			}
			// Case 2: Right Heavy (RR or RL)
			else if (balance < -1) {
				if (GetBalanceFactor(current->GetRight()) > 0) {
					// Right-Left Case
					Debug::LogVerbose(" -> [AVL] Right-Left (RL) imbalance detected.");
					PerformRotateRight(current->GetRight());
					PerformRotateLeft(current);
				}
				else {
					// Right-Right Case
					Debug::LogVerbose(" -> [AVL] Right-Right (RR) imbalance detected.");
					PerformRotateLeft(current);
				}
			}

			// Move up to the next ancestor. 
			// Note: If a rotation occurred, current->GetParent() correctly points to the new parent.
			current = current->GetParent();
		}
		Debug::LogVerbose("TAVLTree: Balancing complete. Invariants secured.");
	}
};