#pragma once
#include <map>
#include <string>

enum DataType {
    ARRAY_TYPE,
    SINGLY_LIST_TYPE,
    DOUBLY_LIST_TYPE,
    STACK_TYPE,
    QUEUE_TYPE,
    RB_TREE_TYPE,
    HASH_TABLE_TYPE
};

struct DataStructure {
    DataType type;
    void* data;
};

extern std::map<std::string, DataStructure*> structures;