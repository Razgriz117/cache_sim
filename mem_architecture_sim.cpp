// External libraries
#include <iostream>
#include <fstream>
#include <optional>
#include <functional>
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
#define DIRECT_MAPPED 1
#define HEX 16
#define MISS std::nullopt
#define EMPTY_BLOCK std::nullopt
#define L1 0

#define VERBOSE true

// Constructor for MemArchitectureSim
MemArchitectureSim::MemArchitectureSim(unsigned int blocksize,
                                       const std::vector<unsigned int> &cache_sizes,
                                       const std::vector<unsigned int> &cache_assocs,
                                       unsigned int repl_policy, unsigned int incl_property,
                                       const std::string &trace_file, Cache &main_memory)

    : blocksize(blocksize), cache_sizes(cache_sizes), cache_assocs(cache_assocs),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property),
      trace_file(trace_file), main_memory(main_memory), memory_traffic(0)
{
     inclusion_property = static_cast<InclusionProperty>(incl_property);
     replacement_policy = static_cast<ReplacementPolicy>(repl_policy);

     readInstructions();
     // printInstructions();

     numCaches = cache_sizes.size();
     constructCaches();

     executeInstructions();

     calculate_miss_rates();
     memory_traffic = main_memory.reads + main_memory.writes;

     print_contents();
}

Block& MemArchitectureSim::read(unsigned int address)
{
     auto hit = caches[L1].read(address);

     if (hit)
     {
          Block& block = *hit;
          return block;
     }
     
     // Allocate to cache on miss.
     caches[L1].write(address);
}

Block MemArchitectureSim::write(unsigned int address)
{
     // for (auto &cache : caches)
     // {
     //      auto hit = cache.search(address);
     //      if (hit)
     //      {
     //           Block hit_block = *hit;
     //           Cache hit_cache = cache;
     //      }
     // }

     // if (VERBOSE) std::cout << "    Writing " << std::hex << address << std::endl;
     // if (VERBOSE) std::cout << "    Searching for " << std::hex << address << "...";

     // auto result = caches[L1].search(address);
     // if (result)
     // {
     //      // Update data
     //      Block& block = result->get();
     //      // block.data = something
     // }

     caches[L1].write(address);

     // // Handle inclusion property.
     // switch (inclusion_property)
     // {
     // case InclusionProperty::NonInclusive:
     //      break;

     // case InclusionProperty::Inclusive:
     //      break;
     // }
}

Block MemArchitectureSim::write_back(unsigned int address)
{
}

Block MemArchitectureSim::writeToCache(unsigned int cache_idx, unsigned int address)
{

}

std::optional<std::reference_wrapper<Block>> MemArchitectureSim::search(unsigned int address)
{
     for (auto& cache : caches)
     {
          auto hit = cache.search(address);
          if (hit) return hit;
     }

     // If we reach this line, we missed in every cache.
     return MISS;
}

void MemArchitectureSim::constructCaches()
{
     numNonEmptyCaches = 0;
     if (VERBOSE) std::cout << "Constructing caches:" << std::endl;
     for (std::size_t i = 0; i < numCaches; i++)
     {
          std::string name = "L" + std::to_string(i + 1); // (i.g. "L1")
          if (VERBOSE) std::cout << "  Adding " << name << " cache to list..." << std::endl;
          if (cache_sizes[i] > 0)
          {
               numNonEmptyCaches++;
               caches.emplace_back(
                   Cache(
                       name,
                       blocksize,
                       cache_sizes[i], cache_assocs[i],
                       replacement_policy, inclusion_property,
                       instructions
                    )
               );
          }
     }

     if (VERBOSE) std::cout << "Linking caches:" << std::endl;

     // Link each cache to next level of memory.
     for (std::size_t i = 0; i < numNonEmptyCaches - 1; i++)
     {
          std::string name = "L" + std::to_string(i + 1); // (i.g. "L1")
          if (VERBOSE) std::cout << "  Linking " << name << " to next cache...";
          caches[i].next_mem_level = &caches[i + 1];
          caches[i + 1].prev_mem_level = &caches[i];
          if (VERBOSE) std::cout << " Done!" << std::endl;
     }

     if (VERBOSE)
          std::cout << "  Linking " << "L" << std::to_string(numNonEmptyCaches) << " to MAIN_MEMORY.";

     // Link final cache to main memory.
     caches[numNonEmptyCaches - 1].next_mem_level = &main_memory;
     main_memory.prev_mem_level = NULL;
     numCaches = numNonEmptyCaches;

     if (VERBOSE) std::cout << "  Done!" << std::endl;
}

void MemArchitectureSim::addCache(const Cache &cache)
{
     caches.push_back(cache);
}

void MemArchitectureSim::calculate_miss_rates()
{
     for (auto& cache : caches)
          cache.calculate_miss_rate();
}

void MemArchitectureSim::executeInstructions()
{
     // if (VERBOSE) std::cout << "Executing instructions:" << std::endl;
     for (auto instruction : instructions)
     {
          // if (VERBOSE) std::cout << "  " << instruction.to_string() << std::endl;
          MemoryAccess operation = static_cast<MemoryAccess>(instruction.op);
          unsigned int address = instruction.address;
          switch (operation)
          {
               case MemoryAccess::Read:  read(address);  break;
               case MemoryAccess::Write: write(address); break;
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

void MemArchitectureSim::print_contents()
{
     for (int i = 0; i < numCaches; i++)
     {
          std::cout << "===== L" << std::to_string(i+1) << " contents =====" << std::endl;
          caches[i].print_contents();
     }
}