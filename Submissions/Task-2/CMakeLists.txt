# 1. Define the executable name
# This is what the students will run (e.g., Task-2.exe)
add_executable(Task-2 main.cpp    )

# 2. Link the Common Library
# This command connects the executable to your StructureReader code.
# Because we used 'PUBLIC' in the reader's setup, Task-2 
# automatically knows where to find "StructureReaderLib.h".
target_link_libraries(Task-2 PRIVATE StructureReader DebugUtils)