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
#include "lcsio.h"

/* displays the high score board */
void viewhighscores()
{
   int s;
   loadhighscores();

   short validsum=0;
   for(s=0;s<SCORENUM;s++)
   {
      if(score[s].valid)validsum++;
   }

   if(!validsum)return;

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("The Liberal ELITE");

   int y=2;
   for(s=0;s<SCORENUM;s++)
   {
      if(score[s].valid)
      {
         if(yourscore==s&&score[s].endtype==END_WON)set_color(COLOR_GREEN,COLOR_BLACK,1);
         else if(yourscore==s)set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(y,0);
         addstr(score[s].slogan);
         if(yourscore==s&&score[s].endtype==END_WON)set_color(COLOR_GREEN,COLOR_BLACK,0);
         else if(yourscore==s)set_color(COLOR_RED,COLOR_BLACK,0);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y+1,0);
         switch(score[s].endtype)
         {
            case END_WON:
               addstr("The Liberal Crime Squad liberalized the country in ");
               break;
            case END_POLICE:
               addstr("The Liberal Crime Squad was brought to justice in ");
               break;
            case END_CIA:
               addstr("The Liberal Crime Squad was blotted out in ");
               break;
            case END_HICKS:
               addstr("The Liberal Crime Squad was mobbed in ");
               break;
            case END_CORP:
               addstr("The Liberal Crime Squad was downsized in ");
               break;
            case END_DEAD:
               addstr("The Liberal Crime Squad was KIA in ");
               break;
            case END_REAGAN:
               addstr("The country was Reaganified in ");
               break;
            case END_PRISON:
               addstr("The Liberal Crime Squad died in prison in ");
               break;
            case END_EXECUTED:
               addstr("The Liberal Crime Squad was executed in ");
               break;
            case END_DATING:
               addstr("The Liberal Crime Squad was on vacation in ");
               break;
            case END_HIDING:
               addstr("The Liberal Crime Squad was in permanent hiding in ");
               break;
            case END_DISBANDLOSS:
               addstr("The Liberal Crime Squad was hunted down in ");
               break;
            case END_DISPERSED:
               addstr("The Liberal Crime Squad was scattered in ");
               break;
            case END_CCS:
               addstr("The Liberal Crime Squad was out-Crime Squadded in ");
               break;
            case END_FIREMEN:
               addstr("The Liberal Crime Squad was burned in ");
               break;
            case END_STALIN:
               addstr("The country was Stalinized in ");
               break;
         }
         addstr(getmonth(score[s].month));
         addstr(" ");
         addstr(score[s].year);
         addstr(". ");

         move(y+2,0);
         addstr("Recruits: ");
         addstr(score[s].stat_recruits);

         move(y+3,0);
         addstr("Martyrs: ");
         addstr(score[s].stat_dead);

         move(y+2,20);
         addstr("Kills: ");
         addstr(score[s].stat_kills);

         move(y+3,20);
         addstr("Kidnappings: ");
         addstr(score[s].stat_kidnappings);

         move(y+2,40);
         addstr("$ Taxed: ");
         addstr(score[s].stat_funds);

         move(y+3,40);
         addstr("$ Spent: ");
         addstr(score[s].stat_spent);

         move(y+2,60);
         addstr("Flags Bought: ");
         addstr(score[s].stat_buys);

         move(y+3,60);
         addstr("Flags Burned: ");
         addstr(score[s].stat_burns);

         y+=4;
      }
   }

   set_color(COLOR_GREEN,COLOR_BLACK,1);

   //UNIVERSAL STATS
   move(22,0);
   addstr("Universal Liberal Statistics:");

   move(23,0);
   addstr("Recruits: ");
   addstr(ustat_recruits);

   move(24,0);
   addstr("Martyrs: ");
   addstr(ustat_dead);

   move(23,20);
   addstr("Kills: ");
   addstr(ustat_kills);

   move(24,20);
   addstr("Kidnappings: ");
   addstr(ustat_kidnappings);

   move(23,40);
   addstr("$ Taxed: ");
   addstr(ustat_funds);

   move(24,40);
   addstr("$ Spent: ");
   addstr(ustat_spent);

   move(23,60);
   addstr("Flags Bought: ");
   addstr(ustat_buys);

   move(24,60);
   addstr("Flags Burned: ");
   addstr(ustat_burns);

   getkey();
}

