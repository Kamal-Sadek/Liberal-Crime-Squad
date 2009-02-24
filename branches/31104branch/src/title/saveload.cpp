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
   int numbytes;
   FILE *h;
   int l;

   h=LCSOpenFile(str, "wb", LCSIO_PRE_HOME);

   if(h!=NULL)
   {
      int lversion=version;
      numbytes=fwrite(&lversion,sizeof(int),1,h);

      numbytes=fwrite(&seed,sizeof(int),1,h);

      numbytes=fwrite(&mode,sizeof(int),1,h);

      numbytes=fwrite(&day,sizeof(int),1,h);
      numbytes=fwrite(&month,sizeof(int),1,h);
      numbytes=fwrite(&year,sizeof(int),1,h);
      numbytes=fwrite(&execterm,sizeof(short),1,h);
      numbytes=fwrite(&presparty,sizeof(short),1,h);
      numbytes=fwrite(&amendnum,sizeof(int),1,h);

      numbytes=fwrite(&stat_recruits,sizeof(int),1,h);
      numbytes=fwrite(&stat_dead,sizeof(int),1,h);
      numbytes=fwrite(&stat_kills,sizeof(int),1,h);
      numbytes=fwrite(&stat_kidnappings,sizeof(int),1,h);
      numbytes=fwrite(&stat_funds,sizeof(int),1,h);
      numbytes=fwrite(&stat_spent,sizeof(int),1,h);
      numbytes=fwrite(&stat_buys,sizeof(int),1,h);
      numbytes=fwrite(&stat_burns,sizeof(int),1,h);

      numbytes=fwrite(&policestation_closed,sizeof(char),1,h);
      numbytes=fwrite(&amradio_closed,sizeof(char),1,h);
      numbytes=fwrite(&cablenews_closed,sizeof(char),1,h);

      numbytes=fwrite(&endgamestate,sizeof(char),1,h);
      numbytes=fwrite(&ccs_kills,sizeof(char),1,h);

      numbytes=fwrite(&curcarid,sizeof(long),1,h);
      numbytes=fwrite(&showcarprefs,sizeof(char),1,h);
      numbytes=fwrite(&curcreatureid,sizeof(long),1,h);
      numbytes=fwrite(&cursquadid,sizeof(long),1,h);
      numbytes=fwrite(&offended_cops,sizeof(short),1,h);
      numbytes=fwrite(&police_heat,sizeof(int),1,h);
      numbytes=fwrite(&offended_corps,sizeof(short),1,h);
      numbytes=fwrite(&offended_cia,sizeof(short),1,h);
      numbytes=fwrite(&offended_amradio,sizeof(short),1,h);
      numbytes=fwrite(&offended_cablenews,sizeof(short),1,h);
      numbytes=fwrite(&offended_firemen,sizeof(short),1,h);
      numbytes=fwrite(&attorneyseed,sizeof(int),1,h);
      numbytes=fwrite(&selectedsiege,sizeof(long),1,h);
      numbytes=fwrite(lcityname,sizeof(char),80,h);
      numbytes=fwrite(&newscherrybusted,sizeof(char),1,h);
      numbytes=fwrite(&moneygained_donate,sizeof(long),1,h);
      numbytes=fwrite(&moneygained_brownies,sizeof(long),1,h);
      numbytes=fwrite(&moneygained_goods,sizeof(long),1,h);
      numbytes=fwrite(&moneygained_ccfraud,sizeof(long),1,h);
      numbytes=fwrite(&moneygained_hustling,sizeof(long),1,h);
      numbytes=fwrite(&moneygained_extortion,sizeof(long),1,h);
      numbytes=fwrite(&moneygained_thievery,sizeof(long),1,h);
      numbytes=fwrite(&moneygained_embezzlement,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_goods,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_trouble,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_rent,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_training,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_manufacture,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_legal,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_food,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_dating,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_compound,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_hostage,sizeof(long),1,h);
      numbytes=fwrite(&moneylost_confiscated,sizeof(long),1,h);

      numbytes=fwrite(slogan,sizeof(char),80,h);
      numbytes=fwrite(&funds,sizeof(int),1,h);
      numbytes=fwrite(&party_status,sizeof(short),1,h);

      numbytes=fwrite(attitude,sizeof(short),VIEWNUM,h);
      numbytes=fwrite(law,sizeof(short),LAWNUM,h);
      numbytes=fwrite(house,sizeof(short),435,h);
      numbytes=fwrite(senate,sizeof(short),100,h);
      numbytes=fwrite(court,sizeof(short),9,h);
      numbytes=fwrite(courtname,sizeof(char)*80,9,h);
      numbytes=fwrite(exec,sizeof(char),EXECNUM,h);
      numbytes=fwrite(execname,sizeof(char)*80,EXECNUM,h);

      //LOCATIONS
      dummy=location.size();
      numbytes=fwrite(&dummy,sizeof(int),1,h);
      for(l=0;l<location.size();l++)
      {
         dummy=location[l]->loot.size();
         numbytes=fwrite(&dummy,sizeof(int),1,h);
         for(int l2=0;l2<location[l]->loot.size();l2++)
         {
            numbytes=fwrite(location[l]->loot[l2],sizeof(itemst),1,h);
         }
         dummy=location[l]->changes.size();
         numbytes=fwrite(&dummy,sizeof(int),1,h);
         for(int l2=0;l2<location[l]->changes.size();l2++)
         {
            numbytes=fwrite(&location[l]->changes[l2],sizeof(sitechangest),1,h);
         }

         numbytes=fwrite(location[l]->name,sizeof(char),40,h);
         numbytes=fwrite(location[l]->shortname,sizeof(char),20,h);
         numbytes=fwrite(&location[l]->type,sizeof(short),1,h);
         numbytes=fwrite(&location[l]->parent,sizeof(long),1,h);
         numbytes=fwrite(&location[l]->renting,sizeof(long),1,h);
         numbytes=fwrite(&location[l]->newrental,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->needcar,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->closed,sizeof(short),1,h);
         numbytes=fwrite(&location[l]->hidden,sizeof(short),1,h);
         numbytes=fwrite(&location[l]->interrogated,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->highsecurity,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->siege,sizeof(siegest),1,h);
         numbytes=fwrite(&location[l]->heat,sizeof(int),1,h);
         numbytes=fwrite(&location[l]->compound_walls,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->compound_stores,sizeof(long),1,h);
         numbytes=fwrite(&location[l]->front_business,sizeof(short),1,h);
         numbytes=fwrite(location[l]->front_name,sizeof(char),40,h);
         numbytes=fwrite(&location[l]->haveflag,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->mapseed,sizeof(int),1,h);
      }

      //VEHICLES
      dummy=vehicle.size();
      numbytes=fwrite(&dummy,sizeof(int),1,h);
      for(l=0;l<vehicle.size();l++)
      {
         numbytes=fwrite(vehicle[l],sizeof(vehiclest),1,h);
      }

      //POOL
      dummy=pool.size();
      numbytes=fwrite(&dummy,sizeof(int),1,h);
      for(int pl=0;pl<pool.size();pl++)
      {
         numbytes=fwrite(pool[pl],sizeof(creaturest),1,h);
         //write extra interrogation data if applicable
         if(pool[pl]->align==-1 && pool[pl]->alive)
         {
            numbytes=fwrite(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nofood,sizeof(int),1,h);
            numbytes=fwrite(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nowater,sizeof(int),1,h);
            numbytes=fwrite(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nosleep,sizeof(int),1,h);
            numbytes=fwrite(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nolight,sizeof(int),1,h);
            numbytes=fwrite(reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->techniques,sizeof(bool[9]),1,h);
            numbytes=fwrite(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->totalspiritcrush,sizeof(int),1,h);
            numbytes=fwrite(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->druguse,sizeof(int),1,h);

            //deep write rapport map
            int size = reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->rapport.size();
            numbytes=fwrite(&size,sizeof(int),1,h);
            
            map<long,float_zero>::iterator i;
            for(i=reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->rapport.begin();
                i!=reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->rapport.end();++i)
            {
               numbytes=fwrite(&((*i).first),sizeof(long),1,h);
               numbytes=fwrite(&((*i).second),sizeof(float_zero),1,h);
            }
         }
      }

      //SQUADS
      dummy=squad.size();
      numbytes=fwrite(&dummy,sizeof(int),1,h);
      for(int sq=0;sq<squad.size();sq++)
      {
         numbytes=fwrite(squad[sq]->name,sizeof(char),40,h);
         numbytes=fwrite(&squad[sq]->activity,sizeof(activityst),1,h);
         numbytes=fwrite(&squad[sq]->id,sizeof(int),1,h);

         for(int pos=0;pos<6;pos++)
         {
            if(squad[sq]->squad[pos]==NULL)dummy_c=0;
            else dummy_c=1;
            numbytes=fwrite(&dummy_c,sizeof(char),1,h);

            if(squad[sq]->squad[pos]!=NULL)
            {
               numbytes=fwrite(&squad[sq]->squad[pos]->id,sizeof(long),1,h);
            }
         }

         dummy=squad[sq]->loot.size();
         numbytes=fwrite(&dummy,sizeof(int),1,h);
         for(int l2=0;l2<squad[sq]->loot.size();l2++)
         {
            numbytes=fwrite(squad[sq]->loot[l2],sizeof(itemst),1,h);
         }
      }

      if(activesquad==NULL)dummy_c=0;
      else dummy_c=1;
      numbytes=fwrite(&dummy_c,sizeof(char),1,h);
      if(activesquad!=NULL)
      {
         numbytes=fwrite(&activesquad->id,sizeof(long),1,h);
      }

      //DATES
      dummy=date.size();
      numbytes=fwrite(&dummy,sizeof(int),1,h);
      for(int dt=0;dt<date.size();dt++)
      {
         numbytes=fwrite(&date[dt]->mac_id,sizeof(long),1,h);
         numbytes=fwrite(&date[dt]->timeleft,sizeof(short),1,h);
         dummy=date[dt]->date.size();
         numbytes=fwrite(&dummy,sizeof(int),1,h);
         for(int dt2=0;dt2<date[dt]->date.size();dt2++)
         {
            numbytes=fwrite(date[dt]->date[dt2],sizeof(creaturest),1,h);
         }
      }

      //RECRUITS
      dummy=recruit.size();
      numbytes=fwrite(&dummy,sizeof(int),1,h);
      for(int rt=0;rt<recruit.size();rt++)
      {
         numbytes=fwrite(&recruit[rt]->recruiter_id,sizeof(long),1,h);
         numbytes=fwrite(&recruit[rt]->timeleft,sizeof(short),1,h);
         numbytes=fwrite(&recruit[rt]->level,sizeof(char),1,h);
         numbytes=fwrite(&recruit[rt]->eagerness1,sizeof(char),1,h);
         numbytes=fwrite(&recruit[rt]->task,sizeof(char),1,h);
         numbytes=fwrite(recruit[rt]->recruit,sizeof(creaturest),1,h);
      }

      //NEWS STORIES
      dummy=newsstory.size();
      numbytes=fwrite(&dummy,sizeof(int),1,h);
      for(int ns=0;ns<newsstory.size();ns++)
      {
         numbytes=fwrite(&newsstory[ns]->type,sizeof(short),1,h);
         numbytes=fwrite(&newsstory[ns]->view,sizeof(short),1,h);

         numbytes=fwrite(&newsstory[ns]->loc,sizeof(long),1,h);
         numbytes=fwrite(&newsstory[ns]->priority,sizeof(long),1,h);
         numbytes=fwrite(&newsstory[ns]->page,sizeof(long),1,h);
         numbytes=fwrite(&newsstory[ns]->positive,sizeof(char),1,h);
         numbytes=fwrite(&newsstory[ns]->siegetype,sizeof(short),1,h);

         if(newsstory[ns]->cr==NULL)dummy_c=0;
         else dummy_c=1;
         numbytes=fwrite(&dummy_c,sizeof(char),1,h);
         if(newsstory[ns]->cr!=NULL)
         {
            numbytes=fwrite(&newsstory[ns]->cr->id,sizeof(long),1,h);
         }

         dummy=newsstory[ns]->crime.size();
         numbytes=fwrite(&dummy,sizeof(int),1,h);
         for(int dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
         {
            numbytes=fwrite(&newsstory[ns]->crime[dt2],sizeof(int),1,h);
         }
      }

      // Liberal Media
      numbytes=fwrite(public_interest,sizeof(public_interest),1,h);
      numbytes=fwrite(background_liberal_influence,sizeof(background_liberal_influence),1,h);

      LCSCloseFile(h);
   }
}


