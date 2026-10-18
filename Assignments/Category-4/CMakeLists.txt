# "ON" = build Option 1, "OFF" = build Option 2.
option(BUILD_CATEGORY_4_OPTION_1 "Build Assignment-Category 2 Option 1 (Standard)" ON)

add_executable(Category-4


    main.cpp
)

if(MSVC)
    set_property(TARGET Category-4 PROPERTY MSVC_DEBUG_INFORMATION_FORMAT
        "$<$<CONFIG:Debug>:EditAndContinue>")
endif()

# Conditionally add the correct source file
if(BUILD_CATEGORY_4_OPTION_1)
    # If ON, add option1.cpp and define 'ASSIGNMENT_OPTION=1' for C++
    target_sources(Category-4
      PRIVATE
        "Option-1/app.cpp"
        "Option-1/app.h"
        "Option-1/TAlgorithmState.cpp"
        "Option-1/TNetworkBuilder.cpp"
    )
    target_compile_definitions(Category-4 PRIVATE "CATEGORY_4_OPTION=1")
else()
    # If OFF, add option2.cpp and define 'ASSIGNMENT_OPTION=2' for C++
    target_sources(Category-4
      PRIVATE
        "Option-2/app.cpp"
        "Option-2/app.h"
    )
    target_compile_definitions(Category-4 PRIVATE "CATEGORY_4_OPTION=2")
endif()

target_link_libraries(Category-4
  PRIVATE
    StructureReader
    DebugUtils
    DataStructure
    MiscUtils
)

add_custom_command(TARGET Category-4 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Category-4 post-build step, add any additional commands here if needed..."
)
