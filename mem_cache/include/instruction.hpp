#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

class Instruction
{
public:
     Instruction(unsigned short op, unsigned int address);

     unsigned short op;
     unsigned int address;
};

#endif // INSTRUCTION_HPP
