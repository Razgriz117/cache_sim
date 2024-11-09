#include "address.hpp"
#include <cmath>

// Helper function to compute log base 2
unsigned int log2(unsigned int n)
{
     return static_cast<unsigned int>(std::log2(n));
}

// Address class constructor
Address::Address(unsigned int address, unsigned int blockSize, unsigned int numSets)
    : value(address)
{
     // Calculate lengths based on block size and number of sets
     offsetLength = log2(blockSize);
     indexLength = log2(numSets);
     tagLength = 32 - offsetLength - indexLength;

     // Mask to extract the least significant `offsetLength` bits
     // e.g. offsetLength = 4
     //   offsetMask:
     //        0000 ... 0000 0001, Initial
     //        0000 ... 0001 0000, left shift by offset length of 4
     //        0000 ... 0000 1111, subtract 0001
     unsigned int offsetMask = (1 << offsetLength) - 1;
     blockOffset = address & offsetMask;

     // Mask to extract the `indexLength` bits after the offset bits
     // e.g. offsetLength = 4, indexLength = 3
     //   offsetMask:
     //        0000 ... 0000 0111, After 1 left shift by index length of 3, subtract 0001
     //        0000 ... 0111 0000, left shift by offset length of 4
     unsigned int indexMask = ((1 << indexLength) - 1) << offsetLength;
     setIndex = (address & indexMask) >> offsetLength;

     // Remaining bits are the tag
     tag = address >> (offsetLength + indexLength);

     // Shave off the block offset bits to get the prefix to each block.
     blockPrefix = address >> offsetLength;
     blockPrefix = blockPrefix << offsetLength;
}

std::string Address::blockToString()
{
     std::stringstream stream;
     stream << std::hex << blockPrefix;
     std::string block(stream.str());

     return block;
}