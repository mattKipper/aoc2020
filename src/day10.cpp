//
// Created by kipper on 2020-12-10.
//
#include "cli.h"

#include <set>
#include <unordered_map>

int solve_part_two(std::set<int>& input) {
    std::unordered_map<int, unsigned long long> paths;
    paths[0] = 1;
    for(auto curr = std::next(input.begin()); curr != input.end(); ++curr) {
    }

    return 0;
}

int solve_part_one(std::set<int>& input) {
    input.insert(0);
    input.insert(*input.rbegin() + 3);

    auto one_diff_count = 0;
    auto three_diff_count = 0;

    auto curr = input.begin();
    auto next = std::next(curr);
    while (next != input.end()) {
        auto diff = *next - *curr;
        if (diff == 1) {
            ++one_diff_count;
        }
        else if (diff == 3) {
            ++three_diff_count;
        }
        ++curr; ++next;
    }

    return one_diff_count * three_diff_count;
}

int solve_puzzle(CLIInput<std::set<int>>& input) {
    switch (input.part) {
        case Part::ONE:
            return solve_part_one(input.data);
        case Part::TWO:
            return solve_part_two(input.data);
        default:
            return -1;
    }
}

CLIInput<std::set<int>> parse_input_nums(int argc, char *argv[]) {
    auto args = parse_arguments(argc, argv);

    CLIInput<std::set<int>> result;
    result.part = args.part;

    std::ifstream input_file(args.data);
    std::string line;

    while (std::getline(input_file, line)) {
        result.data.insert(std::stoi(line));
    }

    return result;
}

int main(int argc, char *argv[]) {
    return solve_cli<std::set<int>,int>(argc, argv, parse_input_nums, solve_puzzle);
}

