#pragma once
#include <string>
#include "DataStructure.h"
#include "Movie.h"

namespace Task1 {

  class TMenu {
  private:
    // The menu holds a reference to the list, but doesn't own the data! (SoC)
    TLinkedList<TMovie*>& movieList;
    bool isRunning;

    // Internal UX methods
    void DisplayMainMenu() const;
    void HandleInput();
    void Pause() const;
    void ClearScreen() const;

    // Action methods
    void ShowAllMovies() const;
    void SearchMovie() const;

  public:
    // Constructor requires the list to operate on
    TMenu(TLinkedList<TMovie*>& aMovieList);

    // The main loop
    void Run();
  };

}