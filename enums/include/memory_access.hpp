#ifndef MEMORY_ACCESS_HPP
#define MEMORY_ACCESS_HPP

enum class MemoryAccess
{
     Read = 0, // Memory read operation
     Write = 1 // Memory write operation
};

// Comparison function to check if an enum is equal to an unsigned short.
inline bool operator==(MemoryAccess access, unsigned short value)
{
     return static_cast<unsigned short>(access) == value;
}

// Overload the reverse for symmetry.
inline bool operator==(unsigned short value, MemoryAccess access)
{
     return access == value;
}

#endif // MEMORY_ACCESS_HPP
