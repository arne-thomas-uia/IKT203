#include "TPrintJob.h"

TPrintJob::TPrintJob(int aJobId, TLinkedList<std::string*>* aDocumentState)
	: _jobId(aJobId), _snapshot(aDocumentState) {}

TPrintJob::~TPrintJob() {
	// The snapshot is created as data-owning (true), so deleting the list deletes the copied strings.
	delete _snapshot;
}

int TPrintJob::GetJobId() const {
	return _jobId;
}

TLinkedList<std::string*>* TPrintJob::GetSnapshot() const {
	return _snapshot;
}