
// Sets the text color to the thematic color for the given alignment
// extended_range forces colors to be set on a 5 point scale instead
// of just basic liberal-moderate-conservative
void set_alignment_color(Alignment alignment, bool extended_range = false);
/* Sets the text color per activity type */
void set_activity_color(long activity_type);
/* location and squad header */
void locheader();
/* party info at top of screen */
void printparty();
/* location info at top of screen */
void printlocation(long loc);
/* character info at top of screen */
void printcreatureinfo(Creature *cr, unsigned char knowledge = 255);
/* full character sheet (with surrounding interface) */
void fullstatus(int p);
/* full screen character sheet, just skills */
void printliberalskills(Creature &cr);
/* full screen character sheet */
void printliberalstats(Creature &cr);
/* Full screen character sheet, crime sheet */
void printliberalcrimes(Creature &cr);
/* draws a horizontal line across the screen */
void makedelimiter(int y = 8, int x = 0);
/* prints a character's general health description (One Leg, Liberal, NearDETH...) */
void printhealthstat(Creature &g, int y, int x, char smll);
/* prints a character's health description for each bodypart (Head, Body...) */
void printwoundstat(Creature &cr, int y, int x);
/* prints amount of money the player has, with optional formatting */
void printfunds(int y = 0, int offset = 1, const char* prefix = "Money: ");
/* prints a short blurb showing how to page forward */
void addnextpagestr();
/* prints a short blurb showing how to page back */
void addprevpagestr();
/* prints a long blurb showing how to page forward and back */
void addpagestr();
/* Variants of addch and mvaddch that work on chars and use translateGraphicsChar(), fixing display of extended characters */
inline int addchar(char ch) { return addch(translateGraphicsChar(ch)); }
inline int mvaddchar(int y, int x, char ch) { return mvaddch(y, x, translateGraphicsChar(ch)); }
inline int addchar(char ch, Log &log) { log.record(ch); return addchar(ch); }
inline int mvaddchar(int y, int x, char ch, Log &log) { log.record(ch); return mvaddchar(y, x, ch); }
/* Redefining addstr() and mvaddstr() so they use addchar() and mvaddchar(), fixing display of extended characters */
#undef addstr
inline int addstr(const char* text) { int ret = ERR; for (int i = 0; i<len(text); i++) ret = addchar(text[i]); return ret; }
#undef mvaddstr
inline int mvaddstr(int y, int x, const char* text) { int ret = move(y, x); if (ret != ERR) ret = addstr(text); return ret; }
/* Various wrappers to addstr() and mvaddstr() which handle permutations of:
- Including or not including the gamelog for external message logging
- std::string or c-style char arrays */
inline int addstr(const char *text, Log &log) { log.record(text); return addstr(text); }
inline int mvaddstr(int y, int x, const char *text, Log &log) { log.record(text); return mvaddstr(y, x, text); }
inline int addstr(const std::string& text) { int ret = ERR; for (int i = 0; i<len(text); i++) ret = addchar(text[i]); return ret; }
inline int addstr(const std::string& text, Log &log) { log.record(text); return addstr(text); }
inline int mvaddstr(int y, int x, const std::string& text) { int ret = move(y, x); if (ret != ERR) ret = addstr(text); return ret; }
inline int mvaddstr(int y, int x, const std::string& text, Log &log) { log.record(text); return mvaddstr(y, x, text); }
/* These wrappers convert numbers to text */
inline int addstr(long num) { return addstr(tostring(num)); }
inline int addstr(long num, Log &log) { return addstr(tostring(num), log); }
inline int mvaddstr(int y, int x, long num) { return mvaddstr(y, x, tostring(num)); }
inline int mvaddstr(int y, int x, long num, Log &log) { return mvaddstr(y, x, tostring(num), log); }
/* addstr with formatted output */
int addstr_f(const char * format, ...);
/* mvaddstr with formatted output */
int mvaddstr_f(int y, int x, const char * format, ...);
/* addstr with formatted output and logging */
int addstr_fl(Log &log, const char * format, ...);
/* mvaddstr with formatted output and logging */
int mvaddstr_fl(int y, int x, Log &log, const char * format, ...);