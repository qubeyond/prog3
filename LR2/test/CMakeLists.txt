find_package(GTest REQUIRED)


add_executable(run_tests test_main.cpp) 

target_include_directories(run_tests PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

target_sources(run_tests PRIVATE
    1/EvaluatorTests.cpp
    
    ${PROJECT_SOURCE_DIR}/src/1/Stack.cpp
    ${PROJECT_SOURCE_DIR}/src/1/Evaluator.cpp
)

target_link_libraries(run_tests PRIVATE
    GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(run_tests)