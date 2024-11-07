#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstddef> // for std::size_t
#include <vector>  // for std::vector

#include "address.hpp"

class Block
{
     
public:
     // Constructors
     Block(std::size_t blocksize, const Address &addr);
     Block(std::size_t blocksize, const Address &addr, const unsigned char *inputData);

     // Destructor to clean up any resources
     ~Block();

     // Copy assignment operator
     Block &operator=(const Block &other);

     // Method to write a byte to the data array at a specific index
     void writeByte(std::size_t index, unsigned char value);

     // Method to read a byte from the data array at a specific index
     unsigned char readByte(std::size_t index) const;

     bool isDirty() { return dirtyBit == true; }
     bool isAvailable() { return empty == true; }

     // Setters
     void setDirty() { dirtyBit = true; }
     void unsetDirty() { dirtyBit = false; }
     void clear() { empty = true; }
     void occupy() { empty = false; }

     // Getters
     std::size_t getBlockSize() const { return blocksize; }
     const Address &getAddress() const { return address; }

private:
     bool empty;
     std::size_t blocksize; // Size of the block in bytes
     Address address;
     unsigned char *data; // Pointer to the data array (each cell stores a byte)
     bool dirtyBit = false;
};

#endif // BLOCK_HPP
