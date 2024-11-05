#include <sstream>

#include "memory_access.hpp"
#include "instruction.hpp"

// Constructor implementation
Instruction::Instruction(unsigned short op, unsigned int address)
    : op(op), address(address) {}

std::string Instruction::to_string() const
{
    MemoryAccess operation = static_cast<MemoryAccess>(op);
    std::string op_str{};
    switch (operation)
    {
        case MemoryAccess::Read:  op_str = "read"; break;
        case MemoryAccess::Write: op_str = "write"; break;
    }

    std::ostringstream oss;
    oss << op_str << " " << std::hex << address;
    return oss.str();
}