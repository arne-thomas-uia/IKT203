#pragma once
#include "TQueue.hpp" // Included for Level-Order Traversal
#include "TTreeNode.hpp"
#include <functional>
#include <stdexcept>
#include <type_traits>

template <typename T>
using FCompareNodes = std::function<int(const std::remove_pointer_t<T>*, const std::remove_pointer_t<T>*)>;

template <typename T, typename TDerived>
class TBSTree {
protected:
	TTreeNode<T>* _root;
	FCompareNodes<T> _compare;
	bool _isDataOwner;

	TDerived* Derived() {
		return static_cast<TDerived*>(this);
	}

	void DestroyTree(TTreeNode<T>* aNode) {
		if (aNode != nullptr) {
			DestroyTree(aNode->GetLeft());
			DestroyTree(aNode->GetRight());

			// Strict manual memory management of the data pointers
			if (_isDataOwner && aNode->GetData() != nullptr) {
				delete aNode->GetData();
			}
			delete aNode;
		}
	}

	template <typename FAction>
	void InOrderTraversalInternal(TTreeNode<T>* aNode, FAction&& aAction) const {
		if (aNode != nullptr) {
			InOrderTraversalInternal(aNode->GetLeft(), aAction);
			aAction(aNode->GetData());
			InOrderTraversalInternal(aNode->GetRight(), aAction);
		}
	}

	template <typename FAction>
	void PreOrderTraversalInternal(TTreeNode<T>* aNode, FAction&& aAction) const {
		if (aNode != nullptr) {
			aAction(aNode->GetData());
			PreOrderTraversalInternal(aNode->GetLeft(), aAction);
			PreOrderTraversalInternal(aNode->GetRight(), aAction);
		}
	}

	template <typename FAction>
	void PostOrderTraversalInternal(TTreeNode<T>* aNode, FAction&& aAction) const {
		if (aNode != nullptr) {
			PostOrderTraversalInternal(aNode->GetLeft(), aAction);
			PostOrderTraversalInternal(aNode->GetRight(), aAction);
			aAction(aNode->GetData());
		}
	}

	// --- Iterative Traversal (Breadth-First) ---
	template <typename FAction>
	void LevelOrderTraversalInternal(TTreeNode<T>* aNode, FAction&& aAction) const {
		if (aNode == nullptr) return;

		TQueue<TTreeNode<T>*> traversalQueue;
		traversalQueue.Enqueue(aNode);

		while (!traversalQueue.IsEmpty()) {
			TTreeNode<T>* current = traversalQueue.Dequeue();

			aAction(current->GetData());

			if (current->GetLeft() != nullptr) {
				traversalQueue.Enqueue(current->GetLeft());
			}
			if (current->GetRight() != nullptr) {
				traversalQueue.Enqueue(current->GetRight());
			}
		}
	}

	// --- Helper for Transplanting Nodes (used in deletion) ---
	void Transplant(TTreeNode<T>* aDest, TTreeNode<T>* aSource) {
		if (aDest->GetParent() == nullptr) {
			_root = aSource;
		}
		else if (aDest == aDest->GetParent()->GetLeft()) {
			aDest->GetParent()->SetLeft(aSource);
		}
		else {
			aDest->GetParent()->SetRight(aSource);
		}
		if (aSource != nullptr) {
			aSource->SetParent(aDest->GetParent());
		}
	}

	// --- Helper to find the minimum node in a subtree ---
	TTreeNode<T>* GetMinimum(TTreeNode<T>* aNode) const {
		while (aNode->GetLeft() != nullptr) {
			aNode = aNode->GetLeft();
		}
		return aNode;
	}

public:
	TBSTree(FCompareNodes<T> aCompare, bool aIsDataOwner)
		: _root(nullptr), _compare(aCompare), _isDataOwner(aIsDataOwner) {

		if constexpr (!std::is_pointer_v<T>) {
			throw std::invalid_argument("TBSTree requires a pointer type for T.");
		}
	}

	~TBSTree() {
		DestroyTree(_root);
	}

