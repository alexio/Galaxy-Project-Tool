/***
 *  $Id$
 **
 *  File: algorithm.hpp
 *  Created: Sep 09, 2011
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2004-2012 Jaroslaw Zola
 *  Distributed under the Boost Software License.
 *  See accompanying LICENSE.
 *
 *  This file is part of jaz.
 */

#ifndef JAZ_ALGORITHM_HPP
#define JAZ_ALGORITHM_HPP

#include <algorithm>
#include <cstddef>
#include <ctime>
#include <functional>
#include <iterator>
#include <random>


namespace jaz {

  /** Function: copy_n
   */
  template <typename InputIter, typename Size, typename OutputIter>
  OutputIter copy_n(InputIter first, InputIter last, Size n, OutputIter out) {
      if (n > 0) {
	  Size count = 0;
	  for (; (first != last) && (count < n); ++first, ++count) {
	      *out++ = *first;
	  } // for
      } // if
      return out;
  } // copy_n


  /** Function: intersection_size
   */
  template <typename IterA, typename IterB, typename Pred>
  std::size_t intersection_size(IterA first_a, IterA last_a, IterB first_b, IterB last_b, Pred pred) {
      std::size_t S = 0;

      while ((first_a != last_a) && (first_b != last_b)) {
	  if (pred(*first_a, *first_b)) ++first_a;
	  else if (pred(*first_b, *first_a)) ++first_b;
	  else {
	      first_a++;
	      first_b++;
	      S++;
	  }
      } // while

      return S;
  } // intersection_size

  /** Function: intersection_size
   */
  template <typename IterA, typename IterB>
  inline std::size_t intersection_size(IterA first_a, IterA last_a, IterB first_b, IterB last_b) {
      typedef typename std::iterator_traits<IterA>::value_type value_type;
      return intersection_size(first_a, last_a, first_b, last_b, std::less<value_type>());
  } // intersection_size


  /** Function: count_unique
   */
  template <typename Iter, typename Pred>
  std::size_t count_unique(Iter first, Iter last, Pred pred) {
      if (first == last) return 0;
      std::size_t S = 1;
      Iter prev = first++;
      for (; first != last; ++first, ++prev) if (pred(*prev, *first) == false) ++S;
      return S;
  } // count_unique

  /** Function: count_unique
   */
  template <typename Iter>
  inline std::size_t count_unique(Iter first, Iter last) {
      typedef typename std::iterator_traits<Iter>::value_type value_type;
      return count_unique(first, last, std::equal_to<value_type>());
  } // count_unique


  /** Function: find_all
   */
  template <typename InputIter, typename OutputIter, typename Pred>
  OutputIter find_all(InputIter first, InputIter last, OutputIter out, Pred pred) {
      for (; first != last; ++first) {
	  if (pred(*first) == true) {
	      *out = first;
	      ++out;
	  }
      } // for
      return out;
  } // find_all


  /** Function: random_sample_n
   */
  template <typename InputIter, typename OutputIter, typename Size, typename Random>
  OutputIter random_sample_n(InputIter first, InputIter last, OutputIter out, Size n, Random&& rand) {
      Size m = std::distance(first, last);
      Size k = std::min(n, m);

      while (k != 0) {
  	  if ((rand() % m) < k) {
  	      *out = *first;
  	      ++out;
  	      --k;
  	  }
  	  --m;
  	  ++first;
      } // while

      return out;
  } // random_sample_n

  /** Function: random_sample_n
   */
  template <typename InputIter, typename OutputIter, typename Size>
  inline OutputIter random_sample_n(InputIter first, InputIter last, OutputIter out, Size n) {
      std::mt19937 rand;
      rand.seed(std::time(0));
      return random_sample_n(first, last, out, n, rand);
  } // random_sample_n


  /** Function: max_vectors
   */
  template <typename Iter, typename Pred>
  Iter max_vectors(Iter first, Iter last, Pred pred) {
      if (first == last) return first;

      Iter beg = first;
      Iter end = last;

      do {
	  Iter cur = beg;
	  Iter i = beg;

	  i++;

	  // find max element
	  for (; i != end; ++i) {
	      if (pred(*i, *beg) == true) {
		  --end; std::swap(*i, *end);
		  --i;
	      } else if (pred(*beg, *i) == true) {
		  std::swap(*i, *beg);
		  cur = i;
		  cur++;
	      }
	  } // for i

	  i = beg;
	  i++;

	  // clean
	  for (; (i != cur) && (i != end); ++i) {
	      if (pred(*i, *beg) == true) {
		  --end; std::swap(*i, *end);
		  --i;
	      }
	  } // for i

	  beg++;
      } while (beg != end);

      return end;
  } // max_vectors

} // namespace jaz

#endif // JAZ_ALGORITHM_HPP
