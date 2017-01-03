/*
File created by Chris Johnson.
These were previously all in game.cpp.

This file includes all the the enums, defines, and included headers required globally by all components.

All code released under GNU GPL.
*/
#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

/*
DEBUG DEFINES
*/
// Don't save the game
//#define NOSAVE

// Enemies don't attack
//#define NOENEMYATTACK

// Laws start archconservative
//#define SHITLAWS

// Laws start elite liberal
//#define PERFECTLAWS

// Public opinion starts at 100% Liberal
//#define REVOLUTIONNOW

// Gives you bloody armor
//#define GIVEBLOODYARMOR

// Start with lots of money
//#define HIGHFUNDS

// Interrogation always enlightens
//#define AUTOENLIGHTEN

// Show polls when you can't see things
//#define SHOWWAIT

// Show die rolls, 100% accurate poll numbers
//#define SHOWMECHANICS

// Interrogation debug, since SHOWMECHANICS is too verbose
//#define INTERROGATION_DEBUG

// Make the year 2100
//#define THEFUTURE

// Make the founder blind
//#define BLIND

// Make the founder unable to walk
//#define NOWALK

// Make the founder have no face
//#define NOFACE

// Make the founder have a severely injured spine
//#define SPINE

// Make the founder have severe internal damage
//#define INTERNAL

// Make age not matter for dating or prostitution
//#define ZEROMORAL

// Re-seed the Random Number Generator every time it's called
//#define MORERANDOM

// Allow experimental, incomplete Stalinist Comrade Squad mode to be chosen for new games
#define ALLOWSTALIN

//PACKAGE_VERSION must be defined here or the game won't compile on Windows! Don't remove it!!!
// -- yetisyny
#define PACKAGE_VERSION "4.10.0"

const int version = 40100;
const int lowestloadversion = 40100;
const int lowestloadscoreversion = 31203;

#include "common.h" /* include this prior to checking if WIN32 is defined */

#include "lcsio.h"
#include "compat.h"
#include "cursesmovie.h"
#include "cursesgraphics.h"
#include <includePolitics.h>

#ifndef NDEBUG
#define NDEBUG
#endif

#define TAB 9
#define ENTER 10
#define ESC 27
#define SPACEBAR 32

class Log;


#define ARMORFLAG_DAMAGED BIT1
#define ARMORFLAG_BLOODY BIT2



/*******************************************************************************
*
*                             Logging Stuff
*                             Folder: "log"
*
*******************************************************************************/

//TODO: Make NEWLINEMODE_LOGFILES_DEFAULT, NEWLINEMODE_GAMELOG, and OVERWRITE_GAMELOG set by the cfg.
//Whether or not it should autonewline logfiles by default.
#define NEWLINEMODE_LOGFILES_DEFAULT 1
//Whether or not it should autonewline the gamelog.
#define NEWLINEMODE_GAMELOG 2
//Whether or not it should overwrite the gamelog every time the game starts.
#define OVERWRITE_GAMELOG false

//The filepath of the gamelog.
//TODO: Make this be set via the cfg.
#define GAMELOG_FILEPATH "gamelog.txt"

#include "log/log.h"

#include <includeEnum2.h>
#include <includeEnum3.h>


#include <includeItems.h>
#include <includeCreatures.h>
#include "vehicle/vehicletype.h"
#include "vehicle/vehicle.h"
#include "locations/locations.h"
#include "configfile.h"
#include "sitemode/sitemap.h"

#endif // INCLUDES_H_INCLUDED
