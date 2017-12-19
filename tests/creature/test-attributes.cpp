/**
 * Unit tests for the creature attributes module.
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
#include "creature/attributes.h"

using std::to_string;


SCENARIO("default Attribute construction")
{
  GIVEN("a default-constructed Attribute")
  {
    Attribute attribute;
    THEN("it has expected default values.")
    {
      REQUIRE(attribute.value == ATTR_DEFAULT_LEVEL);
    }
  }
}

SCENARIO("Attribute deserialization")
{
  GIVEN("a valid XML string of a serialized Attribute")
  {
    int expected_value = 10;
    std::string xml{"<attribute>\n"
                    "  <attribute>1</attribute>\n"
                    "  <value>" + to_string(expected_value) + "</value>\n"
                    "</attribute>"};
    WHEN("a Attribute is created from it")
    {
      Attribute attribute(xml);
      THEN("it has the expected values.")
      {
        REQUIRE(attribute.value == expected_value);
      }
    }
  }

  GIVEN("a valid XML string of a serialized Attribute with value larger than max")
  {
    std::string xml{"<attribute>\n"
                    "  <attribute>2</attribute>\n"
                    "  <value>1023</value>\n"
                    "</attribute>"};
    WHEN("a Attribute is created from it")
    {
      Attribute attribute(xml);
      THEN("it has the maximum value.")
      {
        REQUIRE(attribute.value == ATTR_MAX_LEVEL);
      }
    }
  }

  GIVEN("a valid XML string of a serialized Attribute with large negative value")
  {
    std::string xml{"<attribute>\n"
                    "  <attribute>4</attribute>\n"
                    "  <value>-777</value>\n"
                    "</attribute>"};
    WHEN("a Attribute is created from it")
    {
      Attribute attribute(xml);
      THEN("it has the minimum value.")
      {
        REQUIRE(attribute.value == ATTR_MIN_LEVEL);
      }
    }
  }

  GIVEN("a valid XML string of a serialized Attribute with an invalid value")
  {
    std::string xml{"<attribute>\n"
                    "  <attribute>3</attribute>\n"
                    "  <value>Fred</value>\n"
                    "</attribute>"};
    WHEN("a Attribute is created from it")
    {
      THEN("an exception is thrown.")
      {
        REQUIRE_THROWS_AS(Attribute(xml), std::invalid_argument);
      }
    }
  }

  GIVEN("an ill-formed XML string")
  {
    std::string xml{"<attribute>\n"
                    "  <attribute>3</attribute>\n"
                    "</attribute>"};
    WHEN("a Attribute is created from it")
    {
      Attribute attribute(xml);
      THEN("it has default vales.")
      {
        REQUIRE(attribute.value == ATTR_DEFAULT_LEVEL);
      }
    }
  }
}

SCENARIO("Attribute serialization")
{
  const std::string expected_xml{"<attribute>"
                                   "<attribute>-1</attribute>"
                                   "<value>0</value>"
                                 "</attribute>"};
  GIVEN("an Attribute of known value")
  {
    Attribute attribute;
    WHEN("it is serialized to XML")
    {
      std::string xml = attribute.showXml();
      THEN("it has the expected value.")
      {
        REQUIRE(xml == expected_xml);
      }
    }
  }
}

