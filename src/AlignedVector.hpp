#ifndef ALIGNEDVECTOR_H
#define ALIGNEDVECTOR_H

#include <vector>
#include <boost/align/aligned_allocator.hpp>

/**
 * This is a verbose convenience alias for a std::vector with an aligned allocator.
 */
template <typename Value, std::size_t minimum_alignment = 16>
using AlignedVector = std::vector <Value,
                      boost::alignment::aligned_allocator<Value, minimum_alignment>>;

#endif // ALIGNEDVECTOR_H
