# 1. Define the executable name
# This is what the students will run (e.g., Task-3.exe)
add_executable(Task-3 main.cpp)

# 2. Link the Common Library
# This command connects the executable to your StructureReader code.
# Because we used 'PUBLIC' in the reader's setup, Task-3 
# automatically knows where to find "StructureReaderLib.h".
target_link_libraries(Task-3 PRIVATE StructureReader)

# 3. Handle the Data Files (The "Structure" folder) use excaped quotes to be Linux friendly
target_compile_definitions(Task-3 PRIVATE STRUCTURE_DIR=\"${STRUCTURE_PATH}\")