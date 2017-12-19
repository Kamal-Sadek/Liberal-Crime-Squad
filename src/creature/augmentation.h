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
#ifndef AUGMENTATION_H
#define AUGMENTATION_H

#include <string>


enum Augmentations
{
   AUGMENTATION_HEAD,
   AUGMENTATION_BODY,
   AUGMENTATION_ARMS,
   AUGMENTATION_LEGS,
   AUGMENTATION_SKIN,
   AUGMENTATIONNUM
};


class Augmentation
{
public:
    Augmentation() { }
    Augmentation(std::string const& inputXml);
    std::string showXml() const;

    std::string name{""};
    short type{-1};
    int attribute{-1};
    int effect{-1};
    int value{-1};

    static std::string get_name(int augmentation_type);
    static int get_associated_attribute(int augmentation_type);
};

#endif
