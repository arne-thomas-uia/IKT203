#include "Menu.h"
#include "DebugUtils.h"
#include <iostream>
#include <limits>

namespace Task1 {

	// Searching implentations
	using FCheckMovie = FSearchNode<TMovie*>;

	// Callback A: Search By Title
	static bool SearchByTitle(const TMovie* aMovie, const void* aCriteria) {
		// 1. We know the criteria is actually a std::string*, so we cast it back safely
		const std::string* targetTitle = static_cast<const std::string*>(aCriteria);

		// 2. Perform a substring search (so "Knight" finds "The Dark Knight")
		return aMovie->GetTitle().find(*targetTitle) != std::string::npos;
	}

	// Callback B: Search By Director
	static bool SearchByDirector(const TMovie* aMovie, const void* aCriteria) {
		const std::string* targetDirector = static_cast<const std::string*>(aCriteria);
		return aMovie->GetDirector().find(*targetDirector) != std::string::npos;
	}

	// Callback C: Search By Genre (Bitwise Check)
	static bool SearchByGenre(const TMovie* aMovie, const void* aCriteria) {
		// Here, the criteria is a pointer to an integer representing the flags
		const int* targetFlags = static_cast<const int*>(aCriteria);

		// Bitwise check: Does the movie have the requested flag(s)?
		return (aMovie->GetGenreFlags() & *targetFlags) == *targetFlags;
	}


	TMenu::TMenu(TLinkedList<TMovie*>& aMovieList)
		: movieList(aMovieList), isRunning(false) {
	}

	void TMenu::ClearScreen() const {
		// A simple cross-platform way to clear console noise
		std::cout << "\033[2J\033[1;1H";
	}

	void TMenu::Pause() const {
		std::cout << "\nPress Enter to return to the menu...";
		std::cin.get();
	}

	void TMenu::DisplayMainMenu() const {
		ClearScreen();
		std::cout << "========================================\n";
		std::cout << "       IKT203 - MOVIE DATABASE          \n";
		std::cout << "========================================\n\n";
		std::cout << "  [1] Show All Movies\n";
		std::cout << "  [2] Search for a Movie (By Title)\n";
		std::cout << "  [0] Exit Program\n\n";
		std::cout << "========================================\n";
		std::cout << "Total records loaded: " << movieList.GetSize() << "\n\n";
		std::cout << "Select an option: ";
	}

	void TMenu::Run() {
		isRunning = true;
		while (isRunning) {
			DisplayMainMenu();
			HandleInput();
		}
	}

	void TMenu::HandleInput() {
		int choice;
		std::cin >> choice;

		// UX: Handle bad inputs (like typing letters instead of numbers)
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			choice = -1; // Force into the default switch case
		}
		else {
			// Clear the newline character left in the buffer
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		ClearScreen(); // Clear before showing results

		switch (choice) {
		case 1:
			ShowAllMovies();
			break;
		case 2:
			SearchMovie();
			break;
		case 0:
			isRunning = false;
			std::cout << "Exiting program. Goodbye!\n";
			break;
		default:
			std::cout << "Invalid selection. Please try again.\n";
			Pause();
			break;
		}
	}

	// --- Action Methods ---

	// A callback function needed for our TLinkedList::ForEach
	static void PrintMovieCallback(const TMovie* aMovie, int aIndex) {
		if (aIndex == 0) {
			std::cout << TMovie::GetHeaders() << "\n";
			std::cout << std::string(146, '-') << "\n";
		}
		std::cout << aMovie->ToString(aIndex + 1) << "\n";
	}

	void TMenu::ShowAllMovies() const {
		std::cout << "--- ALL MOVIES ---\n\n";

		// SoC: We don't write a loop here. We ask the list to apply our print function!
		movieList.ForEach(PrintMovieCallback);

		Pause();
	}

	void TMenu::SearchMovie() const {
		std::cout << "--- SEARCH MOVIES (BY TITLE) ---\n\n";
		std::cout << "Enter title keyword: ";

		std::string keyword;
		std::getline(std::cin, keyword);

		std::cout << "\nSearching for '" << keyword << "'...\n";

		// 1. Create a temporary list that DOES NOT own the data (isDataOwner = false)
		TLinkedList<TMovie*> searchResults(false);

		// 2. Populate the search results
		movieList.FindAll(SearchByTitle, &keyword, searchResults);

		// 3. Display the results
		if (!searchResults.IsEmpty()) {
			std::cout << "\nFound " << searchResults.GetSize() << " match(es)!\n";

			// We can reuse our exact same ForEach print callback!
			searchResults.ForEach(PrintMovieCallback);
		}
		else {
			std::cout << "\nNo movies found matching that title.\n";
		}

		Pause();
	}


}