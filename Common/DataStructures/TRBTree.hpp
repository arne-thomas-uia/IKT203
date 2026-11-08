#pragma once
#include "DebugUtils.h"
#include "TRotatingTree.hpp"

enum class ENodeColorType { Black = 0, Red = 1 };

template <typename T>
class TRBTree : public TRotatingTree<T, TRBTree<T>> {
private:
	ENodeColorType GetColor(TTreeNode<T>* aNode) const {
		if (aNode == nullptr) return ENodeColorType::Black;
		return static_cast<ENodeColorType>(aNode->GetMetadata());
	}

	void SetColor(TTreeNode<T>* aNode, ENodeColorType aColor) {
		if (aNode != nullptr) {
			aNode->SetMetadata(static_cast<int>(aColor));
		}
	}

	// --- DRY Helper 1: Uncle is Red (Recolor only) ---
	void HandleRecolorCase(TTreeNode<T>*& aNode, TTreeNode<T>* aParent, TTreeNode<T>* aGrandParent, TTreeNode<T>* aUncle) {
		Debug::LogVerbose("RBT Balancing (Recolor Case): Uncle is Red. Recoloring Parent and Uncle to Black, GrandParent to Red.");
		SetColor(aParent, ENodeColorType::Black);
		SetColor(aUncle, ENodeColorType::Black);
		SetColor(aGrandParent, ENodeColorType::Red);
		aNode = aGrandParent; // Move up the tree to continue evaluation
	}

	// --- DRY Helper 2: Uncle is Black (Rotation required) ---
	// Using templates (F1, F2) to allow compiler inlining and avoid std::function heap allocations
	template <typename F1, typename F2>
	void HandleRotationCase(TTreeNode<T>*& aNode, TTreeNode<T>*& aParent, TTreeNode<T>* aGrandParent, bool aIsInnerChild, F1&& aFirstRotate, F2&& aSecondRotate) {

		Debug::LogVerbose("RBT Balancing (Rotation Case): Uncle is Black. Proceeding with structural rotations.");

		if (aIsInnerChild) {
			// Step 1: Rotate on parent if it's an inner child (Left-Right or Right-Left case)
			Debug::LogVerbose(" -> Inner child detected. Executing first rotation on Parent.");
			aNode = aParent;
			aFirstRotate(aNode);
			aParent = aNode->GetParent();
		}
		// Step 2: Recolor and rotate on grandparent (Left-Left or Right-Right case)
		Debug::LogVerbose(" -> Executing second rotation on GrandParent and recoloring.");
		SetColor(aParent, ENodeColorType::Black);
		SetColor(aGrandParent, ENodeColorType::Red);
		aSecondRotate(aGrandParent);
	}

	template <typename F1, typename F2>
	void HandleDeleteCase(TTreeNode<T>*& aNode, TTreeNode<T>*& aParent, bool aIsLeftChild,
		F1&& aRotateInner, F2&& aRotateOuter) {

		TTreeNode<T>* sibling = aIsLeftChild ? aParent->GetRight() : aParent->GetLeft();

		// Case 1: Sibling is Red
		if (GetColor(sibling) == ENodeColorType::Red) {
			SetColor(sibling, ENodeColorType::Black);
			SetColor(aParent, ENodeColorType::Red);
			aRotateInner(aParent);
			sibling = aIsLeftChild ? aParent->GetRight() : aParent->GetLeft();
		}

		// Case 2: Sibling's children are both Black
		if (GetColor(sibling->GetLeft()) == ENodeColorType::Black &&
			GetColor(sibling->GetRight()) == ENodeColorType::Black) {

			SetColor(sibling, ENodeColorType::Red);
			aNode = aParent;
			aParent = aNode->GetParent();
		}
		else {
			// Case 3: Sibling's inner child is Red, outer is Black
			TTreeNode<T>* innerChild = aIsLeftChild ? sibling->GetLeft() : sibling->GetRight();
			TTreeNode<T>* outerChild = aIsLeftChild ? sibling->GetRight() : sibling->GetLeft();

			if (GetColor(outerChild) == ENodeColorType::Black) {
				SetColor(innerChild, ENodeColorType::Black);
				SetColor(sibling, ENodeColorType::Red);
				aRotateOuter(sibling);
				sibling = aIsLeftChild ? aParent->GetRight() : aParent->GetLeft();
			}

			// Case 4: Sibling's outer child is Red
			SetColor(sibling, GetColor(aParent));
			SetColor(aParent, ENodeColorType::Black);
			if (aIsLeftChild && sibling->GetRight() != nullptr) {
				SetColor(sibling->GetRight(), ENodeColorType::Black);
			}
			else if (!aIsLeftChild && sibling->GetLeft() != nullptr) {
				SetColor(sibling->GetLeft(), ENodeColorType::Black);
			}
			aRotateInner(aParent);

			// Terminate loop
			aNode = this->_root;
			aParent = nullptr;
		}
	}

public:
	TRBTree(FCompareNodes<T> aCompare, bool aIsDataOwner)
		: TRotatingTree<T, TRBTree<T>>(aCompare, aIsDataOwner) {}

