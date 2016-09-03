
#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
#include <includeEnum.h>
#include <includeCommon.h>

/*
stringconversion.cpp
*/
#include "common\\stringconversion.h"

//#include <includeNews.h>
//#include <includeFunctions.h>
//#include <includeTitle.h>

#include <includeTalk.h>
#include <includeExternDefault.h>
extern char homedir[MAX_PATH_SIZE];
extern char artdir[MAX_PATH_SIZE];
//#include <includeExternPolitics.h>
//#include <includeExternStat.h>


std::ifstream* openFile(const std::string& filename, std::ios_base::openmode format)
{
   std::ifstream* file = new std::ifstream();
   addstr("Attempting to open filename: ");
   addstr(filename);
   addstr(" ");

   file->open((artdir+filename).c_str(), format);

   return file;
}

// Reads in an entire configuration file
// Returns true for read successful, returns false if failed read
int readConfigFile(const std::string& filename)
{
   std::string command, value;
   configurable* object=0;

   std::ifstream* file = openFile(filename, ios::in);
   if(!file->is_open())
   {
      delete file;
      return false; // Unable to open; failed read
   }

   // loop through lines
   while(readLine(*file, command, value))
      // if COMMAND is OBJECT,
      // call a object creator factory, passing VALUE
      // (record pointer to that object)
      if(command=="OBJECT") object=createObject(value);
      // if I have an object,
      // pass COMMAND and VALUE to the object
      else if(object) object->configure(command, value);
      else
      {
         file->close();
         delete file;
         return false; // Unknown command and no object to give it to; failed read
      }
   file->close();
   delete file;
   return true;
}


// readLine reads a line from the file, parses it
int readLine(std::ifstream& file, std::string& command, std::string& value)
{
   std::string line;
   int source=0;

   // Search for a non-comment, non-empty line
   do
   {
      if(file.eof()) return false;
      getline(file,line);
      line.erase(std::remove(line.begin(),line.end(),'\r'),line.end());
      line.erase(std::remove(line.begin(),line.end(),'\n'),line.end());
   } while(!len(line)||line[0]=='#');

   // Parse the line
   command.clear();
   value.clear();

   // Leading whitespace
   for(;source<len(line)&&(line[source]==' '||line[source]=='\t');source++);

   // Command
   for(;source<len(line)&&(line[source]!=' '&&line[source]!='\t');command.push_back(line[source++]));

   // Delimiting whitespace
   for(;source<len(line)&&(line[source]==' '||line[source]=='\t');source++);

   // Value
   for(;source<len(line)&&(line[source]!=' '&&line[source]!='\t');value.push_back(line[source++]));

   return true;
}

// Constructs the new object, returns a pointer to it
configurable* createObject(const std::string& objectType)
{
   configurable* object=0;
   if(objectType=="SITEMAP")
      sitemaps.push_back(static_cast<configSiteMap*>(object=new configSiteMap));
   return object;
}

void readMapCBSpecials(int x, int y, int z, int i)
{
   switch(i)
   {
   default:
   case 0: levelmap[x][y][z].special = SPECIAL_NONE; break;
   case 1: levelmap[x][y][z].special = SPECIAL_LAB_COSMETICS_CAGEDANIMALS; break;
   case 2: levelmap[x][y][z].special = SPECIAL_LAB_GENETIC_CAGEDANIMALS; break;
   case 3: levelmap[x][y][z].special = SPECIAL_POLICESTATION_LOCKUP; break;
   case 4: levelmap[x][y][z].special = SPECIAL_COURTHOUSE_LOCKUP; break;
   case 5: levelmap[x][y][z].special = SPECIAL_COURTHOUSE_JURYROOM; break;
   case 6: levelmap[x][y][z].special = SPECIAL_PRISON_CONTROL; break;
   case 7: levelmap[x][y][z].special = SPECIAL_PRISON_CONTROL_LOW; break;
   case 8: levelmap[x][y][z].special = SPECIAL_PRISON_CONTROL_MEDIUM; break;
   case 9: levelmap[x][y][z].special = SPECIAL_PRISON_CONTROL_HIGH; break;
   case 10: levelmap[x][y][z].special = SPECIAL_INTEL_SUPERCOMPUTER; break;
   case 11: levelmap[x][y][z].special = SPECIAL_SWEATSHOP_EQUIPMENT; break;
   case 12: levelmap[x][y][z].special = SPECIAL_POLLUTER_EQUIPMENT; break;
   case 13: levelmap[x][y][z].special = SPECIAL_NUCLEAR_ONOFF; break;
   case 14: levelmap[x][y][z].special = SPECIAL_HOUSE_PHOTOS; break;
   case 15: levelmap[x][y][z].special = SPECIAL_HOUSE_CEO; break;
   case 16: levelmap[x][y][z].special = SPECIAL_CORPORATE_FILES; break;
   case 17: levelmap[x][y][z].special = SPECIAL_RADIO_BROADCASTSTUDIO; break;
   case 18: levelmap[x][y][z].special = SPECIAL_NEWS_BROADCASTSTUDIO; break;
   case 19: levelmap[x][y][z].special = SPECIAL_APARTMENT_LANDLORD; break;
   case 20: levelmap[x][y][z].special = SPECIAL_SIGN_ONE; break;
   case 21: levelmap[x][y][z].special = SPECIAL_RESTAURANT_TABLE; break;
   case 22: levelmap[x][y][z].special = SPECIAL_CAFE_COMPUTER; break;
   case 23: levelmap[x][y][z].special = SPECIAL_PARK_BENCH; break;
   case 24: levelmap[x][y][z].special = SPECIAL_STAIRS_UP; break;
   case 25: levelmap[x][y][z].special = SPECIAL_STAIRS_DOWN; break;
   case 26: levelmap[x][y][z].special = SPECIAL_CLUB_BOUNCER; break;
   case 27: levelmap[x][y][z].special = SPECIAL_CLUB_BOUNCER_SECONDVISIT; break;
   case 28: levelmap[x][y][z].special = SPECIAL_ARMORY; break;
   case 29: levelmap[x][y][z].special = SPECIAL_DISPLAY_CASE; break;
   case 30: levelmap[x][y][z].special = SPECIAL_SIGN_TWO; break;
   case 31: levelmap[x][y][z].special = SPECIAL_SIGN_THREE; break;
   case 32: levelmap[x][y][z].special = SPECIAL_SECURITY_CHECKPOINT; break;
   case 33: levelmap[x][y][z].special = SPECIAL_SECURITY_METALDETECTORS; break;
   case 34: levelmap[x][y][z].special = SPECIAL_SECURITY_SECONDVISIT; break;
   case 35: levelmap[x][y][z].special = SPECIAL_BANK_VAULT; break;
   case 36: levelmap[x][y][z].special = SPECIAL_BANK_TELLER; break;
   case 37: levelmap[x][y][z].special = SPECIAL_BANK_MONEY; break;
   case 38: levelmap[x][y][z].special = SPECIAL_CCS_BOSS; break;
   case 39: levelmap[x][y][z].special = SPECIAL_OVAL_OFFICE_NW; break;
   case 40: levelmap[x][y][z].special = SPECIAL_OVAL_OFFICE_NE; break;
   case 41: levelmap[x][y][z].special = SPECIAL_OVAL_OFFICE_SW; break;
   case 42: levelmap[x][y][z].special = SPECIAL_OVAL_OFFICE_SE; break;
   }
}

