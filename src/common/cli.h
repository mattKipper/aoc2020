#ifndef AOC_2020_CLI_H
#define AOC_2020_CLI_H

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>
#include <sstream>
#include <fstream>

enum class Part {
    ONE,
    TWO
};

template<typename Input>
struct CLIInput {
    Part part;
    Input data;
};

template<typename Input, typename Result>
int solve_cli(int argc, char *argv[], std::function<CLIInput<Input>(int, char**)> parse_input, std::function<Result(const CLIInput<Input>&)> solve) {
    try {
        auto input = parse_input(argc, argv);
        auto result = solve(input);
        std::cout << result << std::endl;

    }
    catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

CLIInput<std::string> parse_arguments(int argc, char *argv[]);

template<typename T>
CLIInput<std::vector<T>> parse_input(int argc, char *argv[], std::function<T(const std::string&)> transform) {
    auto args = parse_arguments(argc, argv);

    CLIInput<std::vector<T>> result;
    result.part = args.part;

    std::ifstream input_file(args.data);
    std::string line;

    while (std::getline(input_file, line)) {
        result.data.push_back(transform(line));
    }

    return result;
}

CLIInput<std::vector<std::string>> parse_input(int argc, char *argv[]);


#endif //AOC_2020_CLI_H
