#ifndef CACHE_HPP
#define CACHE_HPP

#include <memory>
#include <string>
#include <functional>
#include <optional>
#include <vector>

#include "output.hpp"
#include "inclusion_property.hpp"
#include "replacement_policy.hpp"
#include "address.hpp"
#include "block.hpp"
#include "instruction.hpp"
#include "set.hpp"

class Cache
{

public:
     Cache(const std::string name, unsigned int blocksize, unsigned int size, 
           unsigned int assoc,
           ReplacementPolicy replacement_policy, InclusionProperty inclusion_property,
           std::vector<Instruction> &instructions, bool debug);

     std::optional<std::reference_wrapper<Block>> read(unsigned int addr);
     std::optional<std::reference_wrapper<Block>> write(unsigned int addr);
     std::optional<std::reference_wrapper<Block>> search(unsigned int addr);
     std::optional<std::reference_wrapper<Block>> load(unsigned int addr);

     std::optional<std::reference_wrapper<Block>> allocate(unsigned int addr);

     void delete_block(unsigned int addr);

     // Setters
     void access() { numAccesses++; }
     double calculate_miss_rate();

     // Getters
     unsigned int getAssoc() const { return assoc; }
     unsigned int getBlocksize() const { return blocksize; }
     unsigned int getSize() const { return size; }
     unsigned int getNumSets() const { return numSets; }
     unsigned int getNumAccesses() const { return numAccesses; }
     ReplacementPolicy getReplacementPolicy() const { return replacement_policy; }
     InclusionProperty getInclusionProperty() const { return inclusion_property; }
     const std::vector<Set> &getCache() const { return cache; }

     void print_contents();

     Cache *prev_mem_level = NULL;
     Cache *next_mem_level = NULL;
     const std::string name;

     unsigned int reads;
     unsigned int read_misses;
     unsigned int writes;
     unsigned int write_misses;
     double miss_rate;
     unsigned int write_backs;
     unsigned int numAccesses;

private :
     void construct_set_traces(std::vector<Instruction> &instructions);
     void address_output(const Address &address);
     void block_output(Block &block);
     void op_output(std::string op, unsigned int addr);
     void hit_output();
     void miss_output();
     void victim_output(Block &block);
     void no_victim_output();

     bool debug;

     unsigned int assoc;
     unsigned int blocksize;
     unsigned int size;
     unsigned int numSets;

     ReplacementPolicy replacement_policy;
     InclusionProperty inclusion_property;
     std::vector<Instruction> instructions;
     std::vector<Set> cache;
};

#endif // CACHE_HPP
