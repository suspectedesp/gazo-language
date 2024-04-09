#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include <unordered_map>
#include "src/calculate.hpp"
#include "src/file_writing.hpp"
#include "src/process_message.hpp"
std::map<std::string, int> variables;

int evaluateExpression(const std::string& expression) {
    std::string exp = expression;
    for (const auto& pair : variables) {
        size_t pos;
        while ((pos = exp.find(pair.first)) != std::string::npos) {
            exp.replace(pos, pair.first.length(), std::to_string(pair.second));
        }
    }
    return calculate(exp);
}

int main(int argc, char* argv[]) {
    if (argc != 3 || std::string(argv[1]) != "--f") {
        std::cerr << "[Gazo-Compiler Error] | Usage: " << argv[0] << " --f input_file.gazo" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[2]);
    if (!inputFile) {
        std::cerr << "[Gazo-Compiler Error] | Couldn't open input file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Skip empty lines or lines starting with //
        if (line.empty() || line.substr(0, 2) == "//") {
            continue;
        }

        // 187 EOL
        if (line.substr(line.size() - 3) != "187") {
            std::cerr << "[Gazo-Compiler Error] | Jede Zeile (ausser leere Zeilen) muss mit 187 als EOL enden" << std::endl;
            return 1;
        }

        if (line.find("gazo.sagen") != std::string::npos) {
            // Extract string
            size_t startQuote = line.find("\"");
            size_t endQuote = line.find("\"", startQuote + 1);

            // Check if message has double quotes
            std::string message;
            if (startQuote != std::string::npos && endQuote != std::string::npos) {
                message = line.substr(startQuote + 1, endQuote - startQuote - 1);
            }
            else {
                size_t startSingleQuote = line.find("'");
                size_t endSingleQuote = line.find("'", startSingleQuote + 1);
                if (startSingleQuote != std::string::npos && endSingleQuote != std::string::npos) {
                    std::string varName = line.substr(startSingleQuote + 1, endSingleQuote - startSingleQuote - 1);
                    if (variables.find(varName) != variables.end()) {
                        message = std::to_string(variables[varName]);
                    }
                    else {
                        std::cerr << "Variable '" << varName << "' not found." << std::endl;
                        continue; // Skip processing
                    }
                }
                else {
                    std::cerr << "Invalid syntax in gazo.sagen command." << std::endl;
                    continue; // Skip processing
                }
            }

            if (message.find("gazo.rechnen") == 0) {
                processMessage(message);
            }
            else {
                std::cout << message << std::endl;
            }
        }
        // Check if it contains gazo.rechnen
        else if (line.find("gazo.rechnen") != std::string::npos) {
            size_t openParen = line.find("(");
            size_t closeParen = line.find(")");
            std::string expression = line.substr(openParen + 1, closeParen - openParen - 1);

            // Modify expression | replace variable names with their values
            std::string modifiedExpression = "";
            std::string currentVarName = "";
            for (char c : expression) {
                if (std::isalpha(c)) {
                    currentVarName += c; // Build variable name
                }
                else {
                    if (!currentVarName.empty()) {
                        // Replace variable name with its value
                        int varValue = variables[currentVarName];
                        modifiedExpression += std::to_string(varValue);
                        currentVarName = ""; // Reset variable name
                    }
                    modifiedExpression += c; // Include operators and other chars
                }
            }
            if (!currentVarName.empty()) {
                // If there's a variable name left without an operator after it
                int varValue = variables[currentVarName];
                modifiedExpression += std::to_string(varValue);
            }

            int result = calculate(modifiedExpression);
            if (result != 1) {
                // std::cout << result << std::endl; // not sure if result should get printed or just stored as variable so you can then use it later on in the code
            }
        }

        else if (line.find("gazo.lesen") != std::string::npos) {
            size_t openParen = line.find("(");
            size_t commaPos = line.find(",");
            size_t closeParen = line.find(")");

            // Check if necessary separators found
            if (openParen == std::string::npos || commaPos == std::string::npos || closeParen == std::string::npos) {
                std::cerr << "[Gazo-Compiler Error] | Invalid syntax in gazo.lesen command" << std::endl;
                return 1;
            }

            // Extracting arguments
            std::string argument = line.substr(openParen + 1, commaPos - openParen - 1);
            std::string file_name = line.substr(commaPos + 3, closeParen - commaPos - 4);

            int lines_to_read = -1; // Default = read all lines
            if (!argument.empty()) {
                argument.erase(std::remove(argument.begin(), argument.end(), '\"'), argument.end());
                if (std::regex_match(argument, std::regex("\\d+"))) {
                    lines_to_read = std::stoi(argument);
                }
                else {
                    std::cerr << "[Gazo-Compiler Error] | Invalid argument for number of lines to read" << std::endl;
                    return 1;
                }
            }

            read(file_name, lines_to_read);
        }
        else if (line.find("gazo.var") != std::string::npos) {
            size_t quoteStart = line.find("\"");
            size_t quoteEnd = line.find("\"", quoteStart + 1);
            if (quoteStart == std::string::npos || quoteEnd == std::string::npos || quoteEnd <= quoteStart) {
                std::cerr << "[Gazo-Compiler Error] | Invalid syntax in gazo.var command" << std::endl;
                return 1;
            }
            size_t equalPos = line.find("=");
            std::string varName = line.substr(9, equalPos - 10); // Adjusted here
            std::string value = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
            variables[varName] = std::stoi(value);
            // std::cout << "Variable value: " << variables[varName] << std::endl;
        }

        else {
            std::cerr << "[RUNTIME ERROR]: Gazo hat keine Ahnung was du von ihm willst" << std::endl;
        }
    }

    // close input file
    inputFile.close();

    return 0;
}