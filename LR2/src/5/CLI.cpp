#include <iostream>
#include <string>
#include "5/Huffman.hpp"

int main() {
    std::string text;
    if (std::getline(std::cin, text)) {
        Huffman::compress(text);
    }
    return 0;
}