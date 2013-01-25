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

/*
Here's an example of how to use gamelog:

We have:
------

move(8,1);
addstr("A sniper takes out ");
addstr(pool[targ]->name);
addstr("!");

------


Change it to:
------

move(8,1);
addstr("A sniper takes out ", gamelog);
addstr(pool[targ]->name, gamelog);
addstr("!", gamelog);
gamelog.nextMessage();

------


If there are times where you want to log stuff differently than displayed in the
game or not to be displayed at all, simply use:
------

gamelog.log("This is a message we don't want the user to see ingame.");

------


Let's say you want to make a newline in a message. Do the following:
------

addstr("A sniper takes out ", gamelog);
addstr(pool[targ]->name, gamelog);
addstr("!", gamelog);
gamelog.newline();
addstr("The sniper breaks into a dance!");
gamelog.nextMessage();

------

*/

class Log
{
public:

   //This is used to work around all those empty lines output into the log at the
   //end of an encounter's round.
   bool logged_since_last_message;

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
      The following three functions, begl(), endl(), and record(), are specifically
      for the gamelog, not general purpose logging.
   */
   /*//Tells the print function to route input to this.
   void begl();
   //Stops routing input from the print function.
   void endl();
   //Used in conjuction with the above two.
   //When the print function is called, if begl() has been called, the print function
   //calls this function and passes the text it's supposed to be printing to here
   //so that we can log it.
   void record(string text);*/

   //Adds the text given to the buffer.
   void record(string text);

   //Writes out everything currently in the buffer to the file, so as to split the
   //log into logical blocks.
   void nextMessage();

   /*
   Adds the provided text to the file. Logs it, yup.
   parameters:
    - text
       - A string. It's the text that the function is supposed to output.
   Notes:
    - The function automatically appends a newline to the end of the output, if
         not already present, if auto_newline == true.

   Use the record() function in conjunction with the nextMessage() function to log messages.

   Use this function if you want to log something that should not also be displayed ingame.
   */
   bool log(string text);

   //Sets the newline mode.
   //Values work the same as with the initialize function for the newline_mode
   //parameter.
   void newlmode(int new_newline_mode);

   //Writes out a newline.
   void newline();

private:

   //False if not initialized. True if initialized (All of this is via the initialize() function).
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
   //TODO: And make it support further automatic newlines (eg. 3, 4, 5, etc). Not
   //really a priority due to the fact that the current system works good enough
   //(when are you going to need more than two doublelines consistently?).
   int newline_mode;

   //What has been recorded so far (used in begl(), endl(), and record()).
   string buffer;
};

#endif //LOG_H_INCLUDED
