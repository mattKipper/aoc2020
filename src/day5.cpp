//
// Created by kipper on 2020-12-05.
//
#include "cli.h"

int seat_id(int row, int column) {
    return (row * 8) + column;
}

int partition(const char* seat, int length, int chars, char lower, char higher) {
    int low = 0, high = length;
    for (size_t i = 0; i < chars; ++i) {
        auto mid = low + (high - low) / 2;
        if (seat[i] == lower) {
            high = mid;
        }
        else if (seat[i] == higher) {
            low = mid;
        }
        else {
            return -1;
        }
    }
    return low + (high - low) / 2;

}

int row(const std::string& seat) {
    return partition(seat.c_str(), 128, 7, 'F', 'B');
}

int column(const std::string& seat) {
    return partition(seat.c_str() + 7, 8, 3, 'L', 'R');
}

std::vector<bool> occupancy_list(int rows, int columns) {
    return std::vector<bool>(rows * columns, false);
}

int solve_puzzle(const CLIInput<std::vector<std::string>>& input) {
    if (input.part == Part::ONE) {
        int max_id = 0;
        for (const auto &seat: input.data) {
            auto id = seat_id(row(seat), column(seat));
            if (id > max_id) {
                max_id = id;
            }
        }
        return max_id;
    }
    else {
        auto occupied = occupancy_list(128, 8);
        for (const auto& seat: input.data) {
            occupied[row(seat) * 8 + column(seat)] = true;
        }
        for (int i = 1; i < occupied.size(); ++i) {
            if (!occupied[i] && occupied[i-1]) {
                return seat_id(i / 8, i % 8);
            }
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
    return solve_cli<std::vector<std::string>,int>(argc, argv, parse_input, solve_puzzle);
}

