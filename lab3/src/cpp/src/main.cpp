#include <iostream>
#include <string>

#include "CLI.hpp"

int main(int argc, char* argv[]) {
    CLI& cli = CLI::getInstance();

    try {
        if (!cli.init(argc, argv)) {
            return 1;
        }
        std::string result = cli.runQuery();
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        cli.shutdown();
        return 1;
    }

    cli.shutdown();
    return 0;
}