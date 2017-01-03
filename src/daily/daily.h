
void advanceday(char &clearformess, char canseethings);
/* squad members with no chain of command lose contact */
void dispersalcheck(char &clearformess);
/* promote a subordinate to maintain chain of command when boss is lost */
bool promotesubordinates(Creature &cr, char &clearformess);
/* daily - manages too hot timer and when a site map should be re-seeded and renamed */
void advancelocations();
/* daily - returns true if the site type supports high security */
char securityable(int type);
/* daily - seeds and names a site (will re-seed and rename if used after start) */
void initlocation(Location &loc);
/* daily - returns the number of days in the current month */
int monthday();