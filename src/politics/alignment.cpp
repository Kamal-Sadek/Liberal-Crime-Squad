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

