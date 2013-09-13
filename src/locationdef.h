#ifndef LOCATIONDEF_H_INCLUDED
#define LOCATIONDEF_H_INCLUDED

#include "configfile.h"

class locationDef : public configurable
{
	void initConfig();
	void initializeInstance(location &instance, int orgID = -1);
};

#endif