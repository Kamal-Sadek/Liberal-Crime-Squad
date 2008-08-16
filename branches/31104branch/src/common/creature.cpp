/*

Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
                                                                                      //
This file is part of Liberal Crime Squad.                                             //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/

/*
	This file was created by Chris Johnson (grundee@users.sourceforge.net)
	by copying code from game.cpp.
	To see descriptions of files and functions, see the list at 
	the bottom of includes.h in the top src folder.
*/

//#include <includes.h>
#include <externs.h>

/* Age macros for characters */
#define AGE_DOGYEARS    2+LCSrandom(5)   /* for the animals */
#define AGE_CHILD       7+LCSrandom(4)   /* child laborer */
#define AGE_TEENAGER    14+LCSrandom(4)  /* HS dropout, teenager, some fast food workers */
#define AGE_YOUNGADULT  18+LCSrandom(18) /* young lads and ladies */
#define AGE_MATURE      20+LCSrandom(40) /* most folks */
#define AGE_GRADUATE    26+LCSrandom(34) /* most folks, with 6-8 years college */
#define AGE_MIDDLEAGED  35+LCSrandom(25) /* for the more experienced types */
#define AGE_SENIOR      65+LCSrandom(30) /* ah, social security */


//TEMPORARY FUNNNCCCTIIOOONNNN
//TEMPOR-FREAKIN-RARY!
int getSpawnChance(enum CreatureType type)
{
	switch(type)
	{
		case CREATURE_CCS_VIGILANTE:
			return 90;
		case CREATURE_CCS_ARCHCONSERVATIVE:
			return 10;
		case CREATURE_COP:
			return 80;
		case CREATURE_SWAT:
			return 20;
		case CREATURE_DEATHSQUAD:
			if(law[LAW_POLICEBEHAVIOR] == -2)
			{
				return 50;
			}
			else
			{
				return 0;
			}
		case CREATURE_GANGUNIT:
			if(law[LAW_POLICEBEHAVIOR] == -1)
			{
				return 70;
			}
			else
			{
				return 0;
			}
		case CREATURE_HICK:
			return 100;
		case CREATURE_RADIOPERSONALITY:
			return 20;
		case CREATURE_NEWSANCHOR:
			return 20;
		case CREATURE_TANK:
			return 1;
		case CREATURE_SOLDIER:
			return 99;
		case CREATURE_AGENT:
			return 20;
	}
	return 100;
}
/* rolls up a creature's stats and equipment */
void makecreature(creaturest &cr,short type)
{
 int a = 0;
   cr.creatureinit();

   cr.exists=1;
   cr.squadid=-1;
   cr.type=type;
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

   int randomskills=LCSrandom(5)+5;

   int redistatts=0;
   int attcap[ATTNUM];
   for(a=0;a<ATTNUM;a++)attcap[a]=10;

   int sk;

   switch(type)
   {
      case CREATURE_BOUNCER:
         cr.weapon.type=WEAPON_NONE;
         if(mode==GAMEMODE_SITE && location[cursite]->highsecurity)
         {
            strcpy(cr.name,"Enforcer");
            cr.weapon.type=WEAPON_REVOLVER_44;
            cr.weapon.ammo=6;
            cr.clip[CLIP_44]=3;
            
            sk=LCSrandom(3)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         }
         cr.armor.type=ARMOR_SECURITYUNIFORM;
         sk=LCSrandom(3)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         if(disguisesite(sitetype))cr.align=-1;
         else cr.align=0;
         cr.age=AGE_MATURE;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=23;
         cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_AGILITY]=3;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         break;
      case CREATURE_SECURITYGUARD:
         if(law[LAW_GUNCONTROL]==-2)
         {
            cr.weapon.type=WEAPON_SMG_MP5;
            cr.weapon.ammo=15;
            cr.clip[CLIP_SMG]=3;
         }
         else
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.armor.type=ARMOR_SECURITYUNIFORM;
         sk=LCSrandom(3)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         cr.align=-1;
         cr.age=AGE_MATURE;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=25;
         cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_AGILITY]=3;
         cr.att[ATTRIBUTE_STRENGTH]=2;
         break;
      case CREATURE_SCIENTIST_LABTECH:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         else if(!LCSrandom(2))cr.weapon.type=WEAPON_SYRINGE;
         cr.armor.type=ARMOR_LABCOAT;
         cr.align=-1;
         cr.age=AGE_MATURE;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
         cr.att[ATTRIBUTE_INTELLIGENCE]=5;
         sk=LCSrandom(2)+1;cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
         sk=LCSrandom(4)+3;cr.skill[SKILL_SCIENCE]=sk;randomskills-=sk;
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         strcpy(cr.name,"Hangin' Judge");
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
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         randomskills*=2; // Extra skills
         sk=LCSrandom(6)+5;cr.skill[SKILL_LAW]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
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

         randomskills*=2; // Extra skills
         sk=LCSrandom(6)+5;cr.skill[SKILL_LAW]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
         cr.att[ATTRIBUTE_INTELLIGENCE]=5;
         cr.att[ATTRIBUTE_HEART]=10;
         break;
      case CREATURE_SCIENTIST_EMINENT:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         else if(!LCSrandom(2))cr.weapon.type=WEAPON_SYRINGE;
         cr.armor.type=ARMOR_LABCOAT;
         cr.money=LCSrandom(41)+20;
         cr.align=-1;
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         randomskills*=2; // Extra skills
         sk=LCSrandom(3)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         sk=LCSrandom(6)+6;cr.skill[SKILL_SCIENCE]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
         cr.att[ATTRIBUTE_INTELLIGENCE]=10;
         cr.att[ATTRIBUTE_WISDOM]=6;
         break;
      case CREATURE_CORPORATE_MANAGER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(41)+40;
         cr.align=-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+3;cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=21;
			cr.att[ATTRIBUTE_INTELLIGENCE]=2;
			cr.att[ATTRIBUTE_CHARISMA]=5;
         cr.att[ATTRIBUTE_WISDOM]=5;
         break;
      case CREATURE_CORPORATE_CEO:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_44;
            cr.weapon.ammo=6;
            cr.clip[CLIP_44]=3;
         }
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(121)+120;
         cr.align=-1;
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MIDDLEAGED;

         randomskills*=2; // Extra skills
         sk=LCSrandom(6)+6;cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=8;
			cr.att[ATTRIBUTE_INTELLIGENCE]=6;
			cr.att[ATTRIBUTE_CHARISMA]=7;
         cr.att[ATTRIBUTE_WISDOM]=12;
         break;
      case CREATURE_WORKER_SERVANT:
         cr.money=0;
         cr.align=1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_JANITOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.age=AGE_MATURE;
         break;
      case CREATURE_WORKER_SWEATSHOP:
         cr.armor.type=ARMOR_NONE;
         cr.align=1;
         cr.age=AGE_MATURE;
         cr.juice=-20;
         cr.flag|=CREATUREFLAG_ILLEGALALIEN;
         sk=LCSrandom(6)+1;cr.skill[SKILL_GARMENTMAKING]=sk;randomskills-=sk;
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Nonunion Worker");
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         if(cr.align==1)
            cr.align=LCSrandom(2)-1;
         cr.age=AGE_MATURE;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
         cr.att[ATTRIBUTE_STRENGTH]=5;
         break;
      case CREATURE_WORKER_SECRETARY:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         sk=LCSrandom(2)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         break;
      case CREATURE_LANDLORD:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Landlord");
         cr.money=LCSrandom(121)+120;
         cr.align=0;
         cr.age=AGE_MATURE;
         sk=LCSrandom(4)+3;cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         break;
      case CREATURE_TEENAGER:
         //cr.align=LCSrandom(3)-1;
         randomskills>>=1;
         cr.age=AGE_TEENAGER;
         break;
      case CREATURE_LAWYER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=0;
         }
         strcpy(cr.name,"Lawyer");
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(51)+50;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         randomskills*=2; // Extra skills
         sk=LCSrandom(4)+4;cr.skill[SKILL_LAW]=sk;randomskills-=sk;
			randomskills-=cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         break;
      case CREATURE_DOCTOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=0;
         }
         strcpy(cr.name,"Doctor");
         cr.armor.type=ARMOR_LABCOAT;
         cr.money=LCSrandom(21)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         randomskills*=2; // Extra skills
         sk=LCSrandom(4)+4;cr.skill[SKILL_MEDICAL]=sk;randomskills-=sk;
         break;
      case CREATURE_NURSE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=0;
         }
         strcpy(cr.name,"Nurse");
         cr.armor.type=ARMOR_LABCOAT;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_GRADUATE;

         randomskills*=2; // Extra skills
         sk=LCSrandom(4)+1;cr.skill[SKILL_MEDICAL]=sk;randomskills-=sk;
         break;
      case CREATURE_WORKER_FACTORY_UNION:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Union Worker");
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.align=1;
         cr.age=AGE_MATURE;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
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
         strcpy(cr.name,"Mercenary");
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
         cr.juice=LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         sk=LCSrandom(4)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;
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
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(2))
         {
            cr.weapon.type=WEAPON_SHOTGUN_PUMP;
            cr.weapon.ammo=6;
            cr.clip[CLIP_BUCKSHOT]=3;
         }
         else if(!LCSrandom(2))cr.weapon.type=WEAPON_TORCH;
         else cr.weapon.type=WEAPON_PITCHFORK;

         if(!LCSrandom(2))cr.armor.type=ARMOR_OVERALLS;
         else cr.armor.type=ARMOR_WIFEBEATER;

         cr.money=LCSrandom(6)+6;
         cr.align=-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_SOLDIER:
         strcpy(cr.name,"Soldier");
         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_ARMYARMOR;
         cr.money=0;
         cr.align=-1;
         cr.juice=LCSrandom(100);
         cr.age=AGE_YOUNGADULT;

         sk=LCSrandom(4)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=18;
         cr.att[ATTRIBUTE_STRENGTH]=5;
			cr.att[ATTRIBUTE_AGILITY]=5;
			cr.att[ATTRIBUTE_HEALTH]=5;
         break;
      case CREATURE_COP:
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
         cr.juice=10+LCSrandom(50);
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=4;
         break;
      case CREATURE_SWAT:
         if(LCSrandom(3))
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
         cr.armor.type=ARMOR_POLICEARMOR;
         cr.align=-1;
         cr.juice=40+LCSrandom(50);
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(4)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(4)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(4)+1;cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=24;
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=4;
         break;
      case CREATURE_DEATHSQUAD:
         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_ARMYARMOR;
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.juice=90+LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         sk=LCSrandom(4)+2;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=17;
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=6;
         break;
      case CREATURE_CCS_MOLOTOV:
      {
         cr.armor.type=ARMOR_TRENCHCOAT;
         cr.weapon.type=WEAPON_MOLOTOV;
         cr.weapon.ammo=1;
         cr.clip[CLIP_MOLOTOV]=4;

         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+2;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_IMPROVISED]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         redistatts=9;
         cr.att[ATTRIBUTE_STRENGTH]=6;
			cr.att[ATTRIBUTE_AGILITY]=6;
			cr.att[ATTRIBUTE_HEALTH]=6;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      }
      case CREATURE_CCS_SNIPER:
      {
         cr.armor.type=ARMOR_TRENCHCOAT;
         cr.weapon.type=WEAPON_SEMIRIFLE_AR15;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;

         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+2;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(4)+6;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         redistatts=9;
         cr.att[ATTRIBUTE_STRENGTH]=6;
			cr.att[ATTRIBUTE_AGILITY]=6;
			cr.att[ATTRIBUTE_HEALTH]=6;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      }
      case CREATURE_CCS_VIGILANTE:
      {
         cr.armor.type=ARMOR_CLOTHES;
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
         if(mode==GAMEMODE_SITE && sitealarm>0)
         {
            if(cr.armor.type==ARMOR_CIVILLIANARMOR)
            {
               strcpy(cr.name,"Mercenary");
            }
            else if(cr.armor.type==ARMOR_ARMYARMOR)
            {
               strcpy(cr.name,"Soldier");
            }
            else if(cr.weapon.type==WEAPON_SHOTGUN_PUMP||LCSrandom(2))
            {
               switch(LCSrandom(5))
               {
                  case 0:strcpy(cr.name,"Country Boy");break;
                  case 1:strcpy(cr.name,"Hick");break;
                  case 2:strcpy(cr.name,"Redneck");break;
                  case 3:strcpy(cr.name,"Rube");break;
                  case 4:strcpy(cr.name,"Yokel");break;
               }
            }
            else
            {
               switch(LCSrandom(9))
               {
               case 0: strcpy(cr.name,"Biker");break;
               case 1: strcpy(cr.name,"Transient");break;
               case 2: strcpy(cr.name,"Crackhead");break;
               case 3: strcpy(cr.name,"Fast Food Worker");break;
               case 4: strcpy(cr.name,"Telemarketer");break;
               case 5: strcpy(cr.name,"Office Worker");break;
               case 6: strcpy(cr.name,"Mailman");break;
               case 7: strcpy(cr.name,"Musician");break;
               case 8: strcpy(cr.name,"Hairstylist");break;
               }
            }
         }
         cr.money=LCSrandom(21)+20;
         cr.align=-1;
         cr.juice=90+LCSrandom(120);
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+3;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(4)+3;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(4)+3;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=20;
         }
         attcap[ATTRIBUTE_HEART]=4;
         redistatts=15;
         cr.att[ATTRIBUTE_STRENGTH]=4;
			cr.att[ATTRIBUTE_AGILITY]=4;
			cr.att[ATTRIBUTE_HEALTH]=4;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      }
      case CREATURE_CCS_ARCHCONSERVATIVE:
         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=9;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_HEAVYARMOR;
         cr.money=LCSrandom(51)+150;
         cr.align=-1;
         cr.juice=500+LCSrandom(250);
         cr.age=AGE_MIDDLEAGED;

         randomskills*=2; // Extra skills
         sk=LCSrandom(4)+6;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(4)+6;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(4)+6;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(4)+4;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(4)+4;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(6);cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;
         sk=LCSrandom(6);cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         sk=LCSrandom(6)+2;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=25;
         }
         attcap[ATTRIBUTE_HEART]=1;
         redistatts=25;
         cr.att[ATTRIBUTE_STRENGTH]=4;
			cr.att[ATTRIBUTE_AGILITY]=4;
			cr.att[ATTRIBUTE_HEALTH]=4;
         cr.att[ATTRIBUTE_WISDOM]=10;
         break;
      case CREATURE_GANGUNIT:
         
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
         cr.juice=40+LCSrandom(50);
         cr.age=AGE_YOUNGADULT;

         sk=LCSrandom(3)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(4)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;

			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=5;
         break;
      case CREATURE_PRISONGUARD:
         strcpy(cr.name,"Prison Guard");
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
         cr.age=AGE_MATURE;

         sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(3)+2;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
			
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=24;
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
         cr.age=AGE_MATURE;

         sk=LCSrandom(3)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_CLUB]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(4)+3;cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;
			
			for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
         cr.att[ATTRIBUTE_STRENGTH]=3;
			cr.att[ATTRIBUTE_AGILITY]=3;
			cr.att[ATTRIBUTE_HEALTH]=3;
         cr.att[ATTRIBUTE_WISDOM]=4;
         break;
      case CREATURE_AGENT:
         strcpy(cr.name,"Agent");
         switch(LCSrandom(10))
         {
            case 0:
               cr.weapon.type=WEAPON_REVOLVER_22;
               cr.clip[CLIP_22]=3;
               cr.weapon.ammo=6;
               break;
            case 1:
               cr.weapon.type=WEAPON_REVOLVER_44;
               cr.clip[CLIP_44]=3;
               cr.weapon.ammo=6;
               break;
            case 2:
               cr.weapon.type=WEAPON_SEMIPISTOL_9MM;
               cr.clip[CLIP_9]=3;
               cr.weapon.ammo=15;
               break;
            case 3:
               cr.weapon.type=WEAPON_SEMIPISTOL_45;
               cr.clip[CLIP_45]=3;
               cr.weapon.ammo=15;
               break;
            case 4:
               cr.weapon.type=WEAPON_AUTORIFLE_M16;
               cr.clip[CLIP_ASSAULT]=3;
               cr.weapon.ammo=30;
               break;
            case 5:
               cr.weapon.type=WEAPON_AUTORIFLE_AK47;
               cr.clip[CLIP_ASSAULT]=3;
               cr.weapon.ammo=30;
               break;
            case 6:
               cr.weapon.type=WEAPON_SHOTGUN_PUMP;
               cr.clip[CLIP_BUCKSHOT]=3;
               cr.weapon.ammo=6;
               break;
            case 7:
               cr.weapon.type=WEAPON_CARBINE_M4;
               cr.clip[CLIP_ASSAULT]=3;
               cr.weapon.ammo=30;
               break;
            case 8:
               cr.weapon.type=WEAPON_SMG_MP5;
               cr.clip[CLIP_SMG]=3;
               cr.weapon.ammo=15;
               break;
         }
         cr.armor.type=ARMOR_BLACKSUIT;
         cr.align=-1;
         cr.juice=75+LCSrandom(150);
         cr.age=AGE_MATURE;

         sk=LCSrandom(8)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(8)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(8)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(8)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         sk=LCSrandom(8)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         sk=LCSrandom(8)+1;cr.skill[SKILL_INTERROGATION]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=15;
         cr.att[ATTRIBUTE_STRENGTH]=5;
         cr.att[ATTRIBUTE_AGILITY]=5;
         cr.att[ATTRIBUTE_HEALTH]=5;
         cr.att[ATTRIBUTE_WISDOM]=5;
         break;
      case CREATURE_RADIOPERSONALITY:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Radio Personality");
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(51)+50;
         cr.align=-1;
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MATURE;

         sk=LCSrandom(8)+1;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=16;
         cr.att[ATTRIBUTE_CHARISMA]=10;
         cr.att[ATTRIBUTE_WISDOM]=8;
         break;
      case CREATURE_NEWSANCHOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"News Anchor");
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(51)+50;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=16;
         cr.att[ATTRIBUTE_CHARISMA]=10;
         cr.att[ATTRIBUTE_WISDOM]=8;
         cr.align=-1;
         cr.juice=100+LCSrandom(50);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GENETIC:
         switch(LCSrandom(10))
         {
            case 0:strcpy(cr.name,"Genetic Monster");break;
            case 1:
            {
               strcpy(cr.name,"Flaming Rabbit");
               cr.specialattack=ATTACK_FLAME;
               break;
            }
            case 2:strcpy(cr.name,"Genetic Nightmare");break;
            case 3:strcpy(cr.name,"Mad Cow");break;
            case 4:
            {
               strcpy(cr.name,"Giant Mosquito");
               cr.specialattack=ATTACK_SUCK;
               break;
            }
            case 5:strcpy(cr.name,"Six-legged Pig");break;
            case 6:strcpy(cr.name,"Purple Gorilla");break;
            case 7:strcpy(cr.name,"Warped Bear");break;
            case 8:strcpy(cr.name,"Writhing Mass");break;
            case 9:strcpy(cr.name,"Something Bad");break;
         }
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.armor.type=ARMOR_NONE;
         cr.align=-1;
         cr.age=AGE_DOGYEARS;
         break;
      case CREATURE_GUARDDOG:
         strcpy(cr.name,"Guard Dog");
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.armor.type=ARMOR_NONE;
         cr.align=-1;
         cr.age=AGE_DOGYEARS;
         break;
      case CREATURE_PRISONER:
         strcpy(cr.name,"Prisoner");
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
            sk=LCSrandom(5)+3;cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
            sk=LCSrandom(5)+3;cr.skill[SKILL_DISGUISE]=sk;randomskills-=sk;
            sk=LCSrandom(5)+3;cr.skill[SKILL_STEALTH]=sk;randomskills-=sk;
            sk=LCSrandom(5)+3;cr.skill[SKILL_SLEIGHTOFHAND]=sk;randomskills-=sk;
            cr.type=CREATURE_THIEF;
            cr.age=AGE_MATURE;
         }
         else
         {
            switch(LCSrandom(5))
            {
            case 0:
               // Gang member
               sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
               sk=LCSrandom(2)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
               sk=LCSrandom(2)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
               cr.type=CREATURE_GANGMEMBER;
               cr.age=AGE_YOUNGADULT;
               break;
            case 1:
               // Prostitute
		         randomskills-=cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
               sk=LCSrandom(4)+2;cr.skill[SKILL_SEDUCTION]=sk;randomskills-=sk;
               cr.type=CREATURE_PROSTITUTE;
               cr.age=AGE_YOUNGADULT;
               break;
            case 2:
               // Crack head
               //NOTE: DO NOT REDISTRIBUTE ATTRIBUTES
               cr.att[ATTRIBUTE_INTELLIGENCE]=1;
               cr.att[ATTRIBUTE_HEALTH]>>=1;cr.att[ATTRIBUTE_HEALTH]++;
               cr.type=CREATURE_CRACKHEAD;
               cr.age=AGE_YOUNGADULT;
               break;
            case 3:
               // Teenager
               randomskills>>=1;
               cr.age=AGE_TEENAGER;
               cr.type=CREATURE_TEENAGER;
               break;
            case 4:
               // HS Dropout
               randomskills>>=1;
               cr.age=AGE_TEENAGER;
               cr.type=CREATURE_HSDROPOUT;
               break;
            }
         }
         break;
      case CREATURE_JUROR:
         strcpy(cr.name,"Angry Juror");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
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
         randomskills=1;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
         cr.att[ATTRIBUTE_HEART]=8;
         break;
      case CREATURE_SEWERWORKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Sewer Worker");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_COLLEGESTUDENT:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"College Student");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=18+LCSrandom(6); // no macro

         randomskills*=2; // Extra skills
			randomskills-=cr.skill[SKILL_COMPUTERS]=LCSrandom(2);
			randomskills-=cr.skill[SKILL_WRITING]=LCSrandom(2)+1;
         sk=LCSrandom(3);cr.skill[SKILL_SCIENCE]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_ART]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_MUSIC]=sk;randomskills-=sk;
         break;
      case CREATURE_MUSICIAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Musician");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(11);
         cr.age=AGE_MATURE;
         sk=LCSrandom(4)+2;cr.skill[SKILL_MUSIC]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_SEDUCTION]=sk;randomskills-=sk;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_MATHEMATICIAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Mathematician");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(3);cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_SCIENCE]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=26;
         cr.att[ATTRIBUTE_INTELLIGENCE]=7;
         break;
      case CREATURE_TEACHER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Teacher");
         cr.armor.type=ARMOR_CLOTHES;
         sk=LCSrandom(4)+3;cr.skill[SKILL_TEACHING]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_HSDROPOUT:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Highschool Dropout");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         randomskills>>=1;
         cr.age=AGE_TEENAGER;
         break;
      case CREATURE_BUM:
         strcpy(cr.name,"Transient");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         else if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_MUTANT:
         strcpy(cr.name,"Mutant");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         else if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.align=1;
         sk=LCSrandom(3)+1;cr.skill[SKILL_SURVIVAL]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)
         {
            cr.att[a]=1;
            attcap[a]=50;
         }
         redistatts=LCSrandom(55);
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GANGMEMBER:
         strcpy(cr.name,"Gang Member");

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
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }

         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_YOUNGADULT;

         sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         break;
      case CREATURE_CRACKHEAD:
         strcpy(cr.name,"Crack Head");
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         if(!LCSrandom(5))cr.weapon.type=WEAPON_KNIFE;
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.juice=-20;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_YOUNGADULT;

         //NOTE: DO NOT REDISTRIBUTE
         cr.att[ATTRIBUTE_INTELLIGENCE]=1;
         cr.att[ATTRIBUTE_HEALTH]>>=1;cr.att[ATTRIBUTE_HEALTH]++;
         break;
      case CREATURE_PRIEST:
         strcpy(cr.name,"Priest");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         sk=LCSrandom(5)+3;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;
         break;
      case CREATURE_ENGINEER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Engineer");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(2)+1;cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_SCIENCE]=sk;randomskills-=sk;
         break;
      case CREATURE_FASTFOODWORKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Fast Food Worker");
         cr.armor.type=ARMOR_CLOTHES;
         sk=LCSrandom(3);cr.skill[SKILL_COOKING]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         if(LCSrandom(2))
            cr.age=AGE_TEENAGER;
         else
            cr.age=AGE_YOUNGADULT;
         break;
      case CREATURE_TELEMARKETER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Telemarketer");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_YOUNGADULT;

			randomskills-=cr.skill[SKILL_PERSUASION]=LCSrandom(3)+1;
         break;
      case CREATURE_OFFICEWORKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Office Worker");
         cr.armor.type=ARMOR_CLOTHES;
         sk=LCSrandom(3);cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_FOOTBALLCOACH:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Football Coach");
         cr.armor.type=ARMOR_CLOTHES;
         randomskills*=2; // Extra skills
         sk=LCSrandom(3)+1;cr.skill[SKILL_TEACHING]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_LEADERSHIP]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MIDDLEAGED;

         if(LCSrandom(2))
         {
            for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
            cr.att[ATTRIBUTE_HEALTH]=5;
            cr.att[ATTRIBUTE_AGILITY]=5;
            cr.att[ATTRIBUTE_STRENGTH]=5;
         }
			randomskills-=cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         break;
      case CREATURE_PROSTITUTE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Prostitute");
         if(LCSrandom(2))cr.armor.type=ARMOR_CHEAPDRESS;
         else cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
         cr.age=AGE_YOUNGADULT;
			randomskills-=cr.skill[SKILL_PERSUASION]=LCSrandom(4)+2;
         sk=LCSrandom(4)+2;cr.skill[SKILL_SEDUCTION]=sk;randomskills-=sk;
         break;
      case CREATURE_MAILMAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Mail Carrier");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_GARBAGEMAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Garbage Collector");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_PLUMBER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Plumber");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CHEF:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Chef");
         sk=LCSrandom(5)+3;cr.skill[SKILL_COOKING]=sk;randomskills-=sk;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CONSTRUCTIONWORKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Construction Worker");
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=20;
         cr.att[ATTRIBUTE_STRENGTH]=7;
         cr.att[ATTRIBUTE_HEALTH]=7;
         sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_AMATEURMAGICIAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Amateur Magician");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         sk=LCSrandom(3)+2;cr.skill[SKILL_SLEIGHTOFHAND]=sk;randomskills-=sk;
         break;
      case CREATURE_HIPPIE:
         strcpy(cr.name,"Hippie");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.align=1;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=32;
         cr.att[ATTRIBUTE_WISDOM]=1;attcap[ATTRIBUTE_WISDOM]=1;
         sk=LCSrandom(2);cr.skill[SKILL_COOKING]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_MUSIC]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_ART]=sk;randomskills-=sk;
         sk=LCSrandom(2);cr.skill[SKILL_GARMENTMAKING]=sk;randomskills-=sk;
         break;
      case CREATURE_AUTHOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(4);cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
         sk=LCSrandom(4)+4;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         break;
      case CREATURE_JOURNALIST:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.juice=LCSrandom(25);
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+2;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         break;
      case CREATURE_CRITIC_ART:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+2;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         sk=LCSrandom(4)+1;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         break;
      case CREATURE_CRITIC_MUSIC:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(4)+2;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
         sk=LCSrandom(4)+2;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         sk=LCSrandom(4)+1;cr.skill[SKILL_MUSIC]=sk;randomskills-=sk;
         break;
      case CREATURE_SOCIALITE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Socialite");
         if(LCSrandom(2))cr.armor.type=ARMOR_EXPENSIVEDRESS;
         else cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(3)+1;cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_ART]=sk;randomskills-=sk;
         sk=LCSrandom(3);cr.skill[SKILL_MUSIC]=sk;randomskills-=sk;
         break;
      case CREATURE_BIKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Biker");
         cr.armor.type=ARMOR_TRENCHCOAT;
         cr.money=LCSrandom(31)+20;
         cr.align=-1;
         cr.age=AGE_MATURE;
         sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_TRUCKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Truck Driver");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_TAXIDRIVER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Taxi Driver");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;
         sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_PROGRAMMER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         strcpy(cr.name,"Programmer");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         cr.age=AGE_MATURE;

         sk=LCSrandom(5)+4;cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=30;
         cr.att[ATTRIBUTE_INTELLIGENCE]=3;
         break;
      case CREATURE_NUN:
         sk=LCSrandom(3)+3;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_RETIREE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.age=AGE_SENIOR;
         randomskills*=3; // Extra skills
         break;
      case CREATURE_PAINTER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(4)+3;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_SCULPTOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(4)+3;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_DANCER:
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
         cr.att[ATTRIBUTE_AGILITY]=10;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         cr.att[ATTRIBUTE_HEALTH]=10;
         sk=LCSrandom(2)+1;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_MUSIC]=sk;randomskills-=sk;
         cr.age=AGE_YOUNGADULT;
         break;
      case CREATURE_PHOTOGRAPHER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(2)+2;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CAMERAMAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(2)+1;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_HAIRSTYLIST:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(2)+1;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_FASHIONDESIGNER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(3)+8;cr.skill[SKILL_GARMENTMAKING]=sk;randomskills-=sk;
         sk=LCSrandom(3)+2;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_CLERK:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.age=AGE_MATURE;
         break;
      case CREATURE_THIEF:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         switch(LCSrandom(5))
         {
            case 0:getrecruitcreature(cr.name,CREATURE_SOCIALITE);break;
            case 1:getrecruitcreature(cr.name,CREATURE_CLERK);break;
            case 2:getrecruitcreature(cr.name,CREATURE_OFFICEWORKER);break;
            case 3:getrecruitcreature(cr.name,CREATURE_CRITIC_ART);break;
            case 4:getrecruitcreature(cr.name,CREATURE_CRITIC_MUSIC);break;
         }
         sk=LCSrandom(5)+3;cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
         sk=LCSrandom(5)+3;cr.skill[SKILL_DISGUISE]=sk;randomskills-=sk;
         sk=LCSrandom(5)+3;cr.skill[SKILL_STEALTH]=sk;randomskills-=sk;
         sk=LCSrandom(5)+3;cr.skill[SKILL_SLEIGHTOFHAND]=sk;randomskills-=sk;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_ACTOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(5);cr.skill[SKILL_PERSUASION]=sk;randomskills-=sk;
         sk=LCSrandom(5);cr.skill[SKILL_SEDUCTION]=sk;randomskills-=sk;
         sk=LCSrandom(5)+3;cr.skill[SKILL_DISGUISE]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
         cr.att[ATTRIBUTE_WISDOM]=9;
			cr.att[ATTRIBUTE_CHARISMA]=5;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_YOGAINSTRUCTOR:
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
         sk=LCSrandom(4)+2;cr.skill[SKILL_TEACHING]=sk;randomskills-=sk;
         cr.att[ATTRIBUTE_AGILITY]=10;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         cr.att[ATTRIBUTE_HEALTH]=10;
         cr.age=AGE_MATURE;
         break;
      case CREATURE_ATHLETE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
         cr.att[ATTRIBUTE_AGILITY]=8;
         cr.att[ATTRIBUTE_STRENGTH]=8;
         cr.att[ATTRIBUTE_HEALTH]=8;
         cr.age=AGE_YOUNGADULT;
         break;
   }

   if(redistatts>0)
   {
      while(redistatts>0)
      {
         a=LCSrandom(ATTNUM);
         if(cr.att[a]<attcap[a])
         {
            cr.att[a]++;
            redistatts--;
         }
      }
   }

   if(cr.align==1)cr.att[ATTRIBUTE_HEART]+=4;
   else if(cr.align==0)
   {
      cr.att[ATTRIBUTE_HEART]+=2;
      cr.att[ATTRIBUTE_WISDOM]+=2;
   }
   else cr.att[ATTRIBUTE_WISDOM]+=4;

   //RANDOM STARTING SKILLS
   while(randomskills>0)
   {
      cr.skill[LCSrandom(SKILLNUM)]++;
      randomskills--;
   }

   //ALIENATION
   if(sitealienate>=1&&cr.align==0)conservatise(cr);
   if(sitealienate==2&&cr.align==1)conservatise(cr);
}



