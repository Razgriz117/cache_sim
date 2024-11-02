#include <algorithm>
#include <optional>
#include <vector>

#include "inclusion_property.hpp"
#include "replacement_policy.hpp"

#include "address.hpp"
#include "block.hpp"
#include "set.hpp"

#define DIRECT_MAPPED 1
#define ONLY_BLOCK 0
#define FIRST_OF_REMAINING 0
#define MISS std::nullopt
#define EMPTY_BLOCK std::nullopt

Set::Set(unsigned int assoc, unsigned int blocksize, ReplacementPolicy replacement_policy)
    : assoc(assoc), blocksize(blocksize), replacement_policy(replacement_policy),
      LRU_counters(assoc)
{
     size = 0;
     capacity = assoc;

     // Initialize first insertion position.
     open_block = 0;
     trace_idx = 0;
}

std::optional<std::reference_wrapper<Block>> Set::read(const Address &addr)
{
     return search(addr);
}

std::optional<std::reference_wrapper<Block>> Set::write(const Address &addr)
{
     // If the set is not yet full, fill an empty block.
     if (!isFull())
     {
          fillBlock(addr);
          return EMPTY_BLOCK;
     }

     // Otherwise, determine victim block index.
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
     Block& victim_block = blocks[victim_idx];

     // Replace victim block.
     Block block = Block(blocksize, addr);
     blocks[victim_idx] = block;

     // Add data.
     // { Get data arg. Do something. Need tag. }

     // Mark block as dirty.
     blocks[victim_idx].setDirty();

     return victim_block;
}

std::optional<std::reference_wrapper<Block>> Set::search(const Address &addr)
{
     for (auto& block : blocks)
     {
          if (addr.tag == block.getAddress().tag)
               return block;
     }

     // If we reach this line, 
     return MISS;
}

void Set::delete_block(const Address &addr)
{
     auto result = search(addr);
     if (result)
     {
          Block& block = result->get();
          block.unsetDirty();
          size--;
     }
}

void Set::fillBlock(const Address &addr)
{
     // Create new block.
     Block block = Block(blocksize, addr);
     block.setDirty();

     // Add data.
     // { Get data arg. Do something }

     // Update replacement policy.
     FIFO_indices.push(open_block);
     LRU_counters[open_block] = LRU;
     LRU++;

     // If it's the first time filling the blocks array, push back.
     if (blocks.size() <= size)
     {
          blocks.push_back(block);
          size++;
          open_block++;
          return;
     }
     // Otherwise, insert block at the current open position and look for other open spot.
     else
     {
          blocks[open_block] = block;
          size++;

          // If the set still has open spaces, find the first open space to place the next
          // block.
          if (isFull()) return;
          else
          {
               for (int i = 0; i < assoc; i++)
               {
                    if (!blocks[i].isDirty())
                    {
                         open_block = i;
                         return;
                    }
               }
          }
     }
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

     // Create vector of yet to be used blocks' indices.
     std::vector<unsigned int> unused_block_indices;
     for (int i = 0; i < assoc; i++)
          unused_block_indices.push_back(i);

     // Track usage of blocks in set trace until one block in set is unused.
     unsigned int trace_address;
     while (trace_idx < trace.size())
     {
          // Mark any blocks which match the current trace address as used.
          trace_address = trace[trace_idx].value;
          mark_used(trace_address, unused_block_indices);
          trace_idx++;

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
