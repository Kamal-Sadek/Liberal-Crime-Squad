#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <includes.h>
#include <string>
#include <list>
#include <vector>

std::string getNextSet(FILE *cfile);

class configContainerBase
{
public:
	//Hooray.
	virtual void setVariable(std::string data) = 0;

	std::string name;
};

template <class T> class configContainer : public configContainerBase
{
public:
	configContainer(std::string inName, T *inVariable);
	void setVariable(std::string data);
	T *variable;
};

template <class T> configContainer<T>::configContainer(std::string inName, T *inVariable)
{
	name = inName;
	variable = inVariable;
}

template <> void configContainer<int>::setVariable(std::string data)
{
	*variable = (int)atoi(data.c_str());
}

template <> void configContainer<short>::setVariable(std::string data)
{
	*variable = (short)atoi(data.c_str());
}

template <> void configContainer<char>::setVariable(std::string data)
{
	*variable = (char)atoi(data.c_str());
}

template <> void configContainer<bool>::setVariable(std::string data)
{
	*variable = (bool)atoi(data.c_str());
}

template <> void configContainer<float>::setVariable(std::string data)
{
	*variable = (float)atof(data.c_str());
}

template <> void configContainer<std::string>::setVariable(std::string data)
{
	*variable = data;
}

template <> void configContainer<std::vector<int>>::setVariable(std::string data)
{
	variable->push_back(atoi(data.c_str()));
}




class configurable
{
public:
	void initConfig();
	int getVariable(std::string name);
	void configVar(std::string name, std::string data);

	std::vector<configContainerBase*> configInfo;
};



#endif
