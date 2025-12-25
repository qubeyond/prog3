#pragma once

#include <list>
#include <mutex>
#include <unordered_map>

#include "db/buffer/replacer.hpp"

namespace storage {

class LRUReplacer final : public Replacer {
   public:
    explicit LRUReplacer(size_t num_frames);
    ~LRUReplacer() override = default;

    bool victim(frame_id_t* frame_id) override;

    void pin(frame_id_t frame_id) override;

    void unpin(frame_id_t frame_id) override;

    size_t size() override;

   private:
    size_t num_frames_;
    std::list<frame_id_t> lru_list_;
    std::unordered_map<frame_id_t, std::list<frame_id_t>::iterator> map_;
    std::mutex mutex_;
};

}  // namespace storage