/* rolls up a proper name for a creature */
void namecreature(creaturest &cr)
{
   name(cr.name);
   strcpy(cr.propername,cr.name);
}



/* fills a string with a proper name */
void name(char *str)
{
   strcpy(str,"");

   char first[80];
   firstname(first);
   strcat(str,first);

   strcat(str," ");

   char last[80];
   lastname(last);
   strcat(str,last);
}



/* gets a random first name */
void firstname(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(122))
   {
      case 0:strcat(str,"Ryan");break;
      case 1:strcat(str,"Sergio");break;
      case 2:strcat(str,"Laura");break;
      case 3:strcat(str,"Anne");break;
      case 4:strcat(str,"Bill");break;
      case 5:strcat(str,"James");break;
      case 6:strcat(str,"Marty");break;
      case 7:strcat(str,"Jessica");break;
      case 8:strcat(str,"Lisa");break;
      case 9:strcat(str,"Bonita");break;
      case 10:strcat(str,"Angel");break;
      case 11:strcat(str,"Pat");break;
      case 12:strcat(str,"Toshiro");break;
      case 13:strcat(str,"Yan-ping");break;
      case 14:strcat(str,"Tetsuo");break;
      case 15:strcat(str,"Akira");break;
      case 16:strcat(str,"Jimmy");break;
      case 17:strcat(str,"Carlos");break;
      case 18:strcat(str,"William");break;
      case 19:strcat(str,"Billy Bob");break;
      case 20:strcat(str,"Carol");break;
      case 21:strcat(str,"Jenny");break;
      case 22:strcat(str,"Jennifer");break;
      case 23:strcat(str,"Manuela");break;
      case 24:strcat(str,"Douglas");break;
      case 25:strcat(str,"Kristin");break;
      case 26:strcat(str,"Steven");break;
      case 27:strcat(str,"Bonnie");break;
      case 28:strcat(str,"Howard");break;
      case 29:strcat(str,"Donald");break;
      case 30:strcat(str,"Barry");break;
      case 31:strcat(str,"Thomas");break;
      case 32:strcat(str,"Joann");break;
      case 33:strcat(str,"Derek");break;
      case 34:strcat(str,"Gary");break;
      case 35:strcat(str,"Archie");break;
      case 36:strcat(str,"Mayumi");break;
      case 37:strcat(str,"Felicia");break;
      case 38:strcat(str,"Sherry");break;
      case 39:strcat(str,"Judy");break;
      case 40:strcat(str,"Elinor");break;
      case 41:strcat(str,"Ned");break;
      case 42:strcat(str,"Randy");break;
      case 43:strcat(str,"Taylor");break;
      case 44:strcat(str,"Kim");break;
      case 45:strcat(str,"Ruthanne");break;
      case 46:strcat(str,"Roger");break;
      case 47:strcat(str,"Raymond");break;
      case 48:strcat(str,"Harvey");break;
      case 49:strcat(str,"Robert");break;
      case 50:strcat(str,"Michael");break;
      case 51:strcat(str,"Aaron");break;
      case 52:strcat(str,"George");break;
      case 53:strcat(str,"Noel");break;
      case 54:strcat(str,"Adrienne");break;
      case 55:strcat(str,"Lex");break;
      case 56:strcat(str,"Linda");break;
      case 57:strcat(str,"Chuck");break;
      case 58:strcat(str,"Charlie");break;
      case 59:strcat(str,"Charles");break;
      case 60:strcat(str,"Malcolm");break;
      case 61:strcat(str,"Martin");break;
      case 62:strcat(str,"Sean");break;
      case 63:strcat(str,"Raven");break;
      case 64:strcat(str,"Wolf");break;
      case 65:strcat(str,"Miguel");break;
      case 66:strcat(str,"Pablo");break;
      case 67:strcat(str,"Paul");break;
      case 68:strcat(str,"Jesus");break;
      case 69:strcat(str,"Ali");break;
      case 70:strcat(str,"Ingrid");break;
      case 71:strcat(str,"Kweisi");break;
      case 72:strcat(str,"Susanna");break;
      case 73:strcat(str,"Sharon");break;
      case 74:strcat(str,"Marion");break;
      case 75:strcat(str,"Kathy");break;
      case 76:strcat(str,"Bruce");break;
      case 77:strcat(str,"Dick");break;
      case 78:strcat(str,"Phillip");break;
      case 79:strcat(str,"Kirk");break;
      case 80:strcat(str,"Kurt");break;
      case 81:strcat(str,"John");break;
      case 82:strcat(str,"Alexander");break;
      case 83:strcat(str,"David");break;
      case 84:strcat(str,"Beau");break;
      case 85:strcat(str,"Elsie");break;
      case 86:strcat(str,"Satya");break;
      case 87:strcat(str,"Mumtaz");break;
      case 88:strcat(str,"Diwakar");break;
      case 89:strcat(str,"Dale");break;
      case 90:strcat(str,"Woody");break;
      case 91:strcat(str,"Ariel");break;
      case 92:strcat(str,"Hans");break;
      case 93:strcat(str,"Barbara");break;
      case 94:strcat(str,"Jun");break;
      case 95:strcat(str,"Rosemary");break;
      case 96:strcat(str,"Chin-Yuan");break;
      case 97:strcat(str,"Aiko");break;
      case 98:strcat(str,"Vithara");break;
      case 99:strcat(str,"Deepak");break;
		case 100:strcat(str,"Christopher");break;
		case 101:strcat(str,"Matthew");break;
		case 102:strcat(str,"Joseph");break;
		case 103:strcat(str,"James");break;
		case 104:strcat(str,"Daniel");break;
		case 105:strcat(str,"Robert");break;
		case 106:strcat(str,"John");break;
      case 107:strcat(str,"Kennedy");break;
		case 108:strcat(str,"Jonathan");break;
		case 109:strcat(str,"Adam");break;
		case 110:strcat(str,"Justin");break;
		case 111:strcat(str,"Ashley");break;
		case 112:strcat(str,"Alene");break;
		case 113:strcat(str,"Janette");break;
		case 114:strcat(str,"Stephanie");break;
		case 115:strcat(str,"Kelly");break;
		case 116:strcat(str,"Robin");break;
		case 117:strcat(str,"Tiffany");break;
		case 118:strcat(str,"Monica");break;
		case 119:strcat(str,"Jaqueline");break;
		case 120:strcat(str,"Latoya");break;
		case 121:strcat(str,"Veronica");break;
      default:strcat(str,"Default");break;
   }
}



