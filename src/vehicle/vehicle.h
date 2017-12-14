/**
 * Interface for the base WeaponType class.
 */
/*
 * Copyright 2010 Carlos Gustavos  (blomkvist)
 * Copyright 2010 Ari Rahikkala  (arirahikkala)
 * Copyright 2014 Rich McGrew  (yetisyny)
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
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>


enum carparts
{
   CARPART_BODY,
   CARPART_WINDOW,
   CARPARTNUM
};

enum SpecialCarDamage
{  // TODO
   SPECIALCARDAMAGE_LIGHTS, // Reduced visibility, penalty to road hazards.
   SPECIALCARDAMAGE_FUELTANK, // Starts countdown timer on chase - forced bail out if out of gas
   SPECIALCARDAMAGE_ENGINE, // Lose drivebonus, or forced bail out
   SPECIALCARDAMAGE_TIRES, // Extra drive check each turn or crash
   SPECIALCARDAMAGE_WINDOWS, // Lose upper armor protection
   SPECIALCARDAMAGE_COSMETIC, // Extra hassling by conservatives when driving around town.
   SPECIALCARDAMAGENUM
};

enum CarUpgrades
{  // TODO
   CARUPGRADE_BODYARMOR, // ++ to body protection, -- drivebonus
   CARUPGRADE_BULLETPROOFGLASS, // ++ to window protection, -- drivebonus
   CARUPGRADE_NITRO, // ++ escape chance on open-road chase event
   CARUPGRADE_WHEELS, // chance to avoid losing tires when shot.
   CARUPGRADE_ROLLCAGE, // reduced passenger injuries when the car crashes.
   CARUPGRADE_PAINTJOB, // -- to conservative hassling during car trips.
   CARUPGRADE_TINTEDWINDOWS, // -- to conservative hassling when carrying weapons
   CARUPGRADESNUM
};

// These declarations are for stuff that isn't in vehicle.cpp or the Vehicle class.
// They're just needed by this header and implemented elsewhere.
class VehicleType;
extern std::vector<VehicleType*> vehicletype;
int getvehicletype(std::string const& idname);

class Vehicle
{
private:
   void init(const VehicleType& seed, const std::string& color, int myear);

   std::string vtypeidname_;
   long vtypeid_; //If the xml-file is changed vtypeid_ may not be correct after loading a saved game. So it's not actually used now. -XML
   std::string color_;
   //bool displaycolor_;
   short heat_;
   long location_;
   int myear_;
   long id_;
   //int drivebonus_;
   //string longname_;
   //string shortname_;
   //int steal_difficultytofind_;
   //int steal_juice_;
   //int steal_extraheat_;

public:
   explicit Vehicle(const VehicleType& seed);
   Vehicle(const VehicleType& vehicle_type, const std::string& color, int myear){ init(vehicle_type,color,myear); }
   ~Vehicle() { stop_riding_me(); stop_preferring_me(); }

   std::string showXml() const;
   Vehicle(const std::string& inputXml);

   void stop_riding_me() const;
   void stop_preferring_me() const; //GETS RID OF CAR PREFERENCES FROM pool LIBERALS.

   std::string fullname(bool halffull=false) const;

   short get_heat() const { return heat_; }
   void add_heat(short heat) { heat_ += heat; }
   long get_location() const { return location_; }
   void set_location(long new_location) { location_ = new_location; }

   std::string const& vtypeidname() const { return vtypeidname_; }
   long vtypeid() const { return vtypeid_; }
   std::string const& color() const { return color_; }
   bool displayscolor() const;
   int myear() const { return myear_; }
   long id() const { return id_; }
   int modifieddriveskill(int skillLevel);
   int modifieddodgeskill(int skillLevel);
   int attackbonus(bool isDriver) const;
   int gethitlocation(int bodypart);
   std::string getpartname(int hitlocation);
   int armorbonus(int hitlocation) const;
   std::string const& longname() const;
   std::string const& shortname() const;
   int steal_difficultytofind() const;
   int steal_juice() const;
   int steal_extraheat() const;
   int sensealarmchance() const;
   int touchalarmchance() const;
   bool availableatshop() const;
   int price() const;
   int sleeperprice() const;

   static long curcarid;
};

#endif //VEHICLE_H
