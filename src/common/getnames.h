std::string getactivity(activityst &act);
std::string gettitle(Creature &cr);
std::string getview(short view, bool shortname);
std::string getlaw(int l);
std::string cityname(); /* random city name */
						/* Allow player to enter a name with an optional default name */
void enter_name(int y, int x, char *name, int len, const char *defname = NULL);
std::string getlawflag(int type);
std::string getmonth(int month, bool shortname = false);
std::string getalign(Alignment alignment, bool capitalize = false);