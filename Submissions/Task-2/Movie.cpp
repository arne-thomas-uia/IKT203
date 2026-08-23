#include "Movie.h"
#include <string>
#include <typeinfo>
#include <iomanip>
#include <sstream>

namespace Task1
{
	static constexpr int IdWidth = 4;
	static constexpr int TitleWidth = 60;
	static constexpr int DirectorWidth = 25;
	static constexpr int YearWidth = 6;
	static constexpr int RatingWidth = 6;
	static constexpr int GenreWidth = 45;

	static const char* GenreNames[] = {
		"Action", "Comedy", "Drama", "Horror", "Sci-Fi", "Romance",
		"Documentary", "Thriller", "Crime", "Fantasy", "Animation", "Adventure", "Western"
	};

	static const int GenreCount = sizeof(GenreNames) / sizeof(GenreNames[0]);

	// Helper function to safely truncate strings for display
	auto safeTruncate = [](const std::string& str, size_t width) {
		return str.length() > width ? str.substr(0, width) : str;
		};

	static std::string GenreFlagsToString(int aGenreFlags) {
		std::string result;
		for (int i = 0; i < GenreCount; ++i) { // 12 is your COUNT
			if (aGenreFlags & (1 << i)) {
				if (!result.empty()) result += ", ";
				result += GenreNames[i];
			}
		}
		return result.empty() ? "None" : result;
	}

	static int StringToGenreFlags(const std::string& aGenreString) {
		int flags = 0;
		for (int i = 0; i < GenreCount; ++i) {
			if (aGenreString.find(GenreNames[i]) != std::string::npos) {
				flags |= (1 << i);
			}
		}
		return flags;
	}

	// TMovie class implantations:
	
	TMovie::TMovie(const std::string& aTitle, const std::string& aDirector, int aYear, const std::string& aGenreFlags, float aRating)
		: title(aTitle),
		director(aDirector),
		year(aYear),
		genreFlags(StringToGenreFlags(aGenreFlags)),
		rating(aRating)
	{
		// Body can be empty
	}

	std::string TMovie::GetTitle() const
	{
		return title;
	}

	std::string TMovie::GetDirector() const
	{
		return director;
	}

	int TMovie::GetYear() const
	{
		return year;
	}

	int TMovie::GetGenreFlags() const
	{
		return genreFlags;
	}

	float TMovie::GetRating() const
	{
		return rating;
	}

	std::string TMovie::GetGenreString() const {
		return GenreFlagsToString(genreFlags);
	}

	std::string TMovie::ToString(const int aID) const {
		std::ostringstream oss;
		oss << std::left 
			<< std::setw(IdWidth) << aID
			<< std::setw(TitleWidth) << safeTruncate(title, TitleWidth - 1)
			<< std::setw(DirectorWidth) << safeTruncate(director, DirectorWidth - 1)
			<< std::setw(YearWidth) << year
			<< std::setw(RatingWidth) << std::fixed << std::setprecision(1) << rating
			<< std::setw(GenreWidth) << safeTruncate(GetGenreString(), GenreWidth - 1);
		return oss.str();
	}

	std::string TMovie::GetHeaders() {
		std::ostringstream oss;
		oss << std::left 
			<< std::setw(IdWidth) << "ID"
			<< std::setw(TitleWidth) << "Title"
			<< std::setw(DirectorWidth) << "Director"
			<< std::setw(YearWidth) << "Year"
			<< std::setw(RatingWidth) << "Rate"
			<< std::setw(GenreWidth) << "Genres";
		return oss.str();
	}

}
