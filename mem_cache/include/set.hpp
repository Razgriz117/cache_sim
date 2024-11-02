#ifndef SET_HPP
#define SET_HPP

#include <optional>
#include <vector>
#include <queue>
#include "address.hpp"
#include "block.hpp"

class Set
{
public:
     Set(unsigned int assoc, unsigned int blocksize, ReplacementPolicy replacement_policy);

     std::vector<Block> blocks;
     std::queue<unsigned int> FIFO_indices;
     std::vector<unsigned int> LRU_counters;
     std::vector<Address> trace;

     bool isFull() const { return size == capacity; }

     std::optional<std::reference_wrapper<Block>> read(const Address &addr);
     std::optional<std::reference_wrapper<Block>> write(const Address &addr);
     std::optional<std::reference_wrapper<Block>> search(const Address &addr);

     void fillBlock(const Address &addr);
     void delete_block(const Address &addr);

     // Replacement policy methods
     Block replaceBlock_FIFO(const Address &addr);
     unsigned int get_FIFO_replacement();

     unsigned int get_LRU_replacement();
     unsigned int get_optimal_replacement();

     void mark_used(unsigned int address, std::vector<unsigned int> &indices);

private:
     ReplacementPolicy replacement_policy;
     unsigned int LRU;
     unsigned int size;
     unsigned int capacity;
     unsigned int assoc;
     unsigned int blocksize;
     unsigned int open_block;
     unsigned int trace_idx;
};

#endif // SET_HPP
