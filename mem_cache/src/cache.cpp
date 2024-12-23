#include <iostream>
#include <iomanip>
#include <optional>

#include "address.hpp"
#include "block.hpp"
#include "cache.hpp"
#include "instruction.hpp"
#include "set.hpp"

#include "output.hpp"

#define MISS std::nullopt
#define LOAD_FAILURE std::nullopt
#define EMPTY_BLOCK std::nullopt

#define VERBOSE true

// Constructor implementation
Cache::Cache(const std::string name, unsigned int blocksize, unsigned int size,
             unsigned int assoc,
             ReplacementPolicy replacement_policy, InclusionProperty inclusion_property,
             std::vector<Instruction> &instructions, bool debug)

    : name(name), blocksize(blocksize), size(size), assoc(assoc),
      replacement_policy(replacement_policy), inclusion_property(inclusion_property),
      numAccesses(0), reads(0), read_misses(0), writes(0), write_misses(0), write_backs(0),
      miss_rate(0.0), debug(debug)
{
     // Calculate number of sets.
     numSets = size / (blocksize * assoc);

     // Initialize the cache with Set objects, each set containing `assoc` blocks.
     for (unsigned int i = 0; i < numSets; ++i)
     {
          auto defaultAddr = Address(0, blocksize, numSets);
          cache.emplace_back(Set(assoc, blocksize, replacement_policy, name, debug));
          cache[i].initialize(defaultAddr);
          // cache[i].increaseSize();
     }

     // Construct set traces for Optimal replacement policy.
     if (replacement_policy == ReplacementPolicy::Optimal)
          construct_set_traces(instructions);
}

std::optional<std::reference_wrapper<Block>> Cache::read(unsigned int addr)
{
     // Increment cache accesses.
     access();
     reads++;
     op_output("read", addr);

     auto address = Address(addr, blocksize, numSets);
     Set &set = cache[address.setIndex];

     // For main memory, reads never miss, so we return a valid block.
     if (name == "MAIN_MEMORY")
     {
          // Block &newBlock = *(new Block(blocksize, address));
          Block block(blocksize, address);
          Block &newBlock = block;
          return newBlock;
     }

     // Read from current cache.
     auto result = set.search(address);
     if (result)
     {
          Block &found_block = result->get();
          hit_output();
          unsigned int idx = set.getIdx(address);
          set.update_LRU(idx);
          
          set.update_optimal();
          return found_block;
     }

     // If we miss, read from lower level caches.
     if (result == MISS)
     {
          read_misses++;
          miss_output();
          set.update_optimal();
          if (next_mem_level != NULL)
          {
               allocate(addr);
               auto result = next_mem_level->read(addr);
               if (result)
               {
                    Block &loaded_block = result->get();
                    
                    return loaded_block;
               }
          }
     }

     // Error reading block.
     
     return LOAD_FAILURE;
}

std::optional<std::reference_wrapper<Block>> Cache::allocate(unsigned int addr)
{

     // The only modifications to memory should be done with write, not allocate.
     if (name == "MAIN_MEMORY")
     {
          return EMPTY_BLOCK;
     }

     // access(); Shouldn't access because we accessed during read or write to get here?
     // writes++;

     // Decode address.
     auto address = Address(addr, blocksize, numSets);
     Set &set = cache[address.setIndex];

     // Victim output
     auto hit = set.search(address);
     if (hit)
     {
     }
     else if (!set.isFull())
     {
          no_victim_output();
     }
     else
     {
          unsigned int victim_idx = set.get_LRU_replacement();
          victim_output(set.blocks[victim_idx]);
     }

     // Write to the set marked by the address's set index.
     bool displaced_victim = false;
     auto victim = set.allocate(address);
     if (victim)
     {
          Block victim_block = victim->get();
          // victim_output(victim_block);
          displaced_victim = true;
     }
     else
     {
          // no_victim_output();
     }

     // If we evicted a block during allocation, write back to next level of memory.
     if (displaced_victim)
     {
          Block victim_block = victim->get();
          if (victim_block.isDirty() && next_mem_level != NULL)
          {
               write_backs++;
               next_mem_level->write(victim_block.getAddress().value);
          }
          Block &victim_ref = victim_block;
          return victim_ref;
     }

     // Maintain inclusive property.
     if ((inclusion_property == InclusionProperty::Inclusive) && prev_mem_level != NULL)
     {
          // If block was evicted, remove it from lower level caches.
          if (displaced_victim)
          {
               Block victim_block = victim->get();
               unsigned int victim_address = victim_block.getAddress().value;
               prev_mem_level->delete_block(victim_address);
          }
     }

     // No victim block.
     return EMPTY_BLOCK;
}

