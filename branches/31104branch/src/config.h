#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <includes.h>
#include <string>
#include <list>

string getNextSet(FILE *cfile);

enum VarTypes
{
	INTTYPE,
	FLOATTYPE,
	STRINGTYPE,
	BOOLTYPE,
	CHARTYPE,
	VARNUMTYPE
};

class configContainer
{
public:
	void *variable;

	enum VarTypes variableType;

	string name;
};

/*template <class T> class varPointer : public configContainer
{
public:
	T *variable;
};*/

class configurable
{
public:
	void initConfig();
	bool setVariable(string name, string data);

	vector<configContainer> configInfo;
};



#endif