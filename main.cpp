#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include "src/calculate.hpp"
#include "src/file_writing.hpp"

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
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // 187 EOL
        if (line.substr(line.size() - 3) != "187") {
            std::cerr << "[Gazo-Compiler Error] | Jede Zeile (ausser leere Zeilen) muss mit 187 als EOL enden" << std::endl;
            return 1;
        }

        if (line.find("gazo.sagen") != std::string::npos) {
            // Extract string to be outputted
            size_t startQuote = line.find("\"");
            size_t endQuote = line.find("\"", startQuote + 1);
            std::string message = line.substr(startQuote + 1, endQuote - startQuote - 1);

            // Check if the message starts with "gazo.rechnen"
            if (message.find("gazo.rechnen") == 0) {
                // previous bug: used line instead of message after this line, dont repeat stupidness
                size_t openParen = message.find("(");
                size_t closeParen = message.find(")");
                std::string expression = message.substr(openParen + 1, closeParen - openParen - 1);
                // std::cout << message << std::endl; // aka gazo.rechnen(whatever)
                // std::cout << expression << std::endl; // aka the expresion (->whatever)
                int result = calculate(expression);
                if (result != 1) {
                    std::cout << result << std::endl;
                }
            }
            else {
                // Print the message as it is
                std::cout << message << std::endl;
            }
        }
        // Check if the line contains gazo.rechnen
        else if (line.find("gazo.rechnen") != std::string::npos) {
            // Extract the expression to be evaluated
            size_t openParen = line.find("(");
            size_t closeParen = line.find(")");
            std::string expression = line.substr(openParen + 1, closeParen - openParen - 1);
            int result = calculate(expression);
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

        else {
            std::cerr << "[RUNTIME ERROR]: Gazo hat keine Ahnung was du von ihm willst" << std::endl;
        }
    }

    // close input file
    inputFile.close();

    return 0;
}