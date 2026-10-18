#pragma once
#include <string>
#include <functional>

inline const std::string MovieFilePath = std::string(STRUCTURE_DIR) + "/list_movies.txt";
inline const std::string NameFilePath = std::string(STRUCTURE_DIR) + "/list_names.txt";
inline const std::string SongFilePath = std::string(STRUCTURE_DIR) + "/list_songs.txt";
inline const std::string GraphCityFilePath = std::string(STRUCTURE_DIR) + "/graph_city.txt";
inline const std::string GraphNetworkFilePath = std::string(STRUCTURE_DIR) + "/graph_network.txt";
inline const std::string GraphRpgFilePath = std::string(STRUCTURE_DIR) + "/graph_rpg.txt";
inline const std::string GraphEsportArenasFilePath = std::string(STRUCTURE_DIR) + "/graph_esport_arenas.txt";
inline const std::string GraphEsportBoostersFilePath = std::string(STRUCTURE_DIR) + "/graph_esport_boosters.txt";
inline const std::string GraphEsportGlobalFilePath = std::string(STRUCTURE_DIR) + "/graph_esport_global.txt";

// Helper functions for reading graphs.
/*
void readGraphCityFromFile(FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead);
void readGraphNetworkFromFile(FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead);
void readGraphRpgFromFile(FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead);
*/



/// <summary>
/// Delegate type for processing a name read from a file.
/// </summary>
/// <param name="aIndex">The index of the name (0-based).</param>
/// <param name="aTotalCount">The total number of names.</param>
/// <param name="aFirstName">The first name read from the file.</param>
/// <param name="aLastName">The last name read from the file.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
using FNameRead = std::function<bool(const int aIndex, const int aTotalCount, const std::string& aFirstName, const std::string& aLastName)>;

/// <summary>
/// Use this function to read names from a specified file and process them using a callback function.
/// </summary>
///	<function>ReadNamesFromFile</function>
///	<description>Reads names from a specified file and invokes a callback for each name read.</description>
/// <param name="aFilename">The path to the file containing names.</param>
/// <param name="aOnNameRead">A callback function that is called for each name read. It takes two parameters: firstName and lastName. If the callback returns false, the reading process stops.</param>
/// <param name="firstName">The first name read from the file.</param>
/// <param name="lastName">The last name read from the file.</param>
/// <returns>None.</returns>
void ReadNamesFromFile(FNameRead aOnNameRead);


/// <summary>
/// Template overload to allow passing member functions directly.
/// </summary>
template <typename TObject>
void ReadNamesFromFile(TObject* instance, bool (TObject::*method)(const int, const int, const std::string&, const std::string&))
{
    ReadNamesFromFile([instance, method](const int index, const int total, const std::string& first, const std::string& last) {
        return (instance->*method)(index, total, first, last);
    });
}

/// <summary>
/// Delegate type for processing a node read from the file.
/// </summary>
/// <description>Function pointer type for a callback that processes nodes read from a file.</description>
/// <param name="aIndex">The index of the node (0-based).</param>
/// <param name="aTotalCount">The total number of nodes.</param>
/// <param name="aNode">The node std::string.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
using FNodeRead = std::function<bool(const int aIndex, const int aTotalCount, const std::string& aNode)>;

/// <summary>
/// Delegate type for processing an edge read from the file.
/// </summary>
/// <description>Function pointer type for a callback that processes edges read from a file.</description>
/// <param name="aIndex">The index of the edge (0-based).</param>
/// <param name="aTotalCount">The total number of edges.</param>
/// <param name="aFromNode">The from node std::string.</param>
/// <param name="aToNode">The to node std::string.</param>
/// <param name="aWeight">The weight of the edge.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
using FEdgeRead = std::function<bool(const int aIndex, const int aTotalCount, const std::string& aFromNode, const std::string& aToNode, float aWeight)>;

