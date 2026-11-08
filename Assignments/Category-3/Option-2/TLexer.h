#pragma once
#include "DataStructure.h"
#include <string>

// Gentlemen's Rule: Enum prefix 'E' and suffix 'Type'
enum class ETokenType {
	Number,
	Identifier, // Used for variables (e.g., 'x') or functions (e.g., 'sin')
	Operator,   // +, -, *, /, ^, =
	LeftParen,
	RightParen,
	Unknown
};

// The Data Entity for the Lexer
class TToken {
public:
	ETokenType Type;
	std::string Text;
	double Value; // Populated only if Type == Number

	TToken(ETokenType aType, const std::string& aText, double aValue = 0.0);
};

// The Lexer Engine
class TLexer {
private:
	std::string _input;
	size_t _position;

	// --- Internal Utilities ---
	char Peek() const;
	char Advance();
	void SkipWhitespace();

	TToken* ScanNumber();
	TToken* ScanIdentifier();

public:
	TLexer(const std::string& aInput);

	// Returns a heap-allocated Queue of heap-allocated Tokens. 
	// The Parser will take ownership of this memory.
	TQueue<TToken*>* Tokenize();
};