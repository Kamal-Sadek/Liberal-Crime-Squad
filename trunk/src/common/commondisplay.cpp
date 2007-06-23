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

/* location and squad header */
void locheader(void)
{
   char num[20];

   if(activesquad!=NULL)
   {
      if(location[activesquad->squad[0]->location]->siege.siege)
      {
         if(location[activesquad->squad[0]->location]->siege.underattack)set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_YELLOW,COLOR_BLACK,1);
      }
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
   else if(selectedsiege!=-1)
   {
      if(location[selectedsiege]->siege.siege)
      {
         if(location[selectedsiege]->siege.underattack)set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_YELLOW,COLOR_BLACK,1);
      }
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
   else set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(0,0);

   if(activesquad!=NULL)
   {
      addlocationname(location[activesquad->squad[0]->location]);
      addstr(", ");
   }
   else
   {
      if(selectedsiege==-1)
      {
         addstr("No Squad Selected");
         addstr(", ");
      }
      else
      {
         addlocationname(location[selectedsiege]);
         addstr(", ");
      }
   }

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

   set_color(COLOR_GREEN,COLOR_BLACK,1);
   itoa(funds,num,10);
   move(0,78-strlen(num));
   addstr("$");
   addstr(num);

   if(activesquad!=NULL)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      char str[80];
      getactivity(str,activesquad->activity);

      if(activesquad->activity.type==ACTIVITY_NONE)
      {
         int count=0;char haveact=0;
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;
            count++;
            if(activesquad->squad[p]->activity.type!=ACTIVITY_NONE)
            {
               getactivity(str,activesquad->squad[p]->activity);
               haveact=1;
            }
         }
         if(haveact&&count>1)strcpy(str,"Acting Individually");
      }

      move(0,46);
      addstr(str);
   }
}

