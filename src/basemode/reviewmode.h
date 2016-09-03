/* base - review and reorganize liberals */
void review();
void review_mode(short mode);
/* base - review - assemble a squad */
void assemblesquad(squadst *cursquad);
/* base - review - assign new bases to the squadless */
void squadlessbaseassign();
/* base - review - promote liberals */
void promoteliberals();
void sortbyhire(vector<Creature *> &temppool, vector<int> &level);