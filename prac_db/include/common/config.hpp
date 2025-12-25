#pragma once
#include <string>
#include <unordered_map>

namespace common {

class Config {
   public:
    static Config& instance();

    void load(const std::string& path);

    std::string getString(const std::string& key, const std::string& def = "");
    int getInt(const std::string& key, int def = 0);
    bool getBool(const std::string& key, bool def = false);

    std::string getProjectRoot() const;

   private:
    Config() = default;
    std::unordered_map<std::string, std::string> values_;
};

}  // namespace common