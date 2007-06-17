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


/* saves the game to save.dat */
void save(void)
{
 savegame("save.dat");
}



/* saves the game to autosave.dat */
void autosave(void)
{
 savegame("autosave.dat");
}


/* handles saving */
void savegame(char *str)
{
   char dummy_c;
   int dummy;
   DWORD numbytes;
   HANDLE h;
   int l;
   
   h=LCSCreateFile(str, LCSIO_WRITE);
   if(h!=NULL)
   {
      WriteFile(h,&version,sizeof(unsigned long),&numbytes,NULL);

      WriteFile(h,&seed,sizeof(unsigned long),&numbytes,NULL);

      WriteFile(h,&mode,sizeof(int),&numbytes,NULL);

      WriteFile(h,&day,sizeof(int),&numbytes,NULL);
      WriteFile(h,&month,sizeof(int),&numbytes,NULL);
      WriteFile(h,&year,sizeof(int),&numbytes,NULL);
      WriteFile(h,&execterm,sizeof(short),&numbytes,NULL);
      WriteFile(h,&amendnum,sizeof(int),&numbytes,NULL);

      WriteFile(h,&stat_recruits,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&stat_dead,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&stat_kills,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&stat_kidnappings,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&stat_funds,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&stat_spent,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&stat_buys,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&stat_burns,sizeof(unsigned long),&numbytes,NULL);

      WriteFile(h,&curcarid,sizeof(long),&numbytes,NULL);
      WriteFile(h,&showcarprefs,sizeof(char),&numbytes,NULL);
      WriteFile(h,&curcreatureid,sizeof(long),&numbytes,NULL);
      WriteFile(h,&cursquadid,sizeof(long),&numbytes,NULL);
      WriteFile(h,&offended_cops,sizeof(short),&numbytes,NULL);
      WriteFile(h,&police_heat,sizeof(int),&numbytes,NULL);
      WriteFile(h,&offended_corps,sizeof(short),&numbytes,NULL);
      WriteFile(h,&offended_cia,sizeof(short),&numbytes,NULL);
      WriteFile(h,&offended_amradio,sizeof(short),&numbytes,NULL);
      WriteFile(h,&offended_cablenews,sizeof(short),&numbytes,NULL);
      WriteFile(h,&attorneyseed,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&selectedsiege,sizeof(long),&numbytes,NULL);
      WriteFile(h,lcityname,sizeof(char)*80,&numbytes,NULL);
      WriteFile(h,&newscherrybusted,sizeof(char),&numbytes,NULL);
      WriteFile(h,newspaper_topicwork,sizeof(short)*VIEWNUM,&numbytes,NULL);
      WriteFile(h,&moneygained_donate,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneygained_brownies,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneygained_goods,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneygained_ccfraud,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneygained_hustling,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneygained_thievery,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneylost_goods,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneylost_trouble,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneylost_rent,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneylost_manufacture,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneylost_legal,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneylost_compound,sizeof(long),&numbytes,NULL);
      WriteFile(h,&moneylost_hostage,sizeof(long),&numbytes,NULL);

      WriteFile(h,slogan,sizeof(char)*80,&numbytes,NULL);
      WriteFile(h,&funds,sizeof(unsigned long),&numbytes,NULL);
      WriteFile(h,&party_status,sizeof(short),&numbytes,NULL);

      WriteFile(h,attitude,sizeof(short)*VIEWNUM,&numbytes,NULL);
      WriteFile(h,law,sizeof(short)*LAWNUM,&numbytes,NULL);
      WriteFile(h,house,sizeof(short)*435,&numbytes,NULL);
      WriteFile(h,senate,sizeof(short)*100,&numbytes,NULL);
      WriteFile(h,court,sizeof(short)*9,&numbytes,NULL);
      WriteFile(h,courtname,sizeof(char)*9*80,&numbytes,NULL);
      WriteFile(h,exec,sizeof(short)*EXECNUM,&numbytes,NULL);
      WriteFile(h,execname,sizeof(char)*EXECNUM*80,&numbytes,NULL);

      //LOCATIONS
      dummy=location.size();
      WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
      for(l=0;l<location.size();l++)
      {
         dummy=location[l]->loot.size();
         WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
         for(int l2=0;l2<location[l]->loot.size();l2++)
         {
            WriteFile(h,location[l]->loot[l2],sizeof(itemst),&numbytes,NULL);
         }

         WriteFile(h,location[l]->name,sizeof(char)*40,&numbytes,NULL);
         WriteFile(h,location[l]->shortname,sizeof(char)*20,&numbytes,NULL);
         WriteFile(h,&location[l]->type,sizeof(short),&numbytes,NULL);
         WriteFile(h,&location[l]->parent,sizeof(long),&numbytes,NULL);
         WriteFile(h,&location[l]->renting,sizeof(long),&numbytes,NULL);
         WriteFile(h,&location[l]->newrental,sizeof(char),&numbytes,NULL);
         WriteFile(h,&location[l]->needcar,sizeof(char),&numbytes,NULL);
         WriteFile(h,&location[l]->closed,sizeof(short),&numbytes,NULL);
         WriteFile(h,&location[l]->highsecurity,sizeof(char),&numbytes,NULL);
         WriteFile(h,&location[l]->siege,sizeof(siegest),&numbytes,NULL);
         WriteFile(h,&location[l]->heat,sizeof(int),&numbytes,NULL);
         WriteFile(h,&location[l]->compound_walls,sizeof(char),&numbytes,NULL);
         WriteFile(h,&location[l]->compound_stores,sizeof(long),&numbytes,NULL);
         WriteFile(h,&location[l]->front_business,sizeof(short),&numbytes,NULL);
         WriteFile(h,location[l]->front_name,sizeof(char)*40,&numbytes,NULL);
         WriteFile(h,&location[l]->haveflag,sizeof(char),&numbytes,NULL);
         WriteFile(h,&location[l]->mapseed,sizeof(unsigned long),&numbytes,NULL);
      }

      //VEHICLES
      dummy=vehicle.size();
      WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
      for(l=0;l<vehicle.size();l++)
      {
         WriteFile(h,vehicle[l],sizeof(vehiclest),&numbytes,NULL);
      }

      //POOL
      dummy=pool.size();
      WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
      for(int pl=0;pl<pool.size();pl++)
      {
         WriteFile(h,pool[pl],sizeof(creaturest),&numbytes,NULL);
      }

      //SQUADS
      dummy=squad.size();
      WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
      for(int sq=0;sq<squad.size();sq++)
      {
         WriteFile(h,squad[sq]->name,sizeof(char)*40,&numbytes,NULL);
         WriteFile(h,&squad[sq]->activity,sizeof(activityst),&numbytes,NULL);
         WriteFile(h,&squad[sq]->id,sizeof(int),&numbytes,NULL);

         for(int pos=0;pos<6;pos++)
         {
            if(squad[sq]->squad[pos]==NULL)dummy_c=0;
            else dummy_c=1;
            WriteFile(h,&dummy_c,sizeof(char),&numbytes,NULL);

            if(squad[sq]->squad[pos]!=NULL)
            {
               WriteFile(h,&squad[sq]->squad[pos]->id,sizeof(long),&numbytes,NULL);
            }
         }

         dummy=squad[sq]->loot.size();
         WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
         for(int l2=0;l2<squad[sq]->loot.size();l2++)
         {
            WriteFile(h,squad[sq]->loot[l2],sizeof(itemst),&numbytes,NULL);
         }
      }

      if(activesquad==NULL)dummy_c=0;
      else dummy_c=1;
      WriteFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
      if(activesquad!=NULL)
      {
         WriteFile(h,&activesquad->id,sizeof(long),&numbytes,NULL);
      }

      //DATES
      dummy=date.size();
      WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
      for(int dt=0;dt<date.size();dt++)
      {
         WriteFile(h,&date[dt]->mac_id,sizeof(long),&numbytes,NULL);
         WriteFile(h,&date[dt]->timeleft,sizeof(short),&numbytes,NULL);
         dummy=date[dt]->date.size();
         WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
         for(int dt2=0;dt2<date[dt]->date.size();dt2++)
         {
            WriteFile(h,date[dt]->date[dt2],sizeof(creaturest),&numbytes,NULL);
         }
      }

      //NEWS STORIES
      dummy=newsstory.size();
      WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
      for(int ns=0;ns<newsstory.size();ns++)
      {
         WriteFile(h,&newsstory[ns]->type,sizeof(short),&numbytes,NULL);
         WriteFile(h,&newsstory[ns]->view,sizeof(short),&numbytes,NULL);

         WriteFile(h,&newsstory[ns]->loc,sizeof(long),&numbytes,NULL);
         WriteFile(h,&newsstory[ns]->priority,sizeof(long),&numbytes,NULL);
         WriteFile(h,&newsstory[ns]->page,sizeof(long),&numbytes,NULL);
         WriteFile(h,&newsstory[ns]->positive,sizeof(char),&numbytes,NULL);
         WriteFile(h,&newsstory[ns]->siegetype,sizeof(short),&numbytes,NULL);

         if(newsstory[ns]->cr==NULL)dummy_c=0;
         else dummy_c=1;
         WriteFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
         if(newsstory[ns]->cr!=NULL)
         {
            WriteFile(h,&newsstory[ns]->cr->id,sizeof(long),&numbytes,NULL);
         }

         dummy=newsstory[ns]->crime.size();
         WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
         for(int dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
         {
            WriteFile(h,&newsstory[ns]->crime[dt2],sizeof(int),&numbytes,NULL);
         }
      }

      //blog posts
      dummy=blogpost.size();
      WriteFile(h,&dummy,sizeof(int),&numbytes,NULL);
      for(int bp=0;bp<blogpost.size();bp++)
      {
         WriteFile(h,blogpost[l],sizeof(blogpostst),&numbytes,NULL);
      }

      CloseHandle(h);
   }
}


/* loads the game from save.dat */
char load(void)
{
   //LOAD FILE
   unsigned long loadversion;
      int l;
   char dummy_c;
   int dummy;
   long dummy_l;
   DWORD numbytes;
   HANDLE h;
   
   h=LCSCreateFile("save.dat", LCSIO_READ);
      
   if(h!=NULL)
   {
      ReadFile(h,&loadversion,sizeof(unsigned long),&numbytes,NULL);

      //NUKE INVALID SAVE GAMES
      if(loadversion<lowestloadversion)
      {
         CloseHandle(h);

         reset();

         return 0;
      }

      ReadFile(h,&seed,sizeof(unsigned long),&numbytes,NULL);

      ReadFile(h,&mode,sizeof(int),&numbytes,NULL);

      ReadFile(h,&day,sizeof(int),&numbytes,NULL);
      ReadFile(h,&month,sizeof(int),&numbytes,NULL);
      ReadFile(h,&year,sizeof(int),&numbytes,NULL);
      ReadFile(h,&execterm,sizeof(short),&numbytes,NULL);
      ReadFile(h,&amendnum,sizeof(int),&numbytes,NULL);

      ReadFile(h,&stat_recruits,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&stat_dead,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&stat_kills,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&stat_kidnappings,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&stat_funds,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&stat_spent,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&stat_buys,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&stat_burns,sizeof(unsigned long),&numbytes,NULL);

      ReadFile(h,&curcarid,sizeof(long),&numbytes,NULL);
      ReadFile(h,&showcarprefs,sizeof(char),&numbytes,NULL);
      ReadFile(h,&curcreatureid,sizeof(long),&numbytes,NULL);
      ReadFile(h,&cursquadid,sizeof(long),&numbytes,NULL);
      ReadFile(h,&offended_cops,sizeof(short),&numbytes,NULL);
      ReadFile(h,&police_heat,sizeof(int),&numbytes,NULL);
      ReadFile(h,&offended_corps,sizeof(short),&numbytes,NULL);
      ReadFile(h,&offended_cia,sizeof(short),&numbytes,NULL);
      ReadFile(h,&offended_amradio,sizeof(short),&numbytes,NULL);
      ReadFile(h,&offended_cablenews,sizeof(short),&numbytes,NULL);
      ReadFile(h,&attorneyseed,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&selectedsiege,sizeof(long),&numbytes,NULL);
      ReadFile(h,lcityname,sizeof(char)*80,&numbytes,NULL);
      ReadFile(h,&newscherrybusted,sizeof(char),&numbytes,NULL);
      ReadFile(h,newspaper_topicwork,sizeof(short)*VIEWNUM,&numbytes,NULL);
      ReadFile(h,&moneygained_donate,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneygained_brownies,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneygained_goods,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneygained_ccfraud,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneygained_hustling,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneygained_thievery,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneylost_goods,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneylost_trouble,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneylost_rent,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneylost_manufacture,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneylost_legal,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneylost_compound,sizeof(long),&numbytes,NULL);
      ReadFile(h,&moneylost_hostage,sizeof(long),&numbytes,NULL);

      ReadFile(h,slogan,sizeof(char)*80,&numbytes,NULL);
      ReadFile(h,&funds,sizeof(unsigned long),&numbytes,NULL);
      ReadFile(h,&party_status,sizeof(short),&numbytes,NULL);

      ReadFile(h,attitude,sizeof(short)*VIEWNUM,&numbytes,NULL);
      ReadFile(h,law,sizeof(short)*LAWNUM,&numbytes,NULL);
      ReadFile(h,house,sizeof(short)*435,&numbytes,NULL);
      ReadFile(h,senate,sizeof(short)*100,&numbytes,NULL);
      ReadFile(h,court,sizeof(short)*9,&numbytes,NULL);
      ReadFile(h,courtname,sizeof(char)*9*80,&numbytes,NULL);
      ReadFile(h,exec,sizeof(short)*EXECNUM,&numbytes,NULL);
      ReadFile(h,execname,sizeof(char)*EXECNUM*80,&numbytes,NULL);

      //LOCATIONS
      ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
      location.resize(dummy);
      for(l=0;l<location.size();l++)
      {
         location[l]=new locationst;

         ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
         location[l]->loot.resize(dummy);
         for(int l2=0;l2<location[l]->loot.size();l2++)
         {
            location[l]->loot[l2]=new itemst;
            ReadFile(h,location[l]->loot[l2],sizeof(itemst),&numbytes,NULL);
         }

         ReadFile(h,location[l]->name,sizeof(char)*40,&numbytes,NULL);
         ReadFile(h,location[l]->shortname,sizeof(char)*20,&numbytes,NULL);
         ReadFile(h,&location[l]->type,sizeof(short),&numbytes,NULL);
         ReadFile(h,&location[l]->parent,sizeof(long),&numbytes,NULL);
         ReadFile(h,&location[l]->renting,sizeof(long),&numbytes,NULL);
         ReadFile(h,&location[l]->newrental,sizeof(char),&numbytes,NULL);
         ReadFile(h,&location[l]->needcar,sizeof(char),&numbytes,NULL);
         ReadFile(h,&location[l]->closed,sizeof(short),&numbytes,NULL);
         ReadFile(h,&location[l]->highsecurity,sizeof(char),&numbytes,NULL);
         ReadFile(h,&location[l]->siege,sizeof(siegest),&numbytes,NULL);
         ReadFile(h,&location[l]->heat,sizeof(int),&numbytes,NULL);
         ReadFile(h,&location[l]->compound_walls,sizeof(char),&numbytes,NULL);
         ReadFile(h,&location[l]->compound_stores,sizeof(long),&numbytes,NULL);
         ReadFile(h,&location[l]->front_business,sizeof(short),&numbytes,NULL);
         ReadFile(h,location[l]->front_name,sizeof(char)*40,&numbytes,NULL);
         ReadFile(h,&location[l]->haveflag,sizeof(char),&numbytes,NULL);
         ReadFile(h,&location[l]->mapseed,sizeof(unsigned long),&numbytes,NULL);
      }

      //VEHICLES
      ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
      vehicle.resize(dummy);
      for(l=0;l<vehicle.size();l++)
      {
         vehicle[l]=new vehiclest;
         ReadFile(h,vehicle[l],sizeof(vehiclest),&numbytes,NULL);
      }

      //POOL
      ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
      pool.resize(dummy);
      for(int pl=0;pl<pool.size();pl++)
      {
         pool[pl]=new creaturest;
         ReadFile(h,pool[pl],sizeof(creaturest),&numbytes,NULL);
      }

      //SQUADS
      ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
      squad.resize(dummy);
      for(int sq=0;sq<squad.size();sq++)
      {
         squad[sq]=new squadst;

         ReadFile(h,squad[sq]->name,sizeof(char)*40,&numbytes,NULL);
         ReadFile(h,&squad[sq]->activity,sizeof(activityst),&numbytes,NULL);
         ReadFile(h,&squad[sq]->id,sizeof(int),&numbytes,NULL);

         for(int pos=0;pos<6;pos++)
         {
            ReadFile(h,&dummy_c,sizeof(char),&numbytes,NULL);

            //REBUILD SQUAD FROM POOL
            squad[sq]->squad[pos]=NULL;
            if(dummy_c)
            {
               ReadFile(h,&dummy_l,sizeof(long),&numbytes,NULL);
               for(int pl=0;pl<pool.size();pl++)
               {
                  if(pool[pl]->id==dummy_l)
                  {
                     squad[sq]->squad[pos]=pool[pl];
                  }
               }
            }
         }

         ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
         squad[sq]->loot.resize(dummy);
         for(int l2=0;l2<squad[sq]->loot.size();l2++)
         {
            squad[sq]->loot[l2]=new itemst;
            ReadFile(h,squad[sq]->loot[l2],sizeof(itemst),&numbytes,NULL);
         }
      }

      activesquad=NULL;
      ReadFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
      if(dummy_c)
      {
         ReadFile(h,&dummy_l,sizeof(long),&numbytes,NULL);
         for(int sq=0;sq<squad.size();sq++)
         {
            if(squad[sq]->id==dummy_l)
            {
               activesquad=squad[sq];
               break;
            }
         }
      }

      //DATES
      ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
      date.resize(dummy);
      for(int dt=0;dt<date.size();dt++)
      {
         date[dt]=new datest;

         ReadFile(h,&date[dt]->mac_id,sizeof(long),&numbytes,NULL);
         ReadFile(h,&date[dt]->timeleft,sizeof(short),&numbytes,NULL);

         ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
         date[dt]->date.resize(dummy);
         for(int dt2=0;dt2<date[dt]->date.size();dt2++)
         {
            date[dt]->date[dt2]=new creaturest;
            ReadFile(h,date[dt]->date[dt2],sizeof(creaturest),&numbytes,NULL);
         }
      }

      //NEWS STORIES
      ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
      newsstory.resize(dummy);
      for(int ns=0;ns<newsstory.size();ns++)
      {
         newsstory[ns]=new newsstoryst;

         ReadFile(h,&newsstory[ns]->type,sizeof(short),&numbytes,NULL);
         ReadFile(h,&newsstory[ns]->view,sizeof(short),&numbytes,NULL);

         ReadFile(h,&newsstory[ns]->loc,sizeof(long),&numbytes,NULL);
         ReadFile(h,&newsstory[ns]->priority,sizeof(long),&numbytes,NULL);
         ReadFile(h,&newsstory[ns]->page,sizeof(long),&numbytes,NULL);
         ReadFile(h,&newsstory[ns]->positive,sizeof(char),&numbytes,NULL);
         ReadFile(h,&newsstory[ns]->siegetype,sizeof(short),&numbytes,NULL);

         ReadFile(h,&dummy_c,sizeof(char),&numbytes,NULL);
         if(dummy_c)
         {
            WriteFile(h,&dummy_l,sizeof(long),&numbytes,NULL);
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->id==dummy_l)
               {
                  newsstory[ns]->cr=pool[pl];
                  break;
               }
            }
         }

         ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
         newsstory[ns]->crime.resize(dummy);
         for(int dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
         {
            ReadFile(h,&newsstory[ns]->crime[dt2],sizeof(int),&numbytes,NULL);
         }
      }

      //blog posts
      ReadFile(h,&dummy,sizeof(int),&numbytes,NULL);
      blogpost.resize(dummy);
      for(int bp=0;bp<blogpost.size();bp++)
      {
         blogpost[bp]=new blogpostst;
         ReadFile(h,blogpost[l],sizeof(blogpostst),&numbytes,NULL);
      }

      CloseHandle(h);

      return 1;
   }

   return 0;
}


/* deletes save.dat (used on endgame and for invalid save version) */
void reset(void)
{
   unlink("save.dat");
}

