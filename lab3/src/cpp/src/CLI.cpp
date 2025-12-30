#include "CLI.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "array.hpp"
#include "binary_serialization.hpp"
#include "cbtree.hpp"
#include "doubly_list.hpp"
#include "hashtable.hpp"
#include "queue.hpp"
#include "singly_list.hpp"
#include "stack.hpp"

namespace fs = std::filesystem;

bool CLI::init(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc)
            m_filename = argv[++i];
        else if (arg == "--query" && i + 1 < argc)
            m_query = argv[++i];
    }

    if (m_filename.empty() || m_query.empty()) {
        printUsage(argv[0]);
        return false;
    }

    if (fs::exists(m_filename)) {
        if (fs::path(m_filename).extension() == ".bin") {
            BinarySerializer::load(m_filename, m_structures);
        } else {
            loadText();
        }
    }
    return true;
}

std::string CLI::runQuery() {
    std::string result = execute(m_query);
    if (fs::path(m_filename).extension() == ".bin") {
        BinarySerializer::save(m_filename, m_structures);
    } else {
        saveText();
    }
    return result;
}

std::string CLI::execute(const std::string& query) {
    std::istringstream iss(query);
    std::string command, name;
    if (!(iss >> command >> name)) return "Error: Invalid command format";

    if (command == "PRINT") return printStructure(name);

    char typeChar = command[0];
    DataType type;
    if (typeChar == 'M')
        type = ARRAY_TYPE;
    else if (typeChar == 'F')
        type = SINGLY_LIST_TYPE;
    else if (typeChar == 'L')
        type = DOUBLY_LIST_TYPE;
    else if (typeChar == 'S')
        type = STACK_TYPE;
    else if (typeChar == 'Q')
        type = QUEUE_TYPE;
    else if (typeChar == 'T')
        type = RB_TREE_TYPE;
    else if (typeChar == 'H')
        type = HASH_TABLE_TYPE;
    else
        return "Error: Unknown command type";

    DataStructure* ds = getOrCreate(name, type);

    if (type == ARRAY_TYPE) {
        auto* obj = static_cast<Array*>(ds->data);
        if (command == "MPUSH") {
            std::string v;
            iss >> v;
            obj->push(v);
            return "OK";
        }
        if (command == "MINSERT") {
            size_t i;
            std::string v;
            iss >> i >> v;
            obj->insert(i, v);
            return "OK";
        }
        if (command == "MGET") {
            size_t i;
            iss >> i;
            return obj->get(i);
        }
        if (command == "MSET") {
            size_t i;
            std::string v;
            iss >> i >> v;
            obj->set(i, v);
            return "OK";
        }
        if (command == "MDEL") {
            size_t i;
            iss >> i;
            obj->remove(i);
            return "OK";
        }
        if (command == "MLEN") {
            return std::to_string(obj->getSize());
        }
    } else if (type == SINGLY_LIST_TYPE) {
        auto* obj = static_cast<SinglyList*>(ds->data);
        if (command == "FPUSH_HEAD") {
            std::string v;
            iss >> v;
            obj->push_head(v);
            return "OK";
        }
        if (command == "FPUSH_TAIL") {
            std::string v;
            iss >> v;
            obj->push_tail(v);
            return "OK";
        }
        if (command == "FDEL_HEAD") {
            obj->del_head();
            return "OK";
        }
        if (command == "FDEL_TAIL") {
            obj->del_tail();
            return "OK";
        }
        if (command == "FGET") {
            size_t i;
            iss >> i;
            return obj->get(i);
        }
    } else if (type == DOUBLY_LIST_TYPE) {
        auto* obj = static_cast<DoublyList*>(ds->data);
        if (command == "LPUSH_HEAD") {
            std::string v;
            iss >> v;
            obj->push_head(v);
            return "OK";
        }
        if (command == "LPUSH_TAIL") {
            std::string v;
            iss >> v;
            obj->push_tail(v);
            return "OK";
        }
        if (command == "LDEL_HEAD") {
            obj->del_head();
            return "OK";
        }
        if (command == "LDEL_TAIL") {
            obj->del_tail();
            return "OK";
        }
        if (command == "LGET") {
            size_t i;
            iss >> i;
            return obj->get(i);
        }
    } else if (type == STACK_TYPE) {
        auto* obj = static_cast<Stack*>(ds->data);
        if (command == "SPUSH") {
            std::string v;
            iss >> v;
            obj->push(v);
            return "OK";
        }
        if (command == "SPOP") {
            return obj->pop();
        }
    } else if (type == QUEUE_TYPE) {
        auto* obj = static_cast<Queue*>(ds->data);
        if (command == "QPUSH") {
            std::string v;
            iss >> v;
            obj->push(v);
            return "OK";
        }
        if (command == "QPOP") {
            return obj->pop();
        }
    } else if (type == RB_TREE_TYPE) {
        auto* obj = static_cast<CompleteBinaryTree*>(ds->data);
        if (command == "TINSERT") {
            std::string v;
            iss >> v;
            obj->insert(v);
            return "OK";
        }
        if (command == "TSEARCH") {
            std::string v;
            iss >> v;
            return obj->search(v) ? "Found" : "Not found";
        }
    } else if (type == HASH_TABLE_TYPE) {
        auto* obj = static_cast<HashTable*>(ds->data);
        if (command == "HSET") {
            std::string k, v;
            iss >> k >> v;
            obj->insert(k, v);
            return "OK";
        }
        if (command == "HGET") {
            std::string k;
            iss >> k;
            return obj->get(k);
        }
        if (command == "HDEL") {
            std::string k;
            iss >> k;
            obj->remove(k);
            return "OK";
        }
    }

    return "Error: Command not implemented";
}

