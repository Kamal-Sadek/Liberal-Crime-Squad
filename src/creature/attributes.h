/**
 * Interface for the creature Attributes submodule.
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
#ifndef LCS_CREATURE_ATTRIBUTES_H_H
#define LCS_CREATURE_ATTRIBUTES_H_H

#include <string>


enum CreatureAttribute
{
   ATTRIBUTE_STRENGTH,
   ATTRIBUTE_INTELLIGENCE,
   ATTRIBUTE_WISDOM,
   ATTRIBUTE_AGILITY,
   ATTRIBUTE_HEALTH,
   ATTRIBUTE_CHARISMA,
   ATTRIBUTE_HEART
};

constexpr std::size_t ATTNUM = 7;
constexpr int         MIN_ATTR_LEVEL = 0;
constexpr int         MAX_ATTR_LEVEL = 99;


class Attribute
{
public:
   Attribute()
   { }

   Attribute(std::string const& inputXml);

   std::string
   showXml() const;

   void
   set_type(int attribute_type)
   { attribute = attribute_type; }

   static std::string get_name(int attribute_type);

public:
   int value;

private:
   int attribute;
};

#endif /* LCS_CREATURE_ATTRIBUTES_H_H */

