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
     Set(unsigned int assoc, unsigned int blocksize, ReplacementPolicy replacement_policy,
         const std::string cache_name, bool debug);

     void initialize(const Address &addr);

     std::vector<Block> blocks;
     std::queue<unsigned int> FIFO_indices;
     std::vector<unsigned int> LRU_counters;
     std::vector<Address> trace;

     bool isFull() const { return size == capacity; }

     std::optional<std::reference_wrapper<Block>> read(const Address &addr);
     std::optional<std::reference_wrapper<Block>> write(const Address &addr);
     std::optional<std::reference_wrapper<Block>> write(const Block &block);
     std::optional<std::reference_wrapper<Block>> search(const Address &addr);

     std::optional<std::reference_wrapper<Block>> allocate(const Address &addr);
     unsigned int getIdx(const Address &addr) const;

     
     void fillBlock(const Block &addr);
     void delete_block(const Address &addr);

     void increaseSize() { size++; }
     unsigned int getSize() { return size; }

     // Replacement policy methods
     Block replaceBlock_FIFO(const Address &addr);
     unsigned int get_FIFO_replacement();

     unsigned int get_LRU_replacement();
     void update_LRU(unsigned int idx);

     unsigned int get_optimal_replacement();
     void update_optimal() { trace_idx++; }

     void mark_used(unsigned int address, std::vector<unsigned int> &indices);

     void print_contents();
     void print_trace();
     void update_policy_output();
     void dirty_output();

private:
     void leftOut(std::string input);
     void outRight(std::string input);
     
     

     bool debug;

     unsigned int LRU;
     unsigned int size;
     unsigned int capacity;
     unsigned int assoc;
     unsigned int blocksize;
     unsigned int open_block;
     unsigned int trace_idx;

     const std::string cache_name;
     ReplacementPolicy replacement_policy;
};

#endif // SET_HPP
