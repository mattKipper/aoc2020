//
// Created by kipper on 2020-12-06.
//
#include "cli.h"

#include <unordered_set>
#include <unordered_map>

int solve_puzzle_part_two(const std::vector<std::string>& input) {
    int total_count = 0;
    int group_size = 0;
    std::unordered_map<char,int> group_counts;

    auto add_group_count = [&]() {
        for (const auto& count: group_counts) {
            if (count.second == group_size) {
                ++total_count;
            }
        }
    };

    for (const auto& line: input) {
        if (line.empty()) {
            add_group_count();
            group_counts.clear();
            group_size = 0;
        }
        else {
            for (const auto& answer: line) {
                auto match = group_counts.find(answer);
                if (match == group_counts.end()) {
                    group_counts[answer] = 1;
                }
                else {
                    match->second++;
                }
            }
            group_size++;
        }
    }

    // Just in case last line isn't empty
    add_group_count();

    return total_count;
}

int solve_puzzle_part_one(const std::vector<std::string>& input) {
    int counts = 0;
    std::unordered_set<char> group_answers;

    for (const auto& line: input) {
        if (line.empty()) {
            counts += group_answers.size();
            group_answers.clear();
        }
        else {
            for (const auto& answer: line) {
                group_answers.insert(answer);
            }
        }
    }

    // Don't forget the last group (group_answers.size() == 0) if the
    // file ends with an empty line
    counts += group_answers.size();

    return counts;
}

int solve_puzzle(const CLIInput<std::vector<std::string>>& input) {
    switch (input.part) {
        case Part::ONE:
            return solve_puzzle_part_one(input.data);
        case Part::TWO:
            return solve_puzzle_part_two(input.data);
        default:
            throw std::invalid_argument("Unexpected part argument");
    }
}

int main(int argc, char *argv[]) {
    return solve_cli<std::vector<std::string>,int>(argc, argv, parse_input, solve_puzzle);
}

