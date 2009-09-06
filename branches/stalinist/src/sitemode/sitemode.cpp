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



void mode_site(short loc)
{
   sitetype=location[loc]->type;
   cursite=loc;
   sitealarm=0;
   sitealarmtimer=-1;
   postalarmtimer=0;
   siteonfire=0;
   sitealienate=0;
   sitecrime=0;
   initsite(*location[loc]);
   sitestory=newsstory[newsstory.size()-1];
   mode=GAMEMODE_SITE;

   if(!location[loc]->siege.siege)
   {
      //Low profile site action?
      if(activesquad->stance == SQUADSTANCE_ANONYMOUS)
         sitestory->claimed = 0;

      //Start at entrance to map
      locx=MAPX>>1;
      locy=1;
      locz=0;

      //check for sleeper infiltration
      for(int p=0;p<pool.size();p++)
      {
         //sleeper infiltration :D
         if(pool[p]->base==loc || location[loc]->interrogated)
         {
            //make entire site known
            for(int x=0;x<MAPX;x++)
            {
               for(int y=0;y<MAPY;y++)
               {
                  for(int z=0;z<MAPZ;z++)
                  {
                     levelmap[x][y][z].flag|=SITEBLOCK_KNOWN;
                  }
               }
            }
            break;
         }
      }
      
   }
   else
   {
      sitealarm=1;

      location[loc]->siege.attacktime=0;
      location[loc]->siege.kills=0;

      //PLACE YOU
      int maxy=0;

      for(int x=0;x<MAPX;x++)
      {
         for(int y=0;y<MAPY;y++)
         {
            for(int z=0;z<MAPZ;z++)
            {
               if(!location[loc]->siege.lights_off)levelmap[x][y][z].flag|=SITEBLOCK_KNOWN;
               levelmap[x][y][z].flag&=~SITEBLOCK_LOCKED;
               levelmap[x][y][z].flag&=~SITEBLOCK_LOOT;
            }
            if(!(levelmap[x][y][0].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR)))maxy=y;
         }
      }

      //Cops have tanks; firemen have fire.
      if(location[loc]->siege.siegetype==SIEGE_FIREMEN)
      {
         short firesstarted=0;
         short firex=LCSrandom(MAPX);
         short firey=LCSrandom(MAPY);
         do
         {
            firex=LCSrandom(MAPX);
            firey=LCSrandom(MAPY);
            firesstarted++;
            levelmap[firex][firey][0].flag |= SITEBLOCK_FIRE_START;

         } while(!(levelmap[firex][firey][0].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT)) &&
            firesstarted<4);
      }

      do
      {
         locx=LCSrandom(MAPX);
         locy=maxy-LCSrandom(3);
         if(locy<3)locy=3;
         locz=0;
      }while(levelmap[locx][locy][locz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|
         SITEBLOCK_FIRE_START|SITEBLOCK_FIRE_PEAK|SITEBLOCK_FIRE_END));

      //PLACE LOOT
      int lootnum=location[loc]->loot.size();
      if(lootnum>10)lootnum=10;

      int lx,ly,lz;
      for(int l=0;l<lootnum;l++)
      {
         do
         {
            lx=LCSrandom(MAPX);
            ly=LCSrandom(MAPY);
            lz=0;
         }while(levelmap[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT));
         levelmap[lx][ly][lz].flag|=SITEBLOCK_LOOT;
      }

      //PLACE TRAPS
      if(location[loc]->compound_walls & COMPOUND_TRAPS)
      {
         int trapnum=30;
         for(int t=0;t<trapnum;t++)
         {
            do
            {
               lx=LCSrandom(MAPX);
               ly=LCSrandom(MAPY);
               lz=0;
            }while(levelmap[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT|SITEBLOCK_LOOT));
            levelmap[lx][ly][lz].siegeflag|=SIEGEFLAG_TRAP;
         }
      }

      //PLACE UNITS
      int unitnum=10;
      int count=50000;
      for(int t=0;t<unitnum;t++)
      {
         do
         {
            lx=LCSrandom(11)+(MAPX/2)-5;
            ly=LCSrandom(8);
            lz=0;
            count--;
            if(count==0)break;
         }while((levelmap[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
               (levelmap[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
         levelmap[lx][ly][lz].siegeflag|=SIEGEFLAG_UNIT;
      }

      if(!(location[loc]->compound_walls & COMPOUND_TANKTRAPS)&&
         location[loc]->siege.siegetype==SIEGE_POLICE&&
         location[loc]->siege.escalationstate>=2)
      {
         count=50000;
         int hunitnum=3;
         for(int t=0;t<hunitnum;t++)
         {
            do
            {
               lx=LCSrandom(11)+(MAPX/2)-5;
               ly=LCSrandom(8);
               lz=0;
               count--;
               if(count==0)break;
            }while((levelmap[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
                  (levelmap[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
            levelmap[lx][ly][lz].siegeflag|=SIEGEFLAG_HEAVYUNIT;
         }
      }
   }

   mode_site();
}



void mode_site(void)
{
   int p, u;
   int x;
   if(activesquad==NULL)return;

   reloadparty();

   showcarprefs=-1;

   char bail_on_base=1;
   if(cursite==activesquad->squad[0]->base)bail_on_base=0;

   knowmap(locx,locy,locz);

   char hostcheck=0;

   do
   {
      int partysize=0;
      int partyalive=0;
      for(p=0;p<6;p++)
      {
         if(activesquad->squad[p]!=NULL)partysize++;
         else continue;

         if(activesquad->squad[p]->alive==1)partyalive++;
      }
      int encsize=0;
      int freeable=0;
      int enemy=0;
      int majorenemy=0;
      int talkers=0;
      for(int e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists)
         {
            encsize++;
            if(encounter[e].enemy())enemy++;
            if(encounter[e].type==CREATURE_WORKER_SERVANT||
               encounter[e].type==CREATURE_WORKER_FACTORY_CHILD||
               encounter[e].type==CREATURE_WORKER_SWEATSHOP||
               (strcmp(encounter[e].name,"Prisoner")==0&&encounter[e].align==1))freeable++;
            else if((encounter[e].cantbluff!=1||sitealarm)&&!(encounter[e].align==1&&sitealarm&&enemy))talkers++;
            if(encounter[e].type==CREATURE_CORPORATE_CEO||
               encounter[e].type==CREATURE_RADIOPERSONALITY||
               encounter[e].type==CREATURE_NEWSANCHOR||
               encounter[e].type==CREATURE_SCIENTIST_EMINENT||
               encounter[e].type==CREATURE_JUDGE_CONSERVATIVE)majorenemy++;
         }
      }

      //If in combat, do a second check
      if(talkers&&sitealarm&&enemy)
      {
         talkers=0;
         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].exists)
            {
               if(encounter[e].enemy())talkers++;
            }
         }
      }
      int libnum=0;
      for(p=0;p<pool.size();p++)
      {
         if(pool[p]->align==1&&
            pool[p]->alive&&
            pool[p]->location==cursite&&
            !(pool[p]->flag & CREATUREFLAG_SLEEPER))libnum++;
      }

      erase();

      if(location[cursite]->siege.siege)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         move(0,0);
         addlocationname(location[cursite]);
         addstr(", Level ");
         char num[20];
         itoa(locz+1,num,10);
         addstr(num);
         addstr(": Escape or Engage");
      }
      else
      {
         if (postalarmtimer>100)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(postalarmtimer>80)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(0,0);
         addlocationname(location[cursite]);
         addstr(", Level ");
         char num[20];
         itoa(locz+1,num,10);
         addstr(num);

         if(postalarmtimer>100)addstr(": MASSIVE CONSERVATIVE RESPONSE");
         else if(postalarmtimer>80)addstr(": MASSIVE RESPONSE IMMINENT");
         else if(sitealienate==1)addstr(": ALIENATED MASSES");
         else if(sitealienate==2)addstr(": ALIENATED EVERYONE");
         else if(sitealarm)addstr(": CONSERVATIVES ALARMED");
         else if(sitealarmtimer==0)addstr(": CONSERVATIVES SUSPICIOUS");
      }

      //PRINT PARTY
      if(partyalive==0)party_status=-1;
      printparty();

      //PRINT SITE INSTRUCTIONS
      if(partyalive>0)
      {
         if(!enemy||!sitealarm)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(9,1);
         addstr("W,A,D,X - Move");
         if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("O - Change the squad's Liberal order");
         if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,1);
         addstr("# - Check the status of a squad Liberal");
         if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("0 - Show the squad's Liberal status");

         if(groundloot.size()>0||(levelmap[locx][locy][locz].flag&SITEBLOCK_LOOT))
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         }
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(9,17);
         addstr("G - Get Loot");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,32);
         addstr("M - Map");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,32);
         addstr("S - Wait");

         if(!enemy||!sitealarm)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,42);
         addstr("L - Reload");

         if(enemy)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,42);
         addstr("K - Kidnap");

         if(talkers)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(14,17);
         addstr("T - Talk");

         bool graffiti=0;
         if(levelmap[locx][locy][locz].special!=-1&&
            levelmap[locx][locy][locz].special!=SPECIAL_CLUB_BOUNCER_SECONDVISIT)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else if(!(levelmap[locx][locy][locz].flag & (SITEBLOCK_GRAFFITI|SITEBLOCK_BLOODY2)))
         {
            if((levelmap[locx+1][locy][locz].flag & SITEBLOCK_BLOCK)||
               (levelmap[locx-1][locy][locz].flag & SITEBLOCK_BLOCK)||
               (levelmap[locx][locy+1][locz].flag & SITEBLOCK_BLOCK)||
               (levelmap[locx][locy-1][locz].flag & SITEBLOCK_BLOCK))
            {
               int i;
               for(i=0;i<6;i++)
               {
                  if(!activesquad->squad[i])
                  {
                     i=6;
                  }
                  else if(activesquad->squad[i]->weapon.type==WEAPON_SPRAYCAN)
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,0);
                     graffiti=1;
                     break;
                  }
               }
               if(i==6)set_color(COLOR_BLACK,COLOR_BLACK,1);
            }
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
         }
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,42);
         if(graffiti)addstr("U - Graffiti");
         else addstr("U - Use");

         if(enemy&&sitealarm)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,42);
         addstr("V - Evade");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,42);
         addstr("E - Equip");

         if(enemy)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(14,1);
         addstr("F - Fight!");

         if(!location[cursite]->siege.siege)
         {
            if(freeable&&(!enemy||!sitealarm))set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(14,32);
            addstr("R - Release oppressed");
         }
         else
         {
            if(libnum>6)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(14,32);
            addstr("R - Reorganize");
         }
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,1);
         addstr("C - Reflect on your Conservative ineptitude");
      }

      //PRINT SITE MAP
      printsitemap(locx,locy,locz);

      //CHECK IF YOU HAVE A SQUIRMING AMATEUR HOSTAGE
         //hostcheck SHOULD ONLY BE 1 WHEN A NEWENC IS CREATED
      if(hostcheck)
      {
         char havehostage=0;
         //Check your whole squad
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)
            {
               //If they're unarmed and dragging someone
               if(activesquad->squad[p]->prisoner!=NULL&&
                  activesquad->squad[p]->weapon.type==WEAPON_NONE)
               {
                  //And that someone is not an LCS member
                  if(activesquad->squad[p]->prisoner->squadid==-1)
                  {
                     //They scream for help -- flag them kidnapped, cause alarm
                     activesquad->squad[p]->prisoner->flag|=CREATUREFLAG_KIDNAPPED;
                     if(activesquad->squad[p]->type==CREATURE_RADIOPERSONALITY)offended_amradio=1;
                     if(activesquad->squad[p]->type==CREATURE_NEWSANCHOR)offended_cablenews=1;
                     havehostage=1;
                  }
               }
            }
         }

         if(havehostage)
         {
            alienationcheck(1);
            sitecrime+=5;
            criminalizeparty(LAWFLAG_KIDNAPPING);
         }
         hostcheck=0;

         clearmessagearea();
      }

      


      refresh();

      int c;
      if(levelmap[locx][locy][locz].special == SPECIAL_CLUB_BOUNCER)
         c='s';
      else c=getch();
      translategetch(c);

      if(partyalive==0&&c=='c')
      {
         //DESTROY ALL CARS BROUGHT ALONG WITH PARTY
         if(!location[cursite]->siege.siege)
         {
            for(int p=0;p<6;p++)
            {
               if(activesquad->squad[p]==NULL)continue;
               if(activesquad->squad[p]->carid!=-1)
               {
                  long v=id_getcar(activesquad->squad[p]->carid);
                  if(v!=-1)
                  {
                     removecarprefs_pool(vehicle[v]->id);
                     delete vehicle[v];
                     vehicle.erase(vehicle.begin() + v);
                  }
               }
            }
         }

         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;

            for(int pl=pool.size()-1;pl>=0;pl--)
            {
               if(pool[pl]==activesquad->squad[p])
               {
                  pool[pl]->alive=0;
                  pool[pl]->location=-1;
                  //delete pool[pl];
                  //pool.erase(pool.begin() + pl);
                  break;
               }
            }

            activesquad->squad[p]=NULL;
         }

         //END OF GAME CHECK
         if(!endcheck())
         {
            if(location[cursite]->siege.siege)
            {
               if(location[cursite]->siege.underattack)sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEATTACK;
               else sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE;
            }
            else 

            if(!location[cursite]->siege.siege)
            {
               sitestory->type=NEWSSTORY_SQUAD_KILLED_SITE;

               resolvesite();
            }

            mode=GAMEMODE_BASE;

            showcarprefs=0;
            return;
         }
      }

      if(partyalive>0)
      {
         int olocx=locx;
         int olocy=locy;
         int olocz=locz;

         char override=0;

         if(c=='v'&&enemy&&sitealarm)
         {
            clearmessagearea();
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(16,1);
            addstr("Which way?  (W,A,D, and X to move, ENTER to abort)");

            refresh();

            do
            {
               int c2=getch();
               translategetch(c2);

               if(c2=='w'||c2=='a'||c2=='d'||c2=='x'||
                  c2==KEY_LEFT||c2==KEY_RIGHT||c2==KEY_UP||c2==KEY_DOWN)
               {
                  c=c2;
                  override=1;
                  break;
               }

               if(c2==10)break;
            }while(1);
         }

         if((c=='w'||c==KEY_UP)&&locy>0&&(!enemy||!sitealarm||override))
         {
            if(!(levelmap[locx][locy-1][locz].flag & SITEBLOCK_BLOCK))locy--;
         }
         if((c=='a'||c==KEY_LEFT)&&locx>0&&(!enemy||!sitealarm||override))
         {
            if(!(levelmap[locx-1][locy][locz].flag & SITEBLOCK_BLOCK))locx--;
         }
         if((c=='d'||c==KEY_RIGHT)&&locx<MAPX-1&&(!enemy||!sitealarm||override))
         {
            if(!(levelmap[locx+1][locy][locz].flag & SITEBLOCK_BLOCK))locx++;
         }
         if((c=='x'||c==KEY_DOWN)&&locy<MAPY-1&&(!enemy||!sitealarm||override))
         {
            if(!(levelmap[locx][locy+1][locz].flag & SITEBLOCK_BLOCK))locy++;
         }

         if(c=='k'&&enemy)
         {
            kidnapattempt();
         }

         if(c=='u')
         {
            if(levelmap[locx][locy][locz].special!=-1)
            {
               switch(levelmap[locx][locy][locz].special)
               {
                  case SPECIAL_LAB_COSMETICS_CAGEDANIMALS:special_lab_cosmetics_cagedanimals();break;
                  case SPECIAL_NUCLEAR_ONOFF:special_nuclear_onoff();break;
                  case SPECIAL_LAB_GENETIC_CAGEDANIMALS:special_lab_genetic_cagedanimals();break;
                  case SPECIAL_POLICESTATION_LOCKUP:special_policestation_lockup();break;
                  case SPECIAL_COURTHOUSE_LOCKUP:special_courthouse_lockup();break;
                  case SPECIAL_COURTHOUSE_JURYROOM:special_courthouse_jury();break;
                  case SPECIAL_PRISON_CONTROL:special_prison_control();break;
                  case SPECIAL_INTEL_SUPERCOMPUTER:special_intel_supercomputer();break;
                  case SPECIAL_SWEATSHOP_EQUIPMENT:special_sweatshop_equipment();break;
                  case SPECIAL_POLLUTER_EQUIPMENT:special_polluter_equipment();break;
                  case SPECIAL_HOUSE_PHOTOS:special_house_photos();break;
                  case SPECIAL_CORPORATE_FILES:special_corporate_files();break;
                  case SPECIAL_RADIO_BROADCASTSTUDIO:special_radio_broadcaststudio();break;
                  case SPECIAL_NEWS_BROADCASTSTUDIO:special_news_broadcaststudio();break;
                  case SPECIAL_APARTMENT_SIGN:special_readsign(SPECIAL_APARTMENT_SIGN);break;
                  case SPECIAL_STAIRS_UP:locz++;break;
                  case SPECIAL_STAIRS_DOWN:locz--;break;
               }
            }
            else if(!(levelmap[locx][locy][locz].flag & (SITEBLOCK_GRAFFITI|SITEBLOCK_BLOODY2))&&
                    ((levelmap[locx+1][locy][locz].flag & SITEBLOCK_BLOCK)||
                     (levelmap[locx-1][locy][locz].flag & SITEBLOCK_BLOCK)||
                     (levelmap[locx][locy+1][locz].flag & SITEBLOCK_BLOCK)||
                     (levelmap[locx][locy-1][locz].flag & SITEBLOCK_BLOCK)))
            {
               int spray=0;
               for(int i=0;i<6;i++)
               {
                  if(!activesquad->squad[i])break;
                  if(activesquad->squad[i]->weapon.type==WEAPON_SPRAYCAN)
                  {
                     spray=1;
                     break;
                  }
               }
               if(spray)
               {
                  special_graffiti();
               }
            }
         }

         if(c=='t'&&talkers)
         {
            int forcesp=-1;
            int forcetk=-1;

            for(int p=0;p<6;p++)
            {
               if(activesquad->squad[p]!=NULL)
               {
                  if(activesquad->squad[p]->alive)
                  {
                     if(forcesp==-1)forcesp=p;
                     else forcesp=-2;
                  }
               }
            }

            for(int e=0;e<ENCMAX;e++)
            {
               if(encounter[e].exists&&encounter[e].alive&&
                  !(encounter[e].type==CREATURE_WORKER_SERVANT||
                     encounter[e].type==CREATURE_WORKER_SWEATSHOP))
               {
                  if(encounter[e].cantbluff!=1||sitealarm)
                  {
                     if(forcetk==-1)forcetk=e;
                     else forcetk=-2;
                  }
               }
            }

            if(forcetk!=-1&&forcesp!=-1)
            {
               int tk=-1,sp=-1;
               if(forcesp==-2)
               {
                  clearcommandarea();
                  clearmessagearea();
                  clearmaparea();

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(9,1);
                  addstr("Which Liberal will speak?");
                  move(9,50);
                  addstr("Issues");
                  move(9,60);
                  addstr("Dating");
                  move(9,70);
                  addstr("Bluff");

                  char num[20];
                  int y=11;
                  for(int p=0;p<6;p++)
                  {
                     if(activesquad->squad[p]!=NULL)
                     {
                        if(activesquad->squad[p]->alive)
                        {
                           move(y,1);
                           addch(p+'1');
                           addstr(" - ");
                           addstr(activesquad->squad[p]->name);
                           move(y,50);
                           itoa(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
                                activesquad->squad[p]->attval(ATTRIBUTE_HEART)+
                                activesquad->squad[p]->skillval(SKILL_PERSUASION)*2,num,10);
                           addstr(num);
                           move(y,60);
                           itoa(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)*2+
                                activesquad->squad[p]->skillval(SKILL_SEDUCTION)*2,num,10);
                           addstr(num);
                           move(y,70);
                           itoa(activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)*2+
                                activesquad->squad[p]->attval(SKILL_DISGUISE)*2,num,10);
                           addstr(num);
                           y++;
                        }
                     }
                  }

                  do
                  {
                     refresh();
                     int c=getch();
                     translategetch(c);
                     if(c>='1'&&c<='6')
                     {
                        sp=c-'1';
                        if(activesquad->squad[sp]!=NULL)
                        {
                           if(activesquad->squad[sp]->alive)break;
                        }
                     }
                     if(c==10||c==27||c==32){sp=-1;break;}
                  }while(1);
               }
               else sp=forcesp;

               if(sp!=-1)
               {
                  if(forcetk==-2)
                  {
                     do
                     {
                        clearcommandarea();
                        clearmessagearea();
                        clearmaparea();

                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(9,1);
                        addstr("To whom?");

                        int x=1,y=11;
                        for(int t=0;t<ENCMAX;t++)
                        {
                           if(encounter[t].exists)
                           {
                              if (encounter[t].cantbluff!=1)
                              {
                                 set_color(COLOR_WHITE,COLOR_BLACK,1);
                                 move(y,x);
                                 addch(t+'A');
                                 addstr(" - ");
                                 switch(encounter[t].align)
                                 {
                                 case ALIGN_CONSERVATIVE:
                                    set_color(COLOR_RED,COLOR_BLACK,1);
                                    break;
                                 case ALIGN_LIBERAL:
                                    set_color(COLOR_GREEN,COLOR_BLACK,1);
                                    break;
                                 case ALIGN_MODERATE:
                                    set_color(COLOR_WHITE,COLOR_BLACK,1);
                                    break;
                                 }
                                 addstr(encounter[t].name);
                                 add_age(encounter[t]);

                                 y++;
                                 if(y==17)
                                 {
                                    y=11;
                                    x+=30;
                                 }
                              } else
                              {
                                 y++;
                                 if(y==17)
                                 {
                                    y=11;
                                    x+=30;
                                 }
                              }
                           }
                        }

                        refresh();
                        int c=getch();
                        translategetch(c);
                        if(c>='a'&&c<='z')
                        {
                           tk=c-'a';
                           if(tk<ENCMAX)
                           {
                              if(encounter[tk].exists)
                              {
                                 if(encounter[tk].alive&&
                                    !(encounter[tk].type==CREATURE_WORKER_SERVANT||
                                    encounter[tk].type==CREATURE_WORKER_SWEATSHOP))
                                 {
                                    if(encounter[tk].cantbluff==1 &&
                                       !sitealarm)
                                    {
                                       clearcommandarea();
                                       clearmessagearea();
                                       clearmaparea();

                                       set_color(COLOR_WHITE,COLOR_BLACK,1);
                                       move(9,1);
                                       addstr(encounter[tk].name);
                                       addstr(" won't talk to you.");

                                       refresh();
                                       getch();
                                    }
                                    else if(!encounter[tk].enemy()&&sitealarm&&enemy)
                                    {
                                       clearcommandarea();
                                       clearmessagearea();
                                       clearmaparea();

                                       set_color(COLOR_WHITE,COLOR_BLACK,1);
                                       move(9,1);
                                       addstr("You have to deal with the enemies first.");

                                       refresh();
                                       getch();
                                    }
                                    else break;
                                 }
                              }
                           }
                        }
                        if(c==10||c==27||c==32){tk=-1;break;}
                     }while(1);
                  }
                  else tk=forcetk;

                  if(tk!=-1)
                  {
                     if(talk(*activesquad->squad[sp],tk))
                     {
                        if(enemy&&sitealarm)enemyattack();
                        else if(enemy)disguisecheck();
                        creatureadvance();
                     }
                  }
               }
            }
         }

         if(c=='l'&&(!enemy||!sitealarm))
         {
            reloadparty(true);
            printparty();
            refresh();
            creatureadvance();
         }

         if(c=='o'&&partysize>1)orderparty();

         if(c=='0')party_status=-1;

         if(c=='m')
         {
            for(int x=0;x<MAPX;x++)
            {
               for(int y=0;y<MAPY;y++)
               {
                  if(levelmap[x][y][locz].flag & SITEBLOCK_KNOWN)
                  {
                     move(y+1,x+5);
                     if(x==locx&&y==locy)
                     {
                        set_color(COLOR_GREEN,COLOR_BLACK,1);
                        addch(CH_WHITE_SMILING_FACE);
                     }
                     else
                     {
                        set_color(COLOR_WHITE,COLOR_BLACK,0);
                        if(levelmap[x][y][locz].flag & SITEBLOCK_BLOCK)addch(CH_FULL_BLOCK);
                        else if(levelmap[x][y][locz].flag & SITEBLOCK_DOOR)addch(CH_LIGHT_SHADE);
                        else if((levelmap[x][y][locz].siegeflag & SIEGEFLAG_HEAVYUNIT)&&
                           (location[cursite]->compound_walls & COMPOUND_CAMERAS)&&!location[cursite]->siege.cameras_off)
                        {
                           set_color(COLOR_RED,COLOR_BLACK,1);
                           addch(CH_YEN_SIGN);
                        }
                        else if((levelmap[x][y][locz].siegeflag & SIEGEFLAG_UNIT)&&
                           (location[cursite]->compound_walls & COMPOUND_CAMERAS)&&!location[cursite]->siege.cameras_off)
                        {
                           set_color(COLOR_RED,COLOR_BLACK,1);
                           addch(CH_BLACK_SMILING_FACE);
                        }
                        else if((levelmap[x][y][locz].siegeflag & SIEGEFLAG_UNIT_DAMAGED)&&
                           (location[cursite]->compound_walls & COMPOUND_CAMERAS)&&!location[cursite]->siege.cameras_off)
                        {
                           set_color(COLOR_RED,COLOR_BLACK,0);
                           addch(CH_BLACK_SMILING_FACE);
                        }
                        else if(levelmap[x][y][locz].special==SPECIAL_STAIRS_UP)
                        {
                           set_color(COLOR_YELLOW,COLOR_BLACK,1);
                           addch(CH_UPWARDS_ARROW);
                        }
                        else if(levelmap[x][y][locz].special==SPECIAL_STAIRS_DOWN)
                        {
                           set_color(COLOR_YELLOW,COLOR_BLACK,1);
                           addch(CH_DOWNWARDS_ARROW);
                        }
                        else if(levelmap[x][y][locz].special!=-1)
                        {
                           set_color(COLOR_YELLOW,COLOR_BLACK,1);
                           addch('!');
                        }
                        else if(levelmap[x][y][locz].siegeflag & SIEGEFLAG_TRAP)
                        {
                           set_color(COLOR_YELLOW,COLOR_BLACK,1);
                           addch('!');
                        }
                        else if(levelmap[x][y][locz].flag & SITEBLOCK_LOOT)
                        {
                           set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                           addch('$');
                        }
                        else addch(' ');
                     }
                  }
                  else
                  {
                     set_color(COLOR_BLACK,COLOR_BLACK,1);
                     move(y+1,x+5);
                     addch(CH_FULL_BLOCK);
                  }
               }
            }

            refresh();
            getch();
         }

         if(enemy&&c=='f')
         {
            youattack();
            enemyattack();
            creatureadvance();
         }

         if(c=='r'&&location[cursite]->siege.siege&&libnum>6)
         {
            assemblesquad(activesquad);
            autopromote(cursite);
         }

         if(freeable&&(!enemy||!sitealarm)&&c=='r'&&!location[cursite]->siege.siege)
         {
            short followers=0;
            short actgot=0;

            if(enemy)sitealarm=1;
            char freed;
            char flipstart;
            do
            {
               flipstart=0;
               freed=0;
               for(int e=0;e<ENCMAX;e++)
               {
                  if(!encounter[e].exists)break;
                  if((encounter[e].type==CREATURE_WORKER_SERVANT||
                     encounter[e].type==CREATURE_WORKER_FACTORY_CHILD||
                     encounter[e].type==CREATURE_WORKER_SWEATSHOP||
                     (strcmp(encounter[e].name,"Prisoner")==0 && encounter[e].align==1))&&!flipstart)
                  {
                     if(strcmp(encounter[e].name,"Prisoner")==0)
                     {
                        sitealarm=1; /* alarm for prisoner escape */
                        criminalize(encounter[e],LAWFLAG_ESCAPED);
                     }
                     followers++;
                     flipstart=1;
                     freed=1;

                     if(partysize<6)
                     {
                        int i;
                        // Check for people who can recruit followers
                        for(i=0;i<6;i++)
                        {
                           if(activesquad->squad[i]!=NULL)
                           {
                              if(subordinatesleft(*activesquad->squad[i]))
                              {
                                 break;
                              }
                           }
                        }
                        // If someone can, add this person as a newly recruited Liberal!
                        if(i!=6)
                        {
                           Creature *newcr=new Creature;
                              *newcr=encounter[e];
                           newcr->namecreature();

                           newcr->location=activesquad->squad[i]->location;
                           newcr->base=activesquad->squad[i]->base;
                           newcr->hireid=activesquad->squad[i]->id;

                           pool.push_back(newcr);
                           stat_recruits++;

                           for(int p=0;p<6;p++)
                           {
                              if(activesquad->squad[p]==NULL)
                              {
                                 activesquad->squad[p]=newcr;
                                 newcr->squadid=activesquad->id;
                                 break;
                              }
                           }

                           actgot++;
                           partysize++;
                        }
                     }
                  }
                  if(flipstart)
                  {
                     if(e<ENCMAX-1)encounter[e]=encounter[e+1];
                  }
               }
               if(flipstart)encounter[ENCMAX-1].exists=0;

               if(freed)
               {
                  int time=20+LCSrandom(10);
                  if(time<1)time=1;
                  if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
               }
            }while(freed);

            if(followers>0)
            {
               clearmessagearea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr("You free ");
               if(followers>1)addstr("some Oppressed Liberals");
               else addstr("an Oppressed Liberal");
               addstr(" from the Conservatives.");

               if(actgot<followers)
               {
                  refresh();
                  getch();

                  clearmessagearea();

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(16,1);
                  if(actgot==0&&followers>1)addstr("They all leave");
                  else if(followers-actgot>1)addstr("Some leave");
                  else if(actgot==0)addstr("The Liberal leaves");
                  else addstr("One Liberal leaves");
                  addstr(" you, feeling safer getting out alone.");
               }

               refresh();
               getch();
            }
         }

         if(c>='1'&&c<='6')
         {
            if(activesquad->squad[c-'1']!=NULL)
            {
               if(party_status==c-'1')fullstatus(party_status);
               else party_status=c-'1';
            }
         }

         if(c=='e')
         {
            equip(activesquad->loot,-1);

            if(enemy&&sitealarm)enemyattack();
            else if(enemy)disguisecheck();

            creatureadvance();
         }

         if(c=='g'&&(groundloot.size()>0||(levelmap[locx][locy][locz].flag&SITEBLOCK_LOOT)))
         {
            char tookground=0;

            if(levelmap[locx][locy][locz].flag&SITEBLOCK_LOOT)
            {
               levelmap[locx][locy][locz].flag&=~SITEBLOCK_LOOT;

               if(location[cursite]->siege.siege)
               {
                  //GRAB SOME OF THE BASE LOOT
                  int lcount=1; //1 FROM THE ONE DELETED ABOVE

                  for(int x=0;x<MAPX;x++)
                  {
                     for(int y=0;y<MAPY;y++)
                     {
                        for(int z=0;z<MAPZ;z++)
                        {
                           if(levelmap[x][y][z].flag&SITEBLOCK_LOOT)
                           {
                              lcount++;
                           }
                        }
                     }
                  }

                  int lplus=location[cursite]->loot.size()/lcount;
                  if(lcount==1)lplus=location[cursite]->loot.size();

                  itemst *it;
                  int b;

                  while(lplus>0)
                  {
                     b=LCSrandom(location[cursite]->loot.size());
                     it=location[cursite]->loot[b];
                     activesquad->loot.push_back(it);
                     location[cursite]->loot.erase(location[cursite]->loot.begin() + b);

                     lplus--;
                  }
               }
               else
               {
                  int time=20+LCSrandom(10);
                  if(time<1)time=1;
                  if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;

                  itemst *item;

                  switch(sitetype)
                  {
                     case SITE_RESIDENTIAL_TENEMENT:
                        item=new itemst;item->type=ITEM_LOOT;
                           item->loottype=LOOT_TV;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_RESIDENTIAL_APARTMENT:
                     case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
                           else if(!LCSrandom(4))item->loottype=LOOT_SILVERWARE;
                           else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
                           else if(!LCSrandom(2))item->loottype=LOOT_TV;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_LABORATORY_COSMETICS:
                     case SITE_INDUSTRY_NUCLEAR:
                     case SITE_LABORATORY_GENETIC:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(2))item->loottype=LOOT_LABEQUIPMENT;
                           else if(!LCSrandom(2))item->loottype=LOOT_TV;
                           else if(!LCSrandom(5))item->loottype=LOOT_SCANNER;
                           else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
                           else if(!LCSrandom(5))item->loottype=LOOT_CHEMICAL;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_GOVERNMENT_POLICESTATION:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(40))item->loottype=LOOT_POLICERECORDS;
                           else if(!LCSrandom(3))item->loottype=LOOT_TV;
                           else if(!LCSrandom(2))item->loottype=LOOT_PRINTER;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_GOVERNMENT_COURTHOUSE:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(2))item->loottype=LOOT_TV;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_GOVERNMENT_PRISON:
                        item=new itemst;item->type=ITEM_WEAPON;
                        item->weapon.type=WEAPON_SHANK;
                        item->weapon.ammo=0;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_GOVERNMENT_INTELLIGENCEHQ:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(3))item->loottype=LOOT_CELLPHONE;
                           else if(!LCSrandom(2))item->loottype=LOOT_TV;
                           else if(LCSrandom(49))item->loottype=LOOT_COMPUTER;
                           else item->loottype=LOOT_SECRETDOCUMENTS;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_GOVERNMENT_FIRESTATION:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(LCSrandom(2))item->loottype=LOOT_TV;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_INDUSTRY_SWEATSHOP:
                        item=new itemst;item->type=ITEM_LOOT;
                           item->loottype=LOOT_FINECLOTH;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_INDUSTRY_POLLUTER:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(4))item->loottype=LOOT_PRINTER;
                           else if(!LCSrandom(3))item->loottype=LOOT_TV;
                           else if(!LCSrandom(2))item->loottype=LOOT_CHEMICAL;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_CORPORATE_HEADQUARTERS:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(4))item->loottype=LOOT_PRINTER;
                           else if(!LCSrandom(3))item->loottype=LOOT_TV;
                           else if(!LCSrandom(2))item->loottype=LOOT_CELLPHONE;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_CORPORATE_HOUSE:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
                           else if(!LCSrandom(4))item->loottype=LOOT_SILVERWARE;
                           else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
                           else if(!LCSrandom(2))item->loottype=LOOT_TV;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_MEDIA_AMRADIO:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
                           else if(!LCSrandom(4))item->loottype=LOOT_BROADCASTINGEQUIPMENT;
                           else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
                           else if(!LCSrandom(2))item->loottype=LOOT_TV;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                     case SITE_MEDIA_CABLENEWS:
                        item=new itemst;item->type=ITEM_LOOT;
                           if(!LCSrandom(5))item->loottype=LOOT_CELLPHONE;
                           else if(!LCSrandom(4))item->loottype=LOOT_TVCAMERA;
                           else if(!LCSrandom(3))item->loottype=LOOT_PRINTER;
                           else if(!LCSrandom(2))item->loottype=LOOT_TV;
                           else item->loottype=LOOT_COMPUTER;
                        activesquad->loot.push_back(item);
                        break;
                  }
               }
               tookground=1;
            }

            //MAKE GROUND LOOT INTO MISSION LOOT
            for(int l=0;l<groundloot.size();l++)
            {
               activesquad->loot.push_back(groundloot[l]);
            }

            groundloot.clear();

            if(enemy&&sitealarm)enemyattack();
            else if(enemy)disguisecheck();

            if(tookground)
            {
               int maxsleightofhand=0;
               int beststealer=0;
               for(int i=0;i<6;i++)
               {
                  if(!activesquad->squad[i])
                  {
                     break;
                  }
                  if(activesquad->squad[i]->skillval(SKILL_THEFT)>maxsleightofhand)
                  {
                     beststealer=i;
                     maxsleightofhand=activesquad->squad[i]->skillval(SKILL_THEFT);
                  }
               }
               activesquad->squad[beststealer]->train(SKILL_THEFT,5);
               if(!LCSrandom(maxsleightofhand+1))
               {
                  alienationcheck(1);
                  noticecheck(-1);
                  sitecrime++;
                  sitestory->crime.push_back(CRIME_STOLEGROUND);
                  if(enemy)
                  {
                     criminalize(*(activesquad->squad[beststealer]),LAWFLAG_THEFT);
                  }
               }
            }

            creatureadvance();
         }

         int cbase=-1;
         if(activesquad->squad[0]!=NULL)
         {
            cbase=activesquad->squad[0]->base;
         }

         // Removed code that forces a turn to pass if you're
         // looking around your own home base, regardless of
         // what you do... not sure why that was even there
         // in the first place
         if(locx!=olocx||locy!=olocy||locz!=olocz||c=='s')
         {
            //NEED TO GO BACK TO OLD LOCATION IN CASE COMBAT
               //REFRESHES THE SCREEN
            long nlocx=locx;
            long nlocy=locy;
            long nlocz=locz;
            locx=olocx;
            locy=olocy;
            locz=olocz;

            //ENEMIES SHOULD GET FREE SHOTS NOW
            if(enemy&&sitealarm)enemyattack();
            else if(enemy)disguisecheck();

            creatureadvance();

            partyalive=0;
            for(p=0;p<6;p++)
            {
               if(activesquad->squad[p]==NULL)continue;
               if(activesquad->squad[p]->alive==1)partyalive++;
            }

            if(partyalive==0)continue;
            
            //AFTER DEATH CHECK CAN MOVE BACK TO NEW LOCATION
            locx=nlocx;
            locy=nlocy;
            locz=nlocz;
            
            //CHECK FOR EXIT
            if((levelmap[locx][locy][locz].flag & SITEBLOCK_EXIT)||
               (cbase==cursite&&!location[cursite]->siege.siege&&
               bail_on_base))
            {
               //RESET MODE PRIOR TO CHASE
               showcarprefs=0;

               //CHASE SEQUENCE OR FOOT CHASE
               chaseseq.clean();
               chaseseq.location=cursite;
               long level=sitecrime;
               if(sitealarm==0)level=0;
               if(LCSrandom(3)&&level<4)level=0;
               if(LCSrandom(2)&&level<8)level=0;
               if(postalarmtimer<10+(int)LCSrandom(20))level=0;
               else if(postalarmtimer<20+(int)LCSrandom(20)&&LCSrandom(3))level=0;
               else if(postalarmtimer<40+(int)LCSrandom(20)&&!LCSrandom(3))level=0;
               if(location[cursite]->siege.siege)level=1000;

               //MAKE SURE YOU ARE GUILTY OF SOMETHING
               char guilty=0;
               for(p=0;p<6;p++)
               {
                  if(activesquad->squad[p]!=NULL)
                  {
                     if(iscriminal(*activesquad->squad[p]))guilty=1;
                  }
               }
               if(!guilty)level=0;

               makechasers(sitetype,level);

               char havecar=0;
               for(p=0;p<6;p++)
               {
                  if(activesquad->squad[p]!=NULL)
                  {
                     if(activesquad->squad[p]->carid!=-1)
                     {
                        havecar=1;
                        break;
                     }
                  }
               }
               char gotout;
               if(havecar)gotout=chasesequence();
               else gotout=footchase();

               //If you survived
               if(gotout)
               {
                  //Check for hauled prisoners/corpses
                  for(p=0;p<6;p++)
                  {
                     if(activesquad->squad[p]==NULL)continue;
                     if(activesquad->squad[p]->prisoner!=NULL)
                     {
                        //If this is an LCS member or corpse being hauled (marked as in the squad)
                        if(activesquad->squad[p]->prisoner->squadid!=-1)
                        {
                           //Take them out of the squad
                           activesquad->squad[p]->prisoner->squadid=-1;
                           //Set base and current location to squad's safehouse
                           activesquad->squad[p]->prisoner->location=activesquad->squad[p]->base;
                           activesquad->squad[p]->prisoner->base=activesquad->squad[p]->base;
                        }
                        else //A kidnapped conservative
                        {
                           //Convert them into a prisoner
                           kidnaptransfer(*activesquad->squad[p]->prisoner);
                           delete activesquad->squad[p]->prisoner;
                        }
                        activesquad->squad[p]->prisoner=NULL;
                     }
                  }
                  //Clear all bleeding and prison escape flags
                  for(p=0;p<pool.size();p++)
                  {
                     pool[p]->flag&=~CREATUREFLAG_JUSTESCAPED;
                     for(int w=0;w<BODYPARTNUM;w++)
                     {
                        pool[p]->wound[w]&=~WOUND_BLEEDING;
                     }
                  }

                  //END SITE MODE
                  if(location[cursite]->siege.siege)
                  {
                     //Special handling for escaping siege
                     escapesiege(0);
                  }
                  else
                  {
                     resolvesite();
                  }
               }
               //You didn't survive -- handle squad death (unless that ended the game)
               else if(!endcheck())
               {
                  
                  if(location[cursite]->siege.siege)
                  {
                     //Report on squad killed during siege
                     if(location[cursite]->siege.underattack)sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEATTACK;
                     else sitestory->type=NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE;

                     location[cursite]->siege.siege=0;
                  }
                  else 
                  {
                     //Or report on your failed raid
                     sitestory->type=NEWSSTORY_SQUAD_KILLED_SITE;
                     //Would juice the party here, but you're all dead, so...
                     resolvesite();
                  }
               }

               mode=GAMEMODE_BASE;
               return;
            }

            //SEE IF THERE IS AN ENCOUNTER
            char newenc=0;

            if(!location[cursite]->siege.siege&&!LCSrandom(10))
            {
               newenc=1;
            }

            for(int e=0;e<ENCMAX;e++)
            {
               if(encounter[e].exists)newenc=0;
            }

            //LOOK FOR SPECIALS
            long makespecial=-1;
            switch(levelmap[locx][locy][locz].special)
            {
               case SPECIAL_CLUB_BOUNCER:
               case SPECIAL_CLUB_BOUNCER_SECONDVISIT:
               case SPECIAL_APARTMENT_LANDLORD:
               case SPECIAL_RESTAURANT_TABLE:
               case SPECIAL_CAFE_COMPUTER:
               case SPECIAL_PARK_BENCH:
                  makespecial=levelmap[locx][locy][locz].special;
                  newenc=1;
                  break;
            }

            //DO DOORS
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_DOOR)
            {
               if((levelmap[locx][locy][locz].flag & SITEBLOCK_LOCKED) &&
                  !(levelmap[locx][locy][locz].flag & SITEBLOCK_CLOCK))
               {
                  levelmap[locx][locy][locz].flag|=SITEBLOCK_KLOCK;

                  do
                  {
                     clearmessagearea(false);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr("You try the door, but it is locked.");
                     move(17,1);
                     addstr("Try to pick the lock? (Yes or No)");

                     refresh();

                     int c=getch();
                     translategetch(c);

                     if(c=='y')
                     {
                        char actual; // 1 if an actual attempt was made, 0 otherwise
                        
                        // If the unlock was successful
                        if(unlock(UNLOCK_DOOR,actual))
                        {
                           // Unlock the door
                           levelmap[locx][locy][locz].flag&=~SITEBLOCK_LOCKED;
                           sitestory->crime.push_back(CRIME_UNLOCKEDDOOR);
                           //criminalizeparty(LAWFLAG_BREAKING);
                        }
                        // Else perma-lock it if an attempt was made
                        else if(actual)levelmap[locx][locy][locz].flag|=SITEBLOCK_CLOCK;

                        // Check for people noticing you fiddling with the lock
                        if(actual)
                        {
                           alienationcheck(1);
                           noticecheck(-1);
                        }

                        break;
                     }
                     else if(c=='n')break;

                  }while(1);
               }
               else if(levelmap[locx][locy][locz].flag & SITEBLOCK_CLOCK)
               {
                  do
                  {
                     clearmessagearea(false);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr("You shake the handle but it is still locked.");
                     move(17,1);
                     addstr("Force it open? (Yes or No)");

                     refresh();
                     int c=getch();
                     translategetch(c);

                     if(c=='y')
                     {
                        char actual;

                        if(bash(BASH_DOOR,actual))
                        {
                           levelmap[locx][locy][locz].flag&=~SITEBLOCK_DOOR;
                           int time=20+LCSrandom(10);
                           if(time<1)time=1;
                           if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
                           sitecrime++;
                           sitestory->crime.push_back(CRIME_BROKEDOWNDOOR);
                           criminalizeparty(LAWFLAG_BREAKING);
                        }

                        if(actual)
                        {
                           alienationcheck(1);
                           noticecheck(-1);
                        }

                        break;
                     }
                     else if(c=='n')break;

                  }while(1);
               }
               else
               {
                  levelmap[locx][locy][locz].flag&=~SITEBLOCK_DOOR;
               }

               locx=olocx;
               locy=olocy;
               locz=olocz;
               if(encsize>0)newenc=0;
            }

            if(location[cursite]->siege.siege||postalarmtimer>=100) // *JDS* police response added
            {
               if(locx!=olocx||locy!=olocy||locz!=olocz)
               {
                  for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
               }

               //MOVE SIEGE UNITS AROUND
                  //MOVE UNITS
               vector<int> unitx;
               vector<int> unity;
               vector<int> unitz;

               for(x=0;x<MAPX;x++)
               {
                  for(int y=0;y<MAPY;y++)
                  {
                     for(int z=0;z<MAPZ;z++)
                     {
                        if(levelmap[x][y][z].siegeflag & SIEGEFLAG_UNIT)
                        {
                           unitx.push_back(x);
                           unity.push_back(y);
                           unitz.push_back(z);
                        }
                     }
                  }
               }

               int sx=0,sy=0,sz=0;
               for(u=0;u<unitx.size();u++)
               {
                  // don't leave tile if player is here
                  if(unitx[u]==locx&&
                     unity[u]==locy&&
                     unitz[u]==locz)
                  {
                     continue;
                  }
                  // move into player's tile if possible
                  if((unitx[u]==locx-1||
                      unitx[u]==locx+1)&&
                      unity[u]==locy&&
                      unitz[u]==locz)
                  {
                     levelmap[unitx[u]][unity[u]][unitz[u]].siegeflag&=~SIEGEFLAG_UNIT;

                     //Get torched
                     if(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK)
                     {
                        levelmap[locx][locy][locz].siegeflag|=SIEGEFLAG_UNIT_DAMAGED;
                     }

                     //BLOW TRAPS
                     if(levelmap[locx][locy][locz].siegeflag & SIEGEFLAG_TRAP)
                     {
                        levelmap[locx][locy][locz].siegeflag&=~SIEGEFLAG_TRAP;
                        levelmap[locx][locy][locz].siegeflag|=SIEGEFLAG_UNIT_DAMAGED;
                     }
                     else
                     {
                        levelmap[locx][locy][locz].siegeflag|=SIEGEFLAG_UNIT;
                     }
                     continue;
                  }
                  if(unitx[u]==locx&&
                     (unity[u]==locy+1||
                      unity[u]==locy-1)&&
                     unitz[u]==locz)
                  {
                     levelmap[unitx[u]][unity[u]][unitz[u]].siegeflag&=~SIEGEFLAG_UNIT;

                     //BLOW TRAPS
                     if(levelmap[locx][locy][locz].siegeflag & SIEGEFLAG_TRAP)
                     {
                        levelmap[locx][locy][locz].siegeflag&=~SIEGEFLAG_TRAP;
                        levelmap[locx][locy][locz].siegeflag|=SIEGEFLAG_UNIT_DAMAGED;
                     }
                     else
                     {
                        levelmap[locx][locy][locz].siegeflag|=SIEGEFLAG_UNIT;
                     }
                     continue;
                  }
                  sz=0;
                  switch(LCSrandom(4))
                  {
                     case 0:sx=-1;sy=0;
                        break;
                     case 1:sx=1;sy=0;
                        break;
                     case 2:sx=0;sy=1;
                        break;
                     case 3:sx=0;sy=-1;
                        break;
                  }
                  sx=unitx[u]+sx;
                  sy=unity[u]+sy;
                  sz=unitz[u]+sz;

                  if(sx>=0&&sx<MAPX&&sy>=0&&sy<MAPY&&sz>=0&&sz<MAPZ)
                  {
                     if(!(levelmap[sx][sy][sz].flag & SITEBLOCK_BLOCK))
                     {
                        if((levelmap[sx][sy][sz].flag & SITEBLOCK_DOOR))
                        {
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_DOOR;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_LOCKED;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_KLOCK;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_CLOCK;
                        }
                        else
                        {
                           char conf=1;

                           //BLOCK PASSAGE
                           if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_UNIT)conf=0;
                           if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_HEAVYUNIT)conf=0;

                           if(conf)
                           {
                              levelmap[unitx[u]][unity[u]][unitz[u]].siegeflag&=~SIEGEFLAG_UNIT;

                              //BLOW TRAPS
                              if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_TRAP)
                              {
                                 levelmap[sx][sy][sz].siegeflag&=~SIEGEFLAG_TRAP;
                                 levelmap[sx][sy][sz].siegeflag|=SIEGEFLAG_UNIT_DAMAGED;
                              }
                              else
                              {
                                 levelmap[sx][sy][sz].siegeflag|=SIEGEFLAG_UNIT;
                              }
                           }
                        }
                     }
                  }
               }

               unitx.clear();
               unity.clear();
               unitz.clear();

                  //MOVE HEAVY UNITS
               for(x=0;x<MAPX;x++)
               {
                  for(int y=0;y<MAPY;y++)
                  {
                     for(int z=0;z<MAPZ;z++)
                     {
                        if(levelmap[x][y][z].siegeflag & SIEGEFLAG_HEAVYUNIT)
                        {
                           unitx.push_back(x);
                           unity.push_back(y);
                           unitz.push_back(z);
                        }
                     }
                  }
               }

               for(u=0;u<unitx.size();u++)
               {
                  sz=0;
                  switch(LCSrandom(4))
                  {
                     case 0:sx=-1;sy=0;
                        break;
                     case 1:sx=1;sy=0;
                        break;
                     case 2:sx=0;sy=1;
                        break;
                     case 3:sx=0;sy=-1;
                        break;
                  }
                  sx=unitx[u]+sx;
                  sy=unity[u]+sy;
                  sz=unitz[u]+sz;

                  if(sx>=0&&sx<MAPX&&sy>=0&&sy<MAPY&&sz>=0&&sz<MAPZ)
                  {
                     if(!(levelmap[sx][sy][sz].flag & SITEBLOCK_BLOCK))
                     {
                        if((levelmap[sx][sy][sz].flag & SITEBLOCK_DOOR))
                        {
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_DOOR;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_LOCKED;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_KLOCK;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_CLOCK;
                        }
                        else
                        {
                           char conf=1;

                           //BLOCK PASSAGE
                           if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_UNIT)conf=0;
                           if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_HEAVYUNIT)conf=0;

                           if(conf)
                           {
                              levelmap[unitx[u]][unity[u]][unitz[u]].siegeflag&=~SIEGEFLAG_HEAVYUNIT;
                              levelmap[sx][sy][sz].siegeflag|=SIEGEFLAG_HEAVYUNIT;

                              //BLOW (DIFFUSE) TRAPS
                              if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_TRAP)
                              {
                                 levelmap[sx][sy][sz].siegeflag&=~SIEGEFLAG_TRAP;
                              }
                           }
                        }
                     }
                     else
                     {
                        //BREAK WALLS
                        if(sy>=3&&sx>0&&sx<MAPX-1&&sy<MAPY-1)
                        {
                           sitechangest change(sx,sy,sz,SITEBLOCK_DEBRIS);
                           location[cursite]->changes.push_back(change);
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_BLOCK;
                           levelmap[sx][sy][sz].flag|=SITEBLOCK_DEBRIS;
                        }
                     }
                  }
               }

               unitx.clear();
               unity.clear();
               unitz.clear();

               for(u=0;u<unitx.size();u++)
               {
                  sz=0;
                  switch(LCSrandom(4))
                  {
                     case 0:sx=-1;sy=0;
                        break;
                     case 1:sx=1;sy=0;
                        break;
                     case 2:sx=0;sy=1;
                        break;
                     case 3:sx=0;sy=-1;
                        break;
                  }
                  sx=unitx[u]+sx;
                  sy=unity[u]+sy;
                  sz=unitz[u]+sz;

                  if(sx>=0&&sx<MAPX&&sy>=0&&sy<MAPY&&sz>=0&&sz<MAPZ)
                  {
                     if(!(levelmap[sx][sy][sz].flag & SITEBLOCK_BLOCK))
                     {
                        if((levelmap[sx][sy][sz].flag & SITEBLOCK_DOOR))
                        {
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_DOOR;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_LOCKED;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_KLOCK;
                           levelmap[sx][sy][sz].flag&=~SITEBLOCK_CLOCK;
                        }
                        else
                        {
                           char conf=1;

                           //BLOCK PASSAGE
                           if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_UNIT)conf=0;
                           if(levelmap[sx][sy][sz].siegeflag & SIEGEFLAG_HEAVYUNIT)conf=0;
                        }
                     }
                  }
               }

               unitx.clear();
               unity.clear();
               unitz.clear();

               //NEW WAVES
                  //IF THERE AREN'T ENOUGH UNITS AROUND
                     //AND THEY HAVEN'T BEEN SCARED OFF
                        //MORE WAVES WILL ATTACK
                           //AND IT GETS WORSE AND WORSE
               location[cursite]->siege.attacktime++;
               if(!(postalarmtimer % 100) || (location[cursite]->siege.attacktime>=100+LCSrandom(10)&&
                  (locz!=0||locx<(MAPX/2-3)||locx>(MAPX/2+3)||
                  locy>5)))
               {
                  location[cursite]->siege.attacktime=0;

                  //PLACE UNITS
                  int lx,ly,lz;
                  int unitnum=5;
                  if(location[cursite]->type==SITE_GOVERNMENT_POLICESTATION)unitnum=20;
                  int count=10000;
                  for(int t=0;t<unitnum;t++)
                  {
                     count=10000;
                     do
                     {
                        lx=LCSrandom(7)+(MAPX/2)-3;
                        ly=LCSrandom(5);
                        lz=0;
                        count--;
                        if(count==0)break;
                     }while((levelmap[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
                           (levelmap[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
                     levelmap[lx][ly][lz].siegeflag|=SIEGEFLAG_UNIT;
                  }

                  if(!(location[cursite]->compound_walls & COMPOUND_TANKTRAPS)&&
                     location[cursite]->siege.siegetype==SIEGE_POLICE&&
                     location[cursite]->siege.escalationstate>=2)
                  {
                     count=10000;
                     int hunitnum=1;
                     for(int t=0;t<hunitnum;t++)
                     {
                        do
                        {
                           lx=LCSrandom(7)+(MAPX/2)-3;
                           ly=LCSrandom(5);
                           lz=0;
                           count--;
                           if(count==0)break;
                        }while((levelmap[lx][ly][lz].flag & (SITEBLOCK_BLOCK|SITEBLOCK_DOOR|SITEBLOCK_EXIT))||
                              (levelmap[lx][ly][lz].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT|SIEGEFLAG_TRAP)));
                        levelmap[lx][ly][lz].siegeflag|=SIEGEFLAG_HEAVYUNIT;
                     }
                  }
               }

               //CHECK FOR SIEGE UNITS
                  //INCLUDING DAMAGED ONES
               if(levelmap[locx][locy][locz].siegeflag & SIEGEFLAG_UNIT)
               {
                  if(addsiegeencounter(SIEGEFLAG_UNIT))
                  {
                     levelmap[locx][locy][locz].siegeflag&=~SIEGEFLAG_UNIT;
                  }
               }
               if(levelmap[locx][locy][locz].siegeflag & SIEGEFLAG_HEAVYUNIT)
               {
                  if(addsiegeencounter(SIEGEFLAG_HEAVYUNIT))
                  {
                     levelmap[locx][locy][locz].siegeflag&=~SIEGEFLAG_HEAVYUNIT;
                  }
               }
               if(levelmap[locx][locy][locz].siegeflag & SIEGEFLAG_UNIT_DAMAGED)
               {
                  if(addsiegeencounter(SIEGEFLAG_UNIT_DAMAGED))
                  {
                     levelmap[locx][locy][locz].siegeflag&=~SIEGEFLAG_UNIT_DAMAGED;
                  }
               }

               //BAIL UPON VICTORY
               if(location[cursite]->siege.kills>=25&&location[cursite]->siege.siege)
               {
                  if(location[cursite]->siege.underattack)sitestory->type=NEWSSTORY_SQUAD_DEFENDED;
                  else sitestory->type=NEWSSTORY_SQUAD_BROKESIEGE;

                  if(location[cursite]->siege.siegetype==SIEGE_CCS)
                  {
                     if(location[cursite]->type==SITE_INDUSTRY_WAREHOUSE)
                        location[cursite]->renting=0; // CCS DOES NOT capture the warehouse -- reverse earlier assumption of your defeat!
                  }

                  //DEAL WITH PRISONERS AND STOP BLEEDING
                  for(p=0;p<6;p++)
                  {
                     if(activesquad->squad[p]==NULL)continue;
                     if(activesquad->squad[p]->prisoner!=NULL)
                     {
                        if(activesquad->squad[p]->prisoner->squadid!=-1)
                        {
                           //RESTORE POOL MEMBER
                           activesquad->squad[p]->prisoner->squadid=-1;
                           //MUST LOCATE THE MEMBER
                           activesquad->squad[p]->prisoner->location=activesquad->squad[p]->base;
                           activesquad->squad[p]->prisoner->base=activesquad->squad[p]->base;
                        }
                        else
                        {
                           //CONVERT KIDNAP VICTIM
                           kidnaptransfer(*activesquad->squad[p]->prisoner);
                           delete activesquad->squad[p]->prisoner;
                        }
                        activesquad->squad[p]->prisoner=NULL;
                     }
                  }
                  for(p=0;p<pool.size();p++)
                  {
                     pool[p]->flag&=~CREATUREFLAG_JUSTESCAPED;
                     for(int w=0;w<BODYPARTNUM;w++)
                     {
                        pool[p]->wound[w]&=~WOUND_BLEEDING;
                     }
                  }

                  //INFORM
                  clearmessagearea();

                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(16,1);
                  addstr("The Conservatives have shrunk back under");
                  move(17,1);
                  addstr("the power of your Liberal Convictions!");

                  refresh();
                  getch();

                  conquertext();
                  escapesiege(1);

                  //RESET MODE
                  mode=GAMEMODE_BASE;
                  showcarprefs=0;
                  return;
               }
            }
            
            if(!location[cursite]->siege.siege&&newenc)
            {
               switch(makespecial)
               {
                  case SPECIAL_CAFE_COMPUTER:
                     if(sitealarm||sitealienate)
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The computer has been disconnected.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();
                     }
                     else
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The computer is occupied.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();

                        prepareencounter(sitetype,0);
                        for(int e=1;e<ENCMAX;e++)encounter[e].exists=0;
                     }
                     break;
                  case SPECIAL_RESTAURANT_TABLE:
                     if(sitealarm||sitealienate)
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The table has been abandoned.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();
                     }
                     else
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The table is occupied.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();

                        prepareencounter(sitetype,0);
                     }
                     break;
                  case SPECIAL_PARK_BENCH:
                     if(sitealarm||sitealienate)
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The bench is empty.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();
                     }
                     else
                     {
                        clearmessagearea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The there are people sitting here.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();

                        prepareencounter(sitetype,0);
                     }
                     break;
                  case SPECIAL_CLUB_BOUNCER:
                     special_bouncer_assess_squad();
                     break;
                  case SPECIAL_CLUB_BOUNCER_SECONDVISIT:
                     special_bouncer_greet_squad();
                     break;
                  case SPECIAL_APARTMENT_LANDLORD:
                     if(sitealarm||sitealienate||
                        location[cursite]->siege.siege)
                     {
                        clearmessagearea(false);
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The landlord is out of the office.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();
                     }
                     else
                     {
                        clearmessagearea(false);
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        addstr("The landlord is in.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();

                        for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
                        makecreature(encounter[0],CREATURE_LANDLORD);
                     }
                     break;
                  default:
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr("There is someone up ahead.      ");
                     refresh();
                     getch();
                     prepareencounter(sitetype,location[cursite]->highsecurity);
                     break;
               }
               hostcheck=1;
            }

            if(!location[cursite]->siege.siege)
            {
               if((locx!=olocx||locy!=olocy||locz!=olocz)&&!newenc)
               {
                  //PUT BACK SPECIALS
                  for(int e=0;e<ENCMAX;e++)
                  {
                     if(encounter[e].exists)
                     {
                        if(!encounter[e].cantbluff&&
                           encounter[e].type==CREATURE_LANDLORD)
                        {
                           levelmap[olocx][olocy][olocz].special=SPECIAL_APARTMENT_LANDLORD;
                        }
                        encounter[e].exists=0;
                     }
                  }
               }
            }

            if(locx!=olocx||locy!=olocy||locz!=olocz)
            {
               //NUKE GROUND LOOT
               for(int l=0;l<groundloot.size();l++)
               {
                  delete groundloot[l];
               }

               groundloot.clear();

               //MOVE BLOOD
               if(levelmap[olocx][olocy][olocz].flag & SITEBLOCK_BLOODY2)
               {
                  levelmap[locx][locy][locz].flag|=SITEBLOCK_BLOODY;
               }
            }

            knowmap(locx,locy,locz);
         }
      }
      
   }while(1);
}



