/*
This file handles the loading of init.txt

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
#include "lcsio.h"
#include "externs.h"
#include "includes.h"

void setconfigoption(string name, string value)
{
   transform(name.begin(), name.end(), name.begin(), ::tolower);
   transform(value.begin(), value.end(), value.begin(), ::tolower);
   if(name == "pagekeys")
   {
      if(value == "azerty")
      {
         interface_pgup='.';
         interface_pgdn='/';
      }
      else if(value == "brackets")
      {
         interface_pgup='[';
         interface_pgdn=']';
      }
      else if(value == "page")
      {
         interface_pgup=-61;
         interface_pgdn=-55;
      }
   }
   else if(name == "autosave")
   {
      if((value == "off") or (value == "0") or (value == "false") or (value == "no"))
         autosave=false;
   }
}

void loadinitfile(void)
{
   ::fstream file;
   if (LCSOpenFileCPP("init.txt", ios::in, LCSIO_PRE_HOME, file))
   {
      string str;
      int posequal;

      while(getline(file, str))
      {
         str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
         str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());

         if (! str.length()) continue;

         if (str[0] == '#') continue;
         if (str[0] == ';') continue;

         posequal=str.find('=');
         if(posequal==string::npos) continue;

         setconfigoption(str.substr(0,posequal), str.substr(posequal+1));
      }
   }
   file.close();
}
