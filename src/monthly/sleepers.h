/**
 * Interface for sleeprs.
 */
/*
 * Copyright 2017 Stephen M. Webb
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
#ifndef LCS_MONTHLY_SLEEPERS_H
#define LCS_MONTHLY_SLEEPERS_H

#include "includes.h" // for VIEWNUM .. @TODO: you know what to do

class Creature;


void
sleepereffect(Creature& cr, char& clearformess, bool canseethings, int (&libpower)[VIEWNUM]);


#endif /* LCS_MONTHLY_SLEEPERS_H */