/* site - determines spin on site news story, "too hot" timer */
void resolvesite(void)
{
   if(sitealienate)sitestory->positive=0;
   if(sitealarm==1&&sitecrime>100)
   {
      location[cursite]->closed=sitecrime/10;
      
      // Out sleepers
      for(int p=0;p<pool.size();p++)
      {
         if(pool[p]->flag & CREATUREFLAG_SLEEPER &&
            pool[p]->location == cursite)
         {
            pool[p]->flag &= ~CREATUREFLAG_SLEEPER;
            erase();
            move(8,1);
            addstr("Sleeper ");
            addstr(pool[p]->name);
            addstr(" has been outed by your bold attack!");

            move(10,1);
            addstr("The Liberal is now at your command as a normal squad member.");

            pool[p]->base=activesquad->squad[0]->base;
            pool[p]->location=pool[p]->base;
            refresh();
            getch();
         }
      }

      // CCS Safehouse killed?
      if(location[cursite]->type==SITE_RESIDENTIAL_BOMBSHELTER||
         location[cursite]->type==SITE_BUSINESS_BARANDGRILL||
         location[cursite]->type==SITE_OUTDOOR_BUNKER)
      {
         //location[cursite]->hidden=1;  // Either re-hide the location...
         location[cursite]->renting=0; // ...OR convert it to an LCS safehouse
         location[cursite]->closed=0;  // one of the above two should be commented out
         location[cursite]->heat=100;
         ccs_kills++;
         if(ccs_kills<3)
            endgamestate--;
         else
            endgamestate=ENDGAME_CCS_DEFEATED;

         // Move any CCS Sleepers at this location back to the homeless shelter

         
      }
      // Capture a warehouse or crack den?
      else if(location[cursite]->type==SITE_INDUSTRY_WAREHOUSE||
              location[cursite]->type==SITE_BUSINESS_CRACKHOUSE)
      {
         location[cursite]->renting=0; // Capture safehouse for the glory of the LCS!
         location[cursite]->closed=0;
         location[cursite]->heat=100;
      }
   }
   else if(sitealarm==1&&sitecrime>10&&location[cursite]->renting<=-1)
   {
      if(!(location[cursite]->type==SITE_RESIDENTIAL_BOMBSHELTER)&&
         !(location[cursite]->type==SITE_BUSINESS_BARANDGRILL)&&
         !(location[cursite]->type==SITE_OUTDOOR_BUNKER)&&
         !(location[cursite]->type==SITE_INDUSTRY_WAREHOUSE)&&
         !(location[cursite]->type==SITE_BUSINESS_CRACKHOUSE))
      {
         if(securityable(location[cursite]->type))
            location[cursite]->highsecurity=sitecrime;
         else
            location[cursite]->closed=7;
      }
   }

   if(location[cursite]->closed)
   {
      if(location[cursite]->type==SITE_MEDIA_AMRADIO)
      {
         //AM Radio less effective if brought offline
         change_public_opinion(VIEW_AMRADIO,10);
         amradio_closed=1;
      }
      if(location[cursite]->type==SITE_GOVERNMENT_POLICESTATION)
      {
         //People generally want to give police more power if they
         //get closed down
         change_public_opinion(VIEW_POLICEBEHAVIOR,-10);
         policestation_closed=1;
      }
      if(location[cursite]->type==SITE_MEDIA_CABLENEWS)
      {
         //Cable News less influential if brought offline
         change_public_opinion(VIEW_CABLENEWS,10);
         cablenews_closed=1;
      }
   }
}


