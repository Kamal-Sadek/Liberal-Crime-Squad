
/* generates a new random encounter */
void prepareencounter(short type, char sec);
/* generates a new siege encounter */
char addsiegeencounter(char type);
/* rolls up a random creature type according to the passed weighting array */
int getrandomcreaturetype(int cr[CREATURENUM]);