/* loads the game from save.dat */
char load(void)
{
   //LOAD FILE
   int loadversion;
   int l;
   char dummy_c;
   int dummy;
   long dummy_l;
   FILE *h;

   h=LCSOpenFile("save.dat", "rb", LCSIO_PRE_HOME);
   if(h!=NULL)
   {
      fread(&loadversion,sizeof(int),1,h);

      //NUKE INVALID SAVE GAMES
      if(loadversion<lowestloadversion)
      {
         LCSCloseFile(h);

         reset();

         return 0;
      }

      fread(&seed,sizeof(unsigned int),1,h);

      fread(&mode,sizeof(int),1,h);

      fread(&day,sizeof(int),1,h);
      fread(&month,sizeof(int),1,h);
      fread(&year,sizeof(int),1,h);
      fread(&execterm,sizeof(short),1,h);
      fread(&presparty,sizeof(short),1,h);
      fread(&amendnum,sizeof(int),1,h);

      fread(&stat_recruits,sizeof(int),1,h);
      fread(&stat_dead,sizeof(int),1,h);
      fread(&stat_kills,sizeof(int),1,h);
      fread(&stat_kidnappings,sizeof(int),1,h);
      fread(&stat_funds,sizeof(int),1,h);
      fread(&stat_spent,sizeof(int),1,h);
      fread(&stat_buys,sizeof(int),1,h);
      fread(&stat_burns,sizeof(int),1,h);

      fread(&policestation_closed,sizeof(char),1,h);
      fread(&amradio_closed,sizeof(char),1,h);
      fread(&cablenews_closed,sizeof(char),1,h);

      fread(&endgamestate,sizeof(char),1,h);
      fread(&ccs_kills,sizeof(char),1,h);

      fread(&curcarid,sizeof(long),1,h);
      fread(&showcarprefs,sizeof(char),1,h);
      fread(&curcreatureid,sizeof(long),1,h);
      fread(&cursquadid,sizeof(long),1,h);
      fread(&offended_cops,sizeof(short),1,h);
      fread(&police_heat,sizeof(int),1,h);
      fread(&offended_corps,sizeof(short),1,h);
      fread(&offended_cia,sizeof(short),1,h);
      fread(&offended_amradio,sizeof(short),1,h);
      fread(&offended_cablenews,sizeof(short),1,h);
      fread(&offended_firemen,sizeof(short),1,h);
      fread(&attorneyseed,sizeof(int),1,h);
      fread(&selectedsiege,sizeof(long),1,h);
      fread(lcityname,sizeof(char),80,h);
      fread(&newscherrybusted,sizeof(char),1,h);
      fread(&moneygained_donate,sizeof(long),1,h);
      fread(&moneygained_brownies,sizeof(long),1,h);
      fread(&moneygained_goods,sizeof(long),1,h);
      fread(&moneygained_ccfraud,sizeof(long),1,h);
      fread(&moneygained_hustling,sizeof(long),1,h);
      fread(&moneygained_extortion,sizeof(long),1,h);
      fread(&moneygained_thievery,sizeof(long),1,h);
      fread(&moneygained_embezzlement,sizeof(long),1,h);
      fread(&moneylost_goods,sizeof(long),1,h);
      fread(&moneylost_trouble,sizeof(long),1,h);
      fread(&moneylost_rent,sizeof(long),1,h);
      fread(&moneylost_training,sizeof(long),1,h);
      fread(&moneylost_manufacture,sizeof(long),1,h);
      fread(&moneylost_legal,sizeof(long),1,h);
      fread(&moneylost_food,sizeof(long),1,h);
      fread(&moneylost_dating,sizeof(long),1,h);
      fread(&moneylost_compound,sizeof(long),1,h);
      fread(&moneylost_hostage,sizeof(long),1,h);
      fread(&moneylost_confiscated,sizeof(long),1,h);
      fread(slogan,sizeof(char),80,h);
      fread(&funds,sizeof(int),1,h);
      fread(&party_status,sizeof(short),1,h);

      fread(attitude,sizeof(short)*VIEWNUM,1,h);
      fread(law,sizeof(short)*LAWNUM,1,h);
      fread(house,sizeof(short),435,h);
      fread(senate,sizeof(short),100,h);
      fread(court,sizeof(short),9,h);
      fread(courtname,sizeof(char)*80,9,h);
      fread(exec,sizeof(char)*EXECNUM,1,h);
      fread(execname,sizeof(char)*80,EXECNUM,h);

      //LOCATIONS
      fread(&dummy,sizeof(int),1,h);
      location.resize(dummy);
      for(l=0;l<location.size();l++)
      {
         location[l]=new locationst;

         fread(&dummy,sizeof(int),1,h);
         location[l]->loot.resize(dummy);
         for(int l2=0;l2<location[l]->loot.size();l2++)
         {
            location[l]->loot[l2]=new itemst;
            fread(location[l]->loot[l2],sizeof(itemst),1,h);
         }
         fread(&dummy,sizeof(int),1,h);
         location[l]->changes.resize(dummy);
         for(int l2=0;l2<location[l]->changes.size();l2++)
         {
            fread(&location[l]->changes[l2],sizeof(sitechangest),1,h);
         }

         fread(location[l]->name,sizeof(char),40,h);
         fread(location[l]->shortname,sizeof(char),20,h);
         fread(&location[l]->type,sizeof(short),1,h);
         fread(&location[l]->parent,sizeof(long),1,h);
         fread(&location[l]->renting,sizeof(long),1,h);
         fread(&location[l]->newrental,sizeof(char),1,h);
         fread(&location[l]->needcar,sizeof(char),1,h);
         fread(&location[l]->closed,sizeof(short),1,h);
         fread(&location[l]->hidden,sizeof(short),1,h);
         fread(&location[l]->interrogated,sizeof(char),1,h);
         fread(&location[l]->highsecurity,sizeof(char),1,h);
         fread(&location[l]->siege,sizeof(siegest),1,h);
         fread(&location[l]->heat,sizeof(int),1,h);
         fread(&location[l]->compound_walls,sizeof(char),1,h);
         fread(&location[l]->compound_stores,sizeof(long),1,h);
         fread(&location[l]->front_business,sizeof(short),1,h);
         fread(location[l]->front_name,sizeof(char),40,h);
         fread(&location[l]->haveflag,sizeof(char),1,h);
         fread(&location[l]->mapseed,sizeof(int),1,h);
      }

      //VEHICLES
      fread(&dummy,sizeof(int),1,h);
      vehicle.resize(dummy);
      for(l=0;l<vehicle.size();l++)
      {
         vehicle[l]=new vehiclest;
         fread(vehicle[l],sizeof(vehiclest),1,h);
      }

      //POOL
      fread(&dummy,sizeof(int),1,h);
      pool.resize(dummy);
      for(int pl=0;pl<pool.size();pl++)
      {
         pool[pl]=new creaturest;
         fread(pool[pl],sizeof(creaturest),1,h);
         //read extra interrogation data if applicable
         if(pool[pl]->align==-1 && pool[pl]->alive)
         {
            pool[pl]->activity.arg = reinterpret_cast<int>(new interrogation);
            fread(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nofood,sizeof(int),1,h);
            fread(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nowater,sizeof(int),1,h);
            fread(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nosleep,sizeof(int),1,h);
            fread(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->nolight,sizeof(int),1,h);
            fread(reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->techniques,sizeof(bool[9]),1,h);
            fread(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->totalspiritcrush,sizeof(int),1,h);
            fread(&reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->druguse,sizeof(int),1,h);

            reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->rapport.clear();
            int size;
            fread(&size,sizeof(int),1,h);
            int i;
            for(i=0;i<size;++i)
            {
               long id;
               float_zero value;
               fread(&id,sizeof(long),1,h);
               fread(&value,sizeof(float_zero),1,h);
               reinterpret_cast<interrogation*>(pool[pl]->activity.arg)->rapport[id]=value;
            }
         }
      }

      //SQUADS
      fread(&dummy,sizeof(int),1,h);
      squad.resize(dummy);
      for(int sq=0;sq<squad.size();sq++)
      {
         squad[sq]=new squadst;

         fread(squad[sq]->name,sizeof(char),40,h);
         fread(&squad[sq]->activity,sizeof(activityst),1,h);
         fread(&squad[sq]->id,sizeof(int),1,h);

         for(int pos=0;pos<6;pos++)
         {
            fread(&dummy_c,sizeof(char),1,h);

            //REBUILD SQUAD FROM POOL
            squad[sq]->squad[pos]=NULL;
            if(dummy_c)
            {
               fread(&dummy_l,sizeof(long),1,h);
               for(int pl=0;pl<pool.size();pl++)
               {
                  if(pool[pl]->id==dummy_l)
                  {
                     squad[sq]->squad[pos]=pool[pl];
                  }
               }
            }
         }

         fread(&dummy,sizeof(int),1,h);
         squad[sq]->loot.resize(dummy);
         for(int l2=0;l2<squad[sq]->loot.size();l2++)
         {
            squad[sq]->loot[l2]=new itemst;
            fread(squad[sq]->loot[l2],sizeof(itemst),1,h);
         }
      }

      activesquad=NULL;
      fread(&dummy_c,sizeof(char),1,h);
      if(dummy_c)
      {
         fread(&dummy_l,sizeof(long),1,h);
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
      fread(&dummy,sizeof(int),1,h);
      date.resize(dummy);
      for(int dt=0;dt<date.size();dt++)
      {
         date[dt]=new datest;

         fread(&date[dt]->mac_id,sizeof(long),1,h);
         fread(&date[dt]->timeleft,sizeof(short),1,h);

         fread(&dummy,sizeof(int),1,h);
         date[dt]->date.resize(dummy);
         for(int dt2=0;dt2<date[dt]->date.size();dt2++)
         {
            date[dt]->date[dt2]=new creaturest;
            fread(date[dt]->date[dt2],sizeof(creaturest),1,h);
         }
      }

      //RECRUITS
      fread(&dummy,sizeof(int),1,h);
      recruit.resize(dummy);
      for(int rt=0;rt<recruit.size();rt++)
      {
         recruit[rt]=new recruitst;
         fread(&recruit[rt]->recruiter_id,sizeof(long),1,h);
         fread(&recruit[rt]->timeleft,sizeof(short),1,h);
         fread(&recruit[rt]->level,sizeof(char),1,h);
         fread(&recruit[rt]->eagerness1,sizeof(char),1,h);
         fread(&recruit[rt]->task,sizeof(char),1,h);
         recruit[rt]->recruit = new creaturest;
         fread(recruit[rt]->recruit,sizeof(creaturest),1,h);
      }

      //NEWS STORIES
      fread(&dummy,sizeof(int),1,h);
      newsstory.resize(dummy);
      for(int ns=0;ns<newsstory.size();ns++)
      {
         newsstory[ns]=new newsstoryst;

         fread(&newsstory[ns]->type,sizeof(short),1,h);
         fread(&newsstory[ns]->view,sizeof(short),1,h);

         fread(&newsstory[ns]->loc,sizeof(long),1,h);
         fread(&newsstory[ns]->priority,sizeof(long),1,h);
         fread(&newsstory[ns]->page,sizeof(long),1,h);
         fread(&newsstory[ns]->positive,sizeof(char),1,h);
         fread(&newsstory[ns]->siegetype,sizeof(short),1,h);

         fread(&dummy_c,sizeof(char),1,h);
         if(dummy_c)
         {
            fwrite(&dummy_l,sizeof(long),1,h);
            for(int pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->id==dummy_l)
               {
                  newsstory[ns]->cr=pool[pl];
                  break;
               }
            }
         }

         fread(&dummy,sizeof(int),1,h);
         newsstory[ns]->crime.resize(dummy);
         for(int dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
         {
            fread(&newsstory[ns]->crime[dt2],sizeof(int),1,h);
         }
      }

      // Liberal Media
      fread(public_interest,sizeof(public_interest),1,h);
      fread(background_liberal_influence,sizeof(background_liberal_influence),1,h);
      LCSCloseFile(h);

      return 1;
   }

   return 0;
}


/* deletes save.dat (used on endgame and for invalid save version) */
void reset(void)
{
    LCSDeleteFile("save.dat",LCSIO_PRE_HOME);
}

