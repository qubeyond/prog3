cmake_minimum_required(VERSION 3.16)
project(ProgStructures LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(linkedlist_lib src/b/LinkedList.cpp)
target_include_directories(linkedlist_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_executable(linkedlist_cli src/b/LinkedListCLI.cpp)
target_link_libraries(linkedlist_cli linkedlist_lib)
add_executable(test_linkedlist tests/b/LinkedListTests.cpp)
target_link_libraries(test_linkedlist linkedlist_lib)

add_library(array_lib src/a/Array.cpp)
target_include_directories(array_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_executable(array_cli src/a/ArrayCLI.cpp)
target_link_libraries(array_cli array_lib)
add_executable(test_array tests/a/ArrayTests.cpp)
target_link_libraries(test_array array_lib)

add_library(dlinkedlist_lib src/c/DoublyLinkedList.cpp)
target_include_directories(dlinkedlist_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_executable(dlinkedlist_cli src/c/DoublyLinkedListCLI.cpp)
target_link_libraries(dlinkedlist_cli dlinkedlist_lib)
add_executable(test_dlinkedlist tests/c/DoublyLinkedListTests.cpp)
target_link_libraries(test_dlinkedlist dlinkedlist_lib)

add_library(stack_lib src/d/Stack.cpp)
target_include_directories(stack_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_executable(stack_cli src/d/StackCLI.cpp)
target_link_libraries(stack_cli stack_lib)
add_executable(test_stack tests/d/StackTests.cpp)
target_link_libraries(test_stack stack_lib)

add_library(queue_lib src/e/Queue.cpp)
target_include_directories(queue_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_executable(queue_cli src/e/QueueCLI.cpp)
target_link_libraries(queue_cli queue_lib)
add_executable(test_queue tests/e/QueueTests.cpp)
target_link_libraries(test_queue queue_lib)

add_library(rbtree_lib src/f/RedBlackTree.cpp)
target_include_directories(rbtree_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_executable(rbtree_cli src/f/RedBlackTreeCLI.cpp)
target_link_libraries(rbtree_cli rbtree_lib)
add_executable(test_rbtree tests/f/RedBlackTreeTests.cpp)
target_link_libraries(test_rbtree rbtree_lib)

add_custom_target(run_all_tests
    COMMAND $<TARGET_FILE:test_linkedlist>
    COMMAND $<TARGET_FILE:test_array>
    COMMAND $<TARGET_FILE:test_dlinkedlist>
    COMMAND $<TARGET_FILE:test_stack>
    COMMAND $<TARGET_FILE:test_queue>
    COMMAND $<TARGET_FILE:test_rbtree>
    DEPENDS test_linkedlist test_array test_dlinkedlist test_stack test_queue test_rbtree
    COMMENT "Running all tests for all data structures..."
)
