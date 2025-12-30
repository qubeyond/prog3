cmake_minimum_required(VERSION 3.15)
project(Lab4_Concurrency LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

include(Formatting.cmake)

add_executable(lab4_app ${SOURCES})
target_include_directories(lab4_app PUBLIC include)
target_link_libraries(lab4_app PRIVATE Threads::Threads)