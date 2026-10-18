// Option 2 (Advanced): Console Music Player.

#include <iostream>
#include <string>
#include <format>
#include "StructureReader.h"
#include "DebugUtils.h"
#include "song.h"
#include "TMusicPlayer.h"

// Note: Using a wrapper struct/functor for the callback to inject the player instance cleanly
struct TSongReader {
	TMusicPlayer* Player;

	bool operator()(const int aIndex, const int /*aTotalCount*/, const std::string& aArtist, const std::string& aTitle, const std::string& aYear, const std::string& aGenre, const std::string& aSource) const {
		TSong* song = new TSong(aIndex, aArtist, aTitle, aYear, aGenre, aSource);
		Player->AddSongToLibrary(song);
		return true;
	}
};

template<typename... Args>
void PrintMessage(const std::string& aMessage, const Args&... aArgs) {
	std::cout << std::vformat(aMessage, std::make_format_args(aArgs...)) << std::endl;
}

int RunApp(int /*argc*/, char* /*argv*/[]) {
	Debug::TLogger::SetLevel(Debug::ELevel::Verbose);
	PrintMessage("--- Initializing Console Music Player ---");

	// 1. Instantiate the single point of truth for business logic
	TMusicPlayer player;

	// 2. Load the data using the struct-based callback to strictly avoid global variables
	TSongReader reader{ &player };

	// Ensure you use the exact function signature for your StructureReader
	// e.g., ReadSongsFromFile(ListSongsFilePath, reader);
	Debug::LogInfo("Loading songs from file...");
	ReadSongsFromFile(reader); 

	TLinkedList<TSong*>* library = player.GetLibrary();
	int librarySize = library->GetSize();

	// 3. Print Library Forward
	PrintMessage("\n=== Songs in Forward Order ===");
	for (int i = 0; i < librarySize; ++i) {
		TSong* song = library->GetAtIndex(i);
		song->printOut();
	}

	// 4. Print Library Backward (Your SafeGet handles optimization automatically)
	PrintMessage("\n=== Songs in Backward Order ===");
	for (int i = librarySize - 1; i >= 0; --i) {
		TSong* song = library->GetAtIndex(i);
		song->printOut();
	}

	// 5. Populate Queues
	PrintMessage("\n=== Populating Play Queues ===");
	PrintMessage("Queuing 10 songs to the Main Queue...");
	for (int i = 0; i < 10 && i < librarySize; i++) {
		player.EnqueueMain(library->GetAtIndex(i));
	}

	PrintMessage("Queuing 5 songs to the Wish Queue...");
	for (int i = 10; i < 15 && i < librarySize; i++) {
		player.EnqueueWish(library->GetAtIndex(i));
	}

	// 6. Playback Simulation Loop
	PrintMessage("\n=== Starting Playback ===");

	int songsPlayed = 0;

	// Play until both queues are empty
	while (!player.IsMainQueueEmpty() || !player.IsWishQueueEmpty()) {
		TSong* current = player.PlayNext();
		songsPlayed++;

		if (current != nullptr) {
			PrintMessage("\n[Now Playing] Track {}:", songsPlayed);
			current->printOut();
		}

		// Trigger special events based on your exact simulation logic
		if (songsPlayed == 4) {
			PrintMessage("\n>>> EVENT: 4 songs played. Rewinding history by 3 tracks!");
			TSong* lastPlayed = nullptr;
			for (int i = 0; i < 3; i++) {
				lastPlayed = player.PlayPrevious();
			}

			if (lastPlayed != nullptr) {
				PrintMessage(">>> EVENT: Adding track back to the Wish Queue:");
				lastPlayed->printOut();
				player.EnqueueWish(lastPlayed);
			}
		}
		else if (songsPlayed == 14) {
			PrintMessage("\n>>> EVENT: 14 songs played. Adding 5 more to the Wish Queue!");
			for (int i = 15; i < 20 && i < librarySize; i++) {
				player.EnqueueWish(library->GetAtIndex(i));
			}
		}
	}

	PrintMessage("\n=== Playback Complete. All queues are empty. ===");
	std::cout << "Press Enter to exit.";
	std::cin.get();

	// TMusicPlayer automatically cleans up all queues, stacks, and the loaded TSong objects upon going out of scope.
	return 0;
}