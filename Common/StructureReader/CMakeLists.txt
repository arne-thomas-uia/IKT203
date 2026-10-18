# 1. Define the Source Files
# We list the .cpp files that need to be compiled into the library.
set(READER_SOURCES
    
    
    GetRecordCount.cpp
    ReadGraph.cpp
    ReadMovies.cpp
    ReadNames.cpp
    ReadSongs.cpp
)

# 2. Create the Library Target
# We name it 'StructureReader' so students can link to it by name.
add_library(StructureReader STATIC ${READER_SOURCES})

# 3. Handle Include Directories
# This is the "Magic" step. 
# 'PUBLIC' means anyone who links to this library also gets these include paths.
target_include_directories(StructureReader PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}           # For GetRecordCount.h
    ${CMAKE_CURRENT_SOURCE_DIR}/..        # For StructureReader.h (one level up)
)

# 4. Compiler-Specific Flags
# Ensuring MSVC (Visual Studio) treats the code correctly for C++26.
if(MSVC)
    target_compile_options(StructureReader PRIVATE /W4 /WX) # Level 4 warnings as errors
else()
    target_compile_options(StructureReader PRIVATE -Wall -Wextra -Werror)
endif()

# 5. Define the STRUCTURE_DIR Macro, using escaped quotes to be Linux friendly
# Using PUBLIC so the macro propagates to Task-1, Task-2, etc.
target_compile_definitions(StructureReader PUBLIC STRUCTURE_DIR=\"${STRUCTURE_PATH}\")