cmake_minimum_required(VERSION 3.10)
project(LabWork2 CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE LIB_SOURCES "src/*/*.cpp")
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*CLI\\.cpp$")

add_library(lab_lib STATIC ${LIB_SOURCES})
target_include_directories(lab_lib PUBLIC include)

# Задание 1
add_executable(task1.out src/1/CLI.cpp)
target_link_libraries(task1.out PRIVATE lab_lib)

# Задание 2
add_executable(task2.out src/2/CLI.cpp)
target_link_libraries(task2.out PRIVATE lab_lib)

# Задание 3
add_executable(task3.out src/3/CLI.cpp)
target_link_libraries(task3.out PRIVATE lab_lib)

# Задание 4
add_executable(task4.out src/4/CLI.cpp)
target_link_libraries(task4.out PRIVATE lab_lib)

# Задание 5
add_executable(task5.out src/5/CLI.cpp)
target_link_libraries(task5.out PRIVATE lab_lib)

# Задание 6
add_executable(task6.out src/6/CLI.cpp)
target_link_libraries(task6.out PRIVATE lab_lib)

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE FORMAT_SOURCES "src/*.cpp" "include/*.hpp" "test/*.cpp")
    add_custom_target(format COMMAND ${CLANG_FORMAT_EXE} -i ${FORMAT_SOURCES})
endif()

enable_testing()
add_subdirectory(test)