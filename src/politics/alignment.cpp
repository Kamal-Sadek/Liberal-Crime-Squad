/**
 * Alignment operations.
 */
/*
 * This file is part of Liberal Crime Squad.
 *
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "politics/alignment.h"

#include "common.h" // for LCSrandom() -- which should be moved into its own header
#include <iterator>
#include <utility>
#include <vector>


namespace
{

using Mapping = std::pair<std::string, Alignment>;
using NameMap = std::vector<Mapping>; // switch to std::array after C++17

std::vector<Mapping> mapping =
{
  { "PUBLIC_MOOD",       Alignment::PUBLIC_MOOD       },
  { "ARCH_CONSERVATIVE", Alignment::ARCH_CONSERVATIVE },
  { "CONSERVATIVE",      Alignment::CONSERVATIVE      },
  { "MODERATE",          Alignment::MODERATE          },
  { "LIBERAL",           Alignment::LIBERAL           },
  { "ELITE_LIBERAL",     Alignment::ELITE_LIBERAL     },
  { "STALINIST",         Alignment::STALINIST         },
};

} // anonymous namespace


Alignment
choose(std::initializer_list<Alignment> choices)
{
  long selection = LCSrandom(choices.size());
  auto idx = choices.begin();
  std::advance(idx, selection);
  return *idx;
}


bool
is_extreme(Alignment alignment)
{
  if (alignment == Alignment::CONSERVATIVE
   || alignment == Alignment::MODERATE
   || alignment == Alignment::LIBERAL)
  {
    return false;
  }
  return true;
}

bool
to_left_of(Alignment lhs, Alignment rhs)
{
  return to_index(lhs) > to_index(rhs);
}


bool
to_right_of(Alignment lhs, Alignment rhs)
{
  return to_index(lhs) < to_index(rhs);
}


Alignment
shift_left(Alignment alignment, int amount)
{
  for( ; amount; --amount)
  {
    switch (alignment)
    {
      case Alignment::ARCH_CONSERVATIVE:
        alignment = Alignment::CONSERVATIVE;
        break;
      case Alignment::CONSERVATIVE:
        alignment = Alignment::MODERATE;
        break;
      case Alignment::MODERATE:
        alignment = Alignment::LIBERAL;
        break;
      default:
        alignment = Alignment::ELITE_LIBERAL;
        break;
    }
  }
  return alignment;
}


Alignment
shift_right(Alignment alignment, int amount)
{
  for( ; amount; --amount)
  {
    switch (alignment)
    {
      case Alignment::ELITE_LIBERAL:
        alignment = Alignment::LIBERAL;
        break;
      case Alignment::LIBERAL:
        alignment = Alignment::MODERATE;
        break;
      case Alignment::MODERATE:
        alignment = Alignment::CONSERVATIVE;
        break;
      default:
        alignment = Alignment::ARCH_CONSERVATIVE;
        break;
    }
  }
  return alignment;
}


Alignment
shift(Alignment alignment, int steps)
{
  if (steps >= 0)
    return shift_left(alignment, steps);
  else
    return shift_right(alignment, -steps);
}


std::string
as_printable(Alignment alignment, bool capitalize)
{
  switch (alignment)
  {
    case Alignment::ARCH_CONSERVATIVE: return "Arch-Conservative";
    case Alignment::CONSERVATIVE: return "Conservative";
    case Alignment::MODERATE: return (capitalize?"Moderate":"moderate");
    case Alignment::LIBERAL: return "Liberal";
    case Alignment::ELITE_LIBERAL: return "Elite Liberal";
    case Alignment::STALINIST: return "Stalinist";
    default: return "Buggy";
  }
}


std::size_t
to_index(Alignment alignment)
{
  switch (alignment)
  {
    case Alignment::ARCH_CONSERVATIVE: return 0;
    case Alignment::CONSERVATIVE: return 1;
    case Alignment::MODERATE: return 2;
    case Alignment::LIBERAL: return 3;
    case Alignment::ELITE_LIBERAL: return 4;
    case Alignment::STALINIST: return 5;
    default: return -1;
  }
}


std::size_t
diff(Alignment lhs, Alignment rhs)
{
  std::size_t lhsi = to_index(lhs);
  std::size_t rhsi = to_index(rhs);
  return (lhsi > rhsi) ? (lhsi - rhsi) : (rhsi - lhsi);
}


bool
from_string(std::string const& text, Alignment& alignment)
{
  for (auto const& m: mapping)
  {
    if (text == m.first)
    {
      alignment = m.second;
      return true;
    }
  }
  alignment = Alignment::PUBLIC_MOOD;
  return false;
}


std::string
to_string(Alignment alignment)
{
  for (auto const& m: mapping)
  {
    if (alignment == m.second)
    {
      return m.first;
    }
  }
  return "UNKNOWN";
}

