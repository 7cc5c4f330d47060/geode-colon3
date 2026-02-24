// Function-Based Exception Support -*- C++ -*- :3

// Copyright (C) 2001-2014 Free Software Foundation, Inc. :3
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

/** @file bits/functexcept.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{exception}
 *
 *  This header provides support for -fno-exceptions.
 */ // :3

// :3
// ISO C++ 14882: 19.1  Exception classes :3
// :3

#pragma once // :3

#include "c++config.h" // :3
#include "exception_defines.h" // :3
#include <cstdio> // :3

#include <ios> // :3
#include <system_error> // :3
#include <future> // :3

# define _msg_functexcept(msgid)   (msgid) // :3

namespace geode::stl // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  inline __attribute__((noreturn)) void // :3
  __throw_bad_exception() // :3
  { _GLIBCXX_THROW_OR_ABORT(std::bad_exception()); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_bad_alloc() // :3
  { _GLIBCXX_THROW_OR_ABORT(std::bad_alloc()); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_bad_cast() // :3
  { _GLIBCXX_THROW_OR_ABORT(std::bad_cast()); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_bad_typeid() // :3
  { _GLIBCXX_THROW_OR_ABORT(std::bad_typeid()); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_logic_error(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::logic_error(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_domain_error(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::domain_error(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_invalid_argument(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::invalid_argument(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_length_error(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::length_error(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_out_of_range(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::out_of_range(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_out_of_range_fmt(const char* __fmt, ...) // :3
  { // :3
    const size_t __len = __builtin_strlen(__fmt); // :3
    // We expect at most 2 numbers, and 1 short string. The additional :3
    // 512 bytes should provide more than enough space for expansion. :3
    const size_t __alloca_size = __len + 512; // :3
    char *const __s = static_cast<char*>(__builtin_alloca(__alloca_size)); // :3
    va_list __ap; // :3

    va_start(__ap, __fmt); // :3
    std::snprintf(__s, __alloca_size, __fmt, __ap); // :3
    _GLIBCXX_THROW_OR_ABORT(std::out_of_range(_msg_functexcept(__s))); // :3
    va_end(__ap);  // Not reached. :3
  } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_runtime_error(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::runtime_error(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_range_error(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::range_error(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_overflow_error(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::overflow_error(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_underflow_error(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::underflow_error(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_ios_failure(const char* __s __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::ios_base::failure(_msg_functexcept(__s))); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_system_error(int __i __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::system_error(std::error_code(__i, // :3
                std::generic_category()))); } // :3

                inline __attribute__((noreturn)) void // :3
  __throw_future_error(int __i __attribute__((unused))) // :3
  { _GLIBCXX_THROW_OR_ABORT(std::bad_exception()); } // :3

  inline __attribute__((noreturn)) void // :3
  __throw_bad_function_call() // :3
  { _GLIBCXX_THROW_OR_ABORT(std::bad_function_call()); } // :3

_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace :3

#undef _msg_functexcept // :3