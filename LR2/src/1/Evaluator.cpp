#include "1/Evaluator.hpp"
#include "1/Stack.hpp"
#include <vector>
#include <stdexcept>

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    for (size_t i = 0; i < input.length(); ++i) {
        if (isspace(input[i])) continue;
        Token t;
        t.value = input[i];
        if (input[i] == '0' || input[i] == '1') {
            t.type = Token::OPERAND;
            t.logical_value = input[i] - '0';
        } else if (input[i] == '(' || input[i] == ')') {
            t.type = Token::PARENTHESIS;
        } else {
            t.type = Token::OPERATOR;
            initializeTokenProperties(t);
        }
        tokens.push_back(t);
    }
    return tokens;
}

int evaluateRpn(const std::vector<Token>& rpn) {
    Stack valStack;
    initStack(valStack, 100);
    for (auto& t : rpn) {
        if (t.type == Token::OPERAND) push(valStack, t);
        else {
            if (t.is_unary) {
                Token a; if (!pop(valStack, a)) throw std::runtime_error("Err");
                a.logical_value = (t.value == "!" ? !a.logical_value : a.logical_value);
                push(valStack, a);
            } else {
                Token b, a; if (!pop(valStack, b) || !pop(valStack, a)) throw std::runtime_error("Err");
                int res = 0;
                if (t.value == "&") res = a.logical_value && b.logical_value;
                else if (t.value == "|") res = a.logical_value || b.logical_value;
                else if (t.value == "^") res = a.logical_value != b.logical_value;
                a.logical_value = res;
                push(valStack, a);
            }
        }
    }
    Token res; pop(valStack, res); return res.logical_value;
}

int evaluateExpression(const std::string& infix) {
    auto tokens = tokenize(infix);
    Stack opStack; initStack(opStack, 100);
    std::vector<Token> rpn;
    for (auto& t : tokens) {
        if (t.type == Token::OPERAND) rpn.push_back(t);
        else if (t.value == "(") push(opStack, t);
        else if (t.value == ")") {
            Token topT;
            while (!isEmpty(opStack) && top(opStack).value != "(") { pop(opStack, topT); rpn.push_back(topT); }
            pop(opStack, topT);
        } else {
            while (!isEmpty(opStack) && top(opStack).value != "(" && top(opStack).precedence >= t.precedence) {
                Token topT; pop(opStack, topT); rpn.push_back(topT);
            }
            push(opStack, t);
        }
    }
    Token topT;
    while (!isEmpty(opStack)) { pop(opStack, topT); rpn.push_back(topT); }
    return evaluateRpn(rpn);
}