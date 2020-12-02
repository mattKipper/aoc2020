//
// Created by kipper on 2020-12-01.
//
#include "cli.h"

#include <optional>
#include <iostream>
#include <exception>
#include <algorithm>

struct Input {
    std::string password;
    char letter;
    unsigned long low;
    unsigned long high;
};

std::optional<Input> from_line(const std::string& line) {
    Input input;
    size_t parsed;
    std::string line_copy(line);

    try {
        input.low = std::stoul(line_copy, &parsed);
        line_copy.erase(0, parsed + 1); // Remove min + '-'
        input.high = std::stoul(line_copy, &parsed);
        input.letter = line_copy[parsed + 1]; // Skip max + ' '
        input.password = std::string(line_copy.begin() + parsed + 4, line_copy.end());  // Skip max + ' ' + letter + ": "
    }
    catch (std::invalid_argument) {
        std::cerr << "Failed to parse input line: " << line << std::endl;
        return std::nullopt;
    }
    return input;
}

unsigned solve_puzzle(const CLIInput<std::vector<Input>>& input) {
    if (input.part == Part::ONE) {
        return std::count_if(input.data.begin(), input.data.end(), [](const Input &in) {
            auto count = std::count(in.password.begin(), in.password.end(), in.letter);
            return count >= in.low && count <= in.high;
        });
    }
    else {
        return std::count_if(input.data.begin(), input.data.end(), [](const Input& in) {
            bool low_match = in.password[in.low - 1] == in.letter;
            bool high_match = in.password[in.high - 1] == in.letter;
            return low_match != high_match;
        });
    }
}

int main(int argc, char *argv[]) {
    auto parse_inputs = [](int argc, char *argv[]) -> CLIInput<std::vector<Input>> {
        auto raw_input = parse_input_file_lines(argc, argv);
        auto parsed = CLIInput<std::vector<Input>> {};
        parsed.part = raw_input.part;

        std::transform(raw_input.data.begin(), raw_input.data.end(), std::back_inserter(parsed.data),
                       [](const std::string& s) { return *from_line(s); });

        return parsed;
    };

    return solve_cli<std::vector<Input>,unsigned>(argc, argv, parse_inputs, solve_puzzle);

}
