/**
 * Interface for the daily event processing module.
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
#ifndef DAILY_DAILY_H
#define DAILY_DAILY_H

class Creature;


void
advanceday(char& clearformess, char canseethings);

/* squad members with no chain of command lose contact */
void
dispersalcheck(char& clearformess);

/* promote a subordinate to maintain chain of command when boss is lost */
bool
promotesubordinates(Creature& cr, char& clearformess);

/* daily - manages too hot timer and when a site map should be re-seeded and renamed */
void
advancelocations();

/* daily - returns true if the site type supports high security */
char
securityable(int type);

/* daily - returns the number of days in the current month */
int monthday();

#endif /* DAILY_DAILY_H */
