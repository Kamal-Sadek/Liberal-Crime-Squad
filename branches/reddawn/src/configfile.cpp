
#include "configfile.h"
#include "sitemode/sitemap.h"
#include "externs.h"

#define MAX_PATH_SIZE 2048
extern char artdir[MAX_PATH_SIZE];


std::ifstream* openFile(const char* filename, std::ios_base::openmode format)
{
   std::ifstream *file = new std::ifstream();
   addstr("Attempting to open filename: ");
   addstr(filename);
   addstr(" ");

   char extendedfilename[MAX_PATH_SIZE];
   strcpy(extendedfilename,artdir);
   strcat(extendedfilename,filename);
   file->open(extendedfilename, format);

   return file;
}

// Reads in an entire configuration file
// Returns true for read successful, returns false if failed read
int readConfigFile(const char* filename)
{
   std::string command, value;
   configurable* object = 0;

   std::ifstream* file = openFile(filename, ios::in);
   if(!file->is_open())
   {
      delete file;
      return false; // Unable to open; failed read
   }

   // loop through lines
   while(readLine(*file, command, value))
   {
      
      // if COMMAND is OBJECT
      if(command == "OBJECT")
      {
         // call a object creator factory, passing VALUE
         // (record pointer to that object)
         object = createObject(value);
      }
      else
      {
         // if I have an object
         if(object)
         {
            // pass COMMAND and VALUE to the object
            object->configure(command, value);
         }
         else
         {
            file->close();
            delete file;
            return false; // Unknown command and no object to give it to; failed read
         }
      }
   }
   file->close();
   delete file;
   return true;
}


// readLine reads a line from the file, parses it
int readLine(std::ifstream& file, std::string& command, std::string& value)
{
   char line[600];
   int source=0;

   // Search for a non-comment, non-empty line
   do
   {
      if(file.eof()) return 0;
      file.getline(line,598);
   } while(line[0] == '#' || line[0] == 0);

   // Parse the line
   command.clear();
   value.clear();

   // Leading whitespace
   while(source<600 && (line[source]==' ' || line[source]=='\t') && line[source]!=0)
      source++;

   // Command
   while(source<600 && (line[source]!=' ' && line[source]!='\t') && line[source]!=0)
      command.push_back(line[source++]);

   // Delimiting whitespace
   while(source<600 && (line[source]==' ' || line[source]=='\t') && line[source]!=0)
      source++;

   // Value
   while(source<600 && (line[source]!=' ' && line[source]!='\t') && line[source]!=0)
      value.push_back(line[source++]);

   return 1;
}

// Constructs the new object, returns a pointer to it
configurable* createObject(const std::string& objectType)
{
   configurable* object=0;
   if(objectType == "SITEMAP")
   {
      object = new configSiteMap;
      sitemaps.push_back(static_cast<configSiteMap*>(object));
   }
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
   }
}

void makeDoor(int x, int y, int z)
{
   levelmap[x][y][z].flag = SITEBLOCK_DOOR;
   if((x > 0 && (levelmap[x-1][y][z].flag & SITEBLOCK_RESTRICTED)) ||
      (y > 0 && (levelmap[x][y-1][z].flag & SITEBLOCK_RESTRICTED)))
   {
      levelmap[x][y][z].flag |= SITEBLOCK_RESTRICTED;
   }
}

void readMapCBTiles(int x, int y, int z, int i)
{
   switch(i)
   {
   default:
   case 0: levelmap[x][y][z].flag = 0; break;
   case 2: levelmap[x][y][z].flag = SITEBLOCK_BLOCK; break;
   case 3: levelmap[x][y][z].flag = SITEBLOCK_EXIT; break;
   case 4: levelmap[x][y][z].flag = SITEBLOCK_GRASSY; break;
   case 5:
      makeDoor(x,y,z);
      break;
   case 6:
      makeDoor(x,y,z);
      levelmap[x][y][z].flag |= SITEBLOCK_LOCKED;
      break;
   case 7:
      levelmap[x][y][z].flag = SITEBLOCK_RESTRICTED;
      if(x > 0 && (levelmap[x-1][y][z].flag & SITEBLOCK_DOOR))
         levelmap[x-1][y][z].flag |= SITEBLOCK_RESTRICTED;
      if(y > 0 && (levelmap[x][y-1][z].flag & SITEBLOCK_DOOR))
         levelmap[x][y-1][z].flag |= SITEBLOCK_RESTRICTED;
      break;
   case 8:
      levelmap[x][y][z].flag = SITEBLOCK_CHAINLINK;
      break;
   case 9:
      makeDoor(x,y,z);
      levelmap[x][y][z].flag |= (SITEBLOCK_LOCKED | SITEBLOCK_ALARMED);
      break;
   case 10:
      levelmap[x][y][z].flag = (SITEBLOCK_BLOCK | SITEBLOCK_METAL);
      break;
   case 11:
      makeDoor(x,y,z);
      levelmap[x][y][z].flag |= (SITEBLOCK_LOCKED | SITEBLOCK_METAL);
      break;
   }
}

bool readMapFile(const char* filename, const int zLevel, void (*callback)(int,int,int,int))
{
   int x, y, z, i, j;

   // open the file in question
   std::ifstream* file = openFile(filename, ios::in);

   // abort if the file couldn't be opened
   if(!file->is_open())
   {
      delete file;
      return false;
   }

   char line[1024];
   y = 0;
   z = zLevel;
   do
   {
      if(file->eof()) break;
      file->getline(line, 1022);
      x = 0;
      i = 0;
      j = 0;

      do
      {
         
         while(line[j] != ',')
         {
            if(line[j] == 0) break;
            else j++;
         }
         
         if(line[j] != 0) line[j] = 0;
         else break;

         (*callback)(x,y,z,atoi(line+i));
         x++;
         j++;
         i = j;

      } while(true);
      y++;
   } while(true);

   file->close();
   delete file;
   return true;
}

bool readMap(const char* filename)
{
   std::string prefix = std::string("mapCSV_");

   int x,y,z;

   // clear any old map data
   for(x=0; x<MAPX; x++)
   for(y=0; y<MAPY; y++)
   for(z=0; z<MAPZ; z++)
   {
      levelmap[x][y][z].flag = 0;
      levelmap[x][y][z].special = SPECIAL_NONE;
      levelmap[x][y][z].siegeflag = 0;
   }

   // Try first floor (eg "mapCSV_Bank_Tiles.csv"), abort this method if it doesn't exist
   if(!readMapFile((prefix+filename+"_Tiles.csv").c_str(), 0, readMapCBTiles)) return false;
   if(!readMapFile((prefix+filename+"_Specials.csv").c_str(), 0, readMapCBSpecials)) return false;

   // Try upper levels (eg "mapCSV_Bank2_Tiles.csv"), but don't sweat it if they don't exist
   for(z=1; z<MAPZ; z++)
   {
      char str[3];
      itoa(z+1, str, 10);
      if(!readMapFile((prefix+filename+str+"_Tiles.csv").c_str(), z, readMapCBTiles)) break;
      if(!readMapFile((prefix+filename+str+"_Specials.csv").c_str(), z, readMapCBSpecials)) break;
   }

   return true;
}
