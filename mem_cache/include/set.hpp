#ifndef SET_HPP
#define SET_HPP

#include <vector>
#include <queue>
#include "block.hpp"

class Set
{
public:
     Set(unsigned int assoc, unsigned int blocksize);

     std::vector<Block> blocks;
     std::queue<int> FIFO_indices;
     std::vector<int> LRU_counters;
     std::vector<int> set_trace;

     bool isFull() const { return size == capacity; }
     unsigned int get_FIFO_replacement();

private:
     unsigned int size;
     unsigned int capacity;
     unsigned int assoc;
     unsigned int blocksize;
};

#endif // SET_HPP
