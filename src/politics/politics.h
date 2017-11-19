/**
 * Interface for the politics module.
 */
/*
 * Copyright (c) 2002,2003,2004 by Tarn Adams
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
#ifndef LCS_POLITICS_POLICS_H
#define LCS_POLITICS_POLICS_H

/* Calculate presidential approval. */
int
presidentapproval();

/* Promote the Vice President to President, and replaces VP. */
void
promoteVP();

/* Cause the people to vote (presidential, congressional, propositions). */
void
elections(bool clearformess, bool canseethings);

void
elections_senate(int senmod, bool canseethings);

void
elections_house(bool canseethings);

/* Cause the supreme court to hand down decisions. */
void
supremecourt(bool clearformess, bool canseethings);

/* Cause congress to act on legislation. */
void
congress(bool clearformess, bool canseethings);

/* Send letter of amnesty to the LCS from the President (you win). */
void
amnesty();

/* Check if the game is won. */
char
wincheck();

/* Check the prevailing attitude on a specific law, or overall. */
int
publicmood(int l);

bool
stalinview(short view, bool islaw);

#endif /* LCS_POLITICS_POLICS_H */
