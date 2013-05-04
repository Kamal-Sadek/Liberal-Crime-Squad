
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
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]); \
            cr.give_weapon(w,NULL);                            \
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],4);  \
            cr.take_clips(c,4);                                \
            cr.reload(false);                                  \
         } else if(law[LAW_GUNCONTROL]==-2) {                  \
            if(!LCSrandom(10)) {                               \
               Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]); \
               cr.give_weapon(w,NULL);                         \
               Clip c=Clip(*cliptype[getcliptype("CLIP_9")],4);\
               cr.take_clips(c,4);                             \
               cr.reload(false);                               \
            } else if(!LCSrandom(9)) {                         \
               Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_45")]); \
               cr.give_weapon(w,NULL);                         \
               Clip c=Clip(*cliptype[getcliptype("CLIP_45")],4); \
               cr.take_clips(c,4);                             \
               cr.reload(false);                               \
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
void makecreature(Creature &cr,short type) //Lots of temporary solution in this function. -XML
{
   cr.drop_weapons_and_clips(NULL); // Get rid of any old equipment from old encounters.
   cr.strip(NULL);                  //

   Weapon* weapon = NULL;
   Armor* armor = NULL;
   Clip* clips = NULL;
   
   int a = 0;
   cr.creatureinit();

   cr.exists=1;
   cr.squadid=-1;
   cr.type=type;
   cr.infiltration=0;
   getrecruitcreature(cr.name,type);
   {
   Armor a=Armor(*armortype[getarmortype("ARMOR_CLOTHES")]);
   cr.give_armor(a,NULL);
   }
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
      cr.set_attribute(a,1);
   }
   int attnum=40;
   switch(type)
   {
      case CREATURE_BOUNCER:
         GIVE_GENDER_MALE;
         if(mode==GAMEMODE_SITE && location[cursite]->highsecurity)
         {
            strcpy(cr.name,"Enforcer");
            cr.set_skill(SKILL_CLUB,LCSrandom(3)+3);
         }

         if(law[LAW_GUNCONTROL]==-2)
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(law[LAW_GUNCONTROL]==-1)
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_44")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_44")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(law[LAW_GUNCONTROL]==0)
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(3)+1);
         if(disguisesite(sitetype))
         {
                 cr.align=-1;
                 cr.infiltration=0.1f*LCSrandom(4);
         }
         else cr.align=0;
         cr.age=AGE_MATURE;

         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_STRENGTH,4);
         break;
      case CREATURE_SECURITYGUARD:
         GIVE_GENDER_MALE;
         if(law[LAW_GUNCONTROL]==-2)
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(law[LAW_GUNCONTROL]!=2)
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_SECURITYUNIFORM")]);
         cr.give_armor(*armor,NULL);
         cr.set_skill(SKILL_PISTOL,LCSrandom(3)+1);
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(3);
         cr.age=AGE_MATURE;
         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_STRENGTH,2);
         break;
      case CREATURE_SCIENTIST_LABTECH:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         if(!cr.is_armed() && !LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SYRINGE")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_LABCOAT")]);
         cr.give_armor(*armor,NULL);
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,5);
         cr.set_skill(SKILL_COMPUTERS,LCSrandom(2)+1);
         cr.set_skill(SKILL_SCIENCE,LCSrandom(4)+3);
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Hangin' Judge");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_44")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_44")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_GAVEL")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_BLACKROBE")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(41)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         cr.set_skill(SKILL_LAW,LCSrandom(6)+5);
         cr.set_skill(SKILL_WRITING,LCSrandom(3)+1);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,5);
         cr.set_attribute(ATTRIBUTE_WISDOM,10);
         attcap[ATTRIBUTE_HEART]=1;
         break;
      case CREATURE_JUDGE_LIBERAL:
         strcpy(cr.name,"Liberal Judge");
         if(!LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_GAVEL")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_BLACKROBE")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(41)+20;
         cr.align=1;
         cr.age=AGE_MIDDLEAGED;

         cr.set_skill(SKILL_LAW,LCSrandom(6)+5);
         cr.set_skill(SKILL_WRITING,LCSrandom(3)+1);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,5);
         cr.set_attribute(ATTRIBUTE_HEART,10);
         break;
      case CREATURE_SCIENTIST_EMINENT:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         if(!cr.is_armed() && !LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SYRINGE")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_LABCOAT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(41)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         cr.set_skill(SKILL_WRITING,LCSrandom(3)+1);
         cr.set_skill(SKILL_SCIENCE,LCSrandom(6)+6);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,10);
         cr.set_attribute(ATTRIBUTE_WISDOM,6);
         break;
      case CREATURE_CORPORATE_MANAGER:
         GIVE_WEAPON_CIVILIAN;
         armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(41)+40;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(6);
         cr.age=AGE_MATURE;
         GIVE_GENDER_MALE;
         cr.set_skill(SKILL_BUSINESS,LCSrandom(4)+3);

         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,4);
         cr.set_attribute(ATTRIBUTE_CHARISMA,5);
         cr.set_attribute(ATTRIBUTE_WISDOM,5);
         break;
      case CREATURE_CORPORATE_CEO:
         GIVE_GENDER_MALE;
         //if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_DESERT_EAGLE")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_50AE")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_EXPENSIVESUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(1001)+1000;
         cr.align=-1;
         cr.infiltration=1.0f;
         cr.juice=1000;
         cr.age=AGE_MIDDLEAGED;
         generate_name(cr.propername, GENDER_WHITEMALEPATRIARCH);
         strcpy(cr.name,"CEO ");
         strcat(cr.name,cr.propername);
         cr.dontname = true;

         cr.set_skill(SKILL_BUSINESS,LCSrandom(6)+10);
         cr.set_skill(SKILL_DODGE,LCSrandom(6)+10);
         cr.set_skill(SKILL_PISTOL,LCSrandom(6)+10);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,7);
         cr.set_attribute(ATTRIBUTE_CHARISMA,7);
         cr.set_attribute(ATTRIBUTE_WISDOM,12);
         break;
      case CREATURE_WORKER_SERVANT:
         cr.money=0;
         cr.align=1;
         armor=new Armor(*armortype[getarmortype("ARMOR_SERVANTUNIFORM")]);
         cr.give_armor(*armor,NULL);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_JANITOR:
         GIVE_WEAPON_CIVILIAN;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_SWEATSHOP:
         cr.strip(NULL);
         cr.align=1;
         cr.age=AGE_MATURE;
         cr.juice=-20;
         cr.flag|=CREATUREFLAG_ILLEGALALIEN;
         cr.set_skill(SKILL_TAILORING,LCSrandom(5)+1);
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Nonunion Worker");
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_CHAIN")]);
         cr.give_weapon(*weapon,NULL);
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         if(cr.align==1)
            cr.align=LCSrandom(2)-1;
         cr.age=AGE_MATURE;
         
         cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         break;
      case CREATURE_WORKER_SECRETARY:
         GIVE_WEAPON_CIVILIAN;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_WRITING,LCSrandom(2)+1);
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,2);
         break;
      case CREATURE_LANDLORD:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Landlord");
         cr.money=LCSrandom(121)+120;
         cr.align=0;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_BUSINESS,LCSrandom(4)+3);
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,4);
         cr.set_attribute(ATTRIBUTE_WISDOM,4);
         break;
      case CREATURE_BANK_TELLER:
         strcpy(cr.name,"Bank Teller");
         cr.align=0;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_BANK_MANAGER:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Bank Manager");
         cr.money=LCSrandom(121)+120;
         cr.align=ALIGN_CONSERVATIVE;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_BUSINESS,LCSrandom(4)+3);
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,4);
         cr.set_attribute(ATTRIBUTE_WISDOM,4);
         break;
      case CREATURE_TEENAGER:
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_TEENAGER;
         break;
      case CREATURE_LAWYER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],1);
            cr.take_clips(c,1);
            cr.reload(false);
         }
         strcpy(cr.name,"Lawyer");
         armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(51)+50;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         cr.set_skill(SKILL_LAW,LCSrandom(4)+4);
         cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+2);

         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,6);
         cr.set_attribute(ATTRIBUTE_CHARISMA,4);
         break;
      case CREATURE_DOCTOR:
         GIVE_GENDER_MALE;
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],1);
            cr.take_clips(c,1);
            cr.reload(false);
         }
         strcpy(cr.name,"Doctor");
         armor=new Armor(*armortype[getarmortype("ARMOR_LABCOAT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(21)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         cr.set_skill(SKILL_FIRSTAID,LCSrandom(4)+4);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,6);
         break;
      case CREATURE_PSYCHOLOGIST:
         GIVE_GENDER_MALE;
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],1);
            cr.take_clips(c,1);
            cr.reload(false);
         }
         strcpy(cr.name,"Psychologist");
         if(cr.gender_liberal==GENDER_MALE||LCSrandom(2))
         {
            Armor a=Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
            cr.give_armor(a,NULL);
         }
         else
         {
            Armor a=Armor(*armortype[getarmortype("ARMOR_CHEAPDRESS")]);
            cr.give_armor(a,NULL);
         }
         cr.money=LCSrandom(21)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(4)+4);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,6);
         cr.set_attribute(ATTRIBUTE_HEART,6);
         break;
      case CREATURE_NURSE:
         GIVE_GENDER_FEMALE;
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],1);
            cr.take_clips(c,1);
            cr.reload(false);
         }
         strcpy(cr.name,"Nurse");
         armor=new Armor(*armortype[getarmortype("ARMOR_LABCOAT")]);
         cr.give_armor(*armor,NULL);
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         cr.set_skill(SKILL_FIRSTAID,LCSrandom(4)+1);
         break;
      case CREATURE_WORKER_FACTORY_UNION:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Union Worker");
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_CHAIN")]);
         cr.give_weapon(*weapon,NULL);
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.align=1;
         cr.age=AGE_MATURE;
         
         cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         break;
      case CREATURE_TANK:
         strcpy(cr.name,"M1 Abrams Tank");
         cr.animalgloss=ANIMALGLOSS_TANK;
         cr.strip(NULL);
         cr.specialattack=ATTACK_CANNON;
         cr.align=-1;
         cr.set_skill(SKILL_HANDTOHAND,20);
         cr.age=AGE_TEENAGER; // wut?
         break;
      case CREATURE_MERC:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Elite Security");
         if(law[LAW_GUNCONTROL]<1)
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.take_clips(c,7);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIRIFLE_AR15")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.take_clips(c,7);
            cr.reload(false);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_CIVILLIANARMOR")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+2);
         cr.set_skill(SKILL_SECURITY,LCSrandom(3));
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(3));
         cr.set_skill(SKILL_PISTOL,LCSrandom(3));
         cr.set_skill(SKILL_DRIVING,LCSrandom(2));
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(2));
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
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_TORCH")]);
            cr.give_weapon(w,NULL);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_PITCHFORK")]);
            cr.give_weapon(w,NULL);
         }

         if(!LCSrandom(2))
         {
            Armor a=Armor(*armortype[getarmortype("ARMOR_OVERALLS")]);
            cr.give_armor(a,NULL);
         }
         else
         {
            Armor a=Armor(*armortype[getarmortype("ARMOR_WIFEBEATER")]);
            cr.give_armor(a,NULL);
         }
         cr.gender_conservative=cr.gender_liberal=GENDER_MALE;
         cr.money=LCSrandom(6)+6;
         cr.align=-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_SOLDIER:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Soldier");
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")]);
         cr.give_weapon(*weapon,NULL);
         clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
         cr.take_clips(*clips,7);
         cr.reload(false);
         armor=new Armor(*armortype[getarmortype("ARMOR_ARMYARMOR")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(100);
         cr.age=AGE_YOUNGADULT;

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(3)+1);
         cr.set_skill(SKILL_PISTOL,LCSrandom(3)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(3)+1);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3)+1);

         cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         cr.set_attribute(ATTRIBUTE_AGILITY,5);
         cr.set_attribute(ATTRIBUTE_HEALTH,5);
         cr.set_attribute(ATTRIBUTE_WISDOM,5);
         break;
      case CREATURE_VETERAN:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Army Veteran");
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_COMBATKNIFE")]);
         cr.give_weapon(*weapon,NULL);
         cr.money=LCSrandom(21)+20;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(100);
         cr.age=AGE_MIDDLEAGED;

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(3)+1);
         cr.set_skill(SKILL_PISTOL,LCSrandom(3)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(3)+1);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3)+1);

         cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         cr.set_attribute(ATTRIBUTE_AGILITY,5);
         cr.set_attribute(ATTRIBUTE_HEALTH,5);
         break;
      case CREATURE_HARDENED_VETERAN:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Hardened Veteran");
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")]);
         cr.give_weapon(*weapon,NULL);
         clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
         cr.take_clips(*clips,7);
         cr.reload(false);
         armor=new Armor(*armortype[getarmortype("ARMOR_ARMYARMOR")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.juice=LCSrandom(100);
         cr.age=AGE_MIDDLEAGED;

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+4);
         cr.set_skill(SKILL_SECURITY,LCSrandom(3));
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(3)+2);
         cr.set_skill(SKILL_PISTOL,LCSrandom(3)+2);
         cr.set_skill(SKILL_DRIVING,LCSrandom(3)+2);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3)+1);

         cr.set_attribute(ATTRIBUTE_STRENGTH,7);
         cr.set_attribute(ATTRIBUTE_AGILITY,7);
         cr.set_attribute(ATTRIBUTE_HEALTH,7);
         break;
      case CREATURE_COP:
         if(law[LAW_POLICEBEHAVIOR]==2 && cr.align==ALIGN_LIBERAL && !LCSrandom(3)) // Peace Officer
         {
            GIVE_GENDER_MALE;
            cr.align=ALIGN_MODERATE;
            strcpy(cr.name,"Police Negotiator");
            Armor a=Armor(*armortype[getarmortype("ARMOR_POLICEARMOR")]);
            cr.give_armor(a,NULL);
            cr.money=LCSrandom(21)+20;
            cr.juice=10+LCSrandom(50);
            cr.age=AGE_MATURE;

            cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+1);
            cr.set_skill(SKILL_PISTOL,LCSrandom(3)+1);
            cr.set_skill(SKILL_DRIVING,LCSrandom(2)+1);
            cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3)+1);
   
            cr.set_attribute(ATTRIBUTE_STRENGTH,3);
            cr.set_attribute(ATTRIBUTE_AGILITY,3);
            cr.set_attribute(ATTRIBUTE_HEALTH,3);
            cr.set_attribute(ATTRIBUTE_HEART,4);
         }
         else
         {
            GIVE_GENDER_MALE;
            if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
            {
               Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
               cr.give_weapon(w,NULL);
               Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],4);
               cr.take_clips(c,4);
               cr.reload(false);
            }
            else if(!LCSrandom(3))
            {
               Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
               cr.give_weapon(w,NULL);
               Clip c=Clip(*cliptype[getcliptype("CLIP_9")],6);
               cr.take_clips(c,6);
               cr.reload(false);
            }
            else if(!LCSrandom(2))
            {
               Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
               cr.give_weapon(w,NULL);
               Clip c=Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
               cr.take_clips(c,4);
               cr.reload(false);
            }
            else
            {
               Weapon w=Weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")]);
               cr.give_weapon(w,NULL);
            }
            armor=new Armor(*armortype[getarmortype("ARMOR_POLICEARMOR")]);
            cr.give_armor(*armor,NULL);
            cr.money=LCSrandom(21)+20;
            cr.align=-1;
            cr.infiltration=0.3f + 0.1f*LCSrandom(4);
            cr.juice=10+LCSrandom(50);
            cr.age=AGE_MATURE;

            cr.set_skill(SKILL_PISTOL,LCSrandom(4)+1);
            cr.set_skill(SKILL_SHOTGUN,LCSrandom(3)+1);
            cr.set_skill(SKILL_CLUB,LCSrandom(2)+1);
            cr.set_skill(SKILL_HANDTOHAND,LCSrandom(2)+1);
            cr.set_skill(SKILL_DRIVING,LCSrandom(2)+1);
            cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3)+1);
   
            cr.set_attribute(ATTRIBUTE_STRENGTH,3);
            cr.set_attribute(ATTRIBUTE_AGILITY,3);
            cr.set_attribute(ATTRIBUTE_HEALTH,3);
            cr.set_attribute(ATTRIBUTE_WISDOM,4);
         }
         break;
      case CREATURE_SWAT:
         GIVE_GENDER_MALE;
         if(LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],6);
            cr.take_clips(c,6);
            cr.reload(false);
         }
         else if(!LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_CARBINE_M4")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_ASSAULT")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_SWATARMOR")]);
         cr.give_armor(*armor,NULL);
         cr.align=-1;
         cr.infiltration=0.3f + 0.1f*LCSrandom(4);
         cr.juice=40+LCSrandom(50);
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+1);
         cr.set_skill(SKILL_PISTOL,LCSrandom(4)+1);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(4)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(2)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(2)+1);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(4)+1);

         cr.set_attribute(ATTRIBUTE_STRENGTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         cr.set_attribute(ATTRIBUTE_WISDOM,4);
         break;
      case CREATURE_DEATHSQUAD:
         GIVE_GENDER_MALE;
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")]);
         cr.give_weapon(*weapon,NULL);
         clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
         cr.take_clips(*clips,7);
         cr.reload(false);
         armor=new Armor(*armortype[getarmortype("ARMOR_DEATHSQUADUNIFORM")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=90+LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+2);
         cr.set_skill(SKILL_PISTOL,LCSrandom(2)+1);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(2)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(2)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(2)+1);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(4)+2);

         cr.set_attribute(ATTRIBUTE_STRENGTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         cr.set_attribute(ATTRIBUTE_WISDOM,6);
         break;
      case CREATURE_FIREFIGHTER:
         GIVE_GENDER_MALE;
         if(law[LAW_FREESPEECH]==-2)
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_FLAMETHROWER")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_GASOLINE")],4);
            cr.take_clips(c,4);
            cr.reload(false);
            cr.set_skill(SKILL_HEAVYWEAPONS,LCSrandom(3)+2);
            strcpy(cr.name,"Fireman");
            cr.align=-1;
         } else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_AXE")]);
            cr.give_weapon(w,NULL);
            cr.set_skill(SKILL_AXE,LCSrandom(3)+2);
            strcpy(cr.name,"Firefighter");
         }
         
         if(sitealarm)
         {
            // Respond to emergencies in bunker gear
            Armor a=Armor(*armortype[getarmortype("ARMOR_BUNKERGEAR")]);
            cr.give_armor(a,NULL);
         }
         else
         {
            // Other situations have various clothes
            switch(LCSrandom(3))
            {
            case 0:
               armor=new Armor(*armortype[getarmortype("ARMOR_OVERALLS")]);
               cr.give_armor(*armor,NULL);
               break;
            case 1:
               armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
               cr.give_armor(*armor,NULL);
               break;
            case 2:
               armor=new Armor(*armortype[getarmortype("ARMOR_BUNKERGEAR")]);
               cr.give_armor(*armor,NULL);
            }
         }

         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;
         
         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_STRENGTH,3);
         break;
      case CREATURE_CCS_MOLOTOV:
      {
         GIVE_GENDER_MALE;
         armor=new Armor(*armortype[getarmortype("ARMOR_TRENCHCOAT")]);
         cr.give_armor(*armor,NULL);
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_MOLOTOV")],5);
         while (!weapon->empty())
            cr.give_weapon(*weapon,NULL);
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
         {
            nameCCSMember(cr);
         }

         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(4)+2);
         cr.set_skill(SKILL_THROWING,LCSrandom(4)+2);
         cr.set_skill(SKILL_DRIVING,LCSrandom(4)+2);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3));
         cr.set_skill(SKILL_BUSINESS,LCSrandom(3));
         cr.set_skill(SKILL_RELIGION,LCSrandom(3)+1);

         for(a=0;a<ATTNUM;a++)
         {
            cr.set_attribute(a,1);
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         cr.set_attribute(ATTRIBUTE_STRENGTH,6);
         cr.set_attribute(ATTRIBUTE_AGILITY,6);
         cr.set_attribute(ATTRIBUTE_HEALTH,6);
         cr.set_attribute(ATTRIBUTE_WISDOM,8);
         break;
      }
      case CREATURE_CCS_SNIPER:
      {
         GIVE_GENDER_MALE;
         armor=new Armor(*armortype[getarmortype("ARMOR_TRENCHCOAT")]);
         cr.give_armor(*armor,NULL);
         Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIRIFLE_AR15")]);
         cr.give_weapon(w,NULL);
         Clip c=Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
         cr.take_clips(c,7);
         cr.reload(false);

         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
         {
            nameCCSMember(cr);
         }

         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(4)+2);
         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+6);
         cr.set_skill(SKILL_DRIVING,LCSrandom(4)+2);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3));
         cr.set_skill(SKILL_BUSINESS,LCSrandom(3));
         cr.set_skill(SKILL_RELIGION,LCSrandom(3)+1);

         for(a=0;a<ATTNUM;a++)
         {
            cr.set_attribute(a,1);
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         cr.set_attribute(ATTRIBUTE_STRENGTH,6);
         cr.set_attribute(ATTRIBUTE_AGILITY,6);
         cr.set_attribute(ATTRIBUTE_HEALTH,6);
         cr.set_attribute(ATTRIBUTE_WISDOM,8);
         break;
      }
      case CREATURE_CCS_VIGILANTE:
      {
         GIVE_GENDER_MALE;
         switch(LCSrandom(5)+endgamestate)
         {
         case 0:
         case 1:
            break;
         case 2:
            weapon=new Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
            cr.give_weapon(*weapon,NULL);
            clips=new Clip(*cliptype[getcliptype("CLIP_9")],7);
            cr.take_clips(*clips,7);
            cr.reload(false);
            break;
         case 3:
            weapon=new Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_44")]);
            cr.give_weapon(*weapon,NULL);
            clips=new Clip(*cliptype[getcliptype("CLIP_44")],7);
            cr.take_clips(*clips,7);
            cr.reload(false);
            break;
         case 4:
            weapon=new Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
            cr.give_weapon(*weapon,NULL);
            clips=new Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],7);
            cr.take_clips(*clips,7);
            cr.reload(false);
            break;
         case 5:
            weapon=new Weapon(*weapontype[getweapontype("WEAPON_SEMIRIFLE_AR15")]);
            cr.give_weapon(*weapon,NULL);
            clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.take_clips(*clips,7);
            cr.reload(false);
            armor=new Armor(*armortype[getarmortype("ARMOR_CIVILLIANARMOR")]);
            cr.give_armor(*armor,NULL);
            break;
         case 6:
            weapon=new Weapon(*weapontype[getweapontype("WEAPON_SEMIRIFLE_AR15")]);
            cr.give_weapon(*weapon,NULL);
            clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.take_clips(*clips,7);
            cr.reload(false);
            armor=new Armor(*armortype[getarmortype("ARMOR_ARMYARMOR")]);
            cr.give_armor(*armor,NULL);
            break;
         case 7:
            weapon=new Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")]);
            cr.give_weapon(*weapon,NULL);
            clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.take_clips(*clips,7);
            cr.reload(false);
            armor=new Armor(*armortype[getarmortype("ARMOR_ARMYARMOR")]);
            cr.give_armor(*armor,NULL);
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

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+3);
         cr.set_skill(SKILL_PISTOL,LCSrandom(4)+3);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(4)+3);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(4)+2);
         cr.set_skill(SKILL_DRIVING,LCSrandom(4)+2);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3));
         cr.set_skill(SKILL_BUSINESS,LCSrandom(3));
         cr.set_skill(SKILL_RELIGION,LCSrandom(3)+1);

         for(a=0;a<ATTNUM;a++)
         {
            cr.set_attribute(a,1);
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         cr.set_attribute(ATTRIBUTE_STRENGTH,4);
         cr.set_attribute(ATTRIBUTE_AGILITY,4);
         cr.set_attribute(ATTRIBUTE_HEALTH,4);
         cr.set_attribute(ATTRIBUTE_WISDOM,8);
         break;
      }
      case CREATURE_CCS_ARCHCONSERVATIVE:
         GIVE_GENDER_MALE;
         
         armor=new Armor(*armortype[getarmortype("ARMOR_HEAVYARMOR")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(51)+150;
         cr.align=-1;
         cr.infiltration=0.9f + 0.01f*LCSrandom(11);
         cr.juice=500+LCSrandom(250);
         cr.age=AGE_MIDDLEAGED;

         if(location[cursite]->siege.siege) {
            strcpy(cr.name,"CCS Team Leader");
         } else if(ccs_kills < 2) {
            strcpy(cr.name,"CCS Lieutenant");
         } else {
            strcpy(cr.name,"CCS Founder");
         }
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_AA12")]);
         cr.give_weapon(*weapon,NULL);
         clips=new Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],9);
         cr.take_clips(*clips,9);
         cr.reload(false);

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+6);
         cr.set_skill(SKILL_PISTOL,LCSrandom(4)+6);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(4)+6);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(4)+4);
         cr.set_skill(SKILL_DRIVING,LCSrandom(4)+4);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(6));
         cr.set_skill(SKILL_BUSINESS,LCSrandom(6));
         cr.set_skill(SKILL_RELIGION,LCSrandom(6)+2);
         cr.set_skill(SKILL_DODGE,LCSrandom(6)+2);

         for(a=0;a<ATTNUM;a++)
         {
            cr.set_attribute(a,1);
            attcap[a]=25;
         }
         attcap[ATTRIBUTE_HEART]=1;
         cr.set_attribute(ATTRIBUTE_STRENGTH,4);
         cr.set_attribute(ATTRIBUTE_AGILITY,4);
         cr.set_attribute(ATTRIBUTE_HEALTH,4);
         cr.set_attribute(ATTRIBUTE_WISDOM,10);
         break;
      case CREATURE_GANGUNIT:
         GIVE_GENDER_MALE;
         if(!LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_9")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_POLICEARMOR")]);
         cr.give_armor(*armor,NULL);
         cr.align=-1;
         cr.infiltration=0.3f + 0.1f*LCSrandom(4);
         cr.juice=40+LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         cr.set_skill(SKILL_RIFLE,LCSrandom(3)+1);
         cr.set_skill(SKILL_PISTOL,LCSrandom(4)+1);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(3)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(2)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(2)+1);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(4)+2);

         cr.set_attribute(ATTRIBUTE_STRENGTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         cr.set_attribute(ATTRIBUTE_WISDOM,5);
         break;
      case CREATURE_PRISONGUARD:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Prison Guard");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_PRISONGUARD")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PISTOL,LCSrandom(2)+1);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(3)+1);
         cr.set_skill(SKILL_CLUB,LCSrandom(3)+2);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(2)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(2));

         cr.set_attribute(ATTRIBUTE_STRENGTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         break;
      case CREATURE_EDUCATOR:
         strcpy(cr.name,"Educator");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(3))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_9")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SYRINGE")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_LABCOAT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(4);
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_RIFLE,LCSrandom(3)+1);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(3)+1);
         cr.set_skill(SKILL_CLUB,LCSrandom(2)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(2)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(2));
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(4)+3);

         cr.set_attribute(ATTRIBUTE_STRENGTH,3);
         cr.set_attribute(ATTRIBUTE_AGILITY,3);
         cr.set_attribute(ATTRIBUTE_HEALTH,3);
         cr.set_attribute(ATTRIBUTE_WISDOM,4);
         break;
      case CREATURE_AGENT:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Agent");
         switch(LCSrandom(7))
         {
            case 0:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_44")]);
               cr.give_weapon(*weapon,NULL);
               clips=new Clip(*cliptype[getcliptype("CLIP_44")],4);
               cr.take_clips(*clips,4);
               cr.reload(false);
               break;
            case 1:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
               cr.give_weapon(*weapon,NULL);
               clips=new Clip(*cliptype[getcliptype("CLIP_9")],4);
               cr.take_clips(*clips,4);
               cr.reload(false);
               break;
            case 2:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")]);
               cr.give_weapon(*weapon,NULL);
               clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],4);
               cr.take_clips(*clips,4);
               cr.reload(false);
               break;
            case 3:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_AK47")]);
               cr.give_weapon(*weapon,NULL);
               clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],4);
               cr.take_clips(*clips,4);
               cr.reload(false);
               break;
            case 4:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
               cr.give_weapon(*weapon,NULL);
               clips=new Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
               cr.take_clips(*clips,4);
               cr.reload(false);
               break;
            case 5:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_CARBINE_M4")]);
               cr.give_weapon(*weapon,NULL);
               clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],4);
               cr.take_clips(*clips,4);
               cr.reload(false);
               break;
            case 6:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
               cr.give_weapon(*weapon,NULL);
               clips=new Clip(*cliptype[getcliptype("CLIP_SMG")],4);
               cr.take_clips(*clips,4);
               cr.reload(false);
               break;
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_BLACKSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.align=-1;
         cr.infiltration=0.5f*LCSrandom(4);
         cr.juice=200+LCSrandom(150);
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PISTOL,LCSrandom(8)+2);
         cr.set_skill(SKILL_RIFLE,LCSrandom(8)+2);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(8)+2);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(8)+2);
         cr.set_skill(SKILL_CLUB,LCSrandom(8)+2);
         cr.set_skill(SKILL_DRIVING,LCSrandom(6)+4);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(6)+4);
         cr.set_skill(SKILL_DODGE,LCSrandom(4)+4);
         
         cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         cr.set_attribute(ATTRIBUTE_AGILITY,7);
         cr.set_attribute(ATTRIBUTE_HEALTH,5);
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,4);
         cr.set_attribute(ATTRIBUTE_WISDOM,5);
         break;
      case CREATURE_RADIOPERSONALITY:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Radio Personality");
         armor=new Armor(*armortype[getarmortype("ARMOR_EXPENSIVESUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(51)+50;
         cr.align=-1;
         cr.infiltration=0.6f + 0.1f*LCSrandom(4);
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PERSUASION,LCSrandom(8)+1);
         
         cr.set_attribute(ATTRIBUTE_CHARISMA,10);
         cr.set_attribute(ATTRIBUTE_WISDOM,8);
         break;
      case CREATURE_NEWSANCHOR:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"News Anchor");
         armor=new Armor(*armortype[getarmortype("ARMOR_EXPENSIVESUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(51)+50;
         
         cr.set_attribute(ATTRIBUTE_CHARISMA,10);
         cr.set_attribute(ATTRIBUTE_WISDOM,8);
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

         switch(LCSrandom(11))
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
            case 10:strcat(cr.name,"Pink Elephant");break;
         }
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.strip(NULL);
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
         if(law[LAW_ANIMALRESEARCH]!=2)
            cr.money=0;
         break;
      case CREATURE_GUARDDOG:
         strcpy(cr.name,"Guard Dog");
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.strip(NULL);
         cr.align=-1;
         attcap[ATTRIBUTE_INTELLIGENCE]=1;
         attcap[ATTRIBUTE_HEART]=6;
         attcap[ATTRIBUTE_WISDOM]=6;
         cr.set_attribute(ATTRIBUTE_STRENGTH,7);
         attcap[ATTRIBUTE_STRENGTH]=12;
         cr.set_attribute(ATTRIBUTE_AGILITY,9);
         attcap[ATTRIBUTE_AGILITY]=15;
         cr.age=AGE_DOGYEARS;
         if(law[LAW_ANIMALRESEARCH]!=2)
            cr.money=0;
         break;
      case CREATURE_PRISONER:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Prisoner");
         if(!LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHANK")]);
            cr.give_weapon(w,NULL);
         }
         armor=new Armor(*armortype[getarmortype("ARMOR_PRISONER")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);

         // Prisoners should not be "prisoners" after recruiting them -- they should
         // be some brand of criminal
         if(!LCSrandom(10))
         {
            // Thief
            cr.set_skill(SKILL_SECURITY,LCSrandom(5)+3);
            cr.set_skill(SKILL_DISGUISE,LCSrandom(5)+3);
            cr.set_skill(SKILL_STEALTH,LCSrandom(5)+3);
            //cr.set_skill(SKILL_THEFT,LCSrandom(5)+3);
            cr.type=CREATURE_THIEF;
            cr.age=AGE_MATURE;
         }
         else
         {
            switch(LCSrandom(5))
            {
            case 0:
               // Gang member
               cr.set_skill(SKILL_PISTOL,LCSrandom(2)+1);
               cr.set_skill(SKILL_SHOTGUN,LCSrandom(2)+1);
               cr.set_skill(SKILL_RIFLE,LCSrandom(2)+1);
               cr.type=CREATURE_GANGMEMBER;
               cr.age=AGE_YOUNGADULT;
               break;
            case 1:
               // Prostitute
               cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+2);
               cr.set_skill(SKILL_SEDUCTION,LCSrandom(4)+2);
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
         cr.money=0;
         cr.align=-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_FACTORY_CHILD:
         strcpy(cr.name,"Child Worker");
         weapon=new Weapon(*weapontype[getweapontype("WEAPON_CHAIN")]);
         cr.give_weapon(*weapon,NULL);
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.align=1;
         cr.age=AGE_CHILD;
         
         cr.set_attribute(ATTRIBUTE_HEART,8);
         break;
      case CREATURE_SEWERWORKER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Sewer Worker");
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_COLLEGESTUDENT:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"College Student");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=18+LCSrandom(6); // no macro

         cr.set_skill(SKILL_COMPUTERS,LCSrandom(2));
         cr.set_skill(SKILL_WRITING,LCSrandom(2)+1);
         cr.set_skill(SKILL_SCIENCE,LCSrandom(3));
         cr.set_skill(SKILL_ART,LCSrandom(3));
         cr.set_skill(SKILL_MUSIC,LCSrandom(3));
         break;
      case CREATURE_MUSICIAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Musician");
         cr.money=LCSrandom(11);
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_MUSIC,LCSrandom(4)+2);
         cr.set_skill(SKILL_SEDUCTION,LCSrandom(2));
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_MATHEMATICIAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Mathematician");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_COMPUTERS,LCSrandom(3));
         cr.set_skill(SKILL_SCIENCE,LCSrandom(3)+1);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,7);
         break;
      case CREATURE_TEACHER:
         GIVE_GENDER_FEMALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Teacher");
         cr.set_skill(SKILL_TEACHING,LCSrandom(4)+3);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_HSDROPOUT:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Highschool Dropout");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_TEENAGER;
         break;
      case CREATURE_BUM:
         strcpy(cr.name,"Transient");
         GIVE_WEAPON_CIVILIAN;
         if(!cr.is_armed() && !LCSrandom(5))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHANK")]);
            cr.give_weapon(w,NULL);
         }
         cr.money=LCSrandom(31)+20;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_MUTANT:
         strcpy(cr.name,"Mutant");
         GIVE_WEAPON_CIVILIAN;
         if(!cr.is_armed() && !LCSrandom(5))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHANK")]);
            cr.give_weapon(w,NULL);
         }
         cr.money=LCSrandom(31)+20;
         cr.align=1;
         //cr.set_skill(SKILL_SURVIVAL,LCSrandom(3)+1);
         for(a=0;a<ATTNUM;a++)
         {
            attcap[a]=50;
         }
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GANGMEMBER:
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Gang Member");

         if(!LCSrandom(20) || (law[LAW_GUNCONTROL]==-2 && !LCSrandom(5)))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_AK47")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_ASSAULT")],3);
            cr.take_clips(c,3);
            cr.reload(false);
         }
         else if(!LCSrandom(16) || (law[LAW_GUNCONTROL]==-2 && !LCSrandom(5)))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_SMG")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(15))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_45")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_45")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(10))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(4))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_9")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else if(!LCSrandom(2))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")]);
            cr.give_weapon(w,NULL);
            Clip c=Clip(*cliptype[getcliptype("CLIP_38")],4);
            cr.take_clips(c,4);
            cr.reload(false);
         }
         else
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_COMBATKNIFE")]);
            cr.give_weapon(w,NULL);
         }

         cr.money=LCSrandom(31)+20;
         cr.juice=-5;
         // We'll make the crack house a bit dicey
         if(location[cursite]->type == SITE_BUSINESS_CRACKHOUSE)
            cr.align=-1;
         cr.age=AGE_YOUNGADULT;

         cr.set_skill(SKILL_PISTOL,LCSrandom(2)+1);
         cr.set_skill(SKILL_SHOTGUN,LCSrandom(2)+1);
         cr.set_skill(SKILL_RIFLE,LCSrandom(2)+1);

         if(!LCSrandom(2))
         {
            switch(LCSrandom(3))
            {
            case 0://cr.crimes_committed[LAWFLAG_BROWNIES]++;
                   cr.crimes_suspected[LAWFLAG_BROWNIES]++;break;
            case 1://cr.crimes_committed[LAWFLAG_ASSAULT]++;
                   cr.crimes_suspected[LAWFLAG_ASSAULT]++;break;
            case 2://cr.crimes_committed[LAWFLAG_MURDER]++;
                   cr.crimes_suspected[LAWFLAG_MURDER]++;break;
            }
         }
         break;
      case CREATURE_CRACKHEAD:
         strcpy(cr.name,"Crack Head");
         GIVE_WEAPON_CIVILIAN;
         if(!LCSrandom(5))
         {
            Weapon w=Weapon(*weapontype[getweapontype("WEAPON_SHANK")]);
            cr.give_weapon(w,NULL);
         }
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
         GIVE_GENDER_MALE;
         strcpy(cr.name,"Priest");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_RELIGION,LCSrandom(5)+3);
         break;
      case CREATURE_ENGINEER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Engineer");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_COMPUTERS,LCSrandom(2)+1);
         cr.set_skill(SKILL_SCIENCE,LCSrandom(3)+1);
         break;
      case CREATURE_FASTFOODWORKER:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Fast Food Worker");
         //cr.set_skill(SKILL_COOKING,LCSrandom(3));
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
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_YOUNGADULT;

         cr.set_skill(SKILL_PERSUASION,LCSrandom(3)+1);
         break;
	  case CREATURE_CARSALESMAN:
		 GIVE_WEAPON_CIVILIAN;
		 armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
		 cr.give_armor(*armor,NULL);
		 cr.set_skill(SKILL_BUSINESS,LCSrandom(4)+1);
		 cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+1);
		 cr.set_skill(SKILL_DRIVING,LCSrandom(3)+1);
		 cr.money=LCSrandom(41)+40;
		 cr.age=AGE_MATURE;
		 break;
	  case CREATURE_OFFICEWORKER:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Office Worker");
         cr.set_skill(SKILL_BUSINESS,LCSrandom(3));
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_FOOTBALLCOACH:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Football Coach");
         cr.set_skill(SKILL_TEACHING,LCSrandom(3)+2);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MIDDLEAGED;

         if(LCSrandom(2))
         {
            
            cr.set_attribute(ATTRIBUTE_HEALTH,5);
            cr.set_attribute(ATTRIBUTE_AGILITY,5);
            cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         }
         cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+2);
         break;
      case CREATURE_PROSTITUTE:
         GIVE_WEAPON_CIVILIAN;
         if(LCSrandom(7))
            cr.gender_conservative=cr.gender_liberal=GENDER_FEMALE;
         else if(!LCSrandom(3))
            cr.gender_liberal=GENDER_FEMALE;
         strcpy(cr.name,"Prostitute");
         if(LCSrandom(2))
         {
            Armor a=Armor(*armortype[getarmortype("ARMOR_CHEAPDRESS")]);
            cr.give_armor(a,NULL);
         }
         cr.money=LCSrandom(31)+20;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_YOUNGADULT;
         cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+2);
         cr.set_skill(SKILL_SEDUCTION,LCSrandom(4)+2);

         //cr.crimes_committed[LAWFLAG_PROSTITUTION]++;
         if(!LCSrandom(3))
         {
            cr.crimes_suspected[LAWFLAG_PROSTITUTION]++;
         }
         break;
      case CREATURE_MAILMAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Mail Carrier");
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GARBAGEMAN:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Garbage Collector");
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_DRIVING,LCSrandom(2)+1);
         break;
      case CREATURE_PLUMBER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Plumber");
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CHEF:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Chef");
         //cr.set_skill(SKILL_COOKING,LCSrandom(5)+3);
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CONSTRUCTIONWORKER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Construction Worker");
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         
         cr.set_attribute(ATTRIBUTE_STRENGTH,7);
         cr.set_attribute(ATTRIBUTE_HEALTH,7);
         cr.set_skill(SKILL_DRIVING,LCSrandom(2)+1);
         break;
      case CREATURE_AMATEURMAGICIAN:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Amateur Magician");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         //cr.set_skill(SKILL_THEFT,LCSrandom(3)+2);
         break;
      case CREATURE_HIPPIE:
         strcpy(cr.name,"Hippie");
         cr.money=LCSrandom(31)+20;
         cr.align=1;
         
         cr.set_attribute(ATTRIBUTE_HEART,6);
         attcap[ATTRIBUTE_HEART]=15;
         attcap[ATTRIBUTE_WISDOM]=1;
         attnum-=5;
         //cr.set_skill(SKILL_COOKING,LCSrandom(2));
         cr.set_skill(SKILL_MUSIC,LCSrandom(2));
         cr.set_skill(SKILL_ART,LCSrandom(2));
         cr.set_skill(SKILL_TAILORING,LCSrandom(2));

         if(!LCSrandom(10))
         {
            //cr.crimes_committed[LAWFLAG_BROWNIES]++;
            cr.crimes_suspected[LAWFLAG_BROWNIES]++;
         }
         break;
      case CREATURE_AUTHOR:
         GIVE_WEAPON_CIVILIAN;
         armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PERSUASION,LCSrandom(4));
         cr.set_skill(SKILL_WRITING,LCSrandom(4)+4);
         break;
      case CREATURE_JOURNALIST:
         GIVE_WEAPON_CIVILIAN;
         armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.juice=LCSrandom(25);
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+2);
         cr.set_skill(SKILL_WRITING,LCSrandom(4)+2);
         break;
      case CREATURE_CRITIC_ART:
         GIVE_WEAPON_CIVILIAN;
         armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+2);
         cr.set_skill(SKILL_WRITING,LCSrandom(4)+2);
         cr.set_skill(SKILL_ART,LCSrandom(4)+1);
         break;
      case CREATURE_CRITIC_MUSIC:
         GIVE_WEAPON_CIVILIAN;
         armor=new Armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+2);
         cr.set_skill(SKILL_WRITING,LCSrandom(4)+2);
         cr.set_skill(SKILL_MUSIC,LCSrandom(4)+1);
         break;
      case CREATURE_SOCIALITE:
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Socialite");
         if(LCSrandom(2))
         {
            Armor a=Armor(*armortype[getarmortype("ARMOR_EXPENSIVEDRESS")]);
            cr.give_armor(a,NULL);
         }
         else
         {
            Armor a=Armor(*armortype[getarmortype("ARMOR_EXPENSIVESUIT")]);
            cr.give_armor(a,NULL);
         }
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_PERSUASION,LCSrandom(3)+1);
         cr.set_skill(SKILL_ART,LCSrandom(3));
         cr.set_skill(SKILL_MUSIC,LCSrandom(3));
         break;
      case CREATURE_BIKER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Biker");
         armor=new Armor(*armortype[getarmortype("ARMOR_TRENCHCOAT")]);
         cr.give_armor(*armor,NULL);
         cr.money=LCSrandom(31)+20;
         cr.align=-1;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_DRIVING,LCSrandom(3)+3);
         break;
      case CREATURE_TRUCKER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Truck Driver");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_DRIVING,LCSrandom(3)+3);
         break;
      case CREATURE_TAXIDRIVER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Taxi Driver");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         cr.set_skill(SKILL_DRIVING,LCSrandom(3)+3);
         break;
      case CREATURE_PROGRAMMER:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         strcpy(cr.name,"Programmer");
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_COMPUTERS,LCSrandom(5)+4);
         
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE,3);
         break;
      case CREATURE_NUN:
         cr.gender_conservative=cr.gender_liberal=GENDER_FEMALE;
         cr.set_skill(SKILL_RELIGION,LCSrandom(3)+5);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_RETIREE:
         GIVE_WEAPON_CIVILIAN;
         cr.age=AGE_SENIOR;
         break;
      case CREATURE_PAINTER:
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_ART,LCSrandom(4)+3);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_SCULPTOR:
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_ART,LCSrandom(4)+3);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_DANCER:
         cr.set_attribute(ATTRIBUTE_AGILITY,10);
         cr.set_attribute(ATTRIBUTE_STRENGTH,4);
         cr.set_attribute(ATTRIBUTE_HEALTH,10);
         cr.set_skill(SKILL_ART,LCSrandom(2)+1);
         cr.set_skill(SKILL_MUSIC,LCSrandom(2)+1);
         cr.age=AGE_YOUNGADULT;
         break;
      case CREATURE_PHOTOGRAPHER:
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_ART,LCSrandom(2)+2);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CAMERAMAN:
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_ART,LCSrandom(2)+1);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_HAIRSTYLIST:
         GIVE_GENDER_FEMALE;
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_ART,LCSrandom(2)+1);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_FASHIONDESIGNER:
         GIVE_GENDER_FEMALE;
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_TAILORING,LCSrandom(3)+8);
         cr.set_skill(SKILL_ART,LCSrandom(3)+2);
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
         cr.set_skill(SKILL_SECURITY,LCSrandom(5)+3);
         cr.set_skill(SKILL_DISGUISE,LCSrandom(5)+3);
         cr.set_skill(SKILL_STEALTH,LCSrandom(5)+3);
         //cr.set_skill(SKILL_THEFT,LCSrandom(5)+3);
         cr.age=AGE_MATURE;

	 {
	    Armor to_give = Armor(*armortype[getarmortype("ARMOR_BLACKCLOTHES")]);
	    cr.give_armor(to_give,NULL);
	 }

         //cr.crimes_committed[LAWFLAG_BREAKING]++;
         //cr.crimes_committed[LAWFLAG_THEFT]++;
         if(!LCSrandom(10))
         {
            switch(LCSrandom(2))
            {
            case 0:cr.crimes_suspected[LAWFLAG_BREAKING]++;
            case 1:cr.crimes_suspected[LAWFLAG_THEFT]++;
            }
         }
         break;
      case CREATURE_ACTOR:
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_PERSUASION,LCSrandom(5));
         cr.set_skill(SKILL_SEDUCTION,LCSrandom(5));
         cr.set_skill(SKILL_DISGUISE,LCSrandom(5)+3);
         
         cr.set_attribute(ATTRIBUTE_WISDOM,9);
         cr.set_attribute(ATTRIBUTE_CHARISMA,5);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_YOGAINSTRUCTOR:
         GIVE_GENDER_FEMALE;
         cr.set_skill(SKILL_TEACHING,LCSrandom(4)+2);
         cr.set_attribute(ATTRIBUTE_AGILITY,10);
         cr.set_attribute(ATTRIBUTE_STRENGTH,4);
         cr.set_attribute(ATTRIBUTE_HEALTH,10);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_MARTIALARTIST:
         GIVE_GENDER_MALE;
         cr.set_skill(SKILL_TEACHING,LCSrandom(3)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(4)+4);
         cr.set_attribute(ATTRIBUTE_AGILITY,8);
         cr.set_attribute(ATTRIBUTE_STRENGTH,6);
         cr.set_attribute(ATTRIBUTE_HEALTH,6);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_ATHLETE:
         GIVE_GENDER_MALE;
         GIVE_WEAPON_CIVILIAN;
         cr.set_attribute(ATTRIBUTE_AGILITY,8);
         cr.set_attribute(ATTRIBUTE_STRENGTH,8);
         cr.set_attribute(ATTRIBUTE_HEALTH,8);
         cr.age=AGE_YOUNGADULT;
         break;
      case CREATURE_LOCKSMITH:
         GIVE_WEAPON_CIVILIAN;
         cr.set_skill(SKILL_SECURITY, LCSrandom(5) + 3);
         cr.age=AGE_MATURE;
         armor=new Armor(*armortype[getarmortype("ARMOR_WORKCLOTHES")]);
         cr.give_armor(*armor,NULL);
         break;
      case CREATURE_MILITARYPOLICE:
         GIVE_GENDER_MALE;
         switch (LCSrandom(3)) {
            case 0:
               weapon = new Weapon(*weapontype[getweapontype("WEAPON_CARBINE_M4")]);
               clips = new Clip(*cliptype[getcliptype("CLIP_ASSAULT")]);
               break;
            case 1:
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")]);
               clips=new Clip(*cliptype[getcliptype("CLIP_BUCKSHOT")]);
               break;
            default :
               weapon=new Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
               clips=new Clip(*cliptype[getcliptype("CLIP_9")],6);
         }

         cr.give_weapon(*weapon,NULL);
         cr.take_clips(*clips,7);
         cr.reload(false);
         armor=new Armor(*armortype[getarmortype("ARMOR_ARMYARMOR")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f*LCSrandom(6);     //Authority over the regular soldiers!
         cr.juice=LCSrandom(100);
         cr.age=AGE_YOUNGADULT;

         cr.set_skill(SKILL_RIFLE,LCSrandom(3)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(5)+1);
         cr.set_skill(SKILL_PISTOL,LCSrandom(5)+1);
         cr.set_skill(SKILL_DRIVING,LCSrandom(4)+1);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(5)+1);
         cr.set_skill(SKILL_SMG, LCSrandom(5)+1);
         cr.set_skill(SKILL_SHOTGUN, LCSrandom(5)+1);

         cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         cr.set_attribute(ATTRIBUTE_AGILITY,5);
         cr.set_attribute(ATTRIBUTE_HEALTH,5);
         cr.set_attribute(ATTRIBUTE_WISDOM,5);
         break;


      case CREATURE_MILITARYOFFICER:
         GIVE_GENDER_MALE;
         if(LCSrandom(4)) {
            weapon = new Weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
            clips=new Clip(*cliptype[getcliptype("CLIP_9")],6);
            cr.give_weapon(*weapon,NULL);
            cr.take_clips(*clips,3);
            cr.reload(false);
         }
         
         armor=new Armor(*armortype[getarmortype("ARMOR_MILITARY")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.5f + 0.1f*LCSrandom(4);
         cr.juice=100 + LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         cr.set_skill(SKILL_RIFLE,LCSrandom(2)+1);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(6)+1);
         cr.set_skill(SKILL_PISTOL,LCSrandom(6)+1);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(6)+1);
         cr.set_skill(SKILL_PERSUASION, LCSrandom(5)+3);
         
         cr.set_attribute(ATTRIBUTE_CHARISMA, 7);
         cr.set_attribute(ATTRIBUTE_INTELLIGENCE, 7);
         cr.set_attribute(ATTRIBUTE_WISDOM,10);
         break;

      case CREATURE_SEAL:
         GIVE_GENDER_MALE;
         weapon = new Weapon(*weapontype[getweapontype("WEAPON_CARBINE_M4")]); // SEALs use M4s and MP5s

         clips=new Clip(*cliptype[getcliptype("CLIP_ASSAULT")],6);
         cr.give_weapon(*weapon,NULL);
         cr.take_clips(*clips,3);
         cr.reload(false);

         armor=new Armor(*armortype[getarmortype("ARMOR_SEALSUIT")]);
         cr.give_armor(*armor,NULL);
         cr.money=0;
         cr.align=-1;
         cr.infiltration=0.1f + 0.1f*LCSrandom(4);
         cr.juice=100 + LCSrandom(50);
         cr.age=AGE_MATURE;

         cr.set_skill(SKILL_RIFLE,LCSrandom(4)+4);
         cr.set_skill(SKILL_HANDTOHAND,LCSrandom(3)+3);
         cr.set_skill(SKILL_PSYCHOLOGY,LCSrandom(3)+3);
         cr.set_skill(SKILL_DODGE, LCSrandom(4)+2);

         cr.set_attribute(ATTRIBUTE_AGILITY, 7);
         cr.set_attribute(ATTRIBUTE_STRENGTH, 7);
         cr.set_attribute(ATTRIBUTE_HEALTH, 7);
         cr.set_attribute(ATTRIBUTE_WISDOM, 7);
         
         break;
   }
   
   delete weapon;
   delete armor;
   delete clips;

   if(cr.type==CREATURE_MUTANT)attnum=LCSrandom(60)+7;
   for(int a=0;a<ATTNUM;a++)
   {
      attnum-=min(4,cr.get_attribute(a,false));
   }
   while(attnum>0)
   {
      int a=LCSrandom(ATTNUM);
      if(a==ATTRIBUTE_WISDOM && cr.align==1 && LCSrandom(4))
         a=ATTRIBUTE_HEART;
      if(a==ATTRIBUTE_HEART && cr.align==-1 && LCSrandom(4))
         a=ATTRIBUTE_WISDOM;
      if(cr.get_attribute(a,false)<attcap[a])
      {
         cr.adjust_attribute(a,+1);
         attnum--;
      }
   }

   if(cr.align==1)
   {
      cr.infiltration = 0.15f+(LCSrandom(10)-5)*0.01f;
   }
   else if(cr.align==0)
   {
      cr.infiltration = 0.25f+(LCSrandom(10)-5)*0.01f;
   }
   else
   {
      cr.infiltration += 0.35f * (1-cr.infiltration)+(LCSrandom(10)-5)*0.01f;
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
      randomskills-=cr.get_skill(s);
   }*/

   //RANDOM STARTING SKILLS
   while(randomskills>0)
   {
      int randomskill = LCSrandom(SKILLNUM);
      // 95% chance of not allowing some skills for anybody...
      if(LCSrandom(20))
      {
         if(randomskill == SKILL_HEAVYWEAPONS)continue;
         if(randomskill == SKILL_SMG)continue;
         if(randomskill == SKILL_SWORD)continue;
         if(randomskill == SKILL_RIFLE)continue;
         if(randomskill == SKILL_AXE)continue;
         if(randomskill == SKILL_CLUB)continue;
         if(randomskill == SKILL_PSYCHOLOGY)continue;
      }

      // 90% chance of not allowing some skills, other than      
      //   for conservatives      
      if(LCSrandom(10) && cr.align!=ALIGN_CONSERVATIVE)      
      {
         if(randomskill == SKILL_SHOTGUN)continue;
         if(randomskill == SKILL_PISTOL)continue;
      }

      if(cr.skill_cap(randomskill,true)>cr.get_skill(randomskill))
      {
         cr.set_skill(randomskill,cr.get_skill(randomskill)+1);
         randomskills--;
      }

      while(1)
      {
         if(randomskills&&LCSrandom(2) &&
            cr.skill_cap(randomskill,true)>cr.get_skill(randomskill) &&
            cr.get_skill(randomskill) < 4)
         {
            cr.set_skill(randomskill,cr.get_skill(randomskill)+1);
            randomskills--;
         }
         else break;
      }
   }

   //ALIENATION
   if(sitealienate>=1&&cr.align==0)conservatise(cr);
   if(sitealienate==2&&cr.align==1)conservatise(cr);
}


