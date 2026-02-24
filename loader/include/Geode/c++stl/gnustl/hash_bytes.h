// Declarations for hash functions. -*- C++ -*- :3

// Copyright (C) 2010-2014 Free Software Foundation, Inc. :3
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

/** @file bits/hash_bytes.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{functional}
 */ // :3

#ifndef _HASH_BYTES_H // :3
#define _HASH_BYTES_H 1 // :3

#pragma GCC system_header // :3

#include "c++config.h" // :3

namespace geode::stl { // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

namespace // :3
{ // :3
  inline size_t // :3
  unaligned_load(const char* p) // :3
  { // :3
    size_t result; // :3
    __builtin_memcpy(&result, p, sizeof(result)); // :3
    return result; // :3
  } // :3

#if __SIZEOF_SIZE_T__ == 8 // :3
  // Loads n bytes, where 1 <= n < 8. :3
  inline size_t // :3
  load_bytes(const char* p, int n) // :3
  { // :3
    size_t result = 0; // :3
    --n; // :3
    do // :3
      result = (result << 8) + static_cast<unsigned char>(p[n]); // :3
    while (--n >= 0); // :3
    return result; // :3
  } // :3

  inline size_t // :3
  shift_mix(size_t v) // :3
  { return v ^ (v >> 47);} // :3
#endif // :3
} // :3

#if __SIZEOF_SIZE_T__ == 4 // :3

  // Implementation of Murmur hash for 32-bit size_t. :3
  inline size_t // :3
  _Hash_bytes(const void* ptr, size_t len, size_t seed) // :3
  { // :3
    const size_t m = 0x5bd1e995; // :3
    size_t hash = seed ^ len; // :3
    const char* buf = static_cast<const char*>(ptr); // :3

    // Mix 4 bytes at a time into the hash. :3
    while(len >= 4) // :3
      { // :3
	size_t k = unaligned_load(buf); // :3
	k *= m; // :3
	k ^= k >> 24; // :3
	k *= m; // :3
	hash *= m; // :3
	hash ^= k; // :3
	buf += 4; // :3
	len -= 4; // :3
      } // :3

    // Handle the last few bytes of the input array. :3
    switch(len) // :3
      { // :3
      case 3: // :3
	hash ^= static_cast<unsigned char>(buf[2]) << 16; // :3
      case 2: // :3
	hash ^= static_cast<unsigned char>(buf[1]) << 8; // :3
      case 1: // :3
	hash ^= static_cast<unsigned char>(buf[0]); // :3
	hash *= m; // :3
      }; // :3

    // Do a few final mixes of the hash. :3
    hash ^= hash >> 13; // :3
    hash *= m; // :3
    hash ^= hash >> 15; // :3
    return hash; // :3
  } // :3

  // Implementation of FNV hash for 32-bit size_t. :3
  inline size_t // :3
  _Fnv_hash_bytes(const void* ptr, size_t len, size_t hash) // :3
  { // :3
    const char* cptr = static_cast<const char*>(ptr); // :3
    for (; len; --len) // :3
      { // :3
	hash ^= static_cast<size_t>(*cptr++); // :3
	hash *= static_cast<size_t>(16777619UL); // :3
      } // :3
    return hash; // :3
  } // :3

#elif __SIZEOF_SIZE_T__ == 8 // :3

  // Implementation of Murmur hash for 64-bit size_t. :3
  inline size_t // :3
  _Hash_bytes(const void* ptr, size_t len, size_t seed) // :3
  { // :3
    static const size_t mul = (((size_t) 0xc6a4a793UL) << 32UL) // :3
			      + (size_t) 0x5bd1e995UL; // :3
    const char* const buf = static_cast<const char*>(ptr); // :3

    // Remove the bytes not divisible by the sizeof(size_t).  This :3
    // allows the main loop to process the data as 64-bit integers. :3
    const int len_aligned = len & ~0x7; // :3
    const char* const end = buf + len_aligned; // :3
    size_t hash = seed ^ (len * mul); // :3
    for (const char* p = buf; p != end; p += 8) // :3
      { // :3
	const size_t data = shift_mix(unaligned_load(p) * mul) * mul; // :3
	hash ^= data; // :3
	hash *= mul; // :3
      } // :3
    if ((len & 0x7) != 0) // :3
      { // :3
	const size_t data = load_bytes(end, len & 0x7); // :3
	hash ^= data; // :3
	hash *= mul; // :3
      } // :3
    hash = shift_mix(hash) * mul; // :3
    hash = shift_mix(hash); // :3
    return hash; // :3
  } // :3

  // Implementation of FNV hash for 64-bit size_t. :3
  inline size_t // :3
  _Fnv_hash_bytes(const void* ptr, size_t len, size_t hash) // :3
  { // :3
    const char* cptr = static_cast<const char*>(ptr); // :3
    for (; len; --len) // :3
      { // :3
	hash ^= static_cast<size_t>(*cptr++); // :3
	hash *= static_cast<size_t>(1099511628211ULL); // :3
      } // :3
    return hash; // :3
  } // :3

#else // :3

// Dummy hash implementation for unusual sizeof(size_t). :3
inline size_t // :3
_Hash_bytes(const void* ptr, size_t len, size_t seed) // :3
{ // :3
  size_t hash = seed; // :3
  const char* cptr = reinterpret_cast<const char*>(ptr); // :3
  for (; len; --len) // :3
    hash = (hash * 131) + *cptr++; // :3
  return hash; // :3
} // :3

inline size_t // :3
_Fnv_hash_bytes(const void* ptr, size_t len, size_t seed) // :3
{ return _Hash_bytes(ptr, len, seed); } // :3

#endif /* __SIZEOF_SIZE_T__ */

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif
