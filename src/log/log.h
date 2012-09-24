/*
    log.h

    Ciprian Ilies

    This file contains a logging class.
    It logs raw output, not xml or anything fancy.

    This uses c++ file i/o.

This file is part of Liberal Crime Squad.

    Liberal Crime Squad is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Liberal Crime Squad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Liberal Crime Squad; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA

*/

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "includes.h"

class Log
{
public:

   Log();
   ~Log();

   /*
   Initializes a new logger.
   parameters:
    - _filename
       - A string. Stores the name/relevant path of the file it's going to be logging to.
    - overwrite_existing
       - A boolean value.
          - true = overwrite existing file.
          - false = append to it.
    - newline_mode
       - An integer value.
          - 0 = no automatic newline. Leaves it up to the caller to properly format the output.
          - 1 = newline
          - 2 = double newline
   */
   bool initialize(string _filename, bool overwrite_existing, int _newline_mode = NEWLINEMODE_LOGFILES_DEFAULT);

   /*
   Adds the provided text to the file. Logs it, yup.
   parameters:
    - text
       - A string. It's the text that the function is supposed to output.
   Notes:
    - The function automatically appends a newline to the end of the output, if not already present, if auto_newline == true.
   */
   bool log(string text);

private:

   //False if not initialized. True if initialized. (All of this is via the initialize() function)
   //Makes sure that the programmer initialized the function prior to attempted usage.
   bool initialized;

   string filename; //The name/relevant path of the file.
   //The file itself.
   //fstream for easier use in case I (or somebody else) wants to add a "read log" feature.
   //I would like that; so that one could check events and the like ingame.
   fstream file;

   //Controls the automatic addition of newlines.
   //0 = no newlines.
   //1 = newline.
   //2 = double newlines.
   //TODO: And make it support further automatic newlines.
   int newline_mode;
};

#endif //LOG_H_INCLUDED
