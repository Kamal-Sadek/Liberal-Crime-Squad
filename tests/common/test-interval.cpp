/**
 * Unit tests for the Interval helper class.
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
#include "common/interval.h"
#include <stdexcept>


SCENARIO("basic Interval construction")
{
  WHEN("an Interval is constructed with no arguments")
  {
    Interval interval;
    THEN("it has a (min,max) value of [0,0]")
    {
      REQUIRE(interval.min == 0);
      REQUIRE(interval.max == 0);
    }
  }

  WHEN("an Interval is constructed with one argument")
  {
    int test_value = 117;
    Interval interval(test_value);
    THEN("it has the correct (min, max) values")
    {
      REQUIRE(interval.min == test_value);
      REQUIRE(interval.max == test_value);
    }
  }

  WHEN("an Interval is constructed with two arguments")
  {
    int test_min = 51;
    int test_max = 95;
    Interval interval(test_min, test_max);
    THEN("it has the correct (min, max) values")
    {
      REQUIRE(interval.min == test_min);
      REQUIRE(interval.max == test_max);
    }
  }
}

SCENARIO("from_string() construction")
{
  WHEN("an Interval is constructed from a valid single numeric string")
  {
    Interval interval = Interval::from_string("511");
    THEN("it has the expected (min, max) values.")
    {
      REQUIRE(interval.min == 511);
      REQUIRE(interval.max == 511);
    }
  }

  WHEN("an Interval is constructed from a valid numeric interval")
  {
    Interval interval = Interval::from_string("7-10");
    THEN("it has the expected (min, max) values.")
    {
      REQUIRE(interval.min == 7);
      REQUIRE(interval.max == 10);
    }
  }

  WHEN("an Interval is constructed from just a dash")
  {
    THEN("it has the expected (min, max) values.")
    {
      REQUIRE_THROWS_AS(Interval::from_string("-"), std::invalid_argument);
    }
  }

  WHEN("an Interval string has an invalid character")
  {
    THEN("it has the expected (min, max) values.")
    {
      REQUIRE_THROWS_AS(Interval::from_string("2,3"), std::invalid_argument);
    }
  }
}

SCENARIO("Interval comparison operators")
{
  Interval lhs{15,35};
  Interval rhs_match{15, 35};
  Interval rhs_nomatch{-15, 35};

  REQUIRE(lhs == rhs_match);
  REQUIRE(lhs != rhs_nomatch);
}
