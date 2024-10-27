#ifndef CACHE_HPP
#define CACHE_HPP

#include <memory>
#include <string>
#include <optional>
#include <vector>

#include "inclusion_property.hpp"
#include "replacement_policy.hpp"
#include "address.hpp"
#include "block.hpp"
#include "instruction.hpp"
#include "set.hpp"

class Cache
{

public:
     Cache(const std::string &name, unsigned int blocksize, unsigned int size, 
           unsigned int assoc,
           ReplacementPolicy replacement_policy, InclusionProperty inclusion_property,
           std::vector<Instruction> &instructions);

     std::optional<Block> read(unsigned int addr);
     Block write(unsigned int addr);

     std::optional<Block> search(unsigned int addr);

     // Setters
     void access() { numAccesses++; }

     // Getters
     unsigned int getAssoc() const { return assoc; }
     unsigned int getBlocksize() const { return blocksize; }
     unsigned int getSize() const { return size; }
     unsigned int getNumSets() const { return numSets; }
     unsigned int getNumAccesses() const { return numAccesses; }
     ReplacementPolicy getReplacementPolicy() const { return replacement_policy; }
     InclusionProperty getInclusionProperty() const { return inclusion_property; }
     const std::vector<Set> &getCache() const { return cache; }

     Cache *next_mem_level = NULL;
     const std::string &name;

private :
     void construct_set_traces(std::vector<Instruction> &instructions);

     unsigned int assoc;
     unsigned int blocksize;
     unsigned int size;
     unsigned int numSets;
     unsigned int numAccesses;
     ReplacementPolicy replacement_policy;
     InclusionProperty inclusion_property;
     std::vector<Instruction> instructions;
     std::vector<Set> cache;
};

#endif // CACHE_HPP
