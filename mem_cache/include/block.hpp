#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstddef> // for std::size_t
#include <vector>  // for std::vector

class Block
{
     
public:
     // Constructors
     Block(std::size_t blocksize);
     Block(std::size_t blocksize, const unsigned char *data);

     // Destructor to clean up any resources
     ~Block();

     // Method to write a byte to the data array at a specific index
     void writeByte(std::size_t index, unsigned char value);

     // Method to read a byte from the data array at a specific index
     unsigned char readByte(std::size_t index) const;

     // Getter method to access block size
     std::size_t getBlockSize() const;

private:
     std::size_t blocksize; // Size of the block in bytes
     unsigned char *data;   // Pointer to the data array (each cell stores a byte)
};

#endif // BLOCK_HPP
