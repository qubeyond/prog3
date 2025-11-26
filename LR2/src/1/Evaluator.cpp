#include "1/Evaluator.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "1/Stack.hpp"

auto applyBinaryOp(int operand_a, int operand_b, const std::string& operation) -> int
{
    // Явное преобразование bool в int для логических операций
    if (operation == "&") {
        return static_cast<int>(operand_a != 0 && operand_b != 0);
    }
    if (operation == "|") {
        return static_cast<int>(operand_a != 0 || operand_b != 0);
    }
    if (operation == "^") {
        return static_cast<int>((operand_a != 0) != (operand_b != 0));
    }
    throw std::runtime_error("Unknown binary operator: " + operation);
}

auto applyUnaryOp(int operand, const std::string& operation) -> int
{
    // Явное преобразование bool в int для логических операций
    if (operation == "!") {
        return static_cast<int>(!operand);
    }
    throw std::runtime_error("Unknown unary operator: " + operation);
}

auto shuntingYard(const std::string& infix) -> std::vector<Token>
{
    Stack opStack;
    initStack(opStack);
    std::vector<Token> output;

    std::stringstream expression_stream(infix);
    std::string current_token_value;

    while (expression_stream >> current_token_value) {
        Token current_token;
        current_token.value = current_token_value;

        if (current_token.value == "0" || current_token.value == "1") {
            current_token.type = Token::OPERAND;
            current_token.logical_value = std::stoi(current_token.value);
            output.push_back(current_token);

        } else if (current_token.value == "(") {
            current_token.type = Token::PARENTHESIS;
            push(opStack, current_token);

        } else if (current_token.value == ")") {
            Token popped;
            while (!isEmpty(opStack) && top(opStack).value != "(") {
                pop(opStack, popped);
                output.push_back(popped);
            }
            if (isEmpty(opStack)) {
                // Исключение для непарной закрывающей скобки
                throw std::runtime_error("Mismatched parenthesis: unmatched ')'");
            } else {
                pop(opStack, popped);
            }

        } else {
            current_token.type = Token::OPERATOR;
            initializeTokenProperties(current_token);

            while (!isEmpty(opStack) && top(opStack).value != "(") {
                const Token& topOp = top(opStack);

                if (topOp.precedence >= current_token.precedence) {
                    Token poppedOp;
                    pop(opStack, poppedOp);
                    output.push_back(poppedOp);
                } else {
                    break;
                }
            }
            push(opStack, current_token);
        }
    }

    Token remainingOp;
    while (!isEmpty(opStack)) {
        pop(opStack, remainingOp);
        if (remainingOp.value == "(") {  // Только открывающие скобки здесь
            // Исключение для непарной открывающей скобки
            throw std::runtime_error("Mismatched parenthesis: unmatched '('");
        }
        output.push_back(remainingOp);
    }

    clearStack(opStack);
    return output;
}

auto evaluateRpn(const std::vector<Token>& rpn) -> int
{
    Stack valStack;
    initStack(valStack);

    for (const auto& token : rpn) {
        if (token.type == Token::OPERAND) {
            push(valStack, token);
        } else if (token.type == Token::OPERATOR) {
            if (token.is_unary) {
                Token a_token;
                if (!pop(valStack, a_token)) {
                    throw std::runtime_error("RPN ERROR: Missing operand for unary op.");
                }

                a_token.logical_value = applyUnaryOp(a_token.logical_value, token.value);
                push(valStack, a_token);

            } else {
                Token b_token;  // Объявление токенов по отдельности
                Token a_token;

                if (!pop(valStack, b_token) || !pop(valStack, a_token)) {
                    throw std::runtime_error("RPN ERROR: Missing operand for binary op.");
                }

                int result =
                    applyBinaryOp(a_token.logical_value, b_token.logical_value, token.value);

                a_token.logical_value = result;
                push(valStack, a_token);
            }
        }
    }

    Token finalResult;
    if (pop(valStack, finalResult) && isEmpty(valStack)) {
        clearStack(valStack);
        return finalResult.logical_value;
    }

    clearStack(valStack);
    throw std::runtime_error("RPN ERROR: Invalid expression structure.");
}

auto evaluateExpression(const std::string& infix_expression) -> int
{
    try {
        std::vector<Token> rpn = shuntingYard(infix_expression);
        return evaluateRpn(rpn);
    } catch (const std::exception& e) {
        std::cerr << "Evaluation failed: " << e.what() << "\n";
        return -1;
    }
}