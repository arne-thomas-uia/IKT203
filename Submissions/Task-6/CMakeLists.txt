# 1. Define the executable name
# This is what the students will run (e.g., Task-6.exe)
add_executable(Task-6 
    
    
    main.cpp)

# 2. Link the Common Library
# This command connects the executable to your StructureReader code.
# Because we used 'PUBLIC' in the reader's setup, Task-6 
# automatically knows where to find "StructureReaderLib.h".
target_link_libraries(Task-6 PRIVATE StructureReader)

# 3. Handle the Data Files (The "Structure" folder)
# Since the .txt files are in the /Structure folder, we need to make sure
# the executable can find them when it runs.
# This helper command copies the 'Structure' folder into the Build directory.
add_custom_command(TARGET Task-6 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Structure"
        "$<TARGET_FILE_DIR:Task-6>/Structure"
    COMMENT "Copying data files to the build folder for Task-6..."
)