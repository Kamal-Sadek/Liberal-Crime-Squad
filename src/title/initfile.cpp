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

#include <algorithm>
#include <externs.h>

void setconfigoption(std::string name, std::string value)
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
      if(stringtobool(value)==0)
         autosave=false;
      else
         autosave=true;

   }
   #ifdef _WIN32
   else if(name == "fixcleartype") // this setting is only true if set in the file AND running Windows XP or later, otherwise it's false
   {
      if(stringtobool(value)==1)
      { // it's set to true in init.txt, so now we check if we're running Windows XP or later, since earlier versions don't have ClearType
         OSVERSIONINFO osvi;
         ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
         osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
         GetVersionEx(&osvi);
         fixcleartype=((osvi.dwMajorVersion>5)||((osvi.dwMajorVersion==5)&&(osvi.dwMinorVersion>=1))); // Windows XP is version 5.1
      }
   }
   #endif
}

void loadinitfile()
{
   std::fstream file;
   if(LCSOpenFileCPP("init.txt", ios::in, LCSIO_PRE_HOME, file))
   {
      std::string str;
      int posequal;

      while(getline(file,str))
      {
         str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
         str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
         str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
         str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());

         if(!len(str)) continue;

         if(str[0] == '#') continue;
         if(str[0] == ';') continue;

         posequal=str.find('=');
         if(posequal==(int)string::npos) continue;

         setconfigoption(str.substr(0,posequal),str.substr(posequal+1));
      }
   }
   file.close();

   #ifdef _WIN32
   begin_cleartype_fix(); // won't do anything unless fixcleartype is true
   #endif
}
