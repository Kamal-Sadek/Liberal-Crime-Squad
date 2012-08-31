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

/* generates a new random encounter */
void prepareencounter(short type,char sec)
{
   int encslot=0;

   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

   int creaturearray[CREATURENUM];
   memset(creaturearray,0,CREATURENUM*sizeof(int));
   
   if(postalarmtimer>80)
   {
      switch(sitetype)
      {
         case SITE_GOVERNMENT_ARMYBASE:
            creaturearray[CREATURE_SOLDIER]=1000;
            creaturearray[CREATURE_MILITARYPOLICE]=300;
            creaturearray[CREATURE_MILITARYOFFICER]=150;
            creaturearray[CREATURE_SEAL]=150;
            creaturearray[CREATURE_GUARDDOG]=100;
            creaturearray[CREATURE_TANK]=100;
            break;
         case SITE_GOVERNMENT_INTELLIGENCEHQ:
            creaturearray[CREATURE_AGENT]=1000;
            creaturearray[CREATURE_MILITARYOFFICER]=100;
            creaturearray[CREATURE_GUARDDOG]=50;
            break;
         case SITE_CORPORATE_HEADQUARTERS:
         case SITE_CORPORATE_HOUSE:
            creaturearray[CREATURE_MERC]=1000;
            break;
         case SITE_MEDIA_AMRADIO:
         case SITE_MEDIA_CABLENEWS:
            creaturearray[CREATURE_HICK]=1000;
            break;
         case SITE_GOVERNMENT_POLICESTATION:
            if(law[LAW_DEATHPENALTY]==-2&&
               law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=1000;
            else creaturearray[CREATURE_SWAT]=1000;
            break;
         default:
            if(law[LAW_DEATHPENALTY]==-2&&
               law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=1000;
            else if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]=1000;
            else creaturearray[CREATURE_COP]=1000;
            break;
      }
      if(siteonfire && law[LAW_FREESPEECH]!=-2)creaturearray[CREATURE_FIREFIGHTER]=1000;
   }

   if(location[cursite]->renting==RENTING_CCS)
   {
      creaturearray[CREATURE_CCS_VIGILANTE]+=50;
      creaturearray[CREATURE_CCS_ARCHCONSERVATIVE]+=endgamestate;
      creaturearray[CREATURE_PROSTITUTE]+=5;
      creaturearray[CREATURE_CRACKHEAD]+=5;
      creaturearray[CREATURE_PRIEST]+=5;
      creaturearray[CREATURE_RADIOPERSONALITY]+=1;
      for(int n=0;n<LCSrandom(6)+1;n++)
      {
         makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
         conservatise(encounter[encslot]);
         encslot++;
      }
   }
   else
   {
      switch(type)
      {
         case SITE_BUSINESS_CRACKHOUSE:
         {
            creaturearray[CREATURE_TEENAGER]+=100;
            creaturearray[CREATURE_MUSICIAN]+=1;
            creaturearray[CREATURE_MATHEMATICIAN]+=1;
            creaturearray[CREATURE_HSDROPOUT]+=30;
            creaturearray[CREATURE_BUM]+=200;
            if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2&&
               law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=50;
            creaturearray[CREATURE_GANGMEMBER]+=200;
            creaturearray[CREATURE_CRACKHEAD]+=200;
            creaturearray[CREATURE_PROSTITUTE]+=200;
            creaturearray[CREATURE_BIKER]+=5;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_THIEF]+=3;
            creaturearray[CREATURE_ACTOR]+=1;
            creaturearray[CREATURE_JOURNALIST]+=2;
            if(endgamestate<ENDGAME_CCS_DEFEATED && endgamestate>ENDGAME_NONE)
               creaturearray[CREATURE_CCS_VIGILANTE]+=5;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_BUSINESS_JUICEBAR:
         {
            creaturearray[CREATURE_TEENAGER]+=10;
            creaturearray[CREATURE_JUDGE_LIBERAL]+=1;
            creaturearray[CREATURE_COLLEGESTUDENT]+=10;
            creaturearray[CREATURE_MUSICIAN]+=2;
            creaturearray[CREATURE_MATHEMATICIAN]+=1;
            creaturearray[CREATURE_TEACHER]+=1;
            creaturearray[CREATURE_HSDROPOUT]+=1;
            creaturearray[CREATURE_ENGINEER]+=1;
            creaturearray[CREATURE_FASTFOODWORKER]+=1;
            creaturearray[CREATURE_TELEMARKETER]+=1;
            creaturearray[CREATURE_CARSALESMAN]+=1;
			creaturearray[CREATURE_OFFICEWORKER]+=1;
			creaturearray[CREATURE_MAILMAN]+=1;
            creaturearray[CREATURE_CHEF]+=1;
            creaturearray[CREATURE_NURSE]+=1;
            creaturearray[CREATURE_AMATEURMAGICIAN]+=1;
            creaturearray[CREATURE_HIPPIE]+=6;
            creaturearray[CREATURE_CRITIC_ART]+=1;
            creaturearray[CREATURE_CRITIC_MUSIC]+=1;
            creaturearray[CREATURE_AUTHOR]+=1;
            creaturearray[CREATURE_JOURNALIST]+=1;
            creaturearray[CREATURE_SOCIALITE]+=2;
            creaturearray[CREATURE_PROGRAMMER]+=1;
            creaturearray[CREATURE_RETIREE]+=1;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_DANCER]+=1;
            creaturearray[CREATURE_PHOTOGRAPHER]+=1;
            creaturearray[CREATURE_CAMERAMAN]+=1;
            creaturearray[CREATURE_HAIRSTYLIST]+=1;
            creaturearray[CREATURE_FASHIONDESIGNER]+=1;
            creaturearray[CREATURE_CLERK]+=1;
            creaturearray[CREATURE_THIEF]+=1;
            creaturearray[CREATURE_ACTOR]+=1;
            creaturearray[CREATURE_YOGAINSTRUCTOR]+=1;
            creaturearray[CREATURE_MARTIALARTIST]+=1;
            creaturearray[CREATURE_ATHLETE]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=1;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_BUSINESS_CIGARBAR:
         {
            if(sec||sitealarm==1)creaturearray[CREATURE_BOUNCER]+=100;
            else creaturearray[CREATURE_BOUNCER]+=10;
            if(sec)creaturearray[CREATURE_GUARDDOG]+=25;
            creaturearray[CREATURE_SCIENTIST_EMINENT]+=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]+=30;
            creaturearray[CREATURE_COP]=+5;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]+=2;
            if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]+=2;
            creaturearray[CREATURE_JUDGE_CONSERVATIVE]+=1;
            creaturearray[CREATURE_RADIOPERSONALITY]+=1;
            creaturearray[CREATURE_NEWSANCHOR]+=1;
            creaturearray[CREATURE_MILITARYOFFICER]+=1;
            creaturearray[CREATURE_LAWYER]+=15;
            creaturearray[CREATURE_DOCTOR]+=10;
            creaturearray[CREATURE_PSYCHOLOGIST]+=1;
            creaturearray[CREATURE_MUSICIAN]+=1;
            creaturearray[CREATURE_ENGINEER]+=10;
            creaturearray[CREATURE_FOOTBALLCOACH]+=1;
            creaturearray[CREATURE_CRITIC_ART]+=1;
            creaturearray[CREATURE_CRITIC_MUSIC]+=1;
            creaturearray[CREATURE_AUTHOR]+=1;
            creaturearray[CREATURE_JOURNALIST]+=1;
            creaturearray[CREATURE_SOCIALITE]+=2;
            creaturearray[CREATURE_RETIREE]+=1;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_DANCER]+=1;
            creaturearray[CREATURE_PHOTOGRAPHER]+=1;
            creaturearray[CREATURE_FASHIONDESIGNER]+=1;
            creaturearray[CREATURE_THIEF]+=1;
            creaturearray[CREATURE_ACTOR]+=1;
            creaturearray[CREATURE_ATHLETE]+=1;
            creaturearray[CREATURE_FIREFIGHTER]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=1;
            if(endgamestate<ENDGAME_CCS_DEFEATED && endgamestate>ENDGAME_NONE)
               creaturearray[CREATURE_CCS_VIGILANTE]+=5;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_OUTDOOR_PUBLICPARK:
         case SITE_BUSINESS_LATTESTAND:
         {
            creaturearray[CREATURE_SECURITYGUARD]+=5;
            creaturearray[CREATURE_SCIENTIST_LABTECH]+=10;
            creaturearray[CREATURE_SCIENTIST_EMINENT]+=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]+=10;
            creaturearray[CREATURE_WORKER_JANITOR]+=5;
            if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]+=5;
            creaturearray[CREATURE_WORKER_SECRETARY]+=15;
            if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]+=5;
            creaturearray[CREATURE_TEENAGER]+=5;
            creaturearray[CREATURE_COP]+=5;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]+=2;
            if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]+=2;
            creaturearray[CREATURE_JUDGE_LIBERAL]+=1;
            creaturearray[CREATURE_JUDGE_CONSERVATIVE]+=1;
            creaturearray[CREATURE_AGENT]+=1;
            if(endgamestate<ENDGAME_CCS_DEFEATED && endgamestate>ENDGAME_NONE)
               creaturearray[CREATURE_CCS_VIGILANTE]+=endgamestate*4;
            creaturearray[CREATURE_RADIOPERSONALITY]+=1;
            creaturearray[CREATURE_NEWSANCHOR]+=1;
            creaturearray[CREATURE_MILITARYOFFICER]+=1;
            creaturearray[CREATURE_LAWYER]+=5;
            creaturearray[CREATURE_DOCTOR]+=5;
            creaturearray[CREATURE_PSYCHOLOGIST]+=1;
            creaturearray[CREATURE_NURSE]+=5;
            creaturearray[CREATURE_SEWERWORKER]+=1;
            creaturearray[CREATURE_COLLEGESTUDENT]+=30;
            creaturearray[CREATURE_MUSICIAN]+=5;
            creaturearray[CREATURE_MATHEMATICIAN]+=5;
            creaturearray[CREATURE_TEACHER]+=5;
            creaturearray[CREATURE_HSDROPOUT]+=1;
            creaturearray[CREATURE_BUM]+=1;
            if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=1;
            if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=1;
            if(law[LAW_POLLUTION]==-2&&
               law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
            creaturearray[CREATURE_GANGMEMBER]+=1;
            creaturearray[CREATURE_CRACKHEAD]+=1;
            creaturearray[CREATURE_PRIEST]+=1;
            creaturearray[CREATURE_ENGINEER]+=5;
            creaturearray[CREATURE_FASTFOODWORKER]+=5;
            creaturearray[CREATURE_TELEMARKETER]+=5;
            creaturearray[CREATURE_CARSALESMAN]+=3;
			creaturearray[CREATURE_OFFICEWORKER]+=10;
			creaturearray[CREATURE_FOOTBALLCOACH]+=1;
            creaturearray[CREATURE_PROSTITUTE]+=1;
            creaturearray[CREATURE_MAILMAN]+=1;
            creaturearray[CREATURE_GARBAGEMAN]+=1;
            creaturearray[CREATURE_PLUMBER]+=1;
            creaturearray[CREATURE_CHEF]+=1;
            creaturearray[CREATURE_CONSTRUCTIONWORKER]+=3;
            creaturearray[CREATURE_AMATEURMAGICIAN]+=1;
            creaturearray[CREATURE_MERC]+=1;
            creaturearray[CREATURE_SOLDIER]+=1;
            creaturearray[CREATURE_VETERAN]+=3;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]+=1;
            else creaturearray[CREATURE_PRISONGUARD]+=1;
            creaturearray[CREATURE_HIPPIE]+=1;
            creaturearray[CREATURE_CRITIC_ART]+=1;
            creaturearray[CREATURE_CRITIC_MUSIC]+=1;
            creaturearray[CREATURE_AUTHOR]+=1;
            creaturearray[CREATURE_JOURNALIST]+=1;
            creaturearray[CREATURE_SOCIALITE]+=1;
            creaturearray[CREATURE_BIKER]+=1;
            creaturearray[CREATURE_TRUCKER]+=1;
            creaturearray[CREATURE_TAXIDRIVER]+=1;
            creaturearray[CREATURE_PROGRAMMER]+=5;
            creaturearray[CREATURE_RETIREE]+=3;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_DANCER]+=1;
            creaturearray[CREATURE_PHOTOGRAPHER]+=1;
            creaturearray[CREATURE_CAMERAMAN]+=1;
            creaturearray[CREATURE_HAIRSTYLIST]+=1;
            creaturearray[CREATURE_FASHIONDESIGNER]+=1;
            creaturearray[CREATURE_CLERK]+=1;
            creaturearray[CREATURE_THIEF]+=1;
            creaturearray[CREATURE_ACTOR]+=1;
            creaturearray[CREATURE_YOGAINSTRUCTOR]+=1;
            creaturearray[CREATURE_MARTIALARTIST]+=1;
            creaturearray[CREATURE_ATHLETE]+=1;
            creaturearray[CREATURE_FIREFIGHTER]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=1;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_BUSINESS_VEGANCOOP:
         {
            creaturearray[CREATURE_TEENAGER]+=5;
            creaturearray[CREATURE_JUDGE_LIBERAL]+=1;
            creaturearray[CREATURE_COLLEGESTUDENT]+=50;
            creaturearray[CREATURE_MUSICIAN]+=20;
            creaturearray[CREATURE_MATHEMATICIAN]+=1;
            creaturearray[CREATURE_TEACHER]+=1;
            creaturearray[CREATURE_HSDROPOUT]+=10;
            creaturearray[CREATURE_BUM]+=1;
            if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=1;
            if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=1;
            if(law[LAW_POLLUTION]==-2&&
               law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
            creaturearray[CREATURE_HIPPIE]+=50;
            creaturearray[CREATURE_CRITIC_ART]+=1;
            creaturearray[CREATURE_CRITIC_MUSIC]+=1;
            creaturearray[CREATURE_AUTHOR]+=1;
            creaturearray[CREATURE_JOURNALIST]+=1;
            creaturearray[CREATURE_RETIREE]+=1;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_DANCER]+=1;
            creaturearray[CREATURE_PHOTOGRAPHER]+=1;
            creaturearray[CREATURE_YOGAINSTRUCTOR]+=2;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_BUSINESS_INTERNETCAFE:
         {
            creaturearray[CREATURE_SCIENTIST_LABTECH]+=5;
            creaturearray[CREATURE_CORPORATE_MANAGER]+=3;
            creaturearray[CREATURE_TEENAGER]+=15;
            creaturearray[CREATURE_LAWYER]+=3;
            creaturearray[CREATURE_COLLEGESTUDENT]+=25;
            creaturearray[CREATURE_MUSICIAN]+=2;
            creaturearray[CREATURE_MATHEMATICIAN]+=1;
            creaturearray[CREATURE_TEACHER]+=5;
            creaturearray[CREATURE_ENGINEER]+=15;
            creaturearray[CREATURE_DOCTOR]+=1;
			creaturearray[CREATURE_CARSALESMAN]+=3;
		    creaturearray[CREATURE_OFFICEWORKER]+=15;
            creaturearray[CREATURE_WORKER_SECRETARY]+=5;
            creaturearray[CREATURE_HIPPIE]+=1;
            creaturearray[CREATURE_PROGRAMMER]+=15;
            creaturearray[CREATURE_RETIREE]+=5;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_DANCER]+=1;
            creaturearray[CREATURE_PHOTOGRAPHER]+=1;
            creaturearray[CREATURE_CAMERAMAN]+=1;
            creaturearray[CREATURE_CLERK]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=1;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         default:
         case SITE_RESIDENTIAL_SHELTER:
         {
            creaturearray[CREATURE_WORKER_JANITOR]+=5;
            creaturearray[CREATURE_TEENAGER]+=20;
            creaturearray[CREATURE_MUSICIAN]+=3;
            creaturearray[CREATURE_MATHEMATICIAN]+=1;
            creaturearray[CREATURE_BUM]+=200;
            if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2&&
               law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=50;
            creaturearray[CREATURE_GANGMEMBER]+=5;
            creaturearray[CREATURE_CRACKHEAD]+=50;
            creaturearray[CREATURE_PROSTITUTE]+=10;
            creaturearray[CREATURE_AMATEURMAGICIAN]+=1;
            creaturearray[CREATURE_HIPPIE]+=1;
            creaturearray[CREATURE_NURSE]+=5;
            creaturearray[CREATURE_BIKER]+=1;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_DANCER]+=1;
            creaturearray[CREATURE_PHOTOGRAPHER]+=1;
            creaturearray[CREATURE_THIEF]+=5;
            creaturearray[CREATURE_ACTOR]+=1;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_RESIDENTIAL_TENEMENT:
         {
            creaturearray[CREATURE_SECURITYGUARD]+=1;
            creaturearray[CREATURE_SCIENTIST_LABTECH]+=1;
            creaturearray[CREATURE_WORKER_JANITOR]+=3;
            if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]+=1;
            creaturearray[CREATURE_WORKER_SECRETARY]+=2;
            if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]+=1;
            creaturearray[CREATURE_TEENAGER]+=5;
            creaturearray[CREATURE_SEWERWORKER]+=1;
            creaturearray[CREATURE_COLLEGESTUDENT]+=1;
            creaturearray[CREATURE_MUSICIAN]+=1;
            creaturearray[CREATURE_MATHEMATICIAN]+=1;
            creaturearray[CREATURE_TEACHER]+=1;
            creaturearray[CREATURE_HSDROPOUT]+=3;
            creaturearray[CREATURE_BUM]+=3;
            if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2&&
               law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=5;
            creaturearray[CREATURE_GANGMEMBER]+=3;
            creaturearray[CREATURE_CRACKHEAD]+=3;
            creaturearray[CREATURE_FASTFOODWORKER]+=1;
            creaturearray[CREATURE_TELEMARKETER]+=1;
			creaturearray[CREATURE_CARSALESMAN]+=1;
			creaturearray[CREATURE_OFFICEWORKER]+=1;
            creaturearray[CREATURE_PROSTITUTE]+=3;
            creaturearray[CREATURE_MAILMAN]+=1;
            creaturearray[CREATURE_GARBAGEMAN]+=1;
            creaturearray[CREATURE_CONSTRUCTIONWORKER]+=1;
            creaturearray[CREATURE_AMATEURMAGICIAN]+=1;
            creaturearray[CREATURE_HICK]+=1;
            creaturearray[CREATURE_SOLDIER]+=1;
            creaturearray[CREATURE_VETERAN]+=2;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]+=1;
            else creaturearray[CREATURE_PRISONGUARD]+=1;
            creaturearray[CREATURE_HIPPIE]+=1;
            creaturearray[CREATURE_BIKER]+=1;
            creaturearray[CREATURE_TAXIDRIVER]+=1;
            creaturearray[CREATURE_RETIREE]+=1;
            creaturearray[CREATURE_PAINTER]+=1;
            creaturearray[CREATURE_SCULPTOR]+=1;
            creaturearray[CREATURE_DANCER]+=1;
            creaturearray[CREATURE_PHOTOGRAPHER]+=1;
            creaturearray[CREATURE_HAIRSTYLIST]+=1;
            creaturearray[CREATURE_CLERK]+=1;
            creaturearray[CREATURE_THIEF]+=1;
            creaturearray[CREATURE_ACTOR]+=1;
            creaturearray[CREATURE_FIREFIGHTER]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=1;

            int encnum=1;
            if(mode==GAMEMODE_SITE && !(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED))
            {
               encnum=4;
            }
            for(int n=0;n<LCSrandom(encnum)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_RESIDENTIAL_APARTMENT:
         {
            creaturearray[CREATURE_SECURITYGUARD]+=1;
            creaturearray[CREATURE_SCIENTIST_LABTECH]=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]=1;
            creaturearray[CREATURE_WORKER_JANITOR]=1;
            if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1;
            creaturearray[CREATURE_WORKER_SECRETARY]=1;
            if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=1;
            creaturearray[CREATURE_TEENAGER]=3;
            creaturearray[CREATURE_COP]+=1;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]+=1;
            if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]+=1;
            creaturearray[CREATURE_LAWYER]=1;
            creaturearray[CREATURE_SEWERWORKER]=1;
            creaturearray[CREATURE_COLLEGESTUDENT]=1;
            creaturearray[CREATURE_MUSICIAN]=1;
            creaturearray[CREATURE_MATHEMATICIAN]=1;
            creaturearray[CREATURE_TEACHER]=1;
            creaturearray[CREATURE_PRIEST]=1;
            creaturearray[CREATURE_ENGINEER]=1;
            creaturearray[CREATURE_FASTFOODWORKER]=1;
            creaturearray[CREATURE_TELEMARKETER]=1;
			creaturearray[CREATURE_CARSALESMAN]=1;
			creaturearray[CREATURE_OFFICEWORKER]=1;
            creaturearray[CREATURE_FOOTBALLCOACH]=1;
            creaturearray[CREATURE_MAILMAN]=1;
            creaturearray[CREATURE_DOCTOR]=1;
            creaturearray[CREATURE_PSYCHOLOGIST]+=1;
            creaturearray[CREATURE_NURSE]=1;
            creaturearray[CREATURE_GARBAGEMAN]=1;
            creaturearray[CREATURE_PLUMBER]=1;
            creaturearray[CREATURE_CHEF]=1;
            creaturearray[CREATURE_CONSTRUCTIONWORKER]=1;
            creaturearray[CREATURE_AMATEURMAGICIAN]=1;
            creaturearray[CREATURE_SOLDIER]=1;
            creaturearray[CREATURE_VETERAN]=2;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]+=1;
            else creaturearray[CREATURE_PRISONGUARD]+=1;
            creaturearray[CREATURE_HIPPIE]=1;
            creaturearray[CREATURE_CRITIC_ART]=1;
            creaturearray[CREATURE_CRITIC_MUSIC]=1;
            creaturearray[CREATURE_AUTHOR]=1;
            creaturearray[CREATURE_JOURNALIST]=1;
            creaturearray[CREATURE_TAXIDRIVER]=1;
            creaturearray[CREATURE_PROGRAMMER]=1;
            creaturearray[CREATURE_RETIREE]=1;
            creaturearray[CREATURE_PAINTER]=1;
            creaturearray[CREATURE_SCULPTOR]=1;
            creaturearray[CREATURE_DANCER]=1;
            creaturearray[CREATURE_PHOTOGRAPHER]=1;
            creaturearray[CREATURE_CAMERAMAN]=1;
            creaturearray[CREATURE_HAIRSTYLIST]=1;
            creaturearray[CREATURE_CLERK]=1;
            creaturearray[CREATURE_THIEF]=1;
            creaturearray[CREATURE_ACTOR]=1;
            creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
            creaturearray[CREATURE_MARTIALARTIST]+=1;
            creaturearray[CREATURE_ATHLETE]=1;
            creaturearray[CREATURE_FIREFIGHTER]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=1;

            int encnum=1;
            if(mode==GAMEMODE_SITE && !(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED))
            {
               encnum=4;
            }
            for(int n=0;n<LCSrandom(encnum)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
         {
            if(mode==GAMEMODE_SITE && !(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED))
            {
               if(sec)creaturearray[CREATURE_SECURITYGUARD]+=100;
               else creaturearray[CREATURE_SECURITYGUARD]+=10;
               if(sec)creaturearray[CREATURE_GUARDDOG]+=50;
            }
			else if(mode==GAMEMODE_SITE && sec)
			{
				//inside someone's room when security is high. Might meet a policeman.
				if(law[LAW_DEATHPENALTY]==-2&&law[LAW_POLICEBEHAVIOR]==-2)
					creaturearray[CREATURE_DEATHSQUAD]+=5;
				if(law[LAW_POLICEBEHAVIOR]<=-1)
					creaturearray[CREATURE_GANGUNIT]+=10;
				creaturearray[CREATURE_COP]+=15;
			}
            creaturearray[CREATURE_SCIENTIST_EMINENT]+=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]+=5;
            creaturearray[CREATURE_WORKER_JANITOR]=5;
            creaturearray[CREATURE_WORKER_SECRETARY]=1;
            creaturearray[CREATURE_TEENAGER]=3;
            creaturearray[CREATURE_JUDGE_LIBERAL]=1;
            creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
            creaturearray[CREATURE_RADIOPERSONALITY]=1;
            creaturearray[CREATURE_NEWSANCHOR]=1;
            creaturearray[CREATURE_LAWYER]=5;
            creaturearray[CREATURE_DOCTOR]=5;
            creaturearray[CREATURE_PSYCHOLOGIST]+=1;
            creaturearray[CREATURE_NURSE]=1;
            creaturearray[CREATURE_COLLEGESTUDENT]=1;
            creaturearray[CREATURE_MUSICIAN]=1;
            creaturearray[CREATURE_PROSTITUTE]=3;
            creaturearray[CREATURE_MAILMAN]=1;
            creaturearray[CREATURE_CRITIC_ART]=1;
            creaturearray[CREATURE_CRITIC_MUSIC]=1;
            creaturearray[CREATURE_AUTHOR]=1;
            creaturearray[CREATURE_JOURNALIST]=1;
            creaturearray[CREATURE_SOCIALITE]=2;
            creaturearray[CREATURE_PAINTER]=1;
            creaturearray[CREATURE_SCULPTOR]=1;
            creaturearray[CREATURE_DANCER]=1;
            creaturearray[CREATURE_PHOTOGRAPHER]=1;
            creaturearray[CREATURE_FASHIONDESIGNER]=1;
            creaturearray[CREATURE_THIEF]=1;
            creaturearray[CREATURE_ACTOR]=1;
            creaturearray[CREATURE_ATHLETE]=1;
            creaturearray[CREATURE_LOCKSMITH]+=1;

            int encnum=1;
            if(mode==GAMEMODE_SITE && !(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED))
            {
               encnum=4;
            }
            for(int n=0;n<LCSrandom(encnum)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_LABORATORY_COSMETICS:
         {
            if(sec)creaturearray[CREATURE_SECURITYGUARD]+=100;
            else creaturearray[CREATURE_SECURITYGUARD]+=3;
            creaturearray[CREATURE_SCIENTIST_LABTECH]=10;
            creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]=1;
            creaturearray[CREATURE_WORKER_JANITOR]=10;
            creaturearray[CREATURE_WORKER_SECRETARY]=10;
            creaturearray[CREATURE_OFFICEWORKER]=10;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_INDUSTRY_NUCLEAR:
         {
            if(sec)creaturearray[CREATURE_SECURITYGUARD]+=100;
            else creaturearray[CREATURE_SECURITYGUARD]+=10;
            creaturearray[CREATURE_SCIENTIST_LABTECH]=10;
            creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]=1;
            creaturearray[CREATURE_WORKER_JANITOR]=10;
            creaturearray[CREATURE_WORKER_SECRETARY]=10;
            creaturearray[CREATURE_OFFICEWORKER]=10;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_LABORATORY_GENETIC:
         {
            if(sec)creaturearray[CREATURE_GUARDDOG]+=25;
            if(sec)creaturearray[CREATURE_SECURITYGUARD]+=100;
            else creaturearray[CREATURE_SECURITYGUARD]+=3;
            creaturearray[CREATURE_SCIENTIST_LABTECH]=10;
            creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]=1;
            creaturearray[CREATURE_DOCTOR]=1;
            creaturearray[CREATURE_WORKER_JANITOR]=10;
            creaturearray[CREATURE_WORKER_SECRETARY]=10;
            creaturearray[CREATURE_OFFICEWORKER]=10;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_GOVERNMENT_POLICESTATION:
         {
            creaturearray[CREATURE_SCIENTIST_LABTECH]=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]=1;
            creaturearray[CREATURE_WORKER_JANITOR]=50;
            if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1;
            creaturearray[CREATURE_WORKER_SECRETARY]=1;
            if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=1;
            creaturearray[CREATURE_TEENAGER]=5;
            if(sec)creaturearray[CREATURE_COP]+=1000;
            else creaturearray[CREATURE_COP]+=500;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]+=400;
            if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]+=400;
            creaturearray[CREATURE_JUDGE_LIBERAL]=1;
            creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
            creaturearray[CREATURE_AGENT]=1;
            creaturearray[CREATURE_RADIOPERSONALITY]=1;
            creaturearray[CREATURE_NEWSANCHOR]=1;
            creaturearray[CREATURE_MILITARYOFFICER]=1;
            creaturearray[CREATURE_LAWYER]=1;
            creaturearray[CREATURE_DOCTOR]=1;
            creaturearray[CREATURE_PSYCHOLOGIST]+=1;
            creaturearray[CREATURE_NURSE]=1;
            creaturearray[CREATURE_SEWERWORKER]=1;
            creaturearray[CREATURE_COLLEGESTUDENT]=1;
            creaturearray[CREATURE_MUSICIAN]=1;
            creaturearray[CREATURE_MATHEMATICIAN]=1;
            creaturearray[CREATURE_TEACHER]=1;
            creaturearray[CREATURE_HSDROPOUT]=10;
            creaturearray[CREATURE_BUM]=10;
            if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=2;
            if(law[LAW_POLLUTION]==-2&&
               law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=5;
            creaturearray[CREATURE_GANGMEMBER]=10;
            creaturearray[CREATURE_CRACKHEAD]=10;
            creaturearray[CREATURE_PRIEST]=5;
            creaturearray[CREATURE_ENGINEER]=1;
            creaturearray[CREATURE_FASTFOODWORKER]=1;
            creaturearray[CREATURE_TELEMARKETER]=1;
            creaturearray[CREATURE_CARSALESMAN]=1;
			creaturearray[CREATURE_OFFICEWORKER]=1;
            creaturearray[CREATURE_FOOTBALLCOACH]=1;
            creaturearray[CREATURE_PROSTITUTE]=10;
            creaturearray[CREATURE_MAILMAN]=1;
            creaturearray[CREATURE_GARBAGEMAN]=1;
            creaturearray[CREATURE_PLUMBER]=1;
            creaturearray[CREATURE_CHEF]=1;
            creaturearray[CREATURE_CONSTRUCTIONWORKER]=1;
            creaturearray[CREATURE_AMATEURMAGICIAN]=1;
            creaturearray[CREATURE_HICK]=1;
            creaturearray[CREATURE_SOLDIER]+=1;
            creaturearray[CREATURE_VETERAN]+=2;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=1;
            else creaturearray[CREATURE_PRISONGUARD]=1;
            creaturearray[CREATURE_HIPPIE]=1;
            creaturearray[CREATURE_CRITIC_ART]=1;
            creaturearray[CREATURE_CRITIC_MUSIC]=1;
            creaturearray[CREATURE_AUTHOR]=1;
            creaturearray[CREATURE_JOURNALIST]=1;
            creaturearray[CREATURE_SOCIALITE]=1;
            creaturearray[CREATURE_BIKER]=5;
            creaturearray[CREATURE_TRUCKER]=1;
            creaturearray[CREATURE_TAXIDRIVER]=1;
            creaturearray[CREATURE_PROGRAMMER]=1;
            creaturearray[CREATURE_NUN]=1;
            creaturearray[CREATURE_RETIREE]=1;
            creaturearray[CREATURE_PAINTER]=1;
            creaturearray[CREATURE_SCULPTOR]=1;
            creaturearray[CREATURE_DANCER]=1;
            creaturearray[CREATURE_PHOTOGRAPHER]=1;
            creaturearray[CREATURE_CAMERAMAN]=1;
            creaturearray[CREATURE_HAIRSTYLIST]=1;
            creaturearray[CREATURE_FASHIONDESIGNER]=1;
            creaturearray[CREATURE_CLERK]=1;
            creaturearray[CREATURE_THIEF]=10;
            creaturearray[CREATURE_ACTOR]=1;
            creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
            creaturearray[CREATURE_MARTIALARTIST]=1;
            creaturearray[CREATURE_ATHLETE]=1;
            //creaturearray[CREATURE_FIREFIGHTER]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=5;       //Forensic locksmiths

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_GOVERNMENT_COURTHOUSE:
         {
            if(sec)creaturearray[CREATURE_COP]=2000;
            else creaturearray[CREATURE_COP]=200;
            creaturearray[CREATURE_SCIENTIST_LABTECH]=1;
            creaturearray[CREATURE_SCIENTIST_EMINENT]=1;
            creaturearray[CREATURE_CORPORATE_MANAGER]=1;
            creaturearray[CREATURE_WORKER_JANITOR]=50;
            if(law[LAW_LABOR]<2)creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=1;
            creaturearray[CREATURE_WORKER_SECRETARY]=50;
            if(law[LAW_LABOR]>=0)creaturearray[CREATURE_WORKER_FACTORY_UNION]=1;
            creaturearray[CREATURE_TEENAGER]=1;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]=80;
            if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]=80;
            creaturearray[CREATURE_JUDGE_LIBERAL]=20;
            creaturearray[CREATURE_JUDGE_CONSERVATIVE]=20;
            creaturearray[CREATURE_AGENT]=1;
            creaturearray[CREATURE_RADIOPERSONALITY]=1;
            creaturearray[CREATURE_NEWSANCHOR]=1;
            creaturearray[CREATURE_MILITARYOFFICER]=1;
            creaturearray[CREATURE_LAWYER]=200;
            creaturearray[CREATURE_PSYCHOLOGIST]+=20;
            creaturearray[CREATURE_SEWERWORKER]=1;
            creaturearray[CREATURE_COLLEGESTUDENT]=1;
            creaturearray[CREATURE_MUSICIAN]=1;
            creaturearray[CREATURE_MATHEMATICIAN]=1;
            creaturearray[CREATURE_TEACHER]=1;
            creaturearray[CREATURE_HSDROPOUT]=1;
            creaturearray[CREATURE_BUM]=1;
            if(law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=1;
            if(law[LAW_POLLUTION]==-2)creaturearray[CREATURE_MUTANT]+=1;
            if(law[LAW_POLLUTION]==-2&&
               law[LAW_NUCLEARPOWER]==-2)creaturearray[CREATURE_MUTANT]+=2;
            creaturearray[CREATURE_GANGMEMBER]=1;
            creaturearray[CREATURE_CRACKHEAD]=1;
            creaturearray[CREATURE_PRIEST]=1;
            creaturearray[CREATURE_ENGINEER]=1;
            creaturearray[CREATURE_FASTFOODWORKER]=1;
            creaturearray[CREATURE_TELEMARKETER]=1;
            creaturearray[CREATURE_CARSALESMAN]=2;
			creaturearray[CREATURE_OFFICEWORKER]=50;
            creaturearray[CREATURE_FOOTBALLCOACH]=1;
            creaturearray[CREATURE_PROSTITUTE]=1;
            creaturearray[CREATURE_MAILMAN]=1;
            creaturearray[CREATURE_GARBAGEMAN]=1;
            creaturearray[CREATURE_PLUMBER]=1;
            creaturearray[CREATURE_CHEF]=1;
            creaturearray[CREATURE_CONSTRUCTIONWORKER]=1;
            creaturearray[CREATURE_AMATEURMAGICIAN]=1;
            creaturearray[CREATURE_HICK]=1;
            creaturearray[CREATURE_SOLDIER]=1;
            creaturearray[CREATURE_VETERAN]=2;
            if(law[LAW_DEATHPENALTY]==-2&&
                law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]=1;
            else creaturearray[CREATURE_PRISONGUARD]=1;
            creaturearray[CREATURE_HIPPIE]=1;
            creaturearray[CREATURE_CRITIC_ART]=1;
            creaturearray[CREATURE_CRITIC_MUSIC]=1;
            creaturearray[CREATURE_AUTHOR]=1;
            creaturearray[CREATURE_JOURNALIST]=1;
            creaturearray[CREATURE_SOCIALITE]=1;
            creaturearray[CREATURE_BIKER]=1;
            creaturearray[CREATURE_TRUCKER]=1;
            creaturearray[CREATURE_TAXIDRIVER]=1;
            creaturearray[CREATURE_PROGRAMMER]=1;
            creaturearray[CREATURE_NUN]=1;
            creaturearray[CREATURE_RETIREE]=1;
            creaturearray[CREATURE_PAINTER]=1;
            creaturearray[CREATURE_SCULPTOR]=1;
            creaturearray[CREATURE_DANCER]=1;
            creaturearray[CREATURE_PHOTOGRAPHER]=1;
            creaturearray[CREATURE_CAMERAMAN]=1;
            creaturearray[CREATURE_HAIRSTYLIST]=1;
            creaturearray[CREATURE_FASHIONDESIGNER]=1;
            creaturearray[CREATURE_CLERK]=1;
            creaturearray[CREATURE_THIEF]=3;
            creaturearray[CREATURE_ACTOR]=1;
            creaturearray[CREATURE_YOGAINSTRUCTOR]=1;
            creaturearray[CREATURE_MARTIALARTIST]=1;
            creaturearray[CREATURE_ATHLETE]=1;
            //creaturearray[CREATURE_FIREFIGHTER]+=1;
            creaturearray[CREATURE_LOCKSMITH]+=5;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }

         case SITE_GOVERNMENT_FIRESTATION:
         {
            creaturearray[CREATURE_WORKER_JANITOR]=5;
            creaturearray[CREATURE_WORKER_SECRETARY]=2;
            if(sec)
            {
               if(law[LAW_DEATHPENALTY]==-2&&
                   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_DEATHSQUAD]+=50;
               else if(law[LAW_POLICEBEHAVIOR]<=-1)creaturearray[CREATURE_GANGUNIT]+=50;
               else creaturearray[CREATURE_COP]+=50;
            }
            creaturearray[CREATURE_NURSE]=2;
            creaturearray[CREATURE_PRIEST]=5;
            creaturearray[CREATURE_JOURNALIST]=1;
            creaturearray[CREATURE_PHOTOGRAPHER]=1;
            creaturearray[CREATURE_FIREFIGHTER]+=100;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }

         case SITE_GOVERNMENT_PRISON:
         {
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
               creaturearray[CREATURE_PRISONER]=8; // prisoners only in restricted areas

            if(sec)
            {
               if(law[LAW_DEATHPENALTY]==-2&&
                   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]+=3;
               else creaturearray[CREATURE_PRISONGUARD]+=3;
            }
            else
            {
               if(law[LAW_DEATHPENALTY]==-2&&
                   law[LAW_POLICEBEHAVIOR]==-2)creaturearray[CREATURE_EDUCATOR]+=2;
               else creaturearray[CREATURE_PRISONGUARD]+=2;
            }
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_GOVERNMENT_INTELLIGENCEHQ:
         {
            if(sec)creaturearray[CREATURE_AGENT]+=1000;
            else creaturearray[CREATURE_AGENT]+=100;
            creaturearray[CREATURE_WORKER_JANITOR]+=50;
            creaturearray[CREATURE_WORKER_SECRETARY]+=50;
            if(sec)creaturearray[CREATURE_GUARDDOG]+=450;
            creaturearray[CREATURE_GUARDDOG]+=50;
            creaturearray[CREATURE_MATHEMATICIAN]+=5;
            creaturearray[CREATURE_PROGRAMMER]+=5;
            creaturearray[CREATURE_MILITARYOFFICER]+=5;

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_GOVERNMENT_ARMYBASE:
         {
            creaturearray[CREATURE_SOLDIER]+=750;
            if(sec)creaturearray[CREATURE_GUARDDOG]+=230;
            creaturearray[CREATURE_GUARDDOG]+=20;
            creaturearray[CREATURE_MILITARYPOLICE]+=100;
            if(sec)creaturearray[CREATURE_MILITARYPOLICE]+=100;
            creaturearray[CREATURE_MILITARYOFFICER]+=20;
            creaturearray[CREATURE_SEAL]+=20;


            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_INDUSTRY_SWEATSHOP:
         {
            if(sec)creaturearray[CREATURE_SECURITYGUARD]+=1000;
            else creaturearray[CREATURE_SECURITYGUARD]+=100;
            creaturearray[CREATURE_CORPORATE_MANAGER]+=5;
            creaturearray[CREATURE_WORKER_SWEATSHOP]+=800;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_INDUSTRY_POLLUTER:
         {
            if(sec)creaturearray[CREATURE_SECURITYGUARD]+=100;
            creaturearray[CREATURE_CORPORATE_MANAGER]+=1;
            creaturearray[CREATURE_WORKER_JANITOR]+=10;
            creaturearray[CREATURE_WORKER_SECRETARY]+=10;

            if(law[LAW_LABOR]==-2)
            {
               creaturearray[CREATURE_WORKER_FACTORY_NONUNION]+=20;
               creaturearray[CREATURE_WORKER_FACTORY_CHILD]+=140;
            }
            else if(law[LAW_LABOR]==-1)
            {
               creaturearray[CREATURE_WORKER_FACTORY_NONUNION]+=160;
               creaturearray[CREATURE_WORKER_FACTORY_CHILD]+=1;
            }
            else if(law[LAW_LABOR]==0)
            {
               creaturearray[CREATURE_WORKER_FACTORY_NONUNION]+=80;
               creaturearray[CREATURE_WORKER_FACTORY_UNION]+=80;
            }
            else if(law[LAW_LABOR]==1)
            {
               creaturearray[CREATURE_WORKER_FACTORY_NONUNION]=50;
               creaturearray[CREATURE_WORKER_FACTORY_UNION]=110;
            }
            else creaturearray[CREATURE_WORKER_FACTORY_UNION]=160;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_CORPORATE_HEADQUARTERS:
         {
            if(sec)creaturearray[CREATURE_GUARDDOG]+=100;
            if(sec)creaturearray[CREATURE_SECURITYGUARD]=400;
            else creaturearray[CREATURE_SECURITYGUARD]=40;
            creaturearray[CREATURE_CORPORATE_MANAGER]=20;
            creaturearray[CREATURE_WORKER_JANITOR]=20;
            creaturearray[CREATURE_WORKER_SECRETARY]=40;
            creaturearray[CREATURE_JUDGE_CONSERVATIVE]=1;
            creaturearray[CREATURE_LAWYER]=20;
            creaturearray[CREATURE_PRIEST]=1;
            creaturearray[CREATURE_OFFICEWORKER]=80;
            creaturearray[CREATURE_PROSTITUTE]=1;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_CORPORATE_HOUSE:
         {
            if(sec)creaturearray[CREATURE_MERC]=100;
            if(sec)creaturearray[CREATURE_GUARDDOG]=50;
            else creaturearray[CREATURE_GUARDDOG]=5;
            creaturearray[CREATURE_WORKER_SERVANT]=30;
            creaturearray[CREATURE_WORKER_SECRETARY]=5;
            creaturearray[CREATURE_TEENAGER]=5;
            creaturearray[CREATURE_GENETIC]=1;
            creaturearray[CREATURE_LAWYER]=5;
            creaturearray[CREATURE_PRIEST]=1;
            creaturearray[CREATURE_PROSTITUTE]=1;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_MEDIA_AMRADIO:
         {
            if(sec)creaturearray[CREATURE_SECURITYGUARD]=100;
            else creaturearray[CREATURE_SECURITYGUARD]=10;
            creaturearray[CREATURE_CORPORATE_MANAGER]=2;
            creaturearray[CREATURE_WORKER_JANITOR]=10;
            creaturearray[CREATURE_WORKER_SECRETARY]=10;
            creaturearray[CREATURE_RADIOPERSONALITY]=2;
            creaturearray[CREATURE_ENGINEER]=20;
            creaturearray[CREATURE_OFFICEWORKER]=40;
            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
         case SITE_MEDIA_CABLENEWS:
         {
            if(sec)creaturearray[CREATURE_SECURITYGUARD]=100;
            else creaturearray[CREATURE_SECURITYGUARD]=10;
            creaturearray[CREATURE_CORPORATE_MANAGER]=5;
            creaturearray[CREATURE_WORKER_JANITOR]=20;
            creaturearray[CREATURE_WORKER_SECRETARY]=20;
            creaturearray[CREATURE_NEWSANCHOR]=2;
            creaturearray[CREATURE_ENGINEER]=40;
            creaturearray[CREATURE_OFFICEWORKER]=40;
            creaturearray[CREATURE_PHOTOGRAPHER]=5;
            creaturearray[CREATURE_CAMERAMAN]=5;

      #ifdef SHOWWAIT
         creaturearray[CREATURE_NEWSANCHOR]=20000;
      #endif

            for(int n=0;n<LCSrandom(6)+1;n++)
            {
               makecreature(encounter[encslot],getrandomcreaturetype(creaturearray));
               encslot++;
            }
            break;
         }
      }
   }
}



/* generates a new siege encounter */
char addsiegeencounter(char type)
{
   int num;
   int freeslots=0;
   for(int e=0;e<ENCMAX;e++)
   {
      if(!encounter[e].exists)freeslots++;
   }

   switch(type)
   {
      case SIEGEFLAG_UNIT:
      case SIEGEFLAG_UNIT_DAMAGED:
      {
         if(freeslots<6)return 0;

         num=LCSrandom(3)+4;

         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].exists)continue;

            if(location[cursite]->siege.siege)
            {
               switch(location[cursite]->siege.siegetype)
               {
                  case SIEGE_POLICE:
                     if(location[cursite]->siege.escalationstate==0)
                     {
                        makecreature(encounter[e],CREATURE_SWAT);
                     }
                     else
                     {
                        if (location[cursite]->siege.escalationstate<3) makecreature(encounter[e],CREATURE_SOLDIER);
                        else makecreature(encounter[e], CREATURE_SEAL);
                     }
                     break;
                  case SIEGE_CIA:
                     makecreature(encounter[e],CREATURE_AGENT);
                     break;
                  case SIEGE_HICKS:
                     makecreature(encounter[e],CREATURE_HICK);
                     break;
                  case SIEGE_CORPORATE:
                     makecreature(encounter[e],CREATURE_MERC);
                     break;
                  case SIEGE_CCS:
                     if(!LCSrandom(12))
                     {
                        makecreature(encounter[e],CREATURE_CCS_ARCHCONSERVATIVE);
                     }
                     else if(!LCSrandom(11))
                     {
                        makecreature(encounter[e],CREATURE_CCS_MOLOTOV);
                     }
                     else if(!LCSrandom(10))
                     {
                        makecreature(encounter[e],CREATURE_CCS_SNIPER);
                     }
                     else
                     {
                        makecreature(encounter[e],CREATURE_CCS_VIGILANTE);
                     }
                     break;
                  case SIEGE_FIREMEN:
                     makecreature(encounter[e],CREATURE_FIREFIGHTER);
                     break;
               }
            }
            else
            {
               switch(sitetype)
               {
               case SITE_GOVERNMENT_ARMYBASE:
                  if(e==0 && !LCSrandom(2))
                     makecreature(encounter[e],CREATURE_TANK);
                  else
                     makecreature(encounter[e],CREATURE_SOLDIER);
                  break;
               case SITE_GOVERNMENT_INTELLIGENCEHQ:
                  makecreature(encounter[e],CREATURE_AGENT);
                  break;
               case SITE_CORPORATE_HEADQUARTERS:
               case SITE_CORPORATE_HOUSE:
                  makecreature(encounter[e],CREATURE_MERC);
                  break;
               case SITE_MEDIA_AMRADIO:
               case SITE_MEDIA_CABLENEWS:
                  makecreature(encounter[e],CREATURE_HICK);
                  break;
               case SITE_GOVERNMENT_POLICESTATION:
                  if(law[LAW_DEATHPENALTY]==-2&&
                     law[LAW_POLICEBEHAVIOR]==-2)makecreature(encounter[e],CREATURE_DEATHSQUAD);
                  else makecreature(encounter[e],CREATURE_SWAT);
                  break;
               case SITE_BUSINESS_CRACKHOUSE:
                  makecreature(encounter[e],CREATURE_GANGMEMBER);
                  encounter[e].align=ALIGN_CONSERVATIVE;
                  break;
               default:
                  if(location[cursite]->renting==RENTING_CCS)
                  {
                     if(!LCSrandom(12))
                     {
                        makecreature(encounter[e],CREATURE_CCS_ARCHCONSERVATIVE);
                     }
                     else if(!LCSrandom(11))
                     {
                        makecreature(encounter[e],CREATURE_CCS_MOLOTOV);
                     }
                     else if(!LCSrandom(10))
                     {
                        makecreature(encounter[e],CREATURE_CCS_SNIPER);
                     }
                     else
                     {
                        makecreature(encounter[e],CREATURE_CCS_VIGILANTE);
                     }
                  }
                  else if(law[LAW_DEATHPENALTY]==-2&&
                     law[LAW_POLICEBEHAVIOR]==-2)makecreature(encounter[e],CREATURE_DEATHSQUAD);
                  else makecreature(encounter[e],CREATURE_SWAT);
                  break;
               }
            }

            if(type==SIEGEFLAG_UNIT_DAMAGED)
            {
               encounter[e].blood=LCSrandom(75)+1;
            }

            encounter[e].exists=true;

            num--;
            if(num==0)break;
         }
         break;
      }
      case SIEGEFLAG_HEAVYUNIT:
      {
         if(freeslots<1)return 0;

         num=1;

         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].exists)continue;

            makecreature(encounter[e],CREATURE_TANK);

            num--;
            if(num==0)break;
         }
         break;
      }
   }

   return 1;
}



/* rolls up a random creature type according to the passed weighting array */
int getrandomcreaturetype(int cr[CREATURENUM])
{
   int sum=0;
   for(int c=0;c<CREATURENUM;c++)sum+=cr[c];

   if(sum>0)
   {
      int roll=LCSrandom(sum);
      int sel=0;
      while(roll>=0){roll-=cr[sel];sel++;}
      return sel-1;
   }
   else return -1;
}

