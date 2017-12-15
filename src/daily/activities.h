/**
 * Interface for things to do in a day.
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
#ifndef DAILY_ACTIVITIES_H
#define DAILY_ACTIVITIES_H


class Creature;


/* adjust blog power */
void
adjustblogpower(int& power);

/* hostage tending */
void
tendhostage(Creature *cr, char& clearformess);

/* armor repair */
void
repairarmor(Creature& cr, char& clearformess);

/* armor manufacture */
void
makearmor(Creature& cr, char& clearformess);

/* search for polls */
void
survey(Creature* cr);

/* misc activation related things */
void
funds_and_trouble(char& clearformess);

/* steal a car */
bool
stealcar(Creature& cr, char& clearformess);

bool
carselect(Creature& cr, short& cartype);

/* get a wheelchair */
void
getwheelchair(Creature& cr, char& clearformess);

void
augment(Creature& cr, char& clearformess);

#endif /* DAILY_ACTIVITIES_H */
