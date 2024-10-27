#ifndef INCLUSION_PROPERTY_HPP
#define INCLUSION_PROPERTY_HPP

enum class InclusionProperty
{
     NonInclusive = 0, // Non-Inclusive Cache
     Inclusive = 1     // Inclusive Cache
};

// Comparison function to check if an enum is equal to an unsigned short.
inline bool operator==(InclusionProperty property, unsigned short value)
{
     return static_cast<unsigned short>(property) == value;
}

// Overload the reverse for symmetry.
inline bool operator==(unsigned short value, InclusionProperty property)
{
     return property == value;
}

#endif // INCLUSION_PROPERTY_HPP
