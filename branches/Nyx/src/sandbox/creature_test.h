
#include <map>
#include <string>
#include <vector>
#include "weapon.h"
#include "injury_test.h"

enum Lawflags
{
   LAWFLAG_KIDNAPPING,
   LAWFLAG_MURDER,
   LAWFLAG_THEFT,
   LAWFLAG_BREAKING,
   LAWFLAG_TERRORISM,
   LAWFLAG_JURY,
   LAWFLAG_TREASON,
   LAWFLAG_ESCAPED,
   LAWFLAG_HELPESCAPE,
   LAWFLAG_RESIST,
   LAWFLAG_BURNFLAG,
   LAWFLAG_SPEECH,
   LAWFLAG_VANDALISM,
   LAWFLAG_ASSAULT,
   LAWFLAG_CARTHEFT,
   LAWFLAG_INFORMATION,
   LAWFLAG_COMMERCE,
   LAWFLAG_CCFRAUD,
   LAWFLAG_BROWNIES,
   LAWFLAG_BURIAL,
   LAWFLAG_PROSTITUTION,
   LAWFLAG_DISTURBANCE,
   LAWFLAG_HIREILLEGAL,
   LAWFLAG_RACKETEERING,
   LAWFLAG_LOITERING,
   LAWFLAG_GUNCARRY,
   LAWFLAGNUM
};

enum creature_type
{
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
   CREATURE_MILITARYPOLICE,
   CREATURE_MILITARYOFFICER,
   CREATURE_SEAL,
   CREATURENUM
};

enum Attributes
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

enum Skills
{
   SKILL_HANDTOHAND,
   SKILL_KNIFE,
   SKILL_SWORD,
   SKILL_CLUB,
   SKILL_IMPROVISED,
   SKILL_PISTOL,
   SKILL_RIFLE,
   SKILL_SMG,
   SKILL_SHOTGUN,
   SKILL_PERSUASION,
   SKILL_INTERROGATION,
   SKILL_LAW,
   SKILL_SECURITY,
   SKILL_DISGUISE,
   SKILL_COMPUTERS,
   SKILL_GARMENTMAKING,
   SKILL_DRIVING,
   SKILL_WRITING,
   SKILL_COOKING,
   SKILL_MUSIC,
   SKILL_ART,
   SKILL_RELIGION,
   SKILL_SCIENCE,
   SKILL_BUSINESS,
   SKILL_GANGSTERISM,
   SKILL_STEALTH,
   SKILL_SLEIGHTOFHAND,
   SKILL_TEACHING,
   SKILL_SURVIVAL,
   SKILL_STREETSENSE,
   SKILL_SEDUCTION,
   SKILL_LEADERSHIP,
   SKILL_MEDICAL,
   SKILL_TACTICS,
   SKILLNUM
};

enum species
{
   SPECIES_HUMAN,
   SPECIES_TANK,
   SPECIES_ANIMAL,
   SPECIESNUM
};

class creature;

class witness
{
   creature*            Witness;
   std::map<int,int>    Crimes;
};

class sentence
{
   bool           Execution;
   class date*    EndDate;
};

class criminal_record
{
   int alltime[LAWFLAGNUM];
   int acquitted[LAWFLAGNUM];
   int convicted[LAWFLAGNUM];
   int serving[LAWFLAGNUM];
   int pardoned[LAWFLAGNUM];
   int wanted[LAWFLAGNUM];

   std::vector<witness> Witnesses;
   class sentence* Sentence;
};

enum creature_status
{
   STATUS_FREE,
   STATUS_VACATION,
   STATUS_HIDING,
   STATUS_ARRESTED,
   STATUS_SERVINGTIME,
   STATUS_CLINIC,
   STATUSNUM
};

enum unarmed_style
{
   UNARMED_PUGILISM,
   UNARMED_MARTIALARTS,
   UNARMED_TANKCANNON,
   UNARMED_FLAME,
   UNARMED_SUCK,
   UNARMEDNUM
};

#define CREATUREFLAG_HASWHEELCHAIR  BIT1
#define CREATUREFLAG_JUSTESCAPED    BIT2
#define CREATUREFLAG_MISSING        BIT3
#define CREATUREFLAG_KIDNAPPED      BIT4
#define CREATUREFLAG_SLEEPER        BIT5
#define CREATUREFLAG_ILLEGALALIEN   BIT6
#define CREATUREFLAG_LOVESLAVE      BIT7
#define CREATUREFLAG_BRAINWASHED    BIT8
#define CREATUREFLAG_CONVERTED      BIT9

enum gender
{
   GENDER_MALE,
   GENDER_FEMALE,
   GENDER_NONE,
   GENDERNUM
};

class creature
{
public:
//private:
   std::string             Name;
   std::string       ProperName;

   int                     ID;
   int                     SquadID;
   int                     HireID;
   class date*             JoinDate;
   class criminal_record*  CriminalRecord;

   enum creature_type      Type;
   signed char             Align;
   enum species            Species;
   enum gender             Gender;
   enum gender             BirthGender;

   class date*             BirthDate;
   class date*             DeathDate;

   class weapon*           Weapon;
   class armor*            Armor;
   class clothing*         Clothing;
   class special*          Special;
   class creature*         Hostage;

   int                     Clip[CLIPNUM];
   int                     Cash;

   enum unarmed_style      UnarmedAttack;

   enum creature_status    Status;


   int                     Blood;
   bool                    Incapacitated;
   class bodypart          Wound[BODYPARTNUM];
   char                    SpecialWound[SPECIALWOUNDNUM];

   int                     Attributes[ATTNUM];
   int                     Skills[SKILLNUM];
   int                     SkillExp[SKILLNUM];
   int                     Juice;
   
   bool                    Alive();

   int                     Location;
   int                     WorkLocation;
   int                     Safehouse;

   bool                    CantBeBluffed;

   class activity*         Activity;

   int                     CarID;
   bool                    IsDriver;
   int                     Prefer_CarID;
   bool                    Prefer_IsDriver;

   unsigned short          Flags;

public:
   creature(int UniqueID);
   ~creature();
   char CanWalk();
   int Attribute(short attribute,char usejuice=1);
   template<class Archive>
   void serialize(Archive & ar, const unsigned int /* file_version */);
   void NewName();
};

void GenerateName(std::string& str, enum gender name_gender);
