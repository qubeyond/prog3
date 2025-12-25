#pragma once

#include "db/storage/table_heap.hpp"

namespace storage {

class TableIterator {
   public:
    TableIterator(TableHeap* table_heap, RID rid);
    ~TableIterator();

    const Tuple& operator*() const;
    const Tuple* operator->() const;
    TableIterator& operator++();
    bool operator!=(const TableIterator& other) const;
    bool operator==(const TableIterator& other) const;

    const RID& GetRID() const {
        return rid_;
    }

   private:
    void advance();

    TableHeap* table_heap_;
    RID rid_;
    Tuple current_tuple_;
};

}  // namespace storage