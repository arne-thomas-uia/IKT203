#pragma once
#include <string>
#include "DataStructure.h"

class TPrintJob {
private:
	int _jobId;
	TLinkedList<std::string*>* _snapshot; // A deep copy of the document state

public:
	TPrintJob(int aJobId, TLinkedList<std::string*>* aDocumentState);
	~TPrintJob();

	// Rule of Three protections
	TPrintJob(const TPrintJob&) = delete;
	TPrintJob& operator=(const TPrintJob&) = delete;

	int GetJobId() const;
	TLinkedList<std::string*>* GetSnapshot() const;
};