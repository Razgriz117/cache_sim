#include <iostream>

#include "address.hpp"
#include "block.hpp"
#include "cache.hpp"
#include "instruction.hpp"
#include "set.hpp"


// Constructor implementation
Cache::Cache(unsigned int blocksize, unsigned int size, unsigned int assoc,
             ReplacementPolicy replacement_policy, InclusionProperty inclusion_property,
             std::vector<Instruction> &instructions)
    : blocksize(blocksize), size(size), assoc(assoc),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property)
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

Block Cache::write(unsigned int addr)
{
     // Decode address.
     auto address = Address(addr, blocksize, numSets);

     // Write to the set marked by the address's set index.
     Set &set = cache[address.setIndex];
     set.write(address);
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
                    set.trace.push_back(address);
          }
     }

}