	// CRTP Execution: Takes control immediately after TBSTree finishes placement
	void OnAfterInsert(TTreeNode<T>* aNode) {
		Debug::LogInfo("TRBTree: New node inserted. Evaluating Red-Black invariants...");
		SetColor(aNode, ENodeColorType::Red);

		// Modern C++23 auto lambdas for zero-overhead invocation
		auto rotateLeft = [this](TTreeNode<T>* n) {
			Debug::LogVerbose(" -> [Rotate Left]");
			this->RotateLeft(n);
			};
		auto rotateRight = [this](TTreeNode<T>* n) {
			Debug::LogVerbose(" -> [Rotate Right]");
			this->RotateRight(n);
			};

		while (aNode != this->_root && GetColor(aNode->GetParent()) == ENodeColorType::Red) {
			TTreeNode<T>* parent = aNode->GetParent();
			TTreeNode<T>* grandParent = parent->GetParent();

			if (parent == grandParent->GetLeft()) {
				TTreeNode<T>* uncle = grandParent->GetRight();

				if (GetColor(uncle) == ENodeColorType::Red) {
					HandleRecolorCase(aNode, parent, grandParent, uncle);
				}
				else {
					HandleRotationCase(aNode, parent, grandParent, (aNode == parent->GetRight()), rotateLeft, rotateRight);
				}
			}
			else {
				TTreeNode<T>* uncle = grandParent->GetLeft();

				if (GetColor(uncle) == ENodeColorType::Red) {
					HandleRecolorCase(aNode, parent, grandParent, uncle);
				}
				else {
					HandleRotationCase(aNode, parent, grandParent, (aNode == parent->GetLeft()), rotateRight, rotateLeft);
				}
			}
		}

		SetColor(this->_root, ENodeColorType::Black);
	}

	void OnAfterDelete(TTreeNode<T>* aNode, TTreeNode<T>* aParent, int aOriginalMetadata) {
		// If the node that was physically removed/moved was Red, black-height is maintained.
		if (static_cast<ENodeColorType>(aOriginalMetadata) == ENodeColorType::Red) return;

		auto rotateLeft = [this](TTreeNode<T>* n) { this->RotateLeft(n); };
		auto rotateRight = [this](TTreeNode<T>* n) { this->RotateRight(n); };

		// Fixup Loop
		while (aNode != this->_root && GetColor(aNode) == ENodeColorType::Black) {
			if (aNode == aParent->GetLeft() || (aNode == nullptr && aParent->GetLeft() == nullptr)) {
				// Node is a Left Child (Inner Rotate = Left, Outer Rotate = Right)
				HandleDeleteCase(aNode, aParent, true, rotateLeft, rotateRight);
			}
			else {
				// Node is a Right Child (Inner Rotate = Right, Outer Rotate = Left)
				HandleDeleteCase(aNode, aParent, false, rotateRight, rotateLeft);
			}
		}

		SetColor(aNode, ENodeColorType::Black);
	}

};