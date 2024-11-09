#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <iostream>
#include <iomanip>

class Address
{

public:
     Address(unsigned int address, unsigned int blocksize, unsigned int numSets);
     Address(const Address &addr)
         : tag(addr.tag), setIndex(addr.setIndex), blockOffset(addr.blockOffset),
           value(addr.value), blockPrefix(addr.blockPrefix),
           offsetLength(addr.offsetLength), indexLength(addr.indexLength),
           tagLength(addr.tagLength) {}

     std::string blockToString();

     unsigned int value;
     unsigned int blockOffset;
     unsigned int blockPrefix;
     unsigned int setIndex;
     unsigned int tag;

     unsigned short offsetLength;
     unsigned short indexLength;
     unsigned short tagLength;
};

inline std::ostream &operator<<(std::ostream &os, const Address &address)
{

     std::stringstream stream;
     stream << std::hex << address.value;
     std::string val(stream.str());

     os << val;
     return os;
}

#endif // ADDRESS_HPP