//
// Created by kipper on 2020-12-12.
//
#include "cli.h"

#include <cstring>
#include <cmath>

enum class Action {
    MOVE_NORTH,
    MOVE_EAST,
    MOVE_SOUTH,
    MOVE_WEST,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    MOVE_FORWARD
};

Action action_from_char(char c) {
    switch (c) {
        case 'N':
            return Action::MOVE_NORTH;
        case 'E':
            return Action::MOVE_EAST;
        case 'S':
            return Action::MOVE_SOUTH;
        case 'W':
            return Action::MOVE_WEST;
        case 'L':
            return Action::ROTATE_LEFT;
        case 'R':
            return Action::ROTATE_RIGHT;
        case 'F':
            return Action::MOVE_FORWARD;
        default:
            throw std::invalid_argument(std::string("Invalid action: ") + std::string(1, c));
    }
}

enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

Direction to_left(Direction facing) {
    switch (facing) {
        case Direction::NORTH: return Direction::WEST;
        case Direction::EAST: return Direction::NORTH;
        case Direction::SOUTH: return Direction::EAST;
        case Direction::WEST: return Direction::SOUTH;
    }
}

Direction to_right(Direction facing) {
    switch (facing) {
        case Direction::NORTH: return Direction::EAST;
        case Direction::EAST: return Direction::SOUTH;
        case Direction::SOUTH: return Direction::WEST;
        case Direction::WEST: return Direction::NORTH;
    }
}

int turn_count(int degrees) {
    switch (degrees) {
        case 90: return 1;
        case 180: return 2;
        case 270: return 3;
        default: throw std::invalid_argument(std::string("Invalid degrees: ") + std::to_string(degrees));
    }
}

Direction turn(Direction facing, std::function<Direction(Direction)> turn_fn, int degrees) {
    const auto turns = turn_count(degrees);
    for (auto i = 0; i < turns; ++i) {
        facing = turn_fn(facing);
    }
    return facing;
}

Direction turn_left(Direction orig, int degrees) {
    return turn(orig, to_left, degrees);
}

Direction turn_right(Direction orig, int degrees) {
    return turn(orig, to_right, degrees);
}

int magnitude_from_line(const std::string& line) {
    return std::stol(line.c_str() + 1, nullptr, 10);
}

struct Instruction {
    Action action;
    int magnitude;

    Instruction(const std::string& line) :
        action(action_from_char(line[0])),
        magnitude(magnitude_from_line(line)) {}

    Instruction(Action act, int mag) : action(act), magnitude(mag) {}
};


struct Position {
    int x;
    int y;

    Position (int x, int y) : x(x), y(y) {}
};

struct Ship {
    Position position;
    Direction facing;

    Ship() : position(0,0), facing(Direction::EAST) {}
};

Action forward_action(Direction facing) {
    switch (facing) {
        case Direction::NORTH:
            return Action::MOVE_NORTH;
        case Direction::EAST:
            return Action::MOVE_EAST;
        case Direction::SOUTH:
            return Action::MOVE_SOUTH;
        case Direction::WEST:
            return Action::MOVE_WEST;
    }
}

void move(Ship& ship, const Instruction& ins) {
    switch (ins.action) {
        case Action::MOVE_NORTH:
            ship.position.y += ins.magnitude;
            break;
        case Action::MOVE_EAST:
            ship.position.x += ins.magnitude;
            break;
        case Action::MOVE_SOUTH:
            ship.position.y -= ins.magnitude;
            break;
        case Action::MOVE_WEST:
            ship.position.x -= ins.magnitude;
            break;
        case Action::ROTATE_LEFT:
            ship.facing = turn_left(ship.facing, ins.magnitude);
            break;
        case Action::ROTATE_RIGHT:
            ship.facing = turn_right(ship.facing, ins.magnitude);
            break;
        case Action::MOVE_FORWARD:
            move(ship, Instruction(forward_action(ship.facing), ins.magnitude));
            break;
    }
}

int solve_puzzle(const CLIInput<std::vector<Instruction>>& input) {
    Ship ship;
    for (auto instruction: input.data) {
        move(ship, instruction);
    }
    return std::abs(ship.position.x) + std::abs(ship.position.y);
}

int main(int argc, char *argv[]) {
    auto parse_input_instructions = [](int argc, char *argv[]) {
        return parse_input<Instruction>(argc, argv, [](const std::string& s) { return Instruction(s); });
    };
    return solve_cli<std::vector<Instruction>,int>(argc, argv, parse_input_instructions, solve_puzzle);
}


