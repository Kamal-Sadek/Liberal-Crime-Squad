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
#ifndef VEHICLE_TYPE_H
#define VEHICLE_TYPE_H

#include <string>
#include <vector>


class VehicleType
{
public:
  using ColorList = std::vector<std::string>;

public:
  explicit VehicleType(std::string const& xmlstring);

  std::string const&
  idname() const
  { return idname_; }

  long
  id() const
  { return id_; }

  int
  makeyear() const;

  ColorList const&
  color() const
  { return color_; }

  bool
  displayscolor() const
  { return displaycolor_; }

  std::string const&
  longname() const
  { return longname_; }

  std::string const&
  shortname() const
  { return shortname_; }

  int
  modifieddriveskill(int skillLevel);

  int
  modifieddodgeskill(int skillLevel);

  int
  attackbonus(bool isDriving) const
  { return isDriving ? attackbonus_driver_ : attackbonus_passenger_; }

  int
  gethitlocation(int bodypart);

  std::string
  getpartname(int location);

  int
  armorbonus(int location);

  int
  steal_difficultytofind() const
  { return steal_difficultytofind_; }

  int
  steal_juice() const
  { return steal_juice_; }

  int
  steal_extraheat() const
  { return steal_extraheat_; }

  int
  sensealarmchance() const
  { return sensealarmchance_; }

  int
  touchalarmchance() const
  { return touchalarmchance_; }

  bool
  availableatshop() const
  { return availableatshop_; }

  int
  price() const
  { return price_; }

  int
  sleeperprice() const
  { return sleeperprice_; }

  static int number_of_vehicletypes;

private:
  std::string  idname_{"UNDEFINED"};
  long         id_{-1};

  bool         year_startcurrent_{true};
  int          year_start_{0};
  bool         year_randomuptocurrent_{false};
  int          year_addrandom_{0};
  int          year_add_{0};

  ColorList    color_;
  bool         displaycolor_{true};
  std::string  longname_{"UNDEFINED"};
  std::string  shortname_{"UNDEF"};
  int          drivebonus_{0};
  float        drivebonus_factor_{1.f};
  int          drivebonus_limit1_{8};
  int          drivebonus_limit2_{99};
  int          dodgebonus_{0};
  float        dodgebonus_factor_{1.f};
  int          dodgebonus_limit1_{8};
  int          dodgebonus_limit2_{99};
  int          attackbonus_driver_{-2};
  int          attackbonus_passenger_{0};

  int          armormidpoint_{50};
  int          lowarmormin_{4};
  int          lowarmormax_{6};
  int          higharmormin_{0};
  int          higharmormax_{2};

  int          steal_difficultytofind_{1};
  int          steal_juice_{0};
  int          steal_extraheat_{0};
  int          sensealarmchance_{0};
  int          touchalarmchance_{0};

  bool         availableatshop_{true};
  int          price_{1234}; // @TODO make this an Interval
  int          sleeperprice_{1111};
};

#endif // VEHICLE_TYPE_H
