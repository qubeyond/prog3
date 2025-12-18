#include "4/Accents.hpp"
#include <cctype>

std::string AccentChecker::toLower(const std::string& word) {
    std::string res = word;
    for (char& c : res) c = (char)std::tolower((unsigned char)c);
    return res;
}

bool AccentChecker::isValidAccent(const std::string& word) {
    int count = 0;
    for (char c : word) {
        if (std::isupper((unsigned char)c)) count++;
    }
    return count == 1;
}

int AccentChecker::countErrors(const Array<std::string>& dictionary, const Array<std::string>& text) {
    Set dictExact(200003);
    Set dictLower(200003);

    for (size_t i = 0; i < dictionary.size(); ++i) {
        dictExact.add(dictionary[i]);
        dictLower.add(toLower(dictionary[i]));
    }

    int errors = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        const std::string& word = text[i];
        std::string lower = toLower(word);

        if (dictLower.contains(lower)) {
            if (!dictExact.contains(word)) {
                errors++;
            }
        } else {
            if (!isValidAccent(word)) {
                errors++;
            }
        }
    }
    return errors;
}