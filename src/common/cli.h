#ifndef AOC_2020_CLI_H
#define AOC_2020_CLI_H

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>

template<typename Input>
struct CLIInput {
    bool part_2;
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

CLIInput<std::vector<std::string>> parse_input_file_lines(int argc, char *argv[]);

#endif //AOC_2020_CLI_H
