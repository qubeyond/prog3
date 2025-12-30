#include "binary_serialization.hpp"

#include <fstream>
#include <sstream>

#include "array.hpp"
#include "cbtree.hpp"
#include "doubly_list.hpp"
#include "hashtable.hpp"
#include "queue.hpp"
#include "singly_list.hpp"
#include "stack.hpp"

void BinarySerializer::writeInt(std::ostream& os, int32_t value) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

int32_t BinarySerializer::readInt(std::istream& is) {
    int32_t value = 0;
    is.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

void BinarySerializer::writeString(std::ostream& os, const std::string& str) {
    writeInt(os, static_cast<int32_t>(str.length()));
    os.write(str.data(), str.length());
}

std::string BinarySerializer::readString(std::istream& is) {
    int32_t len = readInt(is);
    if (len <= 0) return "";
    std::string str(len, '\0');
    is.read(&str[0], len);
    return str;
}

void BinarySerializer::save(const std::string& filename,
                            const std::map<std::string, DataStructure*>& structures) {
    std::ofstream os(filename, std::ios::binary);
    if (!os.is_open()) return;

    os.write("NSTU", 4);
    writeInt(os, 1);  
    writeInt(os, static_cast<int32_t>(structures.size()));

    for (auto const& [name, ds] : structures) {
        writeString(os, name);
        writeInt(os, static_cast<int32_t>(ds->type));

        std::vector<char> buffer;
        switch (ds->type) {
            case ARRAY_TYPE:
                buffer = serializeArray(ds->data);
                break;
            case SINGLY_LIST_TYPE:
            case DOUBLY_LIST_TYPE:
                buffer = serializeList(ds->data, ds->type);
                break;
            case STACK_TYPE:
            case QUEUE_TYPE:
                buffer = serializeStackQueue(ds->data, ds->type);
                break;
            case RB_TREE_TYPE:
                buffer = serializeCBT(ds->data);
                break;
            case HASH_TABLE_TYPE:
                buffer = serializeHashTable(ds->data);
                break;
        }

        writeInt(os, static_cast<int32_t>(buffer.size()));
        if (!buffer.empty()) os.write(buffer.data(), buffer.size());
    }
}

void BinarySerializer::load(const std::string& filename,
                            std::map<std::string, DataStructure*>& structures) {
    std::ifstream is(filename, std::ios::binary);
    if (!is.is_open()) return;

    char magic[4];
    is.read(magic, 4);
    if (std::string(magic, 4) != "NSTU") return;
    readInt(is);  

    int32_t count = readInt(is);
    for (int i = 0; i < count; ++i) {
        std::string name = readString(is);
        DataType type = static_cast<DataType>(readInt(is));
        int32_t dataSize = readInt(is);

        std::vector<char> buffer(dataSize);
        if (dataSize > 0) is.read(buffer.data(), dataSize);

        DataStructure* ds = new DataStructure{type, nullptr};
        switch (type) {
            case ARRAY_TYPE:
                ds->data = new Array();
                deserializeArray(ds->data, buffer);
                break;
            case SINGLY_LIST_TYPE:
                ds->data = new SinglyList();
                deserializeList(ds->data, buffer, type);
                break;
            case DOUBLY_LIST_TYPE:
                ds->data = new DoublyList();
                deserializeList(ds->data, buffer, type);
                break;
            case STACK_TYPE:
                ds->data = new Stack();
                deserializeStackQueue(ds->data, buffer, type);
                break;
            case QUEUE_TYPE:
                ds->data = new Queue();
                deserializeStackQueue(ds->data, buffer, type);
                break;
            case RB_TREE_TYPE:
                ds->data = new CompleteBinaryTree();
                deserializeCBT(ds->data, buffer);
                break;
            case HASH_TABLE_TYPE:
                ds->data = new HashTable();
                deserializeHashTable(ds->data, buffer);
                break;
        }
        structures[name] = ds;
    }
}

std::vector<char> BinarySerializer::serializeArray(void* data) {
    auto* obj = static_cast<Array*>(data);
    std::stringstream ss(std::ios::binary | std::ios::out);
    writeInt(ss, static_cast<int32_t>(obj->getSize()));
    for (size_t i = 0; i < obj->getSize(); ++i) writeString(ss, obj->get(i));
    std::string s = ss.str();
    return std::vector<char>(s.begin(), s.end());
}

std::vector<char> BinarySerializer::serializeList(void* data, DataType type) {
    std::stringstream ss(std::ios::binary | std::ios::out);
    if (type == SINGLY_LIST_TYPE) {
        auto* obj = static_cast<SinglyList*>(data);
        writeInt(ss, static_cast<int32_t>(obj->getSize()));
        for (size_t i = 0; i < obj->getSize(); ++i) writeString(ss, obj->get(i));
    } else {
        auto* obj = static_cast<DoublyList*>(data);
        writeInt(ss, static_cast<int32_t>(obj->getSize()));
        for (size_t i = 0; i < obj->getSize(); ++i) writeString(ss, obj->get(i));
    }
    std::string s = ss.str();
    return std::vector<char>(s.begin(), s.end());
}

std::vector<char> BinarySerializer::serializeStackQueue(void* data, DataType type) {
    std::stringstream ss(std::ios::binary | std::ios::out);
    size_t size = (type == STACK_TYPE) ? static_cast<Stack*>(data)->size()
                                       : static_cast<Queue*>(data)->size();
    writeInt(ss, static_cast<int32_t>(size));
    if (type == STACK_TYPE) {
        Stack temp = *static_cast<Stack*>(data);
        std::vector<std::string> vals;
        while (!temp.empty()) vals.push_back(temp.pop());
        for (auto it = vals.rbegin(); it != vals.rend(); ++it) writeString(ss, *it);
    } else {
        Queue temp = *static_cast<Queue*>(data);
        while (!temp.empty()) writeString(ss, temp.pop());
    }
    std::string s = ss.str();
    return std::vector<char>(s.begin(), s.end());
}

std::vector<char> BinarySerializer::serializeCBT(void* data) {
    auto* tree = static_cast<CompleteBinaryTree*>(data);
    auto vec = tree->toVector();
    std::stringstream ss(std::ios::binary | std::ios::out);
    writeInt(ss, static_cast<int32_t>(vec.size()));
    for (const auto& val : vec) writeString(ss, val);
    std::string s = ss.str();
    return std::vector<char>(s.begin(), s.end());
}

std::vector<char> BinarySerializer::serializeHashTable(void* data) {
    auto* ht = static_cast<HashTable*>(data);
    std::stringstream ss(std::ios::binary | std::ios::out);

    auto entries = ht->getAll();

    writeInt(ss, static_cast<int32_t>(entries.size()));

    for (const auto& [key, value] : entries) {
        writeString(ss, key);
        writeString(ss, value);
    }

    std::string s = ss.str();
    return std::vector<char>(s.begin(), s.end());
}

void BinarySerializer::deserializeArray(void* data, const std::vector<char>& bytes) {
    if (bytes.empty()) return;
    std::stringstream ss(std::string(bytes.begin(), bytes.end()), std::ios::binary | std::ios::in);
    auto* obj = static_cast<Array*>(data);
    int32_t count = readInt(ss);
    for (int i = 0; i < count; ++i) obj->push(readString(ss));
}

void BinarySerializer::deserializeList(void* data, const std::vector<char>& bytes, DataType type) {
    if (bytes.empty()) return;
    std::stringstream ss(std::string(bytes.begin(), bytes.end()), std::ios::binary | std::ios::in);
    int32_t count = readInt(ss);
    for (int i = 0; i < count; ++i) {
        std::string val = readString(ss);
        if (type == SINGLY_LIST_TYPE)
            static_cast<SinglyList*>(data)->push_tail(val);
        else
            static_cast<DoublyList*>(data)->push_tail(val);
    }
}

void BinarySerializer::deserializeStackQueue(void* data, const std::vector<char>& bytes,
                                             DataType type) {
    if (bytes.empty()) return;
    std::stringstream ss(std::string(bytes.begin(), bytes.end()), std::ios::binary | std::ios::in);
    int32_t count = readInt(ss);
    for (int i = 0; i < count; ++i) {
        std::string val = readString(ss);
        if (type == STACK_TYPE)
            static_cast<Stack*>(data)->push(val);
        else
            static_cast<Queue*>(data)->push(val);
    }
}

void BinarySerializer::deserializeCBT(void* data, const std::vector<char>& bytes) {
    if (bytes.empty()) return;
    std::stringstream ss(std::string(bytes.begin(), bytes.end()), std::ios::binary | std::ios::in);
    auto* tree = static_cast<CompleteBinaryTree*>(data);
    int32_t count = readInt(ss);
    for (int i = 0; i < count; ++i) tree->insert(readString(ss));
}

void BinarySerializer::deserializeHashTable(void* data, const std::vector<char>& bytes) {
    if (bytes.empty()) return;
    std::stringstream ss(std::string(bytes.begin(), bytes.end()), std::ios::binary | std::ios::in);
    auto* ht = static_cast<HashTable*>(data);

    int32_t count = readInt(ss);
    for (int i = 0; i < count; ++i) {
        std::string key = readString(ss);
        std::string value = readString(ss);
        ht->insert(key, value);
    }
}