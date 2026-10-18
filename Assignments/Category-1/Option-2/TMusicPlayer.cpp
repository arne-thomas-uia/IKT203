#include "TMusicPlayer.h"

TMusicPlayer::TMusicPlayer() : _nowPlaying(nullptr) {
	_songLibrary = new TLinkedList<TSong*>(true); // True: Library OWNS the TSong data
	_mainQueue = new TQueue<TSong*>();            // Queue does NOT own data
	_wishQueue = new TQueue<TSong*>();            // Queue does NOT own data
	_historyStack = new TStack<TSong*>();         // Stack does NOT own data
}

TMusicPlayer::~TMusicPlayer() {
	// Delete containers. Only _songLibrary will delete the actual TSong objects.
	delete _mainQueue;
	delete _wishQueue;
	delete _historyStack;
	delete _songLibrary;
}

void TMusicPlayer::AddSongToLibrary(TSong* aSong) {
	if (aSong != nullptr) {
		_songLibrary->Append(aSong);
	}
}

TLinkedList<TSong*>* TMusicPlayer::GetLibrary() const {
	return _songLibrary;
}

void TMusicPlayer::EnqueueMain(TSong* aSong) {
	if (aSong != nullptr) {
		_mainQueue->Enqueue(aSong);
	}
}

void TMusicPlayer::EnqueueWish(TSong* aSong) {
	if (aSong != nullptr) {
		_wishQueue->Enqueue(aSong);
	}
}

TSong* TMusicPlayer::PlayNext() {
	// If a song is currently playing, push it to history before changing tracks
	if (_nowPlaying != nullptr) {
		_historyStack->Push(_nowPlaying);
		_nowPlaying = nullptr;
	}

	// Priority 1: Wish Queue
	if (!_wishQueue->IsEmpty()) {
		_nowPlaying = _wishQueue->Dequeue();
	}
	// Priority 2: Main Queue
	else if (!_mainQueue->IsEmpty()) {
		_nowPlaying = _mainQueue->Dequeue();
	}

	return _nowPlaying;
}

TSong* TMusicPlayer::PlayPrevious() {
	if (_historyStack->IsEmpty()) {
		return nullptr;
	}

	// If we are playing a song, we don't destroy it, we just replace the "now playing" pointer.
	// (In a full app, we might push the interrupted song back to the front of a queue).

	_nowPlaying = _historyStack->Pop();
	return _nowPlaying;
}

TSong* TMusicPlayer::GetNowPlaying() const {
	return _nowPlaying;
}

bool TMusicPlayer::IsMainQueueEmpty() const {
	return _mainQueue->IsEmpty();
}

bool TMusicPlayer::IsWishQueueEmpty() const {
	return _wishQueue->IsEmpty();
}

bool TMusicPlayer::IsHistoryEmpty() const {
	return _historyStack->IsEmpty();
}