#include <cstring>
#include <stdexcept>

#include "block.hpp"

Block::Block(std::size_t blocksize)
    : blocksize(blocksize), data(new unsigned char[blocksize])
{
     // Initialize the data array to zero
     // std::memset(data, 0, blocksize); FIX CONSTRUCTOR
}

Block::Block(std::size_t blocksize, const unsigned char *inputData)
    : blocksize(blocksize), data(new unsigned char[blocksize])
{
     
     std::memcpy(data, inputData, blocksize);
}

// Destructor to free the dynamically allocated memory
Block::~Block()
{
     // delete[] data; FIX DESTRUCTOR
}

// Method to write a byte to the data array at the specified index
void Block::writeByte(std::size_t index, unsigned char value)
{
     if (index >= blocksize)
     {
          throw std::out_of_range("Index out of bounds");
     }
     data[index] = value;
}

// Method to read a byte from the data array at the specified index
unsigned char Block::readByte(std::size_t index) const
{
     if (index >= blocksize)
     {
          throw std::out_of_range("Index out of bounds");
     }
     return data[index];
}

// Getter method to retrieve the block size
std::size_t Block::getBlockSize() const
{
     return blocksize;
}
