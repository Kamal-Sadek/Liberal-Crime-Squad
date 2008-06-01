

#include <includes.h>
#include <string>
#include <list>
#include <vector>
#include "configfile.h"

/*string getNextSet(FILE *cfile)
{
	if(cfile == NULL)
	{
		return "\0";
	}
	char input = ' ';
	string name;
	while(input !=  && input != EOF)
	{
		input = fgetc(cfile);
	}
	if(input == '[')
	{
		//Okay, we got the next config thing.
		fscanf(cfile, "%s:", name);
		return name;
	}
	else
	{
		return "\0";
	}
}*/

int configurable::getVariable(std::string name)
{
	int i = 0;
	int size = configInfo.size();
	while(i < size && configInfo[i]->name != name)
	{
		i++;
	}
	if(i >= size)
	{
		return -1;
	}
	else
	{
		return i;
	}
}

void configurable::configVar(string name, string data)
{
	int var = getVariable(name);
	if(var != -1)
	{
		configInfo[var]->setVariable(data);
	}
}