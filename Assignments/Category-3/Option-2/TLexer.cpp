#include "TLexer.h"
#include "DebugUtils.h"
#include <cctype>
#include <stdexcept>
#include <format>

TToken::TToken(ETokenType aType, const std::string& aText, double aValue)
	: Type(aType), Text(aText), Value(aValue) {}

TLexer::TLexer(const std::string& aInput)
	: _input(aInput), _position(0) {
	Debug::LogVerbose("TLexer: Initialized with input string: '" + aInput + "'");
}

char TLexer::Peek() const {
	if (_position >= _input.length()) return '\0';
	return _input[_position];
}

char TLexer::Advance() {
	if (_position >= _input.length()) return '\0';
	return _input[_position++];
}

void TLexer::SkipWhitespace() {
	while (std::isspace(Peek())) {
		Advance();
	}
}

TToken* TLexer::ScanNumber() {
	std::string numStr;
	bool hasDecimal = false;

	while (std::isdigit(Peek()) || (Peek() == '.' && !hasDecimal)) {
		if (Peek() == '.') {
			hasDecimal = true;
		}
		numStr += Advance();
	}

	double value = std::stod(numStr);
	Debug::LogVerbose(std::format(" -> [Lexer] Scanned Number token: '{}' (Value: {})", numStr, value));
	return new TToken(ETokenType::Number, numStr, value);
}

TToken* TLexer::ScanIdentifier() {
	std::string idStr;
	while (std::isalnum(Peek()) || Peek() == '_') {
		idStr += Advance();
	}
	Debug::LogVerbose(std::format(" -> [Lexer] Scanned Identifier token: '{}'", idStr));
	return new TToken(ETokenType::Identifier, idStr);
}

TQueue<TToken*>* TLexer::Tokenize() {
	Debug::LogVerbose("TLexer: Starting tokenization process...");
	TQueue<TToken*>* tokenQueue = new TQueue<TToken*>();

	while (_position < _input.length()) {
		SkipWhitespace();
		char currentChar = Peek();

		if (currentChar == '\0') break;

		if (std::isdigit(currentChar) || currentChar == '.') {
			tokenQueue->Enqueue(ScanNumber());
		}
		else if (std::isalpha(currentChar)) {
			tokenQueue->Enqueue(ScanIdentifier());
		}
		else if (currentChar == '+' || currentChar == '-' || currentChar == '*' ||
			currentChar == '/' || currentChar == '^' || currentChar == '=') {
			std::string opStr(1, Advance());
			Debug::LogVerbose(std::format(" -> [Lexer] Scanned Operator/Assignment token: '{}'", opStr));
			tokenQueue->Enqueue(new TToken(ETokenType::Operator, opStr));
		}
		else if (currentChar == '(') {
			std::string parenStr(1, Advance());
			Debug::LogVerbose(" -> [Lexer] Scanned Left Parenthesis '('");
			tokenQueue->Enqueue(new TToken(ETokenType::LeftParen, parenStr));
		}
		else if (currentChar == ')') {
			std::string parenStr(1, Advance());
			Debug::LogVerbose(" -> [Lexer] Scanned Right Parenthesis ')'");
			tokenQueue->Enqueue(new TToken(ETokenType::RightParen, parenStr));
		}
		else {
			std::string unknownStr(1, Advance());
			Debug::LogVerbose(std::format(" -> [Lexer] Warning: Unknown character '{}'", unknownStr));
			tokenQueue->Enqueue(new TToken(ETokenType::Unknown, unknownStr));
		}
	}

	Debug::LogVerbose("TLexer: Tokenization complete.");
	return tokenQueue;
}