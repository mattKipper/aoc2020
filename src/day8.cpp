//
// Created by kipper on 2020-12-08.
//
#include "cli.h"

#include <algorithm>

enum class Opcode {
    ACC,
    JMP,
    NOP
};

Opcode opcode_from_line(const std::string& line) {
    auto opcode = line.substr(0, 3);
    if (opcode == "acc") {
        return Opcode::ACC;
    }
    else if (opcode == "jmp") {
        return Opcode::JMP;
    }
    else if (opcode == "nop") {
        return Opcode::NOP;
    }
    else {
        throw std::invalid_argument(std::string("Invalid opcode: ") + std::string(opcode));
    }
}

struct Instruction {
    Opcode op;
    int argument;
};

Instruction instruction_from_line(const std::string& line) {
    return Instruction { opcode_from_line(line), std::stoi(line.substr(4)) };
}


struct MonitoredInstruction {
    Instruction instruction;
    bool evaluated;

    explicit MonitoredInstruction(Instruction ins):
        instruction(ins),
        evaluated(false) {}
};

struct Machine {
    int acc;
    std::vector<MonitoredInstruction> program;
    std::vector<MonitoredInstruction>::iterator pc;

    explicit Machine(const std::vector<Instruction>& prog):
         acc(0) {
         for (const auto& instruction: prog) {
             program.emplace_back(MonitoredInstruction(instruction));
         }
         pc = program.begin();
    }

    Machine() = delete;

     void eval() {
        if (pc != program.end()) {
            pc->evaluated = true;
            switch (pc->instruction.op) {
                case Opcode::ACC:
                    acc += pc->instruction.argument;
                    ++pc;
                    break;
                case Opcode::JMP:
                    pc += pc->instruction.argument;
                    break;
                case Opcode::NOP:
                    ++pc;
                    break;
                default:
                    break;
            }
        }
    }

    void reset() {
         pc = program.begin();
         acc = 0;
     }

    bool has_terminated() const {
        return pc == program.cend();
    }

    bool has_looped() const {
        return pc != program.end() && pc->evaluated;
    }
};


int solve_puzzle(const CLIInput<std::vector<Instruction>>& input) {
    if (input.part == Part::ONE) {
        Machine mach(input.data);
        while (!mach.has_looped()) {
            mach.eval();
        }
        return mach.acc;
    }
    else {
        for (auto i = 0; i < input.data.size(); ++i) {
            Machine mach(input.data);
            Opcode& op = mach.program[i].instruction.op;
            if (op == Opcode::JMP || op == Opcode::NOP) {
                op = (op == Opcode::JMP) ? Opcode::NOP : Opcode::JMP;
                while (!mach.has_looped() && !mach.has_terminated()) {
                    mach.eval();
                }
                if (mach.has_terminated()) {
                    return mach.acc;
                }
            }
        }
        return -1;
    }
}

int main(int argc, char *argv[]) {
    auto parse_input_program = [](int argc, char *argv[]) {
        return parse_input<Instruction>(argc, argv, instruction_from_line);
    };
    return solve_cli<std::vector<Instruction>,int>(argc, argv, parse_input_program, solve_puzzle);
}

