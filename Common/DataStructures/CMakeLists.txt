# 1. We don't need a SOURCES variable for an Interface library 
# because nothing is being compiled.

# 2. Create the Library Target as INTERFACE
add_library(DataStructure INTERFACE)

# 3. Handle Include Directories
# For INTERFACE libraries, we use 'INTERFACE' instead of 'PUBLIC'
target_include_directories(DataStructure INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# 4. Compiler-Specific Flags 
# For Interface libraries, use 'INTERFACE' to pass flags to consumers
if(MSVC)
    target_compile_options(DataStructure INTERFACE /W4 /WX)
else()
    target_compile_options(DataStructure INTERFACE -Wall -Wextra -Werror)
endif()