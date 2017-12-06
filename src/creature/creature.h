
// Please do not directly include this file. Include includes.h instead.
// You will get compiler errors otherwise.

#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED
#include "augmentation.h"
#include "common.h"
#include <deque>
#include "creature/attributes.h"
#include "creature/gender.h"
#include "creature/skill.h"
#include "politics/alignment.h"

#define CREATUREFLAG_WHEELCHAIR BIT1
#define CREATUREFLAG_JUSTESCAPED BIT2
#define CREATUREFLAG_MISSING BIT3
#define CREATUREFLAG_KIDNAPPED BIT4
#define CREATUREFLAG_SLEEPER BIT5
#define CREATUREFLAG_ILLEGALALIEN BIT6
#define CREATUREFLAG_LOVESLAVE BIT7
#define CREATUREFLAG_BRAINWASHED BIT8
#define CREATUREFLAG_CONVERTED BIT9

#define CREATURE_NAMELEN 40

#define MAXSUBORDINATES 30

// MAXATTRIBUTE is maximum value for both attributes & skills, set to 99 to limit it to 2 digits on screen
#define MAXATTRIBUTE 99

enum CreatureTypes
{
   CREATURE_BOUNCER,
   CREATURE_SECURITYGUARD,
   CREATURE_SCIENTIST_LABTECH,
   CREATURE_SCIENTIST_EMINENT,
   CREATURE_CORPORATE_MANAGER,
   CREATURE_CORPORATE_CEO,
   CREATURE_WORKER_SERVANT,
   CREATURE_WORKER_JANITOR,
   CREATURE_WORKER_SWEATSHOP,
   CREATURE_WORKER_FACTORY_NONUNION,
   CREATURE_WORKER_FACTORY_CHILD,
   CREATURE_WORKER_SECRETARY,
   CREATURE_WORKER_FACTORY_UNION,
   CREATURE_LANDLORD,
   CREATURE_TEENAGER,
   CREATURE_COP,
   CREATURE_SWAT,
   CREATURE_DEATHSQUAD,
   CREATURE_FIREFIGHTER,
   CREATURE_EDUCATOR,
   CREATURE_GANGUNIT,
   CREATURE_JUDGE_LIBERAL,
   CREATURE_JUDGE_CONSERVATIVE,
   CREATURE_AGENT,
   CREATURE_RADIOPERSONALITY,
   CREATURE_NEWSANCHOR,
   CREATURE_GENETIC,
   CREATURE_GUARDDOG,
   CREATURE_PRISONER,
   CREATURE_JUROR,
   CREATURE_LAWYER,
   CREATURE_SEWERWORKER,
   CREATURE_COLLEGESTUDENT,
   CREATURE_MUSICIAN,
   CREATURE_MATHEMATICIAN,
   CREATURE_TEACHER,
   CREATURE_HSDROPOUT,
   CREATURE_BUM,
   CREATURE_GANGMEMBER,
   CREATURE_CRACKHEAD,
   CREATURE_PRIEST,
   CREATURE_ENGINEER,
   CREATURE_FASTFOODWORKER,
   CREATURE_BAKER,
   CREATURE_BARISTA,
   CREATURE_BARTENDER,
   CREATURE_TELEMARKETER,
   CREATURE_CARSALESMAN,
   CREATURE_OFFICEWORKER,
   CREATURE_FOOTBALLCOACH,
   CREATURE_PROSTITUTE,
   CREATURE_MAILMAN,
   CREATURE_GARBAGEMAN,
   CREATURE_PLUMBER,
   CREATURE_CHEF,
   CREATURE_CONSTRUCTIONWORKER,
   CREATURE_AMATEURMAGICIAN,
   CREATURE_TANK,
   CREATURE_MERC,
   CREATURE_HICK,
   CREATURE_SOLDIER,
   CREATURE_VETERAN,
   CREATURE_HARDENED_VETERAN,
   CREATURE_PRISONGUARD,
   CREATURE_HIPPIE,
   CREATURE_CRITIC_ART,
   CREATURE_CRITIC_MUSIC,
   CREATURE_SOCIALITE,
   CREATURE_PROGRAMMER,
   CREATURE_RETIREE,
   CREATURE_PAINTER,
   CREATURE_SCULPTOR,
   CREATURE_AUTHOR,
   CREATURE_JOURNALIST,
   CREATURE_DANCER,
   CREATURE_PHOTOGRAPHER,
   CREATURE_CAMERAMAN,
   CREATURE_HAIRSTYLIST,
   CREATURE_FASHIONDESIGNER,
   CREATURE_CLERK,
   CREATURE_THIEF,
   CREATURE_ACTOR,
   CREATURE_YOGAINSTRUCTOR,
   CREATURE_MARTIALARTIST,
   CREATURE_ATHLETE,
   CREATURE_BIKER,
   CREATURE_TRUCKER,
   CREATURE_TAXIDRIVER,
   CREATURE_NUN,
   CREATURE_MUTANT,
   CREATURE_DOCTOR,
   CREATURE_NURSE,
   CREATURE_CCS_VIGILANTE,
   CREATURE_CCS_ARCHCONSERVATIVE,
   CREATURE_POLITICALACTIVIST,
   CREATURE_CCS_MOLOTOV,
   CREATURE_CCS_SNIPER,
   CREATURE_PSYCHOLOGIST,
   CREATURE_LOCKSMITH,
   CREATURE_MILITARYPOLICE,
   CREATURE_SEAL,
   CREATURE_MILITARYOFFICER,
   CREATURE_BANK_TELLER,
   CREATURE_BANK_MANAGER,
   CREATURE_SECRET_SERVICE,
   CREATURE_POLITICIAN,
   CREATURENUM
};

