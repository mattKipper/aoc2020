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

    CartesianPoint& operator+=(const CartesianPoint& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    friend CartesianPoint operator+(CartesianPoint lhs, const CartesianPoint& rhs) {
        lhs += rhs;
        return lhs;
    }
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
    CartesianPoint waypoint;
    float facing;

    Ship() : position(0,0), waypoint(10, 1), facing(0) {}
};

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
            ship.facing += ins.magnitude;
            break;
        case Action::ROTATE_RIGHT:
            ship.facing -= ins.magnitude;
            break;
        case Action::MOVE_FORWARD: {
            auto offset = CartesianPoint(PolarPoint(ins.magnitude, ship.facing));
            ship.position += offset;
            break;
        }
        default:
            break;
    }
}

void move_part_two(Ship& ship, const Instruction& ins) {
    switch (ins.action) {
        case Action::MOVE_NORTH:
            ship.waypoint.y += ins.magnitude;
            break;
        case Action::MOVE_EAST:
            ship.waypoint.x += ins.magnitude;
            break;
        case Action::MOVE_SOUTH:
            ship.waypoint.y -= ins.magnitude;
            break;
        case Action::MOVE_WEST:
            ship.waypoint.x -= ins.magnitude;
            break;
        case Action::ROTATE_LEFT: {
            auto waypoint_p = PolarPoint(ship.waypoint);
            waypoint_p.theta = waypoint_p.theta + ins.magnitude;
            ship.waypoint = CartesianPoint(waypoint_p);
            break;
        }
        case Action::ROTATE_RIGHT: {
            auto waypoint_p = PolarPoint(ship.waypoint);
            waypoint_p.theta = waypoint_p.theta - ins.magnitude;
            ship.waypoint = CartesianPoint(waypoint_p);
            break;
        }
        case Action::MOVE_FORWARD:
            ship.position.x += (ins.magnitude * ship.waypoint.x);
            ship.position.y += (ins.magnitude * ship.waypoint.y);
            break;
    }
}

int solve_part(const std::vector<Instruction>& input, const std::function<void(Ship&,const Instruction&)> move) {
    Ship ship;
    for (auto instruction: input) {
        move(ship, instruction);
    }
    return std::round(std::abs(ship.position.x) + std::abs(ship.position.y));
}

int solve_puzzle(const CLIInput<std::vector<Instruction>>& input) {
    auto move = input.part == Part::ONE ? move_part_one : move_part_two;
    return solve_part(input.data, move);
}

int main(int argc, char *argv[]) {
    auto parse_input_instructions = [](int argc, char *argv[]) {
        return parse_input<Instruction>(argc, argv, [](const std::string& s) { return Instruction(s); });
    };
    return solve_cli<std::vector<Instruction>,int>(argc, argv, parse_input_instructions, solve_puzzle);
}