/* gets a random last name */
void lastname(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(123))
   {
      case 0:strcat(str,"King");break;
      case 1:strcat(str,"Lewis");break;
      case 2:strcat(str,"Black");break;
      case 3:strcat(str,"White");break;
      case 4:strcat(str,"Ames");break;
      case 5:strcat(str,"Warner");break;
      case 6:strcat(str,"Simpson");break;
      case 7:strcat(str,"Parker");break;
      case 8:strcat(str,"Suave");break;
      case 9:strcat(str,"Mifune");break;
      case 10:strcat(str,"Gu");break;
      case 11:strcat(str,"Bolger");break;
      case 12:strcat(str,"Ross");break;
      case 13:strcat(str,"Ramirez");break;
      case 14:strcat(str,"Kurosawa");break;
      case 15:strcat(str,"Johnson");break;
      case 16:strcat(str,"Buchanan");break;
      case 17:strcat(str,"Adamson");break;
      case 18:strcat(str,"Hendrix");break;
      case 19:strcat(str,"Rojo");break;
      case 20:strcat(str,"Villa");break;
      case 21:strcat(str,"Fields");break;
      case 22:strcat(str,"Templeton");break;
      case 23:strcat(str,"Ivanson");break;
      case 24:strcat(str,"Blitzer");break;
      case 25:strcat(str,"Muhammed");break;
      case 26:strcat(str,"Stone");break;
      case 27:strcat(str,"Cho");break;
      case 28:strcat(str,"Childress");break;
      case 29:strcat(str,"Africa");break;
      case 30:strcat(str,"Balgos");break;
      case 31:strcat(str,"Baird");break;
      case 32:strcat(str,"Bailey");break;
      case 33:strcat(str,"Diaz");break;
      case 34:strcat(str,"Decker");break;
      case 35:strcat(str,"Ericson");break;
      case 36:strcat(str,"Loeb");break;
      case 37:strcat(str,"Meffert");break;
      case 38:strcat(str,"McLeod");break;
      case 39:strcat(str,"Tucker");break;
      case 40:strcat(str,"Takayoshi");break;
      case 41:strcat(str,"Tanner");break;
      case 42:strcat(str,"Lipman");break;
      case 43:strcat(str,"Little");break;
      case 44:strcat(str,"Logsdon");break;
      case 45:strcat(str,"Krasow");break;
      case 46:strcat(str,"Krieger");break;
      case 47:strcat(str,"Dahmer");break;
      case 48:strcat(str,"Gacy");break;
      case 49:strcat(str,"Krishna");break;
      case 50:strcat(str,"la Russa");break;
      case 51:strcat(str,"Savedra");break;
      case 52:strcat(str,"Scardino");break;
      case 53:strcat(str,"Keitel");break;
      case 54:strcat(str,"Wallace");break;
      case 55:strcat(str,"Buckman");break;
      case 56:strcat(str,"Fulsom");break;
      case 57:strcat(str,"Smith");break;
      case 58:strcat(str,"Venus");break;
      case 59:strcat(str,"Straley");break;
      case 60:strcat(str,"Purcell");break;
      case 61:strcat(str,"al Fadil");break;
      case 62:strcat(str,"Storm");break;
      case 63:strcat(str,"Patterson");break;
      case 64:strcat(str,"Pelton");break;
      case 65:strcat(str,"Ng");break;
      case 66:strcat(str,"Filler");break;
      case 67:strcat(str,"Buttman");break;
      case 68:strcat(str,"Fingleton");break;
      case 69:strcat(str,"Fenoglio");break;
      case 70:strcat(str,"de la Cruz");break;
      case 71:strcat(str,"Delgado");break;
      case 72:strcat(str,"Hatcher");break;
      case 73:strcat(str,"Jameson");break;
      case 74:strcat(str,"Franklin");break;
      case 75:strcat(str,"Washington");break;
      case 76:strcat(str,"Jefferson");break;
      case 77:strcat(str,"Strossen");break;
      case 78:strcat(str,"Hannemann");break;
      case 79:strcat(str,"Hammond");break;
      case 80:strcat(str,"Logan");break;
      case 81:strcat(str,"Hutchison");break;
      case 82:strcat(str,"Jimison");break;
      case 83:strcat(str,"Sawyer");break;
      case 84:strcat(str,"Santiago");break;
      case 85:strcat(str,"Rudkin");break;
      case 86:strcat(str,"Bump");break;
      case 87:strcat(str,"Simon");break;
      case 88:strcat(str,"Davis");break;
      case 89:strcat(str,"Reagan");break;
      case 90:strcat(str,"Bush");break;
      case 91:strcat(str,"Bradshaw");break;
      case 92:strcat(str,"Yamaguchi");break;
      case 93:strcat(str,"Roy");break;
      case 94:strcat(str,"Colt");break;
      case 95:strcat(str,"Rothstein");break;
      case 96:strcat(str,"Spears");break;
      case 97:strcat(str,"Lopez");break;
      case 98:strcat(str,"Aguilera");break;
      case 99:strcat(str,"Carey");break;
		case 100:strcat(str,"Griffith");break;
		case 101:strcat(str,"Valdez");break;
		case 102:strcat(str,"McGee");break;
		case 103:strcat(str,"Copeland");break;
		case 104:strcat(str,"Blackenship");break;
		case 105:strcat(str,"Faulkner");break;
		case 106:strcat(str,"Melendez");break;
		case 107:strcat(str,"Dunlap");break;
		case 108:strcat(str,"Guy");break;
		case 109:strcat(str,"Duke");break;
		case 110:strcat(str,"Villanueva");break;
		case 111:strcat(str,"Boggs");break;
		case 112:strcat(str,"Godwin");break;
		case 113:strcat(str,"Brewster");break;
		case 114:strcat(str,"Irvin");break;
		case 115:strcat(str,"Galindo");break;
		case 116:strcat(str,"Cordero");break;
		case 117:strcat(str,"Eubanks");break;
		case 118:strcat(str,"Youngblood");break;
		case 119:strcat(str,"Seay");break;
		case 120:strcat(str,"Hope");break;
		case 121:strcat(str,"Winslow");break;
		case 122:strcat(str,"Fox");break;
      default:strcat(str,"Defaultson");break;
   }
}