/* party info at top of screen */
void printparty(void)
{
   creaturest *party[6]={NULL,NULL,NULL,NULL,NULL,NULL};
   if(activesquad!=NULL)
   {
      for(int p=0;p<6;p++)party[p]=activesquad->squad[p];
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   for(int i=2;i<8;i++)
   {
      move(i,0);
      addstr("                                                                                ");
   }

   if(party_status!=-1&&party[party_status]==NULL)party_status=-1;

   if(party_status!=-1)
   {
      printcreatureinfo(party[party_status]);

      char num[20];
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      itoa(party_status+1,num,10);
      move(1,0);
      addstr(num);
   }
   else
   {
      char str[200];
      char num[20];

      move(1,0);
      addstr("#-CODE NAME------------SKILL---WEAPON---------ARMOR----------HEALTH---TRANSPORT-");

      for(int p=0;p<6;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(p+2,0);
         addstr("                                                                                          ");
         move(p+2,0);
         addch('1'+p);

         if(party[p]!=NULL)
         {
            if(party[p]->prisoner!=NULL)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p+2,2);
            addstr(party[p]->name);
            if(party[p]->prisoner!=NULL)addstr("+H");

            unsigned long skill=0;
            char bright=0;
            for(int sk=0;sk<SKILLNUM;sk++)
            {
               skill+=(unsigned long)party[p]->skill[sk];
               if(party[p]->skill_ip[sk]>=100*((10+party[p]->skill[sk])/10)&&
                  party[p]->skill[sk]<party[p]->attval(skillatt(sk))*2)bright=1;
            }

            set_color(COLOR_WHITE,COLOR_BLACK,bright);
            move(p+2,23);
            itoa(skill,num,10);
            addstr(num);
            addstr("/");
            int wsk=SKILL_HANDTOHAND;
            int wsk2=0;
            switch(party[p]->weapon.type)
            {
               case WEAPON_SHANK:
               case WEAPON_KNIFE:
               case WEAPON_SYRINGE:
                  wsk=SKILL_KNIFE;
                  break;
               case WEAPON_CROWBAR:
               case WEAPON_BASEBALLBAT:
               case WEAPON_CHAIN:
               case WEAPON_GAVEL:
               case WEAPON_NIGHTSTICK:
               case WEAPON_HAMMER:
               case WEAPON_MAUL:
               case WEAPON_CROSS:
               case WEAPON_STAFF:
               case WEAPON_TORCH:
                  wsk=SKILL_CLUB;
                  break;
               case WEAPON_SWORD:
                  wsk=SKILL_SWORD;
                  break;
               case WEAPON_PITCHFORK:
                  wsk=SKILL_SPEAR;
                  break;
               case WEAPON_AXE:
                  wsk=SKILL_AXE;
                  break;
               case WEAPON_REVOLVER_22:
               case WEAPON_REVOLVER_44:
               case WEAPON_SEMIPISTOL_9MM:
               case WEAPON_SEMIPISTOL_45:
                  wsk=SKILL_PISTOL;
                  break;
               case WEAPON_SHOTGUN_PUMP:
                  wsk=SKILL_RIFLE;
                  break;
               case WEAPON_AUTORIFLE_M16:
               case WEAPON_AUTORIFLE_AK47:
               case WEAPON_CARBINE_M4:
                  wsk=SKILL_ASSAULTRIFLE;
                  break;
               case WEAPON_SEMIRIFLE_AR15:
                  wsk=SKILL_ASSAULTRIFLE;
                  wsk2=SKILL_RIFLE;
                  break;
               case WEAPON_SMG_MP5:
                  wsk=SKILL_ASSAULTRIFLE;
                  wsk2=SKILL_PISTOL;
                  break;
            }
            if(wsk2)itoa((party[p]->skill[wsk]+party[p]->skill[wsk2])/2,num,10);
            else itoa(party[p]->skill[wsk],num,10);
            addstr(num);

            move(p+2,31);
            if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else
               switch(weaponcheck(*party[p],sitetype))
               {
               case 0:set_color(COLOR_GREEN,COLOR_BLACK,1);break;
               case 1:set_color(COLOR_YELLOW,COLOR_BLACK,1);break;
               case 2:set_color(COLOR_RED,COLOR_BLACK,1);break;
               }
            getweapon(str,party[p]->weapon.type);
            addstr(str);
            //set_color(COLOR_WHITE,COLOR_BLACK,0);
            if(party[p]->weapon.ammo>0)
            {
               //set_color(COLOR_WHITE,COLOR_BLACK,0);
               char num[20];
               itoa(party[p]->weapon.ammo,num,10);
               addstr(" (");
               addstr(num);
               addstr(")");
            }
            else if(ammotype(party[p]->weapon.type)!=-1)
            {
               if(party[p]->clip[ammotype(party[p]->weapon.type)]>0)
               {
                  char num[20];
                  itoa(party[p]->clip[ammotype(party[p]->weapon.type)],num,10);
                  addstr(" (");
                  addstr(num);
                  addstr(")");
               }
               else
               {
                  set_color(COLOR_BLACK,COLOR_BLACK,1);
                  addstr(" (XX)");
               }
            }

            if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else if(hasdisguise(*party[p],sitetype))
            {
               if(party[p]->armor.quality!='1'||
                  party[p]->armor.flag!=0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
               else set_color(COLOR_GREEN,COLOR_BLACK,1);
            }
            else set_color(COLOR_RED,COLOR_BLACK,1);
            move(p+2,46);
            getarmor(str,party[p]->armor.type,party[p]->armor.subtype);
            addstr(str);

            printhealthstat(*party[p],p+2,61,TRUE);

            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(p+2,70);

            long v=-1;
            if(showcarprefs==1)v=id_getcar(party[p]->pref_carid);
            else v=id_getcar(party[p]->carid);
            if(v!=-1&&showcarprefs!=-1)
            {
               getcar(str,vehicle[v]->type);
               char d;
               if(showcarprefs==1)d=party[p]->pref_is_driver;
               else d=party[p]->is_driver;
               if(d)strcat(str,"-D");
            }
            else
            {
               int legok=2;
               if((party[p]->wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
                   (party[p]->wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
               if((party[p]->wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
                   (party[p]->wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
               if(party[p]->flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
               else if(legok>=1)strcpy(str,"On Foot");
               else strcpy(str,"On \"Foot\"");
            }
            addstr(str);
         }
      }
   }

   makedelimiter(8,0);
}


/* location info at top of screen */
void printlocation(long loc)
{
   if(location[loc]->siege.siege)
   {
      if(!location[loc]->siege.underattack)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(2,1);
         addstr("The police have surrounded this location.");
      }
      else
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         move(2,1);
         switch(location[loc]->siege.siegetype)
         {
            case SIEGE_POLICE:
               addstr("The police are raiding this location!");break;
            case SIEGE_CIA:
               addstr("The CIA is raiding this location!");break;
            case SIEGE_HICKS:
               addstr("The masses are storming this location!");break;
            case SIEGE_CORPORATE:
               addstr("The Corporations are raiding this location!");break;
         }
      }
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,1);
      addstr("You are not under siege...  yet.");
   }

   if(location[loc]->type==SITE_INDUSTRY_WAREHOUSE)
   {
      if(numbereating(loc)>0&&location[loc]->compound_walls!=COMPOUND_PRINTINGPRESS)
      {
         if(fooddaysleft(loc))
         {
            if(fooddaysleft(loc)<4)
            {
               if(!location[loc]->siege.siege)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(3,1);
               addstr("This location has food for only a few days.");
            }
         }
         else
         {
            if(!location[loc]->siege.siege)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_RED,COLOR_BLACK,1);
            move(3,1);
            addstr("This location has insufficient food stores.");
         }
      }

      if(location[loc]->compound_walls & COMPOUND_BASIC)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(4,1);
         addstr("FORTIFIED COMPOUND");
      }

      if(location[loc]->compound_walls & COMPOUND_PRINTINGPRESS)
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(6,31);
         addstr("LIBERAL GUARDIAN HQ");
      }

      if(location[loc]->front_business!=-1)
      {
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         move(4,54);
         addstr("BUSINESS FRONT");
      }

      if(location[loc]->compound_walls & COMPOUND_CAMERAS)
      {
         if(location[loc]->siege.siege&&location[loc]->siege.cameras_off)
         {
            set_color(COLOR_RED,COLOR_BLACK,0);
            move(5,1);
            addstr("CAMERAS OFF");
         }
         else
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(5,1);
            addstr("CAMERAS ON");
         }
      }

      if(location[loc]->compound_walls & COMPOUND_TRAPS)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         move(5,21);
         addstr("BOOBY TRAPS");
      }

      if(location[loc]->compound_walls & COMPOUND_TANKTRAPS)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(5,41);
         addstr("TANK TRAPS");
      }

      if(location[loc]->siege.siege&&location[loc]->siege.lights_off)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(5,60);
         addstr("LIGHTS OUT");
      }
      else if(location[loc]->compound_walls & COMPOUND_GENERATOR)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(5,61);
         addstr("GENERATOR");
      }

      int eaters=numbereating(loc);
      int days=fooddaysleft(loc);
      char num[20];

      if(eaters>0&&location[loc]->compound_walls!=COMPOUND_PRINTINGPRESS)
      {
         if(days>=1)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            itoa(days,num,10);
            move(6,50);
            addstr(num);
            addstr(" Day");
            if(days!=1)addstr("s");
            addstr(" of Food Left");
         }
         else if(days==0)
         {
            set_color(COLOR_RED,COLOR_BLACK,0);
            move(6,50);
            addstr("Not Enough Food");
         }
      }
      if(location[loc]->compound_walls!=COMPOUND_PRINTINGPRESS)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         itoa(location[loc]->compound_stores,num,10);
         move(6,1);
         addstr(num);
         addstr(" Daily Ration");
         if(location[loc]->compound_stores!=1)addstr("s");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         itoa(eaters,num,10);
         move(6,30);
         addstr(num);
         addstr(" Eating");
      }
   }
}