/* loads the high scores file */
void loadhighscores()
{
   for(int s=0;s<SCORENUM;s++)score[s].valid=0;

   //LOAD FILE
   int loadversion;

   FILE *h=LCSOpenFile("score.dat", "rb", LCSIO_PRE_HOME);
   if(h!=NULL)
   {
      fread(&loadversion,sizeof(int),1,h);

      if(loadversion<lowestloadscoreversion)
      {
         LCSCloseFile(h);
         return;
      }

      fread(&ustat_recruits,sizeof(int),1,h);
      fread(&ustat_dead,sizeof(int),1,h);
      fread(&ustat_kills,sizeof(int),1,h);
      fread(&ustat_kidnappings,sizeof(int),1,h);
      fread(&ustat_funds,sizeof(int),1,h);
      fread(&ustat_spent,sizeof(int),1,h);
      fread(&ustat_buys,sizeof(int),1,h);
      fread(&ustat_burns,sizeof(int),1,h);
      fread(score,sizeof(highscorest),SCORENUM,h);

      LCSCloseFile(h);
   }
}

/* saves a new high score */
void savehighscore(char endtype)
{
   loadhighscores();

   //MERGE THE STATS
   ustat_recruits+=stat_recruits;
   ustat_dead+=stat_dead;
   ustat_kills+=stat_kills;
   ustat_kidnappings+=stat_kidnappings;
   ustat_funds+=ledger.total_income;
   ustat_spent+=ledger.total_expense;
   ustat_buys+=stat_buys;
   ustat_burns+=stat_burns;

   //PLACE THIS HIGH SCORE BY DATE IF NECESSARY
   yourscore=-1;

   for(int s=0;s<SCORENUM;s++)
   {
      if((endtype==END_WON&&score[s].endtype==END_WON&&
         year==score[s].year&&month==score[s].month&&
         ledger.total_expense+ledger.total_expense>score[s].stat_spent+score[s].stat_funds)||

         (endtype==END_WON&&score[s].endtype==END_WON
         &&(year<score[s].year ||
         (year==score[s].year && month<score[s].month)))||

         (endtype==END_WON&&score[s].endtype!=END_WON)||

         (endtype!=END_WON&&score[s].endtype!=END_WON&&
         ledger.total_expense+ledger.total_income>score[s].stat_spent+score[s].stat_funds)||

         score[s].valid==0)
      {
         for(int s2=SCORENUM-1;s2>=s+1;s2--)
         {
            score[s2]=score[s2-1];
         }

         strcpy(score[s].slogan,slogan);
         score[s].month=month;
         score[s].year=year;
         score[s].stat_recruits=stat_recruits;
         score[s].stat_dead=stat_dead;
         score[s].stat_kills=stat_kills;
         score[s].stat_kidnappings=stat_kidnappings;
         score[s].stat_funds=ledger.total_income;
         score[s].stat_spent=ledger.total_expense;
         score[s].stat_buys=stat_buys;
         score[s].stat_burns=stat_burns;
         score[s].valid=1;
         score[s].endtype=endtype;

         yourscore=s;
         break;
      }
   }

   FILE *h=LCSOpenFile("score.dat","wb",LCSIO_PRE_HOME);
   if(h!=NULL)
   {
      int lversion=version;
      fwrite(&lversion,sizeof(int),1,h);

      fwrite(&ustat_recruits,sizeof(int),1,h);
      fwrite(&ustat_dead,sizeof(int),1,h);
      fwrite(&ustat_kills,sizeof(int),1,h);
      fwrite(&ustat_kidnappings,sizeof(int),1,h);
      fwrite(&ustat_funds,sizeof(int),1,h);
      fwrite(&ustat_spent,sizeof(int),1,h);
      fwrite(&ustat_buys,sizeof(int),1,h);
      fwrite(&ustat_burns,sizeof(int),1,h);
      fwrite(score,sizeof(highscorest),SCORENUM,h);

      LCSCloseFile(h);
   }
}

