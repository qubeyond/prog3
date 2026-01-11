add_library(db_lib src/db_manager.cpp)

target_include_directories(db_lib PUBLIC 
    include 
    ${LIBPQXX_INCLUDE_DIRS}
)

target_link_libraries(db_lib PUBLIC 
    models_lib
    PostgreSQL::PostgreSQL 
    ${LIBPQXX_LIBRARIES}
)

target_link_directories(db_lib PUBLIC ${LIBPQXX_LIBRARY_DIRS})