/* character info at top of screen */
void printcreatureinfo(creaturest *cr)
{
   char num[20],str[200];

   makedelimiter(1,0);

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(1,2);
   addstr(cr->name);
   addstr(", ");
   gettitle(str,*cr);
   addstr(str);

   if(cr->prisoner!=NULL)
   {
      addstr(", holding ");
      switch(cr->prisoner->type)
      {
         case CREATURE_CORPORATE_CEO:addstr("a squirming CEO");break;
         case CREATURE_RADIOPERSONALITY:addstr("a crying Radio Personality");break;
         case CREATURE_NEWSANCHOR:addstr("smarmy News Anchor");break;
         case CREATURE_SCIENTIST_EMINENT:addstr("a frightened Eminent Scientist");break;
         case CREATURE_JUDGE_CONSERVATIVE:addstr("an angry Hangin' Judge");break;
         default:
            addstr(cr->prisoner->name);
            break;
      }
   }

   move(2,0);addstr("Hrt:    ");
   itoa(cr->attval(ATTRIBUTE_HEART),num,10);
   addstr(num);
   move(3,0);addstr("Int:    ");
   itoa(cr->attval(ATTRIBUTE_INTELLIGENCE),num,10);
   addstr(num);
   move(4,0);addstr("Wis:    ");
   itoa(cr->attval(ATTRIBUTE_WISDOM),num,10);
   addstr(num);
   move(5,0);addstr("Hlth:   ");
   itoa(cr->attval(ATTRIBUTE_HEALTH),num,10);
   addstr(num);
   move(2,11);addstr("Agi:    ");
   itoa(cr->attval(ATTRIBUTE_AGILITY),num,10);
   addstr(num);
   move(3,11);addstr("Str:    ");
   itoa(cr->attval(ATTRIBUTE_STRENGTH),num,10);
   addstr(num);
   move(4,11);addstr("Char:   ");
   itoa(cr->attval(ATTRIBUTE_CHARISMA),num,10);
   addstr(num);
   move(5,11);
   addstr("Trans: ");
   long v=-1;
   if(showcarprefs==1)v=id_getcar(cr->pref_carid);
   else v=id_getcar(cr->carid);
   if(v!=-1&&showcarprefs!=-1)
   {
      getcar(str,vehicle[v]->type);
      char d;
      if(showcarprefs==1)d=cr->pref_is_driver;
      else d=cr->is_driver;
      if(d)strcat(str,"-D");
   }
   else
   {
      int legok=2;
      if((cr->wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
          (cr->wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
      if((cr->wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
          (cr->wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
      if(cr->flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
      else if(legok>=1)strcpy(str,"On Foot");
      else strcpy(str,"On \"Foot\"");
   }
   addstr(str);
   move(6,0);
   if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
   else
      switch(weaponcheck(*cr,sitetype))
      {
      case 0:set_color(COLOR_GREEN,COLOR_BLACK,1);break;
      case 1:set_color(COLOR_YELLOW,COLOR_BLACK,1);break;
      case 2:set_color(COLOR_RED,COLOR_BLACK,1);break;
      }
   addstr("Weapon: ");
   getweaponfull(str,cr->weapon.type);
   addstr(str);
   if(ammotype(cr->weapon.type)!=-1)
   {
      char num[20];
      itoa(cr->weapon.ammo,num,10);
      addstr(" (");
      addstr(num);
      int at=ammotype(cr->weapon.type);
      itoa(cr->clip[at],num,10);
      addstr("/");
      addstr(num);
      addstr(")");
   }

   if(mode!=GAMEMODE_SITE)set_color(COLOR_WHITE,COLOR_BLACK,0);
   else if(hasdisguise(*cr,sitetype))
   {
      if(cr->armor.quality!='1'||
         cr->armor.flag>0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
      else set_color(COLOR_GREEN,COLOR_BLACK,1);
   }
   else set_color(COLOR_RED,COLOR_BLACK,1);
   move(7,0);
   addstr("Armor: ");
   getarmorfull(str,cr->armor,0);
   addstr(str);

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   char used[SKILLNUM];
   memset(used,0,sizeof(char)*SKILLNUM);

   int snum=5;
   char printed=1;

   while(snum>0&&printed)
   {
      printed=0;

      unsigned long max=0;
      long maxs=-1;
      for(int s=0;s<SKILLNUM;s++)
      {
         if(cr->skill[s]>max && !used[s])
         {
            max=cr->skill[s];
            maxs=s;
         }
      }

      if(maxs!=-1)
      {
         used[maxs]=1;
         printed=1;

         if(cr->skill_ip[maxs]>=100*((10+cr->skill[maxs])/10)&&
            cr->skill[maxs]<cr->attval(skillatt(maxs))*2)set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         move(3+5-snum,31);
         getskill(str,maxs);
         strcat(str,": ");
         itoa(cr->skill[maxs],num,10);
         strcat(str,num);
         addstr(str);

         if(snum==5&&printed)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(2,31);
            addstr("Top Skills:");
         }
      }

      snum--;
   }

   int woundsum=0;
   for(int w=0;w<BODYPARTNUM;w++)
   {
      if(cr->wound[w]!=0)woundsum++;
   }

   printhealthstat(*cr,1,49,FALSE);

   if(woundsum>0)
   {
      for(int w=0;w<BODYPARTNUM;w++)
      {
         if(cr->wound[w] & WOUND_BLEEDING)set_color(COLOR_RED,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         move(2+w,49);
         switch(w)
         {
            case BODYPART_HEAD:addstr("Head:");break;
            case BODYPART_BODY:addstr("Body:");break;
            case BODYPART_ARM_RIGHT:addstr("Right Arm:");break;
            case BODYPART_ARM_LEFT:addstr("Left Arm:");break;
            case BODYPART_LEG_RIGHT:addstr("Right Leg:");break;
            case BODYPART_LEG_LEFT:addstr("Left Leg:");break;
         }

         move(2+w,61);
         if(cr->wound[w] & WOUND_NASTYOFF)addstr("Ripped off");
         else if(cr->wound[w] & WOUND_CLEANOFF)addstr("Clean sever");
         else
         {
            int sum=0;

            if(cr->wound[w] & WOUND_SHOT)sum++;
            if(cr->wound[w] & WOUND_CUT)sum++;
            if(cr->wound[w] & WOUND_BRUISED)sum++;
            if(cr->wound[w] & WOUND_BURNED)sum++;
            if(cr->wound[w] & WOUND_TORN)sum++;

            if(sum==0)
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr("Liberal");
            }

            if(cr->wound[w] & WOUND_SHOT){addstr("Sht");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_BRUISED){addstr("Brs");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_CUT){addstr("Cut");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_TORN){addstr("Trn");sum--;if(sum>0)addstr(",");}
            if(cr->wound[w] & WOUND_BURNED){addstr("Brn");sum--;if(sum>0)addstr(",");}
         }
      }
      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
}



/* full character sheet (with surrounding interface) */
void fullstatus(int p)
{
   if(activesquad==NULL)return;

   do
   {
      erase();

      set_color(COLOR_GREEN,COLOR_BLACK,1);
      move(0,0);
      addstr("Profile of a Liberal");

      printliberalstats(*activesquad->squad[p],0);

      move(23,0);
      addstr("Press N to change this Liberal's Code Name");
      if(activesquad->squad[1]!=NULL)
      {
         if(interface_pgup=='[')
         {
            addstr(", [] to view the others");
         }
         else if(interface_pgup=='.')
         {
            addstr(", ; and : to view the others");
         }
         else
         {
            addstr(", PGUP/PGDN to view the others");
         }
      }
      move(24,0);
      addstr("Press any other key to continue the Struggle");

      refresh();
      int c=getch();
      translategetch(c);

      if(activesquad->squad[1]!=NULL&&(c==interface_pgup||c==interface_pgdn))
      {
         int sx=1;
         if(c==interface_pgup)sx=-1;
         do
         {
            p=(p+6+sx)%6;
         }while(activesquad->squad[p]==NULL);
         continue;
      }

      if(c=='n')
      {
         move(23,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("What is the new code name?                                       ");
         move(24,0);
         addstr("                                                                 ");

         keypad(stdscr,FALSE);
         raw_output(FALSE);
         echo();
         curs_set(1);
         move(24,0);
         enter_name(activesquad->squad[p]->name,CREATURE_NAMELEN);

         curs_set(0);
         noecho();
         raw_output(TRUE);
         keypad(stdscr,TRUE);
      }
      else break;
   }while(1);
}



/* full screen character sheet */
void printliberalstats(creaturest &cr,char smll)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);

   char num[20],str[200];

   move(2,0);
   addstr("Name: ");
   addstr(cr.name);
   addstr(", ");
   gettitle(str,cr);
   addstr(str);

   move(4,25);addstr("Juice: ");
   itoa(cr.juice,num,10);
   addstr(num);
   if(cr.juice<500)
   {
      move(5,25);addstr("Next: ");
      if(cr.juice<0)addstr("0");
      else if(cr.juice<10)addstr("10");
      else if(cr.juice<50)addstr("50");
      else if(cr.juice<100)addstr("100");
      else if(cr.juice<200)addstr("200");
      else addstr("500");
   }

   move(4,0);addstr("Heart: ");
   itoa(cr.attval(ATTRIBUTE_HEART),num,10);
   addstr(num);
   move(5,0);addstr("Intelligence: ");
   itoa(cr.attval(ATTRIBUTE_INTELLIGENCE),num,10);
   addstr(num);
   move(6,0);addstr("Wisdom: ");
   itoa(cr.attval(ATTRIBUTE_WISDOM),num,10);
   addstr(num);
   move(7,0);addstr("Health: ");
   itoa(cr.attval(ATTRIBUTE_HEALTH),num,10);
   addstr(num);
   move(8,0);addstr("Agility: ");
   itoa(cr.attval(ATTRIBUTE_AGILITY),num,10);
   addstr(num);
   move(9,0);addstr("Strength: ");
   itoa(cr.attval(ATTRIBUTE_STRENGTH),num,10);
   addstr(num);
   move(10,0);addstr("Charisma: ");
   itoa(cr.attval(ATTRIBUTE_CHARISMA),num,10);
   addstr(num);

   char used[SKILLNUM];
   memset(used,0,sizeof(char)*SKILLNUM);

   int snum=14;
   if(smll)snum=7;
   char printed=1;

   while(snum>0&&printed)
   {
      printed=0;

      unsigned long max=0;
      long maxs=-1;
      for(int s=0;s<SKILLNUM;s++)
      {
         if(cr.skill[s]>max && !used[s])
         {
            max=cr.skill[s];
            maxs=s;
         }
      }

      if(maxs!=-1)
      {
         used[maxs]=1;
         printed=1;

         if(cr.skill_ip[maxs]>=100*((10+cr.skill[maxs])/10)&&
            cr.skill[maxs]<cr.attval(skillatt(maxs))*2)set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         if(!smll)move(4+14-snum,40);
         else move(4+7-snum,40);
         getskill(str,maxs);
         strcat(str,": ");
         itoa(cr.skill[maxs],num,10);
         strcat(str,num);
         addstr(str);

         if((snum==14&&!smll)||(snum==7&&smll))
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(2,40);
            getrecruitcreature(str,cr.type);
            addstr(str);
         }
      }

      snum--;
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);

   if(!smll)
   {
      move(12,0);
      addstr("Weapon: ");
      getweaponfull(str,cr.weapon.type);
      addstr(str);

      if(ammotype(cr.weapon.type)!=-1)
      {
         char num[20];
         itoa(cr.weapon.ammo,num,10);
         addstr(" (");
         addstr(num);
         int at=ammotype(cr.weapon.type);
         itoa(cr.clip[at],num,10);
         addstr("/");
         addstr(num);
         addstr(")");
      }

      move(13,0);
      addstr("Armor: ");
      getarmorfull(str,cr.armor,1);
      addstr(str);

      move(14,0);
      addstr("Transport: ");
      long v=-1;
      if(showcarprefs==1)v=id_getcar(cr.pref_carid);
      else v=id_getcar(cr.carid);
      if(v!=-1&&showcarprefs!=-1)
      {
         getcarfull(str,*vehicle[v]);
         char d;
         if(showcarprefs==1)d=cr.pref_is_driver;
         else d=cr.is_driver;
         if(d)strcat(str,"-D");
      }
      else
      {
         int legok=2;
         if((cr.wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
             (cr.wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
         if((cr.wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
             (cr.wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;
         if(cr.flag & CREATUREFLAG_WHEELCHAIR)strcpy(str,"Wheelchair");
         else if(legok>=1)strcpy(str,"On Foot");
         else strcpy(str,"On \"Foot\"");
      }
      addstr(str);

      int woundsum=0;
      for(int w=0;w<BODYPARTNUM;w++)
      {
         if(cr.wound[w]!=0)woundsum++;
      }

      if(woundsum>0)
      {
         for(int w=0;w<BODYPARTNUM;w++)
         {
            if(cr.wound[w] & WOUND_BLEEDING)set_color(COLOR_RED,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);

            move(16+w,0);
            switch(w)
            {
               case BODYPART_HEAD:addstr("Head:");break;
               case BODYPART_BODY:addstr("Body:");break;
               case BODYPART_ARM_RIGHT:addstr("Right Arm:");break;
               case BODYPART_ARM_LEFT:addstr("Left Arm:");break;
               case BODYPART_LEG_RIGHT:addstr("Right Leg:");break;
               case BODYPART_LEG_LEFT:addstr("Left Leg:");break;
            }

            move(16+w,12);
            if(cr.wound[w] & WOUND_NASTYOFF)addstr("Ripped off (Scared Straight)");
            else if(cr.wound[w] & WOUND_CLEANOFF)addstr("Cleanly severed");
            else
            {
               int sum=0;

               if(cr.wound[w] & WOUND_SHOT)sum++;
               if(cr.wound[w] & WOUND_CUT)sum++;
               if(cr.wound[w] & WOUND_BRUISED)sum++;
               if(cr.wound[w] & WOUND_BURNED)sum++;
               if(cr.wound[w] & WOUND_TORN)sum++;

               if(sum==0)
               {
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  addstr("Liberal");
               }

               if(cr.wound[w] & WOUND_SHOT){addstr("Shot");sum--;if(sum>0)addstr(",");}
               if(cr.wound[w] & WOUND_BRUISED){addstr("Bruised");sum--;if(sum>0)addstr(",");}
               if(cr.wound[w] & WOUND_CUT){addstr("Cut");sum--;if(sum>0)addstr(",");}
               if(cr.wound[w] & WOUND_TORN){addstr("Torn");sum--;if(sum>0)addstr(",");}
               if(cr.wound[w] & WOUND_BURNED){addstr("Burned");sum--;if(sum>0)addstr(",");}
            }
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      }
   }

   //SPECIAL WOUNDS
   if(!smll)
   {
      set_color(COLOR_GREEN,COLOR_BLACK,0);

      move(3,62);
      if(cr.special[SPECIALWOUND_HEART]!=1)addstr("Heart Punctured");
      move(4,62);
      if(cr.special[SPECIALWOUND_RIGHTLUNG]!=1)addstr("R. Lung Collapsed");
      move(5,62);
      if(cr.special[SPECIALWOUND_LEFTLUNG]!=1)addstr("L. Lung Collapsed");
      move(6,62);
      if(cr.special[SPECIALWOUND_NECK]!=1)addstr("Broken Neck");
      move(7,62);
      if(cr.special[SPECIALWOUND_UPPERSPINE]!=1)addstr("Broken Up Spine");
      move(8,62);
      if(cr.special[SPECIALWOUND_LOWERSPINE]!=1)addstr("Broken Lw Spine");
      move(9,62);
      if(cr.special[SPECIALWOUND_RIGHTEYE]!=1)addstr("No Right Eye");
      move(10,62);
      if(cr.special[SPECIALWOUND_LEFTEYE]!=1)addstr("No Left Eye");
      move(11,62);
      if(cr.special[SPECIALWOUND_NOSE]!=1)addstr("No Nose");
      move(12,62);
      if(cr.special[SPECIALWOUND_TONGUE]!=1)addstr("No Tongue");
      move(13,62);
      if(cr.special[SPECIALWOUND_TEETH]==0)addstr("No Teeth");
      else if(cr.special[SPECIALWOUND_TEETH]==TOOTHNUM-1)addstr("Missing a Tooth");
      else if(cr.special[SPECIALWOUND_TEETH]<TOOTHNUM)addstr("Missing Teeth");
      move(14,62);
      if(cr.special[SPECIALWOUND_LIVER]!=1)addstr("Liver Damaged");
      move(15,62);
      if(cr.special[SPECIALWOUND_RIGHTKIDNEY]!=1)addstr("R. Kidney Damaged");
      move(16,62);
      if(cr.special[SPECIALWOUND_LEFTKIDNEY]!=1)addstr("L. Kidney Damaged");
      move(17,62);
      if(cr.special[SPECIALWOUND_STOMACH]!=1)addstr("Stomach Injured");
      move(18,62);
      if(cr.special[SPECIALWOUND_SPLEEN]!=1)addstr("Busted Spleen");
      move(19,62);
      if(cr.special[SPECIALWOUND_RIBS]==0)addstr("All Ribs Broken");
      else if(cr.special[SPECIALWOUND_RIBS]==RIBNUM-1)addstr("Broken Rib");
      else if(cr.special[SPECIALWOUND_RIBS]<RIBNUM)addstr("Broken Ribs");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
   }
}


/* draws a horizontal line across the screen */
void makedelimiter(int y,int x)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(y,x);
   addstr("--------------------------------------------------------------------------------");
}



/* print location name (is aware of business fronts) */
void addlocationname(locationst *loc)
{
   if(loc->front_business!=-1)
   {
      addstr(loc->front_name);
   }
   else
   {
      addstr(loc->name);
   }
}

/* prints a character's health description (One Leg, Liberal, NearDETH...) */
void printhealthstat(creaturest &g,int y,int x,char smll)
{
   short woundsum=0;
   char bleeding=0;
   for(int w=0;w<BODYPARTNUM;w++)
   {
      if(g.wound[w]!=0)woundsum++;
      if(g.wound[w] & WOUND_BLEEDING)bleeding=1;
   }

   int armok=2;
   int legok=2;
   if((g.wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
       (g.wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
   if((g.wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
       (g.wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
   if((g.wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
       (g.wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF))legok--;
   if((g.wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
       (g.wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF))legok--;

   move(y,x);
   if(bleeding)set_color(COLOR_RED,COLOR_BLACK,1);
   else set_color(COLOR_WHITE,COLOR_BLACK,1);
   if(!g.alive)
   {
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("Deceased");
   }
   else if(g.blood<=20)
   {
      if(smll)addstr("NearDETH");
      else addstr("Near Death");
   }
   else if(g.blood<=50)
   {
      if(smll)addstr("BadWound");
      else addstr("Badly Wounded");
   }
   else if(g.blood<=75)
   {
      addstr("Wounded");
   }
   else if(g.blood<100)
   {
      if(smll)addstr("LtWound");
      else addstr("Lightly Wounded");
   }
   else if(g.special[SPECIALWOUND_NECK]==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("NckBroke");
      else addstr("Neck Broken");
   }
   else if(g.special[SPECIALWOUND_UPPERSPINE]==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("Quadpleg");
      else addstr("Quadraplegic");
   }
   else if(g.special[SPECIALWOUND_LOWERSPINE]==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("Parapleg");
      else addstr("Paraplegic");
   }
   else if(g.special[SPECIALWOUND_RIGHTEYE]==0&&
      g.special[SPECIALWOUND_LEFTEYE]==0&&
      g.special[SPECIALWOUND_NOSE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("FaceGone");
      else addstr("Face Gone");
   }
   else if(legok==0&&armok==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Limbs");
   }
   else if((legok==1&&armok==0)||(armok==1&&legok==0))
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("One Limb");
   }
   else if(legok==2&&armok==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Arms");
   }
   else if(legok==0&&armok==2)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Legs");
   }
   else if(legok==1&&armok==1)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("1Arm1Leg");
      else addstr("One Arm, One Leg");
   }
   else if(armok==1)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("One Arm");
   }
   else if(legok==1)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("One Leg");
   }
   else if(g.special[SPECIALWOUND_RIGHTEYE]==0&&
      g.special[SPECIALWOUND_LEFTEYE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("Blind");
   }
   else if((g.special[SPECIALWOUND_RIGHTEYE]==0||
      g.special[SPECIALWOUND_LEFTEYE]==0)&&
      g.special[SPECIALWOUND_NOSE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("FaceMutl");
      else addstr("Face Mutilated");
   }
   else if(g.special[SPECIALWOUND_NOSE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("NoseGone");
      else addstr("Missing Nose");
   }
   else if(g.special[SPECIALWOUND_RIGHTEYE]==0||
      g.special[SPECIALWOUND_LEFTEYE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("One Eye");
      else addstr("Missing Eye");
   }
   else if(g.special[SPECIALWOUND_TONGUE]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("NoTongue");
      else addstr("No Tongue");
   }
   else if(g.special[SPECIALWOUND_TEETH]==0)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      addstr("No Teeth");
   }
   else if(g.special[SPECIALWOUND_TEETH]<TOOTHNUM)
   {
      if(!bleeding)set_color(COLOR_GREEN,COLOR_BLACK,0);
      if(smll)addstr("MisTeeth");
      else addstr("Missing Teeth");
   }
   else
   {
      if(g.align==-1)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         if(smll)addstr("Consrvtv");
         else addstr("Conservative");
      }
      else if(g.align==0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("Moderate");
      }
      else
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         addstr("Liberal");
      }
   }
}
