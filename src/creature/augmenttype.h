/**
 * Interface for the augmentations submodule.
 */
/*
 * Copyright 2015,2016 Kamal-Sadek  <kamaljalals@gmail.com>
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
#ifndef AUGMENT_TYPE_H
#define AUGMENT_TYPE_H

#include <string>


class Augmentation;


class AugmentType {
   public:
      explicit AugmentType(const std::string& xmlstring);

      void make_augment(Augmentation& au);

      std::string get_idname() const { return idname_; }
      std::string get_name() const { return name_; }
      long get_id() const { return id_; }
      short get_type() const { return type_; }
      int get_attribute() const { return attribute_; }
      int get_effect() const { return effect_; }
      int get_max_age() const { return max_age_; }
      int get_min_age() const { return min_age_; }
      int get_cost() const { return cost_; }
      int get_difficulty() const { return difficulty_; }
      std::string get_description() const { return description_; }

      static int number_of_augmenttypes;
   private:
      std::string idname_;
      std::string name_;
      int attribute_;
      int effect_;
      int max_age_;
      int min_age_;
      long id_;
      short type_;
      int cost_;
      int difficulty_;
      std::string description_;
};

#endif
