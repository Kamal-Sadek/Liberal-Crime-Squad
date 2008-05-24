
#include "organizationdef.h"


void organizationDef::initConfig()
{
	initVariable<char>("ALIGNMENT", &alignment);
	initVariable<std::string>("NAME", &name);

	initVariable<short>("ATTACKPOWER", &attackPower);
	initVariable<std::vector<int>>("SOLDIERS", &soldiers);

	initVariable<short>("LOBBYPOWER", &lobbyPower);
	initVariable<std::vector<int>>("LOBBYISTS", &lobbyists);

	initVariable<short>("PUBLICITYPOWER", &publicityPower);
	initVariable<std::vector<int>>("PUBLICISTS", &publicists);

	initVariable<std::vector<int>>("INTERESTS", &specialInterests);
}

void organizationDef::initializeInstance(organization &instance)
{
	instance.alignment = alignment;
	instance.attackPower = attackPower;
	instance.name = name;
	instance.attackPower = attackPower;
	instance.soldiers = soldiers;
	instance.publicityPower = publicityPower;
	instance.publicists = publicists;
	instance.lobbyPower = lobbyPower;
	instance.lobbyists = lobbyists;
	instance.specialInterests = specialInterests;
}