#include "block.hpp"
#include "cache.hpp"
#include <iostream>

// Constructor implementation
Cache::Cache(unsigned int blocksize, unsigned int size, unsigned int assoc, 
             unsigned int replacement_policy, unsigned int inclusion_property)
    : blocksize(blocksize), size(size), assoc(assoc),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property) 
{
     // Calculate number of sets.
     numSets = size / (blocksize * assoc);

     // Construct 2D cache with a row for each set and a number of 
     // block columns based on the cache's associativity.
     cache.resize(numSets, std::vector<Block>(assoc, Block(blocksize)));
}
