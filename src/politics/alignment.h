/*
 * This file is part of Liberal Crime Squad.
 *
 * Copyright 2008, 2009 Jonathan Stickles  <jonathansfox@users.sourceforge.net>
 * Copyright 2014 Rich McGrew (yetisyny)
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef ALIGNMENT_H_INCLUDED
#define ALIGNMENT_H_INCLUDED

#include <initializer_list>
#include <iosfwd>
#include <string>


enum class Alignment
{
  PUBLIC_MOOD = -100,
  ARCH_CONSERVATIVE = -2,
  CONSERVATIVE,
  MODERATE,
  LIBERAL,
  ELITE_LIBERAL,
  STALINIST
};


using AlignmentChoices = std::initializer_list<Alignment>;

/**
 * Indicates if the alignment is extremist.
 */
bool
is_extreme(Alignment alignment);

/**
 * Indicates @p lhs is to the left of (more liberal than) @p rhs.
 */
bool
to_left_of(Alignment lhs, Alignment rhs);

/**
 * Indicates @p lhs is to the right of (more conservative than) @p rhs.
 */
bool
to_right_of(Alignment lhs, Alignment rhs);

/**
 * Choose an alignment from a manifest list of choices.
 */
Alignment
choose(AlignmentChoices choices);

/**
 * Return a more liberal alignment.
 */
Alignment
shift_left(Alignment alignment, int amount=1);

/**
 * Return a more conservative alignment.
 */
Alignment
shift_right(Alignment alignment, int amount=1);

/**
 * Shuft to the left (positive) or right (negative).
 */
Alignment
shift(Alignment alignment, int steps);

/**
 * Pretty-print an alignment.
 */
std::string
as_printable(Alignment alignment, bool capitalize = true);

/**
 * Use an Alignment as an array index.
 *
 * The use of this should be replaced by something more sophisticated otherwise
 * there are too many dependencies between the underlying implementation of
 * Alignment and thingd that use it.
 */
std::size_t
to_index(Alignment alignment);

/**
 * Get a numeric difference in magnitude between two alignments.
 */
std::size_t
diff(Alignment lhs, Alignment rhs);


/**
 * Unmarshall an Alignment from a string..
 *
 * @param[in]  text      A string.
 * @param[out] alignment An alignment.
 * @return true if successful, false if the string can not be converted.
 */
bool
from_string(std::string const& text, Alignment& alignment);

/**
 * Marshall an Alignment to a string.
 *
 * @param[in] alignment An alignment.
 * @return A string of a marshalled alignment.
 */
std::string
to_string(Alignment alignment);


inline std::ostream&
operator<<(std::ostream& ostr, Alignment rhs)
{ return ostr << to_string(rhs); }

#endif //ALIGNMENT_H_INCLUDED
