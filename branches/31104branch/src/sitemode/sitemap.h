#ifndef SITEMAP_H_INCLUDED
#define SITEMAP_H_INCLUDED


#include "configfile.h"

#define SITEMAP_ADDTYPE_OR     1
#define SITEMAP_ADDTYPE_ANDNOT 2

enum sitemapscripts
{
   SITEMAPSCRIPT_ROOM,
   SITEMAPSCRIPT_HALLWAY_YAXIS,
   SITEMAPSCRIPT_STAIRS
};

void build_site(std::string name);

// configSiteCommand is anything the configSiteMap stores in its array of stuff to build
class configSiteCommand : public configurable
{
public:
   void virtual build() = 0;
private:
};

// configSiteMap derives from configurable, is a sitemap
class configSiteMap : public configurable
{
public:
   configSiteMap::~configSiteMap();
   void configure(const std::string& command, const std::string& value);
   void build();
   bool operator==(const std::string& rhs) { return name == rhs; }
private:
   std::string name;
   std::string parent;
   std::vector<configSiteCommand *> commands;
   configSiteCommand* current_command;
};

// Paints tiles during map creation
class configSiteTile : public configSiteCommand
{
public:
   configSiteTile(const std::string& value);
   void configure(const std::string& command, const std::string& value);
   void build();
private:
   char xstart, xend, ystart, yend, zstart, zend;
   int tile;
   char addtype;
};

// Executes a complex set of code during map creation
class configSiteScript : public configSiteCommand
{
public:
   configSiteScript(const std::string& value);
   void configure(const std::string& command, const std::string& value);
   void build();
private:
   char xstart, xend, ystart, yend, zstart, zend;
   char script;

   /* recursive dungeon-generating algorithm */
   void generateroom(int rx,int ry,int dx,int dy,int z);
   /* generates a hallway with rooms on either side */
   void generatehallway_y(int rx, int ry, int dx, int dy, int z);
   /* generates a stairwell */
   void generatestairs(int rx, int ry, int rz, int dx, int dy, int dz);
};

// Drops specials down during map creation
class configSiteSpecial : public configSiteCommand
{
public:
   configSiteSpecial(const std::string& value);
   void configure(const std::string& command, const std::string& value);
   void build();
private:
   char xstart, xend, ystart, yend, zstart, zend;
   char special;
   int freq;
};

// Creates a unique during map creation
class configSiteUnique : public configSiteCommand
{
public:
   configSiteUnique(const std::string& value);
   void configure(const std::string& command, const std::string& value);
   void build();
private:
   char xstart, xend, ystart, yend, zstart, zend;
   char unique;
};

// Adds a loot type during map creation
class configSiteLoot : public configSiteCommand
{
public:
   configSiteLoot(const std::string& value);
   void configure(const std::string& command, const std::string& value);
   void build();
private:
   char loot;
   int weight;
};

#endif