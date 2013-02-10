
//#include "includes.h"
#include "externs.h"

//FIXME: These ads can occur more than once on the same newspaper.
void displaysinglead(bool liberalguardian,char addplace[2][3],short* storyx_s,short* storyx_e,int& it2)
{
   int x,y;
   do
   {
      x=LCSrandom(2);y=LCSrandom(3);
   }while(addplace[x][y]);
   addplace[x][y]=1;

   int sx=0,ex=0,sy=0,ey=0;
   if(x==0)
   {
      sx=0;
      ex=23+LCSrandom(4);
   }
   if(x==1)
   {
      sx=57-LCSrandom(4);
      ex=79;
   }
   if(y==0)
   {
      sy=2;
      ey=9;
   }
   if(y==1)
   {
      sy=10;
      ey=17;
   }
   if(y==2)
   {
      sy=18;
      ey=24;
   }

   int ch='?';
   switch(LCSrandom(6))
   {
      case 0:ch=CH_LIGHT_SHADE;break;
      case 1:ch=CH_MEDIUM_SHADE;break;
      case 2:ch=CH_DARK_SHADE;break;
      case 3:ch=CH_FULL_BLOCK;break;
      case 4:ch=CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL;break;
      case 5:ch='*';break;
   }

   for(y=sy;y<=ey;y++)
   {
      if(storyx_s[y]<ex&&ex<43)storyx_s[y]=ex+2;
      if(storyx_e[y]>sx&&sx>43)storyx_e[y]=sx-2;

      if(ey==9&&y==9)continue;
      if(ey==17&&y==17)continue;
      for(x=sx;x<=ex;x++)
      {
         if(y==sy||y==8||y==16||y==24||x==sx||x==ex)
         {
            move(y,x);
            addch(ch);
         }
      }
   }

   //AD CONTENT
   {
      short storyx_s[25];
      short storyx_e[25];
      for(it2=0;it2<25;it2++)storyx_s[it2]=40;
      for(it2=0;it2<25;it2++)storyx_e[it2]=40;
      for(it2=sy+1;it2<=ey-1;it2++)storyx_s[it2]=sx+1;
      for(it2=sy+1;it2<=ey-1;it2++)storyx_e[it2]=ex-1;

      char ad[500];
      if(!liberalguardian)
      {
         switch(LCSrandom(6))
         {                // location[]->name?
            case 0:
               strcpy(ad,"&cNo Fee&r");
               strcat(ad,"&cConsignment Program&r&r");
               strcat(ad,"&cCall for Details&r");
               break;
            case 1:
              {
                int chairprice = LCSrandom(201)+400;
                strcpy(ad,"&cFine Leather Chairs&r&r");
                strcat(ad,"&cSpecial Purchase&r");
                strcat(ad,"&cNow $");
                char chairprice_s [10];
                itoa (chairprice, chairprice_s, 10);
                strcat(ad,chairprice_s);
                strcat(ad,"&r");
                break;
              }
            case 2:
               strcpy(ad,"&cParis Flea Market&r&r");
               strcat(ad,"&cSale&r");
               strcat(ad,"&c50% Off&r");
               break;
            case 3:
              {
               int caryear = year-LCSrandom(15);
               int carprice = LCSrandom(16)+15;
               int carprice2 = LCSrandom(1000);
               strcpy(ad,"&cQuality Pre-Owned&r");
               strcat(ad,"&cVehicles&r");
               strcat(ad,"&c");
               char caryear_s [80];
               itoa (caryear, caryear_s, 10);
               strcat(ad,caryear_s);
               strcat(ad," ");
//               strcat(ad,cartype);
//TODO: leading zeros, etc.
               strcat(ad,"Lexus GS 300&r");
               strcat(ad,"&cSedan 4D&r");
               strcat(ad,"&cOnly $");
               char carprice_s[80];
               itoa (carprice, carprice_s, 10);
               strcat(ad,carprice_s);
               strcat(ad,"&r");
               break;
              }
            case 4:
               strcpy(ad,"&cSpa&r");
               strcat(ad,"&cHealth, Beauty&r");
               strcat(ad,"&cand Fitness&r&r");
               strcat(ad,"&c7 Days a Week&r");
               break;
            case 5:
            {
               strcpy(ad,"&c");
               switch(LCSrandom(5))
               {
                  case 0:strcat(ad,"Searching For Love");break;
                  case 1:strcat(ad,"Seeking Love");break;
                  case 2:strcat(ad,"Are You Lonely?");break;
                  case 3:strcat(ad,"Looking For Love");break;
                  case 4:strcat(ad,"Soulmate Wanted");break;
               }
               char str[10];

               strcat(ad,"&r&r");
               strcat(ad,"&c");
               sexdesc(str);
               strcat(ad,str);
               strcat(ad," ");
               sexwho(str);
               strcat(ad,str);
               strcat(ad," ");
               sexseek(str);
               strcat(ad,str);
               strcat(ad,"&r");
               strcat(ad,"&c");
               sextype(str);
               strcat(ad,str);
               strcat(ad," w/ ");
               sexwho(str);
               strcat(ad,str);
               strcat(ad,"&r");
               break;
            }
         }
      }
      else
      {
         switch(LCSrandom(4)) // Liberal Guardian Ads
         {
         case 0:
            strcpy(ad,"&cWant Organic?&r&r");
            strcat(ad,"&cVisit The Vegan&r");
            strcat(ad,"&cCo-Op&r");
            break;
         case 1:
           {
             int numyears = LCSrandom(11)+20;
             char numyears_s [10];
             itoa (numyears, numyears_s, 10);
             strcpy(ad,"&cLiberal Defense Lawyer&r");
             strcat(ad,"&c");
             strcat(ad,numyears_s);
             strcat(ad," Years Experience&r&r");
             strcat(ad,"&cCall Today&r");
             break;
           }
         case 2:
            strcpy(ad,"&cAbortion Clinic&r&r");
            strcat(ad,"&cWalk-in, No&r");
            strcat(ad,"&cQuestions Asked&r");
            strcat(ad,"&cOpen 24/7&r");
            break;
         case 3:
            {
               strcpy(ad,"&c");
               switch(LCSrandom(4))
               {
                  case 0:strcat(ad,"Searching For Love");break;
                  case 1:strcat(ad,"Seeking Love");break;
                  case 2:strcat(ad,"Are You Lonely?");break;
                  case 3:strcat(ad,"Looking For Love");break;
               }
               char str[10];

               strcat(ad,"&r&r");
               strcat(ad,"&c");
               sexdesc(str);
               strcat(ad,str);
               strcat(ad," ");
               sexwho(str);
               strcat(ad,str);
               strcat(ad," ");
               sexseek(str);
               strcat(ad,str);
               strcat(ad,"&r");
               strcat(ad,"&c");
               sextype(str);
               strcat(ad,str);
               strcat(ad," w/ ");
               sexwho(str);
               strcat(ad,str);
               strcat(ad,"&r");
               break;
            }
         }
      }

      displaynewsstory(ad,storyx_s,storyx_e,sy+1);
   }
}


void displayads(newsstoryst& ns, bool liberalguardian, short* storyx_s, short* storyx_e,int& it2)
{
   int adnumber=0;
   if(!liberalguardian)
   {
      if(ns.page>=10)adnumber++;
      if(ns.page>=20)adnumber+=LCSrandom(2)+1;
      if(ns.page>=30)adnumber+=LCSrandom(2)+1;
      if(ns.page>=40)adnumber+=LCSrandom(2)+1;
      if(ns.page>=50)adnumber+=LCSrandom(2)+1;
   }
   else
   {
      if(ns.guardianpage>=2)adnumber++;
      if(ns.guardianpage>=3)adnumber+=LCSrandom(2)+1;
      if(ns.guardianpage>=4)adnumber+=LCSrandom(2)+1;
      if(ns.guardianpage>=5)adnumber+=LCSrandom(2)+1;
      if(ns.guardianpage>=6)adnumber+=LCSrandom(2)+1;
   }
   char addplace[2][3]={0,0,0,0,0,0};
   if(adnumber>6)adnumber=6;
   while(adnumber>0)
   {
      displaysinglead(liberalguardian,addplace,storyx_s,storyx_e,it2);
      adnumber--;
   }
}
