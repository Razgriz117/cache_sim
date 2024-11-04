#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

class Instruction
{
public:
     Instruction(unsigned short op, unsigned int address);

     unsigned short op;
     unsigned int address;

     std::string to_string() const;
};

#endif // INSTRUCTION_HPP
