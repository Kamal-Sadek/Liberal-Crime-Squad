/**
 * Interface of the creature Gender submodule.
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
#ifndef LCS_CREATURE_GENDER_H
#define LCS_CREATURE_GENDER_H 1


enum CreatureGender
{
   GENDER_NEUTRAL,
   GENDER_MALE,
   GENDER_FEMALE,

   // Used to get some more specific names.
   GENDER_WHITEMALEPATRIARCH,

   // Used in creature creation.
   GENDER_MALE_BIAS,
   GENDER_FEMALE_BIAS,
   GENDER_RANDOM
};


#endif /* LCS_CREATURE_GENDER_H */