void makeDoor(int x,int y,int z,int flags=0)
{
   levelmap[x][y][z].flag=SITEBLOCK_DOOR|flags;
   if((x>0&&(levelmap[x-1][y][z].flag&SITEBLOCK_RESTRICTED))||
      (y>0&&(levelmap[x][y-1][z].flag&SITEBLOCK_RESTRICTED)))
      levelmap[x][y][z].flag|=SITEBLOCK_RESTRICTED;
}

void readMapCBTiles(int x, int y, int z, int i)
{
   switch(i)
   {
   default:
   case 0: levelmap[x][y][z].flag=0; break;
   case 2: levelmap[x][y][z].flag=SITEBLOCK_BLOCK; break;
   case 3: levelmap[x][y][z].flag=SITEBLOCK_EXIT; break;
   case 4: levelmap[x][y][z].flag=SITEBLOCK_GRASSY; break;
   case 5: makeDoor(x,y,z); break;
   case 6: makeDoor(x,y,z,SITEBLOCK_LOCKED); break;
   case 7:
      levelmap[x][y][z].flag=SITEBLOCK_RESTRICTED;
      if(x>0&&(levelmap[x-1][y][z].flag&SITEBLOCK_DOOR))
         levelmap[x-1][y][z].flag|=SITEBLOCK_RESTRICTED;
      if(y>0&&(levelmap[x][y-1][z].flag&SITEBLOCK_DOOR))
         levelmap[x][y-1][z].flag|=SITEBLOCK_RESTRICTED;
      break;
   case 8: levelmap[x][y][z].flag=SITEBLOCK_CHAINLINK; break;
   case 9: makeDoor(x,y,z,SITEBLOCK_LOCKED|SITEBLOCK_ALARMED); break;
   case 10: levelmap[x][y][z].flag=SITEBLOCK_BLOCK|SITEBLOCK_METAL; break;
   case 11: makeDoor(x,y,z,SITEBLOCK_LOCKED|SITEBLOCK_METAL); break;
   }
}

bool readMapFile(const string &filename, const int zLevel, void (*callback)(int,int,int,int))
{
   // open the file in question
   std::ifstream* file = openFile(filename, ios::in);

   // abort if the file couldn't be opened
   if(!file->is_open())
   {
      delete file;
      return false;
   }

   for(int y=0,z=zLevel;!file->eof();y++)
   {
      std::string line;
      getline(*file,line);
      line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
      line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

      for(int x=0,i=0,j=0;j<len(line);x++,i=++j)
      {
         for(;j<len(line)&&line[j]!=',';j++);
         if(j<len(line))
         {
            line[j]=0;
            (*callback)(x,y,z,atoi(line.substr(i)));
         }
      }
   }

   file->close();
   delete file;
   return true;
}

bool readMap(const std::string& filename)
{
   std::string prefix = std::string("mapCSV_");

   // clear any old map data
   for(int x=0;x<MAPX;x++)
   for(int y=0;y<MAPY;y++)
   for(int z=0;z<MAPZ;z++)
   {
      levelmap[x][y][z].flag=0;
      levelmap[x][y][z].special=SPECIAL_NONE;
      levelmap[x][y][z].siegeflag=0;
   }

   // Try first floor (eg "mapCSV_Bank_Tiles.csv"), abort this method if it doesn't exist
   if(!readMapFile(prefix+filename+"_Tiles.csv", 0, readMapCBTiles)) return false;
   if(!readMapFile(prefix+filename+"_Specials.csv", 0, readMapCBSpecials)) return false;

   // Try upper levels (eg "mapCSV_Bank2_Tiles.csv"), but don't sweat it if they don't exist
   for(int z=1;z<MAPZ;z++)
   {
      std::string str=tostring(z+1);
      if(!readMapFile(prefix+filename+str+"_Tiles.csv", z, readMapCBTiles)) break;
      if(!readMapFile(prefix+filename+str+"_Specials.csv", z, readMapCBSpecials)) break;
   }

   return true;
}
