/**
 * Interface for the Interval helper class.
 */
/*
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
#ifndef LCS_COMMON_INTERVAL_H
#define LCS_COMMON_INTERVAL_H

#include <iosfwd>
#include <string>


/**
 * A closed integral interval.
 *
 * Useful for things like age ranges.
 */
class Interval
{
public:
  constexpr
  Interval()
  : min(0), max(0)
  { }

  constexpr
  Interval(int value)
  : min(value), max(value)
  { }

  constexpr
  Interval(int min, int max)
  : min(min), max(max)
  { }

  /**
   * Generates a random number unformly distributed on the range
   * [@p min, @p max].
   */
  int
  roll() const;

  /**
   * Create an Interval fro a string.
   *
   * A factory function that constructs an Interval from a string that contains
   * either a number or two numbers separated by a dash.
   *
   * @throws std::invalid_argument if @p interval_string is not a valid interval
   * format, and std::out_of_range if one of the range values does not fit in a
   * signed int.
   */
  static Interval
  from_string(std::string const& interval_string);

  /** Get a string-marshallable representation of the Interval. */
  std::string
  to_string() const;

public:
  int min;
  int max;
};


inline bool
operator==(Interval const& lhs, Interval const& rhs)
{ return lhs.min == rhs.min && lhs.max == rhs.max; }

inline bool
operator!=(Interval const& lhs, Interval const& rhs)
{ return lhs.min != rhs.min || lhs.max != rhs.max; }

inline std::ostream&
operator<<(std::ostream& ostr, Interval const& rhs)
{ return ostr << rhs.to_string(); }

#endif /* LCS_COMMON_INTERVAL_H */
