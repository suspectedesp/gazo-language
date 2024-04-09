#ifndef PROCESS_MESSAGE_HPP
#define PROCESS_MESSAGE_HPP

#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include "calculate.hpp"

extern std::map<std::string, int> variables;

void processMessage(const std::string& message) {
    // Check if message starts with "gazo.rechnen"
    if (message.find("gazo.rechnen") == 0) {
        size_t openParen = message.find("(");
        size_t closeParen = message.find(")");
        std::string expression = message.substr(openParen + 1, closeParen - openParen - 1);
        std::string modifiedExpression = "";
        std::string currentVarName = "";
        for (char c : expression) {
            if (std::isalpha(c)) {
                currentVarName += c;
            }
            else {
                if (!currentVarName.empty()) {
                    // Replace variable name with its value
                    int varValue = variables[currentVarName];
                    modifiedExpression += std::to_string(varValue);
                    currentVarName = ""; // Reset variable name
                }
                modifiedExpression += c; // Include operators and other characters
            }
        }
        if (!currentVarName.empty()) {
            // If there's a variable name left without an operator after it
            int varValue = variables[currentVarName];
            modifiedExpression += std::to_string(varValue);
        }
        int result = calculate(modifiedExpression);
        if (result != 1) {
            std::cout << result << std::endl;
        }
    }
    else {
        // Print the message as it is
        std::cout << message << std::endl;
    }
}

#endif // PROCESS_MESSAGE_HPP