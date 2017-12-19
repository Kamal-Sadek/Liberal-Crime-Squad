/**
 * Interface for the Vehicle Type class.
 */
/*
 * Copyright 2010 Carlos Gustavos  <blomkvist>
 * Copyright 2014 Rich McGrew  <yetisyny>
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
#include "vehicletype.h"
#include <externs.h>
#include "tinyxml2.h"

using std::string;

int VehicleType::number_of_vehicletypes = 0;


VehicleType::
VehicleType(std::string const& xml)
: id_{number_of_vehicletypes++}
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    xmllog.log("error parsing VehicleType XML");
    return;
  }

  for (auto elem = doc.FirstChildElement(); elem; elem = elem->NextSiblingElement())
  {
    auto attr_idname = elem->Attribute("idname");
    if (attr_idname == nullptr)
    {
      idname_ = "LACKS IDNAME " + std::to_string(id_);
    }
    else
    {
      this->idname_ = attr_idname;
    }

    for (auto e = elem->FirstChildElement(); e; e = e->NextSiblingElement())
    {
      std::string ename = e->Name();

      if (ename == "year")
      {
        for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
        {
          std::string tag = we->Name();

          if (tag == "start_at_current_year")
          {
            int b = stringtobool(we->GetText());
            if (b == 1)
              year_startcurrent_ = true;
            else if (b == 0)
              year_startcurrent_ = false;
          }
          else if (tag == "start_at_year")
            year_start_ = std::stoi(we->GetText());
          else if (tag == "add_random_up_to_current_year")
          {
            int b = stringtobool(we->GetText());
            if (b == 1)
              year_randomuptocurrent_ = true;
            else if (b == 0)
              year_randomuptocurrent_ = false;
          }
          else if (tag == "add_random")
            year_addrandom_ = std::stoi(we->GetText());
          else if (tag == "add")
            year_add_ = std::stoi(we->GetText());
        }
      }
      else if (ename == "colors")
      {
        for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
        {
          std::string tag = we->Name();

          if (tag == "color")
          {
            color_.push_back(we->GetText());
          }
          else if (tag == "display_color")
          {
            int b = stringtobool(we->GetText());
            if (b == 1)
              displaycolor_ = true;
            else if (b == 0)
              displaycolor_ = false;
          }
        }
      }
      else if (ename == "drivebonus")
      {
        for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
        {
          std::string tag = we->Name();

          if (tag == "base")
            drivebonus_ = std::stoi(we->GetText());
          else if (tag == "skillfactor")
            drivebonus_factor_ = std::stof(we->GetText());
          else if (tag == "softlimit")
            drivebonus_limit1_ = std::stoi(we->GetText());
          else if (tag == "hardlimit")
            drivebonus_limit2_ = std::stoi(we->GetText());
        }
      }
      else if (ename == "dodgebonus")
      {
        for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
        {
          std::string tag = we->Name();

          if (tag == "base")
            dodgebonus_ = std::stoi(we->GetText());
          else if (tag == "skillfactor")
            dodgebonus_factor_ = std::stof(we->GetText());
          else if (tag == "softlimit")
            dodgebonus_limit1_ = std::stoi(we->GetText());
          else if (tag == "hardlimit")
            dodgebonus_limit2_ = std::stoi(we->GetText());
        }
      }
      else if (ename == "attackbonus")
      {
        for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
        {
          std::string tag = we->Name();

          if (tag == "driver")
            attackbonus_driver_ = std::stoi(we->GetText());
          else if (tag == "passenger")
            attackbonus_passenger_ = std::stoi(we->GetText());
        }
      }
      else if (ename == "longname")
        longname_ = e->GetText();
      else if (ename == "shortname")
      {
        shortname_ = e->GetText();
        if (len(shortname_) > 7)
          shortname_.resize(7); // Only seven characters allowed for shortname_.
      }
      else if (ename == "stealing")
      {
        for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
        {
          std::string tag = we->Name();

          if (tag == "difficulty_to_find")
            steal_difficultytofind_ = std::stoi(we->GetText());
          else if (tag == "juice")
            steal_juice_ = std::stoi(we->GetText());
          else if (tag == "extra_heat")
            steal_extraheat_ = std::stoi(we->GetText());
          else if (tag == "sense_alarm_chance")
            sensealarmchance_ = std::stoi(we->GetText());
          else if (tag == "touch_alarm_chance")
            touchalarmchance_ = std::stoi(we->GetText());
        }
      }
      else if (ename == "armor")
      {
        for (auto we = e->FirstChildElement(); we; we = we->NextSiblingElement())
        {
          std::string tag = we->Name();

          if (tag == "low_armor_min")
            lowarmormin_ = std::stoi(we->GetText());
          else if (tag == "low_armor_max")
            lowarmormax_ = std::stoi(we->GetText());
          else if (tag == "high_armor_min")
            higharmormin_ = std::stoi(we->GetText());
          else if (tag == "high_armor_max")
            higharmormax_ = std::stoi(we->GetText());
          else if (tag == "armor_midpoint")
            armormidpoint_ = std::stoi(we->GetText());
        }
      }
      else if (ename == "available_at_dealership")
      {
        int b = stringtobool(e->GetText());
        if (b == 1)
          availableatshop_ = true;
        else if (b == 0)
          availableatshop_ = false;
      }
      else if (ename == "price")
        price_ = std::stoi(e->GetText());
      else if (ename == "sleeperprice")
        sleeperprice_ = std::stoi(e->GetText());
    }
  }

  if (len(color_) == 0)
    color_.push_back("Translucent"); // Default.
}


int VehicleType::
makeyear() const
{
  int myear = 0;

  if (year_startcurrent_)
    myear = year;
  else
    myear = year_start_;

  if (year_randomuptocurrent_)
    myear += LCSrandom(year - year_start_ + 1);

  if (year_addrandom_ > 0)
    myear += LCSrandom(year_addrandom_);
  else if (year_addrandom_ < 0)
    myear -= LCSrandom(-year_addrandom_);

  myear += year_add_;

  return myear;
}


int VehicleType::
gethitlocation(int bodypart)
{
  switch (bodypart)
  {
    case BODYPART_HEAD: return CARPART_WINDOW; break;
    case BODYPART_BODY:
    case BODYPART_ARM_RIGHT:
    case BODYPART_ARM_LEFT:
      return (LCSrandom(100) < armormidpoint_) ? CARPART_BODY : CARPART_WINDOW;
      break;
    case BODYPART_LEG_RIGHT:
    case BODYPART_LEG_LEFT: return CARPART_BODY; break;
  }

  return CARPART_WINDOW;
}


std::string VehicleType::
getpartname(int location)
{
  return location == CARPART_WINDOW ? "window" : "body";
}


int VehicleType::
armorbonus(int location)
{
  switch (location)
  {
    case CARPART_BODY: return LCSrandom(lowarmormax_ - lowarmormin_ + 1) + lowarmormin_;
    case CARPART_WINDOW: return LCSrandom(higharmormax_ - higharmormin_ + 1) + higharmormin_;
  }
  return 0;
}


int VehicleType::
modifieddriveskill(int skillLevel)
{
  int score = (skillLevel + drivebonus_) * drivebonus_factor_;
  if (score < drivebonus_limit1_)
    return score;
  if (score > drivebonus_limit1_)
    score = (score + drivebonus_limit1_) / 2; // half credit after limit1
  return (score > drivebonus_limit2_) ? drivebonus_limit2_ : score;
}


int VehicleType::
modifieddodgeskill(int skillLevel)
{
  int score = (skillLevel + dodgebonus_) * dodgebonus_factor_;
  if (score < dodgebonus_limit1_)
    return score;
  if (score > dodgebonus_limit1_)
    score = (score + dodgebonus_limit1_) / 2; // half credit after limit1
  return (score > dodgebonus_limit2_) ? dodgebonus_limit2_ : score;
}
