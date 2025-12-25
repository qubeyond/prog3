#include <filesystem>

#include "common/config.hpp"
#include "common/defs.hpp"

int main() {
    auto& cfg = common::Config::instance();
    cfg.load("../db.conf");

    return 0;
}