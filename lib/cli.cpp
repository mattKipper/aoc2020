#include "cli.h"

#include <sstream>
#include <fstream>
#include <iostream>

CLIInput<std::vector<std::string>> parse_input_file_lines(int argc, char *argv[]) {

    CLIInput<std::vector<std::string>> result;

    char* input_filename;

    if (argc > 3) {
        throw std::invalid_argument("Too many arguments provided.");
    }
    else if (argc < 2) {
        throw std::invalid_argument("At least one argument must be provided.");
    }
    else if (argc == 3) {
        if (std::string(argv[1]) != "-2") {
            throw std::invalid_argument("Only supported flag is '-2' for part 2 of puzzle.");
        }
        else {
            input_filename = argv[2];
            result.part_2 = true;
        }
    }
    else {  // argc == 2
        input_filename = argv[1];
        result.part_2 = false;
    }

    std::ifstream input_file(input_filename);
    std::string line;

    while (std::getline(input_file, line)) {
        result.data.push_back(line);
    }

    return result;
}


