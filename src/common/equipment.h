/* prompt user to enter an amount of items to equip, move, or sell */
long prompt_amount(long min, long max);
/* review squad equipment */
void equip(vector<Item *> &loot, int loc);
/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<Item *> &dest, vector<Item *> &source);
/* equipment - assign new bases to the equipment */
void equipmentbaseassign();
/* combines multiple items of the same type into stacks */
void consolidateloot(vector<Item *> &loot);
/* compares two items, used in sorting gear */
char itemcompare(Item *a, Item *b);
/* check if the squad has a certain weapon */
char squadhasitem(squadst &sq, int type, int subtype);