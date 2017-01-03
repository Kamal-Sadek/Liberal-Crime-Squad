/* common - test for possible game over */
char endcheck(char cause = -1);
/* common - tests if the person is a wanted criminal */
bool iscriminal(Creature &cr);
/* common - sends somebody to the hospital */
void hospitalize(int loc, Creature &patient);
/* common - determines how long a creature's injuries will take to heal */
int clinictime(Creature &g);
/* common - applies a crime to everyone in the active party */
void criminalizeparty(short crime);
/* common - applies a crime to everyone in a location, or the entire LCS */
void criminalizepool(short crime, long exclude = -1, short loc = -1);
/* common - applies a crime to a person */
void criminalize(Creature &cr, short crime);
/* common - gives juice to everyone in the active party */
void juiceparty(long juice, long cap);
/* common - gives juice to a given creature */
void addjuice(Creature &cr, long juice, long cap);
/* common - removes the liberal from all squads */
void removesquadinfo(Creature &cr);
/* common - purges empty squads from existance */
void cleangonesquads();
/* common - moves all squad members and their cars to a new location */
void locatesquad(squadst *st, long loc);
/* common - assigns a new base to all members of a squad */
void basesquad(squadst *st, long loc);
/* common - shifts public opinion on an issue */
void change_public_opinion(int v, int power, char affect = 1, char cap = 100);
/* returns the amount of heat associated with a given crime */
int lawflagheat(int lawflag);
/* Determines the number of subordinates a creature may command */
int maxsubordinates(const Creature& cr);
/* Determines the number of subordinates a creature may recruit,
based on their max and the number they already command */
int subordinatesleft(const Creature& cr);
// Determines the number of love slaves a creature has
int loveslaves(const Creature& cr);
/* Determines the number of loveslaves a creature may recruit,
based on their max and the number they already command */
int loveslavesleft(const Creature& cr);
// Determines the number of recruitment meetings a creature has scheduled
int scheduledmeetings(const Creature& cr);
// Determines the number of dates a creature has scheduled
int scheduleddates(const Creature& cr);
/* common - random issue by public interest */
int randomissue(bool core_only = 0);
// Picks a random option, based on the weights provided
int choose_one(const int * weight_list, int number_of_options, int default_value);
// Prompt to convert a new recruit into a sleeper
void sleeperize_prompt(Creature &converted, Creature &recruiter, int y);
/* common - Sort a list of creatures.*/
void sortliberals(std::vector<Creature *>& liberals, short sortingchoice, bool dosortnone = false);
/* common - Functions used when sorting vectors of creatures. */
bool sort_none(const Creature* first, const Creature* second);
inline bool sort_name(const Creature* first, const Creature* second) { return strcmp(first->name, second->name)<0; }
bool sort_locationandname(const Creature* first, const Creature* second);
bool sort_squadorname(const Creature* first, const Creature* second);
/* common - Prompt to decide how to sort liberals.*/
void sorting_prompt(short listforsorting);
/* common - Returns appropriate sortingchoice enum value for a reviewmode enum value.
(Is currently unnecessary unless the enums are changed.)*/
short reviewmodeenum_to_sortingchoiceenum(short reviewmode);
/* common - Display a list of options and return choice. */
int choiceprompt(const string &firstline, const string &secondline,
	const vector<string> &option, const string &optiontypename,
	bool allowexitwochoice, const string &exitstring = "");
/* common - Displays a list of things to buy and returns an int corresponding
to the index of the chosen thing in the nameprice vector. */
int buyprompt(const string &firstline, const string &secondline,
	const vector< pair<string, int> > &nameprice, int namepaddedlength,
	const string &producttype, const string &exitstring);
/* tells how many total members a squad has (including dead members) */
int squadsize(const squadst *st);
/* tells how many members a squad has who are alive */
int squadalive(const squadst *st);