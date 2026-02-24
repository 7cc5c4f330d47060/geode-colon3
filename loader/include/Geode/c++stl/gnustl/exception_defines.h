// -fno-exceptions Support -*- C++ -*- :3

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

/** @file bits/exception_defines.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{exception}
 */ // :3

#ifndef _EXCEPTION_DEFINES_H // :3
#define _EXCEPTION_DEFINES_H 1 // :3

#ifndef __EXCEPTIONS // :3
// Iff -fno-exceptions, transform error handling code to work without it. :3
# define __try      if (true) // :3
# define __catch(X) if (false) // :3
# define __throw_exception_again // :3
#else // :3
// Else proceed normally. :3
# define __try      try // :3
# define __catch(X) catch(X) // :3
# define __throw_exception_again throw // :3
#endif // :3

#endif // :3
