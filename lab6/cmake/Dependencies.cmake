include(FetchContent)

# 1. Находим PostgreSQL (стандартный модуль CMake)
find_package(PostgreSQL REQUIRED)

# 2. Находим libpqxx через PkgConfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

# 3. Google Test (как было раньше)
if(BUILD_TESTING)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif()