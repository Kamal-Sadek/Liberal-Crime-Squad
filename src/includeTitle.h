
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