#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

// OBJECTIVE: SUPPORT TAB-SEPARATED CONFIGURATION FILES
//   TO MAKE LCS CONTENT HIGHLY MODDABLE AND DATA-DRIVEN,
//   INCLUDING EQUIPMENT, LOCATIONS, MAPS, CREATURES,
//   AND ANY ADDITIONAL SYSTEMS THAT WOULD BENEFIT FROM
//   THIS APPROACH
// 
// CONFIG FILE FORMAT
//
// (newline) (leading whitespace) COMMAND (delimiting whitespace) VALUE
//
// Initial tests for the tab-separated configuration files
// will be a full-scale implementation of sitemap creation
// using the sitemaps.txt config file. From there, support
// can be extended to additional systems.
//
// TODO:
// Sitemaps Short-Term Roadmap (Feb 12, 2009):
// [x] convert names of tiles, scripts, specials, uniques, and
//     loots to game data in their respective configSite___ objects
// [x] sitemap construction logic, to build the actual map in
//     the game when player visits a site, in various configSite___
//     objects (this is a new method, say, build() for example),
//     with the exception of the configSiteLoot, which will remain
//     no-op until later (the data structure and logic of loot
//     handling will need to be revised to support dynamic loot)
// [x] move definitions to headers files, include correct headers,
//     etc. (sitemap stuff should probably go in its own file,
//     perhaps where the existing map building logic is now)
// [x] global sitemaps vector containing configSiteMap objects
// [x] global build_site(string) function that calls .build() on
//     the sitemap with the specified name
// [x] add all additional defines and enumerations assumed in the
//     code
// [x] link these to the actual game
//   [x] call the configSiteMap construction logic instead of the
//       existing map creation code when visiting a site
//   [x] for now, just hard-code the sitemap names into the site
//       data, parameterization will come when site data is moved
//       to config files
//   [x] load in sitemaps.txt on game start
// [x] BENCHMARK: SUCCESSFUL COMPILE
// [x] verify names are correct in sitemaps.txt
// [x] ensure all maps are supported in the sitemaps.txt configuration
//     file
// [x] test game extensively and fix any remaining problems
//   [x] resolve off by 1 errors if they occur
// [x] clean deprecated map generation code
// [x] BENCHMARK: TERRAIN AND SPECIALS CONFIGURABLE AND ACTIVE IN GAME
// [ ] SVN commit of all new files and changes
// [ ] change loot data storage to static item storage on map tiles
// [ ] propogate changes to dropped loot in combat and all other
//     areas where stuff exists on the floor in the game world
// [ ] implement .build() method for configSiteLoot
// [ ] ensure loot data is transferred to config file
// [ ] clean deprecated loot generation code
// [ ] any additional changes required to support new loot system
// [ ] BENCHMARK: LOOT CHANGES COMPLETED, SITEMAPS FULLY CONFIGURABLE
//
//
// Long-Term Roadmap to Configuration File Bliss:
//
// [ ] Sitemaps configuration
// [ ] Site data configuration (prereq for National LCS)
// [ ] Weapons configuration
// [ ] Clothing configuration
// [ ] Creature type configuration
// [ ] Organizations configuration? (are we still doing organizations?)
// ... and more?
//
// ~ Jonathan S. Fox

#include "includes.h"
#include <string>
#include <fstream>

// - configurable is a base class object for anything
//   that is implemented in the config file
// - configurable objects implement a configure() method
//   that takes two strings, a command and a value
class configurable
{
public:
   void virtual configure(const std::string& command, const std::string& value) = 0;
private:
};

// Reads in an entire configuration file
// Returns 0 for read successful, returns -1 if failed read
int readConfigFile(char* filename);
// readLine reads a line from the file, parses it
int readLine(std::fstream& file, std::string& command, std::string& value);
// Constructs the new object, returns a pointer to it
configurable* createObject(const std::string& objectType);

#endif
