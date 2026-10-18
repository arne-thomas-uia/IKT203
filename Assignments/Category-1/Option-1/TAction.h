#pragma once
#include <string>

enum class EActionType {
	Append,
	DeleteLast
};

struct TAction {
	EActionType Type;
	std::string Text; // Stores the text that was appended or deleted for restoration

	TAction(EActionType aType, std::string aText) : Type(aType), Text(aText) {}
};