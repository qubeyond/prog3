add_executable(file_classifier 
    main.cpp
)

target_link_libraries(file_classifier PRIVATE 
    db_lib 
    models_lib
)