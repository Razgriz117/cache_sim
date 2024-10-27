#ifndef MEM_ARCHITECTURE_SIM_HPP
#define MEM_ARCHITECTURE_SIM_HPP

#include <vector>    // for std::vector
#include <string>    // for std::string

#include "block.hpp"
#include "cache.hpp"
#include "instruction.hpp"

class MemArchitectureSim
{
public:
     // Constructor
     MemArchitectureSim(unsigned int blocksize, const std::vector<unsigned int> &cache_sizes,
                        const std::vector<unsigned int> &cache_assocs, unsigned int repl_policy,
                        unsigned int incl_property, const std::string &trace_file);

     void constructCaches();
     void addCache(const Cache &cache);
     void readInstructions();
     void printInstructions();

     Block write(const Address &addr);
     Block write_back(const Address &addr);

     // Getters
     unsigned int getBlocksize() const { return blocksize; }
     unsigned int getNumCaches() const { return numCaches; }
     ReplacementPolicy getReplacementPolicy() const { return replacement_policy; }
     InclusionProperty getInclusionProperty() const { return inclusion_property; }
     std::string  getTraceFile() const { return trace_file; }

private:
     Block writeToCache(unsigned int cache_idx, const Address &addr);

     unsigned int blocksize;
     ReplacementPolicy replacement_policy;
     InclusionProperty inclusion_property;
     std::string trace_file;
     std::vector<Instruction> instructions;
     std::size_t numCaches;
     std::vector<Cache> caches;
     const std::vector<unsigned int> &cache_assocs;
     const std::vector<unsigned int> &cache_sizes;
};

#endif // MEM_ARCHITECTURE_SIM_HPP