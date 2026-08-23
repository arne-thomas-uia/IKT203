# 1. Define the Source Files
set(DEBUG_SOURCES 
    
    
    DebugUtils.cpp)

# 2. Create the Library Target
add_library(DebugUtils STATIC ${DEBUG_SOURCES})

# 3. Handle Include Directories
# We point to the parent folder (Common) so users can find DebugUtils.h
target_include_directories(DebugUtils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# 4. Compiler-Specific Flags (Maintain strictness across the whole project)
if(MSVC)
    target_compile_options(DebugUtils PRIVATE /W4 /WX)
else()
    target_compile_options(DebugUtils PRIVATE -Wall -Wextra -Werror)
endif()