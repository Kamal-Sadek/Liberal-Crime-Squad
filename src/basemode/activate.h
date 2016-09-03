/* base - activate the uninvolved */
void activate();
void activate(Creature *cr);
void activatebulk();
/* base - activate - hostages */
void select_tendhostage(Creature *cr);
long select_hostagefundinglevel(Creature *cr, Creature *hs);
/* activation-based recruitment */
int recruitFindDifficulty(int creatureType);
char* recruitName(int creatureType);
void recruitSelect(Creature &cr);
/* base - activate - make clothing */
void select_makeclothing(Creature *cr);
/* base - activate - augmentation */
void select_augmentation(Creature *cr);
int armor_makedifficulty(Armor& type, Creature *cr); //Replace with Armor class method? -XML
int armor_makedifficulty(ArmorType& type, Creature *cr); //Replace with Armor class method? -XML
														 /* base - activate - trouble */
long select_troublefundinglevel(Creature *cr);
/* base - activate - select a topic to write about */
char select_view(Creature *cr, int &v);