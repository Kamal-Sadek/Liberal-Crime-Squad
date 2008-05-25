
#include "includes.h"
#include "externs.h"

void preparepage(newsstoryst& ns, bool liberalguardian)
{
   for(int x=0;x<80;x++)
   {
      for(int y=0;y<25;y++)
      {
         move(y,x);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addch(CH_FULL_BLOCK);
      }
   }

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
         char num[20];
         if(day>=10)move(3,66);
         else move(3,67);
         switch(month)
         {
            case 1:addstr("Jan");break;
            case 2:addstr("Feb");break;
            case 3:addstr("Mar");break;
            case 4:addstr("Apr");break;
            case 5:addstr("May");break;
            case 6:addstr("Jun");break;
            case 7:addstr("Jul");break;
            case 8:addstr("Aug");break;
            case 9:addstr("Sep");break;
            case 10:addstr("Oct");break;
            case 11:addstr("Nov");break;
            case 12:addstr("Dec");break;
         }
         addstr(". ");
         itoa(day,num,10);
         addstr(num);
         addstr(", ");
         itoa(year,num,10);
         addstr(num);
      }
   }
   else
   {
      //PAGE
      set_color(COLOR_BLACK,COLOR_WHITE,0);
      char num[20];
      if(!liberalguardian)itoa(ns.page,num,10);
      else itoa(ns.guardianpage,num,10);
      move(0,76);
      addstr(num);
   }
}