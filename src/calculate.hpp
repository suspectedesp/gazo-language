#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <iostream>
#include <regex>

int calculate(const std::string& expression) {
    std::regex pattern("(-?\\d+)\\s*([-+*/])\\s*(-?\\d+)");
    std::smatch match;
    if (std::regex_match(expression, match, pattern)) {
        int operand1 = std::stoi(match[1]);
        int operand2 = std::stoi(match[3]);
        char op = match[2].str()[0];

        int result;
        switch (op) {
        case '+':
            result = operand1 + operand2;
            break;
        case '-':
            result = operand1 - operand2;
            break;
        case '*':
            result = operand1 * operand2;
            break;
        case '/':
            result = operand1 / operand2;
            break;
        default:
            std::cerr << "Error: Invalid operator." << std::endl;
            return 1;
        }

        if (result < -100 || result > 100) {
            std::cerr << "[RUNTIME ERROR]: Da Gazo aus der Volkschule gedropped ist kann er nur Rechnungen mit Ergebnissen zwischen -100 und 100 berechnen" << std::endl;
            return 1;
        }

        return result;
    }
    else {
        std::cerr << "[RUNTIME ERROR]: Da Gazo aus der Volkschule gedropped ist kann er nur bis zu zwei Zahlen auf einmal berechnen" << std::endl;
        return 1;

        /*
        std::cerr << "Error: Invalid expression: " << expression << std::endl;
        return 0;
        */
    }
}

#endif // CALCULATE_HPP