/* ensures that the creature's work location is appropriate to its type */
void verifyworklocation(creaturest &cr)
{
   char okaysite[SITENUM];
   memset(okaysite,0,SITENUM*sizeof(char));

   switch(cr.type)
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
         break;
      case CREATURE_SCIENTIST_LABTECH:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         break;
      case CREATURE_SCIENTIST_EMINENT:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
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
         break;
      case CREATURE_PRISONER:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
      case CREATURE_JUROR:
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         break;
      case CREATURE_LAWYER:
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         break;
      case CREATURE_DOCTOR:
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
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_MERC:
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         break;
      case CREATURE_HICK:
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_SOLDIER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
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
         break;
      case CREATURE_CRITIC_MUSIC:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
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
      default:
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
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
      do
      {
         cr.worklocation=LCSrandom(SITENUM);
      }while(!okaysite[cr.worklocation]);

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
}



/* turns a creature into a conservative */
void conservatise(creaturest &cr)
{
   if(cr.align==ALIGN_LIBERAL && cr.juice>0)cr.juice=0;
   
   cr.align=ALIGN_CONSERVATIVE;

   switch(cr.type)
   {
      case CREATURE_WORKER_FACTORY_UNION:
         strcpy(cr.name,"Ex-Union Worker");
         break;
      case CREATURE_JUDGE_LIBERAL:
         strcpy(cr.name,"Jaded Liberal Judge");
         break;
   }
}



/* turns a creature into a liberal */
void liberalize(creaturest &cr,bool rename)
{
   if(cr.align==ALIGN_CONSERVATIVE && cr.juice>0)cr.juice=0;

   cr.align=ALIGN_LIBERAL;

   switch(cr.type)
   {
      case CREATURE_WORKER_FACTORY_NONUNION:
         strcpy(cr.name,"New Union Worker");
         break;
   }
}
