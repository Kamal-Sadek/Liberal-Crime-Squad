/**
 * Implementation of the Interval helper class.
 */
/*
 * Copyright 2017 Stephen M. Webb
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
#include "common/interval.h"

#include "includes.h"
#include <stdexcept>


namespace
{

/**
 * Check if a string is a number. Assumes non-numeric characters other than
 * dashes have already been checked for.
 */
bool
valid(std::string const& v)
{
  std::string::size_type len = v.length();
  return len > 0
    && (len !=1 || v[0] != '-')
    && v.find('-', 1) == std::string::npos;
}

} // anonymous namespace


Interval::
Interval()
: min(0)
, max(0)
{ }


Interval::
Interval(int value)
: min(value)
, max(value)
{ }


Interval::
Interval(int low, int high)
: min(low)
, max(high)
{ }


Interval Interval::
from_string(std::string const& interval_string)
{
  if (interval_string.length() == 0 || interval_string.find_first_not_of("1234567890-") != std::string::npos)
    throw std::invalid_argument("invalid interval format");

  Interval interval;

  size_t dashpos = interval_string.find('-',1);
  if (dashpos == std::string::npos) // Just a constant.
  {
    if (!valid(interval_string))
      throw std::invalid_argument("invalid interval format");
    int val = stoi(interval_string);
    return Interval(val, val);
  }

  std::string smin = interval_string.substr(0, dashpos);
  std::string smax = interval_string.substr(dashpos + 1);
  if (!valid(smin) || !valid(smax))
    return false;

  int tmin = stoi(smin);
  int tmax = stoi(smax);
  if (tmin > tmax)
    throw std::invalid_argument("min  > max");

  return Interval(tmin, tmax);
}


int Interval::
roll() const
{
  return LCSrandom(max-min+1)+min;
}

