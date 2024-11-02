#include <iostream>
#include <optional>

#include "address.hpp"
#include "block.hpp"
#include "cache.hpp"
#include "instruction.hpp"
#include "set.hpp"

#define MISS std::nullopt
#define EMPTY_BLOCK std::nullopt

// Constructor implementation
Cache::Cache(const std::string &name, unsigned int blocksize, unsigned int size,
             unsigned int assoc,
             ReplacementPolicy replacement_policy, InclusionProperty inclusion_property,
             std::vector<Instruction> &instructions)

    : name(name), blocksize(blocksize), size(size), assoc(assoc),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property),
      numAccesses(0)
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

     // Decode address.
     auto address = Address(addr, blocksize, numSets);

     // Read from the matching set.
     Set &set = cache[address.setIndex];
     auto result = set.read(address);

     // If we miss, try the next cache.
     if (result == MISS && next_mem_level != NULL)
          next_mem_level->read(addr);

     return result;
}

std::optional<std::reference_wrapper<Block>> Cache::write(unsigned int addr)
{
     // Increment cache accesses.
     access();

     // Decode address.
     auto address = Address(addr, blocksize, numSets);

     // if (name == "MAIN_MEMORY")
     //      return Block(blocksize, address);

     // Write to the set marked by the address's set index.
     Set &set = cache[address.setIndex];
     auto victim = set.write(address);

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

     // If we evicted a block during writing, write back to next level of memory.
     if (victim)
     {
          Block &victim_block = victim->get();
          if (victim_block.isDirty() && next_mem_level != NULL)
               next_mem_level->write(victim_block.getAddress().value);
          return victim_block;
     }

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
