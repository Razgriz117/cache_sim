#ifndef REPLACEMENT_POLICY_HPP
#define REPLACEMENT_POLICY_HPP

enum class ReplacementPolicy
{
     LRU = 0,    // Least Recently Used
     FIFO = 1,   // First In, First Out
     Optimal = 2 // Optimal Replacement Policy
};

// Comparison function to check if an enum is equal to an unsigned short.
inline bool operator==(ReplacementPolicy policy, unsigned short value)
{
     return static_cast<unsigned short>(policy) == value;
}

// Overload the reverse for symmetry.
inline bool operator==(unsigned short value, ReplacementPolicy policy)
{
     return policy == value;
}

#endif // REPLACEMENT_POLICY_HPP
