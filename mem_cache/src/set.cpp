#include "set.hpp"

Set::Set(unsigned int assoc, unsigned int blocksize)
    : assoc(assoc), blocksize(blocksize), blocks(assoc, Block(blocksize)),
      LRU_counters(assoc), set_trace(assoc)
{

     size = 0;
     capacity = assoc;
}

unsigned int Set::get_FIFO_replacement()
{
     unsigned int replacement_idx = FIFO_indices.front();
     FIFO_indices.pop();
     FIFO_indices.push(replacement_idx);
     return replacement_idx;
}
