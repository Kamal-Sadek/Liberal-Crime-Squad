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

/* displays the high score board */
void viewhighscores(void)
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

   char num[20];

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
         }
         switch(score[s].month)
         {
            case 1:addstr("January");break;
            case 2:addstr("February");break;
            case 3:addstr("March");break;
            case 4:addstr("April");break;
            case 5:addstr("May");break;
            case 6:addstr("June");break;
            case 7:addstr("July");break;
            case 8:addstr("August");break;
            case 9:addstr("September");break;
            case 10:addstr("October");break;
            case 11:addstr("November");break;
            case 12:addstr("December");break;
         }
         addstr(" ");
         itoa(score[s].year,num,10);
         addstr(num);
         addstr(". ");

         move(y+2,0);
         addstr("Recruits: ");
         itoa(score[s].stat_recruits,num,10);
         addstr(num);

         move(y+3,0);
         addstr("Martyrs: ");
         itoa(score[s].stat_dead,num,10);
         addstr(num);

         move(y+2,20);
         addstr("Kills: ");
         itoa(score[s].stat_kills,num,10);
         addstr(num);

         move(y+3,20);
         addstr("Kidnappings: ");
         itoa(score[s].stat_kidnappings,num,10);
         addstr(num);

         move(y+2,40);
         addstr("$ Taxed: ");
         itoa(score[s].stat_funds,num,10);
         addstr(num);

         move(y+3,40);
         addstr("$ Spent: ");
         itoa(score[s].stat_spent,num,10);
         addstr(num);

         move(y+2,60);
         addstr("Flags Bought: ");
         itoa(score[s].stat_buys,num,10);
         addstr(num);

         move(y+3,60);
         addstr("Flags Burned: ");
         itoa(score[s].stat_burns,num,10);
         addstr(num);

         y+=4;
      }
   }

   set_color(COLOR_GREEN,COLOR_BLACK,1);

   //UNIVERSAL STATS
   move(22,0);
   addstr("Universal Liberal Statistics:");

   move(23,0);
   addstr("Recruits: ");
   itoa(ustat_recruits,num,10);
   addstr(num);

   move(24,0);
   addstr("Martyrs: ");
   itoa(ustat_dead,num,10);
   addstr(num);

   move(23,20);
   addstr("Kills: ");
   itoa(ustat_kills,num,10);
   addstr(num);

   move(24,20);
   addstr("Kidnappings: ");
   itoa(ustat_kidnappings,num,10);
   addstr(num);

   move(23,40);
   addstr("$ Taxed: ");
   itoa(ustat_funds,num,10);
   addstr(num);

   move(24,40);
   addstr("$ Spent: ");
   itoa(ustat_spent,num,10);
   addstr(num);

   move(23,60);
   addstr("Flags Bought: ");
   itoa(ustat_buys,num,10);
   addstr(num);

   move(24,60);
   addstr("Flags Burned: ");
   itoa(ustat_burns,num,10);
   addstr(num);

   refresh();
   getch();
}

/* loads the high scores file */
void loadhighscores(void)
{
   for(unsigned int s=0;s<SCORENUM;s++)score[s].valid=0;

   //LOAD FILE
   unsigned long loadversion;

   unsigned int numbytes;
   FILE *h;

   h=LCSOpenFile("score.dat", "rb", LCSIO_PRE_HOME);

   if(h!=NULL)
   {
      numbytes=fread(&loadversion,sizeof(unsigned long),1,h);

      if(loadversion<lowestloadscoreversion)
      {
         LCSCloseFile(h);
         return;
      }

      numbytes=fread(&ustat_recruits,sizeof(unsigned long),1,h);
      numbytes=fread(&ustat_dead,sizeof(unsigned long),1,h);
      numbytes=fread(&ustat_kills,sizeof(unsigned long),1,h);
      numbytes=fread(&ustat_kidnappings,sizeof(unsigned long),1,h);
      numbytes=fread(&ustat_funds,sizeof(unsigned long),1,h);
      numbytes=fread(&ustat_spent,sizeof(unsigned long),1,h);
      numbytes=fread(&ustat_buys,sizeof(unsigned long),1,h);
      numbytes=fread(&ustat_burns,sizeof(unsigned long),1,h);
      numbytes=fread(score,sizeof(highscorest),SCORENUM,h);

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
   ustat_funds+=stat_funds;
   ustat_spent+=stat_spent;
   ustat_buys+=stat_buys;
   ustat_burns+=stat_burns;

   //PLACE THIS HIGH SCORE BY DATE IF NECESSARY
   yourscore=-1;

   for(unsigned int s=0;s<SCORENUM;s++)
   {
      if((endtype==END_WON&&score[s].endtype==END_WON&&
         year==score[s].year&&month==score[s].month&&
         stat_spent+stat_funds>score[s].stat_spent+score[s].stat_funds)||

         (endtype==END_WON&&score[s].endtype==END_WON
         &&(year<score[s].year ||
         (year==score[s].year && month<score[s].month)))||

         (endtype==END_WON&&score[s].endtype!=END_WON)||

         (endtype!=END_WON&&score[s].endtype!=END_WON&&
         stat_spent+stat_funds>score[s].stat_spent+score[s].stat_funds)||

         score[s].valid==0)
      {
         for(unsigned int s2=SCORENUM-1;s2>=s+1;s2--)
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
         score[s].stat_funds=stat_funds;
         score[s].stat_spent=stat_spent;
         score[s].stat_buys=stat_buys;
         score[s].stat_burns=stat_burns;
         score[s].valid=1;
         score[s].endtype=endtype;

         yourscore=s;
         break;
      }
   }


   unsigned int numbytes;
   FILE *h;
   h=LCSOpenFile("score.dat","wb",LCSIO_PRE_HOME);
   if(h!=NULL)
   {
      unsigned int lversion=version;
      numbytes=fwrite(&lversion,sizeof(unsigned long),1,h);

      numbytes=fwrite(&ustat_recruits,sizeof(unsigned long),1,h);
      numbytes=fwrite(&ustat_dead,sizeof(unsigned long),1,h);
      numbytes=fwrite(&ustat_kills,sizeof(unsigned long),1,h);
      numbytes=fwrite(&ustat_kidnappings,sizeof(unsigned long),1,h);
      numbytes=fwrite(&ustat_funds,sizeof(unsigned long),1,h);
      numbytes=fwrite(&ustat_spent,sizeof(unsigned long),1,h);
      numbytes=fwrite(&ustat_buys,sizeof(unsigned long),1,h);
      numbytes=fwrite(&ustat_burns,sizeof(unsigned long),1,h);
      numbytes=fwrite(score,sizeof(highscorest),SCORENUM,h);

      LCSCloseFile(h);
   }
}

