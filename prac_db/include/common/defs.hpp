#pragma once

#include <cstddef>

namespace common::defs {

inline constexpr size_t PAGE_SIZE = 4096;
inline constexpr size_t DEFAULT_BUF_SIZE = 1024;

inline constexpr const char* DEFAULT_DB_PATH = "data/";

inline constexpr int METADATA_PAGE_ID = 0;

}  // namespace common::defs