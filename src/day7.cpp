//
// Created by kipper on 2020-12-07.
//

#include "cli.h"

#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <unordered_set>

struct Rule {
    std::string colour;
    std::vector<std::pair<std::string,int>> held;
};

Rule from_line(const std::string& line) {
    Rule rule;
    const std::string colour_key("bag");
    auto colour_end = line.find(colour_key) - 1;
    rule.colour = line.substr(0, colour_end);

    std::smatch match;
    std::regex re("(\\d+) (\\w+ \\w+) bag");
    auto start = line.cbegin() + colour_end + 3;
    while (std::regex_search(start, line.end(), match, re)) {
        rule.held.push_back(std::make_pair(match[2], std::stoi(match[1])) );
        start = match.suffix().first;
    }

    return rule;
}

void get_containers(std::unordered_set<std::string>& containers, const std::unordered_map<std::string,std::vector<std::string>>& lookup, const std::string& key) {
    auto match = lookup.find(key);
    if (match != lookup.end()) {
        for (const auto& container: match->second) {
            containers.insert(container);
            get_containers(containers, lookup, container);
        }
    }
}

int solve_part_1(const std::vector<Rule>& input) {
    std::unordered_map<std::string,std::vector<std::string>> lookup;

    for (const auto& rule: input) {
        for (const auto& held: rule.held) {
            auto match = lookup.find(held.first);
            if (match == lookup.end()) {
                lookup[held.first] = std::vector<std::string> { rule.colour };
            }
            else {
                match->second.push_back(rule.colour);
            }
        }
    }

    std::unordered_set<std::string> containers;
    get_containers(containers, lookup, std::string("shiny gold"));

    return containers.size();
}

int count_bags(const std::unordered_map<std::string,std::vector<std::pair<std::string,int>>>& lookup, const std::string& key) {
    int count = 1;
    auto match = lookup.find(key);
    if (match != lookup.end()) {
        for (auto held: match->second) {
            count += held.second * count_bags(lookup, held.first);
        }
    }
    return count;
};

int solve_part_2(const std::vector<Rule>& input) {
    std::unordered_map<std::string,std::vector<std::pair<std::string,int>>> lookup;
    for (const auto& rule: input) {
        lookup[rule.colour] = rule.held;
    }
    return count_bags(lookup, "shiny gold") - 1;
}

int solve_puzzle(const CLIInput<std::vector<Rule>>& input) {
    switch (input.part) {
        case Part::ONE:
            return solve_part_1(input.data);
        case Part::TWO:
            return solve_part_2(input.data);
        default:
            return -1;
    }
}

int main(int argc, char *argv[]) {
    auto parse_input_rules = [](int argc, char *argv[]) {
        return parse_input<Rule>(argc, argv, from_line);
    };

    return solve_cli<std::vector<Rule>,int>(argc, argv, parse_input_rules, solve_puzzle);
}

