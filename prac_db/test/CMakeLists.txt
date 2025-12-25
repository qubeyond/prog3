file(GLOB_RECURSE TEST_SOURCES
    "${PROJECT_SOURCE_DIR}/test/*.cpp"
    "${PROJECT_SOURCE_DIR}/test/*/*.cpp"
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE db_core common GTest::gtest_main)

gtest_discover_tests(run_tests)