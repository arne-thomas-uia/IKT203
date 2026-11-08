#include "TExpressionNode.h"
#include "DataStructure.h"
#include "DebugUtils.h"
#include <cmath>
#include <stdexcept>
#include <format>

// ==========================================
// TNumberNode
// ==========================================
TNumberNode::TNumberNode(double aValue) : _value(aValue) {}

double TNumberNode::Evaluate(const TAVLTree<TVariable*>* /*aSymbolTable*/) const {
	Debug::LogVerbose(std::format(" -> [Eval] Number Node: evaluated to {}", _value));
	return _value;
}

// ==========================================
// TVariableNode
// ==========================================
TVariableNode::TVariableNode(const std::string& aName) : _name(aName) {}

double TVariableNode::Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const {
	if (aSymbolTable == nullptr) {
		throw std::runtime_error("Symbol table is missing.");
	}
	Debug::LogVerbose(std::format(" -> [Eval] Variable Node: looking up '{}' in AVL Symbol Table...", _name));

	TVariable searchDummy(_name, 0.0);
	TVariable* foundVariable = aSymbolTable->Search(&searchDummy);

	if (foundVariable == nullptr) {
		throw std::runtime_error("Undefined variable: " + _name);
	}

	Debug::LogVerbose(std::format(" -> [Eval] Variable Node: found '{}' = {}", _name, foundVariable->Value));
	return foundVariable->Value;
}

// ==========================================
// TOperatorNode
// ==========================================
TOperatorNode::TOperatorNode(char aOperator, TExpressionNode* aLeft, TExpressionNode* aRight)
	: _operator(aOperator), _left(aLeft), _right(aRight) {}

TOperatorNode::~TOperatorNode() {
	delete _left;
	delete _right;
	_left = nullptr;
	_right = nullptr;
}

double TOperatorNode::Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const {
	Debug::LogVerbose(std::format(" -> [Eval] Operator Node '{}': evaluating left child...", _operator));
	double leftVal = _left->Evaluate(aSymbolTable);

	Debug::LogVerbose(std::format(" -> [Eval] Operator Node '{}': evaluating right child...", _operator));
	double rightVal = _right->Evaluate(aSymbolTable);

	double result = 0.0;
	switch (_operator) {
	case '+': result = leftVal + rightVal; break;
	case '-': result = leftVal - rightVal; break;
	case '*': result = leftVal * rightVal; break;
	case '/':
		if (rightVal == 0.0) throw std::runtime_error("Division by zero.");
		result = leftVal / rightVal;
		break;
	case '^': result = std::pow(leftVal, rightVal); break;
	default: throw std::runtime_error("Unknown operator encountered.");
	}

	Debug::LogVerbose(std::format(" -> [Eval] Operator Node: {} {} {} = {}", leftVal, _operator, rightVal, result));
	return result;
}

// ==========================================
// TFunctionNode
// ==========================================
TFunctionNode::TFunctionNode(const std::string& aFunctionName, TExpressionNode* aArgument)
	: _functionName(aFunctionName), _argument(aArgument) {}

TFunctionNode::~TFunctionNode() {
	delete _argument;
	_argument = nullptr;
}

double TFunctionNode::Evaluate(const TAVLTree<TVariable*>* aSymbolTable) const {
	Debug::LogVerbose(std::format(" -> [Eval] Function Node '{}': evaluating argument...", _functionName));
	double argVal = _argument->Evaluate(aSymbolTable);

	double result = 0.0;
	if (_functionName == "sin") result = std::sin(argVal);
	else if (_functionName == "cos") result = std::cos(argVal);
	else if (_functionName == "tan") result = std::tan(argVal);
	else if (_functionName == "log" || _functionName == "ln") {
		if (argVal <= 0.0) throw std::runtime_error("Logarithm domain error.");
		result = std::log(argVal);
	}
	else if (_functionName == "exp") result = std::exp(argVal);
	else {
		throw std::runtime_error("Unknown mathematical function: " + _functionName);
	}

	Debug::LogVerbose(std::format(" -> [Eval] Function Node: {}({}) = {}", _functionName, argVal, result));
	return result;
}