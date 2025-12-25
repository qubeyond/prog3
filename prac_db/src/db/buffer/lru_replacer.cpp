#include "db/buffer/lru_replacer.hpp"

namespace storage {

LRUReplacer::LRUReplacer(size_t num_frames) : num_frames_(num_frames) {
}

bool LRUReplacer::victim(frame_id_t* frame_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (lru_list_.empty()) {
        return false;
    }

    *frame_id = lru_list_.front();

    lru_list_.pop_front();
    map_.erase(*frame_id);

    return true;
}

void LRUReplacer::pin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (map_.count(frame_id)) {
        lru_list_.erase(map_.at(frame_id));
        map_.erase(frame_id);
    }
}

void LRUReplacer::unpin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (map_.count(frame_id)) {
        lru_list_.erase(map_.at(frame_id));
        map_.erase(frame_id);
    }

    lru_list_.push_back(frame_id);

    map_[frame_id] = std::prev(lru_list_.end());
}

size_t LRUReplacer::size() {
    std::lock_guard<std::mutex> lock(mutex_);
    return lru_list_.size();
}

}  // namespace storage