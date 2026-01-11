#pragma once
#include <optional>
#include <string>
#include <vector>

namespace classifier::models {

struct FileType {
    int id;
    std::string extension;
    std::string description;
};

struct FileRecord {
    int id;
    std::string filename;
    long long size;
    int type_id;
};

struct MLModel {
    int id;
    std::string name;
    std::string version;
};

struct FeatureSet {
    int id;
    int file_id;
    std::vector<double> features;
};

struct ClassificationResult {
    int id;
    int file_id;
    int model_id;
    std::string label;
    double confidence;
};

}  // namespace classifier::models