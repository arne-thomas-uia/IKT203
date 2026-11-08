#pragma once
#include "TLexer.h"
#include "TExpressionNode.h"
#include "DataStructure.h"
#include <stdexcept>

class TParser {
private:
	TQueue<TToken*>* _tokenQueue;
	TToken* _currentToken;

	// --- Memory & Queue Management ---
	void Advance();
	void Eat(ETokenType aExpectedType);

	// --- Recursive Descent Parsing Logic (PEMDAS) ---
	TExpressionNode* ParseFactor();     // Handles numbers, variables, functions, and ()
	TExpressionNode* ParsePower();      // Handles ^
	TExpressionNode* ParseTerm();       // Handles * and /
	TExpressionNode* ParseExpression(); // Handles + and -

public:
	// Constructor intentionally blank. The parser acts as a service.
	TParser() : _tokenQueue(nullptr), _currentToken(nullptr) {}

	// Takes absolute ownership of the Token Queue and returns the root of the AST
	TExpressionNode* Parse(TQueue<TToken*>* aTokenQueue);
};