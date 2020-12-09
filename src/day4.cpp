//
// Created by kipper on 2020-12-04.
//
#include "cli.h"

#include <unordered_map>
#include <unordered_set>

static const std::unordered_set<std::string> REQUIRED_FIELDS {
    "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
};

bool valid_year(const std::string& year, int min, int max) {
    try {
        int num = std::stoi(year);
        return year.size() == 4 && num >= min && num <= max;
    }
    catch (...) {
        return false;
    }
}

bool valid_height(const std::string& height) {
    try {
        size_t end;
        int num = std::stoi(height, &end);
        if (height.size() - end == 2) {
            if (height[end] == 'c' && height[end + 1] == 'm') {
                return num >= 150 && num <= 193;
            } else if (height[end] == 'i' && height[end + 1] == 'n') {
                return num >= 59 && num <= 76;
            }
        }
        return false;
    }
    catch (...) {
        return false;
    }
}

bool valid_hair_colour(const std::string& hc) {
    if (hc.size() != 7 || hc[0] != '#') {
        return false;
    }
    for(size_t i = 1; i < hc.size(); ++i) {
        if (!((hc[i] >= '0' && hc[i] <= '9') || (hc[i] >= 'a' && hc[i] <='f'))) {
            return false;
        }
    }
    return true;
}

bool valid_eye_colour(const std::string& ec) {
    return ec == "amb" || ec == "blu" || ec == "brn" || ec == "gry" ||
           ec == "grn" || ec == "hzl" || ec == "oth";
}

bool valid_passport_id(const std::string& id) {
    try {
        std::stoi(id, nullptr, 10);
        return id.size() == 9;
    }
    catch (...) {
        return false;
    }
}

static const std::unordered_map<std::string, std::function<bool(const std::string&)>> validators {
    { "byr",  [](const std::string& birth_year) { return valid_year(birth_year, 1920, 2002); } },
    { "iyr",  [](const std::string& issue_year) { return valid_year(issue_year, 2010, 2020); } },
    { "eyr",  [](const std::string& expiry_year) { return valid_year(expiry_year, 2020, 2030); } },
    { "hgt", valid_height },
    { "hcl", valid_hair_colour },
    { "ecl", valid_eye_colour },
    { "pid", valid_passport_id }
};

bool is_valid(const std::unordered_map<std::string,std::string>& passport, bool validate) {
    auto required { REQUIRED_FIELDS };
    for (const auto& field : passport) {
        required.erase(field.first);
        if (validate) {
            auto validator = validators.find(field.first);
            if (validator != validators.end() && !validator->second(field.second)) {
                return false;
            }
        }
    }
    return required.empty();
}

int solve_puzzle(const CLIInput<std::vector<std::string>>& input) {
    std::unordered_map<std::string,std::string> passport;
    int valid_passports = 0;
    bool validate = input.part == Part::TWO;
    for (const auto& line: input.data) {
        if (line.empty()) {
            valid_passports += is_valid(passport, validate);
            passport.clear();
        }
        else {
            size_t start = 0;
            size_t end;
            char next_token = ':';
            std::string key;
            do {
                end = line.find(next_token, start);
                std::string sub = line.substr(start, end - start);
                if (next_token == ':') {
                    key = sub;
                    next_token = ' ';
                }
                else {
                    passport[key] = sub;
                    next_token = ':';
                }
                start = end + 1;
            } while(end != std::string::npos);
        }
    }
    return valid_passports;
}

int main(int argc, char *argv[]) {
    return solve_cli<std::vector<std::string>,int>(argc, argv, parse_input, solve_puzzle);
}