std::optional<std::reference_wrapper<Block>> Cache::write(unsigned int addr)
{
     // Increment cache accesses.
     access();
     writes++;
     op_output("write", addr);

     // For main memory, writes never miss and no victim exists.
     if (name == "MAIN_MEMORY")
     {
          return EMPTY_BLOCK;
     }

     // op_output("write", addr); Put it here to remove MAIN_MEMORY output

     // Decode address.
     auto address = Address(addr, blocksize, numSets);
     Set &set = cache[address.setIndex];

     // Load block if it already exists in cache.
     bool miss_flag = false;
     std::optional<std::reference_wrapper<Block>> found_block = EMPTY_BLOCK;
     auto result = set.search(address);
     if (result)
     {
          found_block = result->get();
          hit_output();
          set.update_optimal();
     }

     // If we miss, attempt to update block read from lower level caches.
     else if (result == MISS)
     {
          miss_flag = true;
          write_misses++;
          miss_output();
     }

     Block block(blocksize, address);
     if (miss_flag && next_mem_level != NULL)
     {
          found_block = next_mem_level->read(addr);
     }

     // Write to the set marked by the address's set index.
     auto victim = set.write(block);
     bool displaced_victim = false;
     if (victim)
     {
          Block victim_block = victim->get();
          victim_output(victim_block);
          displaced_victim = true;
     }
     else
          no_victim_output();

     // If we evicted a block during writing, write back to next level of memory.
     if (displaced_victim)
     {
          Block victim_block = victim->get();
          if (victim_block.isDirty() && next_mem_level != NULL)
          {
               write_backs++;
               next_mem_level->write(victim_block.getAddress().value);
          }
          Block &victim_ref = victim_block;
          set.dirty_output();
          set.update_optimal();
          return victim_ref;
     }

     // Maintain inclusive property.
     if ((inclusion_property == InclusionProperty::Inclusive) && prev_mem_level != NULL)
     {
          // If block was evicted, remove it from lower level caches.
          if (displaced_victim)
          {
               Block victim_block = victim->get();
               unsigned int victim_address = victim_block.getAddress().value;
               prev_mem_level->delete_block(victim_address);
          }
     }

     // No victim block.
     
     set.dirty_output();
     set.update_optimal();
     return EMPTY_BLOCK;
}

std::optional<std::reference_wrapper<Block>> Cache::search(unsigned int addr)
{
     // Decode address.
     auto address = Address(addr, blocksize, numSets);

     // Search for block in the specified set.
     Set &set = cache[address.setIndex];

     return set.search(address);
}

std::optional<std::reference_wrapper<Block>> Cache::load(unsigned int addr)
{
     // Load block from current cache, if present.
     auto result = search(addr);
     if (result)
     {
          Block& found_block = result->get();
          return found_block;
     }

     // Load from lower level caches, if any.
     if (next_mem_level != NULL)
          return next_mem_level->load(addr);

     // Load from main memory if not found in any cache.
     if (name == "MAIN_MEMORY")
     {
          auto address = Address(addr, blocksize, numSets);
          Block &newBlock = *(new Block(blocksize, address));
          return newBlock;
     }

     // Reached main memory and failed to load.
     return LOAD_FAILURE;
}

