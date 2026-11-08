#pragma once
#include "TVariable.h"
#include <string>

// Forward declaration of the AVL Tree to avoid circular includes
template <typename T>
class TAVLTree;

// --- Abstract Base Class (Polymorphism & Abstraction) ---
class TExpressionNode {
public:
	// Virtual destructor ensures proper cascading deletion of derived classes
	virtual ~TExpressionNode() = default;

	// Pure virtual function enforcing the evaluation contract
	virtual double Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const = 0;
};

// --- Concrete Leaf: Constant Number ---
class TNumberNode : public TExpressionNode {
private:
	double _value;

public:
	TNumberNode(double aValue);
	double Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const override;
};

// --- Concrete Leaf: Variable (Requires Symbol Table) ---
class TVariableNode : public TExpressionNode {
private:
	std::string _name;

public:
	TVariableNode(const std::string& aName);
	double Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const override;
};

// --- Concrete Internal Node: Binary Operator (+, -, *, /, ^) ---
class TOperatorNode : public TExpressionNode {
private:
	char _operator;
	TExpressionNode* _left;
	TExpressionNode* _right;

public:
	TOperatorNode(char aOperator, TExpressionNode* aLeft, TExpressionNode* aRight);
	~TOperatorNode() override; // Manual memory management of children

	double Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const override;
};

// --- Concrete Internal Node: Math Function (sin, cos, log) ---
class TFunctionNode : public TExpressionNode {
private:
	std::string _functionName;
	TExpressionNode* _argument;

public:
	TFunctionNode(const std::string& aFunctionName, TExpressionNode* aArgument);
	~TFunctionNode() override; // Manual memory management of child

	double Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const override;
};