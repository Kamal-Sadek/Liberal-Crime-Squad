/*******************************************************************************
*
*                                Common Stuff
*                                Folder: "common"
*
*******************************************************************************/
/*
misc.cpp
*/
/* pick a descriptor acronym */
void sexdesc(char *str);
/* what kind of person? */
void sexwho(char *str);
/* seeking acronym */
void sexseek(char *str);
/* what type of sex? */
void sextype(char *str);
/* generate a vanity plate for the squad. */
//void vanity(char *str);
/* return a letter, number, or one of either. */
void chooseLetterOrNumber(char *str, int type);
/* generate a non-vanity plate for the squad. */
void plate(char *str);
/* name of a state (random state, by default) */
const char* statename(int state = -1);
/* endgame - converts an integer into a roman numeral for amendments */
void romannumeral(int amendnum);
/* code for bool Interval::set_interval(const string& interval); is also in misc.cpp */

//#include "common\\help.h"

/*******************************************************************************
*
*                             Title Screen and New Game
*                             Folder: "title"
*
*******************************************************************************/

/*
titlescreen.cpp
*/
void mode_title();

/*
initfile.cpp
*/
/* Handles a init.txt line */
void setconfigoption(std::string name, std::string value);
/* Loads and parses init.txt */
void loadinitfile();

/*
newgame.cpp
*/
/* new game options screen */
void setup_newgame();
/* creates your founder */
void makecharacter();

/*
saveload.cpp
*/
/* saves the game to save.dat */
void savegame(const std::string& filename);
/* loads the game from save.dat */
char load(const std::string& filename);
/* deletes save.dat (used on endgame and for invalid save version) */
void reset();
/* check if file exists */
bool file_exists(const std::string& filename);

/*******************************************************************************
*
*                        The Main Game Screen: Base Mode
*                        Folder: "basemode"
*
*******************************************************************************/


/*
basemode.cpp
*/
void mode_base();

/*
baseactions.cpp
*/
/* base - burn the flag */
void burnflag();
/* base - new slogan */
void getslogan();
/* base - reorder party */
void orderparty();
/* base - go forth to stop evil */
void stopevil();
/* base - invest in this location */
void investlocation();
/* base - assign a vehicle to this squad */
void setvehicles();

/*
liberalagenda.cpp
*/
/* base - liberal agenda */
bool liberalagenda(signed char won);
/* base - liberal agenda - disband */
bool confirmdisband();


/*******************************************************************************
*
*              End of Day Events
*              Folder: "daily"
*
*******************************************************************************/

/*
shopsnstuff.cpp
*/
/* active squad visits the hospital */
void hospital(int loc);
/* active squad visits the pawn shop */
void pawnshop(int loc);
/* active squad visits the car dealership */
void dealership(int loc);
/* active squad visits the arms dealer */
void armsdealer(int loc);
/* active squad visits the department store */
void deptstore(int loc);
/* active squad visits the oubliette */
void halloweenstore(int loc);
/* choose buyer */
void choose_buyer(short &buyer);
