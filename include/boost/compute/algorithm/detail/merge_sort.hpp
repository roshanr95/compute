//---------------------------------------------------------------------------//
// Copyright (c) 2014 Roshan <thisisroshansmail@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://kylelutz.github.com/compute for more information.
//---------------------------------------------------------------------------//

#ifndef BOOST_COMPUTE_ALGORITHM_DETAIL_MERGE_SORT_HPP
#define BOOST_COMPUTE_ALGORITHM_DETAIL_MERGE_SORT_HPP

#include <boost/compute/algorithm/detail/insertion_sort.hpp>
#include <boost/compute/algorithm/inplace_merge.hpp>
#include <boost/compute/command_queue.hpp>
#include <boost/compute/detail/iterator_range_size.hpp>
#include <boost/compute/system.hpp>

namespace boost {
namespace compute {
namespace detail {

///
/// \brief Merge sort algorithm
///
/// Sorts the range [first, last) using the merge sort algorithm
///
/// \param first Iterator pointing to start of range
/// \param last Iterator pointing to end of range
/// \param comp Comparator
/// \param queue Queue on which to execute
///
template<class InputIterator, class Compare>
inline void merge_sort(InputIterator first,
                        InputIterator last,
                        Compare comp,
                        command_queue &queue)
{
    size_t serial_limit = 32;

    size_t count = iterator_range_size(first, last);

    if(count < 2) return;

    if(count <= serial_limit)
        return serial_insertion_sort(first, last, comp, queue);

    merge_sort(first, first + count/2, comp, queue);
    merge_sort(first + count/2, last, comp, queue);

    ::boost::compute::inplace_merge(first, first + count/2, last, comp, queue);
}

/// \overload
template<class InputIterator>
inline void merge_sort(InputIterator first,
                        InputIterator last,
                        command_queue &queue)
{
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    ::boost::compute::less<value_type> less_than;
    return merge_sort(first, last, less_than, queue);
}

} // end detail namespace
} // end compute namespace
} // end boost namespace

#endif // BOOST_COMPUTE_ALGORITHM_DETAIL_MERGE_SORT_HPP