void Cache::delete_block(unsigned int addr)
{
     if (name == "MAIN_MEMORY")
          return;

     // Decode address.
     auto address = Address(addr, blocksize, numSets);

     // Search for block in the specified set.
     Set &set = cache[address.setIndex];

     set.delete_block(address);

     // Maintain inclusion property.
     if (inclusion_property == InclusionProperty::Inclusive && next_mem_level != NULL)
          next_mem_level->delete_block(addr);
}

void Cache::update_optimal(unsigned int addr)
{
     auto address = Address(addr, blocksize, numSets);

     return;
}

    double
    Cache::calculate_miss_rate()
{
     if (reads + writes == 0) return 0.0;
     unsigned int wrongs;
     if (write_misses == 0)
     {
          wrongs = 0;
     }
     else
     {
          wrongs = writes;
     }
     miss_rate = static_cast<double>(read_misses + write_misses) / (reads + wrongs);
     return miss_rate;
}

void Cache::construct_set_traces(std::vector<Instruction> &instructions)
{
     for (auto &instruction : instructions)
     {

          // Extract address from instruction.
          auto address = Address(instruction.address, blocksize, numSets);

          for (int curr_set = 0; curr_set < numSets; curr_set++)
          {
               // Append address to set trace if it maps to the current set.
               if (address.setIndex == curr_set)
               {
                    // Get current set.
                    Set &set = cache[curr_set];
                    set.trace.push_back(address);
                    break;
               }
          }
     }

     // std::cout << "CACHE: " << name << std::endl;
     // for (int i = 0; i < cache.size(); i++)
     // {
     //      std::cout << "Set " << i << " trace: " << std::endl;
     //      for (auto addr : cache[i].trace)
     //      {
     //           std::cout << "  Address:  " << addr << std::endl;
     //           std::cout << "  Block: " << addr.blockToString() << std::endl;
     //           std::cout << std::endl;
     //      }
     // }
}

void Cache::print_contents()
{
     for (int i = 0; i < numSets; i++)
     {
          std::string set = std::to_string(i) + ":";
          Output::leftOut("Set"); Output::leftOut(set);
          cache[i].print_contents();
     }
}

void Cache::address_output(const Address &address)
{
     if (!debug || name == "MAIN_MEMORY")
          return;

     unsigned int tag = address.tag;
     unsigned int index = address.setIndex;

     std::ostringstream tag_stream;
     tag_stream << std::hex << tag;

     std::ostringstream address_stream;
     address_stream << std::hex << address.blockPrefix;

     std::cout << address_stream.str() << " ";
     std::cout << "(tag " << tag_stream.str() << ", index " << index;
}

void Cache::block_output(Block &block)
{
     if (!debug || name == "MAIN_MEMORY")
          return;

     auto& address = block.getAddress();
     address_output(address);

     std::string cleanliness{};
     if (block.isDirty())
          cleanliness = "dirty";
     else
          cleanliness = "clean";

     std::cout << ", " << cleanliness << ")" << std::endl;
}

void Cache::victim_output(Block &block)
{
     if (!debug || name == "MAIN_MEMORY")
          return;

     std::cout << name << " victim: ";
     block_output(block);
}

void Cache::no_victim_output()
{
     if (!debug || name == "MAIN_MEMORY")
          return;

     std::cout << name << " victim: none" << std::endl;
}

void Cache::op_output(std::string op, unsigned int addr)
{
     if (!debug || name == "MAIN_MEMORY") 
          return;

     auto address = Address(addr, blocksize, numSets);
     std::cout << name << " " << op << " : ";
     address_output(address);
     std::cout << ")" << std::endl;
}

void Cache::hit_output()
{
     if (!debug || name == "MAIN_MEMORY")
          return;

     std::cout << name << " hit" << std::endl;
}


void Cache::miss_output()
{
     if (!debug || name == "MAIN_MEMORY")
          return;

     std::cout << name << " miss" << std::endl;
}