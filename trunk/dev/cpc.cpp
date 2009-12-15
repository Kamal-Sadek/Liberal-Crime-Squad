/**
 * Running `g++ -lncurses -o cpc -DNCURSES cpc.cpp' works fine for me -- LK
 */
#include <vector>
#include <map>
#include <string.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define CONSOLE_SUPPORT
   #ifdef CH_USE_UNICODE
     // Make sure we don't override addch for consolesupport.cpp,
     // because addch_unicode may use addch internally.
     #ifndef CONSOLE_SUPPORT
        #undef addch
        #define addch(a) addch_unicode(a)
     #endif
   #endif

using namespace std;

#define CH_USE_ASCII_HACK
#define CURSES_GRAPHICS

#include "../src/cursesgraphics.h"

 /* raw_output() is provided in PDcurses/Xcurses but is not in ncurses.
   * This function is for compatibility and is currently a do nothing function.
   */
 #ifdef NCURSES
 inline int raw_output(bool bf)
 {
    raw();
 return OK;
 }

 #endif

unsigned long picnum,dimx,dimy;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[5][80][5][4];
unsigned char newspic[20][78][18][4];

void translategetch(int &c);

int translateGraphicsChar(int c)
{
    // This will be no good for Unicode...
    int cin = c;
    int cout = cin;

//    if ((cin >= 0) && (cin <32))
//    {
//        cout = (cin | A_ALTCHARSET);
//    }
    if ((cin >= 0) && (cin < 256))
    {
        cout = gchar[cin].native_code;
        if (cout == 0)
        {
            cout = cin;
        }
    }
    return cout;
}

//sets current color to desired setting
void set_color(short f,short b,char bright,char blink=0)
{
   //color swap required for PDcurses
   if(f==7&&b==0)
   {
      f=0;b=0;
   }
   else if(f==0&&b==0)
   {
      f=7;b=0;
   }

   chtype blinky=0, brighty=0;
   
   if(blink)blinky=A_BLINK;
   if(bright)brighty=A_BOLD;

   //pick color pair based on foreground and background
   if(bright)attrset(brighty | blinky | COLOR_PAIR(f*8+b));
   else
   {
      attrset(COLOR_PAIR(f*8+b));
   }
}

//IN CASE FUNKY ARROW KEYS ARE SENT IN, TRANSLATE THEM BACK
void translategetch(int &c)
{
   //if(c==-63)c='7';
   //if(c==-62)c='8';
   //if(c==-61)c='9';
   //if(c==-60)c='4';
   //if(c==-59)c='5';
   //if(c==-58)c='6';
   //if(c==-57)c='1';
   //if(c==-56)c='2';
   //if(c==-55)c='3';

   if(c==-6)c='0';
   if(c==-50)c='.';
   if(c==-53)c=10;
   if(c==-47)c='+';
   if(c==-48)c='-';
   if(c==-49)c='*';
   if(c==-54)c='/';

   /*
   if(c==2)c='2';
   if(c==3)c='8';
   if(c==4)c='4';
   if(c==5)c='6';
   */

   //if(c>='A'&&c<='Z'){c-='A';c+='a';}

   /* Support Cursor Keys...*/
   //if(c==KEY_LEFT)c='a';
   //if(c==KEY_RIGHT)c='d';
   //if(c==KEY_UP)c='w';
   //if(c==KEY_DOWN)c='x';
}

void displaycenterednewsfont(char *str,int y)
{
   int width=-1;
   int s;
   for(s=0;s<strlen(str);s++)
   {
      if(str[s]>='A'&&str[s]<='Z')width+=6;
      else if(str[s]=='\'')width+=4;
      else width+=3;
   }

   int x=39-width/2;

   for(s=0;s<strlen(str);s++)
   {
      if((str[s]>='A'&&str[s]<='Z')||str[s]=='\'')
      {
         int p;
         if(str[s]>='A'&&str[s]<='Z')p=str[s]-'A';
         else p=26;
         int lim=6;
         if(str[s]=='\'')lim=4;
         if(s==strlen(str)-1)lim--;
         for(int x2=0;x2<lim;x2++)
         {
            for(int y2=0;y2<7;y2++)
            {
               move(y+y2,x+x2);

#ifdef NCURSES
               // Clean the square first.
               set_color(COLOR_BLACK, COLOR_BLACK, 0);
               addch(CH_FULL_BLOCK);
               move(y+y2,x+x2);
#endif

               if(x2==5)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addch(CH_FULL_BLOCK);
               }
               else
               {
                  set_color(bigletters[p][x2][y2][1],
                     bigletters[p][x2][y2][2],
                     bigletters[p][x2][y2][3]);
                  addch(translateGraphicsChar(bigletters[p][x2][y2][0]));
               }
            }
         }
         x+=lim;
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         for(int x2=0;x2<3;x2++)
         {
            for(int y2=0;y2<7;y2++)
            {
               move(y+y2,x+x2);
               addch(CH_FULL_BLOCK);
            }
         }
         x+=3;
      }
   }
}

