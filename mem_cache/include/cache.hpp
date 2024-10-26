#ifndef CACHE_HPP
#define CACHE_HPP

#include <string>
#include <vector>

#include "inclusion_property.hpp"
#include "replacement_policy.hpp"
#include "set.hpp"

class Cache
{

public:
     Cache(unsigned int blocksize, unsigned int size, unsigned int assoc,
           unsigned int replacement_policy, unsigned int inclusion_property);

     // Getters
     unsigned int getAssoc() const { return assoc; }
     unsigned int getBlocksize() const { return blocksize; }
     unsigned int getSize() const { return size; }
     unsigned int getNumSets() const { return numSets; }
     unsigned int getReplacementPolicy() const { return replacement_policy; }
     unsigned int getInclusionProperty() const { return inclusion_property; }
     const std::vector<Set> &getCache() const { return cache; }

private : 
     unsigned int assoc;
     unsigned int blocksize;
     unsigned int size;
     unsigned int numSets;
     unsigned int replacement_policy;
     unsigned int inclusion_property;
     std::vector<Set> cache;
};

#endif // CACHE_HPP
