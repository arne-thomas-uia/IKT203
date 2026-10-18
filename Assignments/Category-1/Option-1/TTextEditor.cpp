#include "TTextEditor.h"

TTextEditor::TTextEditor() : _nextPrintJobId(1) {
	_document = new TLinkedList<std::string*>(true); // Owns the strings
	_undoStack = new TStack<TAction*>();             // Does NOT own pointers by default
	_redoStack = new TStack<TAction*>();
	_printQueue = new TQueue<TPrintJob*>();
}

TTextEditor::~TTextEditor() {
	ClearActionStack(_undoStack);
	ClearActionStack(_redoStack);

	while (!_printQueue->IsEmpty()) {
		TPrintJob* job = _printQueue->Dequeue();
		delete job;
	}

	delete _undoStack;
	delete _redoStack;
	delete _printQueue;
	delete _document; // Automatically deletes the string pointers inside
}

void TTextEditor::ClearActionStack(TStack<TAction*>* aStack) {
	while (!aStack->IsEmpty()) {
		TAction* action = aStack->Pop();
		delete action;
	}
}

void TTextEditor::ClearRedoStack() {
	ClearActionStack(_redoStack);
}

void TTextEditor::AppendLine(const std::string& aText) {
	_document->Append(new std::string(aText));
	ClearRedoStack();
	_undoStack->Push(new TAction(EActionType::Append, aText));
}

void TTextEditor::DeleteLastLine() {
	if (_document->IsEmpty()) return;

	int lastIndex = _document->GetSize() - 1;
	std::string* lastLine = _document->GetAtIndex(lastIndex);
	std::string savedText = *lastLine;

	_document->Remove(lastLine);
	ClearRedoStack();
	_undoStack->Push(new TAction(EActionType::DeleteLast, savedText));
}

void TTextEditor::ExecuteInverseAction(TAction* aAction) {
	if (aAction->Type == EActionType::Append) {
		// Inverse of append is deleting the last line
		std::string* lastLine = _document->GetAtIndex(_document->GetSize() - 1);
		_document->Remove(lastLine);
	}
	else if (aAction->Type == EActionType::DeleteLast) {
		// Inverse of delete is appending the saved text back
		_document->Append(new std::string(aAction->Text));
	}
}

bool TTextEditor::Undo() {
	if (_undoStack->IsEmpty()) return false;

	TAction* action = _undoStack->Pop();
	ExecuteInverseAction(action);

	_redoStack->Push(action);
	return true;
}

bool TTextEditor::Redo() {
	if (_redoStack->IsEmpty()) return false;

	TAction* action = _redoStack->Pop();

	// The original action is executed again
	if (action->Type == EActionType::Append) {
		_document->Append(new std::string(action->Text));
	}
	else if (action->Type == EActionType::DeleteLast) {
		std::string* lastLine = _document->GetAtIndex(_document->GetSize() - 1);
		_document->Remove(lastLine);
	}

	_undoStack->Push(action);
	return true;
}

void TTextEditor::QueuePrint() {
	// Create a Deep Copy of the document so the print job captures a fixed state in time
	TLinkedList<std::string*>* snapshot = new TLinkedList<std::string*>(true);

	int size = _document->GetSize();
	for (int i = 0; i < size; ++i) {
		std::string* currentLine = _document->GetAtIndex(i);
		snapshot->Append(new std::string(*currentLine));
	}

	TPrintJob* newJob = new TPrintJob(_nextPrintJobId++, snapshot);
	_printQueue->Enqueue(newJob);
}

TPrintJob* TTextEditor::ProcessNextPrintJob() {
	if (_printQueue->IsEmpty()) return nullptr;
	return _printQueue->Dequeue();
}

bool TTextEditor::IsPrintQueueEmpty() const {
	return _printQueue->IsEmpty();
}

TLinkedList<std::string*>* TTextEditor::GetDocument() const {
	return _document;
}