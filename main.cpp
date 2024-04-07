#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
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
            return 0;
        }

        if (result < -100 || result > 100) {
            std::cerr << "[RUNTIME ERROR]: Da Gazo aus der Volkschule gedropped ist kann er nur Rechnungen mit Ergebnissen zwischen -100 und 100 berechnen" << std::endl;
            return 0;
        }

        return result;
    }
    else {
        std::cerr << "[RUNTIME ERROR]: Da Gazo aus der Volkschule gedropped ist kann er nur bis zu zwei Zahlen auf einmal berechnen" << std::endl;
        return 0;

        /*
        std::cerr << "Error: Invalid expression: " << expression << std::endl;
        return 0;
        */
    }
}

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

        // Check if the line contains gazo.sagen
        if (line.find("gazo.sagen") != std::string::npos) {
            // Extract the string to be outputted
            size_t startQuote = line.find("\"");
            size_t endQuote = line.find("\"", startQuote + 1);
            std::string message = line.substr(startQuote + 1, endQuote - startQuote - 1);
            std::cout << message << std::endl;
        }
        // Check if the line contains gazo.rechnen
        else if (line.find("gazo.rechnen") != std::string::npos) {
            // Extract the expression to be evaluated
            size_t openParen = line.find("(");
            size_t closeParen = line.find(")");
            std::string expression = line.substr(openParen + 1, closeParen - openParen - 1);
            int result = calculate(expression);
            std::cout << result << std::endl;
        }
        // Handle other types of lines
        else {
            std::cerr << "Error: Unknown command." << std::endl;
        }
    }

    // close input file
    inputFile.close();

    return 0;
}