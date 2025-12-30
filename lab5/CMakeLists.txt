cmake_minimum_required(VERSION 3.15)
project(Lab5_Ecosystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Собираем все файлы
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Подключаем модуль форматирования
include(Formatting.cmake)

# Создаем исполняемый файл
add_executable(ecosystem_app ${SOURCES})

# Указываем путь к заголовкам
target_include_directories(ecosystem_app PUBLIC include)