/**
 * Interface for the base WeaponType class.
 */
/*
 * Copyright 2010 Carlos Gustavos  (blomkvist)
 * Copyright 2010 Ari Rahikkala  (arirahikkala)
 * Copyright 2014 Rich McGrew  (yetisyny)
 * Copyright 2014 Nick Dumas  (nickdumas)
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
#include "vehicle/vehicle.h"

#include <externs.h>
#include <sstream>
#include "tinyxml2.h"


namespace
{
  const std::string VEHICLE_XML_VEHICLE_ELEMENT{"vehicle"};
} // anonymous namespace


long Vehicle::curcarid = 0;


std::string Vehicle::
showXml() const
{
  return std::string{
    "<" + VEHICLE_XML_VEHICLE_ELEMENT + ">"
      "<vtypeidname>" + vtypeidname_ + "</vtypeidname>"
      "<vtypeid>" + std::to_string(vtypeid_) + "</vtypeid>"
      "<color>" + color_ + "</color>"
      "<heat>" + std::to_string(heat_) + "</heat>"
      "<location>" + std::to_string(location_) + "</location>"
      "<myear>" + std::to_string(myear_) + "</myear>"
      "<id>" + std::to_string(id_) + "</id>"
    "</" + VEHICLE_XML_VEHICLE_ELEMENT + ">"
  };
}


Vehicle::
Vehicle(const VehicleType& vehicle_type)
{
  init(vehicle_type, pickrandom(vehicle_type.color()), vehicle_type.makeyear());
}


Vehicle::
Vehicle(std::string const& xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing item XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == "vtypeidname")
      {
        char const* val = element->GetText();
        if (val)
          this->vtypeidname_ = val;
      }
      else if (tag == "vtypeid")
      {
        char const* val = element->GetText();
        if (val)
          this->vtypeid_ = std::stoi(val);
      }
      else if (tag == "color")
      {
        char const* val = element->GetText();
        if (val)
          this->color_ = val;
      }
      else if (tag == "heat")
      {
        char const* val = element->GetText();
        if (val)
          this->heat_ = std::stoi(val);
      }
      else if (tag == "location")
      {
        char const* val = element->GetText();
        if (val)
          this->location_ = std::stoi(val);
      }
      else if (tag == "myear")
      {
        char const* val = element->GetText();
        if (val)
          this->myear_ = std::stoi(val);
      }
      else if (tag == "id")
      {
        char const* val = element->GetText();
        if (val)
          this->id_ = std::stoi(val);
      }
    }
  }
}


void Vehicle::
init(const VehicleType& vehicle_type, const string& color, int myear)
{
   id_=curcarid++;
   heat_=0;
   location_=-1;
   vtypeidname_=vehicle_type.idname();
   vtypeid_=vehicle_type.id();
   color_=color;
   myear_=myear;
}


void Vehicle::
stop_riding_me() const
{
   for(int p=0;p<len(pool);p++)
      if (pool[p]->carid==id_)
         pool[p]->carid=-1;
}


void Vehicle::
stop_preferring_me() const
{
   for(int p=0;p<len(pool);p++)
      if (pool[p]->pref_carid==id_)
         pool[p]->pref_carid=-1;
}


std::string Vehicle::
fullname(bool halffull) const
{
   string s;
   int words=0;
   if (heat_)
   {
      s="Stolen ";
      words++;
   }
   if (displayscolor())
   {
      s+=color_+" ";
      words++;
   }
   if (myear_!=-1&&words<2) //don't print year if that will make the name too long.
      s+=std::to_string(myear_)+" ";
   if (halffull) s+=shortname();
   else s+=longname();

   return s;
}


int Vehicle::
modifieddriveskill(int skillLevel)
{
   return vehicletype[getvehicletype(vtypeidname_)]->modifieddriveskill(skillLevel); // Todo - add bonus if car is upgraded with nitro
}


int Vehicle::
modifieddodgeskill(int skillLevel)
{
   return vehicletype[getvehicletype(vtypeidname_)]->modifieddodgeskill(skillLevel); // Todo - add bonus if car is upgraded
}


bool Vehicle::
displayscolor() const
{ return vehicletype[getvehicletype(vtypeidname_)]->displayscolor(); }

int Vehicle::
attackbonus(bool isDriver) const
{ return vehicletype[getvehicletype(vtypeidname_)]->attackbonus(isDriver); }

int Vehicle::
gethitlocation(int bodypart)
{ return vehicletype[getvehicletype(vtypeidname_)]->gethitlocation(bodypart); }

string Vehicle::
getpartname(int hitlocation)
{ return vehicletype[getvehicletype(vtypeidname_)]->getpartname(hitlocation); }

int Vehicle::
armorbonus(int hitlocation) const
{ return vehicletype[getvehicletype(vtypeidname_)]->armorbonus(hitlocation); }

std::string const& Vehicle::
longname() const
{ return vehicletype[getvehicletype(vtypeidname_)]->longname(); }

string const& Vehicle::
shortname() const
{ return vehicletype[getvehicletype(vtypeidname_)]->shortname(); }

int Vehicle::
steal_difficultytofind() const
{ return vehicletype[getvehicletype(vtypeidname_)]->steal_difficultytofind(); }

int Vehicle::
steal_juice() const
{ return vehicletype[getvehicletype(vtypeidname_)]->steal_juice(); }

int Vehicle::
steal_extraheat() const
{ return vehicletype[getvehicletype(vtypeidname_)]->steal_extraheat(); }

int Vehicle::
sensealarmchance() const
{ return vehicletype[getvehicletype(vtypeidname_)]->sensealarmchance(); }

int Vehicle::
touchalarmchance() const
{ return vehicletype[getvehicletype(vtypeidname_)]->touchalarmchance(); }

bool Vehicle::
availableatshop() const
{ return vehicletype[getvehicletype(vtypeidname_)]->availableatshop(); }

int Vehicle::
price() const
{ return vehicletype[getvehicletype(vtypeidname_)]->price(); }

int Vehicle::
sleeperprice() const
{ return vehicletype[getvehicletype(vtypeidname_)]->sleeperprice(); }

