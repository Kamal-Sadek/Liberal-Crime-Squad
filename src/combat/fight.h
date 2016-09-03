/* attack handling for each side as a whole */
void youattack();
void enemyattack();
/* attack handling for an individual creature and its target */
void attack(Creature &a, Creature &t, char mistake, char &actual, bool force_melee = false);
void specialattack(Creature &a, Creature &t, char &actual);
/* modifies a combat roll based on the creature's critical injuries */
void healthmodroll(int &aroll, Creature &a);
/* adjusts attack damage based on armor, other factors */
void damagemod(Creature &t, char &damtype, int &damamount, char hitlocation, char armorpenetration, int mod, int extraarmor);
/* destroys armor, masks, drops weapons based on severe damage */
void severloot(Creature &cr, vector<Item *> &loot);
/* damages the selected armor if it covers the body part specified */
void armordamage(Armor &armor, int bp, int damamount);
/* blood explosions */
void bloodblast(Armor* armor);
/* kills the specified creature from the encounter, dropping loot */
void delenc(short e, char loot);
/* generates the loot dropped by a creature when it dies */
void makeloot(Creature &cr, vector<Item *> &loot);
/* abandoned liberal is captured by conservatives */
void capturecreature(Creature &t);
/* checks if the creature can fight and prints flavor text if they can't */
char incapacitated(Creature &a, char noncombat, char &printed);
/* describes a character's death */
void adddeathmessage(Creature &cr);
/* pushes people into the current squad (used in a siege) */
void autopromote(int loc);