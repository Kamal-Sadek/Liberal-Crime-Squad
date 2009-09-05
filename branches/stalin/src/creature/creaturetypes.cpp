
#include "includes.h"
#include "externs.h"

/* Age macros for characters */
#define AGE_DOGYEARS    2+LCSrandom(5)   /* for the animals */
#define AGE_CHILD       7+LCSrandom(4)   /* child laborer */
#define AGE_TEENAGER    14+LCSrandom(4)  /* HS dropout, teenager, some fast food workers */
#define AGE_YOUNGADULT  18+LCSrandom(18) /* young lads and ladies */
#define AGE_MATURE      20+LCSrandom(40) /* most folks */
#define AGE_GRADUATE    26+LCSrandom(34) /* most folks, with 6-8 years college */
#define AGE_MIDDLEAGED  35+LCSrandom(25) /* for the more experienced types */
#define AGE_SENIOR      65+LCSrandom(30) /* ah, social security */


/* Weapon assignment macros */
#define GIVE_WEAPON_CIVILIAN                                   \
         if(law[LAW_GUNCONTROL]==-1 && !LCSrandom(30)) {       \
            cr.weapon.type=WEAPON_REVOLVER_38;                 \
            cr.weapon.ammo=6;                                  \
            cr.clip[CLIP_38]=3;                                \
         } else if(law[LAW_GUNCONTROL]==-2) {                  \
            if(!LCSrandom(10)) {                               \
               cr.weapon.type=WEAPON_SEMIPISTOL_9MM;           \
               cr.weapon.ammo=15;                              \
               cr.clip[CLIP_9]=3;                              \
            } else if(!LCSrandom(9)) {                         \
               cr.weapon.type=WEAPON_SEMIPISTOL_45;            \
               cr.weapon.ammo=15;                              \
               cr.clip[CLIP_45]=3;                             \
            }                                                  \
         }

/* Gender macros */
#define GIVE_GENDER_MALE                                       \
   if(law[LAW_WOMEN]==-2 ||                              \
   (law[LAW_WOMEN]==-1 && LCSrandom(25)) ||           \
   (law[LAW_WOMEN]==0 && LCSrandom(10)) ||            \
   (law[LAW_WOMEN]==1 && LCSrandom(4))) {             \
   cr.gender_conservative=cr.gender_liberal=GENDER_MALE; \
         }

#define GIVE_GENDER_FEMALE                                     \
   if(law[LAW_WOMEN]==-2 ||                              \
   (law[LAW_WOMEN]==-1 && LCSrandom(25)) ||           \
   (law[LAW_WOMEN]==0 && LCSrandom(10)) ||            \
   (law[LAW_WOMEN]==1 && LCSrandom(4))) {             \
   cr.gender_conservative=cr.gender_liberal=GENDER_FEMALE; \
   }

