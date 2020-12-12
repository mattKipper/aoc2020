//
// Created by kipper on 2020-12-12.
//
#define _USE_MATH_DEFINES
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
    return (degrees / 90) % 4;
}

Direction turn(Direction facing, const std::function<Direction(Direction)>& turn_fn, int degrees) {
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

    explicit Instruction(const std::string& line) :
        action(action_from_char(line[0])),
        magnitude(magnitude_from_line(line)) {}

    Instruction(Action act, int mag) : action(act), magnitude(mag) {}
};

float to_deg(float rad) {
    constexpr float FACTOR = 180 / M_PI;
    return rad * FACTOR;
}

float to_rad(float deg) {
    constexpr float FACTOR = M_PI / 180;
    return deg * FACTOR;
}

struct PolarPoint;
struct CartesianPoint {
    double x;
    double y;
    CartesianPoint (double x, double y) : x(x), y(y) {}
    CartesianPoint(const PolarPoint& p);
};

struct PolarPoint {
    double r;
    double theta;
    PolarPoint(double r, double theta) : r(r), theta(theta) {}
    PolarPoint(const CartesianPoint& p);
};

CartesianPoint::CartesianPoint(const PolarPoint& p) {
    auto theta_rad = to_rad(p.theta);
    x = p.r * std::cos(theta_rad);
    y = p.r * std::sin(theta_rad);
}

PolarPoint::PolarPoint(const CartesianPoint& p) :
    r(std::sqrt(std::pow(p.x,2) + std::pow(p.y,2))),
    theta(to_deg(std::atan2(p.y, p.x))) {}


struct Ship {
    CartesianPoint position;
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

void move_part_one(Ship& ship, const Instruction& ins) {
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
            move_part_one(ship, Instruction(forward_action(ship.facing), ins.magnitude));
            break;
    }
}

struct NavShip {
    CartesianPoint ship;
    CartesianPoint waypoint;
    NavShip() : ship(0,0), waypoint(10,1) {}
};

void move_part_two(NavShip& nav, const Instruction& ins) {
    switch (ins.action) {
        case Action::MOVE_NORTH:
            nav.waypoint.y += ins.magnitude;
            break;
        case Action::MOVE_EAST:
            nav.waypoint.x += ins.magnitude;
            break;
        case Action::MOVE_SOUTH:
            nav.waypoint.y -= ins.magnitude;
            break;
        case Action::MOVE_WEST:
            nav.waypoint.x -= ins.magnitude;
            break;
        case Action::ROTATE_LEFT: {
            auto waypoint_p = PolarPoint(nav.waypoint);
            waypoint_p.theta = waypoint_p.theta + ins.magnitude;
            nav.waypoint = CartesianPoint(waypoint_p);
            break;
        }
        case Action::ROTATE_RIGHT: {
            auto waypoint_p = PolarPoint(nav.waypoint);
            waypoint_p.theta = waypoint_p.theta - ins.magnitude;
            nav.waypoint = CartesianPoint(waypoint_p);
            break;
        }
        case Action::MOVE_FORWARD:
            nav.ship.x += (ins.magnitude * nav.waypoint.x);
            nav.ship.y += (ins.magnitude * nav.waypoint.y);
            break;
    }
}
int solve_part_one(const std::vector<Instruction>& input) {
    Ship ship;
    for (auto instruction: input) {
        move_part_one(ship, instruction);
    }
    return std::abs(ship.position.x) + std::abs(ship.position.y);
}

int solve_part_two(const std::vector<Instruction>& input) {
    NavShip nav;
    for (auto instruction: input) {
        move_part_two(nav, instruction);
    }
    return std::round(std::abs(nav.ship.x) + std::abs(nav.ship.y));
}

int solve_puzzle(const CLIInput<std::vector<Instruction>>& input) {
    switch (input.part) {
        case Part::ONE: return solve_part_one(input.data);
        case Part::TWO: return solve_part_two(input.data);
        default: return -1;
    }
}

int main(int argc, char *argv[]) {
    auto parse_input_instructions = [](int argc, char *argv[]) {
        return parse_input<Instruction>(argc, argv, [](const std::string& s) { return Instruction(s); });
    };
    return solve_cli<std::vector<Instruction>,int>(argc, argv, parse_input_instructions, solve_puzzle);
}