/* ensures that the creature's work location is appropriate to its type */
bool verifyworklocation(Creature &cr, char test_location, char test_type)
{
   int okaysite[SITENUM];
   memset(okaysite,0,SITENUM*sizeof(int));

   short type;

   // If the caller sets test_type, they're just
   // asking if the chosen creature type is appropriate
   // to the location they provided, not actually setting
   // the creature work location -- this is useful
   // for things like stealth
   if(test_type!=-1)
      type=test_type;
   else
      type=cr.type;

   switch(type)
   {
      case CREATURE_BOUNCER:
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         break;
      case CREATURE_CORPORATE_CEO:
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         break;
      case CREATURE_SECURITYGUARD:
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         okaysite[SITE_BUSINESS_BANK]=1;
         //okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_BANK_MANAGER:
      case CREATURE_BANK_TELLER:
         okaysite[SITE_BUSINESS_BANK]=1;
         break;
      case CREATURE_SCIENTIST_LABTECH:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         break;
      case CREATURE_SCIENTIST_EMINENT:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         break;
      case CREATURE_CORPORATE_MANAGER:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_WORKER_SERVANT:
         okaysite[SITE_CORPORATE_HOUSE]=1;
         break;
      case CREATURE_WORKER_JANITOR:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_HOSPITAL_CLINIC]=1;
         okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_GOVERNMENT_PRISON]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_BUSINESS_PAWNSHOP]=1;
         okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
         okaysite[SITE_BUSINESS_JUICEBAR]=1;
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         okaysite[SITE_BUSINESS_LATTESTAND]=1;
         okaysite[SITE_BUSINESS_VEGANCOOP]=1;
         okaysite[SITE_BUSINESS_INTERNETCAFE]=1;
         okaysite[SITE_BUSINESS_DEPTSTORE]=1;
         okaysite[SITE_BUSINESS_HALLOWEEN]=1;
         okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_WORKER_SWEATSHOP:
         okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         break;
      case CREATURE_WORKER_FACTORY_CHILD:
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         break;
      case CREATURE_WORKER_SECRETARY:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_HOSPITAL_CLINIC]=1;
         okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_WORKER_FACTORY_UNION:
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         break;
      case CREATURE_LANDLORD:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         break;
      case CREATURE_TEENAGER:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         break;
      case CREATURE_COP:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_DEATHSQUAD:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_FIREFIGHTER:
         okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_GANGUNIT:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_SWAT:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_JUDGE_LIBERAL:
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         break;
      case CREATURE_AGENT:
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         break;
      case CREATURE_RADIOPERSONALITY:
         okaysite[SITE_MEDIA_AMRADIO]=1;
         break;
      case CREATURE_NEWSANCHOR:
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_GENETIC:
         okaysite[SITE_LABORATORY_GENETIC]=1;
         break;
      case CREATURE_GUARDDOG:
         okaysite[SITE_GOVERNMENT_PRISON]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         okaysite[SITE_GOVERNMENT_ARMYBASE]=1;
         break;
      case CREATURE_PRISONER:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
      case CREATURE_JUROR:
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
      case CREATURE_LAWYER:
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         break;
      case CREATURE_DOCTOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_PSYCHOLOGIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_NURSE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_CCS_ARCHCONSERVATIVE:
      case CREATURE_CCS_MOLOTOV:
      case CREATURE_CCS_SNIPER:
      case CREATURE_CCS_VIGILANTE:
         if(ccs_kills==2)okaysite[SITE_OUTDOOR_BUNKER]=1;
         if(ccs_kills==1)okaysite[SITE_RESIDENTIAL_BOMBSHELTER]=1;
         if(ccs_kills==0)okaysite[SITE_BUSINESS_BARANDGRILL]=1;
         break;
      case CREATURE_SEWERWORKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_COLLEGESTUDENT:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_MUSICIAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_MATHEMATICIAN:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_TEACHER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_HSDROPOUT:
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_BUM:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
      case CREATURE_GANGMEMBER:
         okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
         break;
      case CREATURE_CRACKHEAD:
         okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
         break;
      case CREATURE_PRIEST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_ENGINEER:
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         break;
      case CREATURE_FASTFOODWORKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_TELEMARKETER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
	  case CREATURE_CARSALESMAN:
         okaysite[SITE_BUSINESS_CARDEALERSHIP]=1;
         break;
	  case CREATURE_OFFICEWORKER:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_HOSPITAL_CLINIC]=1;
         okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_BUSINESS_DEPTSTORE]=1;
         break;
      case CREATURE_FOOTBALLCOACH:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_PROSTITUTE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_MAILMAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_GARBAGEMAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PLUMBER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_CHEF:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         break;
      case CREATURE_CONSTRUCTIONWORKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_AMATEURMAGICIAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_TANK:
         okaysite[SITE_GOVERNMENT_ARMYBASE]=1;
         break;
      case CREATURE_MERC:
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_BUSINESS_BANK]=1;
         break;
      case CREATURE_HICK:
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_OUTOFTOWN]=1;
         break;
      case CREATURE_VETERAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_HARDENED_VETERAN:
      case CREATURE_SOLDIER:
      case CREATURE_MILITARYPOLICE:
      case CREATURE_MILITARYOFFICER:
      case CREATURE_SEAL:
         okaysite[SITE_GOVERNMENT_ARMYBASE]=1;
         break;
      case CREATURE_EDUCATOR:
      case CREATURE_PRISONGUARD:
         okaysite[SITE_GOVERNMENT_PRISON]=1;
         break;
      case CREATURE_HIPPIE:
         okaysite[SITE_BUSINESS_VEGANCOOP]=1;
         break;
      case CREATURE_CRITIC_ART:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_CRITIC_MUSIC:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         break;
      case CREATURE_SOCIALITE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PROGRAMMER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         break;
      case CREATURE_RETIREE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PAINTER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_SCULPTOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_AUTHOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_JOURNALIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_DANCER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PHOTOGRAPHER:
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_CAMERAMAN:
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_HAIRSTYLIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_FASHIONDESIGNER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_CLERK:
         okaysite[SITE_BUSINESS_JUICEBAR]=1;
         okaysite[SITE_BUSINESS_LATTESTAND]=1;
         okaysite[SITE_BUSINESS_INTERNETCAFE]=1;
         okaysite[SITE_BUSINESS_DEPTSTORE]=1;
         okaysite[SITE_BUSINESS_HALLOWEEN]=1;
         break;
      case CREATURE_THIEF:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_ACTOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_YOGAINSTRUCTOR:
         okaysite[SITE_BUSINESS_VEGANCOOP]=1;
         break;
      case CREATURE_MARTIALARTIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_ATHLETE:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_BIKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_TRUCKER:
         okaysite[SITE_OUTOFTOWN]=1;
         break;
      case CREATURE_TAXIDRIVER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_NUN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_LOCKSMITH:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      default:
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
   }

   // Quick exit if only checking if a certain type works
   if(test_type!=-1)
   {
      return okaysite[test_location];
   }

   char swap=0;
   if(cr.worklocation==-1)swap=1;
   else
   {
      if(!okaysite[location[cr.worklocation]->type])swap=1;
   }

   if(swap)
   {
      //PICK A TYPE OF WORK LOCATION
      cr.worklocation=choose_one(okaysite,SITENUM,0);

      //FIND ONE OF THESE
      vector<int> goodlist;

      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==cr.worklocation)
         {
            goodlist.push_back(l);
         }
      }
// Sadler - This line sometimes causes a memory fault
//               Only thing I can think of is if loop above didn'
//               find any locations of type == to cr.worklocation
//               My hunch is that some locations, such as the 1st four
//               are special and cannot be used here..
//      
//   TODO There was a bug in the makecharacter() code where th
//   SITE_OUTOFTOWN was not set properly. This was fixed but the bug here
//   is still occuring, normally at the Latte Bar Downtown ;
      if (goodlist.size()==0)
      {
         cr.worklocation=0;
      }
      else
      {
         cr.worklocation=goodlist[LCSrandom(goodlist.size())];
      }
   }
   return false;
}