enum Bodyparts
{
   BODYPART_HEAD,
   BODYPART_BODY,
   BODYPART_ARM_RIGHT,
   BODYPART_ARM_LEFT,
   BODYPART_LEG_RIGHT,
   BODYPART_LEG_LEFT,
   BODYPARTNUM
};

enum SpecialWounds
{
   SPECIALWOUND_TEETH,
   SPECIALWOUND_RIGHTEYE,
   SPECIALWOUND_LEFTEYE,
   SPECIALWOUND_NOSE,
   SPECIALWOUND_TONGUE,
   SPECIALWOUND_RIGHTLUNG,
   SPECIALWOUND_LEFTLUNG,
   SPECIALWOUND_HEART,
   SPECIALWOUND_LIVER,
   SPECIALWOUND_STOMACH,
   SPECIALWOUND_RIGHTKIDNEY,
   SPECIALWOUND_LEFTKIDNEY,
   SPECIALWOUND_SPLEEN,
   SPECIALWOUND_RIBS,
   SPECIALWOUND_NECK,
   SPECIALWOUND_UPPERSPINE,
   SPECIALWOUND_LOWERSPINE,
   SPECIALWOUNDNUM
};

#define RIBNUM 10
#define TOOTHNUM 32

#define WOUND_SHOT BIT1
#define WOUND_CUT BIT2
#define WOUND_BRUISED BIT3
#define WOUND_BURNED BIT4
#define WOUND_BLEEDING BIT5
#define WOUND_TORN BIT6
#define WOUND_NASTYOFF BIT7
#define WOUND_CLEANOFF BIT8


class Creature
{
public:
   Creature()
   { creatureinit(); }

   Creature(const Creature& org)
   { copy(org); }

   explicit Creature(const std::string& inputXml);

   ~Creature();

   Creature&
    operator=(const Creature& rhs);

   void
   creatureinit();

   void set_attribute(int attribute, int amount) { attributes[attribute].value=MIN(amount,MAXATTRIBUTE); }
   int get_attribute(int attribute, bool use_juice) const;
   void adjust_attribute(int attribute, int amount) { set_attribute(attribute,attributes[attribute].value+amount); }
   int attribute_roll(int attribute) const;
   bool attribute_check(int attribute, int difficulty) const;

   void set_skill(int skill, int amount) { skills[skill].value=MIN(amount,MAX_SKILL_LEVEL); }
   int get_skill(int skill) const { return MIN(skills[skill].value,MAX_SKILL_LEVEL); }
   int skill_roll(int skill) const;
   bool skill_check(int skill, int difficulty) const;
   int get_weapon_skill() const;

   Augmentation& get_augmentation(int aug_num) { return augmentations[aug_num]; }
   
   void die();

   void train(int trainedskill, int experience, int upto=MAX_SKILL_LEVEL);
   void skill_up();
   int get_skill_ip(int skill) const { return skill_experience[skill]; }
   std::string get_type_name() const; // this function is implemented inline in creaturetype.h (can't do it here since CreatureType has to be defined after Creature)

   bool enemy() const;

