#pragma once

#include <vector>

#include "db/catalog/column.hpp"

namespace catalog {

class Schema {
   public:
    explicit Schema(const std::vector<Column>& columns) : columns_(columns) {
    }
    const std::vector<Column>& getColumns() const {
        return columns_;
    }

   private:
    std::vector<Column> columns_;
};

}  // namespace catalog