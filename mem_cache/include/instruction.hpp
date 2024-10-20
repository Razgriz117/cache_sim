#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

class Instruction
{
public:
     Instruction(unsigned short instruction, unsigned int address);

     // Public fields
     unsigned short instruction;
     unsigned int address;
};

#endif // INSTRUCTION_HPP
