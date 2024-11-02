#ifndef MEM_ARCHITECTURE_SIM_HPP
#define MEM_ARCHITECTURE_SIM_HPP

#include <memory>
#include <optional>
#include <functional>
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
                        unsigned int incl_property, const std::string &trace_file,
                        Cache &main_memory);

     void constructCaches();
     void addCache(const Cache &cache);
     void readInstructions();
     void printInstructions();
     void executeInstructions();

     Block& read(unsigned int address);
     Block write(unsigned int address);
     Block write_back(unsigned int address);

     std::optional<std::reference_wrapper<Block>> search(unsigned int addr);

     // Getters
     unsigned int getBlocksize() const { return blocksize; }
     unsigned int getNumCaches() const { return numCaches; }
     ReplacementPolicy getReplacementPolicy() const { return replacement_policy; }
     InclusionProperty getInclusionProperty() const { return inclusion_property; }
     std::string  getTraceFile() const { return trace_file; }

private:
     Block writeToCache(unsigned int cache_idx, unsigned int address);

     unsigned int blocksize;
     ReplacementPolicy replacement_policy;
     InclusionProperty inclusion_property;
     std::string trace_file;
     std::vector<Instruction> instructions;
     std::size_t numCaches;
     std::vector<Cache> caches;
     Cache main_memory;
     const std::vector<unsigned int> &cache_assocs;
     const std::vector<unsigned int> &cache_sizes;
};

#endif // MEM_ARCHITECTURE_SIM_HPP