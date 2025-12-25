#pragma once

#include <cstddef>

#include "db/storage/page.hpp"

namespace storage {

class Replacer {
   public:
    virtual ~Replacer() = default;

    virtual bool victim(frame_id_t* frame_id) = 0;

    virtual void pin(frame_id_t frame_id) = 0;
    virtual void unpin(frame_id_t frame_id) = 0;

    virtual size_t size() = 0;
};

}  // namespace storage