#ifndef ENUM_COMPARATORS_HPP
#define ENUM_COMPARATORS_HPP

#include "inclusion_property.hpp"
#include "replacement_policy.hpp"
#include "memory_access.hpp"

enum class EnumClass
{
     Zero = 0, // Memory read operation
     One = 1 // Memory write operation
};

// Comparison function to check if an enum is equal to an unsigned short.
inline bool operator==(EnumClass num, unsigned short value)
{
     return static_cast<unsigned short>(num) == value;
}

// Overload the reverse for symmetry.
inline bool operator==(unsigned short value, EnumClass num)
{
     return num == value; // Calls the above function
}

#endif // ENUM_COMPARATORS_HPP