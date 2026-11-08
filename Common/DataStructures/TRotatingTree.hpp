#pragma once
#include "TBSTree.hpp"

// Inherits from TBSTree, but passes TDerived up the chain to maintain CRTP
template <typename T, typename TDerived>
class TRotatingTree : public TBSTree<T, TDerived> {
protected:
	void RotateLeft(TTreeNode<T>* aNode) {
		TTreeNode<T>* rightChild = aNode->GetRight();
		aNode->SetRight(rightChild->GetLeft());

		if (rightChild->GetLeft() != nullptr) {
			rightChild->GetLeft()->SetParent(aNode);
		}

		rightChild->SetParent(aNode->GetParent());

		if (aNode->GetParent() == nullptr) {
			this->_root = rightChild;
		}
		else if (aNode == aNode->GetParent()->GetLeft()) {
			aNode->GetParent()->SetLeft(rightChild);
		}
		else {
			aNode->GetParent()->SetRight(rightChild);
		}

		rightChild->SetLeft(aNode);
		aNode->SetParent(rightChild);
	}

	void RotateRight(TTreeNode<T>* aNode) {
		TTreeNode<T>* leftChild = aNode->GetLeft();
		aNode->SetLeft(leftChild->GetRight());

		if (leftChild->GetRight() != nullptr) {
			leftChild->GetRight()->SetParent(aNode);
		}

		leftChild->SetParent(aNode->GetParent());

		if (aNode->GetParent() == nullptr) {
			this->_root = leftChild;
		}
		else if (aNode == aNode->GetParent()->GetRight()) {
			aNode->GetParent()->SetRight(leftChild);
		}
		else {
			aNode->GetParent()->SetLeft(leftChild);
		}

		leftChild->SetRight(aNode);
		aNode->SetParent(leftChild);
	}

public:
	TRotatingTree(FCompareNodes<T> aCompare, bool aIsDataOwner)
		: TBSTree<T, TDerived>(aCompare, aIsDataOwner) {}
};