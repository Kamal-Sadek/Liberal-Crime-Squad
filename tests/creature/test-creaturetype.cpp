/**
 * Unit tests for the creature types module.
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
#include "creature/creaturetype.h"
#include <string>


SCENARIO("creating a CreatureType from valid XML")
{
  WHEN("A valid minimal XML string is passed to the constructor")
  {
    std::string idname{"CREATURE"};
    std::string xml{"<creaturetype idname=\"" + idname + "\"></creaturetype>"};
    THEN("")
    {
      CreatureType ct(xml);
      REQUIRE(ct.get_idname() == idname);
      // REQUIRE(ct.get_type() == "UNDEFINED"); @TODO replace hard-coded types
      // REQUIRE(ct.age_range() == Interval{18, 57}); @TODO add this accessor
      REQUIRE(ct.get_encounter_name() == "UNDEFINED");
      REQUIRE(ct.alignment() == Alignment::PUBLIC_MOOD);
    }
  }
  WHEN("A valid non-minimal XML string is passed to the constructor")
  {
  }
  WHEN("A valid XML string with a single weapon is passed to the constructor")
  {
  }
  WHEN("A valid XML string with multiple weapons is passed to the constructor")
  {
  }
}

SCENARIO("creating a CreatureType from invalid XML")
{
  WHEN("An empty XML string is passed to the constructor")
  {
  }
  WHEN("An XML string with unexpected elements is passed to the constructor")
  {
  }
  WHEN("An ill-formed XML string is passed to the constructor")
  {
  }
}
