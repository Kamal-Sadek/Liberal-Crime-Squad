
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
   cr.drop_weapons_and_clips(NULL); // Get rid of any old equipment from old encounters.
   cr.strip(NULL);                  //

   cr.creatureinit();
   cr.exists=1;
   cr.squadid=-1;
   cr.type=type;
   cr.infiltration=0;
   cr.location=cursite;
   cr.worklocation=cursite;
   verifyworklocation(cr);

   const CreatureType* crtype=getcreaturetype(type);
   crtype->make_creature(cr);
   int attnum=crtype->attribute_points_.roll();
   int attcap[ATTNUM];
   for (int i=0;i<ATTNUM;i++)
   {
      cr.set_attribute(i,crtype->attributes_[i].min);
      attcap[i]=crtype->attributes_[i].max;
   }

   switch(type)
   {
      case CREATURE_BOUNCER:
         if(mode==GAMEMODE_SITE && location[cursite]->highsecurity)
         {
            strcpy(cr.name,"Enforcer");
            cr.set_skill(SKILL_CLUB,LCSrandom(3)+3);
         }
         if(law[LAW_GUNCONTROL]==-2)
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_SMG")],4);
         }
         else if(law[LAW_GUNCONTROL]==-1)
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_44")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_44")],4);
         }
         else if(law[LAW_GUNCONTROL]==0)
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_38")],4);
         }
         else
            cr.give_weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")],NULL);
         cr.reload(false);
         if(disguisesite(sitetype))
         {
            cr.align=ALIGN_CONSERVATIVE;
            cr.infiltration=0.1f*LCSrandom(4);
         }
         else cr.align=ALIGN_MODERATE;
         break;
      case CREATURE_SECURITYGUARD:
         if(law[LAW_GUNCONTROL]==-2)
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_SMG")],4);
         }
         else if(law[LAW_GUNCONTROL]!=2)
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_38")],4);
         }
         else
            cr.give_weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")],NULL);
         cr.reload(false);
         break;
      case CREATURE_SCIENTIST_LABTECH:
         crtype->give_weapon_civilian(cr);
         if(!cr.is_armed()&&!LCSrandom(2))
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SYRINGE")],NULL);
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         if(law[LAW_GUNCONTROL]==-2&&!LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_44")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_44")],4);
         }
         else if(!LCSrandom(2))
            cr.give_weapon(*weapontype[getweapontype("WEAPON_GAVEL")],NULL);
         cr.reload(false);
         break;
      case CREATURE_SCIENTIST_EMINENT:
         crtype->give_weapon_civilian(cr);
         if(!cr.is_armed()&&!LCSrandom(2))
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SYRINGE")],NULL);
         break;
      case CREATURE_CORPORATE_CEO:
         generate_name(cr.propername,GENDER_WHITEMALEPATRIARCH);
         strcpy(cr.name,"CEO ");
         strcat(cr.name,cr.propername);
         cr.dontname=true;
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:
         crtype->give_weapon_civilian(cr);
         if(!cr.is_armed())
            cr.give_weapon(*weapontype[getweapontype("WEAPON_CHAIN")],NULL);
         if(cr.align==ALIGN_LIBERAL) cr.align=LCSrandom(2)-1;
         break;
      case CREATURE_WORKER_SWEATSHOP:
         cr.flag|=CREATUREFLAG_ILLEGALALIEN;
         break;
      case CREATURE_LAWYER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_38")],1);
            cr.reload(false);
         }
         break;
      case CREATURE_DOCTOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_38")],1);
            cr.reload(false);
         }
         break;
      case CREATURE_PSYCHOLOGIST:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_38")],1);
            cr.reload(false);
         }
         if(cr.gender_liberal==GENDER_MALE||LCSrandom(2))
            cr.give_armor(*armortype[getarmortype("ARMOR_CHEAPSUIT")],NULL);
         else
            cr.give_armor(*armortype[getarmortype("ARMOR_CHEAPDRESS")],NULL);
         break;
      case CREATURE_NURSE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_38")],1);
            cr.reload(false);
         }
         break;
      case CREATURE_WORKER_FACTORY_UNION:
         crtype->give_weapon_civilian(cr);
         if(!cr.is_armed())
            cr.give_weapon(*weapontype[getweapontype("WEAPON_CHAIN")],NULL);
         break;
      case CREATURE_TANK:
         cr.animalgloss=ANIMALGLOSS_TANK;
         cr.specialattack=ATTACK_CANNON;
         break;
      case CREATURE_MERC:
         cr.give_weapon(*weapontype[getweapontype(law[LAW_GUNCONTROL]<1?"WEAPON_AUTORIFLE_M16":"WEAPON_SEMIRIFLE_AR15")],NULL);
         cr.take_clips(*cliptype[getcliptype("CLIP_ASSAULT")],7);
         cr.reload(false);
         break;
      case CREATURE_HICK:
         switch(LCSrandom(7))
         {
            case 0:strcpy(cr.name,"Country Boy");break;
			case 1:strcpy(cr.name,"Good ol' Boy");break;
			case 2:strcpy(cr.name,"Hick");break;
			case 3:strcpy(cr.name,"Hillbilly");break;
			case 4:strcpy(cr.name,"Redneck");break;
            case 5:strcpy(cr.name,"Rube");break;
            case 6:strcpy(cr.name,"Yokel");break;
         }
         if((law[LAW_GUNCONTROL]==-2&&!LCSrandom(2))||!LCSrandom(10))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
         }
         else
            cr.give_weapon(*weapontype[getweapontype(LCSrandom(2)?"WEAPON_TORCH":"WEAPON_PITCHFORK")],NULL);
         cr.reload(false);
         break;
      case CREATURE_COP:
         if(law[LAW_POLICEBEHAVIOR]==2 && cr.align==ALIGN_LIBERAL && !LCSrandom(3)) // Peace Officer
         {
            cr.align=ALIGN_MODERATE;
            strcpy(cr.name,"Police Negotiator");
            cr.set_skill(SKILL_PERSUASION,LCSrandom(4)+1);
            cr.set_skill(SKILL_PISTOL,LCSrandom(3)+1);
            cr.set_attribute(ATTRIBUTE_HEART,4);
         }
         else
         {
            if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
            {
               cr.give_weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")],NULL);
               cr.take_clips(*cliptype[getcliptype("CLIP_SMG")],4);
            }
            else if(!LCSrandom(3))
            {
               cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")],NULL);
               cr.take_clips(*cliptype[getcliptype("CLIP_9")],4);
            }
            else if(!LCSrandom(2))
            {
               cr.give_weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")],NULL);
               cr.take_clips(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
            }
            else
               cr.give_weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")],NULL);
            cr.reload(false);
            cr.align=ALIGN_CONSERVATIVE;
            cr.set_skill(SKILL_PISTOL,LCSrandom(4)+1);
            cr.set_skill(SKILL_SHOTGUN,LCSrandom(3)+1);
            cr.set_skill(SKILL_CLUB,LCSrandom(2)+1);
            cr.set_skill(SKILL_HANDTOHAND,LCSrandom(2)+1);
            cr.set_attribute(ATTRIBUTE_WISDOM,4);
         }
         break;
      case CREATURE_FIREFIGHTER:
         if(law[LAW_FREESPEECH]==-2)
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_FLAMETHROWER")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_GASOLINE")],4);
            cr.reload(false);
            cr.set_skill(SKILL_HEAVYWEAPONS,LCSrandom(3)+2);
            strcpy(cr.name,"Fireman");
            cr.align=ALIGN_CONSERVATIVE;
         }
         else
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_AXE")],NULL);
            cr.set_skill(SKILL_AXE,LCSrandom(3)+2);
            strcpy(cr.name,"Firefighter");
         }
         if(sitealarm) // Respond to emergencies in bunker gear
            cr.give_armor(*armortype[getarmortype("ARMOR_BUNKERGEAR")],NULL);
         break;
      case CREATURE_CCS_MOLOTOV:
         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
            nameCCSMember(cr);
         break;
      case CREATURE_CCS_SNIPER:
         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
            nameCCSMember(cr);
         break;
      case CREATURE_CCS_VIGILANTE:
         cr.give_armor(*armortype[getarmortype("ARMOR_CLOTHES")],NULL);
         switch(LCSrandom(5)+endgamestate)
         {
         case 0:
         case 1:
            break;
         case 2:
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_9")],7);
            break;
         }
         case 3:
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_44")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_44")],7);
            break;
         }
         case 4:
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_BUCKSHOT")],7);
            break;
         }
         case 5:
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIRIFLE_AR15")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.give_armor(*armortype[getarmortype("ARMOR_CIVILLIANARMOR")],NULL);
            break;
         }
         case 6:
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIRIFLE_AR15")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.give_armor(*armortype[getarmortype("ARMOR_ARMYARMOR")],NULL);
            break;
         }
         default:
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_M16")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_ASSAULT")],7);
            cr.give_armor(*armortype[getarmortype("ARMOR_ARMYARMOR")],NULL);
            break;
         }
         }
         cr.reload(false);
         if(mode==GAMEMODE_SITE/* && sitealarm>0*/)
            nameCCSMember(cr);
         break;
      case CREATURE_CCS_ARCHCONSERVATIVE:
         strcpy(cr.name,(location[cursite]->siege.siege?"CCS Team Leader":(ccs_kills<2?"CCS Lieutenant":"CCS Founder")));
         break;
      case CREATURE_PRISONGUARD:
         if(law[LAW_GUNCONTROL]==-2&&!LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_SMG")],4);
         }
         else if(!LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
         }
         else
            cr.give_weapon(*weapontype[getweapontype("WEAPON_NIGHTSTICK")],NULL);
         cr.reload(false);
         break;
      case CREATURE_EDUCATOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_SMG")],4);
         }
         else if(!LCSrandom(3))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_9")],4);
         }
         else
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SYRINGE")],NULL);
         cr.reload(false);
         break;
      case CREATURE_GENETIC:
         if(location[cursite]->type==SITE_CORPORATE_HOUSE)
         {
            strcpy(cr.name,"Pet ");
            attcap[ATTRIBUTE_CHARISMA]=10;
         }
         else
            strcpy(cr.name,"");

         switch(LCSrandom(11))
         {
         case 0:strcat(cr.name,"Genetic Monster");break;
         case 1:strcat(cr.name,"Flaming Rabbit");
            cr.specialattack=ATTACK_FLAME;break;
         case 2:strcat(cr.name,"Genetic Nightmare");break;
         case 3:strcat(cr.name,"Mad Cow");break;
         case 4:strcat(cr.name,"Giant Mosquito");
            cr.specialattack=ATTACK_SUCK;break;
         case 5:strcat(cr.name,"Six-legged Pig");break;
         case 6:strcat(cr.name,"Purple Gorilla");break;
         case 7:strcat(cr.name,"Warped Bear");break;
         case 8:strcat(cr.name,"Writhing Mass");break;
         case 9:strcat(cr.name,"Something Bad");break;
         case 10:strcat(cr.name,"Pink Elephant");break;
         }
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         if(law[LAW_ANIMALRESEARCH]!=2)cr.money=0;
         break;
      case CREATURE_GUARDDOG:
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         if(law[LAW_ANIMALRESEARCH]!=2)cr.money=0;
         break;
      case CREATURE_PRISONER:
         // Prisoners should not be "prisoners" after recruiting them,
         // they should be some brand of criminal
         if(!LCSrandom(10))
         {
            // Thief
            makecreature(cr,CREATURE_THIEF);
         }
         else switch(LCSrandom(5))
         {
         case 0:
            // Gang member
            makecreature(cr,CREATURE_GANGMEMBER);
            break;
         case 1:
            // Prostitute
            makecreature(cr,CREATURE_PROSTITUTE);
            break;
         case 2:
            // Crack head
            makecreature(cr,CREATURE_CRACKHEAD);
            break;
         case 3:
            // Teenager
            makecreature(cr,CREATURE_TEENAGER);
            break;
         case 4:
            // HS Dropout
            makecreature(cr,CREATURE_HSDROPOUT);
            break;
         }

         cr.drop_weapons_and_clips(NULL);
         crtype->give_weapon(cr);
         cr.strip(NULL);
         crtype->give_armor(cr);
         cr.money=crtype->money_.roll();
         cr.juice=crtype->juice_.roll();
         cr.gender_liberal=cr.gender_conservative=crtype->roll_gender();
         strcpy(cr.name,crtype->get_encounter_name());
         if(cr.align==ALIGN_CONSERVATIVE)
            cr.align=LCSrandom(2);
         break;
      case CREATURE_BUM:
         crtype->give_weapon_civilian(cr);
         if(!cr.is_armed()&&!LCSrandom(5))
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SHANK")],NULL);
         if(cr.align==ALIGN_CONSERVATIVE)cr.align=LCSrandom(2);
         break;
      case CREATURE_MUTANT:
         crtype->give_weapon_civilian(cr);
         if(!cr.is_armed()&&!LCSrandom(5))
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SHANK")],NULL);
         break;
      case CREATURE_GANGMEMBER:
         if(!LCSrandom(20)||(law[LAW_GUNCONTROL]==-2&&!LCSrandom(5)))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_AUTORIFLE_AK47")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_ASSAULT")],3);
         }
         else if(!LCSrandom(16)||(law[LAW_GUNCONTROL]==-2&&!LCSrandom(5)))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SMG_MP5")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_SMG")],4);
         }
         else if(!LCSrandom(15))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_45")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_45")],4);
         }
         else if(!LCSrandom(10))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SHOTGUN_PUMP")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_BUCKSHOT")],4);
         }
         else if(!LCSrandom(4))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_9")],4);
         }
         else if(!LCSrandom(2))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_38")],4);
         }
         else
            cr.give_weapon(*weapontype[getweapontype("WEAPON_COMBATKNIFE")],NULL);
         cr.reload(false);
         // We'll make the crack house a bit dicey
         if(location[cursite]->type==SITE_BUSINESS_CRACKHOUSE)cr.align=ALIGN_CONSERVATIVE;
         if(!LCSrandom(2))switch(LCSrandom(3))
         {
         case 0://cr.crimes_committed[LAWFLAG_BROWNIES]++;
            cr.crimes_suspected[LAWFLAG_BROWNIES]++;break;
         case 1://cr.crimes_committed[LAWFLAG_ASSAULT]++;
            cr.crimes_suspected[LAWFLAG_ASSAULT]++;break;
         case 2://cr.crimes_committed[LAWFLAG_MURDER]++;
            cr.crimes_suspected[LAWFLAG_MURDER]++;break;
         }
         break;
      case CREATURE_CRACKHEAD:
         crtype->give_weapon_civilian(cr);
         if(!LCSrandom(5))
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SHANK")],NULL);
         if(cr.align==ALIGN_CONSERVATIVE)cr.align=LCSrandom(2);
         attcap[ATTRIBUTE_HEALTH]=1+LCSrandom(5);
         break;
      case CREATURE_FASTFOODWORKER:
         cr.age=(LCSrandom(2)?AGE_TEENAGER:AGE_YOUNGADULT);
         break;
      case CREATURE_FOOTBALLCOACH:
         if(LCSrandom(2))
         {
            cr.set_attribute(ATTRIBUTE_HEALTH,5);
            cr.set_attribute(ATTRIBUTE_AGILITY,5);
            cr.set_attribute(ATTRIBUTE_STRENGTH,5);
         }
         break;
      case CREATURE_PROSTITUTE:
         if(LCSrandom(7))cr.gender_conservative=cr.gender_liberal=GENDER_FEMALE;
         else if(!LCSrandom(3))cr.gender_liberal=GENDER_FEMALE;
         if(cr.align==ALIGN_CONSERVATIVE)cr.align=LCSrandom(2);
         if(!LCSrandom(3))cr.crimes_suspected[LAWFLAG_PROSTITUTION]++;
         break;
      case CREATURE_HIPPIE:
         if(!LCSrandom(10))
            cr.crimes_suspected[LAWFLAG_BROWNIES]++;
         break;
      case CREATURE_SOCIALITE:
         cr.give_armor(*armortype[getarmortype(cr.gender_liberal==GENDER_FEMALE?"ARMOR_EXPENSIVEDRESS":"ARMOR_EXPENSIVESUIT")],NULL);
         break;
      case CREATURE_THIEF:
         switch(LCSrandom(5))
         {
            case 0:strcpy(cr.name,getcreaturetype(CREATURE_SOCIALITE)->get_encounter_name());break;
            case 1:strcpy(cr.name,getcreaturetype(CREATURE_CLERK)->get_encounter_name());break;
            case 2:strcpy(cr.name,getcreaturetype(CREATURE_OFFICEWORKER)->get_encounter_name());break;
            case 3:strcpy(cr.name,getcreaturetype(CREATURE_CRITIC_ART)->get_encounter_name());break;
            case 4:strcpy(cr.name,getcreaturetype(CREATURE_CRITIC_MUSIC)->get_encounter_name());break;
         }

         if(!LCSrandom(10))cr.crimes_suspected[(LCSrandom(2)?LAWFLAG_BREAKING:LAWFLAG_THEFT)]++;
         break;
      case CREATURE_MILITARYOFFICER:
         if(LCSrandom(4))
         {
            cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")],NULL);
            cr.take_clips(*cliptype[getcliptype("CLIP_9")],4);
            cr.reload(false);
         }
         break;
      case CREATURE_JUDGE_LIBERAL:
      case CREATURE_POLITICIAN:
      case CREATURE_CORPORATE_MANAGER:
      case CREATURE_WORKER_SERVANT:
      case CREATURE_WORKER_JANITOR:
      case CREATURE_WORKER_SECRETARY:
      case CREATURE_LANDLORD:
      case CREATURE_BANK_TELLER:
      case CREATURE_BANK_MANAGER:
      case CREATURE_TEENAGER:
      case CREATURE_SOLDIER:
      case CREATURE_VETERAN:
      case CREATURE_HARDENED_VETERAN:
      case CREATURE_SWAT:
      case CREATURE_DEATHSQUAD:
      case CREATURE_GANGUNIT:
      case CREATURE_AGENT:
      case CREATURE_SECRET_SERVICE:
      case CREATURE_RADIOPERSONALITY:
      case CREATURE_NEWSANCHOR:
      case CREATURE_JUROR:
      case CREATURE_WORKER_FACTORY_CHILD:
      case CREATURE_SEWERWORKER:
      case CREATURE_COLLEGESTUDENT:
      case CREATURE_MUSICIAN:
      case CREATURE_MATHEMATICIAN:
      case CREATURE_TEACHER:
      case CREATURE_HSDROPOUT:
      case CREATURE_PRIEST:
      case CREATURE_ENGINEER:
      case CREATURE_TELEMARKETER:
      case CREATURE_CARSALESMAN:
      case CREATURE_OFFICEWORKER:
      case CREATURE_MAILMAN:
      case CREATURE_GARBAGEMAN:
      case CREATURE_PLUMBER:
      case CREATURE_CHEF:
      case CREATURE_CONSTRUCTIONWORKER:
      case CREATURE_AMATEURMAGICIAN:
      case CREATURE_AUTHOR:
      case CREATURE_JOURNALIST:
      case CREATURE_CRITIC_ART:
      case CREATURE_CRITIC_MUSIC:
      case CREATURE_BIKER:
      case CREATURE_TRUCKER:
      case CREATURE_TAXIDRIVER:
      case CREATURE_PROGRAMMER:
      case CREATURE_NUN:
      case CREATURE_RETIREE:
      case CREATURE_PAINTER:
      case CREATURE_SCULPTOR:
      case CREATURE_DANCER:
      case CREATURE_PHOTOGRAPHER:
      case CREATURE_CAMERAMAN:
      case CREATURE_HAIRSTYLIST:
      case CREATURE_FASHIONDESIGNER:
      case CREATURE_CLERK:
      case CREATURE_ACTOR:
      case CREATURE_YOGAINSTRUCTOR:
      case CREATURE_MARTIALARTIST:
      case CREATURE_ATHLETE:
      case CREATURE_LOCKSMITH:
      case CREATURE_MILITARYPOLICE:
      case CREATURE_SEAL:
         break;
   }

   vector<int> possible;
   for(int a=0;a<ATTNUM;a++)
   {
      attnum-=min(4,cr.get_attribute(a,false));
      possible.push_back(a);
   }
   while(attnum>0&&!possible.empty())
   {
      int i=LCSrandom(possible.size());
      int a=possible[i];
      if(a==ATTRIBUTE_WISDOM&&cr.align==ALIGN_LIBERAL&&LCSrandom(4))a=ATTRIBUTE_HEART;
      if(a==ATTRIBUTE_HEART&&cr.align==ALIGN_CONSERVATIVE&&LCSrandom(4))a=ATTRIBUTE_WISDOM;
      if(cr.get_attribute(a,false)<attcap[a])
      {
         cr.adjust_attribute(a,+1);
         attnum--;
      }
      else
         possible.erase(possible.begin()+i);
   }
   if(cr.align==ALIGN_LIBERAL)cr.infiltration=0.15f+(LCSrandom(10)-5)*0.01f;
   else if(cr.align==ALIGN_MODERATE)cr.infiltration=0.25f+(LCSrandom(10)-5)*0.01f;
   else cr.infiltration+=0.35f*(1-cr.infiltration)+(LCSrandom(10)-5)*0.01f;
   if(cr.infiltration<0)cr.infiltration=0;
   if(cr.infiltration>1)cr.infiltration=1;
   int randomskills=LCSrandom(4)+4;
   if(cr.age>20)randomskills+=static_cast<int>(randomskills*((cr.age-20.0)/20.0));
   else randomskills-=(20-cr.age)/2;
   possible.clear();
   for(int s=0;s<SKILLNUM;s++)possible.push_back(s);
   //RANDOM STARTING SKILLS
   while(randomskills>0&&!possible.empty())
   {
      int i=LCSrandom(possible.size());
      int randomskill=possible[i];
      // 95% chance of not allowing some skills for anybody...
      if(LCSrandom(20))
      {
         if(randomskill==SKILL_HEAVYWEAPONS)continue;
         if(randomskill==SKILL_SMG)continue;
         if(randomskill==SKILL_SWORD)continue;
         if(randomskill==SKILL_RIFLE)continue;
         if(randomskill==SKILL_AXE)continue;
         if(randomskill==SKILL_CLUB)continue;
         if(randomskill==SKILL_PSYCHOLOGY)continue;
      }
      // 90% chance of not allowing some skills, other than
      //   for conservatives
      if(LCSrandom(10)&&cr.align!=ALIGN_CONSERVATIVE)
      {
         if(randomskill==SKILL_SHOTGUN)continue;
         if(randomskill==SKILL_PISTOL)continue;
      }
      if(cr.skill_cap(randomskill,true)>cr.get_skill(randomskill))
      {
         cr.set_skill(randomskill,cr.get_skill(randomskill)+1);
         randomskills--;
         while(randomskills&&LCSrandom(2))
         {
            if(cr.skill_cap(randomskill,true)>cr.get_skill(randomskill) &&
               cr.get_skill(randomskill) < 4)
            {
               cr.set_skill(randomskill,cr.get_skill(randomskill)+1);
               randomskills--;
            }
            else
            {
               possible.erase(possible.begin()+i);
               break;
            }
         }
      }
      else possible.erase(possible.begin()+i);
   }
   //ALIENATION
   if((sitealienate>=1&&cr.align==ALIGN_MODERATE)||(sitealienate==2&&cr.align==ALIGN_LIBERAL))conservatise(cr);
}

