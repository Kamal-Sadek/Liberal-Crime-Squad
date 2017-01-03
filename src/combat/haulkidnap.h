
/* prompt after you've said you want to kidnap someone */
void kidnapattempt();
/* prompt after you've said you want to release someone */
void releasehostage();
/* roll on the kidnap attempt and show the results */
bool kidnap(Creature &a, Creature &t, bool &amateur);
/* hostage freed due to host unable to haul */
void freehostage(Creature &cr, char situation);
/* haul dead/paralyzed */
void squadgrab_immobile(char dead);
/* names the new hostage and stashes them in your base */
void kidnaptransfer(Creature &cr);