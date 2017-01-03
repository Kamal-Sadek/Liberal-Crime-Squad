/* transforms a squad id number into the index of that squad in the global vector */
int getsquad(int id);
/* transforms a car id number into the index of that car in the global vector */
int id_getcar(int id);
/* transforms a creature id number into the index of that person in the pool */
int getpoolcreature(int id);
/* transforms a vehicle type id into the index of that vehicle type in the global vector */
int getvehicletype(int id);
/* transforms a vehicle type idname into the index of that vehicle type in the global vector */
int getvehicletype(const string &idname);
/* transforms a clip type id into the index of that clip type in the global vector */
int getcliptype(int id);
/* transforms a clip type name into the index of that clip type in the global vector */
int getcliptype(const string &idname);
/* transforms a weapon type id into the index of that weapon type in the global vector */
int getweapontype(int id);
/* transforms a weapon type name into the index of that weapon type in the global vector */
int getweapontype(const string &idname);
/* transforms a armor type id into the index of that armor type in the global vector */
int getarmortype(int id);
/* transforms a armor type name into the index of that armor type in the global vector */
int getarmortype(const string &idname);
/* transforms a loot type id into the index of that loot type in the global vector */
int getloottype(int id);
/* transforms a loot type name into the index of that loot type in the global vector */
int getloottype(const string &idname);
/* transforms a CreatureTypes value into a pointer to that creature type */
const CreatureType* getcreaturetype(short crtype);
/* transforms a creature type name into a pointer to that creature type */
const CreatureType* getcreaturetype(const std::string& crtype);