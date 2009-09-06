
#include <map>
#include <string>
#include "location_test.h"
#include "creature_test.h"
#include "weapon.h"

class creaturetype
{
   std::string name;
   std::map<std::string,int> information;
   char possible_work_location[SITENUM]; // verifyworklocation
   enum Activity default_teach_activity; // activate.cpp (651)
   bool is_law_enforcement;              // chase.cpp (281, 295, 549, 562)
   bool default_attack_persuasion;       // fight.cpp (99, 320, 426)
   //vector<string>* persuasion_dialect;   // fight.cpp (453) don't need this; use best skill
   bool major_character;                 // fight.cpp (352), haulkidnap.cpp (185)
   std::string hostage_struggle_verb;         // commondisplay.cpp (512)
   std::vector<enum Weapons> weapons;          // makecreature
   bool ignore_gun_control;              // used with weapons vector to determine if gun control
                                         // laws are ignored
   enum Clothing clothing;
   enum Armor armor;
   enum Special special;

   std::map<short,int> skillpackages;         // determines what skills to roll up for them
   std::vector<short> additionalskills;
   bool always_conservative;
   bool always_liberal;


};