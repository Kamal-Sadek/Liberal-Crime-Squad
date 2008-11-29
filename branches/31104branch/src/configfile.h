#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED



//std::string getNextSet(FILE *cfile);

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
	configContainer(std::string inName, T *inVariable)
	{
		name = inName;
		variable = inVariable;
	}
	void setVariable(std::string data)
	{
		*variable = (T)atoi(data.c_str());
	}
	T *variable;
};

template <> class configContainer<std::string> : public configContainerBase
{
public:
	configContainer(std::string inName, std::string *inVariable)
	{
		name = inName;
		variable = inVariable;
	}
	void setVariable(std::string data)
	{
		*variable = data;
	}
	std::string *variable;
};

template <> class configContainer<float> : public configContainerBase
{
public:
	configContainer(std::string inName, float *inVariable)
	{
		name = inName;
		variable = inVariable;
	}
	void setVariable(std::string data)
	{
		*variable = *variable = (float)atof(data.c_str());
	}
	float *variable;
};

template <> class configContainer<std::vector<int> > : public configContainerBase
{
public:
	configContainer(std::string inName, std::vector<int> *inVariable)
	{
		name = inName;
		variable = inVariable;
	}
	void setVariable(std::string data)
	{
		variable->push_back(atoi(data.c_str()));
	}
	std::vector<int> *variable;
};

class configurable : public managedObject
{
public:
	virtual void initConfig() = 0;
	template <class T> void initVariable(std::string name, T *variable);
	int getVariable(std::string name);
	void configVar(std::string name, std::string data);

	std::vector<configContainerBase*> configInfo;

	//This is to create a new entity of this type.
	//template <class T> T *initializeInstance();

};

template <class T> void configurable::initVariable(std::string name, T *variable)
{
	configContainer<T> *container = new configContainer<T>(name, variable);
	configInfo.push_back(container);
}



#endif
