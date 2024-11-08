#include <cstring>
#include <stdexcept>

#include "block.hpp"
#include "address.hpp"

Block::Block(std::size_t blocksize, const Address &addr)
    : blocksize(blocksize), data(new unsigned char[blocksize]), address(addr), empty(false)
{
     // Initialize the data array to zero
     // std::memset(data, 0, blocksize); FIX CONSTRUCTOR
}

Block::Block(std::size_t blocksize, const Address &addr, const unsigned char *inputData)
    : blocksize(blocksize), data(new unsigned char[blocksize]), address(addr), empty(false)
{
     
     std::memcpy(data, inputData, blocksize);
}

// Destructor to free the dynamically allocated memory
Block::~Block()
{
     // delete[] data; FIX DESTRUCTOR
}

// Copy assignment operator
Block &Block::operator=(const Block &other)
{
     // If we assign a block to itself in memory, assign its reference (i.e. do nothing).
     if (this == &other)
     {
          return *this;
     }

     // If blocksize differs, we have incompatible blocks.
     if (blocksize != other.blocksize)
     {
          throw std::runtime_error("Mismatched block sizes in assignment");
     }

     // Otherwise, copy the data contents from the right argument's object into the left's.
     std::memcpy(data, other.data, blocksize);

     auto copyAddress = Address(other.address);
     this->address = copyAddress;
     this->empty = other.empty;
     this->dirtyBit = other.dirtyBit;

     // Address reference remains the same
     return *this;
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
