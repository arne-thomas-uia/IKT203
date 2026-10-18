add_library(DebugUtils STATIC DebugUtils.cpp)

target_include_directories(DebugUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

if(MSVC)
    target_compile_options(DebugUtils PRIVATE /W4 /WX)
else()
    target_compile_options(DebugUtils PRIVATE -Wall -Wextra -Werror)
endif()
