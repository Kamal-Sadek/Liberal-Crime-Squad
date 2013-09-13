
#include "creature_test.h"

class creature_factory
{
public:
   creature* MakeCreature(enum CreatureType type);
private:
   creature_factory();
   int CurrentCreatureID;
};

/* rolls up a creature's stats and equipment */
creature* creature_factory::MakeCreature(enum CreatureType type)
{
   creature* cr = new creature(CurrentCreatureID++);
   //int a = 0;

   return cr;

   /*cr->exists=1;
   cr->squadid=-1;
   cr->type=type;
   getrecruitcreature(cr.name,type);
   cr->armor.type=ARMOR_CLOTHES;
   cr->money=LCSrandom(21)+20;
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
         strcpy(cr.name,"Nonunion Worker");
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
         strcpy(cr.name,"Landlord");
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
         strcpy(cr.name,"Lawyer");
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
         strcpy(cr.name,"Doctor");
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
         strcpy(cr.name,"Nurse");
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
         strcpy(cr.name,"Union Worker");
         cr.weapon.type=WEAPON_CHAIN;
         cr.armor.type=ARMOR_WORKCLOTHES;
         cr.align=1;

         for(a=0;a<ATTNUM;a++)cr.att[a]=1;redistatts=28;
         cr.att[ATTRIBUTE_STRENGTH]=5;
         break;
      case CREATURE_TANK:
         strcpy(cr.name,"Tank");
         cr.animalgloss=ANIMALGLOSS_TANK;
         cr.armor.type=ARMOR_NONE;
         cr.specialattack=ATTACK_CANNON;
         cr.align=-1;
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
         break;
      case CREATURE_SOLDIER:
         strcpy(cr.name,"Soldier");
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
         break;
      case CREATURE_GUARDDOG:
         strcpy(cr.name,"Guard Dog");
         cr.animalgloss=ANIMALGLOSS_ANIMAL;
         cr.armor.type=ARMOR_NONE;
         cr.align=-1;
         break;
      case CREATURE_PRISONER:
         strcpy(cr.name,"Prisoner");
         if(!LCSrandom(2))cr.weapon.type=WEAPON_SHANK;
         cr.armor.type=ARMOR_PRISONER;
         cr.money=0;
         cr.juice=-5;
         if(cr.align==-1)
            cr.align=LCSrandom(2);

         sk=LCSrandom(5)+1;cr.skill[SKILL_SECURITY]=sk;randomskills-=sk;
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
         break;
      case CREATURE_WORKER_FACTORY_CHILD:
         strcpy(cr.name,"Child Worker");
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
         strcpy(cr.name,"Sewer Worker");
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
         strcpy(cr.name,"College Student");
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
         strcpy(cr.name,"Musician");
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
         strcpy(cr.name,"Mathematician");
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
         strcpy(cr.name,"Teacher");
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
         strcpy(cr.name,"Highschool Dropout");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
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

         sk=LCSrandom(2)+1;cr.skill[SKILL_PISTOL]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_SHOTGUN]=sk;randomskills-=sk;
         sk=LCSrandom(2)+1;cr.skill[SKILL_RIFLE]=sk;randomskills-=sk;
         sk=LCSrandom(3)+1;cr.skill[SKILL_GANGSTERISM]=sk;randomskills-=sk;
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

         //NOTE: DO NOT REDISTRIBUTE
         cr.att[ATTRIBUTE_INTELLIGENCE]=1;
         cr.att[ATTRIBUTE_HEALTH]>>=1;cr.att[ATTRIBUTE_HEALTH]++;
         break;
      case CREATURE_PRIEST:
         strcpy(cr.name,"Priest");
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
         strcpy(cr.name,"Engineer");
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
         strcpy(cr.name,"Fast Food Worker");
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
         strcpy(cr.name,"Telemarketer");
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
         strcpy(cr.name,"Office Worker");
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
         strcpy(cr.name,"Football Coach");
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
         strcpy(cr.name,"Prostitute");
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
         strcpy(cr.name,"Mail Carrier");
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
         strcpy(cr.name,"Garbage Collector");
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
         strcpy(cr.name,"Plumber");
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
         strcpy(cr.name,"Chef");
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
         strcpy(cr.name,"Construction Worker");
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
         strcpy(cr.name,"Amateur Magician");
         cr.armor.type=ARMOR_CLOTHES;
         cr.money=LCSrandom(31)+20;
         //cr.align=LCSrandom(3)-1;
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
         strcpy(cr.name,"Socialite");
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
         strcpy(cr.name,"Biker");
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
         strcpy(cr.name,"Truck Driver");
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
         strcpy(cr.name,"Taxi Driver");
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
         strcpy(cr.name,"Programmer");
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
   if(sitealienate==2&&cr.align==1)conservatise(cr);*/
}