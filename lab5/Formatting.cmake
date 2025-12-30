find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE FORMAT_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FORMAT_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Ecosystem: Formatting code according to .clang-format..."
        COMMAND_EXPAND_LISTS
        VERBATIM
    )

    message(STATUS "Clang-format found: ${CLANG_FORMAT_EXE}")
else()
    message(WARNING "Clang-format not found!")
endif()