void displaynewspicture(int p,int y)
{
   for(int x2=0;x2<78;x2++)
   {
      for(int y2=0;y2<15;y2++)
      {
         if(y+y2>24)break;
         move(y+y2,1+x2);
         set_color(newspic[p][x2][y2][1],
            newspic[p][x2][y2][2],
            newspic[p][x2][y2][3]);
         addch(translateGraphicsChar(newspic[p][x2][y2][0]));
      }
   }
}

void loadgraphics(void)
{
   int p, x, y;

   int numbytes;
   FILE *h;

   h = fopen("newspic.cpc", "rb");
   if(h!=NULL)
   {
      numbytes=fread(&picnum,sizeof(int),1,h);
      numbytes=fread(&dimx,sizeof(int),1,h);
      numbytes=fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&newspic[p][x][y][0],sizeof(char),4,h);
            }
         }
      }
   fclose(h);
   }

   h = fopen("newstops.cpc", "rb");
   if(h!=NULL)
   {
      numbytes=fread(&picnum,sizeof(int),1,h);
      numbytes=fread(&dimx,sizeof(int),1,h);
      numbytes=fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&newstops[p][x][y][0],sizeof(char),4,h);
            }
         }
      }
   fclose(h);
   }

   h = fopen("largecap.cpc", "rb");
   if(h!=NULL)
   {
      numbytes=fread(&picnum,sizeof(int),1,h);
      numbytes=fread(&dimx,sizeof(int),1,h);
      numbytes=fread(&dimy,sizeof(int),1,h);
      for(int p=0;p<picnum;p++)
      {
         for(int x=0;x<dimx;x++)
         {
            for(int y=0;y<dimy;y++)
            {
               numbytes=fread(&bigletters[p][x][y][0],sizeof(char),4,h);
            }
         }
      }
   fclose(h);
   }
}

void set_tile(unsigned char &c)
{
   clear();
   set_color(COLOR_GREEN,COLOR_BLACK,1);
   move(1,2);
   addstr("Type a letter, number, or symbol to use that tile.");
   move(2,2);
   addstr("OR, browse the special symbols below and press enter to select one.");

   for(unsigned char i=176;i<224;i++)
   {
      move(5+2*((i-176)/16),6+2*((i-176)%16));
      addch(i);
   }

   static unsigned char i=176;
   int input;
   while(1)
   {
      move(5+2*((i-176)/16),6+2*((i-176)%16));
      refresh();
      input = getch();
      translategetch(input);
      if((input<='Z' && input>='A') || (input<='z' && input>='a') || (input<='9' && input>='0'))
      {
         c=input;
         return;
      }
      else if(input == KEY_UP)
      {
         i-=16;
         if(i<176)i+=48;
      }
      else if(input == KEY_DOWN)
      {
         i+=16;
         if(i>223)i-=48;
      }
      else if(input == KEY_LEFT)
      {
         i--;
         if(i==175)i=223;
      }
      else if(input == KEY_RIGHT)
      {
         i++;
         if(i==224)i=176;
      }
      else if(input==10)
      {
         c=i;
         return;
      }
   }
}

int main(/*int nargs, char *args[]*/)
{
   initscr();
   noecho();
   //initialize curses color
   start_color();
   //initialize the array of color pairs
   for(int i=0;i<8;i++)
   {
      for(int j=0;j<8;j++)
      {
         if(i==0&&j==0)
         {
            init_pair(7*8,0,0);
            continue;
         }
         if(i==7&&j==0)continue;
         init_pair(i*8+j,i,j);
      }
   }
   //turns off cursor
   //curs_set(0);
   //begin the game loop
   keypad(stdscr,TRUE);
   raw_output(TRUE);
   loadgraphics();

   int c=0;
   int index=0; // Progresses as "0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C [seg fault]"
   int x=0;
   int y=0;
   short frontcolor=COLOR_WHITE;
   short backcolor=COLOR_BLACK;
   char brightness=0;
   unsigned char draw_character='x';
   while(1)
   {
      // Render
      clear();
      move(0,0);
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      addstr("Press X to exit, or press [ or ] to browse pictures.");
      move(1,0);
      addstr("Currently displaying Liberal picture ");
      addch('0'+index);
      displaynewspicture(index,2);
      move(16,0);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Currently drawing with: ");
      set_color(frontcolor,backcolor,brightness);
      addch(draw_character);
      move(17,0);
      addstr("To draw, press ENTER. (Not yet implemented.)");
      move(18,0);
      addstr("To change tile, press T.");
      move(19,0);
      addstr("To change foreground color, press F. (Not yet implemented.)");
      move(20,0);
      addstr("To change background color, press B. (Not yet implemented.)");
      move(2+y,1+x);
      refresh();
      
      // Read input
      c=getch();
      translategetch(c);
      // Act on input
      switch(c)
      {
      case 'x':return 0;
      case ']':index++;break;
      case '[':index--;break; // Allow dropping to index -1
      case KEY_LEFT:if(x>0)x--;break;
      case KEY_RIGHT:if(x<77)x++;break;
      case KEY_UP:if(y>0)y--;break;
      case KEY_DOWN:if(y<11)y++;break;
      case 't':set_tile(draw_character);break;
      }
      if(index==-1)
         index=20;
      if(index==21)
         index=0;
      c=0;
   }
}
