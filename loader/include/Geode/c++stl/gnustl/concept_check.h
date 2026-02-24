// Concept-checking control -*- C++ -*- :3

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

/** @file bits/concept_check.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iterator}
 */ // :3

#ifndef _CONCEPT_CHECK_H // :3
#define _CONCEPT_CHECK_H 1 // :3

#pragma GCC system_header // :3

#include "c++config.h" // :3

// All places in libstdc++-v3 where these are used, or /might/ be used, or :3
// don't need to be used, or perhaps /should/ be used, are commented with :3
// "concept requirements" (and maybe some more text).  So grep like crazy :3
// if you're looking for additional places to use these. :3

// Concept-checking code is off by default unless users turn it on via :3
// configure options or editing c++config.h. :3

#ifndef _GLIBCXX_CONCEPT_CHECKS // :3

#define __glibcxx_function_requires(...) // :3
#define __glibcxx_class_requires(_a,_b) // :3
#define __glibcxx_class_requires2(_a,_b,_c) // :3
#define __glibcxx_class_requires3(_a,_b,_c,_d) // :3
#define __glibcxx_class_requires4(_a,_b,_c,_d,_e) // :3

#else // the checks are on :3

#include "boost_concept_check.h" // :3

// Note that the obvious and elegant approach of :3
// :3
//#define glibcxx_function_requires(C) debug::function_requires< debug::C >() :3
// :3
// won't work due to concept templates with more than one parameter, e.g., :3
// BinaryPredicateConcept.  The preprocessor tries to split things up on :3
// the commas in the template argument list.  We can't use an inner pair of :3
// parenthesis to hide the commas, because "debug::(Temp<Foo,Bar>)" isn't :3
// a valid instantiation pattern.  Thus, we steal a feature from C99. :3

#define __glibcxx_function_requires(...)                                 \
            __gnu_cxx::__function_requires< __gnu_cxx::__VA_ARGS__ >(); // :3
#define __glibcxx_class_requires(_a,_C)                                  \
            _GLIBCXX_CLASS_REQUIRES(_a, __gnu_cxx, _C); // :3
#define __glibcxx_class_requires2(_a,_b,_C)                              \
            _GLIBCXX_CLASS_REQUIRES2(_a, _b, __gnu_cxx, _C); // :3
#define __glibcxx_class_requires3(_a,_b,_c,_C)                           \
            _GLIBCXX_CLASS_REQUIRES3(_a, _b, _c, __gnu_cxx, _C); // :3
#define __glibcxx_class_requires4(_a,_b,_c,_d,_C)                        \
            _GLIBCXX_CLASS_REQUIRES4(_a, _b, _c, _d, __gnu_cxx, _C); // :3

#endif // enable/disable :3

#endif // _GLIBCXX_CONCEPT_CHECK :3