#pragma once

#include <cstdint>
#include <string>

#include "db/catalog/type.hpp"

namespace catalog {

class Column {
   public:
    Column(std::string name, TypeId type, uint32_t offset) : name_(name), type_(type), offset_(offset) {
    }

    std::string getName() const {
        return name_;
    }
    TypeId getType() const {
        return type_;
    }
    uint32_t getOffset() const {
        return offset_;
    }

   private:
    std::string name_;
    TypeId type_;
    uint32_t offset_;
};

}  // namespace catalog