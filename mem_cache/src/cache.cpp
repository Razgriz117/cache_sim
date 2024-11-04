#include <iostream>
#include <optional>

#include "address.hpp"
#include "block.hpp"
#include "cache.hpp"
#include "instruction.hpp"
#include "set.hpp"

#include "output.hpp"

#define MISS std::nullopt
#define LOAD_FAILURE std::nullopt
#define EMPTY_BLOCK std::nullopt

#define VERBOSE true

// Constructor implementation
Cache::Cache(const std::string name, unsigned int blocksize, unsigned int size,
             unsigned int assoc,
             ReplacementPolicy replacement_policy, InclusionProperty inclusion_property,
             std::vector<Instruction> &instructions)

    : name(name), blocksize(blocksize), size(size), assoc(assoc),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property),
      numAccesses(0), reads(0), read_misses(0), writes(0), write_misses(0), write_backs(0),
      miss_rate(0.0)
{
     // Calculate number of sets.
     numSets = size / (blocksize * assoc);

     // Initialize the cache with Set objects, each set containing `assoc` blocks.
     for (unsigned int i = 0; i < numSets; ++i)
     {
          cache.emplace_back(Set(assoc, blocksize, replacement_policy));
     }

     // Construct set traces for Optimal replacement policy.
     if (replacement_policy == ReplacementPolicy::Optimal)
          construct_set_traces(instructions);
}

std::optional<std::reference_wrapper<Block>> Cache::read(unsigned int addr)
{
     // Increment cache accesses.
     access();
     reads++;
     // if (VERBOSE) std::cout << "Read " << std::hex << addr << " from " << name << std::endl;
     // fflush(stdout);

     // For main memory, reads never miss, so we return a valid block.
     if (name == "MAIN_MEMORY")
     {
          auto address = Address(addr, blocksize, numSets);
          Set &set = cache[address.setIndex];
          Block &newBlock = *(new Block(blocksize, address));
          return newBlock;
     }

     // if (VERBOSE) std::cout << "    Reading " << std::hex << addr << " in cache " << name << std::endl;

     // std::string next_cache_name{};
     // if (next_mem_level == NULL) next_cache_name = "NONE!";
     // else next_cache_name = next_mem_level->name;
     // if (VERBOSE) std::cout << "    Next cache: " << next_cache_name << std::endl;

     // Decode address.
     // auto address = Address(addr, blocksize, numSets);

     // // Read from the matching set.
     // Set &set = cache[address.setIndex];
     // auto result = set.read(address);

     // Read from current cache.
     auto result = search(addr);
     if (result)
     {
          Block &found_block = result->get();
          return found_block;
     }

     // // For main memory, reads never miss, so we return a valid block.
     // if (name == "MAIN_MEMORY")
     // {
     //      auto address = Address(addr, blocksize, numSets);
     //      Set &set = cache[address.setIndex];
     //      Block &newBlock = *(new Block(blocksize, address));

     //      set.write(newBlock);
     //      return newBlock;
     // }

     // If we miss, read from lower level caches.
     if (result == MISS)
     {
          read_misses++;
          if (next_mem_level != NULL) return next_mem_level->read(addr);
     }

     // Error reading block.
     return LOAD_FAILURE;
}

