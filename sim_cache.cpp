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
#include "cache.hpp"
#include "sim_cache.hpp"

// Global constants
#define HEX 16

// Constructor for Sim_Cache
Sim_Cache::Sim_Cache(unsigned int blocksize, const std::vector<unsigned int> &cache_sizes,
                     const std::vector<unsigned int> &cache_assocs, 
                     unsigned int replacement_policy, unsigned int inclusion_property, 
                     const std::string &trace_file)
    : blocksize(blocksize), cache_sizes(cache_sizes), cache_assocs(cache_assocs),
    replacement_policy(replacement_policy), inclusion_property(inclusion_property), 
    trace_file(trace_file)
{
     numCaches = cache_sizes.size();
     constructCaches();
     readInstructions();

     for (const auto &instr : instructions)
     {
          // Print "Write" for 1 and "Read" for 0
          std::string instructionType = (instr.instruction == 1) ? "Write" : "Read";

          // Print the instruction and the address in hexadecimal
          std::cout << "Instruction: " << instructionType
                    << ", Address: 0x" << std::hex << instr.address << std::endl;
     }
}

void Sim_Cache::constructCaches()
{
     for (std::size_t i = 0; i < numCaches; i++)
     {
          caches.emplace_back(
              Cache(
                  blocksize,
                  cache_sizes[i], cache_assocs[i],
                  replacement_policy, inclusion_property));
     }
}

void Sim_Cache::addCache(const Cache &cache)
{
     caches.push_back(cache);
}

void Sim_Cache::readInstructions()
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