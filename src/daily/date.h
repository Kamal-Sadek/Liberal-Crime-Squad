/**
 * Interface for Date module.
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
#ifndef LCS_DAILY_DATE_H
#define LCS_DAILY_DATE_H

class datest;


/* Date p gets back from vacation. */
bool
completevacation(datest& d, int p, char& clearformess);

/* Date p goes on some dates. */
bool
completedate(datest& d, int p, char& clearformess);

#endif /* LCS_DAILY_DATE_H */
