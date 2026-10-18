#pragma once
#include "song.h"
#include "DataStructure.h"

class TMusicPlayer {
private:
	TLinkedList<TSong*>* _songLibrary; // Master list (owns memory)
	TQueue<TSong*>* _mainQueue;        // FIFO (does not own memory)
	TQueue<TSong*>* _wishQueue;        // Priority FIFO (does not own memory)
	TStack<TSong*>* _historyStack;     // LIFO (does not own memory)
	TSong* _nowPlaying;

public:
	TMusicPlayer();
	~TMusicPlayer();

	// Rule of Three protections
	TMusicPlayer(const TMusicPlayer&) = delete;
	TMusicPlayer& operator=(const TMusicPlayer&) = delete;

	// --- Library Management ---
	void AddSongToLibrary(TSong* aSong);
	TLinkedList<TSong*>* GetLibrary() const;

	// --- Queue Management ---
	void EnqueueMain(TSong* aSong);
	void EnqueueWish(TSong* aSong);

	// --- Playback Controls ---
	TSong* PlayNext();
	TSong* PlayPrevious();
	TSong* GetNowPlaying() const;

	// --- State Checks ---
	bool IsMainQueueEmpty() const;
	bool IsWishQueueEmpty() const;
	bool IsHistoryEmpty() const;
};