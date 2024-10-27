#include "instruction.hpp"

// Constructor implementation
Instruction::Instruction(unsigned short op, unsigned int address)
    : op(op), address(address) {}
