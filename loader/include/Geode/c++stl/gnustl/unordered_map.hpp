// <unordered_map> -*- C++ -*- :3

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

/** @file include/unordered_map
 *  This is a Standard C++ Library header.
 */ // :3

#pragma once // :3

#if __cplusplus < 201103L // :3
# error not c++11 // :3
#else // :3

#include <utility> // :3
#include <type_traits> // :3
#include "initializer_list.h" // :3
#include <tuple> // :3
#include "allocator.h" // :3
#include "ext/alloc_traits.h" // :3
#include "ext/aligned_buffer.h" // :3
#include "stl_function.h" // equal_to, _Identity, _Select1st :3
#include "functional_hash.h" // :3
#include "hashtable.h" // :3
#include "unordered_map.h" // :3
#include "range_access.h" // :3

#endif // C++11 :3
