#pragma once

#include <map>
#include <string>
#include <vector>

#include "common.hpp"

class CLI {
   public:
    static CLI& getInstance() {
        static CLI instance;
        return instance;
    }

    bool init(int argc, char* argv[]);
    std::string runQuery();
    void shutdown();

   private:
    CLI() = default;

    void loadText();
    void saveText();

    std::string execute(const std::string& query);
    DataStructure* getOrCreate(const std::string& name, DataType type);
    std::string printStructure(const std::string& name);
    void destroyStructure(DataStructure* ds);
    void printUsage(const char* programName);

    std::string m_filename;
    std::string m_query;
    std::map<std::string, DataStructure*> m_structures;
};