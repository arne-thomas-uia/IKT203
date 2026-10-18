#pragma once

#include <format>
#include <string>
#include <string_view>
#include <utility>

namespace Debug {
	enum class ELevel {
		None = 0,
		Error = 1,
		Warning = 2,
		Information = 3,
		Verbose = 4
	};

	class TLogger {
	public:
		static void SetLevel(ELevel aLevel);
		static ELevel GetLevel();

		template<typename... Args>
		static void Log(ELevel aLevel, std::string_view aFormat, Args&&... aArgs) {
			if (aLevel == ELevel::None || aLevel > GetLevel()) return;
			Write(aLevel, std::vformat(aFormat, std::make_format_args(aArgs...)));
		}

	private:
		static void Write(ELevel aLevel, std::string_view aMessage);
		static ELevel& GetThreshold();
		static const char* GetColorCode(ELevel aLevel);
		static const char* GetLevelName(ELevel aLevel);
	};

	template<typename... Args>
	void LogInfo(std::string_view aFormat, Args&&... aArgs) {
		TLogger::Log(ELevel::Information, aFormat, std::forward<Args>(aArgs)...);
	}

	template<typename... Args>
	void LogWarning(std::string_view aFormat, Args&&... aArgs) {
		TLogger::Log(ELevel::Warning, aFormat, std::forward<Args>(aArgs)...);
	}

	template<typename... Args>
	void LogError(std::string_view aFormat, Args&&... aArgs) {
		TLogger::Log(ELevel::Error, aFormat, std::forward<Args>(aArgs)...);
	}

	template<typename... Args>
	void LogVerbose(std::string_view aFormat, Args&&... aArgs) {
		TLogger::Log(ELevel::Verbose, aFormat, std::forward<Args>(aArgs)...);
	}
}

void debugSetLevel(Debug::ELevel aLevel);

template<typename... Args>
void debugText(Debug::ELevel aLevel, std::string_view aFormat, Args&&... aArgs) {
	Debug::TLogger::Log(aLevel, aFormat, std::forward<Args>(aArgs)...);
}