/* rolls up a creature's stats and equipment */
void makecreature(Creature &cr,short type)
{
   int a = 0;
   cr.creatureinit();

   cr.exists=1;
   cr.squadid=-1;
   cr.type=type;
   cr.infiltration=0;
   getrecruitcreature(cr.name,type);
   cr.armor.type=ARMOR_CLOTHES;
   cr.money=LCSrandom(21)+20;
   {
      int mood=publicmood(-1);
      conservatise(cr);
      if(LCSrandom(100)<mood)cr.align++;
      if(LCSrandom(100)<mood)cr.align++;
   }
   //cr.align=LCSrandom(3)-1;
   cr.worklocation=cursite;
   verifyworklocation(cr);

   int attcap[ATTNUM];
   for(a=0;a<ATTNUM;a++)
   {
      attcap[a]=10;
      cr.att[a]=1;
   }
   int attnum=40;
   switch(type)
   {
      case CREATURE_BOUNCER:
         GIVE_GENDER_MALE;
         cr.weapon.type=WEAPON_NONE;
         if(mode==GAMEMODE_SITE && location[cursite]->highsecurity)
         {
            strcpy(cr.name,"Enforcer");
            cr.skill[SKILL_CLUB]=LCSrandom(3)+3;
         }

         if(law[LAW_GUNCONTROL]==-2)
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.weapon.ammo=15;
            cr.clip[CLIP_SMG]=3;
         }
         else if(law[LAW_GUNCONTROL]==-1)
         {
            cr.weapon.type=WEAPON_REVOLVER_44;
            cr.weapon.ammo=6;
            cr.clip[CLIP_44]=3;
         }
         else if(law[LAW_GUNCONTROL]==0)
         {
            cr.weapon.type=WEAPON_REVOLVER_38;
            cr.weapon.ammo=6;
            cr.clip[CLIP_38]=3;
         }
         else
         {
            cr.weapon.type=WEAPON_NIGHTSTICK;
         }
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(3)+1;
         if(disguisesite(sitetype)) { cr.align=-1; cr.infiltration=0.1f*LCSrandom(4); }
         else cr.align=0;
         cr.age=AGE_MATURE;

         cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_AGILITY]=3;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         break;
      case CREATURE_SECURITYGUARD:
         GIVE_GENDER_MALE;         
         if(law[LAW_GUNCONTROL]==-2)
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.weapon.ammo=15;
            cr.clip[CLIP_SMG]=3;
         }
         else if(law[LAW_GUNCONTROL]==-1)
         {
            cr.weapon.type=WEAPON_REVOLVER_44;
            cr.weapon.ammo=6;
            cr.clip[CLIP_44]=3;
         }
         else if(law[LAW_GUNCONTROL]!=2)
         {
            cr.weapon.type=WEAPON_REVOLVER_38;
            cr.weapon.ammo=6;
            cr.clip[CLIP_38]=3;
         }
         else
         {
            cr.weapon.type=WEAPON_NIGHTSTICK;
         }
         cr.armor.type=ARMOR_SECURITYUNIFORM;
         cr.skill[SKILL_PISTOL]=LCSrandom(3)+1;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(3);
         cr.age=AGE_MATURE;
         cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_AGILITY]=3;
         cr.att[ATTRIBUTE_STRENGTH]=2;
         break;
      case CREATURE_SCIENTIST_LABTECH:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         if(cr.weapon.type == WEAPON_NONE && !LCSrandom(2))
            cr.weapon.type=WEAPON_SYRINGE;
         cr.armor.type=ARMOR_LABCOAT;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;
         
         cr.att[ATTRIBUTE_INTELLIGENCE]=5;
         cr.skill[SKILL_COMPUTERS]=LCSrandom(2)+1;
         cr.skill[SKILL_SCIENCE]=LCSrandom(4)+3;
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Hangin' Judge");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_44;
            cr.weapon.ammo=6;
            cr.clip[CLIP_44]=3;
         }
         else if(!LCSrandom(2))cr.weapon.type=WEAPON_GAVEL;
         cr.armor.type=ARMOR_BLACKROBE;
         cr.money=LCSrandom(41)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         cr.skill[SKILL_LAW]=LCSrandom(6)+5;
         cr.skill[SKILL_WRITING]=LCSrandom(3)+1;
         
         cr.att[ATTRIBUTE_INTELLIGENCE]=5;
         cr.att[ATTRIBUTE_WISDOM]=10;
         attcap[ATTRIBUTE_HEART]=1;
         break;
      case CREATURE_JUDGE_LIBERAL:
         strcpy(cr.name,"Liberal Judge");
         if(!LCSrandom(2))cr.weapon.type=WEAPON_GAVEL;
         cr.armor.type=ARMOR_BLACKROBE;
         cr.money=LCSrandom(41)+20;
         cr.align=1;
         cr.age=AGE_MIDDLEAGED;

         cr.skill[SKILL_LAW]=LCSrandom(6)+5;
         cr.skill[SKILL_WRITING]=LCSrandom(3)+1;
         
         cr.att[ATTRIBUTE_INTELLIGENCE]=5;
         cr.att[ATTRIBUTE_HEART]=10;
         break;
      case CREATURE_SCIENTIST_EMINENT:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         if(cr.weapon.type==WEAPON_NONE && !LCSrandom(2))cr.weapon.type=WEAPON_SYRINGE;
         cr.armor.type=ARMOR_LABCOAT;
         cr.money=LCSrandom(41)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         cr.skill[SKILL_WRITING]=LCSrandom(3)+1;
         cr.skill[SKILL_SCIENCE]=LCSrandom(6)+6;
         
         cr.att[ATTRIBUTE_INTELLIGENCE]=10;
         cr.att[ATTRIBUTE_WISDOM]=6;
         break;
      case CREATURE_CORPORATE_MANAGER:
         GIVE_WEAPON_CIVILIAN;
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(41)+40;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(6);
         cr.age=AGE_MATURE;
         GIVE_GENDER_MALE;
         cr.skill[SKILL_BUSINESS]=LCSrandom(4)+3;

         
         cr.att[ATTRIBUTE_INTELLIGENCE]=4;
         cr.att[ATTRIBUTE_CHARISMA]=5;
         cr.att[ATTRIBUTE_WISDOM]=5;
         break;
      case CREATURE_CORPORATE_CEO:
         GIVE_GENDER_MALE;         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_44;
            cr.weapon.ammo=6;
            cr.clip[CLIP_44]=3;
         }
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(121)+120;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         cr.gender_liberal=GENDER_MALE;
         if(LCSrandom(10))cr.gender_conservative=GENDER_MALE;
         else if(cr.gender_conservative==GENDER_FEMALE)cr.skill[SKILL_DISGUISE]=LCSrandom(3)+4;

         cr.skill[SKILL_BUSINESS]=LCSrandom(6)+6;

         
         cr.att[ATTRIBUTE_INTELLIGENCE]=7;
         cr.att[ATTRIBUTE_CHARISMA]=7;
         cr.att[ATTRIBUTE_WISDOM]=12;
         break;
      case CREATURE_WORKER_SERVANT:
         cr.money=0;
         cr.align=1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_JANITOR:
         GIVE_WEAPON_CIVILIAN;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_SWEATSHOP:
         cr.armor.type=ARMOR_NONE;
         cr.align=1;
         cr.age=AGE_MATURE;
         cr.juice=-20;
         cr.flag|=CREATUREFLAG_ILLEGALALIEN;
         cr.skill[SKILL_TAILORING]=LCSrandom(5)+1;
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Nonunion Worker");
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         if(cr.align==1)
            cr.align=LCSrandom(2)-1;
         cr.age=AGE_MATURE;
         
         cr.att[ATTRIBUTE_STRENGTH]=5;
         break;
      case CREATURE_WORKER_SECRETARY:
         GIVE_WEAPON_CIVILIAN;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_WRITING]=LCSrandom(2)+1;
         cr.att[ATTRIBUTE_INTELLIGENCE]=2;
         break;
      case CREATURE_LANDLORD:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Landlord");
         cr.money=LCSrandom(121)+120;
         cr.align=0;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_BUSINESS]=LCSrandom(4)+3;
         cr.att[ATTRIBUTE_INTELLIGENCE]=4;
         cr.att[ATTRIBUTE_WISDOM]=4;
         break;
      case CREATURE_TEENAGER:
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_TEENAGER;
         break;
      case CREATURE_LAWYER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_38;
            cr.weapon.ammo=6;
            cr.clip[CLIP_38]=0;
         }
         strcpy(cr.name,"Lawyer");
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(51)+50;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         cr.skill[SKILL_LAW]=LCSrandom(4)+4;
         cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;

         cr.att[ATTRIBUTE_INTELLIGENCE]=6;
         cr.att[ATTRIBUTE_CHARISMA]=4;
         break;
      case CREATURE_DOCTOR:
         GIVE_GENDER_MALE;         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_38;
            cr.weapon.ammo=6;
            cr.clip[CLIP_38]=0;
         }
         strcpy(cr.name,"Doctor");
         cr.armor.type=ARMOR_LABCOAT;
         cr.money=LCSrandom(21)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         cr.skill[SKILL_FIRSTAID]=LCSrandom(4)+4;
         
         cr.att[ATTRIBUTE_INTELLIGENCE]=6;
         break;
      case CREATURE_NURSE:
         GIVE_GENDER_FEMALE;         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_38;
            cr.weapon.ammo=6;
            cr.clip[CLIP_38]=0;
         }
         strcpy(cr.name,"Nurse");
         cr.armor.type=ARMOR_LABCOAT;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         cr.skill[SKILL_FIRSTAID]=LCSrandom(4)+1;
         break;
      case CREATURE_WORKER_FACTORY_UNION:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Union Worker");
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.align=1;
         cr.age=AGE_MATURE;
         
         cr.att[ATTRIBUTE_STRENGTH]=5;
         break;
      case CREATURE_TANK:
         strcpy(cr.name,"Tank");
         cr.animalgloss=ANIMALGLOSS_TANK;
         cr.armor.type=ARMOR_NONE;
         cr.specialattack=ATTACK_CANNON;
         cr.align=-1;
         cr.age=AGE_TEENAGER; // wut?
         break;
      case CREATURE_MERC:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Mercenary");
         if(law[LAW_GUNCONTROL]==-2)
         {
            cr.weapon.type=WEAPON_AUTORIFLE_M16;
            cr.weapon.ammo=30;
            cr.clip[CLIP_ASSAULT]=6;
         }
         else
         {
            cr.weapon.type=WEAPON_SEMIRIFLE_AR15;
            cr.clip[CLIP_ASSAULT]=6;
            cr.weapon.ammo=30;
         }
         cr.armor.type=ARMOR_CIVILLIANARMOR;
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+1;
         cr.skill[SKILL_SECURITY]=LCSrandom(3);
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(3);
         cr.skill[SKILL_PISTOL]=LCSrandom(3);
         cr.skill[SKILL_DRIVING]=LCSrandom(2);
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(2);
         break;
      case CREATURE_HICK:
         switch(LCSrandom(5))
         {
            case 0:strcpy(cr.name,"Country Boy");break;
            case 1:strcpy(cr.name,"Hick");break;
            case 2:strcpy(cr.name,"Redneck");break;
            case 3:strcpy(cr.name,"Rube");break;
            case 4:strcpy(cr.name,"Yokel");break;
         }
         if((law[LAW_GUNCONTROL]==-2 && !LCSrandom(2)) || !LCSrandom(10))
         {
            cr.weapon.type=WEAPON_SHOTGUN_PUMP;
            cr.weapon.ammo=6;
            cr.clip[CLIP_BUCKSHOT]=3;
         }
         else if(!LCSrandom(2))cr.weapon.type=WEAPON_TORCH;
         else cr.weapon.type=WEAPON_PITCHFORK;

         if(!LCSrandom(2))cr.armor.type=ARMOR_OVERALLS;
         else cr.armor.type=ARMOR_WIFEBEATER;
         cr.gender_conservative=cr.gender_liberal=GENDER_MALE;
         cr.money=LCSrandom(6)+6;
         cr.align=-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_SOLDIER:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Soldier");
         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_ARMYARMOR;
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(100);
         cr.age=AGE_YOUNGADULT;

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+1;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(3)+1;
         cr.skill[SKILL_PISTOL]=LCSrandom(3)+1;
         cr.skill[SKILL_DRIVING]=LCSrandom(3)+1;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3)+1;

         cr.att[ATTRIBUTE_STRENGTH]=5;
         cr.att[ATTRIBUTE_AGILITY]=5;
         cr.att[ATTRIBUTE_HEALTH]=5;
         cr.att[ATTRIBUTE_WISDOM]=5;
         break;
      case CREATURE_VETERAN:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Army Veteran");
         cr.money=LCSrandom(21)+20;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(100);
         cr.age=AGE_MIDDLEAGED;

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+1;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(3)+1;
         cr.skill[SKILL_PISTOL]=LCSrandom(3)+1;
         cr.skill[SKILL_DRIVING]=LCSrandom(3)+1;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3)+1;
			
         cr.att[ATTRIBUTE_STRENGTH]=5;
			cr.att[ATTRIBUTE_AGILITY]=5;
			cr.att[ATTRIBUTE_HEALTH]=5;
         break;
      case CREATURE_HARDENED_VETERAN:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Hardened Veteran");
         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_ARMYARMOR;
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(100);
         cr.age=AGE_MIDDLEAGED;

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+4;
         cr.skill[SKILL_SECURITY]=LCSrandom(3);
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(3)+2;
         cr.skill[SKILL_PISTOL]=LCSrandom(3)+2;
         cr.skill[SKILL_DRIVING]=LCSrandom(3)+2;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3)+1;
			
         cr.att[ATTRIBUTE_STRENGTH]=7;
			cr.att[ATTRIBUTE_AGILITY]=7;
			cr.att[ATTRIBUTE_HEALTH]=7;
         break;
      case CREATURE_COP:
         if(law[LAW_POLICEBEHAVIOR]==2 && cr.align==ALIGN_LIBERAL && !LCSrandom(3)) // Peace Officer
         {
            GIVE_GENDER_MALE;            cr.align=ALIGN_MODERATE;
            strcpy(cr.name,"Police Negotiator");
            cr.weapon.type=WEAPON_NONE;
            cr.armor.type=ARMOR_POLICEARMOR;
            cr.money=LCSrandom(21)+20;
            cr.juice=10+LCSrandom(50);
            cr.age=AGE_MATURE;

            cr.skill[SKILL_PERSUASION]=LCSrandom(4)+1;
            cr.skill[SKILL_PISTOL]=LCSrandom(3)+1;
            cr.skill[SKILL_DRIVING]=LCSrandom(2)+1;
            cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3)+1;
			   
            cr.att[ATTRIBUTE_STRENGTH]=3;
			   cr.att[ATTRIBUTE_AGILITY]=3;
			   cr.att[ATTRIBUTE_HEALTH]=3;
            cr.att[ATTRIBUTE_HEART]=4;
         }
         else
         {
            GIVE_GENDER_MALE;            if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
            {
               cr.weapon.type=WEAPON_SMG_MP5;
               cr.weapon.ammo=15;
               cr.clip[CLIP_SMG]=3;
            }
            else if(!LCSrandom(3))
            {
               cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
               cr.clip[CLIP_9]=3;
               cr.weapon.ammo=15;
            }
            else if(!LCSrandom(2))
            {
               cr.weapon.type=WEAPON_SHOTGUN_PUMP;
               cr.clip[CLIP_BUCKSHOT]=3;
               cr.weapon.ammo=6;
            }
            else cr.weapon.type=WEAPON_NIGHTSTICK;
            cr.armor.type=ARMOR_POLICEARMOR;
            cr.money=LCSrandom(21)+20;
            cr.align=-1;
            cr.infiltration=0.3f + 0.1f*LCSrandom(4);
            cr.juice=10+LCSrandom(50);
            cr.age=AGE_MATURE;

            cr.skill[SKILL_PISTOL]=LCSrandom(4)+1;
            cr.skill[SKILL_SHOTGUN]=LCSrandom(3)+1;
            cr.skill[SKILL_CLUB]=LCSrandom(2)+1;
            cr.skill[SKILL_HANDTOHAND]=LCSrandom(2)+1;
            cr.skill[SKILL_DRIVING]=LCSrandom(2)+1;
            cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3)+1;
			   
            cr.att[ATTRIBUTE_STRENGTH]=3;
			   cr.att[ATTRIBUTE_AGILITY]=3;
			   cr.att[ATTRIBUTE_HEALTH]=3;
            cr.att[ATTRIBUTE_WISDOM]=4;
         }
         break;
      case CREATURE_SWAT:
         GIVE_GENDER_MALE;         if(LCSrandom(3))
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.clip[CLIP_SMG]=3;
            cr.weapon.ammo=15;
         }
         else if(!LCSrandom(2))
         {
            cr.weapon.type=WEAPON_SHOTGUN_PUMP;
            cr.clip[CLIP_BUCKSHOT]=3;
            cr.weapon.ammo=6;
         }
         else
         {
            cr.weapon.type=WEAPON_CARBINE_M4;
            cr.clip[CLIP_ASSAULT]=3;
            cr.weapon.ammo=30;
         }
         cr.armor.type=ARMOR_SWATARMOR;
         cr.align=-1;
         cr.infiltration=0.3f + 0.1f*LCSrandom(4);
         cr.juice=40+LCSrandom(50);
         cr.age=AGE_MATURE;

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+1;
         cr.skill[SKILL_PISTOL]=LCSrandom(4)+1;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(4)+1;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(2)+1;
         cr.skill[SKILL_DRIVING]=LCSrandom(2)+1;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(4)+1;

         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=4;
         break;
      case CREATURE_DEATHSQUAD:
         GIVE_GENDER_MALE;         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_DEATHSQUADUNIFORM;
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=90+LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+2;
         cr.skill[SKILL_PISTOL]=LCSrandom(2)+1;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(2)+1;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(2)+1;
         cr.skill[SKILL_DRIVING]=LCSrandom(2)+1;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(4)+2;
			
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=6;
         break;
      case CREATURE_FIREFIGHTER:
         GIVE_GENDER_MALE;         if(law[LAW_FREESPEECH]==-2)
         {
            cr.weapon.type=WEAPON_FLAMETHROWER;
            cr.weapon.ammo=2;
            cr.clip[CLIP_GASOLINE]=3;
            cr.skill[SKILL_FLAMETHROWER]=LCSrandom(3)+2;
            strcpy(cr.name,"Fireman");
            cr.align=-1;
         } else
         {
            cr.weapon.type=WEAPON_AXE;
            cr.skill[SKILL_AXE]=LCSrandom(3)+2;
            strcpy(cr.name,"Firefighter");
         }
         
         if(sitealarm)
         {
            // Respond to emergencies in bunker gear
            cr.armor.type=ARMOR_BUNKERGEAR;
         }
         else
         {
            // Other situations have various clothes
            switch(LCSrandom(3))
            {
            case 0:
               cr.armor.type=ARMOR_OVERALLS;
               break;
            case 1:
               cr.armor.type=ARMOR_WORKCLOTHES;
               break;
            case 2:
               cr.armor.type=ARMOR_BUNKERGEAR;
            }
         }

         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;
         
         cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_AGILITY]=3;
         cr.att[ATTRIBUTE_STRENGTH]=3;
         break;
      case CREATURE_CCS_MOLOTOV:
      {
         GIVE_GENDER_MALE;         cr.armor.type=ARMOR_TRENCHCOAT;
         cr.weapon.type=WEAPON_MOLOTOV;
         cr.weapon.ammo=1;
         cr.clip[CLIP_MOLOTOV]=4;

         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
         {
            nameCCSMember(cr);
         }

         cr.skill[SKILL_HANDTOHAND]=LCSrandom(4)+2;
         cr.skill[SKILL_THROWING]=LCSrandom(4)+2;
         cr.skill[SKILL_DRIVING]=LCSrandom(4)+2;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3);
         cr.skill[SKILL_BUSINESS]=LCSrandom(3);
         cr.skill[SKILL_RELIGION]=LCSrandom(3)+1;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         cr.att[ATTRIBUTE_STRENGTH]=6;
			cr.att[ATTRIBUTE_AGILITY]=6;
			cr.att[ATTRIBUTE_HEALTH]=6;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      }
      case CREATURE_CCS_SNIPER:
      {
         GIVE_GENDER_MALE;         cr.armor.type=ARMOR_TRENCHCOAT;
         cr.weapon.type=WEAPON_SEMIRIFLE_AR15;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;

         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
         {
            nameCCSMember(cr);
         }

         cr.skill[SKILL_HANDTOHAND]=LCSrandom(4)+2;
         cr.skill[SKILL_RIFLE]=LCSrandom(4)+6;
         cr.skill[SKILL_DRIVING]=LCSrandom(4)+2;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3);
         cr.skill[SKILL_BUSINESS]=LCSrandom(3);
         cr.skill[SKILL_RELIGION]=LCSrandom(3)+1;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         cr.att[ATTRIBUTE_STRENGTH]=6;
			cr.att[ATTRIBUTE_AGILITY]=6;
			cr.att[ATTRIBUTE_HEALTH]=6;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      }
      case CREATURE_CCS_VIGILANTE:
      {
         GIVE_GENDER_MALE;         cr.armor.type=ARMOR_CLOTHES;
         switch(LCSrandom(5)+endgamestate)
         {
         case 0:
         case 1:
            break;
         case 2:
            cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
            cr.clip[CLIP_9]=6;
            cr.weapon.ammo=15;
            break;
         case 3:
            cr.weapon.type=WEAPON_REVOLVER_44;
            cr.weapon.ammo=6;
            cr.clip[CLIP_44]=6;
            break;
         case 4:
            cr.weapon.type=WEAPON_SHOTGUN_PUMP;
            cr.clip[CLIP_BUCKSHOT]=6;
            cr.weapon.ammo=6;
            break;
         case 5:
            cr.weapon.type=WEAPON_SEMIRIFLE_AR15;
            cr.clip[CLIP_ASSAULT]=6;
            cr.weapon.ammo=30;
            cr.armor.type=ARMOR_CIVILLIANARMOR;
            break;
         case 6:
            cr.weapon.type=WEAPON_SEMIRIFLE_AR15;
            cr.clip[CLIP_ASSAULT]=6;
            cr.weapon.ammo=30;
            cr.armor.type=ARMOR_ARMYARMOR;
            break;
         case 7:
            cr.weapon.type=WEAPON_AUTORIFLE_M16;
            cr.clip[CLIP_ASSAULT]=6;
            cr.weapon.ammo=30;
            cr.armor.type=ARMOR_ARMYARMOR;
            break;
         }
         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
         {
            nameCCSMember(cr);
         }
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+3;
         cr.skill[SKILL_PISTOL]=LCSrandom(4)+3;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(4)+3;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(4)+2;
         cr.skill[SKILL_DRIVING]=LCSrandom(4)+2;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(3);
         cr.skill[SKILL_BUSINESS]=LCSrandom(3);
         cr.skill[SKILL_RELIGION]=LCSrandom(3)+1;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         cr.att[ATTRIBUTE_STRENGTH]=4;
			cr.att[ATTRIBUTE_AGILITY]=4;
			cr.att[ATTRIBUTE_HEALTH]=4;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      }
      case CREATURE_CCS_ARCHCONSERVATIVE:
         GIVE_GENDER_MALE;         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=9;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_HEAVYARMOR;
         cr.money=LCSrandom(51)+150;
         cr.align=-1;
         cr.infiltration=0.9f + 0.01f*LCSrandom(11);
         cr.juice=500+LCSrandom(250);
         cr.age=AGE_MIDDLEAGED;

         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
         {
            nameCCSMember(cr);
         }

         cr.skill[SKILL_RIFLE]=LCSrandom(4)+6;
         cr.skill[SKILL_PISTOL]=LCSrandom(4)+6;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(4)+6;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(4)+4;
         cr.skill[SKILL_DRIVING]=LCSrandom(4)+4;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(6);
         cr.skill[SKILL_BUSINESS]=LCSrandom(6);
         cr.skill[SKILL_RELIGION]=LCSrandom(6)+2;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=25;
         }
         attcap[ATTRIBUTE_HEART]=1;
         cr.att[ATTRIBUTE_STRENGTH]=4;
			cr.att[ATTRIBUTE_AGILITY]=4;
			cr.att[ATTRIBUTE_HEALTH]=4;
         cr.att[ATTRIBUTE_WISDOM]=10;
         break;
      case CREATURE_GANGUNIT:
         GIVE_GENDER_MALE;
         if(!LCSrandom(3))
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.clip[CLIP_SMG]=3;
            cr.weapon.ammo=15;
         }
         else if(LCSrandom(2))
         {
            cr.weapon.type=WEAPON_SHOTGUN_PUMP;
            cr.clip[CLIP_BUCKSHOT]=3;
            cr.weapon.ammo=6;
         }
         else
         {
            cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
            cr.clip[CLIP_9]=3;
            cr.weapon.ammo=15;
         }
         cr.armor.type=ARMOR_POLICEARMOR;
         cr.align=-1;
         cr.infiltration=0.3f + 0.1f*LCSrandom(4);
         cr.juice=40+LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         cr.skill[SKILL_RIFLE]=LCSrandom(3)+1;
         cr.skill[SKILL_PISTOL]=LCSrandom(4)+1;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(3)+1;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(2)+1;
         cr.skill[SKILL_DRIVING]=LCSrandom(2)+1;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(4)+2;
			
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=5;
         break;
      case CREATURE_PRISONGUARD:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Prison Guard");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.weapon.ammo=15;
            cr.clip[CLIP_SMG]=3;
         }
         else if(!LCSrandom(3))
         {
            cr.weapon.type=WEAPON_SHOTGUN_PUMP;
            cr.clip[CLIP_BUCKSHOT]=3;
            cr.weapon.ammo=6;
         }
         else cr.weapon.type=WEAPON_NIGHTSTICK;
         cr.armor.type=ARMOR_PRISONGUARD;
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PISTOL]=LCSrandom(2)+1;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(3)+1;
         cr.skill[SKILL_CLUB]=LCSrandom(3)+2;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(2)+1;
         cr.skill[SKILL_DRIVING]=LCSrandom(2);
			
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         break;
      case CREATURE_EDUCATOR:
         strcpy(cr.name,"Educator");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.weapon.ammo=15;
            cr.clip[CLIP_SMG]=3;
         }
         else if(!LCSrandom(3))
         {
            cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
            cr.clip[CLIP_9]=3;
            cr.weapon.ammo=15;
         }
         else cr.weapon.type=WEAPON_SYRINGE;
         cr.armor.type=ARMOR_LABCOAT;
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;

         cr.skill[SKILL_RIFLE]=LCSrandom(3)+1;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(3)+1;
         cr.skill[SKILL_CLUB]=LCSrandom(2)+1;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(2)+1;
         cr.skill[SKILL_DRIVING]=LCSrandom(2);
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(4)+3;
			
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=4;
         break;
      case CREATURE_AGENT:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Agent");
         switch(LCSrandom(7))
         {
            case 0:
               cr.weapon.type=WEAPON_REVOLVER_44;
               cr.clip[CLIP_44]=3;
               cr.weapon.ammo=6;
               break;
            case 1:
               cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
               cr.clip[CLIP_9]=3;
               cr.weapon.ammo=15;
               break;
            case 2:
               cr.weapon.type=WEAPON_AUTORIFLE_M16;
               cr.clip[CLIP_ASSAULT]=3;
               cr.weapon.ammo=30;
               break;
            case 3:
               cr.weapon.type=WEAPON_AUTORIFLE_AK47;
               cr.clip[CLIP_ASSAULT]=3;
               cr.weapon.ammo=30;
               break;
            case 4:
               cr.weapon.type=WEAPON_SHOTGUN_PUMP;
               cr.clip[CLIP_BUCKSHOT]=3;
               cr.weapon.ammo=6;
               break;
            case 5:
               cr.weapon.type=WEAPON_CARBINE_M4;
               cr.clip[CLIP_ASSAULT]=3;
               cr.weapon.ammo=30;
               break;
            case 6:
               cr.weapon.type=WEAPON_SMG_MP5;
               cr.clip[CLIP_SMG]=3;
               cr.weapon.ammo=15;
               break;
         }
         cr.armor.type=ARMOR_BLACKSUIT;
         cr.align=-1;
         cr.infiltration=0.5f*LCSrandom(4);
         cr.juice=200+LCSrandom(150);
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PISTOL]=LCSrandom(8)+2;
         cr.skill[SKILL_RIFLE]=LCSrandom(8)+2;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(8)+2;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(8)+2;
         cr.skill[SKILL_CLUB]=LCSrandom(8)+2;
         cr.skill[SKILL_DRIVING]=LCSrandom(6)+4;
         cr.skill[SKILL_PSYCHOLOGY]=LCSrandom(6)+4;
         cr.skill[SKILL_DODGE]=LCSrandom(4)+4;
         
         cr.att[ATTRIBUTE_STRENGTH]=5;
         cr.att[ATTRIBUTE_AGILITY]=7;
         cr.att[ATTRIBUTE_HEALTH]=5;
         cr.att[ATTRIBUTE_INTELLIGENCE]=4;
         cr.att[ATTRIBUTE_WISDOM]=5;
         break;
      case CREATURE_RADIOPERSONALITY:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Radio Personality");
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(51)+50;
         cr.align=-1;
         cr.infiltration=0.6f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PERSUASION]=LCSrandom(8)+1;
         
         cr.att[ATTRIBUTE_CHARISMA]=10;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      case CREATURE_NEWSANCHOR:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"News Anchor");
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(51)+50;
         
         cr.att[ATTRIBUTE_CHARISMA]=10;
         cr.att[ATTRIBUTE_WISDOM]=8;
         cr.align=-1;
         cr.infiltration=0.6f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GENETIC:
         if(location[cursite]->type==SITE_CORPORATE_HOUSE)
            strcpy(cr.name,"Pet ");
         else
            strcpy(cr.name,"");

         switch(LCSrandom(10))
         {
            case 0:strcat(cr.name,"Genetic Monster");break;
            case 1:
            {
               strcat(cr.name,"Flaming Rabbit");
               cr.specialattack=ATTACK_FLAME;
               break;
            }
            case 2:strcat(cr.name,"Genetic Nightmare");break;
            case 3:strcat(cr.name,"Mad Cow");break;
            case 4:
            {
               strcat(cr.name,"Giant Mosquito");
               cr.specialattack=ATTACK_SUCK;
               break;
            }
            case 5:strcat(cr.name,"Six-legged Pig");break;
            case 6:strcat(cr.name,"Purple Gorilla");break;
            case 7:strcat(cr.name,"Warped Bear");break;
            case 8:strcat(cr.name,"Writhing Mass");break;
            case 9:strcat(cr.name,"Something Bad");break;
         }
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.armor.type=ARMOR_NONE;
         cr.align=-1;
         if(location[cursite]->type==SITE_CORPORATE_HOUSE)
            attcap[ATTRIBUTE_CHARISMA]=10;
         else
            attcap[ATTRIBUTE_CHARISMA]=2;
         attcap[ATTRIBUTE_INTELLIGENCE]=1;
         attcap[ATTRIBUTE_HEART]=6;
         attcap[ATTRIBUTE_WISDOM]=6;
         attcap[ATTRIBUTE_STRENGTH]=20;
         attcap[ATTRIBUTE_AGILITY]=20;
         cr.age=AGE_DOGYEARS;
         break;
      case CREATURE_GUARDDOG:
         strcpy(cr.name,"Guard Dog");
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.armor.type=ARMOR_NONE;
         cr.align=-1;
         attcap[ATTRIBUTE_INTELLIGENCE]=1;
         attcap[ATTRIBUTE_HEART]=6;
         attcap[ATTRIBUTE_WISDOM]=6;
         cr.att[ATTRIBUTE_STRENGTH]=7;attcap[ATTRIBUTE_STRENGTH]=12;
         cr.att[ATTRIBUTE_AGILITY]=9;attcap[ATTRIBUTE_AGILITY]=15;
         cr.age=AGE_DOGYEARS;
         break;
      case CREATURE_PRISONER:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Prisoner");
         if(!LCSrandom(2))cr.weapon.type=WEAPON_SHANK;
         cr.armor.type=ARMOR_PRISONER;
         cr.money=0;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);

         // Prisoners should not be "prisoners" after recruiting them -- they should
         // be some brand of criminal
         if(!LCSrandom(10))
         {
            // Thief
            cr.skill[SKILL_SECURITY]=LCSrandom(5)+3;
            cr.skill[SKILL_DISGUISE]=LCSrandom(5)+3;
            cr.skill[SKILL_STEALTH]=LCSrandom(5)+3;
            cr.skill[SKILL_THEFT]=LCSrandom(5)+3;
            cr.type=CREATURE_THIEF;
            cr.age=AGE_MATURE;
         }
         else
         {
            switch(LCSrandom(5))
            {
            case 0:
               // Gang member
               cr.skill[SKILL_PISTOL]=LCSrandom(2)+1;
               cr.skill[SKILL_SHOTGUN]=LCSrandom(2)+1;
               cr.skill[SKILL_RIFLE]=LCSrandom(2)+1;
               cr.type=CREATURE_GANGMEMBER;
               cr.age=AGE_YOUNGADULT;
               break;
            case 1:
               // Prostitute
		         cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
               cr.skill[SKILL_SEDUCTION]=LCSrandom(4)+2;
               cr.type=CREATURE_PROSTITUTE;
               cr.age=AGE_YOUNGADULT;
               break;
            case 2:
               // Crack head
               attcap[ATTRIBUTE_INTELLIGENCE]=1;
               attcap[ATTRIBUTE_HEALTH]=1+LCSrandom(5);
               attnum-=10;
               cr.type=CREATURE_CRACKHEAD;
               cr.age=AGE_YOUNGADULT;
               break;
            case 3:
               // Teenager
               cr.age=AGE_TEENAGER;
               cr.type=CREATURE_TEENAGER;
               break;
            case 4:
               // HS Dropout
               cr.age=AGE_TEENAGER;
               cr.type=CREATURE_HSDROPOUT;
               break;
            }
         }
         break;
      case CREATURE_JUROR:
         strcpy(cr.name,"Angry Juror");
         GIVE_WEAPON_CIVILIAN;
         if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=0;
         cr.align=-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_FACTORY_CHILD:
         strcpy(cr.name,"Child Worker");
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=0;
         cr.align=1;
         cr.age=AGE_CHILD;
         
         cr.att[ATTRIBUTE_HEART]=8;
         break;
      case CREATURE_SEWERWORKER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Sewer Worker");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_COLLEGESTUDENT:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"College Student");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=18+LCSrandom(6); // no macro

			cr.skill[SKILL_COMPUTERS]=LCSrandom(2);
			cr.skill[SKILL_WRITING]=LCSrandom(2)+1;
         cr.skill[SKILL_SCIENCE]=LCSrandom(3);
         cr.skill[SKILL_ART]=LCSrandom(3);
         cr.skill[SKILL_MUSIC]=LCSrandom(3);
         break;
      case CREATURE_MUSICIAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Musician");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(11);
         cr.age=AGE_MATURE;
         cr.skill[SKILL_MUSIC]=LCSrandom(4)+2;
         cr.skill[SKILL_SEDUCTION]=LCSrandom(2);
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_MATHEMATICIAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Mathematician");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.skill[SKILL_COMPUTERS]=LCSrandom(3);
         cr.skill[SKILL_SCIENCE]=LCSrandom(3)+1;
         
         cr.att[ATTRIBUTE_INTELLIGENCE]=7;
         break;
      case CREATURE_TEACHER:
         GIVE_GENDER_FEMALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Teacher");
         cr.armor.type=ARMOR_CLOTHES;
         cr.skill[SKILL_TEACHING]=LCSrandom(4)+3;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_HSDROPOUT:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Highschool Dropout");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_TEENAGER;
         break;
      case CREATURE_BUM:
         strcpy(cr.name,"Transient");
         GIVE_WEAPON_CIVILIAN;
         if(cr.weapon.type==WEAPON_NONE && !LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_MUTANT:
         strcpy(cr.name,"Mutant");
         GIVE_WEAPON_CIVILIAN;
         if(cr.weapon.type==WEAPON_NONE && !LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.align=1;
         //cr.skill[SKILL_SURVIVAL]=LCSrandom(3)+1;
         for(a=0;a<ATTNUM;a++)
         {
            attcap[a]=50;
         }
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GANGMEMBER:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Gang Member");

         if(!LCSrandom(20) || (law[LAW_GUNCONTROL]==-2 && !LCSrandom(5)))
         {
            cr.weapon.type=WEAPON_AUTORIFLE_AK47;
            cr.weapon.ammo=30;
            cr.clip[CLIP_ASSAULT]=2;
         }
         else if(!LCSrandom(16) || (law[LAW_GUNCONTROL]==-2 && !LCSrandom(5)))
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.weapon.ammo=15;
            cr.clip[CLIP_SMG]=3;
         }
         else if(!LCSrandom(15))
         {
            cr.weapon.type=WEAPON_SEMIPISTOL_45;
            cr.weapon.ammo=15;
            cr.clip[CLIP_45]=3;
         }
         else if(!LCSrandom(10))
         {
            cr.weapon.type=WEAPON_SHOTGUN_PUMP;
            cr.weapon.ammo=6;
            cr.clip[CLIP_BUCKSHOT]=3;
         }
         else if(!LCSrandom(4))
         {
            cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
            cr.weapon.ammo=15;
            cr.clip[CLIP_9]=3;
         }
         else if(!LCSrandom(2))
         {
            cr.weapon.type=WEAPON_REVOLVER_38;
            cr.weapon.ammo=6;
            cr.clip[CLIP_38]=3;
         }

         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_YOUNGADULT;

         cr.skill[SKILL_PISTOL]=LCSrandom(2)+1;
         cr.skill[SKILL_SHOTGUN]=LCSrandom(2)+1;
         cr.skill[SKILL_RIFLE]=LCSrandom(2)+1;

         if(!LCSrandom(2))
         {
            switch(LCSrandom(3))
            {
            case 0:cr.lawflag[LAWFLAG_BROWNIES]++;
            case 1:cr.lawflag[LAWFLAG_ASSAULT]++;
            case 2:cr.lawflag[LAWFLAG_MURDER]++;
            }
         }
         break;
      case CREATURE_CRACKHEAD:
         strcpy(cr.name,"Crack Head");
         GIVE_WEAPON_CIVILIAN;
         if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.juice=-20;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_YOUNGADULT;

         attcap[ATTRIBUTE_INTELLIGENCE]=1;
         attcap[ATTRIBUTE_HEALTH]=1+LCSrandom(5);
         attnum-=10;
         break;
      case CREATURE_PRIEST:
         GIVE_GENDER_MALE;         strcpy(cr.name,"Priest");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_RELIGION]=LCSrandom(5)+3;
         break;
      case CREATURE_ENGINEER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Engineer");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.skill[SKILL_COMPUTERS]=LCSrandom(2)+1;
         cr.skill[SKILL_SCIENCE]=LCSrandom(3)+1;
         break;
      case CREATURE_FASTFOODWORKER:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Fast Food Worker");
         cr.armor.type=ARMOR_CLOTHES;
         cr.skill[SKILL_COOKING]=LCSrandom(3);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         if(LCSrandom(2))
            cr.age=AGE_TEENAGER;
         else
            cr.age=AGE_YOUNGADULT;
         break;
      case CREATURE_TELEMARKETER:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Telemarketer");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_YOUNGADULT;

			cr.skill[SKILL_PERSUASION]=LCSrandom(3)+1;
         break;
      case CREATURE_OFFICEWORKER:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Office Worker");
         cr.armor.type=ARMOR_CLOTHES;
         cr.skill[SKILL_BUSINESS]=LCSrandom(3);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_FOOTBALLCOACH:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Football Coach");
         cr.armor.type=ARMOR_CLOTHES;
         cr.skill[SKILL_TEACHING]=LCSrandom(3)+1;
         cr.skill[SKILL_LEADERSHIP]=LCSrandom(3)+1;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MIDDLEAGED;

         if(LCSrandom(2))
         {
            
            cr.att[ATTRIBUTE_HEALTH]=5;
            cr.att[ATTRIBUTE_AGILITY]=5;
            cr.att[ATTRIBUTE_STRENGTH]=5;
         }
			cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         break;
      case CREATURE_PROSTITUTE:
         GIVE_WEAPON_CIVILIAN;
         if(LCSrandom(7))            cr.gender_conservative=cr.gender_liberal=GENDER_FEMALE;         else if(!LCSrandom(3))            cr.gender_liberal=GENDER_FEMALE;         strcpy(cr.name,"Prostitute");
         if(LCSrandom(2))cr.armor.type=ARMOR_CHEAPDRESS;
         else cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_YOUNGADULT;
			cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         cr.skill[SKILL_SEDUCTION]=LCSrandom(4)+2;

         if(!LCSrandom(3))
         {
            cr.lawflag[LAWFLAG_PROSTITUTION]++;
         }
         break;
      case CREATURE_MAILMAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Mail Carrier");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GARBAGEMAN:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Garbage Collector");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_DRIVING]=LCSrandom(2)+1;
         break;
      case CREATURE_PLUMBER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Plumber");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CHEF:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Chef");
         cr.skill[SKILL_COOKING]=LCSrandom(5)+3;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CONSTRUCTIONWORKER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Construction Worker");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         
         cr.att[ATTRIBUTE_STRENGTH]=7;
         cr.att[ATTRIBUTE_HEALTH]=7;
         cr.skill[SKILL_DRIVING]=LCSrandom(2)+1;
         break;
      case CREATURE_AMATEURMAGICIAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Amateur Magician");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_THEFT]=LCSrandom(3)+2;
         break;
      case CREATURE_HIPPIE:
         strcpy(cr.name,"Hippie");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.align=1;
         
         cr.att[ATTRIBUTE_HEART]=6;
         attcap[ATTRIBUTE_HEART]=15;         attcap[ATTRIBUTE_WISDOM]=1;         attnum-=5;         cr.skill[SKILL_COOKING]=LCSrandom(2);
         cr.skill[SKILL_MUSIC]=LCSrandom(2);
         cr.skill[SKILL_ART]=LCSrandom(2);
         cr.skill[SKILL_TAILORING]=LCSrandom(2);

         if(!LCSrandom(10))
         {
            cr.lawflag[LAWFLAG_BROWNIES]++;
         }
         break;
      case CREATURE_AUTHOR:
         GIVE_WEAPON_CIVILIAN;
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PERSUASION]=LCSrandom(4);
         cr.skill[SKILL_WRITING]=LCSrandom(4)+4;
         break;
      case CREATURE_JOURNALIST:
         GIVE_WEAPON_CIVILIAN;
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.juice=LCSrandom(25);
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         cr.skill[SKILL_WRITING]=LCSrandom(4)+2;
         break;
      case CREATURE_CRITIC_ART:
         GIVE_WEAPON_CIVILIAN;
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         cr.skill[SKILL_WRITING]=LCSrandom(4)+2;
         cr.skill[SKILL_ART]=LCSrandom(4)+1;
         break;
      case CREATURE_CRITIC_MUSIC:
         GIVE_WEAPON_CIVILIAN;
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         cr.skill[SKILL_WRITING]=LCSrandom(4)+2;
         cr.skill[SKILL_MUSIC]=LCSrandom(4)+1;
         break;
      case CREATURE_SOCIALITE:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Socialite");
         if(LCSrandom(2))cr.armor.type=ARMOR_EXPENSIVEDRESS;
         else cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.skill[SKILL_PERSUASION]=LCSrandom(3)+1;
         cr.skill[SKILL_ART]=LCSrandom(3);
         cr.skill[SKILL_MUSIC]=LCSrandom(3);
         break;
      case CREATURE_BIKER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Biker");
         cr.armor.type=ARMOR_TRENCHCOAT;
         cr.money=LCSrandom(31)+20;
         cr.align=-1;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_DRIVING]=LCSrandom(3)+3;
         break;
      case CREATURE_TRUCKER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Truck Driver");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_DRIVING]=LCSrandom(3)+3;
         break;
      case CREATURE_TAXIDRIVER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Taxi Driver");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.skill[SKILL_DRIVING]=LCSrandom(3)+3;
         break;
      case CREATURE_PROGRAMMER:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Programmer");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.skill[SKILL_COMPUTERS]=LCSrandom(5)+4;
         
         cr.att[ATTRIBUTE_INTELLIGENCE]=3;
         break;
      case CREATURE_NUN:
         cr.gender_conservative=cr.gender_liberal=GENDER_FEMALE;
         cr.skill[SKILL_RELIGION]=LCSrandom(3)+5;         cr.age=AGE_MATURE;
         break;
      case CREATURE_RETIREE:
         GIVE_WEAPON_CIVILIAN;
         cr.age=AGE_SENIOR;
         break;
      case CREATURE_PAINTER:
         GIVE_WEAPON_CIVILIAN;
         cr.skill[SKILL_ART]=LCSrandom(4)+3;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_SCULPTOR:
         GIVE_WEAPON_CIVILIAN;
         cr.skill[SKILL_ART]=LCSrandom(4)+3;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_DANCER:
         cr.att[ATTRIBUTE_AGILITY]=10;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         cr.att[ATTRIBUTE_HEALTH]=10;
         cr.skill[SKILL_ART]=LCSrandom(2)+1;
         cr.skill[SKILL_MUSIC]=LCSrandom(2)+1;
         cr.age=AGE_YOUNGADULT;
         break;
      case CREATURE_PHOTOGRAPHER:
         GIVE_WEAPON_CIVILIAN;
         cr.skill[SKILL_ART]=LCSrandom(2)+2;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CAMERAMAN:
         GIVE_WEAPON_CIVILIAN;
         cr.skill[SKILL_ART]=LCSrandom(2)+1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_HAIRSTYLIST:
         GIVE_GENDER_FEMALE;         GIVE_WEAPON_CIVILIAN;
         cr.skill[SKILL_ART]=LCSrandom(2)+1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_FASHIONDESIGNER:
         GIVE_GENDER_FEMALE;         GIVE_WEAPON_CIVILIAN;
         cr.skill[SKILL_TAILORING]=LCSrandom(3)+8;
         cr.skill[SKILL_ART]=LCSrandom(3)+2;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CLERK:
         GIVE_WEAPON_CIVILIAN;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_THIEF:
         GIVE_WEAPON_CIVILIAN;
         switch(LCSrandom(5))
         {
            case 0:getrecruitcreature(cr.name,CREATURE_SOCIALITE);break;
            case 1:getrecruitcreature(cr.name,CREATURE_CLERK);break;
            case 2:getrecruitcreature(cr.name,CREATURE_OFFICEWORKER);break;
            case 3:getrecruitcreature(cr.name,CREATURE_CRITIC_ART);break;
            case 4:getrecruitcreature(cr.name,CREATURE_CRITIC_MUSIC);break;
         }
         cr.skill[SKILL_SECURITY]=LCSrandom(5)+3;
         cr.skill[SKILL_DISGUISE]=LCSrandom(5)+3;
         cr.skill[SKILL_STEALTH]=LCSrandom(5)+3;
         cr.skill[SKILL_THEFT]=LCSrandom(5)+3;
         cr.age=AGE_MATURE;

         if(!LCSrandom(10))
         {
            switch(LCSrandom(2))
            {
            case 0:cr.lawflag[LAWFLAG_BREAKING]++;
            case 1:cr.lawflag[LAWFLAG_THEFT]++;
            }
         }
         break;
      case CREATURE_ACTOR:
         GIVE_WEAPON_CIVILIAN;
         cr.skill[SKILL_PERSUASION]=LCSrandom(5);
         cr.skill[SKILL_SEDUCTION]=LCSrandom(5);
         cr.skill[SKILL_DISGUISE]=LCSrandom(5)+3;
         
         cr.att[ATTRIBUTE_WISDOM]=9;
			cr.att[ATTRIBUTE_CHARISMA]=5;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_YOGAINSTRUCTOR:
         GIVE_GENDER_FEMALE;
         cr.skill[SKILL_TEACHING]=LCSrandom(4)+2;
         cr.att[ATTRIBUTE_AGILITY]=10;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         cr.att[ATTRIBUTE_HEALTH]=10;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_MARTIALARTIST:
         GIVE_GENDER_MALE;
         cr.skill[SKILL_TEACHING]=LCSrandom(3)+1;
         cr.skill[SKILL_HANDTOHAND]=LCSrandom(4)+4;
         cr.att[ATTRIBUTE_AGILITY]=8;
         cr.att[ATTRIBUTE_STRENGTH]=6;
         cr.att[ATTRIBUTE_HEALTH]=6;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_ATHLETE:
         GIVE_GENDER_MALE;         GIVE_WEAPON_CIVILIAN;
         cr.att[ATTRIBUTE_AGILITY]=8;
         cr.att[ATTRIBUTE_STRENGTH]=8;
         cr.att[ATTRIBUTE_HEALTH]=8;
         cr.age=AGE_YOUNGADULT;
         break;
   }

   if(cr.type==CREATURE_MUTANT)attnum=LCSrandom(60)+7;
   for(int a=0;a<ATTNUM;a++)
   {
      attnum-=min(4,cr.att[a]);
   }
   while(attnum>0)
   {
      int a=LCSrandom(ATTNUM);
      if(a==ATTRIBUTE_WISDOM && cr.align==1 && LCSrandom(2))
         a=ATTRIBUTE_HEART;
      if(a==ATTRIBUTE_HEART && cr.align==-1 && LCSrandom(2))
         a=ATTRIBUTE_WISDOM;
      if(cr.att[a]<attcap[a])
      {
         cr.att[a]++;
         attnum--;
      }
   }

   if(cr.align==1)
   {
      cr.infiltration = 0.15f+LCSrandom(10)-5;
   }
   else if(cr.align==0)
   {
      cr.infiltration = 0.25f+LCSrandom(10)-5;
   }
   else
   {
      cr.infiltration += 0.35f * (1-cr.infiltration)+LCSrandom(10)-5;
   }
   if(cr.infiltration < 0) cr.infiltration = 0;
   if(cr.infiltration > 1) cr.infiltration = 1;

   int randomskills=LCSrandom(4)+4;

   if(cr.age>20)
   {
      randomskills+=static_cast<int>(randomskills*((cr.age-20.0)/20.0));
   }
   else
   {
      randomskills-=(20-cr.age)/2;
   }

   /*for(int s=0;s<SKILLNUM;s++)
   {
      randomskills-=cr.skill[s];
   }*/

   //RANDOM STARTING SKILLS
   while(randomskills>0)
   {
      int randomskill = LCSrandom(SKILLNUM);
      // 95% chance of not allowing some skills...
      if(LCSrandom(20))
      {
         if(randomskill == SKILL_FLAMETHROWER)continue;
         if(randomskill == SKILL_SMG)continue;
         if(randomskill == SKILL_SWORD)continue;
         if(randomskill == SKILL_RIFLE)continue;
         if(randomskill == SKILL_AXE)continue;
         if(randomskill == SKILL_CLUB)continue;
         if(randomskill == SKILL_PSYCHOLOGY)continue;
      }
      // 90% chance of not allowing some skills, other than      //   for conservatives      if(LCSrandom(10) && cr.align!=ALIGN_CONSERVATIVE)      {         if(randomskill == SKILL_SHOTGUN)continue;         if(randomskill == SKILL_PISTOL)continue;      }      if(maxskill(randomskill,cr)>cr.skill[randomskill])
      {
         cr.skill[randomskill]++;
         randomskills--;
      }

      while(1)
      {
         if(randomskills&&LCSrandom(2) &&
            maxskill(randomskill,cr)>cr.skill[randomskill] &&
            cr.skill[randomskill] < 4)         {
            cr.skill[randomskill]++;
            randomskills--;
         }
         else break;
      }
   }

   //ALIENATION
   if(sitealienate>=1&&cr.align==0)conservatise(cr);
   if(sitealienate==2&&cr.align==1)conservatise(cr);
}