/// </summary>
/// Use this function to read a graph from a specified file and process its nodes and edges using callback functions.
/// </summary>
/// <function>ReadGraphFromFile</function>
/// <description>
/// Reads a graph from a specified file and invokes callbacks for each node and edge read.
/// All nodes are read first, followed by edges.
/// </description>
/// <param name="aFilename">The path to the file containing the graph data.</param>
/// <param name="aOnNodeRead">A callback function that is called for each node read. It takes one parameter: the node std::string. If the callback returns false, the reading process stops.</param>
/// <param name="aOnEdgeRead">A callback function that is called for each edge read. It takes three parameters: the fromNode std::string, the toNode std::string, and the weight float. If the callback returns false, the reading process stops.</param>
void ReadGraphFromFile(const std::string& aFilename, FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead);

/// <summary>
/// Template overload for ReadGraphFromFile to allow member functions.
/// </summary>
template <typename TNode, typename TEdge>
void ReadGraphFromFile(const std::string& aFilename, TNode&& aOnNodeRead, TEdge&& aOnEdgeRead)
{
  ReadGraphFromFile(aFilename,
    FNodeRead(std::forward<TNode>(aOnNodeRead)),
    FEdgeRead(std::forward<TEdge>(aOnEdgeRead)));
}


/// <summary>
/// Delegate type for processing a song read from the file.
/// </summary>
/// <param name="aIndex">The index of the song (0-based).</param>
/// <param name="aTotalCount">The total number of songs.</param>
/// <param name="aArtist">The artist.</param>
/// <param name="aTitle">The title.</param>
/// <param name="aYear">The release year (as a std::string).</param>
/// <param name="aGenre">The genre.</param>
/// <param name="aSource">The source.</param>
/// <returns>Returns true to continue reading, false to stop.</returns>
using FSongRead = std::function<bool(const int aIndex, const int aTotalCount, const std::string& aArtist, const std::string& aTitle, const std::string& aYear, const std::string& aGenre, const std::string& aSource)>;

/// <summary>
/// Reads song data from a file and processes them using a callback.
/// This function automatically skips the "records:=" header.
/// </summary>
/// <param name="aFilename">The path to the file (e.g., "songs.txt").</param>
/// <param name="aOnSongRead">The callback function called for each song.</param>
void ReadSongsFromFile(FSongRead aOnSongRead);

/// <summary>
/// Template overload to allow passing member functions directly (like Qt's connect).
/// </summary>
template <typename TObject>
void ReadSongsFromFile(TObject* instance, bool (TObject::*method)(const int, const int, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&))
{
    ReadSongsFromFile([instance, method](const int index, const int total, const std::string& artist, const std::string& title, const std::string& year, const std::string& genre, const std::string& source) {
        return (instance->*method)(index, total, artist, title, year, genre, source);
    });
}

/// <summary>
/// Delegate type for processing a movie read from the file.
/// </summary>
/// <param name="aIndex">The index of the movie (0-based).</param>
/// <param name="aTotalCount">The total number of movies.</param>
/// <param name="aTitle">The title of the movie.</param>
/// <param name="aDirector">The director of the movie.</param>
/// <param name="year">The release year of the movie.</param>
/// <param name="aGenre">The genre of the movie. Comma-separated if multiple genres.</param>
/// <param name="aRating">The rating of the movie (e.g., IMDb rating).</param>
using FMovieRead = std::function<bool(const int aIndex, const int aTotalCount, const std::string& aTitle, const std::string& aDirector, const unsigned short year, const std::string& aGenre, const float aRating)>;

/// <summary>
/// Reads movie data from a file and processes them using a callback.
/// this function automatically skips the "records:=" header.
/// </summary>
/// <param name="aFilename">The path to the file (e.g., "movies.txt").</param>
/// <param name="aOnMovieRead">The callback function called for each movie.</param>
void ReadMoviesFromFile(FMovieRead aOnMovieRead);

/// <summary>
/// Template overload to allow passing member functions directly.
/// </summary>
template <typename TObject>
void ReadMoviesFromFile(TObject* instance, bool (TObject::*method)(const int, const int, const std::string&, const std::string&, const unsigned short, const std::string&, const float))
{
    ReadMoviesFromFile([instance, method](const int index, const int total, const std::string& title, const std::string& director, const unsigned short year, const std::string& genre, const float rating) {
        return (instance->*method)(index, total, title, director, year, genre, rating);
    });
}
