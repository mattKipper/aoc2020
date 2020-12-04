//
// Created by kipper on 2020-12-03.
//
#include "cli.h"

#include <numeric>

unsigned solve_slope(const std::vector<std::string>& input, size_t rise, size_t run) {
    unsigned count = 0;
    size_t col = 0;
    for (size_t row = 0; row < input.size(); row += rise) {
        if (input[row][col] == '#') {
            ++count;
        }
        col = (col + run) % input[row].size();
    }
    return count;
}

unsigned solve_puzzle(const CLIInput<std::vector<std::string>>& input) {
    switch (input.part) {
        case Part::ONE:
            return solve_slope(input.data, 1, 3);
        case Part::TWO:
            return solve_slope(input.data, 1, 1) *
                   solve_slope(input.data, 1, 3) *
                   solve_slope(input.data, 1, 5) *
                   solve_slope(input.data, 1, 7) *
                   solve_slope(input.data, 2, 1);
    }
}

int main(int argc, char *argv[]) {
    return solve_cli<std::vector<std::string>,unsigned>(argc, argv, parse_input_file_lines, solve_puzzle);
}
