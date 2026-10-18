// Option 1 (Standard): Console Text Editor 📝

#include <iostream>
#include <string>
#include <format>
#include "DebugUtils.h"
#include "TTextEditor.h"

template<typename... Args>
void PrintMessage(const std::string& aMessage, const Args&... aArgs) {
	std::cout << std::vformat(aMessage, std::make_format_args(aArgs...)) << std::endl;
}

void PrintDocumentState(const TLinkedList<std::string*>* aDocument, bool aPrintBackwards = false) {
	int size = aDocument->GetSize();
	if (size == 0) {
		PrintMessage("  [Document is empty]");
		return;
	}

	if (!aPrintBackwards) {
		for (int i = 0; i < size; ++i) {
			PrintMessage("  {}: {}", i + 1, *aDocument->GetAtIndex(i));
		}
	}
	else {
		for (int i = size - 1; i >= 0; --i) {
			PrintMessage("  {}: {}", i + 1, *aDocument->GetAtIndex(i));
		}
	}
}

int RunApp(int /*argc*/, char* /*argv*/[]) {
	Debug::TLogger::SetLevel(Debug::ELevel::Verbose);
	PrintMessage("--- Initializing Console Text Editor ---");

	TTextEditor editor;

	PrintMessage("\n>>> ACTION: Appending 3 lines");
	editor.AppendLine("Line 1: Initialize the core sequence.");
	editor.AppendLine("Line 2: Establish the database connection.");
	editor.AppendLine("Line 3: Execute the primary logic loop.");

	PrintMessage("\n=== Current Document ===");
	PrintDocumentState(editor.GetDocument());

	PrintMessage("\n>>> ACTION: Sending document to Print Queue");
	editor.QueuePrint();

	PrintMessage("\n>>> ACTION: Deleting the last line");
	editor.DeleteLastLine();

	PrintMessage("\n=== Current Document ===");
	PrintDocumentState(editor.GetDocument());

	PrintMessage("\n>>> ACTION: UNDO triggered");
	editor.Undo();

	PrintMessage("\n=== Current Document (After Undo) ===");
	PrintDocumentState(editor.GetDocument());

	PrintMessage("\n>>> ACTION: REDO triggered");
	editor.Redo();

	PrintMessage("\n=== Current Document (After Redo) ===");
	PrintDocumentState(editor.GetDocument());

	PrintMessage("\n>>> ACTION: Sending updated document to Print Queue");
	editor.QueuePrint();

	PrintMessage("\n=== Processing Print Queue (FIFO) ===");
	while (!editor.IsPrintQueueEmpty()) {
		TPrintJob* job = editor.ProcessNextPrintJob();
		PrintMessage("\n[PRINT JOB #{}]", job->GetJobId());

		TLinkedList<std::string*>* snapshot = job->GetSnapshot();
		PrintDocumentState(snapshot);

		// The Presentation Layer takes responsibility for cleaning up the returned data object
		delete job;
	}

	PrintMessage("\n>>> ACTION: Displaying final document backwards");
	PrintDocumentState(editor.GetDocument(), true);

	PrintMessage("\n=== Editor Session Complete. ===");
	std::cout << "Press Enter to exit.";
	std::cin.get();

	return 0;
}