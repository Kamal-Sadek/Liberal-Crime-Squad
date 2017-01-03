
void mode_site(short loc);
void mode_site();
/* site - determines spin on site news story, "too hot" timer */
void resolvesite();
/* behavior when the player bumps into a door in sitemode */
void open_door(bool restricted);
/* re-create site from seed before squad arrives */
void initsite(Location &loc);
/* recursive dungeon-generating algorithm */
void generateroom(int rx, int ry, int dx, int dy, int z = 0);
/* marks the area around the specified tile as explored */
void knowmap(int locx, int locy, int locz);