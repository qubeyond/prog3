find_package(GTest REQUIRED)
include(GoogleTest)

# Автоматически находим все файлы тестов (EvaluatorTests.cpp и т.д.)
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE lab_lib GTest::gtest_main)
target_include_directories(run_tests PRIVATE ../include)

gtest_discover_tests(run_tests)