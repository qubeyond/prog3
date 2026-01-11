find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_FORMAT_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/libs/**/*.cpp"
        "${PROJECT_SOURCE_DIR}/libs/**/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
        "${PROJECT_SOURCE_DIR}/tests/**/*.cpp"
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ALL_FORMAT_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "CLANG: Formatting code to match .clang-format style..."
        VERBATIM
    )

    message(STATUS "Clang-format found: ${CLANG_FORMAT_EXE}")
else()
    message(WARNING "Clang-format not found! Automatic formatting will be unavailable.")
endif()