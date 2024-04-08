#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include "calculate.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3 || std::string(argv[1]) != "--f") {
        std::cerr << "Usage: " << argv[0] << " --f input_file.gazo" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[2]);
    if (!inputFile) {
        std::cerr << "Error: Couldn't open input file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
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
                // std::cout << expression << std::endl; // aka the expresion (whatever)
                int result = calculate(expression);
                if (result != 1) {
                    // std::cout << result << std::endl; // not sure if result should get printed or just stored as variable so you can then use it later on in the code
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
                std::cout << result << std::endl;
            }
        }
        // Handle other types of lines
        else {
            std::cerr << "[RUNTIME ERROR]: Gazo hat keine Ahnung was du von ihm willst" << std::endl;
        }
    }

    // close input file
    inputFile.close();

    return 0;
}