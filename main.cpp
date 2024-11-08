// Standard libraries
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <limits>
#include <vector>

// Enums
#include "replacement_policy.hpp"
#include "inclusion_property.hpp"

// Local libraries
#include "cache.hpp"
#include "instruction.hpp"
#include "mem_architecture_sim.hpp"

// Global constants
#define DECIMAL 10
#define DIRECT_MAPPED 1
#define FORMAT_SPACE 23
const bool DEBUG = false;

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

void out(std::string var, std::string val)
{
     using std::cout;
     using std::endl;
     using std::left;
     using std::setw;
     cout << left << setw(FORMAT_SPACE) << var << val << endl;
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

     // Decode input.
     ReplacementPolicy policy = static_cast<ReplacementPolicy>(REPLACEMENT_POLICY);
     InclusionProperty property = static_cast<InclusionProperty>(INCLUSION_PROPERTY);
     std::string replacement_policy;
     std::string inclusion_property;
     switch (policy)
     {
          case ReplacementPolicy::LRU: replacement_policy = "LRU"; break;
          case ReplacementPolicy::FIFO: replacement_policy = "FIFO"; break;
          case ReplacementPolicy::Optimal: replacement_policy = "Optimal"; break;
     }
     switch (property)
     {
          case InclusionProperty::NonInclusive: inclusion_property = "non-inclusive"; break;
          case InclusionProperty::Inclusive: inclusion_property = "inclusive"; break;
     }

     // Display input parameters.
     std::cout << "===== Simulator configuration =====" << std::endl;
     out("BLOCKSIZE:", std::to_string(BLOCKSIZE));
     out("L1_SIZE:", std::to_string(L1_SIZE));
     out("L1_ASSOC:", std::to_string(L1_ASSOC));
     out("L2_SIZE:", std::to_string(L2_SIZE));
     out("L2_ASSOC:", std::to_string(L2_ASSOC));
     out("REPLACEMENT_POLICY:", replacement_policy);
     out("INCLUSION_PROPERTY:", inclusion_property);
     out("trace_file:", trace_file);

     std::vector<unsigned int> CACHE_SIZES = {L1_SIZE, L2_SIZE};
     std::vector<unsigned int> CACHE_ASSOCS = {L1_ASSOC, L2_ASSOC};

     // Create main memory.
     unsigned int main_memory_size = 0;
     std::vector<Instruction> mem_instructions;
     for (unsigned int cache_size : CACHE_SIZES) main_memory_size += cache_size;
     Cache main_memory = Cache(
         "MAIN_MEMORY",
         BLOCKSIZE,
         main_memory_size,
         DIRECT_MAPPED,
         static_cast<ReplacementPolicy>(REPLACEMENT_POLICY),
         static_cast<InclusionProperty>(INCLUSION_PROPERTY),
         mem_instructions,
         DEBUG
     );

     // Construct cache simulator.
     MemArchitectureSim simulator(
          BLOCKSIZE,
          CACHE_SIZES,
          CACHE_ASSOCS,
          REPLACEMENT_POLICY,
          INCLUSION_PROPERTY,
          trace_file,
          main_memory,
          DEBUG
     );

     return 0;
}
