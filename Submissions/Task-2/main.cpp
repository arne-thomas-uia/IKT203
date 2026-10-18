#include <iostream>
#include <iomanip>
#include <string>

#include "StructureReader.h"
#include "DataStructure.h"
#include "DebugUtils.h"
#include "Movie.h"
#include "Menu.h"

using namespace Task1;

// FIX: Initialize the list right here since it requires the 'isDataOwner' boolean.
static TLinkedList<TMovie*> movieList(true);

// Callback for Movies
static bool OnMovieFound(
  const int aIndex,
  const int aTotalCount,
  const std::string& aTitle,
  const std::string& aDirector,
  const unsigned short aYear,
  const std::string& aGenre,
  const float aRating)
{
  // 1. Create a new TMovie instance on the heap
  // (Notice that aYear implicitly casts from unsigned short to int, which is perfectly safe here)
  TMovie* newMovie = new TMovie(aTitle, aDirector, aYear, aGenre, aRating);

  // 2. Append it to our global linked list
  movieList.Append(newMovie);

  // 3. Log a success message when we hit the final record
  if (aIndex == aTotalCount - 1) {
    debugText(Debug::ELevel::Information, "Successfully loaded {} movies into the Linked List.", aTotalCount);
  }

  return true; // Return true to continue reading the next movie
}

int main() {
  // Set up our colored logger
  Debug::TLogger::SetLevel(Debug::ELevel::Information);

  // (Adjusted the text to say Task 1!)
  debugText(Debug::ELevel::Information, "IKT203 - Task 1: Movie Data and Linked List Application");

  // Call the parser. For every line it reads, it will trigger OnMovieFound and populate movieList!
  ReadMoviesFromFile(OnMovieFound);

  // Quick verification to prove to ourselves that it worked
  debugText(Debug::ELevel::Information, "Current List Size: {}", movieList.GetSize());

  // TODO: Create a TMenu class that allows the user to interact with the movie list (e.g., display movies, search, filter, etc.)
  debugText(Debug::ELevel::Information, "Launching User Interface...");

  // Pass the populated list into the menu
  TMenu appMenu(movieList);
  appMenu.Run(); // The program stays inside this loop until the user presses 0

  return 0;
}