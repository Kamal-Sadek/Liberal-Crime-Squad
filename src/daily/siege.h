
/* siege - updates upcoming sieges */
void siegecheck(char canseethings);
/* siege - updates sieges in progress */
void siegeturn(char clearformess);
/* siege - handles giving up */
void giveup();
/* siege - checks how many days of food left at the site */
int fooddaysleft(int loc);
/* siege - checks how many people are eating at the site */
int numbereating(int loc);
/* siege - prepares for massed combat outside the safehouse */
void sally_forth();
/* siege - prepares for entering site mode to fight the siege */
void escape_engage();
/* siege - what happens when you escaped the siege */
void escapesiege(char won);
/* siege - flavor text when you fought off the raid */
void conquertext();
/* siege - flavor text when you crush a CCS safe house */
void conquertextccs();
/* siege - "you are wanted for _______ and other crimes..." */
void statebrokenlaws(int loc);
void statebrokenlaws(Creature &cr);