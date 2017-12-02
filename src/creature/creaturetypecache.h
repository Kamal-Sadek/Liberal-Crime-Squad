/**
 * The CreatureType collection.
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
#ifndef LCS_CREATURETYPECACHE_H
#define LCS_CREATURETYPECACHE_H

#include "creature/creaturetype.h"
#include <memory>
#include <string>
#include <vector>


/**
 * A collection of CreatureType objects.
 */
class CreatureTypeCache
{
public:
  using size_type = std::size_t;

public:
  ~CreatureTypeCache();

  /** Create a creature type cache from a string containing XML. */
  void
  load_from_xml_string(std::string const& xml);

  /** Get a creature type by internal identifier. */
  CreatureType*
  get_by_type(short type);

  /** Get a creature type by idname. */
  CreatureType*
  get_by_idname(std::string const& idname);

  /** Get the number of creature type definitions in the cache. */
  size_type
  size() const
  { return creature_type_bag.size(); }

private:
  using CreatureTypeOwningPtr = std::unique_ptr<CreatureType>;
  using CreatureTypeBag = std::vector<CreatureTypeOwningPtr>;

  CreatureTypeBag creature_type_bag;
};

#endif /* LCS_CREATURETYPECACHE_H */
