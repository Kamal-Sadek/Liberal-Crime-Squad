
/*******************************************************************************
*
*                                Common Stuff
*                                Folder: "common"
*
*******************************************************************************/

#include <includeCommonCommon.h>
/*
creature.cpp
*/
#include "common\\creature.h"


/*******************************************************************************
*
*                             Title Screen and New Game
*                             Folder: "title"
*
*******************************************************************************/

/*
highscore.cpp
*/
#include "title\\highscore.h"



/*******************************************************************************
*
*                        The Main Game Screen: Base Mode
*                        Folder: "basemode"
*
*******************************************************************************/


/*
activate_sleepers.cpp
*/
#include "basemode\\activate_sleepers.h"

/*
activate.cpp
*/
#include "basemode\\activate.h"

/*
reviewmode.cpp
*/
#include "basemode\\reviewmode.h"

/*******************************************************************************
*
*                             Fighting Evil: Site Mode
*                             Folder: "sitemode"
*
*******************************************************************************/

/*
sitemode.cpp
*/
#include "sitemode\\sitemode.h"

/*
newencounter.cpp
*/
#include "sitemode\\newencounter.h"

/*
talk.cpp
*/
/* bluff, date, issues */
char talk(Creature &a, int t);

/*
stealth.cpp
*/
#include "sitemode\\stealth.h"

/*
advance.cpp
*/
/* handles end of round stuff for everyone */
void creatureadvance();
/* handles end of round stuff for one creature */
void advancecreature(Creature &cr);

/*******************************************************************************
*
*                        Combat and Chase Sequences
*                        Folder: "combat"
*
*******************************************************************************/

/*
fight.cpp
*/
#include "combat\\fight.h"


/*
chase.cpp
*/
#include "combat\\chase.h"

/*
haulkidnap.cpp
*/
#include "combat\\haulkidnap.h"

/*******************************************************************************
*
*              End of Day Events
*              Folder: "daily"
*
*******************************************************************************/

/*
daily.cpp
*/
#include "daily\\daily.h"

/*
activities.cpp
*/
#include "daily\\activities.h"

/*
date.cpp
*/
/* daily - date - dater p gets back from vacation */
char completevacation(datest &d, int p, char &clearformess);
/* daily - date - dater p goes on some dates */
char completedate(datest &d, int p, char &clearformess);

/*
recruit.cpp
*/
/* automatic finding recruits from the activity screen */
char recruitment_activity(Creature &cr, char &clearformess);
/* daily - recruit - recruit meeting*/
char completerecruitmeeting(recruitst &d, int p, char &clearformess);

/*
siege.cpp
*/
#include "daily\\siege.h"

/*******************************************************************************
*
*                             End of Day News Stories
*                             Folder: "news"
*
*******************************************************************************/
/* news - determines the priority of a news story */
void setpriority(newsstoryst &ns);
/* news - show major news story */
void displaystory(newsstoryst &ns, bool liberalguardian, int header);
/* news - graphics */
void loadgraphics();
void displaycenterednewsfont(const std::string& str, int y);
void displaycenteredsmallnews(const std::string& str, int y);
void displaynewspicture(int p, int y);
/* news - constructs non-LCS related event stories */
void constructeventstory(char *story, short view, char positive);
/* news - draws the specified block of text to the screen */
void displaynewsstory(char *story, short *storyx_s, short *storyx_e, int y);
/* news - shows animated news stories */
void run_television_news_stories();
/* news - make some filler junk */
void generatefiller(char *story, int amount);
/* news - major newspaper reporting on lcs and other topics */
void majornewspaper(char &clearformess, char canseethings);

/*******************************************************************************
*
*                             End of Month Events
*                             Folder: "monthly"
*
*******************************************************************************/

/*
monthly.cpp
*/
#include "monthly\\monthly.h"

/*
lcsmonthly.cpp
*/
#include "monthly\\lcsmonthly.h"

/*
sleeper_update.cpp
*/
#include "monthly\\sleeper_update.h"

/*
justice.cpp
*/
#include "monthly\\justice.h"
/*
TempFunctions
Ideally, all of these should be made obsolete
*/
#include <includeTempFunctions.h>