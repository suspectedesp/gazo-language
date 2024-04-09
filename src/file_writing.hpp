#ifndef FILE_WRITING_HPP
#define FILE_WRITING_HPP

int read(const std::string& file_name, int lines_to_read = -1) {
    std::ifstream inputFile(file_name);
    if (!inputFile) {
        std::cerr << "[Gazo-Compiler Error] | Couldn't open input file" << std::endl;
        return 1;
    }

    int lines_read = 0;
    std::string line;
    while (std::getline(inputFile, line) && (lines_to_read == -1 || lines_read < lines_to_read)) {
        if (line.empty()) {
            continue;
        }

        std::cout << file_name + ": " << line << std::endl;

        lines_read++;
    }

    inputFile.close();
    return 0;
}


#endif // FILE_WIRITNG_HPP