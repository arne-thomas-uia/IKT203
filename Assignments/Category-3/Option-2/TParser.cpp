#include "TParser.h"

// --- Memory & Queue Management ---
void TParser::Advance() {
	if (_currentToken != nullptr) {
		delete _currentToken;
		_currentToken = nullptr;
	}

	if (!_tokenQueue->IsEmpty()) {
		_currentToken = _tokenQueue->Dequeue();
	}
}

void TParser::Eat(ETokenType aExpectedType) {
	if (_currentToken != nullptr && _currentToken->Type == aExpectedType) {
		Debug::LogVerbose(std::format(" -> [Parser] Consumed token: '{}'", _currentToken->Text));
		Advance();
	}
	else {
		throw std::runtime_error("Parser syntax error: Unexpected token type.");
	}
}

TExpressionNode* TParser::ParseFactor() {
	if (_currentToken == nullptr) {
		throw std::runtime_error("Unexpected end of expression.");
	}

	TToken* token = _currentToken;
	Debug::LogVerbose(std::format(" -> [Parser] Parsing Factor from token: '{}'", token->Text));

	if (token->Type == ETokenType::Number) {
		TExpressionNode* node = new TNumberNode(token->Value);
		Advance();
		return node;
	}

	if (token->Type == ETokenType::Identifier) {
		std::string name = token->Text;
		Advance();

		if (_currentToken != nullptr && _currentToken->Type == ETokenType::LeftParen) {
			Debug::LogVerbose(std::format(" -> [Parser] Detected function call: '{}'", name));
			Eat(ETokenType::LeftParen);
			TExpressionNode* argumentNode = ParseExpression();
			Eat(ETokenType::RightParen);
			return new TFunctionNode(name, argumentNode);
		}

		Debug::LogVerbose(std::format(" -> [Parser] Created Variable Node for: '{}'", name));
		return new TVariableNode(name);
	}

	if (token->Type == ETokenType::LeftParen) {
		Debug::LogVerbose(" -> [Parser] Entering parenthesized sub-expression");
		Eat(ETokenType::LeftParen);
		TExpressionNode* node = ParseExpression();
		Eat(ETokenType::RightParen);
		return node;
	}

	throw std::runtime_error("Parser syntax error: Unrecognized factor.");
}

TExpressionNode* TParser::ParsePower() {
	Debug::LogVerbose(" -> [Parser] Parsing Power level (^)...");
	TExpressionNode* leftNode = ParseFactor();

	while (_currentToken != nullptr &&
		_currentToken->Type == ETokenType::Operator &&
		_currentToken->Text == "^") {

		char op = _currentToken->Text[0];
		Debug::LogVerbose(" -> [Parser] Found exponentiation operator '^'");
		Advance();
		TExpressionNode* rightNode = ParseFactor();
		leftNode = new TOperatorNode(op, leftNode, rightNode);
	}

	return leftNode;
}

TExpressionNode* TParser::ParseTerm() {
	Debug::LogVerbose(" -> [Parser] Parsing Term level (*, /)...");
	TExpressionNode* leftNode = ParsePower();

	while (_currentToken != nullptr &&
		_currentToken->Type == ETokenType::Operator &&
		(_currentToken->Text == "*" || _currentToken->Text == "/")) {

		char op = _currentToken->Text[0];
		Debug::LogVerbose(std::format(" -> [Parser] Found multiplicative operator '{}'", op));
		Advance();
		TExpressionNode* rightNode = ParsePower();
		leftNode = new TOperatorNode(op, leftNode, rightNode);
	}

	return leftNode;
}

TExpressionNode* TParser::ParseExpression() {
	Debug::LogVerbose(" -> [Parser] Parsing Expression level (+, -)...");
	TExpressionNode* leftNode = ParseTerm();

	while (_currentToken != nullptr &&
		_currentToken->Type == ETokenType::Operator &&
		(_currentToken->Text == "+" || _currentToken->Text == "-")) {

		char op = _currentToken->Text[0];
		Debug::LogVerbose(std::format(" -> [Parser] Found additive operator '{}'", op));
		Advance();
		TExpressionNode* rightNode = ParseTerm();
		leftNode = new TOperatorNode(op, leftNode, rightNode);
	}

	return leftNode;
}

TExpressionNode* TParser::Parse(TQueue<TToken*>* aTokenQueue) {
	if (aTokenQueue == nullptr || aTokenQueue->IsEmpty()) {
		throw std::invalid_argument("Cannot parse an empty token queue.");
	}

	Debug::LogVerbose("TParser: Starting recursive descent parsing...");
	_tokenQueue = aTokenQueue;
	_currentToken = _tokenQueue->Dequeue();

	TExpressionNode* rootNode = ParseExpression();

	if (_currentToken != nullptr) {
		delete _currentToken;
		_currentToken = nullptr;
	}

	while (!_tokenQueue->IsEmpty()) {
		TToken* orphanedToken = _tokenQueue->Dequeue();
		delete orphanedToken;
	}

	delete _tokenQueue;
	_tokenQueue = nullptr;

	Debug::LogVerbose("TParser: Abstract Syntax Tree (AST) successfully constructed.");
	return rootNode;
}