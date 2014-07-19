
//#include "includes.h"
#include "externs.h"

void preparepage(newsstoryst& ns, bool liberalguardian)
{
   set_color(COLOR_WHITE,COLOR_WHITE,0);
   for(int x=0;x<80;x++)
      for(int y=0;y<25;y++)
         mvaddchar(y,x,' ');
   set_color(COLOR_WHITE,COLOR_BLACK,0);

   if(ns.page==1||(liberalguardian&&ns.guardianpage==1))
   {
      //TOP
      int pap=LCSrandom(5);
      for(int x=0;x<80;x++)
      {
         for(int y=0;y<5;y++)
         {
            move(y,x);
            if(liberalguardian)
            {
               set_color(newstops[5][x][y][1],
                         newstops[5][x][y][2],
                         newstops[5][x][y][3]);
               addch(translateGraphicsChar(newstops[5][x][y][0]));
            }
            else
            {
               set_color(newstops[pap][x][y][1],
                         newstops[pap][x][y][2],
                         newstops[pap][x][y][3]);

               addch(translateGraphicsChar(newstops[pap][x][y][0]));
            }
         }
      }

      if(!liberalguardian) // Liberal Guardian graphics don't support adding a date
      {
         //DATE
         set_color(COLOR_BLACK,COLOR_WHITE,0);
         int pday=day, pmonth=month, pyear=year;
         if(pday>monthday()) // Day counter has increased but end-of-month has not yet
         {                   // been checked so it has to be accounted for here.
            pday=1,pmonth++;
            if(pmonth>12) pmonth=1,pyear++;
         }
         move(3,66+(pday<10));
         addstr(getmonth(pmonth,true));
         addstr(" ");
         addstr(pday);
         addstr(", ");
         addstr(pyear);
      }
   }
   else
   {
      //PAGE
      set_color(COLOR_BLACK,COLOR_WHITE,0);
      move(0,76);
      if(!liberalguardian) addstr(ns.page);
      else addstr(ns.guardianpage);
   }
}
