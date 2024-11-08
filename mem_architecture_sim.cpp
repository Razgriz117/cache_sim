// External libraries
#include <iostream>
#include <iomanip>
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
#define LAST_INSTRUCTION 90
#define SPACES 30

// Constructor for MemArchitectureSim
MemArchitectureSim::MemArchitectureSim(unsigned int blocksize,
                                       const std::vector<unsigned int> &cache_sizes,
                                       const std::vector<unsigned int> &cache_assocs,
                                       unsigned int repl_policy, unsigned int incl_property,
                                       const std::string &trace_file, Cache &main_memory,
                                       bool debug)

    : blocksize(blocksize), cache_sizes(cache_sizes), cache_assocs(cache_assocs),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property),
      trace_file(trace_file), main_memory(main_memory), memory_traffic(0), debug(debug)
{

     inclusion_property = static_cast<InclusionProperty>(incl_property);
     replacement_policy = static_cast<ReplacementPolicy>(repl_policy);

     readInstructions();
     // printInstructions();

     numCaches = cache_sizes.size();
     constructCaches();

     if (debug) 
          print_debug();
     else 
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
     caches[L1].allocate(address);
}

Block MemArchitectureSim::write(unsigned int address)
{
     caches[L1].write(address);
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
     for (std::size_t i = 0; i < numCaches; i++)
     {
          std::string name = "L" + std::to_string(i + 1); // (i.g. "L1")
          if (cache_sizes[i] > 0)
          {
               numNonEmptyCaches++;
               caches.emplace_back(
                   Cache(
                       name,
                       blocksize,
                       cache_sizes[i], cache_assocs[i],
                       replacement_policy, inclusion_property,
                       instructions,
                       debug
                    )
               );
          }
     }

     // Link each cache to next level of memory.
     for (std::size_t i = 0; i < numNonEmptyCaches - 1; i++)
     {
          std::string name = "L" + std::to_string(i + 1); // (i.g. "L1")
          caches[i].next_mem_level = &caches[i + 1];
          caches[i + 1].prev_mem_level = &caches[i];
     }

     // Link final cache to main memory.
     caches[numNonEmptyCaches - 1].next_mem_level = &main_memory;
     main_memory.prev_mem_level = NULL;
     numCaches = numNonEmptyCaches;
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
          execute(instruction);
     }
}

void MemArchitectureSim::execute(Instruction &instruction)
{
     MemoryAccess operation = static_cast<MemoryAccess>(instruction.op);
     unsigned int address = instruction.address;
     switch (operation)
     {
          case MemoryAccess::Read: read(address); break;
          case MemoryAccess::Write: write(address); break;
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

void out(std::string output)
{
     using std::cout;
     using std::endl;
     using std::left;
     using std::setw;
     cout << left << setw(SPACES) << output;
     cout << setw(0);
}

void MemArchitectureSim::print_contents()
{
     for (int i = 0; i < numCaches; i++)
     {
          std::cout << "===== L" << std::to_string(i+1) << " contents =====" << std::endl;
          caches[i].print_contents();
     }

     std::cout << "===== Simulation results (raw) =====" << std::endl;

     char label = 'a';
     std::string reads, read_misses, writes, write_misses, miss_rate, writebacks;
     std::string memory_traffic{};
     unsigned int originalNumCaches = cache_sizes.size();
     for (std::size_t i = 0; i < originalNumCaches; i++)
     {
          std::string name = "L" + std::to_string(i + 1); // (i.g. "L1")
          reads = std::string(1, label++) + ". number of " + name + " reads:";
          read_misses = std::string(1, label++) + ". number of " + name + " read misses:";
          writes = std::string(1, label++) + ". number of " + name + " writes:";
          write_misses = std::string(1, label++) + ". number of " + name + " write misses:";
          miss_rate = std::string(1, label++) + ". " + name + " miss rate:";
          writebacks = std::string(1, label++) + ". number of " + name + " writebacks:";

          if (cache_sizes[i] > 0)
          {
               out(reads);
               std::cout << std::to_string(caches[i].reads) << std::endl;

               out(read_misses);
               std::cout << std::to_string(caches[i].read_misses) << std::endl;

               out(writes);
               std::cout << std::to_string(caches[i].writes) << std::endl;

               out(write_misses);
               std::cout << std::to_string(caches[i].write_misses) << std::endl;

               out(miss_rate);
               std::cout << std::to_string(caches[i].miss_rate) << std::endl;

               out(writebacks);
               std::cout << std::to_string(caches[i].write_backs) << std::endl;
          }
          else
          {
               out(reads);
               std::cout << "0" << std::endl;

               out(read_misses);
               std::cout << "0" << std::endl;

               out(writes);
               std::cout << "0" << std::endl;

               out(write_misses);
               std::cout << "0" << std::endl;
               
               out(miss_rate);
               std::cout << "0" << std::endl;

               out(writebacks);
               std::cout << "0" << std::endl;
          }
     }
     memory_traffic = std::string(1, label++) + ". total memory traffic:";
     out(memory_traffic);
     std::cout << std::to_string(main_memory.numAccesses) << std::endl;

     // std::string memory_writes = std::string(1, label++) + ". total memory writes:";
     // out(memory_writes);
     // std::cout << std::to_string(main_memory.writes) << std::endl;

     // std::string memory_reads = std::string(1, label++) + ". total memory reads:";
     // out(memory_reads);
     // std::cout << std::to_string(main_memory.reads) << std::endl;
}

void MemArchitectureSim::print_debug()
{
     using std::cout;
     using std::endl;
     using std::setw;
     using std::left;

     cout << "----------------------------------------" << endl;

     int numInstructions = instructions.size();
     int i;
     for (i = 0; i < numInstructions; i++)
     {
          cout << "# " << std::to_string(i + 1) << " : ";
          cout << instructions[i].to_string() << endl;
          execute(instructions[i]);
          cout << "----------------------------------------" << endl;

          // if (i + 1 >= LAST_INSTRUCTION) break;
     }


     // std::vector<Set> sets = caches[L1].getCache();
     // for (int i = 0; i < sets.size(); i++)
     // {
     //      std::cout << "Set " << i << " size: " << sets[i].getSize() << std::endl;
     // }
     // std::vector<Block> blocks = sets[26].blocks;
     // Block first = blocks[0];
     // Block second = blocks[1];

     // // sets[26].print_contents();

     // cout << first.isDirty() << endl;
     // cout << second.isDirty() << endl;

     // cout << std::hex << first.getAddress().value << endl;
     // cout << std::hex << second.getAddress().value << endl;
}