DataStructure* CLI::getOrCreate(const std::string& name, DataType type) {
    if (m_structures.count(name)) return m_structures[name];
    DataStructure* ds = new DataStructure;
    ds->type = type;
    switch (type) {
        case ARRAY_TYPE:
            ds->data = new Array();
            break;
        case SINGLY_LIST_TYPE:
            ds->data = new SinglyList();
            break;
        case DOUBLY_LIST_TYPE:
            ds->data = new DoublyList();
            break;
        case STACK_TYPE:
            ds->data = new Stack();
            break;
        case QUEUE_TYPE:
            ds->data = new Queue();
            break;
        case RB_TREE_TYPE:
            ds->data = new CompleteBinaryTree();
            break;
        case HASH_TABLE_TYPE:
            ds->data = new HashTable();
            break;
    }
    m_structures[name] = ds;
    return ds;
}

std::string CLI::printStructure(const std::string& name) {
    if (!m_structures.count(name)) return "Error: Not found";
    DataStructure* ds = m_structures[name];
    switch (ds->type) {
        case ARRAY_TYPE:
            return static_cast<Array*>(ds->data)->print();
        case SINGLY_LIST_TYPE:
            return static_cast<SinglyList*>(ds->data)->print();
        case DOUBLY_LIST_TYPE:
            return static_cast<DoublyList*>(ds->data)->print();
        case STACK_TYPE:
            return static_cast<Stack*>(ds->data)->print();
        case QUEUE_TYPE:
            return static_cast<Queue*>(ds->data)->print();
        case RB_TREE_TYPE:
            return static_cast<CompleteBinaryTree*>(ds->data)->print();
        case HASH_TABLE_TYPE:
            return static_cast<HashTable*>(ds->data)->print();
        default:
            return "Error: Unknown type";
    }
}

void CLI::printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " --file <filename> --query '<query>'" << std::endl;
}

void CLI::loadText() {
    std::ifstream file(m_filename);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        execute(line);
    }
}

void CLI::saveText() {
    std::ofstream file(m_filename);
    if (!file.is_open()) return;

    for (auto const& [name, ds] : m_structures) {
        if (!ds->data) continue;

        switch (ds->type) {
            case ARRAY_TYPE: {
                auto* obj = static_cast<Array*>(ds->data);
                for (size_t i = 0; i < obj->getSize(); ++i)
                    file << "MPUSH " << name << " " << obj->get(i) << "\n";
                break;
            }
            case SINGLY_LIST_TYPE: {
                auto* obj = static_cast<SinglyList*>(ds->data);
                for (size_t i = 0; i < obj->getSize(); ++i)
                    file << "FPUSH_TAIL " << name << " " << obj->get(i) << "\n";
                break;
            }
            case DOUBLY_LIST_TYPE: {
                auto* obj = static_cast<DoublyList*>(ds->data);
                for (size_t i = 0; i < obj->getSize(); ++i)
                    file << "LPUSH_TAIL " << name << " " << obj->get(i) << "\n";
                break;
            }
            case STACK_TYPE: {
                // Чтобы сохранить порядок стека, создаем временную копию
                auto* obj = static_cast<Stack*>(ds->data);
                Stack temp = *obj;
                std::vector<std::string> elements;
                while (!temp.empty()) elements.push_back(temp.pop());
                // Записываем в файл с конца (от дна к вершине), чтобы SPUSH восстановил стек
                for (auto it = elements.rbegin(); it != elements.rend(); ++it)
                    file << "SPUSH " << name << " " << *it << "\n";
                break;
            }
            case QUEUE_TYPE: {
                auto* obj = static_cast<Queue*>(ds->data);
                Queue temp = *obj;
                while (!temp.empty())
                    file << "QPUSH " << name << " " << temp.pop() << "\n";
                break;
            }
            case RB_TREE_TYPE: {
                auto* obj = static_cast<CompleteBinaryTree*>(ds->data);
                auto vec = obj->toVector();
                for (const auto& val : vec)
                    file << "TINSERT " << name << " " << val << "\n";
                break;
            }
            case HASH_TABLE_TYPE: {
                auto* obj = static_cast<HashTable*>(ds->data);
                auto entries = obj->getAll(); // Используем уже готовый метод из hpp
                for (const auto& entry : entries)
                    file << "HSET " << name << " " << entry.first << " " << entry.second << "\n";
                break;
            }
        }
    }
}

void CLI::shutdown() {
    for (auto& [name, ds] : m_structures) {
        switch (ds->type) {
            case ARRAY_TYPE:
                delete static_cast<Array*>(ds->data);
                break;
            case SINGLY_LIST_TYPE:
                delete static_cast<SinglyList*>(ds->data);
                break;
            case DOUBLY_LIST_TYPE:
                delete static_cast<DoublyList*>(ds->data);
                break;
            case STACK_TYPE:
                delete static_cast<Stack*>(ds->data);
                break;
            case QUEUE_TYPE:
                delete static_cast<Queue*>(ds->data);
                break;
            case RB_TREE_TYPE:
                delete static_cast<CompleteBinaryTree*>(ds->data);
                break;
            case HASH_TABLE_TYPE:
                delete static_cast<HashTable*>(ds->data);
                break;
        }
        delete ds;
    }
    m_structures.clear();
}