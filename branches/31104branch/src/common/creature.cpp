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

#include <includes.h>
#include <externs.h>


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
      cr.align=-1;
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
         cr.armor.type=ARMOR_SECURITYUNIFORM;
         sk=LCSrandom(3)+1;cr.skill[SKILL_HANDTOHAND]=sk;randomskills-=sk;
         if(sitealarm||disguisesite(sitetype))cr.align=-1;
         else cr.align=0;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=18;
         cr.att[ATTRIBUTE_HEALTH]=6;
         cr.att[ATTRIBUTE_AGILITY]=4;
         cr.att[ATTRIBUTE_STRENGTH]=6;
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
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
         cr.att[ATTRIBUTE_INTELLIGENCE]=5;
         sk=LCSrandom(2)+1;cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
         sk=LCSrandom(4)+3;cr.skill[SKILL_SCIENCE]=sk;randomskills-=sk;
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         cr.name="Hangin' Judge";
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

         sk=LCSrandom(6)+5;cr.skill[SKILL_LAW]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=19;
         cr.att[ATTRIBUTE_INTELLIGENCE]=5;
         cr.att[ATTRIBUTE_WISDOM]=10;
         attcap[ATTRIBUTE_HEART]=1;
         break;
      case CREATURE_JUDGE_LIBERAL:
         cr.name="Liberal Judge";
         if(!LCSrandom(2))cr.weapon.type=WEAPON_GAVEL;
         cr.armor.type=ARMOR_BLACKROBE;
         cr.money=LCSrandom(41)+20;
         cr.align=1;

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

         sk=LCSrandom(6)+6;cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=8;
			cr.att[ATTRIBUTE_INTELLIGENCE]=6;
			cr.att[ATTRIBUTE_CHARISMA]=7;
         cr.att[ATTRIBUTE_WISDOM]=12;
         break;
      case CREATURE_WORKER_SERVANT:
         cr.money=0;
         cr.align=1;
         break;
      case CREATURE_WORKER_JANITOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         break;
      case CREATURE_WORKER_SWEATSHOP:
         cr.armor.type=ARMOR_NONE;
         cr.align=1;
         cr.juice=-20;
         cr.flag|=CREATUREFLAG_ILLEGALALIEN;
         sk=LCSrandom(6)+5;cr.skill[SKILL_GARMENTMAKING]=sk;randomskills-=sk;
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Nonunion Worker";
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         if(cr.align==1)
            cr.align=LCSrandom(2)-1;

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
         sk=LCSrandom(2)+1;cr.skill[SKILL_WRITING]=sk;randomskills-=sk;
         break;
      case CREATURE_LANDLORD:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Landlord";
         cr.money=LCSrandom(121)+120;
         cr.align=0;
         sk=LCSrandom(4)+3;cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         break;
      case CREATURE_TEENAGER:
         //cr.align=LCSrandom(3)-1;
         randomskills>>=1;
         break;
      case CREATURE_LAWYER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=0;
         }
         cr.name="Lawyer";
         cr.armor.type=ARMOR_CHEAPSUIT;
         cr.money=LCSrandom(51)+50;
         //cr.align=LCSrandom(3)-1;

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
         cr.name="Doctor";
         cr.armor.type=ARMOR_LABCOAT;
         cr.money=LCSrandom(21)+20;
         //cr.align=LCSrandom(3)-1;

         sk=LCSrandom(4)+4;cr.skill[SKILL_MEDICAL]=sk;randomskills-=sk;
         break;
      case CREATURE_NURSE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(3))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=0;
         }
         cr.name="Nurse";
         cr.armor.type=ARMOR_LABCOAT;
         //cr.align=LCSrandom(3)-1;

         sk=LCSrandom(4)+1;cr.skill[SKILL_MEDICAL]=sk;randomskills-=sk;
         break;
      case CREATURE_WORKER_FACTORY_UNION:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Union Worker";
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.align=1;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
         cr.att[ATTRIBUTE_STRENGTH]=5;
         break;
      case CREATURE_TANK:
         cr.name="Tank";
         cr.animalgloss=ANIMALGLOSS_TANK;
         cr.armor.type=ARMOR_NONE;
         cr.specialattack=ATTACK_CANNON;
         cr.align=-1;
         break;
      case CREATURE_MERC:
         cr.name="Mercenary";
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
         if(!LCSrandom(2))cr.armor.type=ARMOR_MILITARY;
         else
         {
            cr.armor.type=ARMOR_BALLISTICVEST;
            cr.armor.subtype=BVEST_MILITARY;
         }
         cr.money=0;
         cr.align=-1;

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
            case 0:cr.name="Country Boy";break;
            case 1:cr.name="Hick";break;
            case 2:cr.name="Redneck";break;
            case 3:cr.name="Rube";break;
            case 4:cr.name="Yokel";break;
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
         break;
      case CREATURE_SOLDIER:
         cr.name="Soldier";
         cr.weapon.type=WEAPON_AUTORIFLE_M16;
         cr.clip[CLIP_ASSAULT]=6;
         cr.weapon.ammo=30;
         cr.armor.type=ARMOR_BALLISTICVEST;
         cr.armor.subtype=BVEST_MILITARY;
         cr.money=0;
         cr.align=-1;

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
         cr.armor.type=ARMOR_POLICEUNIFORM;
         cr.money=LCSrandom(21)+20;
         cr.align=-1;

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
         cr.armor.type=ARMOR_BALLISTICVEST;
         cr.armor.subtype=BVEST_POLICE;
         cr.align=-1;

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
         cr.armor.type=ARMOR_BALLISTICVEST;
         cr.armor.subtype=BVEST_POLICE;
         cr.money=LCSrandom(21)+20;
         cr.align=-1;

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
      case CREATURE_CCS_VIGILANTE:
      {
         cr.armor.type=ARMOR_MILITARY;
         switch(LCSrandom(5)+endgamestate)
         {
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
            break;
         case 6:
            cr.weapon.type=WEAPON_SEMIRIFLE_AR15;
            cr.clip[CLIP_ASSAULT]=6;
            cr.weapon.ammo=30;
            cr.armor.type=ARMOR_BALLISTICVEST;
            cr.armor.subtype=BVEST_MILITARY;
            break;
         case 7:
            cr.weapon.type=WEAPON_AUTORIFLE_M16;
            cr.clip[CLIP_ASSAULT]=6;
            cr.weapon.ammo=30;
            cr.armor.type=ARMOR_BALLISTICVEST;
            cr.armor.subtype=BVEST_MILITARY;
            break;
         }
         cr.money=LCSrandom(21)+20;
         cr.align=-1;

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
         cr.armor.type=ARMOR_HEAVYBALLISTICVEST;
         cr.money=LCSrandom(51)+150;
         cr.align=-1;

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
         redistatts=40;
         cr.att[ATTRIBUTE_STRENGTH]=8;
			cr.att[ATTRIBUTE_AGILITY]=8;
			cr.att[ATTRIBUTE_HEALTH]=8;
         cr.att[ATTRIBUTE_WISDOM]=16;
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
         cr.armor.type=ARMOR_BALLISTICVEST;
         cr.armor.subtype=BVEST_POLICE;
         cr.align=-1;

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
         cr.name="Prison Guard";
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
         cr.name="Educator";
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
         cr.name="Agent";
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
         cr.name="Radio Personality";
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(51)+50;
         cr.align=-1;

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
         cr.name="News Anchor";
         cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(51)+50;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=16;
         cr.att[ATTRIBUTE_CHARISMA]=10;
         cr.att[ATTRIBUTE_WISDOM]=8;
         cr.align=-1;
         break;
      case CREATURE_GENETIC:
         switch(LCSrandom(10))
         {
            case 0:cr.name="Genetic Monster";break;
            case 1:
            {
               cr.name="Flaming Rabbit";
               cr.specialattack=ATTACK_FLAME;
               break;
            }
            case 2:cr.name="Genetic Nightmare";break;
            case 3:cr.name="Mad Cow";break;
            case 4:
            {
               cr.name="Giant Mosquito";
               cr.specialattack=ATTACK_SUCK;
               break;
            }
            case 5:cr.name="Six-legged Pig";break;
            case 6:cr.name="Purple Gorilla";break;
            case 7:cr.name="Warped Bear";break;
            case 8:cr.name="Writhing Mass";break;
            case 9:cr.name="Something Bad";break;
         }
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.armor.type=ARMOR_NONE;
         cr.align=-1;
         break;
      case CREATURE_GUARDDOG:
         cr.name="Guard Dog";
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.armor.type=ARMOR_NONE;
         cr.align=-1;
         break;
      case CREATURE_PRISONER:
         cr.name="Prisoner";
         if(!LCSrandom(2))cr.weapon.type=WEAPON_SHANK;
         cr.armor.type=ARMOR_PRISONER;
         cr.money=0;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);

         sk=LCSrandom(5)+1;cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
         break;
      case CREATURE_JUROR:
         cr.name="Angry Juror";
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
         break;
      case CREATURE_WORKER_FACTORY_CHILD:
         cr.name="Child Worker";
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=0;
         cr.align=1;
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
         cr.name="Sewer Worker";
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_COLLEGESTUDENT:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="College Student";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;

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
         cr.name="Musician";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(11);
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
         cr.name="Mathematician";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;

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
         cr.name="Teacher";
         cr.armor.type=ARMOR_CLOTHES;
         sk=LCSrandom(4)+3;cr.skill[SKILL_TEACHING]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_HSDROPOUT:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Highschool Dropout";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_BUM:
         cr.name="Transient";
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
         break;
      case CREATURE_MUTANT:
         cr.name="Mutant";
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
         break;
      case CREATURE_GANGMEMBER:
         cr.name="Gang Member";

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

         sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_GANGSTERISM]=sk;randomskills-=sk;
         break;
      case CREATURE_CRACKHEAD:
         cr.name="Crack Head";
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

         //NOTE: DO NOT REDISTRIBUTE
         cr.att[ATTRIBUTE_INTELLIGENCE]=1;
         cr.att[ATTRIBUTE_HEALTH]>>=1;cr.att[ATTRIBUTE_HEALTH]++;
         break;
      case CREATURE_PRIEST:
         cr.name="Priest";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         sk=LCSrandom(5)+3;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;
         break;
      case CREATURE_ENGINEER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Engineer";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;

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
         cr.name="Fast Food Worker";
         cr.armor.type=ARMOR_CLOTHES;
         sk=LCSrandom(3);cr.skill[SKILL_COOKING]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_TELEMARKETER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Telemarketer";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;

			randomskills-=cr.skill[SKILL_PERSUASION]=LCSrandom(3)+1;
         break;
      case CREATURE_OFFICEWORKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Office Worker";
         cr.armor.type=ARMOR_CLOTHES;
         sk=LCSrandom(3);cr.skill[SKILL_BUSINESS]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_FOOTBALLCOACH:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Football Coach";
         cr.armor.type=ARMOR_CLOTHES;
         sk=LCSrandom(3)+1;cr.skill[SKILL_TEACHING]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_LEADERSHIP]=sk;randomskills-=sk;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;

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
         cr.name="Prostitute";
         if(LCSrandom(2))cr.armor.type=ARMOR_CHEAPDRESS;
         else cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);
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
         cr.name="Mail Carrier";
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_GARBAGEMAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Garbage Collector";
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         sk=LCSrandom(2)+1;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_PLUMBER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Plumber";
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_CHEF:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Chef";
         sk=LCSrandom(5)+3;cr.skill[SKILL_COOKING]=sk;randomskills-=sk;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         break;
      case CREATURE_CONSTRUCTIONWORKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Construction Worker";
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;

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
         cr.name="Amateur Magician";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         sk=LCSrandom(3)+2;cr.skill[SKILL_SLEIGHTOFHAND]=sk;randomskills-=sk;
         break;
      case CREATURE_HIPPIE:
         cr.name="Hippie";
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
         cr.name="Socialite";
         if(LCSrandom(2))cr.armor.type=ARMOR_EXPENSIVEDRESS;
         else cr.armor.type=ARMOR_EXPENSIVESUIT;
         cr.money=LCSrandom(131)+100;
         //cr.align=LCSrandom(3)-1;

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
         cr.name="Biker";
         cr.armor.type=ARMOR_TRENCHCOAT;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_TRUCKER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Truck Driver";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_TAXIDRIVER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Taxi Driver";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
         sk=LCSrandom(3)+3;cr.skill[SKILL_DRIVING]=sk;randomskills-=sk;
         break;
      case CREATURE_PROGRAMMER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         cr.name="Programmer";
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;

         sk=LCSrandom(5)+4;cr.skill[SKILL_COMPUTERS]=sk;randomskills-=sk;
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=30;
         cr.att[ATTRIBUTE_INTELLIGENCE]=3;
         break;
      case CREATURE_NUN:
         sk=LCSrandom(3)+3;cr.skill[SKILL_RELIGION]=sk;randomskills-=sk;
         break;
      case CREATURE_RETIREE:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         break;
      case CREATURE_PAINTER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(4)+3;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         break;
      case CREATURE_SCULPTOR:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(4)+3;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         break;
      case CREATURE_DANCER:
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
         cr.att[ATTRIBUTE_AGILITY]=10;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         cr.att[ATTRIBUTE_HEALTH]=10;
         sk=LCSrandom(2)+1;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_MUSIC]=sk;randomskills-=sk;
         break;
      case CREATURE_PHOTOGRAPHER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(2)+2;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         break;
      case CREATURE_CAMERAMAN:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(2)+1;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         break;
      case CREATURE_HAIRSTYLIST:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(2)+1;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         break;
      case CREATURE_FASHIONDESIGNER:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
         sk=LCSrandom(6)+5;cr.skill[SKILL_GARMENTMAKING]=sk;randomskills-=sk;
         sk=LCSrandom(3)+2;cr.skill[SKILL_ART]=sk;randomskills-=sk;
         break;
      case CREATURE_CLERK:
         if(law[LAW_GUNCONTROL]==-2 && !LCSrandom(5))
         {
            cr.weapon.type=WEAPON_REVOLVER_22;
            cr.weapon.ammo=6;
            cr.clip[CLIP_22]=3;
         }
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
         break;
      case CREATURE_YOGAINSTRUCTOR:
         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=11;
         sk=LCSrandom(4)+2;cr.skill[SKILL_TEACHING]=sk;randomskills-=sk;
         cr.att[ATTRIBUTE_AGILITY]=10;
         cr.att[ATTRIBUTE_STRENGTH]=4;
         cr.att[ATTRIBUTE_HEALTH]=10;
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
   cr.propername=cr.name;
}



