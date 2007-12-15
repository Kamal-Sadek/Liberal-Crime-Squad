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
   int32 dummy;
   uint32 numbytes;
   FILE *h;
   int32 l;

   uint32 temp;
   uint16 temp2;

   h=LCSOpenFile(str, "wb", LCSIO_PRE_HOME);
   if(h!=NULL)
   {
      uint32 lversion=version;
      temp=swap_endian_32(lversion);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);

      temp=swap_endian_32(seed);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);

      temp=swap_endian_32(mode);
      numbytes=fwrite(&temp,sizeof(int32),1,h);

      temp=swap_endian_32(day);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(month);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(year);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_16(execterm);
      numbytes=fwrite(&temp,sizeof(int16),1,h);
      temp=swap_endian_32(amendnum);
      numbytes=fwrite(&temp,sizeof(int32),1,h);

      temp=swap_endian_32(stat_recruits);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(stat_dead);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(stat_kills);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(stat_kidnappings);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(stat_funds);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(stat_spent);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(stat_buys);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(stat_burns);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);

      temp=swap_endian_32(curcarid);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      numbytes=fwrite(&showcarprefs,sizeof(char),1,h);
      temp=swap_endian_32(curcreatureid);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(cursquadid);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp2=swap_endian_16(offended_cops);
      numbytes=fwrite(&temp2,sizeof(int16),1,h);
      temp=swap_endian_32(police_heat);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp2=swap_endian_16(offended_corps);
      numbytes=fwrite(&temp2,sizeof(int16),1,h);
      temp2=swap_endian_16(offended_cia);
      numbytes=fwrite(&temp2,sizeof(int16),1,h);
      temp2=swap_endian_16(offended_amradio);
      numbytes=fwrite(&temp2,sizeof(int16),1,h);
      temp2=swap_endian_16(offended_cablenews);
      numbytes=fwrite(&temp2,sizeof(int16),1,h);
      temp=swap_endian_32(attorneyseed);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp=swap_endian_32(selectedsiege);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      numbytes=fwrite(lcityname,sizeof(char),80,h);
      numbytes=fwrite(&newscherrybusted,sizeof(char),1,h);
      temp=swap_endian_32(moneygained_donate);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneygained_brownies);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneygained_goods);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneygained_ccfraud);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneygained_hustling);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneygained_thievery);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneylost_goods);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneylost_trouble);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneylost_rent);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneylost_manufacture);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneylost_legal);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneylost_compound);
      numbytes=fwrite(&temp,sizeof(int32),1,h);
      temp=swap_endian_32(moneylost_hostage);
      numbytes=fwrite(&temp,sizeof(int32),1,h);

      numbytes=fwrite(slogan,sizeof(char),80,h);
      temp=swap_endian_32(funds);
      numbytes=fwrite(&temp,sizeof(uint32),1,h);
      temp2=swap_endian_16(party_status);
      numbytes=fwrite(&temp2,sizeof(int16),1,h);

      int16 *temparr=new int16[VIEWNUM];
      for(int i=0;i<VIEWNUM;i++)
        temparr[i]=swap_endian_16(attitude[i]);
      numbytes=fwrite(temparr,sizeof(int16),VIEWNUM,h);
      delete temparr;

      temparr=new int16[LAWNUM];
      for(int i=0;i<LAWNUM;i++)
        temparr[i]=swap_endian_16(law[i]);
      numbytes=fwrite(temparr,sizeof(int16),LAWNUM,h);
      delete temparr;

      temparr=new int16[435];
      for(int i=0;i<435;i++)
        temparr[i]=swap_endian_16(house[i]);
      numbytes=fwrite(temparr,sizeof(int16),435,h);
      delete temparr;

      temparr=new int16[100];
      for(int i=0;i<100;i++)
        temparr[i]=swap_endian_16(senate[i]);
      numbytes=fwrite(temparr,sizeof(int16),100,h);
      delete temparr;


      temparr=new int16[100];
      for(int i=0;i<100;i++)
        temparr[i]=swap_endian_16(court[i]);
      numbytes=fwrite(temparr,sizeof(int16),9,h);
      delete temparr;

      numbytes=fwrite(courtname,sizeof(char)*80,9,h);
      temp=swap_endian_32(mode);

      temparr=new int16[EXECNUM];
      for(int i=0;i<EXECNUM;i++)
        temparr[i]=swap_endian_16(exec[i]);
      numbytes=fwrite(temparr,sizeof(int16),EXECNUM,h);
      delete temparr;

      numbytes=fwrite(execname,sizeof(char)*80,EXECNUM,h);


      //LOCATIONS

      dummy=location.size();
      temp=swap_endian_32(mode);
      numbytes=fwrite(&temp,sizeof(int32),1,h);



      //todo: continue converting from here

      for(l=0;l<location.size();l++)
      {
         dummy=swap_endian_32(location[l]->loot.size());
         numbytes=fwrite(&dummy,sizeof(int32),1,h);
         for(int32 l2=0;l2<location[l]->loot.size();l2++)
         {

            numbytes=fwrite(location[l]->loot[l2],sizeof(itemst),1,h);
         }

         numbytes=fwrite(location[l]->name,sizeof(char),40,h);
         numbytes=fwrite(location[l]->shortname,sizeof(char),20,h);
         numbytes=fwrite(&location[l]->type,sizeof(int16),1,h);
         numbytes=fwrite(&location[l]->parent,sizeof(int32),1,h);
         numbytes=fwrite(&location[l]->renting,sizeof(int32),1,h);
         numbytes=fwrite(&location[l]->newrental,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->needcar,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->closed,sizeof(int16),1,h);
         numbytes=fwrite(&location[l]->highsecurity,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->siege,sizeof(siegest),1,h);
         numbytes=fwrite(&location[l]->heat,sizeof(int32),1,h);
         numbytes=fwrite(&location[l]->compound_walls,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->compound_stores,sizeof(int32),1,h);
         numbytes=fwrite(&location[l]->front_business,sizeof(int16),1,h);
         numbytes=fwrite(location[l]->front_name,sizeof(char),40,h);
         numbytes=fwrite(&location[l]->haveflag,sizeof(char),1,h);
         numbytes=fwrite(&location[l]->mapseed,sizeof(uint32),1,h);
      }

      //VEHICLES
      dummy=vehicle.size();
      numbytes=fwrite(&dummy,sizeof(int32),1,h);
      for(l=0;l<vehicle.size();l++)
      {
         numbytes=fwrite(vehicle[l],sizeof(vehiclest),1,h);
      }

      //POOL
      dummy=pool.size();
      numbytes=fwrite(&dummy,sizeof(int32),1,h);
      for(int32 pl=0;pl<pool.size();pl++)
      {
         numbytes=fwrite(pool[pl],sizeof(creaturest),1,h);
      }

      //SQUADS
      dummy=squad.size();
      numbytes=fwrite(&dummy,sizeof(int32),1,h);
      for(int32 sq=0;sq<squad.size();sq++)
      {
         numbytes=fwrite(squad[sq]->name,sizeof(char),40,h);
         numbytes=fwrite(&squad[sq]->activity,sizeof(activityst),1,h);
         numbytes=fwrite(&squad[sq]->id,sizeof(int32),1,h);

         for(int32 pos=0;pos<6;pos++)
         {
            if(squad[sq]->squad[pos]==NULL)dummy_c=0;
            else dummy_c=1;
            numbytes=fwrite(&dummy_c,sizeof(char),1,h);

            if(squad[sq]->squad[pos]!=NULL)
            {
               numbytes=fwrite(&squad[sq]->squad[pos]->id,sizeof(int32),1,h);
            }
         }

         dummy=squad[sq]->loot.size();
         numbytes=fwrite(&dummy,sizeof(int32),1,h);
         for(int32 l2=0;l2<squad[sq]->loot.size();l2++)
         {
            numbytes=fwrite(squad[sq]->loot[l2],sizeof(itemst),1,h);
         }
      }

      if(activesquad==NULL)dummy_c=0;
      else dummy_c=1;
      numbytes=fwrite(&dummy_c,sizeof(char),1,h);
      if(activesquad!=NULL)
      {
         numbytes=fwrite(&activesquad->id,sizeof(int32),1,h);
      }

      //DATES
      dummy=date.size();
      numbytes=fwrite(&dummy,sizeof(int32),1,h);
      for(int32 dt=0;dt<date.size();dt++)
      {
         numbytes=fwrite(&date[dt]->mac_id,sizeof(int32),1,h);
         numbytes=fwrite(&date[dt]->timeleft,sizeof(int16),1,h);
         dummy=date[dt]->date.size();
         numbytes=fwrite(&dummy,sizeof(int32),1,h);
         for(int32 dt2=0;dt2<date[dt]->date.size();dt2++)
         {
            numbytes=fwrite(date[dt]->date[dt2],sizeof(creaturest),1,h);
         }
      }

      //NEWS STORIES
      dummy=newsstory.size();
      numbytes=fwrite(&dummy,sizeof(int32),1,h);
      for(int32 ns=0;ns<newsstory.size();ns++)
      {
         numbytes=fwrite(&newsstory[ns]->type,sizeof(int16),1,h);
         numbytes=fwrite(&newsstory[ns]->view,sizeof(int16),1,h);

         numbytes=fwrite(&newsstory[ns]->loc,sizeof(int32),1,h);
         numbytes=fwrite(&newsstory[ns]->priority,sizeof(int32),1,h);
         numbytes=fwrite(&newsstory[ns]->page,sizeof(int32),1,h);
         numbytes=fwrite(&newsstory[ns]->positive,sizeof(char),1,h);
         numbytes=fwrite(&newsstory[ns]->siegetype,sizeof(int16),1,h);

         if(newsstory[ns]->cr==NULL)dummy_c=0;
         else dummy_c=1;
         numbytes=fwrite(&dummy_c,sizeof(char),1,h);
         if(newsstory[ns]->cr!=NULL)
         {
            numbytes=fwrite(&newsstory[ns]->cr->id,sizeof(int32),1,h);
         }

         dummy=newsstory[ns]->crime.size();
         numbytes=fwrite(&dummy,sizeof(int32),1,h);
         for(int32 dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
         {
            numbytes=fwrite(&newsstory[ns]->crime[dt2],sizeof(int32),1,h);
         }
      }

      // Liberal Media
      numbytes=fwrite(newspaper_topicwork1,sizeof(newspaper_topicwork1),1,h);
      numbytes=fwrite(newspaper_topicwork2,sizeof(newspaper_topicwork2),1,h);

      LCSCloseFile(h);
   }
}


