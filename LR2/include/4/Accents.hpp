#pragma once
#include "4/Array.hpp"
#include "2/Set.hpp"
#include <string>

class AccentChecker {
public:
    static int countErrors(const Array<std::string>& dictionary, const Array<std::string>& text);
private:
    static bool isValidAccent(const std::string& word);
    static std::string toLower(const std::string& word);
};