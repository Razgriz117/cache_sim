#include "cache.hpp"
#include "set.hpp"
#include <iostream>

// Constructor implementation
Cache::Cache(unsigned int blocksize, unsigned int size, unsigned int assoc,
             unsigned int replacement_policy, unsigned int inclusion_property)
    : blocksize(blocksize), size(size), assoc(assoc),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property)
{
     // Calculate number of sets.
     numSets = size / (blocksize * assoc);

     // Initialize the cache with Set objects, each set containing `assoc` blocks
     for (unsigned int i = 0; i < numSets; ++i)
     {
          cache.emplace_back(Set(assoc, blocksize));
     }
}