/* loads the game from save.dat */
char load(void)
{
   //LOAD FILE
   uint32 loadversion;
      int32 l;
   char dummy_c;
   int32 dummy;
   int32 dummy_l;
   uint32 numbytes;
   FILE *h;

   h=LCSOpenFile("save.dat", "rb", LCSIO_PRE_HOME);

   if(h!=NULL)
   {
      fread(&loadversion,sizeof(uint32),1,h);

      //NUKE INVALID SAVE GAMES
      if(loadversion<lowestloadversion)
      {
         LCSCloseFile(h);

         reset();

         return 0;
      }

      fread(&seed,sizeof(uint32),1,h);

      fread(&mode,sizeof(int32),1,h);

      fread(&day,sizeof(int32),1,h);
      fread(&month,sizeof(int32),1,h);
      fread(&year,sizeof(int32),1,h);
      fread(&execterm,sizeof(int16),1,h);
      fread(&amendnum,sizeof(int32),1,h);

      fread(&stat_recruits,sizeof(uint32),1,h);
      fread(&stat_dead,sizeof(uint32),1,h);
      fread(&stat_kills,sizeof(uint32),1,h);
      fread(&stat_kidnappings,sizeof(uint32),1,h);
      fread(&stat_funds,sizeof(uint32),1,h);
      fread(&stat_spent,sizeof(uint32),1,h);
      fread(&stat_buys,sizeof(uint32),1,h);
      fread(&stat_burns,sizeof(uint32),1,h);

      fread(&curcarid,sizeof(int32),1,h);
      fread(&showcarprefs,sizeof(char),1,h);
      fread(&curcreatureid,sizeof(int32),1,h);
      fread(&cursquadid,sizeof(int32),1,h);
      fread(&offended_cops,sizeof(int16),1,h);
      fread(&police_heat,sizeof(int32),1,h);
      fread(&offended_corps,sizeof(int16),1,h);
      fread(&offended_cia,sizeof(int16),1,h);
      fread(&offended_amradio,sizeof(int16),1,h);
      fread(&offended_cablenews,sizeof(int16),1,h);
      fread(&attorneyseed,sizeof(uint32),1,h);
      fread(&selectedsiege,sizeof(int32),1,h);
      fread(lcityname,sizeof(char),80,h);
      fread(&newscherrybusted,sizeof(char),1,h);
      fread(&moneygained_donate,sizeof(int32),1,h);
      fread(&moneygained_brownies,sizeof(int32),1,h);
      fread(&moneygained_goods,sizeof(int32),1,h);
      fread(&moneygained_ccfraud,sizeof(int32),1,h);
      fread(&moneygained_hustling,sizeof(int32),1,h);
      fread(&moneygained_thievery,sizeof(int32),1,h);
      fread(&moneylost_goods,sizeof(int32),1,h);
      fread(&moneylost_trouble,sizeof(int32),1,h);
      fread(&moneylost_rent,sizeof(int32),1,h);
      fread(&moneylost_manufacture,sizeof(int32),1,h);
      fread(&moneylost_legal,sizeof(int32),1,h);
      fread(&moneylost_compound,sizeof(int32),1,h);
      fread(&moneylost_hostage,sizeof(int32),1,h);

      fread(slogan,sizeof(char),80,h);
      fread(&funds,sizeof(uint32),1,h);
      fread(&party_status,sizeof(int16),1,h);

      fread(attitude,sizeof(int16)*VIEWNUM,1,h);
      fread(law,sizeof(int16)*LAWNUM,1,h);
      fread(house,sizeof(int16),435,h);
      fread(senate,sizeof(int16),100,h);
      fread(court,sizeof(int16),9,h);
      fread(courtname,sizeof(char)*80,9,h);
      fread(exec,sizeof(int16)*EXECNUM,1,h);
      fread(execname,sizeof(char)*80,EXECNUM,h);

      //LOCATIONS
      fread(&dummy,sizeof(int32),1,h);
      location.resize(dummy);
      for(l=0;l<location.size();l++)
      {
         location[l]=new locationst;

         fread(&dummy,sizeof(int32),1,h);
         location[l]->loot.resize(dummy);
         for(int32 l2=0;l2<location[l]->loot.size();l2++)
         {
            location[l]->loot[l2]=new itemst;
            fread(location[l]->loot[l2],sizeof(itemst),1,h);
         }

         fread(location[l]->name,sizeof(char),40,h);
         fread(location[l]->shortname,sizeof(char),20,h);
         fread(&location[l]->type,sizeof(int16),1,h);
         fread(&location[l]->parent,sizeof(int32),1,h);
         fread(&location[l]->renting,sizeof(int32),1,h);
         fread(&location[l]->newrental,sizeof(char),1,h);
         fread(&location[l]->needcar,sizeof(char),1,h);
         fread(&location[l]->closed,sizeof(int16),1,h);
         fread(&location[l]->highsecurity,sizeof(char),1,h);
         fread(&location[l]->siege,sizeof(siegest),1,h);
         fread(&location[l]->heat,sizeof(int32),1,h);
         fread(&location[l]->compound_walls,sizeof(char),1,h);
         fread(&location[l]->compound_stores,sizeof(int32),1,h);
         fread(&location[l]->front_business,sizeof(int16),1,h);
         fread(location[l]->front_name,sizeof(char),40,h);
         fread(&location[l]->haveflag,sizeof(char),1,h);
         fread(&location[l]->mapseed,sizeof(uint32),1,h);
      }

      //VEHICLES
      fread(&dummy,sizeof(int32),1,h);
      vehicle.resize(dummy);
      for(l=0;l<vehicle.size();l++)
      {
         vehicle[l]=new vehiclest;
         fread(vehicle[l],sizeof(vehiclest),1,h);
      }

      //POOL
      fread(&dummy,sizeof(int32),1,h);
      pool.resize(dummy);
      for(int32 pl=0;pl<pool.size();pl++)
      {
         pool[pl]=new creaturest;
         fread(pool[pl],sizeof(creaturest),1,h);
      }

      //SQUADS
      fread(&dummy,sizeof(int32),1,h);
      squad.resize(dummy);
      for(int32 sq=0;sq<squad.size();sq++)
      {
         squad[sq]=new squadst;

         fread(squad[sq]->name,sizeof(char),40,h);
         fread(&squad[sq]->activity,sizeof(activityst),1,h);
         fread(&squad[sq]->id,sizeof(int32),1,h);

         for(int32 pos=0;pos<6;pos++)
         {
            fread(&dummy_c,sizeof(char),1,h);

            //REBUILD SQUAD FROM POOL
            squad[sq]->squad[pos]=NULL;
            if(dummy_c)
            {
               fread(&dummy_l,sizeof(int32),1,h);
               for(int32 pl=0;pl<pool.size();pl++)
               {
                  if(pool[pl]->id==dummy_l)
                  {
                     squad[sq]->squad[pos]=pool[pl];
                  }
               }
            }
         }

         fread(&dummy,sizeof(int32),1,h);
         squad[sq]->loot.resize(dummy);
         for(int32 l2=0;l2<squad[sq]->loot.size();l2++)
         {
            squad[sq]->loot[l2]=new itemst;
            fread(squad[sq]->loot[l2],sizeof(itemst),1,h);
         }
      }

      activesquad=NULL;
      fread(&dummy_c,sizeof(char),1,h);
      if(dummy_c)
      {
         fread(&dummy_l,sizeof(int32),1,h);
         for(int32 sq=0;sq<squad.size();sq++)
         {
            if(squad[sq]->id==dummy_l)
            {
               activesquad=squad[sq];
               break;
            }
         }
      }

      //DATES
      fread(&dummy,sizeof(int32),1,h);
      date.resize(dummy);
      for(int32 dt=0;dt<date.size();dt++)
      {
         date[dt]=new datest;

         fread(&date[dt]->mac_id,sizeof(int32),1,h);
         fread(&date[dt]->timeleft,sizeof(int16),1,h);

         fread(&dummy,sizeof(int32),1,h);
         date[dt]->date.resize(dummy);
         for(int32 dt2=0;dt2<date[dt]->date.size();dt2++)
         {
            date[dt]->date[dt2]=new creaturest;
            fread(date[dt]->date[dt2],sizeof(creaturest),1,h);
         }
      }

      //NEWS STORIES
      fread(&dummy,sizeof(int32),1,h);
      newsstory.resize(dummy);
      for(int32 ns=0;ns<newsstory.size();ns++)
      {
         newsstory[ns]=new newsstoryst;

         fread(&newsstory[ns]->type,sizeof(int16),1,h);
         fread(&newsstory[ns]->view,sizeof(int16),1,h);

         fread(&newsstory[ns]->loc,sizeof(int32),1,h);
         fread(&newsstory[ns]->priority,sizeof(int32),1,h);
         fread(&newsstory[ns]->page,sizeof(int32),1,h);
         fread(&newsstory[ns]->positive,sizeof(char),1,h);
         fread(&newsstory[ns]->siegetype,sizeof(int16),1,h);

         fread(&dummy_c,sizeof(char),1,h);
         if(dummy_c)
         {
            fwrite(&dummy_l,sizeof(int32),1,h);
            for(int32 pl=0;pl<pool.size();pl++)
            {
               if(pool[pl]->id==dummy_l)
               {
                  newsstory[ns]->cr=pool[pl];
                  break;
               }
            }
         }

         fread(&dummy,sizeof(int32),1,h);
         newsstory[ns]->crime.resize(dummy);
         for(int32 dt2=0;dt2<newsstory[ns]->crime.size();dt2++)
         {
            fread(&newsstory[ns]->crime[dt2],sizeof(int32),1,h);
         }
      }

      // Liberal Media
      fread(newspaper_topicwork1,sizeof(newspaper_topicwork1),1,h);
      fread(newspaper_topicwork2,sizeof(newspaper_topicwork2),1,h);
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

