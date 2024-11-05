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
#include "output.hpp"

class MemArchitectureSim
{
public:
     // Constructor
     MemArchitectureSim(unsigned int blocksize, 
                        const std::vector<unsigned int> &cache_sizes,
                        const std::vector<unsigned int> &cache_assocs, 
                        unsigned int repl_policy, unsigned int incl_property, 
                        const std::string &trace_file,
                        Cache &main_memory, bool debug);

     void constructCaches();
     void addCache(const Cache &cache);
     void readInstructions();
     void printInstructions();
     void executeInstructions();
     void execute(Instruction &instruction);

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

     void print_contents();
     void print_debug();

private:
     Block writeToCache(unsigned int cache_idx, unsigned int address);
     void calculate_miss_rates();

     bool debug;

     unsigned int blocksize;
     ReplacementPolicy replacement_policy;
     InclusionProperty inclusion_property;
     std::string trace_file;
     std::vector<Instruction> instructions;
     std::size_t numCaches;
     std::size_t numNonEmptyCaches;
     std::vector<Cache> caches;
     Cache main_memory;
     unsigned int memory_traffic;
     const std::vector<unsigned int> &cache_assocs;
     const std::vector<unsigned int> &cache_sizes;

     std::vector<Output>outputs;
};

#endif // MEM_ARCHITECTURE_SIM_HPP