#pragma once
#include <string>
#include "DataStructure.h"
#include "TAction.h"
#include "TPrintJob.h"

class TTextEditor {
private:
	TLinkedList<std::string*>* _document;
	TStack<TAction*>* _undoStack;
	TStack<TAction*>* _redoStack;
	TQueue<TPrintJob*>* _printQueue;
	int _nextPrintJobId;

	void ClearRedoStack();
	void ClearActionStack(TStack<TAction*>* aStack);

	// Helper to perform the inverse action during undo/redo without triggering new action pushes
	void ExecuteInverseAction(TAction* aAction);

public:
	TTextEditor();
	~TTextEditor();

	TTextEditor(const TTextEditor&) = delete;
	TTextEditor& operator=(const TTextEditor&) = delete;

	// --- Editing Operations ---
	void AppendLine(const std::string& aText);
	void DeleteLastLine();

	// --- History Operations ---
	bool Undo();
	bool Redo();

	// --- Print Operations ---
	void QueuePrint();
	TPrintJob* ProcessNextPrintJob();
	bool IsPrintQueueEmpty() const;

	// --- Accessors ---
	TLinkedList<std::string*>* GetDocument() const;
};