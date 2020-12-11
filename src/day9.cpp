//
// Created by kipper on 2020-12-10.
//
#include "cli.h"

#include <vector>
#include <set>
#include <algorithm>

class Queue {
public:
    Queue(std::vector<long long>::const_iterator seed, size_t len) {
        for(size_t i = 0; i < len; ++i) {
            seq_.push_back(*seed);
            ord_.insert(*seed++);
        }
    }

    bool insert_if_valid(long long n) {
        auto larger = std::next(ord_.begin());
        while(larger != ord_.end() && *larger <= n) {
            auto smaller = ord_.begin();
            while (smaller != larger && *smaller + *larger <= n) {
                if (*smaller + *larger == n) {
                    auto oldest = seq_.front();
                    seq_.pop_front();
                    ord_.erase(oldest);

                    seq_.push_back(n);
                    ord_.insert(n);

                    return true;
                }
                ++smaller;
            }
            ++larger;
        }
        return false;
    }

private:
    std::list<long long> seq_;
    std::set<long long> ord_;
};

long long solve_part_one(const std::vector<long long>& input, int len) {
    auto n = input.cbegin();
    Queue q(n, len);

    n += len;
    while(n != input.cend() && q.insert_if_valid(*n)) {
        ++n;
    }

    if (n == input.cend()) {
        return -1;
    }
    return *n;
}

long long solve_part_two(const std::vector<long long>& input, long long target) {
    auto lower = input.cbegin();
    auto upper = std::next(lower);

    long long sum = *lower + *upper;
    while (upper != input.end()) {
        if (sum == target) {
            return *std::max_element(lower, std::next(upper)) + *std::min_element(lower, std::next(upper));
        }
        else if (sum < target) {
            ++upper;
            sum += *upper;
        }
        else {
            ++lower;
            upper = std::next(lower);
            sum = *lower + *upper;
        }
    }
    return -1;
}

long long solve_puzzle(const CLIInput<std::vector<long long>>& input) {
    auto part_one_solution = solve_part_one(input.data, 25);
    if (input.part == Part::ONE) {
        return part_one_solution;
    }
    else {
        return solve_part_two(input.data, part_one_solution);
    }
}

int main(int argc, char *argv[]) {
    auto parse_input_nums = [](int argc, char *argv[]) {
        return parse_input<long long>(argc, argv, [](const std::string& s) { return std::stoll(s); });
    };
    return solve_cli<std::vector<long long>,long long>(argc, argv, parse_input_nums, solve_puzzle);
}

