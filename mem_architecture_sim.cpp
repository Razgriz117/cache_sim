// External libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Local enums
#include "inclusion_property.hpp"
#include "replacement_policy.hpp"
#include "memory_access.hpp"

// Local libraries
#include "block.hpp"
#include "cache.hpp"
#include "mem_architecture_sim.hpp"

// Global constants
#define HEX 16

// Constructor for MemArchitectureSim
MemArchitectureSim::MemArchitectureSim(unsigned int blocksize, 
                     const std::vector<unsigned int> &cache_sizes,
                     const std::vector<unsigned int> &cache_assocs, 
                     unsigned int repl_policy, unsigned int incl_property, 
                     const std::string &trace_file)
    : blocksize(blocksize), cache_sizes(cache_sizes), cache_assocs(cache_assocs),
    replacement_policy(replacement_policy), inclusion_property(inclusion_property), 
    trace_file(trace_file)
{
     inclusion_property = static_cast<InclusionProperty>(incl_property);
     replacement_policy = static_cast<ReplacementPolicy>(repl_policy);

     readInstructions();
     printInstructions();

     numCaches = cache_sizes.size();
     constructCaches();
}

Block MemArchitectureSim::read(unsigned int addr)
{
     // Call cache read function
}

Block MemArchitectureSim::write(unsigned int address)
{
     // Handle inclusion property.
     switch (inclusion_property)
     {
     case InclusionProperty::NonInclusive:
          break;

     case InclusionProperty::Inclusive:
          break;
     }
}

Block MemArchitectureSim::write_back(unsigned int address)
{
}

Block MemArchitectureSim::writeToCache(unsigned int cache_idx, unsigned int address)
{

}

void MemArchitectureSim::constructCaches()
{

     for (std::size_t i = 0; i < numCaches; i++)
     {
          caches.emplace_back(
               Cache(
                    blocksize,
                    cache_sizes[i], cache_assocs[i],
                    replacement_policy, inclusion_property,
                    instructions
               )
          );
     }
}

void MemArchitectureSim::addCache(const Cache &cache)
{
     caches.push_back(cache);
}

void MemArchitectureSim::executeInstructions()
{
     for (auto instruction : instructions)
     {
          MemoryAccess operation = static_cast<MemoryAccess>(instruction.op);
          switch (operation)
          {
               case MemoryAccess::Read: read(instruction.address); break;
               case MemoryAccess::Write: write(instruction.address); break;
          }
     }
}    

void MemArchitectureSim::readInstructions()
{
     std::ifstream file(trace_file);

     if (!file.is_open())
     {
          std::cerr << "Error: Unable to open trace file: " << trace_file << std::endl;
          return;
     }

     // Read in all lines from file.
     std::string line;
     while (std::getline(file, line))
     {
          std::istringstream input_stream(line);
          std::string instruction_str;
          std::string address_str;

          // Read the instruction and address from the line.
          if (!(input_stream >> instruction_str >> address_str))
          {
               std::cerr << "Error: Invalid line format: " << line << std::endl;
               continue; // Skip to the next line
          }

          // Convert instruction_str to MemoryAccess enum
          MemoryAccess access;
          if (instruction_str == "r") { access = MemoryAccess::Read; }
          else if (instruction_str == "w") { access = MemoryAccess::Write; }
          else
          {
               std::cerr << "Error: Unknown instruction type: " << instruction_str 
               << std::endl;
               continue; // Skip to the next line
          }

          // Convert the address string to an unsigned int
          unsigned int address;
          try { address = std::stoul(address_str, nullptr, HEX); }
          catch (const std::invalid_argument &e)
          {
               std::cerr << "Error: Invalid address format: " << address_str << std::endl;
               continue; // Skip to the next line
          }

          // Create an Instruction object and store it
          instructions.emplace_back(static_cast<unsigned short>(access), address);
     }

     file.close(); // Close the file after reading
}

void MemArchitectureSim::printInstructions()
{
     std::string instruction_str;
     for (const auto &instruction : instructions)
     {
          instruction_str = (instruction.op == MemoryAccess::Write) ? "Write" : "Read";

          // Print the instruction and the address in hexadecimal
          std::cout << "Instruction: " << instruction_str
                    << ", Address: 0x" << std::hex << instruction.address << std::endl;
     }
}