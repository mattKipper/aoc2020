//
// Created by kipper on 2020-12-12.
//
#include "cli.h"

#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

enum class State {
    EMPTY,
    OCCUPIED,
    UNAVAILABLE
};
using Row = std::vector<State>;
using Grid = std::vector<Row>;

State from_char(char c) {
    switch (c) {
        case 'L':
            return State::EMPTY;
        case '#':
            return State::OCCUPIED;
        case '.':
        default:
            return State::UNAVAILABLE;
    }
}

Row from_line(const std::string& line) {
    Row states;
    std::transform(line.begin(), line.end(), std::back_inserter(states), from_char);
    return states;
}


State next(const Grid& g, size_t row, size_t col, size_t rows, size_t cols) {
    auto s = g[row][col];
    if (s == State::UNAVAILABLE) {
        return State::UNAVAILABLE;
    }
    else {
        auto occ = [](State s) { return s == State::OCCUPIED; };
        bool top = row == 0;
        bool bottom = row == rows - 1;
        bool left = col == 0;
        bool right = col == cols - 1;

        int adj_count = 0;
        if (!top) {
            adj_count += occ(g[row-1][col]) ? 1 : 0;
            adj_count += (!left && occ(g[row-1][col-1])) ? 1 : 0;
            adj_count += (!right && occ(g[row-1][col+1])) ? 1 : 0;
        }
        if (!bottom) {
            adj_count += occ(g[row+1][col]) ? 1 : 0;
            adj_count += (!left && occ(g[row+1][col-1])) ? 1 : 0;
            adj_count += (!right && occ(g[row+1][col+1])) ? 1 : 0;
        }
        adj_count += (!left && occ(g[row][col-1])) ? 1 : 0;
        adj_count += (!right && occ(g[row][col+1])) ? 1 : 0;

        if (s == State::EMPTY && adj_count == 0) {
            return State::OCCUPIED;
        }
        else if (s == State::OCCUPIED && adj_count >= 4) {
            return State::EMPTY;
        }
        else {
            return s;
        }
    }
}

bool update(Grid& original) {
    const size_t rows = original.size();
    const size_t cols = original[0].size();

    Grid copy(original);
    bool changed = false;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            copy[row][col] = next(original, row, col, rows, cols);
            if (copy[row][col] != original[row][col]) {
                changed = true;
            }
        }
    }
    original = std::move(copy);
    return changed;
}

int solve_part_one(Grid& input) {
    unsigned iters = 0;
    while(update(input)) {
        ++iters;
    }
    return std::accumulate(input.begin(), input.end(), 0, [](int count, const Row& row) {
        return count + std::count_if(row.begin(), row.end(), [](const State& s) { return s == State::OCCUPIED; });
    });
}

int solve_puzzle(CLIInput<Grid>& input) {
    switch (input.part) {
        case Part::ONE:
            return solve_part_one(input.data);
        case Part::TWO:
        default:
            return -1;
    }
}

int main(int argc, char *argv[]) {
    auto parse_input_states = [](int argc, char *argv[]) {
        return parse_input<Row>(argc, argv, from_line);
    };
    return solve_cli<Grid,int>(argc, argv, parse_input_states, solve_puzzle);
}

