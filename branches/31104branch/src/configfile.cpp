
#include "configfile.h"
#include "sitemode/sitemap.h"
#include "externs.h"

// Reads in an entire configuration file
// Returns 0 for read successful, returns -1 if failed read
int readConfigFile(char* filename)
{
   std::fstream file;
   std::string command, value;
   configurable* object = 0;

   file.open(filename, ios::in);
   if(!file.is_open()) return -1; // Unable to open; failed read
   // loop through lines
   while(readLine(file, command, value))
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
            file.close();
            return -1; // Unknown command and no object to give it to; failed read
         }
      }
   }
   file.close();
   return 0;
}


// readLine reads a line from the file, parses it
int readLine(std::fstream& file, std::string& command, std::string& value)
{
   std::string line;
   unsigned int source=0;

   // Search for a non-comment, non-empty line
   do
   {
      getline(file,line);
      if(file.eof()) return 0;
   } while(line[0] == '#' || line[0] == 0);

   // Parse the line
   command.clear();
   value.clear();

   // Leading whitespace
   while(source<line.length() && (line[source]==' ' || line[source]=='\t'))
      source++;

   // Command
   while(source<line.length() && (line[source]!=' ' && line[source]!='\t'))
      command.push_back(line[source++]);

   // Delimiting whitespace
   while(source<line.length() && (line[source]==' ' || line[source]=='\t'))
      source++;

   // Value
   while(source<line.length() && (line[source]!=' ' && line[source]!='\t'))
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
