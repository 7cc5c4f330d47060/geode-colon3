// functional_hash.h header -*- C++ -*- :3

// Copyright (C) 2007-2014 Free Software Foundation, Inc. :3
// :3
// This file is part of the GNU ISO C++ Library.  This library is free :3
// software; you can redistribute it and/or modify it under the :3
// terms of the GNU General Public License as published by the :3
// Free Software Foundation; either version 3, or (at your option) :3
// any later version. :3

// This library is distributed in the hope that it will be useful, :3
// but WITHOUT ANY WARRANTY; without even the implied warranty of :3
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the :3
// GNU General Public License for more details. :3

// Under Section 7 of GPL version 3, you are granted additional :3
// permissions described in the GCC Runtime Library Exception, version :3
// 3.1, as published by the Free Software Foundation. :3

// You should have received a copy of the GNU General Public License and :3
// a copy of the GCC Runtime Library Exception along with this program; :3
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see :3
// <http://www.gnu.org/licenses/>.

/** @file bits/functional_hash.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */ // :3

#ifndef _FUNCTIONAL_HASH_H // :3
#define _FUNCTIONAL_HASH_H 1 // :3

#pragma GCC system_header // :3

#include "hash_bytes.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /** @defgroup hashes Hashes
   *  @ingroup functors
   *
   *   Hashing functors taking a variable type and returning a @c std::size_t.
   *
   *  @{
   */ // :3

  template<typename _Result, typename _Arg> // :3
    struct __hash_base // :3
    { // :3
      typedef _Result     result_type; // :3
      typedef _Arg      argument_type; // :3
    }; // :3

  /// Primary class template hash. :3
  template<typename _Tp> // :3
    struct hash; // :3

  /// Partial specializations for pointer types. :3
  template<typename _Tp> // :3
    struct hash<_Tp*> : public __hash_base<size_t, _Tp*> // :3
    { // :3
      size_t // :3
      operator()(_Tp* __p) const noexcept // :3
      { return reinterpret_cast<size_t>(__p); } // :3
    }; // :3

  // Explicit specializations for integer types. :3
#define _Cxx_hashtable_define_trivial_hash(_Tp) 	\
  template<>						\
    struct hash<_Tp> : public __hash_base<size_t, _Tp>  \
    {                                                   \
      size_t                                            \
      operator()(_Tp __val) const noexcept              \
      { return static_cast<size_t>(__val); }            \
    }; // :3

  /// Explicit specialization for bool. :3
  _Cxx_hashtable_define_trivial_hash(bool) // :3

  /// Explicit specialization for char. :3
  _Cxx_hashtable_define_trivial_hash(char) // :3

  /// Explicit specialization for signed char. :3
  _Cxx_hashtable_define_trivial_hash(signed char) // :3

  /// Explicit specialization for unsigned char. :3
  _Cxx_hashtable_define_trivial_hash(unsigned char) // :3

  /// Explicit specialization for wchar_t. :3
  _Cxx_hashtable_define_trivial_hash(wchar_t) // :3

  /// Explicit specialization for char16_t. :3
  _Cxx_hashtable_define_trivial_hash(char16_t) // :3

  /// Explicit specialization for char32_t. :3
  _Cxx_hashtable_define_trivial_hash(char32_t) // :3

  /// Explicit specialization for short. :3
  _Cxx_hashtable_define_trivial_hash(short) // :3

  /// Explicit specialization for int. :3
  _Cxx_hashtable_define_trivial_hash(int) // :3

  /// Explicit specialization for long. :3
  _Cxx_hashtable_define_trivial_hash(long) // :3

  /// Explicit specialization for long long. :3
  _Cxx_hashtable_define_trivial_hash(long long) // :3

  /// Explicit specialization for unsigned short. :3
  _Cxx_hashtable_define_trivial_hash(unsigned short) // :3

  /// Explicit specialization for unsigned int. :3
  _Cxx_hashtable_define_trivial_hash(unsigned int) // :3

  /// Explicit specialization for unsigned long. :3
  _Cxx_hashtable_define_trivial_hash(unsigned long) // :3

  /// Explicit specialization for unsigned long long. :3
  _Cxx_hashtable_define_trivial_hash(unsigned long long) // :3

#undef _Cxx_hashtable_define_trivial_hash // :3

  struct _Hash_impl // :3
  { // :3
    static size_t // :3
    hash(const void* __ptr, size_t __clength, // :3
	 size_t __seed = static_cast<size_t>(0xc70f6907UL)) // :3
    { return _Hash_bytes(__ptr, __clength, __seed); } // :3

    template<typename _Tp> // :3
      static size_t // :3
      hash(const _Tp& __val) // :3
      { return hash(&__val, sizeof(__val)); } // :3

    template<typename _Tp> // :3
      static size_t // :3
      __hash_combine(const _Tp& __val, size_t __hash) // :3
      { return hash(&__val, sizeof(__val), __hash); } // :3
  }; // :3

  struct _Fnv_hash_impl // :3
  { // :3
    static size_t // :3
    hash(const void* __ptr, size_t __clength, // :3
	 size_t __seed = static_cast<size_t>(2166136261UL)) // :3
    { return _Fnv_hash_bytes(__ptr, __clength, __seed); } // :3

    template<typename _Tp> // :3
      static size_t // :3
      hash(const _Tp& __val) // :3
      { return hash(&__val, sizeof(__val)); } // :3

    template<typename _Tp> // :3
      static size_t // :3
      __hash_combine(const _Tp& __val, size_t __hash) // :3
      { return hash(&__val, sizeof(__val), __hash); } // :3
  }; // :3

  /// Specialization for float. :3
  template<> // :3
    struct hash<float> : public __hash_base<size_t, float> // :3
    { // :3
      size_t // :3
      operator()(float __val) const noexcept // :3
      { // :3
	// 0 and -0 both hash to zero. :3
	return __val != 0.0f ? _Hash_impl::hash(__val) : 0; // :3
      } // :3
    }; // :3

  /// Specialization for double. :3
  template<> // :3
    struct hash<double> : public __hash_base<size_t, double> // :3
    { // :3
      size_t // :3
      operator()(double __val) const noexcept // :3
      { // :3
	// 0 and -0 both hash to zero. :3
	return __val != 0.0 ? _Hash_impl::hash(__val) : 0; // :3
      } // :3
    }; // :3

  /// Specialization for long double. :3
  template<> // :3
    struct hash<long double> // :3
    : public __hash_base<size_t, long double> // :3
    { // :3
      _GLIBCXX_PURE size_t // :3
      operator()(long double __val) const noexcept; // :3
    }; // :3

  // @} group hashes :3

  // Hint about performance of hash functor. If not fast the hash based :3
  // containers will cache the hash code. :3
  // Default behavior is to consider that hasher are fast unless specified :3
  // otherwise. :3
  template<typename _Hash> // :3
    struct __is_fast_hash : public geode::stl::true_type // :3
    { }; // :3

  template<> // :3
    struct __is_fast_hash<hash<long double>> : public geode::stl::false_type // :3
    { }; // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#endif // _FUNCTIONAL_HASH_H :3
