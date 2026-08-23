#pragma once
#include <string>
#include <string_view>
#include <format>
#include <iostream>

namespace Debug {
  enum class ELevel {
    None = 0,
    Error = 1,      // Red
    Warning = 2,    // Yellow
    Information = 3,// Cyan
    Verbose = 4     // White
  };

  // Global controller for the library
  class TLogger {
  public:
    static void SetLevel(ELevel aLevel);
    static ELevel GetLevel();

    // The core formatting function
    template<typename... Args>
    static void Log(ELevel aLevel, std::string_view aFormat, Args&&... aArgs);

  private:
    static ELevel& GetThreshold();
    static const char* GetColorCode(ELevel aLevel);
    static const char* GetLevelName(ELevel aLevel);
  };
}

namespace Debug {
  template<typename... Args>
  void TLogger::Log(ELevel aLevel, std::string_view aFormat, Args&&... aArgs) {
    if (aLevel > GetThreshold() || aLevel == ELevel::None) return;

    // Apply color based on level
    std::cout << GetColorCode(aLevel);
    std::cout << "[" << GetLevelName(aLevel) << "] ";

    // Format the user string
    std::cout << std::vformat(aFormat, std::make_format_args(aArgs...));

    // Reset color and end line
    std::cout << "\033[0m" << std::endl;
  }
}

void debugSetLevel(Debug::ELevel aLevel);

// Simple macro or helper for shorter syntax
template<typename... Args>
void debugText(Debug::ELevel aLevel, std::string_view aFormat, Args&&... aArgs) {
  Debug::TLogger::Log(aLevel, aFormat, std::forward<Args>(aArgs)...);
}
