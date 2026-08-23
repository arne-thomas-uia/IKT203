#pragma once
#include <string>

namespace Task1 {

	enum EGenreFlags {
		Action = 0x0001,
		Comedy = 0x0002,
		Drama = 0x0004,
		Horror = 0x0008,
		SciFi = 0x0010,
		Romance = 0x0020,
		Documentary = 0x0040,
		Thriller = 0x0080,
		Crime = 0x0100,
		Fantasy = 0x0200,
		Animation = 0x0400,
		Adventure = 0x0800,
		Western = 0x1000
	};

	class TMovie {
	private:
		std::string title;
		std::string director;
		int year;
		int genreFlags; // Bitwise combination of EGenreFlags
		float rating; // Scale from 0.0 to 10.0
	public:
		TMovie(const std::string& aTitle, const std::string& aDirector, int aYear, const std::string& aGenreFlags, float aRating);
		std::string GetTitle() const;
		std::string GetDirector() const;
		int GetYear() const;
		int GetGenreFlags() const;
		float GetRating() const;
		std::string GetGenreString() const;
		std::string ToString(const int aID) const;
	  
		static std::string GetHeaders();
	};


}