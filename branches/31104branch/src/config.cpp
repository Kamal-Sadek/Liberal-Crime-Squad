

#include "config.h"

string getNextSet(FILE *cfile)
{
	char input = ' ';
	string name;
	while(input != '[' && input != EOF)
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
		return NULL;
	}
}

void configurable::initConfig()
{
}

bool configurable::setVariable(string name, string data)
{
	int i = 0;
	int size = configInfo.size();
	while(i < size && configInfo[i].name != name)
	{
		i++;
	}
	if(i >= size)
	{
		return false;
	}
	else
	{
		switch(configInfo[i].variableType)
		{
			case INTTYPE:
				*(int*)configInfo[i].variable = atoi(data.c_str());
				break;
			case FLOATTYPE:
				*(float*)configInfo[i].variable = atof(data.c_str());
				break;
			case STRINGTYPE:
				*(string*)configInfo[i].variable = data;
				break;
			case BOOLTYPE:
				*(bool*)configInfo[i].variable = (bool)atoi(data.c_str());
				break;
			case CHARTYPE:
				*(char*)configInfo[i].variable = (char)atoi(data.c_str());
				break;
			default:
				break;
		}
	}
	return false;
}