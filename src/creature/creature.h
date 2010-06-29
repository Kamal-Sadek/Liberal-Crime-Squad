
// Please do not directly include this file. Include includes.h instead.
// You will get compiler errors otherwise.

#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED
#include <string>

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

#define GENDER_NEUTRAL 0
#define GENDER_MALE    1
#define GENDER_FEMALE  2
#define GENDER_WHITEMALEPATRIARCH 3

enum CheckDifficulty
{
   DIFFICULTY_AUTOMATIC    = 1,
   DIFFICULTY_VERYEASY     = 3,
   DIFFICULTY_EASY         = 5,
   DIFFICULTY_AVERAGE      = 7,
   DIFFICULTY_CHALLENGING  = 9,
   DIFFICULTY_HARD         = 11,
   DIFFICULTY_FORMIDABLE   = 13,
   DIFFICULTY_HEROIC       = 15,
   DIFFICULTY_SUPERHEROIC  = 17,
   DIFFICULTY_IMPOSSIBLE   = 19
};

enum CreatureAttribute
{
   ATTRIBUTE_STRENGTH,
   ATTRIBUTE_INTELLIGENCE,
   ATTRIBUTE_WISDOM,
   ATTRIBUTE_AGILITY,
   ATTRIBUTE_HEALTH,
   ATTRIBUTE_CHARISMA,
   ATTRIBUTE_HEART,
   ATTNUM
};

enum CreatureSkill
{
   SKILL_HANDTOHAND,
   SKILL_KNIFE,
   SKILL_SWORD,
   SKILL_CLUB,
   SKILL_AXE,
   SKILL_THROWING,
   SKILL_PISTOL,
   SKILL_RIFLE,
   SKILL_SMG,
   SKILL_SHOTGUN,
   SKILL_PERSUASION,
   SKILL_PSYCHOLOGY,
   SKILL_LAW,
   SKILL_SECURITY,
   SKILL_DISGUISE,
   SKILL_COMPUTERS,
   SKILL_TAILORING,
   SKILL_DRIVING,
   SKILL_WRITING,
   SKILL_COOKING,
   SKILL_MUSIC,
   SKILL_ART,
   SKILL_RELIGION,
   SKILL_SCIENCE,
   SKILL_BUSINESS,
   SKILL_STEALTH,
   //SKILL_THEFT,
   SKILL_TEACHING,
   SKILL_STREETSENSE,
   SKILL_SEDUCTION,
   SKILL_LEADERSHIP,
   SKILL_FIRSTAID,
   SKILL_DODGE,
   SKILL_FLAMETHROWER,
   //SKILL_SURVIVAL,
   SKILLNUM
};

enum CreatureType
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
   CREATURE_TELEMARKETER,
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

class Skill
{
private:
   int associated_attribute;
   int skill;
public:
   int value;
   void set_type(int skill_type);
   int get_attribute();
   static std::string get_name(int skill_type);
   static CreatureAttribute get_associated_attribute(int skill_type);
};

class Attribute
{
private:
   int attribute;
public:
   int value;
   //void set_type(int attribute_type);
   static std::string get_name(int attribute_type);
};

class Creature
{
private:
   class Attribute attributes[ATTNUM];
   class Skill skills[SKILLNUM];
   int skill_experience[SKILLNUM];
   int roll_check(int skill);
public:
   void set_attribute(int attribute, int amount);
   int get_attribute(int attribute, bool use_juice) const;
   void adjust_attribute(int attribute, int amount);
   int attribute_roll(int attribute);
   bool attribute_check(int attribute, int difficulty);

   void set_skill(int skill, int amount);
   int get_skill(int skill) const;
   int skill_roll(int skill);
   bool skill_check(int skill, int difficulty);
   
   char name[CREATURE_NAMELEN];
   char propername[CREATURE_NAMELEN];
   char gender_conservative;
   char gender_liberal;
   int squadid;//REMEMBER, THIS IS ID NUMBER, NOT ARRAY INDEX
   int age;
   int birthday_month;
   int birthday_day;
   bool exists;
   char align;
   bool alive;
   void die();
   short type;
   float infiltration;
   char animalgloss;
   short specialattack;
   short clinic;
   short dating;
   short hiding;
   short trainingtime;
   short trainingsubject;
   Creature *prisoner;
   short sentence;
   char confessions;
   char deathpenalty;
   int joindays;
   int deathdays;
   int id;
   int hireid;
   int meetings;
   
   char forceinc;

   void train(int trainedskill, int experience);
   void train(int trainedskill, int experience, int upto);
   void skill_up();
   int get_skill_ip(int skill);

   bool enemy();

   int stunned;

   weaponst weapon;
   armorst armor;
   int clip[CLIPNUM];

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

   void stop_hauling_me();
   
   Creature();
   ~Creature();

   char canwalk(void)
   {
      if(!alive)return 0;
      int legok=2;
      if((wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
          (wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
      if((wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
          (wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
      if(special[SPECIALWOUND_NECK]!=1||
         special[SPECIALWOUND_UPPERSPINE]!=1||
         special[SPECIALWOUND_LOWERSPINE]!=1)return 0;
      if(legok==0)return 0;

      return 1;
   }
   void creatureinit(void);
   //int attval(short a,char usejuice=1);

   /* are they interested in talking about the issues? */
   bool talkreceptive();
   /* are the characters close enough in age to date? */
   bool can_date(Creature &a);
   /* rolls up a proper name for a creature */
   void namecreature();
   bool dontname;
   /* finds the cap for a particular skill */
   int skill_cap(int skill, bool use_juice);
   const char *heshe();
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

public:
   int CEO_state;
   UniqueCreatures() : CEO_ID(-1) {};

   Creature& CEO();

   void initialize();
};

#endif //CREATURE_H_INCLUDED
