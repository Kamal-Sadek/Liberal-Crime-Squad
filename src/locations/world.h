/**
 * Interface for the World submodule.
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
#ifndef LCS_LOCATIONS_WORLD_H
#define LCS_LOCATIONS_WORLD_H

class Creature;


Location*
find_site_by_id(int id);

Location*
find_site_in_city(int site_type, int city);

int
find_site_index_in_city(int site_type, int city);

/* find local versions of these locations */
int
find_site_index_in_same_city(int site_type, int site_index);

int
find_site_index_in_same_city(int site_type, Creature const& cr);

int
find_police_station(int site_index);

int
find_police_station(Creature const& cr);

int
find_clinic(int site_index);

int
find_clinic(Creature const& cr);

int
find_homeless_shelter(int site_index);

int
find_homeless_shelter(Creature const& cr);

int
find_courthouse(int site_index);

int
find_courthouse(Creature const& cr);

int
find_hospital(int site_index);

int
find_hospital(Creature const& cr);

/* sets up the list of locations */
void
make_world(bool hasmaps);

#endif /* LCS_LOCATIONS_WORLD_H */