/* fills a string with a proper name */
void name(string& str)
{
   str="";

   string first;
   firstname(first);
   str+=first;

   str+=" ";

   string last;
   lastname(last);
   str+=last;
}



/* gets a random first name */
void firstname(string& str)
{
   str="";

   switch(LCSrandom(122))
   {
      case 0:str+="Ryan";break;
      case 1:str+="Sergio";break;
      case 2:str+="Laura";break;
      case 3:str+="Anne";break;
      case 4:str+="Bill";break;
      case 5:str+="James";break;
      case 6:str+="Marty";break;
      case 7:str+="Jessica";break;
      case 8:str+="Lisa";break;
      case 9:str+="Bonita";break;
      case 10:str+="Angel";break;
      case 11:str+="Pat";break;
      case 12:str+="Toshiro";break;
      case 13:str+="Yan-ping";break;
      case 14:str+="Tetsuo";break;
      case 15:str+="Akira";break;
      case 16:str+="Jimmy";break;
      case 17:str+="Carlos";break;
      case 18:str+="William";break;
      case 19:str+="Billy Bob";break;
      case 20:str+="Carol";break;
      case 21:str+="Jenny";break;
      case 22:str+="Jennifer";break;
      case 23:str+="Manuela";break;
      case 24:str+="Douglas";break;
      case 25:str+="Kristin";break;
      case 26:str+="Steven";break;
      case 27:str+="Bonnie";break;
      case 28:str+="Howard";break;
      case 29:str+="Donald";break;
      case 30:str+="Barry";break;
      case 31:str+="Thomas";break;
      case 32:str+="Joann";break;
      case 33:str+="Derek";break;
      case 34:str+="Gary";break;
      case 35:str+="Archie";break;
      case 36:str+="Mayumi";break;
      case 37:str+="Felicia";break;
      case 38:str+="Sherry";break;
      case 39:str+="Judy";break;
      case 40:str+="Elinor";break;
      case 41:str+="Ned";break;
      case 42:str+="Randy";break;
      case 43:str+="Taylor";break;
      case 44:str+="Kim";break;
      case 45:str+="Ruthanne";break;
      case 46:str+="Roger";break;
      case 47:str+="Raymond";break;
      case 48:str+="Harvey";break;
      case 49:str+="Robert";break;
      case 50:str+="Michael";break;
      case 51:str+="Aaron";break;
      case 52:str+="George";break;
      case 53:str+="Noel";break;
      case 54:str+="Adrienne";break;
      case 55:str+="Lex";break;
      case 56:str+="Linda";break;
      case 57:str+="Chuck";break;
      case 58:str+="Charlie";break;
      case 59:str+="Charles";break;
      case 60:str+="Malcolm";break;
      case 61:str+="Martin";break;
      case 62:str+="Sean";break;
      case 63:str+="Raven";break;
      case 64:str+="Wolf";break;
      case 65:str+="Miguel";break;
      case 66:str+="Pablo";break;
      case 67:str+="Paul";break;
      case 68:str+="Jesus";break;
      case 69:str+="Ali";break;
      case 70:str+="Ingrid";break;
      case 71:str+="Kweisi";break;
      case 72:str+="Susanna";break;
      case 73:str+="Sharon";break;
      case 74:str+="Marion";break;
      case 75:str+="Kathy";break;
      case 76:str+="Bruce";break;
      case 77:str+="Dick";break;
      case 78:str+="Phillip";break;
      case 79:str+="Kirk";break;
      case 80:str+="Kurt";break;
      case 81:str+="John";break;
      case 82:str+="Alexander";break;
      case 83:str+="David";break;
      case 84:str+="Beau";break;
      case 85:str+="Elsie";break;
      case 86:str+="Satya";break;
      case 87:str+="Mumtaz";break;
      case 88:str+="Diwakar";break;
      case 89:str+="Dale";break;
      case 90:str+="Woody";break;
      case 91:str+="Ariel";break;
      case 92:str+="Hans";break;
      case 93:str+="Barbara";break;
      case 94:str+="Jun";break;
      case 95:str+="Rosemary";break;
      case 96:str+="Chin-Yuan";break;
      case 97:str+="Aiko";break;
      case 98:str+="Vithara";break;
      case 99:str+="Deepak";break;
		case 100:str+="Christopher";break;
		case 101:str+="Matthew";break;
		case 102:str+="Joseph";break;
		case 103:str+="James";break;
		case 104:str+="Daniel";break;
		case 105:str+="Robert";break;
		case 106:str+="John";break;
      case 107:str+="Kennedy";break;
		case 108:str+="Jonathan";break;
		case 109:str+="Adam";break;
		case 110:str+="Justin";break;
		case 111:str+="Ashley";break;
		case 112:str+="Alene";break;
		case 113:str+="Janette";break;
		case 114:str+="Stephanie";break;
		case 115:str+="Kelly";break;
		case 116:str+="Robin";break;
		case 117:str+="Tiffany";break;
		case 118:str+="Monica";break;
		case 119:str+="Jaqueline";break;
		case 120:str+="Latoya";break;
		case 121:str+="Veronica";break;
      default:str+="Default";break;
   }
}



