/**
 * Unit tests for the creature type cache module.
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
#include "creature/creaturetypecache.h"
#include <string>


SCENARIO("default behaviour")
{
  GIVEN("a default-constructed CreatureTypeCache")
  {
    CreatureTypeCache ctc;
    THEN("it should have zero size")
    {
      REQUIRE(ctc.size() == 0);
    }
  }
}

SCENARIO("loading valid XML")
{
  CreatureTypeCache ctc;
  REQUIRE(ctc.size() == 0);

  WHEN("an XML with just one CreatureType definition is loaded")
  {
    std::string xml{"<creatures>\n"
                    "  <creaturetype>\n"
                    "  </creaturetype>\n"
                    "</creatures>"};
    ctc.load_from_xml_string(xml);
    THEN("it will have size 1")
    {
      REQUIRE(ctc.size() == 1);
    }
  }

  WHEN("an XML with two CreatureType definitions are loaded")
  {
    std::string xml{"<creatures>\n"
                    "  <creaturetype idname=\"CREATURE_TEENAGER\">\n"
                    "  </creaturetype>\n"
                    "  <creaturetype idname=\"CREATURE_LAWYER\">\n"
                    "  </creaturetype>\n"
                    "</creatures>"};
    ctc.load_from_xml_string(xml);
    THEN("it will have size 2")
    {
      REQUIRE(ctc.size() == 2);
    }
    AND_THEN("a load creature type can be retrieved.")
    {
      CreatureType* ct = ctc.get_by_idname("CREATURE_TEENAGER");
      REQUIRE(ct != nullptr);
      REQUIRE(ct->get_idname() == "CREATURE_TEENAGER");
    }
  }
}

SCENARIO("loading invalid XML")
{
  GIVEN("An empty creature type cache")
  {
    CreatureTypeCache ctc;
    REQUIRE(ctc.size() == 0);
    WHEN("an attempt is made to load invalid XML")
    {
      ctc.load_from_xml_string("garbage");
      THEN("the cache remains unchanged.")
      {
        REQUIRE(ctc.size() == 0);
      }
    }
  }
}
