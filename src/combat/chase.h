
bool chasesequence();
bool footchase();
void evasivedrive();
void evasiverun();
int driveskill(Creature &cr, Vehicle &v);
Vehicle* getChaseVehicle(const Creature &c);
Creature* getChaseDriver(const Creature &c);
bool drivingupdate(short &obstacle);
void makechasers(long sitetype, long sitecrime);
bool obstacledrive(short obstacle, char choice);
bool dodgedrive();
void crashfriendlycar(int v);
void crashenemycar(int v);
void chase_giveup();
/* the next two functions force a chase sequence with a specific liberal */
bool footchase(Creature &cr);
bool chasesequence(Creature &cr, Vehicle &v);