/* ensures that the creature's work location is appropriate to its type */
bool verifyworklocation(Creature &cr, char test_location, char test_type)
{
   int okaysite[SITENUM];
   memset(okaysite,0,SITENUM*sizeof(int));
   // If the caller sets test_type, they're just
   // asking if the chosen creature type is appropriate
   // to the location they provided, not actually setting
   // the creature work location -- this is useful
   // for things like stealth
   short type=(test_type!=-1?test_type:cr.type);
   switch(type)
   {
      case CREATURE_BOUNCER:
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         break;
      case CREATURE_POLITICIAN:
         okaysite[SITE_GOVERNMENT_WHITE_HOUSE]=1;
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
         okaysite[SITE_GOVERNMENT_WHITE_HOUSE]=1;
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
      case CREATURE_SECRET_SERVICE:
         okaysite[SITE_GOVERNMENT_WHITE_HOUSE]=1;
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
         okaysite[SITE_GOVERNMENT_WHITE_HOUSE]=1;
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
	  case CREATURE_BAKER:
		 okaysite[SITE_DOWNTOWN]=1;
		 okaysite[SITE_UDISTRICT]=1;
		 okaysite[SITE_INDUSTRIAL]=1;
	     break;
	  case CREATURE_BARISTA:
		 okaysite[SITE_BUSINESS_LATTESTAND]=1;
		 okaysite[SITE_BUSINESS_INTERNETCAFE]=1;
	     break;  
	  case CREATURE_BARTENDER:
		 okaysite[SITE_BUSINESS_CIGARBAR]=1;
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
         okaysite[SITE_GOVERNMENT_WHITE_HOUSE]=1;
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
   if(test_type!=-1) return okaysite[(int)test_location];
   char swap=0;
   if(cr.worklocation==-1)swap=1;
   else if(!okaysite[(int)location[(int)cr.worklocation]->type])swap=1;
   if(swap)
   {
      //int city = location[cr.location]->city;
      //PICK A TYPE OF WORK LOCATION
      //cr.worklocation=choose_one(okaysite,SITENUM,0);
      //FIND ONE OF THESE
      vector<int> goodlist;
      //find_site_index_in_city(cr.worklocation, location[cr.location]->city);
      for(int l=0;l<(int)location.size();l++)
         //if(location[l]->type==cr.worklocation && (!multipleCityMode || location[l]->city == cr.location))
         if(okaysite[(int)location[l]->type] && (!multipleCityMode || location[l]->city == location[(int)cr.location]->city))
            goodlist.push_back(l);
// Sadler - This line sometimes causes a memory fault
//               Only thing I can think of is if loop above didn'
//               find any locations of type == to cr.worklocation
//               My hunch is that some locations, such as the 1st four
//               are special and cannot be used here..
//
//   TODO There was a bug in the makecharacter() code where th
//   SITE_OUTOFTOWN was not set properly. This was fixed but the bug here
//   is still occuring, normally at the Latte Bar Downtown ;
      if (goodlist.size()==0) cr.worklocation=0;
      else cr.worklocation=pickrandom(goodlist);
   }
   return false;
}
