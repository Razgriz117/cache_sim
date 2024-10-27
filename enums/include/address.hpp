#ifndef ADDRESS_HPP
#define ADDRESS_HPP

class Address
{

public:
     Address(unsigned int address, unsigned int blocksize, unsigned int numSets);

     unsigned int value;
     unsigned int blockOffset;
     unsigned int setIndex;
     unsigned int tag;

     unsigned short offsetLength;
     unsigned short indexLength;
     unsigned short tagLength;
};

#endif // ADDRESS_HPP