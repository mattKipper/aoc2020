#include "cli.h"


CLIInput<std::string> parse_arguments(int argc, char *argv[]) {
    CLIInput<std::string> args;

    if (argc > 3) {
        throw std::invalid_argument("Too many arguments provided.");
    } else if (argc < 2) {
        throw std::invalid_argument("At least one argument must be provided.");
    } else if (argc == 3) {
        if (std::string(argv[1]) != "-2") {
            throw std::invalid_argument("Only supported flag is '-2' for part 2 of puzzle.");
        } else {
            args.data = argv[2];
            args.part = Part::TWO;
        }
    } else {  // argc == 2
        args.data = argv[1];
        args.part = Part::ONE;
    }

    return args;
}

CLIInput<std::vector<std::string>> parse_input(int argc, char *argv[]) {
    return parse_input<std::string>(argc, argv, [](const std::string& s) { return s; });
}


