# 1. Define the Source Files
set(MISC_SOURCES 
  CalculateHash.cpp 
)

# 2. Create the Library Target
add_library(MiscUtils STATIC ${MISC_SOURCES} "CalculateHash.h")

# 3. Handle Include Directories
# We point to the parent folder (Common) so users can find MiscUtils.h
target_include_directories(MiscUtils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# 4. Compiler-Specific Flags (Maintain strictness across the whole project)
if(MSVC)
    target_compile_options(MiscUtils PRIVATE /W4 /WX)
else()
    target_compile_options(MiscUtils PRIVATE -Wall -Wextra -Werror)
endif()