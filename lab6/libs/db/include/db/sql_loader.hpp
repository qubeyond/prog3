#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace classifier::db {

class SqlLoader {
   public:
    static std::string load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open SQL file: " + filename);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

}  // namespace classifier::db