cmake_minimum_required(VERSION 3.15)

project(DBProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy") 
endif()

# ---- Core project files ----

file(GLOB_RECURSE CORE_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*/*.cpp"
)

list(REMOVE_ITEM CORE_SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

# ---- Common library ----

file(GLOB_RECURSE COMMON_SOURCES
    "${PROJECT_SOURCE_DIR}/src/common/*.cpp"
)

add_library(common STATIC ${COMMON_SOURCES})
target_include_directories(common PUBLIC "${PROJECT_SOURCE_DIR}/include")

# ---- Core library ----

add_library(db_core STATIC ${CORE_SOURCES})
target_include_directories(db_core PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(db_core PUBLIC common)

# ---- Main executable ----

add_executable(db_exec src/main.cpp)
target_link_libraries(db_exec PRIVATE db_core common)

# ---- clang-format target ----

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE FORMAT_SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/*/*.hpp"
        "${PROJECT_SOURCE_DIR}/test/*.cpp"
    )

    set(FORMAT_FILE_LIST ${CMAKE_CURRENT_BINARY_DIR}/format_files.txt)

    string(REPLACE ";" "\n" FORMAT_SOURCES_NL "${FORMAT_SOURCES}")

    file(WRITE ${FORMAT_FILE_LIST} "${FORMAT_SOURCES_NL}")

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i @${FORMAT_FILE_LIST}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Formatting sources."
        VERBATIM
    )
endif()

# ---- Unit tests ----

enable_testing()
find_package(GTest REQUIRED)

add_subdirectory(test)