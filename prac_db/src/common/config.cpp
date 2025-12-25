#include "common/config.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace common {

static std::string project_root_path;

Config& Config::instance() {
    static Config cfg;

    if (project_root_path.empty()) {
#ifdef PROJECT_SOURCE_DIR
        project_root_path = PROJECT_SOURCE_DIR;
#else
        project_root_path = (std::filesystem::current_path() / "..").string();
#endif
    }

    return cfg;
}

void Config::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);

        values_[key] = val;
    }
}

std::string Config::getString(const std::string& key, const std::string& def) {
    if (values_.count(key))
        return values_[key];
    return def;
}

int Config::getInt(const std::string& key, int def) {
    if (!values_.count(key))
        return def;
    return std::stoi(values_[key]);
}

bool Config::getBool(const std::string& key, bool def) {
    if (!values_.count(key))
        return def;
    return values_[key] == "1" || values_[key] == "true";
}

std::string Config::getProjectRoot() const {
    return project_root_path;
}

}  // namespace common
