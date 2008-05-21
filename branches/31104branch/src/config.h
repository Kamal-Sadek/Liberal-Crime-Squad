#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <includes.h>
#include <string>
#include <list>

void getNextSet();


class configContainer
{
	string name;

	bool isNumeric;
};

template <class T> class varPointer : public configContainer
{
	T *variable;
};

class configurable
{
	
	vector<configContainer> configInfo;
};



#endif