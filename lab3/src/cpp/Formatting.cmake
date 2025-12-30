find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    # Собираем список всех файлов для форматирования
    file(GLOB_RECURSE FORMAT_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/tests/*.cpp"
    )

    # Создаем таргет 'format'
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FORMAT_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Formatting code according to .clang-format..."
        COMMAND_EXPAND_LISTS
        VERBATIM
    )

    message(STATUS "Clang-format found: ${CLANG_FORMAT_EXE}")
else()
    message(WARNING "Clang-format not found in the system!")
endif()