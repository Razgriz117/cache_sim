#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include "address.hpp"
#include "block.hpp"
#include "replacement_policy.hpp"

// Set appropriate format spacing for output
const int FORMAT_SPACE_LEFT = 8;
const int FORMAT_SPACE_RIGHT = 10;


     class Output
     {
     public:
          Output(std::string name, bool debug, unsigned int blocksize, 
                 unsigned int numSets, ReplacementPolicy replacement_policy)
              : name(name), debug(debug), blocksize(blocksize), numSets(numSets), 
                replacement_policy(replacement_policy) {}

          void address_output(const Address &address);
          void block_output(Block &block);
          void victim_output(Block &block);
          void op_output(const std::string &op, unsigned int addr);
          void hit_output();
          void miss_output();
          void update_policy_output();
          void dirty_output();

          // Inline functions
          static inline void leftOut(const std::string &input)
          {
               using std::left;
               using std::setw;
               std::cout << left << setw(FORMAT_SPACE_LEFT) << input;
               std::cout << setw(0);
          }

          static inline void outRight(const std::string &input)
          {
               using std::left;
               using std::setw;
               std::cout << left << setw(FORMAT_SPACE_RIGHT) << input;
               std::cout << setw(0);
          }

     private:
          std::string name;
          bool debug;
          unsigned int blocksize;
          unsigned int numSets;
          ReplacementPolicy replacement_policy;
     };

#endif // OUTPUT_HPP
