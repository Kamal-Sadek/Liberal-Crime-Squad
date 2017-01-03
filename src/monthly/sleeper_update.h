
/* monthly - sleeper behavior */
void sleepereffect(Creature &cr, char &clearformess, char canseethings, int(&libpower)[VIEWNUM]);
/* assistant functions for specific sleeper tasks */
void sleeper_recruit(Creature &cr, char &clearformess, char canseethings, int(&libpower)[VIEWNUM]);
void sleeper_influence(Creature &cr, char &clearformess, char canseethings, int(&libpower)[VIEWNUM]);
void sleeper_spy(Creature &cr, char &clearformess, char canseethings, int(&libpower)[VIEWNUM]);
void sleeper_scandal(Creature &cr, char &clearformess, char canseethings, int(&libpower)[VIEWNUM]);
void sleeper_embezzle(Creature &cr, char &clearformess, char canseethings, int(&libpower)[VIEWNUM]);
void sleeper_steal(Creature &cr, char &clearformess, char canseethings, int(&libpower)[VIEWNUM]);