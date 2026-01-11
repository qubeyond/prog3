cmake_minimum_required(VERSION 3.14)
project(file_classifier_db CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(BUILD_TESTING "Build tests" ON)

include(Dependencies)
include(Formatting)

add_subdirectory(libs/models) 
add_subdirectory(libs/db)
add_subdirectory(src)

if(BUILD_TESTING)
    enable_testing()
    include(Tests)
    add_subdirectory(tests)
endif()