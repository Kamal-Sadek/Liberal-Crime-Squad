
/* adjust blog power */
void adjustblogpower(int &power);
/* hostage tending */
void tendhostage(Creature *cr, char &clearformess);
/* armor repair */
void repairarmor(Creature &cr, char &clearformess);
/* armor manufacture */
void makearmor(Creature &cr, char &clearformess);
/* search for polls */
void survey(Creature *cr);
/* misc activation related things */
void funds_and_trouble(char &clearformess);
/* steal a car */
bool stealcar(Creature &cr, char &clearformess);
bool carselect(Creature &cr, short &cartype);
/* get a wheelchair */
void getwheelchair(Creature &cr, char &clearformess);
void augment(Creature &cr, char &clearformess);