#pragma once

template <typename T>
class TTreeNode {
private:
	T _data;
	TTreeNode* _left;
	TTreeNode* _right;
	TTreeNode* _parent;
	int _metadata; // Used for ENodeColorType (RBT) or Height (AVL)

public:
	TTreeNode(T aData)
		: _data(aData), _left(nullptr), _right(nullptr), _parent(nullptr), _metadata(0) {}

	~TTreeNode() {
		_data = nullptr;
		_left = nullptr;
		_right = nullptr;
		_parent = nullptr;
	}

	T GetData() const { return _data; }
	TTreeNode* GetLeft() const { return _left; }
	TTreeNode* GetRight() const { return _right; }
	TTreeNode* GetParent() const { return _parent; }
	int GetMetadata() const { return _metadata; }

	void SetLeft(TTreeNode* aLeft) { _left = aLeft; }
	void SetRight(TTreeNode* aRight) { _right = aRight; }
	void SetParent(TTreeNode* aParent) { _parent = aParent; }
	void SetMetadata(int aMetadata) { _metadata = aMetadata; }
};