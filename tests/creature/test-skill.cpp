/**
 * Unit tests for the creature skill module.
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
#include "creature/skill.h"

using std::to_string;


constexpr int default_skill_value = 0;


SCENARIO("default construction")
{
  GIVEN("a default-constructed Skill")
  {
    Skill skill;
    THEN("it has expected default values.")
    {
      REQUIRE(skill.value == default_skill_value);
    }
  }
}

SCENARIO("deserialization")
{
  GIVEN("a valid XML string of a serialized Skill")
  {
    int expected_value = 10;
    std::string xml{"<skill>\n"
                    "  <associated_attribue>3</associated_attribute>\n"
                    "  <skill>1</skill>\n"
                    "  <value>" + to_string(expected_value) + "</value>\n"
                    "</skill>"};
    WHEN("a Skill is created from it")
    {
      Skill skill(xml);
      THEN("it has the expected values.")
      {
        REQUIRE(skill.value == expected_value);
      }
    }
  }

  GIVEN("a valid XML string of a serialized Skill with value larger than max")
  {
    std::string xml{"<skill>\n"
                    "  <associated_attribue>1</associated_attribute>\n"
                    "  <skill>2</skill>\n"
                    "  <value>1023</value>\n"
                    "</skill>"};
    WHEN("a Skill is created from it")
    {
      Skill skill(xml);
      THEN("it has the maximum value.")
      {
        REQUIRE(skill.value == MAX_SKILL_LEVEL);
      }
    }
  }

  GIVEN("a valid XML string of a serialized Skill with large negative value")
  {
    std::string xml{"<skill>\n"
                    "  <associated_attribue>1</associated_attribute>\n"
                    "  <skill>4</skill>\n"
                    "  <value>-777</value>\n"
                    "</skill>"};
    WHEN("a Skill is created from it")
    {
      Skill skill(xml);
      THEN("it has the maximum value.")
      {
        REQUIRE(skill.value == 0);
      }
    }
  }

  GIVEN("a valid XML string of a serialized Skill with an invalid value")
  {
    std::string xml{"<skill>\n"
                    "  <associated_attribue>1</associated_attribute>\n"
                    "  <skill>3</skill>\n"
                    "  <value>Fred</value>\n"
                    "</skill>"};
    WHEN("a Skill is created from it")
    {
      THEN("an exception is thrown.")
      {
        REQUIRE_THROWS_AS(Skill(xml), std::invalid_argument);
      }
    }
  }

  GIVEN("an ill-formed XML string")
  {
    std::string xml{"<skill>\n"
                    "  <associated_attribue>1\n"
                    "  <skill>3</skill>\n"
                    "</skill>"};
    WHEN("a Skill is created from it")
    {
      Skill skill(xml);
      THEN("it has default vales.")
      {
        REQUIRE(skill.value == default_skill_value);
      }
    }
  }
}

SCENARIO("serialization")
{
  const std::string expected_xml{"<skill>"
                                   "<associated_attribute>-1</associated_attribute>"
                                   "<skill>-1</skill>"
                                   "<value>0</value>"
                                 "</skill>"};
  GIVEN("a Skll of known value")
  {
    Skill skill;
    WHEN("it is serialized to XML")
    {
      std::string xml = skill.showXml();
      THEN("it has the expected value.")
      {
        REQUIRE(xml == expected_xml);
      }
    }
  }
}

