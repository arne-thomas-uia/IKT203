#include "../DebugUtils.h"

#ifdef _WIN32
#include <windows.h>
// Static block to initialize Windows Console colors immediately
namespace {
  struct WindowsColorInitializer {
    WindowsColorInitializer() {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
          dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
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

  const char* TLogger::GetColorCode(ELevel aLevel) {
    switch (aLevel) {
    case ELevel::Error:       return "\033[31m"; // Red
    case ELevel::Warning:     return "\033[33m"; // Yellow
    case ELevel::Information: return "\033[36m"; // Cyan
    case ELevel::Verbose:     return "\033[37m"; // White
    default:                 return "\033[0m";
    }
  }

  const char* TLogger::GetLevelName(ELevel aLevel) {
    switch (aLevel) {
    case ELevel::Error:       return "!";
    case ELevel::Warning:     return "W";
    case ELevel::Information: return "i";
    case ELevel::Verbose:     return "v";
    default:                 return " ";
    }
  }
}

void debugSetLevel(Debug::ELevel aLevel) {
  Debug::TLogger::SetLevel(aLevel);
}
