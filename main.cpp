// Standard libraries
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>

// Local libraries
#include "cache.hpp"
#include "mem_architecture_sim.hpp"

// Global constants
#define DECIMAL 10

// Convert string to unsigned int with error checking.
unsigned int convertToUnsignedInt(const char *arg)
{
     char *end;
     unsigned long val = std::strtoul(arg, &end, DECIMAL);

     // Check if the conversion is valid and within the unsigned int range
     if (*end != '\0' || val > std::numeric_limits<unsigned int>::max())
     {
          std::cerr << "Error: Argument (" << arg << ") is not a valid unsigned integer." << std::endl;
          exit(1);
     }
     return static_cast<unsigned int>(val);
}

int main(int argc, char *argv[])
{
     // Check input parameters.
     if (argc != 9)
     {
          std::cerr << "Usage: " << argv[0] << " <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> "
                    << "<L2_SIZE> <L2_ASSOC> <REPLACEMENT_POLICY> <INCLUSION_PROPERTY> " 
                    << "<trace_file>" << std::endl;
          return 1;
     }

     // Parameters
     unsigned int BLOCKSIZE = convertToUnsignedInt(argv[1]);
     unsigned int L1_SIZE = convertToUnsignedInt(argv[2]);
     unsigned int L1_ASSOC = convertToUnsignedInt(argv[3]);
     unsigned int L2_SIZE = convertToUnsignedInt(argv[4]);
     unsigned int L2_ASSOC = convertToUnsignedInt(argv[5]);
     unsigned int REPLACEMENT_POLICY = convertToUnsignedInt(argv[6]);
     unsigned int INCLUSION_PROPERTY = convertToUnsignedInt(argv[7]);
     std::string trace_file = argv[8];

     // Display parsed values
     std::cout << "Parsed Values:" << std::endl;
     std::cout << "BLOCKSIZE: " << BLOCKSIZE << std::endl;
     std::cout << "L1_SIZE: " << L1_SIZE << std::endl;
     std::cout << "L1_ASSOC: " << L1_ASSOC << std::endl;
     std::cout << "L2_SIZE: " << L2_SIZE << std::endl;
     std::cout << "L2_ASSOC: " << L2_ASSOC << std::endl;
     std::cout << "REPLACEMENT_POLICY: " << REPLACEMENT_POLICY << std::endl;
     std::cout << "INCLUSION_PROPERTY: " << INCLUSION_PROPERTY << std::endl;
     std::cout << "Trace File: " << trace_file << std::endl;

     std::vector<unsigned int> CACHE_SIZES = {L1_SIZE, L2_SIZE};
     std::vector<unsigned int> CACHE_ASSOCS = {L1_ASSOC, L2_ASSOC};

     MemArchitectureSim simulator(
         BLOCKSIZE,
         CACHE_SIZES,
         CACHE_ASSOCS,
         REPLACEMENT_POLICY,
         INCLUSION_PROPERTY,
         trace_file);

     return 0;
}
