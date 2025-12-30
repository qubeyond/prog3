cmake_minimum_required(VERSION 3.10)
project(LabWork1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Formatting.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/Formatting.cmake")
else()
    message(WARNING "Formatting.cmake not found at ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

file(GLOB_RECURSE LIB_SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(FILTER LIB_SRCS EXCLUDE REGEX "src/main\\.cpp$")

add_library(ds_lib STATIC ${LIB_SRCS})

add_executable(main_app src/main.cpp)
target_link_libraries(main_app PRIVATE ds_lib)

enable_testing()
find_package(GTest QUIET)

if(GTest_FOUND)
    file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
    add_executable(run_tests ${TEST_SOURCES})
    target_link_libraries(run_tests PRIVATE ds_lib GTest::gtest GTest::gtest_main)
    add_test(NAME AllTests COMMAND run_tests)

    find_program(LCOV_EXE lcov)
    find_program(GENHTML_EXE genhtml)

    if(LCOV_EXE AND GENHTML_EXE)
        add_custom_target(coverage_report
            # 1. Собираем все данные
            COMMAND ${LCOV_EXE} --directory . --capture --output-file coverage.info --ignore-errors mismatch,gcov
            
            # 2. Удаляем лишнее: системные файлы, тесты, main, cli и сериализацию
            COMMAND ${LCOV_EXE} --remove coverage.info 
                '/usr/*' 
                '*/tests/*' 
                '*/googletest/*' 
                '*/src/main.cpp' 
                '*/src/CLI.cpp' 
                '*/src/binary_serialization.cpp' 
                --output-file coverage.info --ignore-errors unused,mismatch
            
            # 3. Генерируем HTML
            COMMAND ${GENHTML_EXE} coverage.info --output-directory out_coverage --ignore-errors mismatch
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating cleaned HTML coverage report..."
        )
    endif()
endif()