   bool is_armed() const { return weapon; }
   bool is_naked() const { return !armor; }
   Weapon& get_weapon() const;
   Armor& get_armor() const;
   bool will_do_ranged_attack(bool force_ranged,bool force_melee) const; //force_melee is likely unnecessary. -XML
   bool can_reload() const;
   bool will_reload(bool force_ranged, bool force_melee) const;
   bool reload(bool wasteful);
   bool ready_another_throwing_weapon();
   bool take_clips(Item& clip, int number);
   bool take_clips(Clip& clip, int number);
   bool take_clips(const ClipType& ct, int number);
   int count_clips() const;
   void give_weapon(Weapon& w, vector<Item*>* lootpile);
   void give_weapon(const WeaponType& wt, vector<Item*>* lootpile);
   void drop_weapon(vector<Item*>* lootpile);
   void drop_weapons_and_clips(vector<Item*>* lootpile);
   int count_weapons() const;
   void give_armor(Armor& a, vector<Item*>* lootpile);
   void give_armor(const ArmorType& at, vector<Item*>* lootpile);
   void strip(vector<Item*>* lootpile);
   bool weapon_is_concealed() const { return is_armed()&&get_armor().conceals_weapon(*weapon); }
   string get_weapon_string(int subtype) const;
   string get_armor_string(bool fullname) const { return get_armor().equip_title(fullname); }

   void stop_hauling_me();

   string showXml() const;

   bool is_active_liberal() const;
   bool is_imprisoned() const;
   bool is_lcs_sleeper() const;
   bool canwalk() const;
   //int attval(short a,char usejuice=1);

   /* are they interested in talking about the issues? */
   bool talkreceptive() const;
   /* are the characters close enough in age to date? */
   bool can_date(const Creature &a) const;
   /* rolls up a proper name for a creature */
   void namecreature();
   /* can turn the tables on datenapping */
   bool kidnap_resistant() const;
   bool reports_to_police() const;
   /* returns the creature's maximum level in the given skill */
   int skill_cap(int skill, bool use_juice) const { return get_attribute(Skill::get_associated_attribute(skill),use_juice); }
   const char* heshe(bool capitalize=false) const;
   const char* hisher(bool capitalize=false) const;
   const char* himher(bool capitalize=false) const;

public:
   char name[CREATURE_NAMELEN];
   char propername[CREATURE_NAMELEN];
   char gender_conservative;
   char gender_liberal;
   int squadid;//REMEMBER, THIS IS ID NUMBER, NOT ARRAY INDEX
   int age;
   int birthday_month;
   int birthday_day;
   bool exists;
   Alignment  align;
   bool alive;
   short type;
   std::string type_idname;
   float infiltration;
   char animalgloss;
   short specialattack;
   short clinic;
   short dating;
   short hiding;
   short trainingtime;
   short trainingsubject;
   Creature* prisoner;
   short sentence;
   char confessions;
   char deathpenalty;
   int joindays;
   int deathdays;
   int id;
   int hireid;
   int meetings;
   char forceinc;
   deque<Weapon*> extra_throwing_weapons;
   deque<Clip*> clips;
   bool has_thrown_weapon;
   int stunned;
   int money;
   short juice;
   short income;
   unsigned char wound[BODYPARTNUM];
   short blood;
   char special[SPECIALWOUNDNUM];
   //int crimes_committed[LAWFLAGNUM];
   int crimes_suspected[LAWFLAGNUM];
   //int crimes_convicted[LAWFLAGNUM];
   int heat;
   int location;
   int worklocation;
   char cantbluff;
   int base;
   activityst activity;
   int carid;
   char is_driver;
   int pref_carid;
   char pref_is_driver;
   short flag;
   bool dontname;

private:
   void copy(const Creature& org);
   class Attribute attributes[ATTNUM];
   class Skill skills[SKILLNUM];
   class Augmentation augmentations[AUGMENTATIONNUM];
   int skill_experience[SKILLNUM];
   Weapon* weapon;
   Armor*  armor;
};


enum uniqueCreatureData
{
   UNIQUECREATURE_ALIVE,
   UNIQUECREATURE_DEAD,
   UNIQUECREATURE_LIBERAL
};


class UniqueCreatures
{
private:
   Creature CEO_;
   int CEO_ID;
   Creature Pres_;
   int Pres_ID;

public:
   int CEO_state;
   int Pres_state;
   UniqueCreatures() : CEO_ID(-1), Pres_ID(-1) { }
   explicit UniqueCreatures(const std::string& inputXml);
   string showXml() const;

   void newCEO();
   void newPresident();
   Creature& CEO() { if(CEO_ID==-1) newCEO(); return CEO_; }
   Creature& President() { if(Pres_ID==-1) newPresident(); return Pres_; }
   void initialize() { newCEO(); newPresident(); }
};

#endif //CREATURE_H_INCLUDED
