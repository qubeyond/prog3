#include <iostream>
#include <sstream>
#include <string>

#include "1/Evaluator.hpp"

void processCommand(const std::string &commandLine)
{
    std::stringstream command_stream(commandLine);
    std::string command;
    command_stream >> command;

    if (command == "EVAL") {
        std::string expression_part;
        std::string expression;

        std::getline(command_stream, expression, '\0');

        if (!expression.empty() && expression[0] == ' ') {
            expression.erase(0, 1);
        }

        if (expression.empty()) {
            std::cout << "Usage: EVAL <logical expression>\n";
            return;
        }

        std::cout << "Evaluating: " << expression << "\n";
        int result = evaluateExpression(expression);

        if (result != -1) {
            std::cout << "Result: " << result << " (Boolean: " << (result == 1 ? "True" : "False")
                      << ")\n";
        } else {
            std::cout << "Evaluation failed due to malformed expression.\n";
        }
        return;
    }

    std::cout << "Unknown command: " << command << "\n";
    std::cout << "Available commands: EVAL, EXIT\n";
}

int main()
{
    std::cout << "Logical Expression Evaluator CLI (using custom Stack).\n"
              << "Commands:\n"
              << " \tEVAL <expr> \t- evaluate expression (e.g., EVAL ( 1 & ! 0 ) ^ 1)\n"
              << " \tEXIT \t\t- quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "EXIT") {
            break;
        }

        if (!line.empty()) {
            processCommand(line);
        }
    }

    return 0;
}