	void Insert(T aData) {
		TTreeNode<T>* newNode = new TTreeNode<T>(aData);

		if (_root == nullptr) {
			_root = newNode;
			Derived()->OnAfterInsert(newNode);
			return;
		}

		TTreeNode<T>* current = _root;
		TTreeNode<T>* parent = nullptr;

		while (current != nullptr) {
			parent = current;
			if (_compare(aData, current->GetData()) < 0) {
				current = current->GetLeft();
			}
			else {
				current = current->GetRight();
			}
		}

		newNode->SetParent(parent);
		if (_compare(aData, parent->GetData()) < 0) {
			parent->SetLeft(newNode);
		}
		else {
			parent->SetRight(newNode);
		}

		// Compile-time polymorphic hook for balancing
		Derived()->OnAfterInsert(newNode);
	}

	T Search(T aTarget) const {
		TTreeNode<T>* current = _root;
		while (current != nullptr) {
			int cmp = _compare(aTarget, current->GetData());
			if (cmp == 0) return current->GetData();

			if (cmp < 0) current = current->GetLeft();
			else current = current->GetRight();
		}
		return nullptr;
	}

	template <typename FAction>
	void TraverseInOrder(FAction&& aAction) const {
		InOrderTraversalInternal(_root, std::forward<FAction>(aAction));
	}

	template <typename FAction>
	void TraversePreOrder(FAction&& aAction) const {
		PreOrderTraversalInternal(_root, std::forward<FAction>(aAction));
	}

	template <typename FAction>
	void TraversePostOrder(FAction&& aAction) const {
		PostOrderTraversalInternal(_root, std::forward<FAction>(aAction));
	}

	template <typename FAction>
	void TraverseLevelOrder(FAction&& aAction) const {
		LevelOrderTraversalInternal(_root, std::forward<FAction>(aAction));
	}

	// Default hook (shadowed by TDerived)
	void OnAfterInsert(TTreeNode<T>* aNode) {}

	bool Delete(T aTarget) {
		TTreeNode<T>* z = _root;

		// 1. Standard Search
		while (z != nullptr) {
			int cmp = _compare(aTarget, z->GetData());
			if (cmp == 0) break;
			if (cmp < 0) z = z->GetLeft();
			else z = z->GetRight();
		}

		if (z == nullptr) return false; // Target not found

		TTreeNode<T>* x = nullptr;
		TTreeNode<T>* xParent = nullptr;
		TTreeNode<T>* y = z;
		int yOriginalMetadata = y->GetMetadata(); // Save color/height before structural changes

		// 2. Standard BST Removal Logic
		if (z->GetLeft() == nullptr) {
			x = z->GetRight();
			xParent = z->GetParent();
			Transplant(z, z->GetRight());
		}
		else if (z->GetRight() == nullptr) {
			x = z->GetLeft();
			xParent = z->GetParent();
			Transplant(z, z->GetLeft());
		}
		else {
			y = GetMinimum(z->GetRight());
			yOriginalMetadata = y->GetMetadata();
			x = y->GetRight();

			if (y->GetParent() == z) {
				xParent = y; // x might be null, explicitly track its parent
			}
			else {
				xParent = y->GetParent();
				Transplant(y, y->GetRight());
				y->SetRight(z->GetRight());
				y->GetRight()->SetParent(y);
			}

			Transplant(z, y);
			y->SetLeft(z->GetLeft());
			y->GetLeft()->SetParent(y);
			y->SetMetadata(z->GetMetadata()); // Y assumes Z's original metadata (color/height)
		}

		// 3. Trigger CRTP Hook for rebalancing
		Derived()->OnAfterDelete(x, xParent, yOriginalMetadata);

		// 4. Strict Manual Memory Management
		if (_isDataOwner && z->GetData() != nullptr) {
			delete z->GetData();
		}
		delete z;

		return true;
	}

	// Default CRTP hook (shadowed by TDerived)
	void OnAfterDelete(TTreeNode<T>* aNode, TTreeNode<T>* aParent, int aOriginalMetadata) {}
};