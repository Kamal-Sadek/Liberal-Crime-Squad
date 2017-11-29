/**
 * Unit tests for the political Alignment module.
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
#include "catch/catch.hpp"
#include "politics/alignment.h"

SCENARIO("check for extreme political alignment")
{
  WHEN("the alignment is ARCH_CONSERVATIVE")
  {
    auto alignment = Alignment::ARCH_CONSERVATIVE;
    THEN("it is extreme")
    {
      REQUIRE(is_extreme(alignment) == true);
    }
  }
  WHEN("the alignment is CONSERVATIVE")
  {
    auto alignment = Alignment::CONSERVATIVE;
    THEN("it is not extreme")
    {
      REQUIRE(is_extreme(alignment) == false);
    }
  }
  WHEN("the alignment is MODERATE")
  {
    auto alignment = Alignment::MODERATE;
    THEN("it is not extreme")
    {
      REQUIRE(is_extreme(alignment) == false);
    }
  }
  WHEN("the alignment is LIBERAL")
  {
    auto alignment = Alignment::LIBERAL;
    THEN("it is not extreme")
    {
      REQUIRE(is_extreme(alignment) == false);
    }
  }
  WHEN("the alignment is ELITE_LIBERAL")
  {
    auto alignment = Alignment::ELITE_LIBERAL;
    THEN("it is extreme")
    {
      REQUIRE(is_extreme(alignment) == true);
    }
  }
}

SCENARIO("relative ordering of alignments")
{
  WHEN("CON+ and CON are compared")
  {
    THEN("CON+ is right of CON and CON is left of CON+")
    {
      REQUIRE(to_right_of(Alignment::ARCH_CONSERVATIVE, Alignment::CONSERVATIVE));
      REQUIRE(to_left_of(Alignment::CONSERVATIVE, Alignment::ARCH_CONSERVATIVE));
    }
  }
  WHEN("CON and MOD are compared")
  {
    THEN("CON is right of MOD and MOD is left of CON")
    {
      REQUIRE(to_right_of(Alignment::CONSERVATIVE, Alignment::MODERATE));
      REQUIRE(to_left_of(Alignment::MODERATE, Alignment::CONSERVATIVE));
    }
  }
  WHEN("MOD and LIB are compared")
  {
    THEN("MOD is right of LIB and LIB is left of MOD")
    {
      REQUIRE(to_right_of(Alignment::MODERATE, Alignment::LIBERAL));
      REQUIRE(to_left_of(Alignment::LIBERAL, Alignment::MODERATE));
    }
  }
  WHEN("LIB and LIB+ are compared")
  {
    THEN("LIB is right of LIB+ and LIB+ is left of LIB")
    {
      REQUIRE(to_right_of(Alignment::LIBERAL, Alignment::ELITE_LIBERAL));
      REQUIRE(to_left_of(Alignment::ELITE_LIBERAL, Alignment::LIBERAL));
    }
  }
}

SCENARIO("shifting alignments")
{
  WHEN("CON is shifted right")
  {
    THEN("it becomes CON+")
    {
      REQUIRE(shift_right(Alignment::CONSERVATIVE) == Alignment::ARCH_CONSERVATIVE);
    }
  }
  WHEN("CON+ is shifted right")
  {
    THEN("it stays CON+")
    {
      REQUIRE(shift_right(Alignment::ARCH_CONSERVATIVE) == Alignment::ARCH_CONSERVATIVE);
    }
  }
  WHEN("LIB is shifted left")
  {
    THEN("it becomes LIB+")
    {
      REQUIRE(shift_left(Alignment::LIBERAL) == Alignment::ELITE_LIBERAL);
    }
  }
  WHEN("LIB+ is shifted left")
  {
    THEN("it stays LIB+")
    {
      REQUIRE(shift_left(Alignment::ELITE_LIBERAL) == Alignment::ELITE_LIBERAL);
    }
  }
  WHEN("MOD is shited 1")
  {
    THEN("it becomes LIB")
    {
      REQUIRE(shift(Alignment::MODERATE, 1) == Alignment::LIBERAL);
    }
  }
  WHEN("MOD is shited 10")
  {
    THEN("it becomes LIB+")
    {
      REQUIRE(shift(Alignment::MODERATE, 10) == Alignment::ELITE_LIBERAL);
    }
  }
  WHEN("MOD is shited -1")
  {
    THEN("it becomes CON`")
    {
      REQUIRE(shift(Alignment::MODERATE, -1) == Alignment::CONSERVATIVE);
    }
  }
  WHEN("MOD is shited -10")
  {
    THEN("it becomes CON+`")
    {
      REQUIRE(shift(Alignment::MODERATE, -10) == Alignment::ARCH_CONSERVATIVE);
    }
  }
}

SCENARIO("marshalling and unmarshalling")
{
  WHEN("CON+ is marshalled to a string")
  {
    auto alignment = Alignment::ARCH_CONSERVATIVE;
    std::string m = to_string(alignment);
    THEN("it unmarshalls as the same alignment");
    {
      Alignment a2;
      from_string(m, a2);
      REQUIRE(alignment == a2);
    }
  }
  WHEN("CON is marshalled to a string")
  {
    auto alignment = Alignment::CONSERVATIVE;
    std::string m = to_string(alignment);
    THEN("it unmarshalls as the same alignment");
    {
      Alignment a2;
      from_string(m, a2);
      REQUIRE(alignment == a2);
    }
  }
  WHEN("MOD is marshalled to a string")
  {
    auto alignment = Alignment::MODERATE;
    std::string m = to_string(alignment);
    THEN("it unmarshalls as the same alignment");
    {
      Alignment a2;
      from_string(m, a2);
      REQUIRE(alignment == a2);
    }
  }
  WHEN("LIB is marshalled to a string")
  {
    auto alignment = Alignment::LIBERAL;
    std::string m = to_string(alignment);
    THEN("it unmarshalls as the same alignment");
    {
      Alignment a2;
      from_string(m, a2);
      REQUIRE(alignment == a2);
    }
  }
  WHEN("LIB+ is marshalled to a string")
  {
    auto alignment = Alignment::ELITE_LIBERAL;
    std::string m = to_string(alignment);
    THEN("it unmarshalls as the same alignment");
    {
      Alignment a2;
      from_string(m, a2);
      REQUIRE(alignment == a2);
    }
  }
}

SCENARIO("choosing an alignment from a list")
{
  WHEN("a list of two alignments is passed to choose()")
  {
    THEN("one is always chosen")
    {
      REQUIRE(choose({Alignment::MODERATE, Alignment::MODERATE}) == Alignment::MODERATE);
    }
  }
}

SCENARIO("differences in alignments")
{
  REQUIRE(diff(Alignment::ARCH_CONSERVATIVE, Alignment::ELITE_LIBERAL) == 4);
}
