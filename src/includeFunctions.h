/*******************************************************************************
*
*                             Fighting Evil: Site Mode
*                             Folder: "sitemode"
*
*******************************************************************************/
/*
sitedisplay.cpp
*/
/* prints the 'map graphics' on the bottom right */
void printsitemap(int x, int y, int z);
void printblock(int x, int y, int z, int px, int py);
/* prints the names of creatures you see */
void printencounter();
/* prints the names of creatures you see in car chases */
void printchaseencounter();
/* blanks a part of the screen */
void clearcommandarea();
void clearmessagearea(bool redrawmaparea = true);
void clearmaparea(bool lower = true, bool upper = true);

/*
miscactions.cpp
*/
/* unlock attempt */
char unlock(short type, char &actual);
/* bash attempt */
char bash(short type, char &actual);
/* computer hack attempt */
char hack(short type, char &actual);
/* run a radio broadcast */
char radio_broadcast();
/* run a tv broadcast */
char news_broadcast();
/* rescues people held at the activeparty's current location */
void partyrescue(short special);
/* everybody reload! */
void reloadparty(bool wasteful = false);

/*
mapspecials.cpp
*/
void special_bouncer_assess_squad();
void special_bouncer_greet_squad();
void special_lab_cosmetics_cagedanimals();
void special_readsign(int sign);
void special_nuclear_onoff();
void special_lab_genetic_cagedanimals();
void special_policestation_lockup();
void special_courthouse_lockup();
void special_courthouse_jury();
void special_prison_control(short prison_control_type);
void special_intel_supercomputer();
void special_sweatshop_equipment();
void special_polluter_equipment();
void special_house_photos();
void special_corporate_files();
void special_radio_broadcaststudio();
void special_news_broadcaststudio();
void special_graffiti();
void special_armory();
void special_display_case();
void special_security_checkpoint();
void special_security_metaldetectors();
void special_security_secondvisit();
void special_bank_teller();
void special_bank_money();
void special_bank_vault();
void special_ccs_boss();
void special_oval_office();
/*******************************************************************************
*
*                             Politics Stuff
*                             Folder: "politics"
*
*******************************************************************************/
/*
politics.cpp
*/
/* politics - calculate presidential approval */
int presidentapproval();
/* politics -- gets the leaning of an issue voter for an election */
int getswingvoter(bool stalin);
/* politics -- gets the leaning of a partyline voter for an election */
int getsimplevoter(int leaning);
/* politics -- promotes the Vice President to President, and replaces VP */
void promoteVP();
/* politics -- appoints a figure to an executive office, based on the President's alignment */
void fillCabinetPost(int position);
/* politics - causes the people to vote (presidential, congressional, propositions) */
void elections(char clearformess, char canseethings);
void elections_senate(int senmod, char canseethings);
void elections_house(char canseethings);
/* politics - causes the supreme court to hand down decisions */
void supremecourt(char clearformess, char canseethings);
/* politics - causes congress to act on legislation */
void congress(char clearformess, char canseethings);
// letter of amnesty to the LCS from the President (you win)
void amnesty();
/* politics - checks if the game is won */
char wincheck();
/* politics - checks the prevailing attitude on a specific law, or overall */
int publicmood(int l);
/* returns true if Stalinists agree with Elite Liberals on a view/law, false if they strongly disagree with libs  *
* the input bool islaw, if true, returns Stalinist opinion on laws, if false, returns Stalinist opinion on views */
bool stalinview(short view, bool islaw);
/*******************************************************************************
*
*                             End of Month Events
*                             Folder: "monthly"
*
*******************************************************************************/

/*
endgame.cpp
*/
/* endgame - attempts to pass a constitutional amendment to help win the game */
void tossjustices(char canseethings);
/* endgame - attempts to pass a constitutional amendment to help win the game */
void amendment_termlimits(char canseethings);
/* endgame - attempts to pass a constitutional amendment to lose the game */
void reaganify(char canseethings);
/* endgame - attempts to pass a constitutional amendment to lose the game */
void stalinize(char canseethings);
/* endgame - checks if a constitutional amendment is ratified */
char ratify(int level, int view, int lawview, char congress, char canseethings);
/* endgame - header for announcing constitutional amendments */
void amendmentheading();
