include(GoogleTest)

function(create_db_test NAME SOURCES LIBS)
    add_executable(${NAME} ${SOURCES})

    target_link_libraries(${NAME} PRIVATE
        ${LIBS}
        GTest::gtest_main
    )

    target_include_directories(${NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/libs/db/include"
        "${PROJECT_SOURCE_DIR}/libs/models/include"
    )

    gtest_discover_tests(${NAME})
endfunction()