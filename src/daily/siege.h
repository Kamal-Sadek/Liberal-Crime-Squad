/**
 * Interface for the seige module.
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
#ifndef LCS_DAILY_SIEGE_H
#define LCS_DAILY_SIEGE_H

class Creature;


/* Update upcoming sieges. */
void
siegecheck(bool canseethings);

/* Update sieges in progress. */
void
siegeturn(bool clearformess);

/* Handles giving up. */
void
giveup();

/* Check how many days of food left at the site. */
int
fooddaysleft(int loc);

/* Check how many people are eating at the site. */
int
numbereating(int loc);

/* Prepare for massed combat outside the safehouse. */
void
sally_forth();

/* Prepare for entering site mode to fight the siege. */
void
escape_engage();

/* Escape the siege. */
void
escapesiege(char won);

/* Flavor text when you fought off the raid. */
void
conquertext();

/* Flavor text when you crush a CCS safe house. */
void
conquertextccs();

void
statebrokenlaws(Creature const& cr);

#endif /* LCS_DAILY_SIEGE_H */
