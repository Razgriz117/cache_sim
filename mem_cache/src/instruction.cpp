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
        case MemoryAccess::Read:  op_str = "Read "; break;
        case MemoryAccess::Write: op_str = "Write"; break;
    }

    std::ostringstream oss;      // Create an output string stream
    oss << op_str << " " << std::hex << address; // Output the op followed by the address
    return oss.str();            // Return the string representation
}