#include <iostream>
#include <iomanip>

#define FORMAT_SPACE_LEFT 8
#define FORMAT_SPACE_RIGHT 10

namespace Output
{

     inline void leftOut(std::string input)
     {
          using std::left;
          using std::setw;
          std::cout << left << setw(FORMAT_SPACE_LEFT) << input;
     }

     inline void outRight(std::string input)
     {
          using std::left;
          using std::setw;
          std::cout << left << setw(FORMAT_SPACE_RIGHT) << input;
     }

} // namespace Output