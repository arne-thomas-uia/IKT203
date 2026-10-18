#include "DebugUtils.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>

namespace {
  struct WindowsColorInitializer {
    WindowsColorInitializer() {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
          dwMode |= 0x0004;
          SetConsoleMode(hOut, dwMode);
        }
      }
    }
  } g_colorInit;
}
#endif

namespace Debug {
  void TLogger::SetLevel(ELevel aLevel) { GetThreshold() = aLevel; }
  ELevel TLogger::GetLevel() { return GetThreshold(); }

  ELevel& TLogger::GetThreshold() {
    static ELevel threshold = ELevel::Information;
    return threshold;
  }

  void TLogger::Write(ELevel aLevel, std::string_view aMessage) {
    std::cout << GetColorCode(aLevel)
              << "[" << GetLevelName(aLevel) << "] "
              << aMessage
              << "\033[0m" << std::endl;
  }

  const char* TLogger::GetColorCode(ELevel aLevel) {
    switch (aLevel) {
    case ELevel::Error:       return "\033[31m";
    case ELevel::Warning:     return "\033[33m";
    case ELevel::Information: return "\033[36m";
    case ELevel::Verbose:     return "\033[37m";
    default:                  return "\033[0m";
    }
  }

  const char* TLogger::GetLevelName(ELevel aLevel) {
    switch (aLevel) {
    case ELevel::Error:       return "!";
    case ELevel::Warning:     return "W";
    case ELevel::Information: return "i";
    case ELevel::Verbose:     return "v";
    default:                  return " ";
    }
  }
}

void debugSetLevel(Debug::ELevel aLevel) {
  Debug::TLogger::SetLevel(aLevel);
}
