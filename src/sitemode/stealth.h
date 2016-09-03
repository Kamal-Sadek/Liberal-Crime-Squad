
/* checks if your liberal activity is noticed */
void noticecheck(int exclude, int difficulty = DIFFICULTY_EASY);
/* checks if your liberal behavior/attack alienates anyone */
char alienationcheck(char mistake);
/* checks if conservatives see through your disguise */
void disguisecheck(int encounter_timer);
/* checks if a creature's weapon is suspicious or illegal */
char weaponcheck(const Creature &cr, bool metaldetect = false);
/* checks if a creature's uniform is appropriate to the location */
char hasdisguise(const Creature &cr);
/* returns true if the entire site is not open to public */
char disguisesite(long type);