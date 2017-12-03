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


static constexpr Alignment default_alignment{Alignment::PUBLIC_MOOD};
static constexpr Interval  default_age_range{18, 57};
static constexpr Interval  default_juice_range{0};
static constexpr Interval  default_money_range{20, 40};


SCENARIO("default-constructed CreatureType")
{
  GIVEN("A default-constructed CreatureType")
  {
    CreatureType ct;
    THEN("it has appropriate default values.")
    {
      REQUIRE(ct.alignment()  == default_alignment);
      REQUIRE(ct.age_range()  == default_age_range);
      REQUIRE(ct.juice()      == default_juice_range);
      REQUIRE(ct.money()      == default_money_range);
    }
  }
}

SCENARIO("creating a CreatureType from valid XML")
{
  GIVEN("A default-constructed CreatureType")
  {
    CreatureType ct;
    WHEN("it is initialized from a minimal valid XML string")
    {
      std::string idname{"CREATURE"};
      ct.initialize_from_xml("<creaturetype idname=\"" + idname + "\"></creaturetype>");
      THEN("The CreatureType should have the idname from the XML and defaults for the rest.")
      {
        REQUIRE(ct.get_idname() == idname);
        // REQUIRE(ct.get_type() == "UNDEFINED"); @TODO replace hard-coded types
        REQUIRE(ct.alignment()  == Alignment::PUBLIC_MOOD);
        REQUIRE(ct.age_range()  == default_age_range);
        REQUIRE(ct.juice()      == default_juice_range);
        REQUIRE(ct.money()      == default_money_range);
        REQUIRE(ct.get_encounter_name() == "UNDEFINED");
      }
    }
    WHEN("it is initialized from a non-minimal valid XML string")
    {
      std::string idname{"CREATURE2"};
      std::string encounter_name{"Creature 2"};
      Alignment   alignment = Alignment::MODERATE;
      std::string xml{"<creaturetype idname=\"" + idname + "\">\n"
                      " <encounter_name>" + encounter_name +"</encounter_name>\n"
                      " <alignment>" + to_string(alignment) + "</alignment>\n"
                      " <attributes>\n"
                      "  <health>15-20</health>\n"
                      "  <heart>12-16</heart>\n"
                      "  <intelligence>3-7</intelligence>\n"
                      " </attributes>\n"
                      " <juice>10-15</juice>\n"
                      " <money>50-100</money>\n"
                      "</creaturetype>"};

      ct.initialize_from_xml(xml);
      THEN("it should have the values from the XML.")
      {
        REQUIRE(ct.get_idname() == idname);
        // REQUIRE(ct.get_type() == "UNDEFINED"); @TODO replace hard-coded types
        REQUIRE(ct.alignment()  == Alignment::MODERATE);
        REQUIRE(ct.age_range()  == Interval{18, 57});
        REQUIRE(ct.juice()      == Interval{10, 15});
        REQUIRE(ct.money()      == Interval{50, 100});
        REQUIRE(ct.get_encounter_name() == encounter_name);
      }
    }
    WHEN("A valid XML string with a single weapon is passed to the constructor")
    {
      std::string idname{"CREATURE3"};
      std::string xml{"<creaturetype idname=\"" + idname + "\">\n"
                      "  <weapon>WEAPON_CARBINE_M4</weapon>\n"
                      "</creaturetype>"};
      ct.initialize_from_xml(xml);
    }
    WHEN("A valid XML string with multiple weapons is passed to the constructor")
    {
      std::string idname{"CREATURE4"};
      std::string xml{"<creaturetype idname=\"" + idname + "\">\n"
                      "  <weapon>\n"
                      "    <type>WEAPON_AUTORIFLE_M16</type>\n"
                      "    <number_clips>7</number_clips>\n"
                      "  </weapon>\n"
                      "</creaturetype>"};
      ct.initialize_from_xml(xml);
    }
  }
}

SCENARIO("initializing a CreatureType from invalid XML")
{
  GIVEN("A default-constructed CreatureType")
  {
    CreatureType ct;
    CreatureType other_ct;
    WHEN("it is initialized from an empty XML string")
    {
      ct.initialize_from_xml("");
      THEN("a type with a unique idname should be created with default attributes")
      {
        REQUIRE(ct.get_idname() != other_ct.get_idname());
        // REQUIRE(ct.get_type() == "UNDEFINED"); @TODO replace hard-coded types
        REQUIRE(ct.alignment()  == Alignment::PUBLIC_MOOD);
        REQUIRE(ct.age_range()  == default_age_range);
        REQUIRE(ct.juice()      == default_juice_range);
        REQUIRE(ct.money()      == default_money_range);
        REQUIRE(ct.get_encounter_name() == "UNDEFINED");
      }
    }
    WHEN("it is initialized from an XML string with unexpected elements")
    {
      std::string idname{"CREATURE101"};
      std::string xml{"<creaturetype idname=\"" + idname + "\" notloB=\"Bolton\">\n"
                      " <barf attr=\"6\">bogus!</barf>\n"
                      " <money>50-100</money>\n"
                      "</creaturetype>"};
      ct.initialize_from_xml(xml);
      THEN("the bogus elements should be ignored")
      {
        REQUIRE(ct.get_idname() != other_ct.get_idname());
        REQUIRE(ct.get_idname() == idname);
        // REQUIRE(ct.get_type() == "UNDEFINED"); @TODO replace hard-coded types
        REQUIRE(ct.alignment()  == Alignment::PUBLIC_MOOD);
        REQUIRE(ct.age_range()  == default_age_range);
        REQUIRE(ct.juice()      == default_juice_range);
        REQUIRE(ct.money()      == Interval{50, 100});
        REQUIRE(ct.get_encounter_name() == "UNDEFINED");
      }
    }
    WHEN("it is initialized from an ill-formed XML string")
    {
      std::string idname{"CREATURE102"};
      std::string xml{"<creaturetype idname=\"" + idname + "\" notloB=\"Bolton\">\n"
                      " <barf attr=\"6\">bogus!</barf>\n"
                      " <money>50-100\n"
                      "</creaturetype>"};
      ct.initialize_from_xml(xml);
      THEN("a type with a unique idname should be created with default attributes")
      {
        REQUIRE(ct.get_idname() != other_ct.get_idname());
      }
    }
  }
}
