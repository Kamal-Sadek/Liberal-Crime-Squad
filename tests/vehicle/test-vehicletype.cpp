/**
 * Unit tests for the vehicle types module.
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
#include "vehicle/vehicletype.h"


SCENARIO("creating a VehicleType from valid XML")
{
  GIVEN("a minimal XML string")
  {
    std::string idname{"VEHICLE_A"};
    std::string xml{"<vehicletype idname=\"" + idname + "\"></vehicletype>"};
    WHEN("a VehicleType is created fro mit")
    {
      VehicleType vt{xml};
      THEN("the vhecileType has the expected values")
      {
        REQUIRE(vt.idname() == idname);
        REQUIRE(vt.id() >= 0);
        REQUIRE(vt.color().size() > 0);
        REQUIRE(vt.longname() == "UNDEFINED");
        REQUIRE(vt.shortname() == "UNDEF");
        REQUIRE(vt.steal_difficultytofind() == 1);
        REQUIRE(vt.steal_juice() == 0);
        REQUIRE(vt.steal_extraheat() == 0);
        REQUIRE(vt.sensealarmchance() == 0);
        REQUIRE(vt.touchalarmchance() == 0);
        REQUIRE(vt.availableatshop() == true);
        REQUIRE(vt.price() == 1234);
        REQUIRE(vt.sleeperprice() == 1111);
      }
    }
  }
  GIVEN("a more complete XML string")
  {
    std::string idname{"VEHICLE_B"};
    std::string long_name{"LONG NAME"};
    std::string short_name{"SHORT"};
    int         difficulty_to_find = 7;
    int         juice = 8;
    int         extra_heat = 9;
    int         sense_alarm_chance = 10;
    int         touch_alarm_chance = 11;
    int         price = 256;
    int         sleeperprice = 128;
    std::string xml{"<vehicletype idname=\"" + idname + "\">"
                    " <longname>" + long_name + "</longname>"
                    " <shortname>" + short_name + "</shortname>"
                    " <stealing>"
                    "  <difficulty_to_find>" + std::to_string(difficulty_to_find) + "</difficulty_to_find>"
                    "  <juice>" + std::to_string(juice) + "</juice>"
                    "  <extra_heat>" + std::to_string(extra_heat) + "</extra_heat>"
                    "  <sense_alarm_chance>" + std::to_string(sense_alarm_chance) + "</sense_alarm_chance>"
                    "  <touch_alarm_chance>" + std::to_string(touch_alarm_chance) + "</touch_alarm_chance>"
                    " </stealing>"
                    " <available_at_dealership>false</available_at_dealership>"
                    " <price>" + std::to_string(price) + "</price>"
                    " <sleeperprice>" + std::to_string(sleeperprice) + "</sleeperprice>"
                    "</vehicletype>"};
    WHEN("a VehicleType is created from it")
    {
      VehicleType vt{xml};
      THEN("the vhecileType has the expected values")
      {
        REQUIRE(vt.idname() == idname);
        REQUIRE(vt.id() >= 0);
        REQUIRE(vt.color().size() > 0);
        REQUIRE(vt.longname() == long_name);
        REQUIRE(vt.shortname() == short_name);
        REQUIRE(vt.steal_difficultytofind() == difficulty_to_find);
        REQUIRE(vt.steal_juice() == juice);
        REQUIRE(vt.steal_extraheat() == extra_heat);
        REQUIRE(vt.sensealarmchance() == sense_alarm_chance);
        REQUIRE(vt.touchalarmchance() == touch_alarm_chance);
        REQUIRE(vt.availableatshop() == false);
        REQUIRE(vt.price() == price);
        REQUIRE(vt.sleeperprice() == sleeperprice);
      }
    }
  }
}

SCENARIO("creating a VehicleType from invalid XML")
{
  GIVEN("an ill-formed XML string")
  {
    std::string xml{"<vehicletype></vhcletyp>"};
    WHEN("a VehicleType is created from it")
    {
      THEN("an exception is thrown")
      {
        // @TODO currently you silently get a default-constructed vehicle type.
        // REQUIRE_THROWS_AS(VehicleType(xml), std::invalid_argument);
      }
    }
  }
}
