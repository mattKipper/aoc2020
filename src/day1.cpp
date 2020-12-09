//
// Created by kipper on 2020-11-30.
//

#include "common/cli.h"

#include <algorithm>
#include <numeric>
#include <string>

bool solve_impl(const std::vector<int>& input, std::vector<int>& results, size_t start, size_t terms, size_t sum) {

    if (results.size() < terms - 1) {
        size_t i = start;
        while (i < input.size() && std::accumulate(results.begin(), results.end(), 0) < sum) {
            results.push_back(input[i]);
            if (solve_impl(input, results, i + 1, terms, sum)) {
                return true;
            }
            results.pop_back();
            ++i;
        }
        return false;
    }
    else {
        for (size_t i = start; i < input.size(); ++i) {
            results.push_back(input[i]);
            auto possible_sum = std::accumulate(results.begin(), results.end(), 0);
            if (possible_sum == sum) {
                return true;
            }
            else {
                results.pop_back();
                if (possible_sum > sum) {
                    return false;
                }
            }
        }
        return false;
    }
}

int solve_puzzle(const CLIInput<std::vector<int>>& input) {

    const size_t count = input.part == Part::ONE ? 2 : 3;
    const size_t sum = 2020;

    std::vector<int> sorted(input.data);
    std::sort(sorted.begin(), sorted.end());

    std::vector<int> results;
    results.reserve(count);

    solve_impl(sorted, results, 0, count, sum);

    return std::accumulate(results.begin(), results.end(), 1, [](const int& a, const int& b) { return a * b; });
}

int main(int argc, char *argv[]) {
    auto from_str = [](const std::string& s) {
        return std::stoi(s);
    };

    auto parse_input_nums = [from_str](int argc, char *argv[]) {
        return parse_input<int>(argc, argv, from_str);
    };

    return solve_cli<std::vector<int>,int>(argc, argv, parse_input_nums, solve_puzzle);
}