/* gets a random last name */
void lastname(string& str)
{
   str="";

   switch(LCSrandom(123))
   {
      case 0:str+="King";break;
      case 1:str+="Lewis";break;
      case 2:str+="Black";break;
      case 3:str+="White";break;
      case 4:str+="Ames";break;
      case 5:str+="Warner";break;
      case 6:str+="Simpson";break;
      case 7:str+="Parker";break;
      case 8:str+="Suave";break;
      case 9:str+="Mifune";break;
      case 10:str+="Gu";break;
      case 11:str+="Bolger";break;
      case 12:str+="Ross";break;
      case 13:str+="Ramirez";break;
      case 14:str+="Kurosawa";break;
      case 15:str+="Johnson";break;
      case 16:str+="Buchanan";break;
      case 17:str+="Adamson";break;
      case 18:str+="Hendrix";break;
      case 19:str+="Rojo";break;
      case 20:str+="Villa";break;
      case 21:str+="Fields";break;
      case 22:str+="Templeton";break;
      case 23:str+="Ivanson";break;
      case 24:str+="Blitzer";break;
      case 25:str+="Muhammed";break;
      case 26:str+="Stone";break;
      case 27:str+="Cho";break;
      case 28:str+="Childress";break;
      case 29:str+="Africa";break;
      case 30:str+="Balgos";break;
      case 31:str+="Baird";break;
      case 32:str+="Bailey";break;
      case 33:str+="Diaz";break;
      case 34:str+="Decker";break;
      case 35:str+="Ericson";break;
      case 36:str+="Loeb";break;
      case 37:str+="Meffert";break;
      case 38:str+="McLeod";break;
      case 39:str+="Tucker";break;
      case 40:str+="Takayoshi";break;
      case 41:str+="Tanner";break;
      case 42:str+="Lipman";break;
      case 43:str+="Little";break;
      case 44:str+="Logsdon";break;
      case 45:str+="Krasow";break;
      case 46:str+="Krieger";break;
      case 47:str+="Dahmer";break;
      case 48:str+="Gacy";break;
      case 49:str+="Krishna";break;
      case 50:str+="la Russa";break;
      case 51:str+="Savedra";break;
      case 52:str+="Scardino";break;
      case 53:str+="Keitel";break;
      case 54:str+="Wallace";break;
      case 55:str+="Buckman";break;
      case 56:str+="Fulsom";break;
      case 57:str+="Smith";break;
      case 58:str+="Venus";break;
      case 59:str+="Straley";break;
      case 60:str+="Purcell";break;
      case 61:str+="al Fadil";break;
      case 62:str+="Storm";break;
      case 63:str+="Patterson";break;
      case 64:str+="Pelton";break;
      case 65:str+="Ng";break;
      case 66:str+="Filler";break;
      case 67:str+="Buttman";break;
      case 68:str+="Fingleton";break;
      case 69:str+="Fenoglio";break;
      case 70:str+="de la Cruz";break;
      case 71:str+="Delgado";break;
      case 72:str+="Hatcher";break;
      case 73:str+="Jameson";break;
      case 74:str+="Franklin";break;
      case 75:str+="Washington";break;
      case 76:str+="Jefferson";break;
      case 77:str+="Strossen";break;
      case 78:str+="Hannemann";break;
      case 79:str+="Hammond";break;
      case 80:str+="Logan";break;
      case 81:str+="Hutchison";break;
      case 82:str+="Jimison";break;
      case 83:str+="Sawyer";break;
      case 84:str+="Santiago";break;
      case 85:str+="Rudkin";break;
      case 86:str+="Bump";break;
      case 87:str+="Simon";break;
      case 88:str+="Davis";break;
      case 89:str+="Reagan";break;
      case 90:str+="Bush";break;
      case 91:str+="Bradshaw";break;
      case 92:str+="Yamaguchi";break;
      case 93:str+="Roy";break;
      case 94:str+="Colt";break;
      case 95:str+="Rothstein";break;
      case 96:str+="Spears";break;
      case 97:str+="Lopez";break;
      case 98:str+="Aguilera";break;
      case 99:str+="Carey";break;
		case 100:str+="Griffith";break;
		case 101:str+="Valdez";break;
		case 102:str+="McGee";break;
		case 103:str+="Copeland";break;
		case 104:str+="Blackenship";break;
		case 105:str+="Faulkner";break;
		case 106:str+="Melendez";break;
		case 107:str+="Dunlap";break;
		case 108:str+="Guy";break;
		case 109:str+="Duke";break;
		case 110:str+="Villanueva";break;
		case 111:str+="Boggs";break;
		case 112:str+="Godwin";break;
		case 113:str+="Brewster";break;
		case 114:str+="Irvin";break;
		case 115:str+="Galindo";break;
		case 116:str+="Cordero";break;
		case 117:str+="Eubanks";break;
		case 118:str+="Youngblood";break;
		case 119:str+="Seay";break;
		case 120:str+="Hope";break;
		case 121:str+="Winslow";break;
		case 122:str+="Fox";break;
      default:str+="Defaultson";break;
   }
}



/* ensures that the creature's work location is appropriate to its type */
void verifyworklocation(creaturest &cr)
{
   char okaysite[SITENUM];
   memset(okaysite,0,SITENUM*sizeof(char));

   switch(cr.type)
   {
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
   cr.align=-1;

   switch(cr.type)
   {
      case CREATURE_WORKER_FACTORY_UNION:
         cr.name="Ex-union Worker";
         break;
      case CREATURE_JUDGE_LIBERAL:
         cr.name="Jaded Liberal Judge";
         break;
   }
}



/* turns a creature into a liberal */
void liberalize(creaturest &cr)
{
   cr.align=1;

   switch(cr.type)
   {
      case CREATURE_WORKER_FACTORY_NONUNION:
         cr.name="New Union Worker";
         break;
   }
}
