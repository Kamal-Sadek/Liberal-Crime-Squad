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


void creaturest::train(int trainedskill, int experience)
{
   if(animalgloss==ANIMALGLOSS_ANIMAL)return; // poor animals don't get experience
   // Skill gain scaled by ability in the area
   skill_ip[trainedskill]+=max(1,static_cast<int>(experience * maxskill(trainedskill,*this,false) / 6.0));
   // Don't rush it
   if(skill_ip[trainedskill]>=(100+10*skill[trainedskill])*1.5)
   {
      skill_ip[trainedskill]=static_cast<int>((100+10*skill[trainedskill])*1.5);
   }
}

void creaturest::skill_up()
{
   for(int s=0;s<SKILLNUM;s++)
   {
      while(skill_ip[s]>=100+10*skill[s]&&
            skill[s]<maxskill(s,*this))
      {
         skill_ip[s]-=100+10*skill[s];
         skill[s]++;
      }
   }
}

int creaturest::get_skill_ip(int getskill)
{
   return skill_ip[getskill];
}

bool creaturest::enemy()
{
   if(align==-1)
   {
      return true;
   }
   else
   {
      if(type==CREATURE_COP)
      {
         for(int i=0;i<pool.size();i++)
         {
            if(pool[i]==this)
            {
               return false;
            }
         }
         return true;
      }
   }
   return false;
}


//Temporary function used in the organization code
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

   switch(LCSrandom(123))
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
      case 122:strcat(str,"Guy");break;
      default:strcat(str,"Default");break;
   }
}



/* gets a random last name */
void lastname(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(124))
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
      case 123:strcat(str,"Montag");break;
      default:strcat(str,"Defaultson");break;
   }
}



/* ensures that the creature's work location is appropriate to its type */
bool verifyworklocation(creaturest &cr, char test_location, char test_type)
{
   char okaysite[SITENUM];
   memset(okaysite,0,SITENUM*sizeof(char));

   char type;

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
         //okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
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
      case CREATURE_VETERAN:
      case CREATURE_HARDENED_VETERAN:
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
      case CREATURE_MARTIALARTIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
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
   return false;
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

/* gives a CCS member a cover name */
void nameCCSMember(creaturest &cr)
{
   if(cr.armor.type==ARMOR_CIVILLIANARMOR)
   {
      strcpy(cr.name,"Mercenary");
   }
   else if(cr.armor.type==ARMOR_ARMYARMOR)
   {
      strcpy(cr.name,"Soldier");
   }
   else if(cr.armor.type==ARMOR_HEAVYARMOR)
   {
      strcpy(cr.name,"Hardened Veteran");
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