/**
 * Unit tests for the unique creatures module.
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
#include "creature/creature.h"


SCENARIO("default UniqueCreatures construction")
{
  GIVEN("a default-constructed UniqueCreatures")
  {
    UniqueCreatures uniqueCreatures;
    THEN("it has expected default values.")
    {
      REQUIRE(uniqueCreatures.CEO_state == UNIQUECREATURE_ALIVE);
      REQUIRE(uniqueCreatures.Pres_state == UNIQUECREATURE_ALIVE);
    }
  }
}

SCENARIO("UniqueCreatures deserialization")
{
  GIVEN("a valid XML string of a serialized UniqueCreatures")
  {
    int expected_value = 10;
    std::string xml{"<uniquecreatures>\n"
                    "  <CEO_state>1</CEO_state>\n"
                    "  <Pres_state>1</Pres_state>\n"
                    "</uniquecreatures>"};
    WHEN("a UniqueCreatures is created from it")
    {
      UniqueCreatures uniqueCreatures(xml);
      THEN("it has the expected values.")
      {
        REQUIRE(uniqueCreatures.CEO_state == UNIQUECREATURE_DEAD);
        REQUIRE(uniqueCreatures.Pres_state == UNIQUECREATURE_DEAD);
      }
    }
  }

  GIVEN("a valid XML string of a serialized UniqueCreatures with an invalid value")
  {
    std::string xml{"<uniquecreatures>\n"
                    "  <CEO_state>233</CEO_state>\n"
                    "</uniquecreatures>"};
    WHEN("a UniqueCreatures is created from it")
    {
      THEN("an exception is thrown.")
      {
        REQUIRE_THROWS_AS(UniqueCreatures(xml), std::invalid_argument);
      }
    }
  }

  GIVEN("an ill-formed XML string")
  {
    std::string xml{"<uniquecreatures>\n"
                    "  <CEO_state>\n"
                    "</uniquecreatures>"};
    WHEN("a UniqueCreatures is created from it")
    {
      UniqueCreatures uniqueCreatures(xml);
      THEN("it has default vales.")
      {
        REQUIRE(uniqueCreatures.CEO_state == UNIQUECREATURE_ALIVE);
      }
    }
  }
}

