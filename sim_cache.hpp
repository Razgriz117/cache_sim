#ifndef SIM_CACHE_HPP
#define SIM_CACHE_HPP

#include <vector>    // for std::vector
#include <string>    // for std::string

#include "cache.hpp"
#include "instruction.hpp"

class Sim_Cache
{
public:
     // Constructor
     Sim_Cache(unsigned int blocksize, const std::vector<unsigned int> &cache_sizes,
               const std::vector<unsigned int> &cache_assocs, unsigned int replacement_policy,
               unsigned int inclusion_property, const std::string &trace_file);

     void constructCaches();
     void addCache(const Cache &cache);
     void readInstructions();
     void printInstructions();

     // Getters
     unsigned int getBlocksize() const { return blocksize; }
     unsigned int getNumCaches() const { return numCaches; }
     unsigned int getReplacementPolicy() const { return replacement_policy; }
     unsigned int getInclusionProperty() const { return inclusion_property; }
     std::string  getTraceFile() const { return trace_file; }

private:
     unsigned int blocksize;
     unsigned int replacement_policy;
     unsigned int inclusion_property;
     std::string trace_file;
     std::vector<Instruction> instructions;
     std::size_t numCaches;
     std::vector<Cache> caches;
     const std::vector<unsigned int> &cache_assocs;
     const std::vector<unsigned int> &cache_sizes;
};

#endif // SIM_CACHE_HPP