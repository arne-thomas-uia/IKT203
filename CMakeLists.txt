# (CMake 3.28+ fully supports C++23). However, the latestversion is now at 4.X.X
cmake_minimum_required(VERSION 3.28)

# Define the project name and enable the C++ language (CXX).
project(IKT203_Algorithms_DataStructure LANGUAGES CXX)

# Enforce C++23 globally.
# Because we set this here, CMake AUTOMATICALLY adds /std:c++latest for MSVC 
# and -std=c++2b / -std=c++23 for GCC/Clang. We do not need to add them manually!
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific optimizations and quality-of-life flags
if(MSVC)
    # Fixes MSVC's notoriously inaccurate __cplusplus macro
    add_compile_options("/Zc:__cplusplus")
    # Enable multi-processor compilation for faster builds
    add_compile_options(/MP)
    # Enable parallel PDB writes
    add_compile_options(/FS)
    # Stops MSVC complaining about standard C functions like 'fopen' or 'strcpy'
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# 1. Define a variable for the original Structure folder path
set(STRUCTURE_ORG_PATH "${CMAKE_SOURCE_DIR}/Structure")
# 2. Copy the Structure folder to the build directory so the executables can read the .txt files
file(COPY "${STRUCTURE_ORG_PATH}" DESTINATION "${CMAKE_BINARY_DIR}")
# 3. Define a variable that points to the copied Structure folder in the build directory
set(STRUCTURE_PATH "${CMAKE_BINARY_DIR}/Structure")

# Orchestrate the subdirectories. 
# CMake will step into each of these folders and process their own CMakeLists.txt files.
add_subdirectory(Common)
add_subdirectory(Assignments)
add_subdirectory(Submissions)