# "ON" = build Option 1, "OFF" = build Option 2.
option(BUILD_CATEGORY_3_OPTION_1 "Build Assignment-Category 3 Option 1 (Standard)" OFF)

add_executable(Category-3
    main.cpp
     "Option-2/TParser.h" "Option-2/TParser.cpp")

# Conditionally add the correct source file
if(BUILD_CATEGORY_3_OPTION_1)
    # If ON, add option1.cpp and define 'ASSIGNMENT_OPTION=1' for C++
    target_sources(Category-3
      PRIVATE
        "Option-1/app.cpp" 
        "Option-1/app.h"
        "Option-1/TEmployee.cpp"
        "Option-1/TEmployee.h"
        "Option-2/menu.h"
    )
    target_compile_definitions(Category-3 PRIVATE "CATEGORY_3_OPTION=1")
else()
    # If OFF, add option2.cpp and define 'ASSIGNMENT_OPTION=2' for C++
    target_sources(Category-3
      PRIVATE
        "Option-2/app.cpp" 
        "Option-2/app.h"
        "Option-2/menu.cpp"
        "Option-2/TLexer.h"
        "Option-2/TLexer.cpp"
        "Option-2/TVariable.h"
        "Option-2/TExpressionNode.h"
        "Option-2/TExpressionNode.cpp"
    )
    target_compile_definitions(Category-3 PRIVATE "CATEGORY_3_OPTION=2")
endif()

target_link_libraries(Category-3 PRIVATE StructureReader)
target_link_libraries(Category-3 PUBLIC DebugUtils)
target_link_libraries(Category-3 PUBLIC DataStructure)
target_link_libraries(Category-3 PUBLIC MiscUtils)

add_custom_command(TARGET Category-3 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Category-3 post-build step, add any additional commands here if needed..."
)
