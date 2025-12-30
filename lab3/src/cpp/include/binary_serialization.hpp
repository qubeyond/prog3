#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "common.hpp"

class BinarySerializer {
   public:
    static void save(const std::string& filename,
                     const std::map<std::string, DataStructure*>& structures);
    static void load(const std::string& filename,
                     std::map<std::string, DataStructure*>& structures);

   private:
    static void writeInt(std::ostream& os, int32_t value);
    static int32_t readInt(std::istream& is);
    static void writeString(std::ostream& os, const std::string& str);
    static std::string readString(std::istream& is);

    static std::vector<char> serializeArray(void* data);
    static std::vector<char> serializeList(void* data, DataType type);
    static std::vector<char> serializeStackQueue(void* data, DataType type);
    static std::vector<char> serializeCBT(void* data);
    static std::vector<char> serializeHashTable(void* data);

    static void deserializeArray(void* data, const std::vector<char>& bytes);
    static void deserializeList(void* data, const std::vector<char>& bytes, DataType type);
    static void deserializeStackQueue(void* data, const std::vector<char>& bytes, DataType type);
    static void deserializeCBT(void* data, const std::vector<char>& bytes);
    static void deserializeHashTable(void* data, const std::vector<char>& bytes);
};