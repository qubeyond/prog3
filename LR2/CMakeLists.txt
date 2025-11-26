cmake_minimum_required(VERSION 3.10)

project(LabWork2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,-clang-analyzer-*,performance-*,modernize-*,readability-*,bugprone-*,cert-*,cppcoreguidelines-*;--extra-arg-before=-xc++" CACHE STRING "Clang-Tidy checks.")
endif()


set(PROJECT_CLI_NAME cli_app)

set(SOURCE_FILES
    src/1/Stack.cpp
    src/1/Evaluator.cpp
    src/1/CLI.cpp
)


add_executable(${PROJECT_CLI_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_CLI_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/1"
)


find_program(CLANG_FORMAT_EXE NAMES clang-format)
find_program(XARGS_EXE NAMES xargs)

if(CLANG_FORMAT_EXE)
    # Используем GLOB_RECURSE для поиска всех файлов в src и test
    file(GLOB_RECURSE FORMAT_SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/test/*.cpp"
    )
endif()

if(CLANG_FORMAT_EXE AND FORMAT_SOURCES)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${FORMAT_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Formatting project sources with clang-format..."
        VERBATIM
    )
    message(STATUS "Custom target 'format' available to run clang-format.")
else()
    message(WARNING "clang-format not found or no source files found. Target 'format' skipped.")
endif()


enable_testing()

add_subdirectory(test)