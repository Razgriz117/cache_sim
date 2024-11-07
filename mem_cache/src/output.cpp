#include <iostream>
#include <iomanip>
#include <string>

#include "address.hpp"
#include "block.hpp"
#include "replacement_policy.hpp"
#include "output.hpp"

void Output::address_output(const Address &address)
{
     unsigned int index = address.setIndex;

     std::stringstream stream;
     stream << std::hex << address.tag;
     std::string tag(stream.str());

     std::cout << std::hex << address.value << " ";
     std::cout << "(tag " << tag << ", index " << index;
}

void Output::block_output(Block &block)
{
     auto &address = block.getAddress();
     address_output(address);

     std::string cleanliness{};
     if (block.isDirty())
          cleanliness = "dirty";
     else
          cleanliness = "clean";

     std::cout << ", " << cleanliness << ")" << std::endl;
}

void Output::victim_output(Block &block)
{
     if (!debug)
          return;

     std::cout << name << " victim: ";
     block_output(block);
}

void Output::op_output(const std::string &op, unsigned int addr)
{
     if (!debug)
          return;

     auto address = Address(addr, blocksize, numSets);
     std::cout << name << " " << op << " : ";
     address_output(address);
     std::cout << ")" << std::endl;
}

void Output::hit_output()
{
     if (!debug)
          return;

     std::cout << name << " hit" << std::endl;
}

void Output::miss_output()
{
     if (!debug)
          return;

     std::cout << name << " miss" << std::endl;
}

void Output::update_policy_output()
{
     if (!debug)
          return;

     std::cout << name << " update ";
     std::string policy;
     switch (replacement_policy)
     {
          case ReplacementPolicy::LRU: policy = "LRU"; break;
          case ReplacementPolicy::FIFO: policy = "FIFO"; break;
          case ReplacementPolicy::Optimal: policy = "optimal"; break;
     }

     std::cout << policy << std::endl;
}

void Output::dirty_output()
{
     if (!debug)
          return;

     std::cout << name << " set dirty" << std::endl;
}
