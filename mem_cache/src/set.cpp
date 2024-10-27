#include <algorithm>
#include <vector>

#include "inclusion_property.hpp"
#include "replacement_policy.hpp"

#include "address.hpp"
#include "block.hpp"
#include "set.hpp"

#define DIRECT_MAPPED 1
#define ONLY_BLOCK 0
#define FIRST_OF_REMAINING 0

Set::Set(unsigned int assoc, unsigned int blocksize, ReplacementPolicy replacement_policy)
    : assoc(assoc), blocksize(blocksize), replacement_policy(replacement_policy),
      LRU_counters(assoc)
{
     size = 0;
     capacity = assoc;

     // Initialize first insertion position.
     open_block = 0;

     // replacement_policy = static_cast<ReplacementPolicy>(repl_policy);
}

Block Set::write(const Address &addr)
{
     // Determine victim block index.
     unsigned int victim_idx;
     switch (replacement_policy)
     {
          case ReplacementPolicy::LRU:
               victim_idx = get_LRU_replacement(); break;

          case ReplacementPolicy::FIFO:
               victim_idx = get_FIFO_replacement(); break;

          case ReplacementPolicy::Optimal:
               victim_idx = get_optimal_replacement(); break;
     }

     // Get victim block.
     Block victim_block = blocks[victim_idx];

     // Replace victim block.
     Block block = Block(blocksize, addr);
     blocks[victim_idx] = block;

     // Add data.
     // { Get data arg. Do something. Need tag. }

     // Mark block as dirty.
     blocks[victim_idx].setDirty();

     return victim_block;
}

void Set::fillBlock(const Address &addr)
{
     // Create new block.
     Block block = Block(blocksize, addr);
     block.setDirty();

     // Place block in open block position.
     blocks.push_back(block);

     // Add data.
     // { Get data arg. Do something }

     // Update replacement policy.
     FIFO_indices.push(open_block);
     LRU_counters[open_block] = LRU;
     LRU++;

     open_block++;
     size++;
}

Block Set::replaceBlock_FIFO(const Address &addr)
{
     // Determine victim block.
     unsigned int victim_idx = get_FIFO_replacement();
     Block victim_block = blocks[victim_idx];

     // Replace victim block.
     Block block = Block(blocksize, addr);
     blocks[victim_idx] = block;

     // Add data.
     // { Get data arg. Do something. Need tag. }

     // Mark block as dirty.
     blocks[victim_idx].setDirty();

     return victim_block;
}

unsigned int Set::get_LRU_replacement()
{
     // Direct mapped cache always replaces the same block.
     if (assoc == DIRECT_MAPPED)
          return ONLY_BLOCK;

     // Get iterator to the minimum element
     auto minIt = std::min_element(LRU_counters.begin(), LRU_counters.end());

     // Calculate the index from the iterator
     unsigned int victim_idx = std::distance(LRU_counters.begin(), minIt);

     // Update most recently used.
     LRU_counters[victim_idx] = LRU;
     LRU++;

     return victim_idx;
}

unsigned int Set::get_FIFO_replacement()
{
     // Direct mapped cache always replaces the same block.
     if (assoc == DIRECT_MAPPED)
          return ONLY_BLOCK;

     unsigned int victim_idx = FIFO_indices.front();
     FIFO_indices.pop();
     FIFO_indices.push(victim_idx);
     return victim_idx;
}

unsigned int Set::get_optimal_replacement()
{
     // Direct mapped cache always replaces the same block.
     if (assoc == DIRECT_MAPPED)
          return ONLY_BLOCK;

     std::vector<unsigned int> unused_block_indices;
     for (int i = 0; i < assoc; i++)
          unused_block_indices.push_back(i);

     unsigned int trace_address;
     for (int i = 0; i < trace.size(); i++)
     {
          // Mark any blocks which match the current trace address as used.
          trace_address = trace[i].value;
          mark_used(trace_address, unused_block_indices);

          // If we've removed all but one block from our list of unused,
          // the only block left is either last to be reused or never used at all.
          if (unused_block_indices.size() == 1)
               return ONLY_BLOCK;
     }

     // If we reach this line, there is a tie for unused blocks. Return the first of them.
     return FIRST_OF_REMAINING;
}

void Set::mark_used(unsigned int address, std::vector<unsigned int> &indices)
{
     // Check each block in our unused blocks list to see if it matches the current trace.
     for (unsigned int index : indices)
     {
          // Each time a trace address matches an item in our list of unused blocks,
          // mark that block as used (i.e. remove from list of unused).
          if (blocks[index].getAddress().value == address)
          {
               // Removes unique cell containing "index" from indices vector.
               auto it = std::find(indices.begin(), indices.end(), index);
               if (it != indices.end()) indices.erase(it);

               // Removes all instances of index from indices vector.
               // indices.erase(
               //     std::remove(indices.begin(), indices.end(), index), indices.end()
               // );
          }
     }
}