std::optional<std::reference_wrapper<Block>> Cache::write(unsigned int addr)
{
     // Increment cache accesses.
     access();
     writes++;

     if (VERBOSE) std::cout << "Write " << std::hex << addr << "to " << name << std::endl;
     fflush(stdout);

     // For main memory, writes never miss, so we generate a valid block.
     if (name == "MAIN_MEMORY")
     {
          return EMPTY_BLOCK;
     }

     // Decode address.
     auto address = Address(addr, blocksize, numSets);
     Set &set = cache[address.setIndex];

     // Load block if it already exists in cache.
     std::optional<std::reference_wrapper<Block>> found_block = EMPTY_BLOCK;
     auto result = set.search(address);
     if (result)
     {
          if (VERBOSE)
               std::cout << "Found " << std::hex << addr << "in " << name << std::endl;
          found_block = result->get();
     }

     // // For main memory, writes never miss, so we generate a valid block.
     // if (result == MISS && name == "MAIN_MEMORY")
     // {
     //      set.write(address);
     //      return EMPTY_BLOCK;
     // }

     // If we miss, attempt to update block read from lower level caches.
     else if (result == MISS)
     {
          if (VERBOSE)
               std::cout << "Write miss of " << std::hex << addr << "in " << name << std::endl;
          write_misses++;
          if (next_mem_level != NULL)
          {
               auto load_result = next_mem_level->read(addr);
               if (load_result) found_block = load_result->get();
               else
               {
                    std::cout << "Error loading " << std::hex << addr;
                    std::cout << " from lower caches." << std::endl;
                    return LOAD_FAILURE;
               }
          }
     }

     if (!found_block)
     {
          std::cout << "ERROR: Block " << std::hex << addr << "not found." << std::endl;
          return LOAD_FAILURE;
     }

     if (VERBOSE)
          std::cout << "Retrieved block " << std::hex << addr << "in " << name << std::endl;
     Block &block = found_block->get();

     // Write to the set marked by the address's set index.
     auto victim = set.write(block);

     if (VERBOSE)
          std::cout << "Wrote " << std::hex << addr << "to set in " << name << std::endl;

     // Maintain inclusive property.
     if (inclusion_property == InclusionProperty::Inclusive && next_mem_level != NULL)
     {
          // If block was evicted, remove it from lower level caches.
          if (victim && prev_mem_level != NULL)
          {
               Block &victim_block = victim->get();
               unsigned int victim_address = victim_block.getAddress().value;
               prev_mem_level->delete_block(victim_address);
          }
          // A block written to this cache must be included in next level of memory.
          next_mem_level->write(addr);
     }

     if (VERBOSE)
          std::cout << "Handled inclusion of " << std::hex << addr << " in " << name << std::endl;

     // If we evicted a block during writing, write back to next level of memory.
     if (victim)
     {
          Block &victim_block = victim->get();
          if (victim_block.isDirty() && next_mem_level != NULL)
               next_mem_level->write(victim_block.getAddress().value);
          write_backs++;
          return victim_block;
     }

     if (VERBOSE)
          std::cout << "Handled write back of victim in " << name << std::endl;

     // No victim block.
     return EMPTY_BLOCK;
}

std::optional<std::reference_wrapper<Block>> Cache::search(unsigned int addr)
{
     // Decode address.
     auto address = Address(addr, blocksize, numSets);

     // Search for block in the specified set.
     Set &set = cache[address.setIndex];

     return set.search(address);
}

std::optional<std::reference_wrapper<Block>> Cache::load(unsigned int addr)
{
     // Load block from current cache, if present.
     auto result = search(addr);
     if (result)
     {
          Block& found_block = result->get();
          return found_block;
     }

     // Load from lower level caches, if any.
     if (next_mem_level != NULL)
          return next_mem_level->load(addr);

     // Load from main memory if not found in any cache.
     if (name == "MAIN_MEMORY")
     {
          auto address = Address(addr, blocksize, numSets);
          Block &newBlock = *(new Block(blocksize, address));
          return newBlock;
     }

     // Reached main memory and failed to load.
     return LOAD_FAILURE;
}

void Cache::delete_block(unsigned int addr)
{
     // Decode address.
     auto address = Address(addr, blocksize, numSets);

     // Search for block in the specified set.
     Set &set = cache[address.setIndex];

     set.delete_block(address);

     // Maintain inclusion property.
     if (inclusion_property == InclusionProperty::Inclusive && prev_mem_level != NULL)
          prev_mem_level->delete_block(addr);
}

double Cache::calculate_miss_rate()
{
     if (reads + writes == 0) return 0.0;
     miss_rate = static_cast<double>(read_misses + write_misses) / (reads + writes);
     return miss_rate;
}

void Cache::construct_set_traces(std::vector<Instruction> &instructions)
{
     for (auto &instruction : instructions)
     {
          for (int curr_set = 0; curr_set < numSets; curr_set++)
          {
               // Get current set.
               Set &set = cache[curr_set];

               // Extract address from instruction.
               auto address = Address(instruction.address, blocksize, numSets);

               // Append address to set trace if it maps to the current set.
               if (address.setIndex == curr_set)
               {
                    set.trace.push_back(address);
                    break;
               }
          }
     }
}

void Cache::print_contents()
{
     for (int i = 0; i < numSets; i++)
     {
          std::string set = std::to_string(i) + ":";
          Output::leftOut("Set:"); Output::leftOut(set);
          cache[i].print_contents();
     }
}
