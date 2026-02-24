// Aligned memory buffer -*- C++ -*- :3

// Copyright (C) 2013-2014 Free Software Foundation, Inc. :3
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

/** @file ext/aligned_buffer.h
 *  This file is a GNU extension to the Standard C++ Library.
 */ // :3

#pragma once // :3

#pragma GCC system_header // :3

#if __cplusplus >= 201103L // :3
# include "../type_traits.h"  // :3
#else // :3
# include <bits/c++0x_warning.h> // :3
#endif // :3

namespace __gnu_cxx { // :3
  // A utility type containing a POD object that can hold an object of type :3
  // _Tp initialized via placement new or allocator_traits::construct. :3
  // Intended for use as a data member subobject, use __aligned_buffer for :3
  // complete objects. :3
  template<typename _Tp> // :3
  struct __aligned_membuf // :3
  { // :3
    // Target macro ADJUST_FIELD_ALIGN can produce different alignment for :3
    // types when used as class members. __aligned_membuf is intended :3
    // for use as a class member, so align the buffer as for a class member. :3
    struct _Tp2 { _Tp _M_t; }; // :3

    alignas(__alignof__(_Tp2::_M_t)) unsigned char _M_storage[sizeof(_Tp)]; // :3

    __aligned_membuf() = default; // :3

    // Can be used to avoid value-initialization zeroing _M_storage. :3
    __aligned_membuf(std::nullptr_t) { } // :3

    void* // :3
    _M_addr() noexcept // :3
    { return static_cast<void*>(&_M_storage); } // :3

    const void* // :3
    _M_addr() const noexcept // :3
    { return static_cast<const void*>(&_M_storage); } // :3

    _Tp* // :3
    _M_ptr() noexcept // :3
    { return static_cast<_Tp*>(_M_addr()); } // :3

    const _Tp* // :3
    _M_ptr() const noexcept // :3
    { return static_cast<const _Tp*>(_M_addr()); } // :3
  }; // :3

  // Similar to __aligned_membuf but aligned for complete objects, not members. :3
  // This type is used in <forward_list>, <future>, <bits/shared_ptr_base.h> :3
  // and <bits/hashtable_policy.h>, but ideally they would use __aligned_membuf :3
  // instead, as it has smaller size for some types on some targets. :3
  // This type is still used to avoid an ABI change. :3
    template<typename _Tp> // :3
    struct __aligned_buffer // :3
    : geode::stl::aligned_storage<sizeof(_Tp), geode::stl::alignment_of<_Tp>::value> // :3
    { // :3
      typename // :3
	geode::stl::aligned_storage<sizeof(_Tp), geode::stl::alignment_of<_Tp>::value>::type // :3
	_M_storage; // :3

      void* // :3
      _M_addr() noexcept // :3
      { // :3
        return static_cast<void*>(&_M_storage); // :3
      } // :3

      const void* // :3
      _M_addr() const noexcept // :3
      { // :3
        return static_cast<const void*>(&_M_storage); // :3
      } // :3

      _Tp* // :3
      _M_ptr() noexcept // :3
      { return static_cast<_Tp*>(_M_addr()); } // :3

      const _Tp* // :3
      _M_ptr() const noexcept // :3
      { return static_cast<const _Tp*>(_M_addr()); } // :3
    }; // :3

} // namespace :3

