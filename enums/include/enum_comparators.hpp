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
bool operator==(EnumClass num, unsigned short value)
{
     return static_cast<unsigned short>(num) == value;
}

// Overload the reverse for symmetry.
bool operator==(unsigned short value, EnumClass num)
{
     return num == value; // Calls the above function
}

// InclusionProperty
bool operator==(InclusionProperty property, unsigned short value)
{
     return static_cast<unsigned short>(property) == value;
}

bool operator==(unsigned short value, InclusionProperty property)
{
     return property == value;
}

// MemoryAccess
bool operator==(MemoryAccess access, unsigned short value)
{
     return static_cast<unsigned short>(access) == value;
}
bool operator==(unsigned short value, MemoryAccess access)
{
     return access == value;
}

// ReplacementPolicy
bool operator==(ReplacementPolicy policy, unsigned short value)
{
     return static_cast<unsigned short>(policy) == value;
}
bool operator==(unsigned short value, ReplacementPolicy policy)
{
     return policy == value;
}

#endif // ENUM_COMPARATORS_HPP