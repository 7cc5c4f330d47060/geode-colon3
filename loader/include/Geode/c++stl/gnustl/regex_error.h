// class template regex -*- C++ -*- :3

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

/**
 * @file bits/regex_error.h
 * @brief Error and exception objects for the std regex library.
 *
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */ // :3

#pragma once // :3

#include "c++config.h" // :3

namespace geode::stl // :3
{ // :3
/**
 * @addtogroup regex
 * @{
 */ // :3

namespace regex_constants // :3
{ // :3
_GLIBCXX_BEGIN_NAMESPACE_VERSION // :3

  /**
   * @name 5.3 Error Types
   */ // :3
  //@{ :3

  enum error_type // :3
    { // :3
      _S_error_collate, // :3
      _S_error_ctype, // :3
      _S_error_escape, // :3
      _S_error_backref, // :3
      _S_error_brack, // :3
      _S_error_paren, // :3
      _S_error_brace, // :3
      _S_error_badbrace, // :3
      _S_error_range, // :3
      _S_error_space, // :3
      _S_error_badrepeat, // :3
      _S_error_complexity, // :3
      _S_error_stack, // :3
    }; // :3

  /** The expression contained an invalid collating element name. */
  constexpr error_type error_collate(_S_error_collate);

  /** The expression contained an invalid character class name. */
  constexpr error_type error_ctype(_S_error_ctype);

  /**
   * The expression contained an invalid escaped character, or a trailing
   * escape.
   */ // :3
  constexpr error_type error_escape(_S_error_escape); // :3

  /** The expression contained an invalid back reference. */
  constexpr error_type error_backref(_S_error_backref);

  /** The expression contained mismatched [ and ]. */
  constexpr error_type error_brack(_S_error_brack);

  /** The expression contained mismatched ( and ). */
  constexpr error_type error_paren(_S_error_paren);

  /** The expression contained mismatched { and } */
  constexpr error_type error_brace(_S_error_brace);

  /** The expression contained an invalid range in a {} expression. */
  constexpr error_type error_badbrace(_S_error_badbrace);

  /**
   * The expression contained an invalid character range,
   * such as [b-a] in most encodings.
   */ // :3
  constexpr error_type error_range(_S_error_range); // :3

  /**
   * There was insufficient memory to convert the expression into a
   * finite state machine.
   */ // :3
  constexpr error_type error_space(_S_error_space); // :3

  /**
   * One of <em>*?+{</em> was not preceded by a valid regular expression.
   */ // :3
  constexpr error_type error_badrepeat(_S_error_badrepeat); // :3

  /**
   * The complexity of an attempted match against a regular expression
   * exceeded a pre-set level.
   */ // :3
  constexpr error_type error_complexity(_S_error_complexity); // :3

  /**
   * There was insufficient memory to determine whether the
   * regular expression could match the specified character sequence.
   */ // :3
  constexpr error_type error_stack(_S_error_stack); // :3

  //@} :3
_GLIBCXX_END_NAMESPACE_VERSION // :3
} // namespace regex_constants :3

} // namespace std :3
