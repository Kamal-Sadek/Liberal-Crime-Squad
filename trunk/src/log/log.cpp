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

#include "includes.h"
#include "externs.h"
#include "lcsio.h"

#include <fstream>

//Constructor.
Log::Log()
{
   //This is set in the initialize() function.
   initialized = false;

   //Yes, seems redundant. However, I've had situations in the past where the compiler
   //(some compiler in windows) murdered me if I didn't do this. And I want to live.
   //Besides, it's good practice to always explicitly initialize a variable.
   //You never know what a compiler's going to initialize something as :)
   filename = "";

   //Set this to whatever the default has been defined as.
   newline_mode = NEWLINEMODE_LOGFILES_DEFAULT;
}

//Deconstructor.
Log::~Log()
{
   //Check if the logger is initialized.
   if(initialized)
   {
      //It is.

      LCSCloseFileCPP(file); //Close the file.
   }
}

//The initialization function.
bool Log::initialize(string _filename, bool overwrite_existing, int _newline_mode)
{
   filename = _filename; //Assign the filepath.

   newline_mode = _newline_mode; //Set the newline mode

   if(!overwrite_existing) //check If it is to append the output to the end of the log file rather than to overwrite the file.
   {
      //Yes, it is to append.
      if(!LCSOpenFileCPP(filename, ios::out | ios::app, LCSIO_PRE_HOME, file)) //Open the file in append mode. With error checking.
      {
         return false; //Failed to open file.
      }
   }
   else //overwrite_existing = true. Overwrite the file.
   {
      //Open the file. Use the trunc parameter to ensure the file is going to be overwritten.
      if(!LCSOpenFileCPP(filename, ios::out | ios::trunc, LCSIO_PRE_HOME, file)) //With error checking.
      {
         return false; //Failed to open file.
      }
   }
   //File's open and everything. Okay to proceed.

   initialized = true; //The logger is now initialized.

   return true; //*super smash brothers melee announcer* Success!
}

//This is the actual logging function.
bool Log::log(string text)
{
   if(text == "") //Check if no text given.
   {
      //No text given. No reason to continue.
      //Also, it acts as a guard to the text.size() < 2 function.
      //At least for vectors, text.size() where there are no elements is...it returns some random huge number.
      //So, let's play it safe.
      //It also acts as a guard to the text.size() - 1 functions, since size should be == 0,
      //which means there is no elemenet 1, and thus SEGFAULT. Which we don't want.
      return true; //Abort.
   }

   if(initialized && file.is_open()) //Guard to make sure that it doesn't try to write text when the logger isn't even initialzied or the file isn't even loaded!
   {
      file.clear(); //First, clear the state flags.

      file << text; //Output the text.

      if(newline_mode != 0) //If the log is supposed to be adding newlines.
      {
         //I could make this support more than double newlines, but that's not needed right now.
         if(text[text.size() - 1] != '\n') //Check if the last character is not a newline.
         {
            //It is not. Go ahead and write a newline character to the file.
            for(int i = 0; i < newline_mode; ++i) //This loop ensures that all the required newlines are written.
            {
               file << "\n";
            }
         }
         else if(newline_mode == 2) //If supposed to use double lines.
         {
            if(text.size() < 2) //Check if text is only one character long.
            {
               //Text is too small to have two newlines. That means it has only one element, and it's already newline.
               file << "\n"; //It only needs one more newline.
            }
            else if(text[text.size() - 2] != '\n') //One before the last not a newline? Add a newline then.
            {
               file << "\n"; //Add a newline.
            }
            //There is no else. We don't need to do anything if two newlines are already present.
         }
      }

      file.flush(); //Force it to write out everything now.

      if(!file.good()) //Make sure everything's good and there are no problems.
      {
         return false; //Ruh Roh! Something went wrong!
      }

      return true; //Success!
   }
   else
   {
      //The logger is not initialized or the file isn't open. Something done went wrong.
      return false; //Abort!
   }
}
