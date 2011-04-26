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

void fight_subdued(void)
{
   int p;
   int ps=-1;
   for(int l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
      {
         ps=l;
      }
   }

   for(int v=0;v<chaseseq.friendcar.size();v++)
   {
      for(int v2=(int)vehicle.size()-1;v2>=0;v2--)
      {
         if(vehicle[v2]==chaseseq.friendcar[v])
         {
            delete vehicle[v2];
            vehicle.erase(vehicle.begin() + v2);
         }
      }
   }
   chaseseq.friendcar.clear();
   int hostagefreed=0;
   int stolen=0;
   // Police assess stolen goods in inventory
   for(int l=0;l<activesquad->loot.size();l++)
   {
      if(activesquad->loot[l]->is_loot())
         stolen++;
   }
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      activesquad->squad[p]->crimes_suspected[LAWFLAG_THEFT]+=stolen;
      capturecreature(*(activesquad->squad[p]));
      activesquad->squad[p]=NULL;
   }
   for(p=0;p<pool.size();p++)
   {
      for(int w=0;w<BODYPARTNUM;w++)
      {
         pool[p]->wound[w]&=~WOUND_BLEEDING;
      }
   }

   clearmessagearea();
   clearcommandarea();
   set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   move(16,1);
   addstr("The police subdue and arrest the squad.");
   if(hostagefreed>0)
   {
      move(17,1);
      addstr("Your hostage");
      if(hostagefreed>1)
         addstr("s are free.");
      else
         addstr(" is free.");
   }
   refresh();
   getch();
}

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
      ccs_siege_kills=0;

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
      location[loc]->siege.tanks=0;

      //PLACE YOU
      int maxy=0;

      for(int x=0;x<MAPX;x++)
      {
         for(int y=0;y<MAPY;y++)
         {
            for(int z=0;z<MAPZ;z++)
            {
               if(!(location[loc]->siege.lights_off))levelmap[x][y][z].flag|=SITEBLOCK_KNOWN;
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
         // Some bugs with degenarate spawn outside the map are occurring
         // Unknown why, but hard-coding limits to spawn location should help
         //locx=LCSrandom(MAPX);
         //locy=maxy-LCSrandom(3);
         locx = MAPX/2+LCSrandom(25)-12;
         locy = 15-LCSrandom(3);
         //if(locy<3)locy=3;
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
      int unitnum=6;
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
         levelmap[MAPX/2][1][0].siegeflag|=SIEGEFLAG_HEAVYUNIT;
         location[loc]->siege.tanks=1;
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

   int encounter_timer=0;

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

      // Let the squad stop stressing out over the encounter if there are no enemies this round
      if(!enemy) encounter_timer=0;

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
         if (postalarmtimer>80)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(postalarmtimer>60)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(0,0);
         addlocationname(location[cursite]);
         addstr(", Level ");
         char num[20];
         itoa(locz+1,num,10);
         addstr(num);

         if(postalarmtimer>80)
         {
            switch(location[cursite]->type)
            {
            case SITE_GOVERNMENT_ARMYBASE:
               addstr(": SOLDIERS AND TANKS RESPONDING");
               break;
            case SITE_GOVERNMENT_INTELLIGENCEHQ:
               addstr(": AGENTS RESPONDING");
               break;
            case SITE_CORPORATE_HEADQUARTERS:
            case SITE_CORPORATE_HOUSE:
               addstr(": MERCENARIES RESPONDING");
               break;
            case SITE_MEDIA_AMRADIO:
            case SITE_MEDIA_CABLENEWS:
               addstr(": ANGRY MOB RESPONDING");
               break;
            case SITE_BUSINESS_CRACKHOUSE:
               addstr(": GANG MEMBERS RESPONDING");
               break;
            case SITE_GOVERNMENT_POLICESTATION:
            default:
               if(location[cursite]->renting==RENTING_CCS)
               {
                  addstr(": CCS VIGILANTIES RESPONDING");
               }
               else if(law[LAW_DEATHPENALTY]==-2&&
                  law[LAW_POLICEBEHAVIOR]==-2)addstr(": DEATH SQUADS RESPONDING");
               else addstr(": POLICE RESPONDING");
               break;
            }
         }
         else if(postalarmtimer>60)addstr(": CONSERVATIVE REINFORCEMENTS INCOMING");
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
                  else if(activesquad->squad[i]->get_weapon().can_graffiti())
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

         move(12,42);
         if(enemy&&sitealarm)
         {
            bool evade = false;
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            for(int e=0;e<ENCMAX;e++)
            {
               if(encounter[e].exists &&
                  encounter[e].alive  &&
                  encounter[e].cantbluff == 2)
               {
                  // You can't sneak past this person; they already know you're there
                  evade = true;
                  break;
               }
            }
            if(!evade)
               addstr("V - Sneak");
            else
               addstr("V - Run");
         }
         else
         {
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            addstr("V - Evade");
         }

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
                  !activesquad->squad[p]->is_armed())
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
      {
         if(location[cursite]->renting==RENTING_PERMANENT)
            levelmap[locx][locy][locz].special = SPECIAL_NONE;
         else
            c='s';
      }
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
                  pool[pl]->die();
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
                  case SPECIAL_ARMYBASE_ARMORY:special_armybase_armory();break;
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
                  if(activesquad->squad[i]->get_weapon().can_graffiti())
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
                           itoa(activesquad->squad[p]->get_attribute(ATTRIBUTE_CHARISMA,true)/2+
                                activesquad->squad[p]->get_skill(SKILL_PERSUASION),num,10);
                           addstr(num);
                           move(y,60);
                           itoa(activesquad->squad[p]->get_attribute(ATTRIBUTE_CHARISMA,true)/2+
                                activesquad->squad[p]->get_skill(SKILL_SEDUCTION),num,10);
                           addstr(num);
                           move(y,70);
                           itoa(activesquad->squad[p]->get_attribute(ATTRIBUTE_CHARISMA,true)/2+
                                activesquad->squad[p]->get_skill(SKILL_DISGUISE),num,10);
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
                        else if(enemy)disguisecheck(encounter_timer);
                        creatureadvance();
                        encounter_timer++;
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
            encounter_timer++;
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
            for(int i=0;i<ENCMAX;i++)
            {
               if(encounter[i].exists&&
                  encounter[i].blood>60&&
                  (encounter[i].type==CREATURE_COP))
               {
                  for(i=0;i<6;i++)
                  {
                     if(activesquad->squad[i] &&
                        activesquad->squad[i]->alive &&
                        activesquad->squad[i]->blood > 40)
                     {
                        break;
                     }
                  }
                  if(i==6)
                  {
                     fight_subdued();
                     return;
                  }
                  break;
               }
            }
            
            youattack();
            enemyattack();
            creatureadvance();
            encounter_timer++;
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
            else if(enemy)disguisecheck(encounter_timer);

            creatureadvance();
            encounter_timer++;
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

                  Item *it;
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

                  Item *item;

                  string newLootType;
                  string newWeaponType;
                  string newArmorType;

                  switch(sitetype)
                  {
                  case SITE_RESIDENTIAL_TENEMENT:
                     if(!LCSrandom(25))
                     {
                        string rndWeps[] = {"WEAPON_BASEBALLBAT", "WEAPON_CROWBAR", "WEAPON_KNIFE", "WEAPON_SHANK", 
                           "WEAPON_SYRINGE", "WEAPON_CHAIN", "WEAPON_GUITAR","WEAPON_SPRAYCAN"};
                        //make sure the number of types matches the random range...
                        newWeaponType=rndWeps[LCSrandom(8)];
                     }
                     else if(!LCSrandom(20))
                     {
                        string rndArmors[] = {"ARMOR_CHEAPDRESS", "ARMOR_CHEAPSUIT", "ARMOR_CLOTHES", "ARMOR_TRENCHCOAT", 
                           "ARMOR_WORKCLOTHES", "ARMOR_TOGA", "ARMOR_PRISONER"};
                        //make sure the number of types matches the random range...
                        newArmorType=rndArmors[LCSrandom(7)];
                     }
                     else if(!LCSrandom(3))newLootType="LOOT_KIDART";
                     else if(!LCSrandom(2))newLootType="LOOT_DIRTYSOCK";
                     else newLootType="LOOT_FAMILYPHOTO";
                     break;
                  case SITE_RESIDENTIAL_APARTMENT:
                     if(!LCSrandom(25))
                     {
                        string rndWeps[] = {"WEAPON_BASEBALLBAT", "WEAPON_KNIFE", "WEAPON_REVOLVER_38", 
                           "WEAPON_REVOLVER_44", "WEAPON_NIGHTSTICK", "WEAPON_GUITAR"};
                        newWeaponType=rndWeps[LCSrandom(6)];
                     }
                     else if(!LCSrandom(20))
                     {
                        string rndArmors[] = {"ARMOR_CHEAPDRESS", "ARMOR_CHEAPSUIT", "ARMOR_CLOTHES", "ARMOR_TRENCHCOAT", 
                           "ARMOR_WORKCLOTHES", "ARMOR_CLOWNSUIT", "ARMOR_ELEPHANTSUIT", "ARMOR_DONKEYSUIT"};
                        //make sure the number of types matches the random range...
                        newArmorType=rndArmors[LCSrandom(8)];
                     }
                     else if(!LCSrandom(5))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(4))newLootType="LOOT_SILVERWARE";
                     else if(!LCSrandom(3))newLootType="LOOT_TRINKET";
                     else if(!LCSrandom(2))newLootType="LOOT_CHEAPJEWELERY";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
                     if(!LCSrandom(30))
                     {
                        string rndWeps[] = {"WEAPON_BASEBALLBAT", "WEAPON_KNIFE", "WEAPON_DAISHO", "WEAPON_SHOTGUN_PUMP", 
                           "WEAPON_REVOLVER_44", "WEAPON_SEMIPISTOL_45", "WEAPON_SEMIRIFLE_AR15", "WEAPON_AUTORIFLE_M16"};
                        newWeaponType=rndWeps[LCSrandom(6 - law[LAW_GUNCONTROL])];
                     }
                     else if(!LCSrandom(20))
                     {
                        string rndArmors[] = {"ARMOR_EXPENSIVEDRESS", "ARMOR_BLACKDRESS", "ARMOR_EXPENSIVESUIT", "ARMOR_BLACKSUIT", 
                           "ARMOR_BONDAGEGEAR", "ARMOR_CIVILLIANARMOR", "ARMOR_BLACKROBE", "ARMOR_LABCOAT"};
                        //make sure the number of types matches the random range...
                        newArmorType=rndArmors[LCSrandom(8)];
                     }
                     else if(!LCSrandom(10))newLootType="LOOT_EXPENSIVEJEWELERY";
                     else if(!LCSrandom(5))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(4))newLootType="LOOT_SILVERWARE";
                     else if(!LCSrandom(3))newLootType="LOOT_PDA";
                     else if(!LCSrandom(2))newLootType="LOOT_CHEAPJEWELERY";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_LABORATORY_COSMETICS:
                  case SITE_INDUSTRY_NUCLEAR:
                  case SITE_LABORATORY_GENETIC:
                     if(!LCSrandom(20))newLootType="LOOT_RESEARCHFILES";
                     else if(!LCSrandom(2))newLootType="LOOT_LABEQUIPMENT";
                     else if(!LCSrandom(2))newLootType="LOOT_COMPUTER";
                     else if(!LCSrandom(5))newLootType="LOOT_PDA";
                     else if(!LCSrandom(5))newLootType="LOOT_CHEMICAL";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_GOVERNMENT_POLICESTATION:
                     if(!LCSrandom(25))
                     {
                        string rndWeps[] = {"WEAPON_NIGHTSTICK", "WEAPON_NIGHTSTICK", "WEAPON_SHOTGUN_PUMP", "WEAPON_SEMIPISTOL_9MM",
                           "WEAPON_SMG_MP5", "WEAPON_CARBINE_M4", "WEAPON_AUTORIFLE_M16", "WEAPON_AUTORIFLE_M16"};
                        newWeaponType=rndWeps[LCSrandom(4) + 2 - law[LAW_GUNCONTROL]];
                     }
                     else if(!LCSrandom(25))
                     {
                        string rndArmors[] = {"ARMOR_POLICEUNIFORM", "ARMOR_POLICEUNIFORM", "ARMOR_POLICEARMOR", "ARMOR_POLICEUNIFORM", 
                           "ARMOR_SWATARMOR", "ARMOR_POLICEUNIFORM", "ARMOR_POLICEARMOR", "ARMOR_DEATHSQUADUNIFORM"};
                        newArmorType=rndArmors[LCSrandom(4) + 2 - law[LAW_GUNCONTROL]];
                     }
                     else if(!LCSrandom(20))newLootType="LOOT_POLICERECORDS";
                     else if(!LCSrandom(3))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(2))newLootType="LOOT_PDA";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_GOVERNMENT_COURTHOUSE:
                     if(!LCSrandom(20))newLootType="LOOT_JUDGEFILES";
                     else if(!LCSrandom(3))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(2))newLootType="LOOT_PDA";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_GOVERNMENT_PRISON:
                     if(!LCSrandom(5))
                     {
                        newArmorType="ARMOR_PRISONER";
                     }
                     else
                     {
                        newWeaponType="WEAPON_SHANK";
                     }
                     break;
                  case SITE_GOVERNMENT_ARMYBASE:
                     if(!LCSrandom(3))
                     {
                        string rndWeps[] = {"WEAPON_SEMIPISTOL_9MM", "WEAPON_CARBINE_M4", "WEAPON_AUTORIFLE_M16"};
                        newWeaponType=rndWeps[LCSrandom(3)];
                     }
                     else if(!LCSrandom(2))
                     {
                        string rndArmors[] = {"ARMOR_ARMYARMOR"};
                        newArmorType=rndArmors[LCSrandom(1)];
                     }
                     else if(!LCSrandom(20))newLootType="LOOT_SECRETDOCUMENTS";
                     else if(!LCSrandom(3))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(2))newLootType="LOOT_WATCH";
                     else newLootType="LOOT_TRINKET";
                     break;
                  case SITE_GOVERNMENT_INTELLIGENCEHQ:
                     if(!LCSrandom(24))
                     {
                        string rndWeps[] = {"WEAPON_FLAMETHROWER", "WEAPON_SEMIPISTOL_45",
                           "WEAPON_SMG_MP5", "WEAPON_CARBINE_M4", "WEAPON_AUTORIFLE_M16"};
                        newWeaponType=rndWeps[LCSrandom(5)];
                     }
                     else if(!LCSrandom(30))
                     {
                        string rndArmors[] = {"ARMOR_HEAVYARMOR"};
                        newArmorType=rndArmors[LCSrandom(1)];
                     }
                     else if(!LCSrandom(20))newLootType="LOOT_SECRETDOCUMENTS";
                     else if(!LCSrandom(3))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(2))newLootType="LOOT_PDA";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_GOVERNMENT_FIRESTATION:
                     if(!LCSrandom(25)) newArmorType="ARMOR_BUNKERGEAR";
                     else if(LCSrandom(2))newLootType="LOOT_TRINKET";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_INDUSTRY_SWEATSHOP:
                     newLootType="LOOT_FINECLOTH";
                     break;
                  case SITE_INDUSTRY_POLLUTER:
                     newLootType="LOOT_CHEMICAL";
                     break;
                  case SITE_CORPORATE_HEADQUARTERS:
                     if(!LCSrandom(50))newLootType="LOOT_CORPFILES";
                     else if(!LCSrandom(3))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(2))newLootType="LOOT_PDA";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_CORPORATE_HOUSE:
                     if(!LCSrandom(50))
                     {
                        string rndArmors[] = {"ARMOR_EXPENSIVEDRESS", "ARMOR_EXPENSIVESUIT", "ARMOR_EXPENSIVESUIT", 
                           "ARMOR_EXPENSIVESUIT", "ARMOR_BONDAGEGEAR"};
                        //make sure the number of types matches the random range...
                        newArmorType=rndArmors[LCSrandom(5)];
                     }
                     if(!LCSrandom(8))newLootType="LOOT_TRINKET";
                     else if(!LCSrandom(7))newLootType="LOOT_WATCH";
                     else if(!LCSrandom(6))newLootType="LOOT_PDA";
                     else if(!LCSrandom(5))newLootType="LOOT_CELLPHONE";
                     else if(!LCSrandom(4))newLootType="LOOT_SILVERWARE";
                     else if(!LCSrandom(3))newLootType="LOOT_CHEAPJEWELERY";
                     else if(!LCSrandom(2))newLootType="LOOT_FAMILYPHOTO";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_MEDIA_AMRADIO:
                     if(!LCSrandom(20))newLootType="LOOT_AMRADIOFILES";
                     else if(!LCSrandom(4))newLootType="LOOT_MICROPHONE";
                     else if(!LCSrandom(3))newLootType="LOOT_PDA";
                     else if(!LCSrandom(2))newLootType="LOOT_CELLPHONE";
                     else newLootType="LOOT_COMPUTER";
                     break;
                  case SITE_MEDIA_CABLENEWS:
                     if(!LCSrandom(20))newLootType="LOOT_CABLENEWSFILES";
                     else if(!LCSrandom(4))newLootType="LOOT_MICROPHONE";
                     else if(!LCSrandom(3))newLootType="LOOT_PDA";
                     else if(!LCSrandom(2))newLootType="LOOT_CELLPHONE";
                     else newLootType="LOOT_COMPUTER";
                     break;
                case SITE_BUSINESS_BARANDGRILL:
                case SITE_OUTDOOR_BUNKER:
                case SITE_RESIDENTIAL_BOMBSHELTER:
                   //storming a CCS stronghold. Logically you ought to get all the leftover stuff if you win...
                   string rndWeps[] = {"WEAPON_SEMIPISTOL_9MM", "WEAPON_SEMIPISTOL_45", "WEAPON_REVOLVER_38", "WEAPON_REVOLVER_44",
                                       "WEAPON_SMG_MP5", "WEAPON_CARBINE_M4", "WEAPON_AUTORIFLE_M16"};
                   string rndArmors[] = {"ARMOR_CHEAPSUIT", "ARMOR_CLOTHES", "ARMOR_TRENCHCOAT", "ARMOR_WORKCLOTHES", 
                                         "ARMOR_SECURITYUNIFORM", "ARMOR_CIVILLIANARMOR", "ARMOR_ARMYARMOR", "ARMOR_HEAVYARMOR"};
                   switch (LCSrandom(3))
                   {
                   case 0:
                      newWeaponType=rndWeps[LCSrandom(7)];
                      break;
                   case 1:
                      newArmorType=rndArmors[LCSrandom(8)];
                      break;
                   default:
                      if(!LCSrandom(5))newLootType="LOOT_CELLPHONE";
                      else if(!LCSrandom(4))newLootType="LOOT_SILVERWARE";
                      else if(!LCSrandom(3))newLootType="LOOT_TRINKET";
                      else if(!LCSrandom(2))newLootType="LOOT_CHEAPJEWELERY";
                      else newLootType="LOOT_COMPUTER";
                      break;
                   }
                   break;
               }
               item = NULL;
               if (!newLootType.empty())
               {
                  item=new Loot(*loottype[getloottype(newLootType)]);
                  activesquad->loot.push_back(item);
               }
               if (!newArmorType.empty())
               {
                  int quality = 1;
                  if (!LCSrandom(3))
                  {
                     quality=2;
                  }
                  Armor *a=new Armor(*armortype[getarmortype(newArmorType)],quality);
                  if (!LCSrandom(3))
                  {
                     a->set_damaged(true);
                  }
                  item = a;
                  activesquad->loot.push_back(item);
               }
              
              if (!newWeaponType.empty())
              {
               Weapon *w=new Weapon(*weapontype[getweapontype(newWeaponType)]);
               if (w->uses_ammo())
               {
                  if (LCSrandom(2) || //50% chance of being loaded...
                     //except for the most exotic weapons, which are always loaded.
                     w->get_itemtypename() == "WEAPON_DESERT_EAGLE" ||
                     w->get_itemtypename() == "WEAPON_FLAMETHROWER") //Make weapon property? -XML
                  {
                     vector<int> cti;
                     for(int ct=0; ct<cliptype.size(); ++ct)
                     {
                        if(w->acceptable_ammo(*cliptype[ct]))
                           cti.push_back(ct);
                     }
                     Clip c(*cliptype[cti[LCSrandom(cti.size())]]);
                     w->reload(c);
                  }
               }
               item = w;
               activesquad->loot.push_back(item);
              }
              char str[200];
              if (item)
              {
                 string s = item->equip_title();
                 clearmessagearea();
                 set_color(COLOR_WHITE,COLOR_BLACK,0);
                 move(16,1);
                 addstr("You find: ");
                 move(17,1);
                 addstr(s.c_str());
                 getch(); //wait for key press before clearing.
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
            else if(enemy)disguisecheck(encounter_timer);

            if(tookground)
            {
               int maxsleightofhand=0;
               int beststealer=0;
               juiceparty(1,200);
               alienationcheck(0);
               noticecheck(-1);
               sitecrime++;
               sitestory->crime.push_back(CRIME_STOLEGROUND);
               if(enemy)
               {
                  criminalize(*(activesquad->squad[beststealer]),LAWFLAG_THEFT);
               }
            }

            creatureadvance();
            encounter_timer++;
         }

         int cbase=-1;
         if(activesquad->squad[0]!=NULL)
         {
            cbase=activesquad->squad[0]->base;
         }


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
            if(enemy&&sitealarm)
            {
               bool snuck_away = true;
               int e;
               // Try to sneak past
               for(e=0;e<ENCMAX;e++)
               {
                  if(encounter[e].exists &&
                     encounter[e].alive  &&
                     encounter[e].cantbluff == 2)
                  {
                     // You can't sneak past this person; they already know you're there
                     break;
                  }
               }

               // If you can sneak past all enemies
               if(e==ENCMAX)
               {
                  int breakout=false;
                  for(e=0;e<ENCMAX;e++)
                  {
                     if(!encounter[e].exists)continue;
                     for(int i=0;i<6;i++)
                     {
                        if(activesquad->squad[i])
                        {
                           if(!(activesquad->squad[i]->skill_check(SKILL_STEALTH,DIFFICULTY_HARD)))
                           {
                              breakout=true;
                              break;
                           }
                        }
                     }
                     if(breakout) break;
                  }
               }

               // If snuck past everyone
               if(e==ENCMAX)
               {
                  for(int i=0;i<6;i++)
                  {
                     if(activesquad->squad[i]!=NULL)
                     {
                        activesquad->squad[i]->train(SKILL_STEALTH,10);
                     }
                  }
                  clearmessagearea();
                  set_color(COLOR_CYAN,COLOR_BLACK,1);
                  move(16,1);
                  addstr("The squad sneaks past the conservatives!");
                  getch();
               }
               else
               {
                  enemyattack();
               }
            }
            else if(enemy)disguisecheck(encounter_timer);

            creatureadvance();
            encounter_timer++;

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
                        for(int i=0;i<6;i++)
                        {
                           if(activesquad->squad[i]!=NULL&&
                              activesquad->squad[i]->carid==-1)
                           {
                              activesquad->squad[i]->carid=activesquad->squad[p]->carid;
                           }
                        }
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

            // 10% chance of encounter normally
            // 20% chance of encounter after massive response
            // 0% chance of encounter during sieges
            if(!LCSrandom(5))
            {
               if(postalarmtimer>80||!LCSrandom(2))
               {
                  if(!location[cursite]->siege.siege)
                  {
                     newenc=1;
                  }
               }
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
               case SPECIAL_HOUSE_CEO:
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
               bool has_security=false;
               for(int i=0;i<6;i++)
               {
                  if(activesquad->squad[i] &&
                     activesquad->squad[i]->get_skill(SKILL_SECURITY) != 0)
                  {
                     has_security=true;
                     break;
                  }
               }

               if((levelmap[locx][locy][locz].flag & SITEBLOCK_LOCKED) &&
                  !(levelmap[locx][locy][locz].flag & SITEBLOCK_CLOCK) &&
                  has_security==true)
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
                           alienationcheck(0);
                           noticecheck(-1);
                        }

                        break;
                     }
                     else if(c=='n')break;

                  }while(1);
               }
               else if(levelmap[locx][locy][locz].flag & SITEBLOCK_LOCKED)
               {
                  do
                  {
                     clearmessagearea(false);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr("You shake the handle but it is ");
                     if(has_security==true)addstr("still ");
                     addstr("locked.");
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
                           noticecheck(-1,DIFFICULTY_HEROIC);
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

            //BAIL UPON VICTORY (version 2 -- defeated CCS safehouse)
            if(ccs_siege_kills>=12&&!location[cursite]->siege.siege&&location[cursite]->renting==RENTING_CCS)
            {
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
               addstr("The CCS has been broken!");

               refresh();
               getch();
               
               location[cursite]->renting=RENTING_PERMANENT;
               location[cursite]->closed=0;
               location[cursite]->heat=100;
                  
               // CCS Safehouse killed?
               if(location[cursite]->type==SITE_RESIDENTIAL_BOMBSHELTER||
                  location[cursite]->type==SITE_BUSINESS_BARANDGRILL||
                  location[cursite]->type==SITE_OUTDOOR_BUNKER)
               {
                  ccs_kills++;
                  if(ccs_kills<3)
                     endgamestate--;
                  else
                     endgamestate=ENDGAME_CCS_DEFEATED;
               }

               conquertextccs();

               //RESET MODE
               mode=GAMEMODE_BASE;
               showcarprefs=0;
               return;
            }

            if(location[cursite]->siege.siege) // *JDS* police response added
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
                  if((((unitx[u]==locx-1||unitx[u]==locx+1)&&
                        unity[u]==locy)||
                      ((unity[u]==locy-1||unity[u]==locy+1)&&
                        unitx[u]==locx))
                        &&unitz[u]==locz)
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

               /*
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
               // End Heavy Units
               */

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
                     //but not as bad as it used to get,
                     //since the extra waves are small
               location[cursite]->siege.attacktime++;
               if((location[cursite]->siege.attacktime>=100+LCSrandom(10)&&
                  (locz!=0||locx<(MAPX/2-3)||locx>(MAPX/2+3)||
                  locy>5)))
               {
                  location[cursite]->siege.attacktime=0;

                  int existingUnits = 0;
                  for(int x=0;x<MAPX;x++)
                  {
                     for(int y=0;y<MAPY;y++)
                     {
                        for(int z=0;z<MAPZ;z++)
                        {
                           if(levelmap[x][y][z].siegeflag & (SIEGEFLAG_UNIT|SIEGEFLAG_HEAVYUNIT))
                           {
                              existingUnits++;
                           }
                        }
                     }
                  }

                  //PLACE UNITS
                  int lx,ly,lz;
                  int unitnum=7-existingUnits;
                  if(unitnum<0)unitnum=0;

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
               if(location[cursite]->siege.kills>=10&&
                  location[cursite]->siege.tanks==0&&
                  location[cursite]->siege.siege)
               {
                  if(location[cursite]->siege.underattack)sitestory->type=NEWSSTORY_SQUAD_DEFENDED;
                  else sitestory->type=NEWSSTORY_SQUAD_BROKESIEGE;

                  if(location[cursite]->siege.siegetype==SIEGE_CCS)
                  {
                     // CCS DOES NOT capture the warehouse -- reverse earlier assumption of your defeat!
                     if(location[cursite]->type==SITE_INDUSTRY_WAREHOUSE || SITE_BUSINESS_CRACKHOUSE)
                        location[cursite]->renting=RENTING_PERMANENT;
                     else if(location[cursite]->type==SITE_RESIDENTIAL_TENEMENT)
                        location[cursite]->renting=200;
                     else if(location[cursite]->type==SITE_RESIDENTIAL_APARTMENT)
                        location[cursite]->renting=650;
                     else if(location[cursite]->type==SITE_RESIDENTIAL_APARTMENT_UPSCALE)
                        location[cursite]->renting=1500;
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
                        addstr("The computer has been unplugged.");
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
                        addstr("Some people are hiding under the table.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();

                        prepareencounter(sitetype,0);
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
                  case SPECIAL_HOUSE_CEO:
                     if((sitealarm||sitealienate||location[cursite]->siege.siege)&&
                        uniqueCreatures.CEO_state==UNIQUECREATURE_ALIVE)
                     {
                        clearmessagearea(false);
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(16,1);
                        if(law[LAW_FREESPEECH]!=ALIGN_ARCHCONSERVATIVE)
                           addstr("Damn! ");
                        else
                           addstr("[Rats!] ");
                        addstr("The CEO must have fled to a panic room.");
                        levelmap[locx][locy][locz].special=-1;
                        refresh();
                        getch();
                     }
                     else
                     {
                        switch(uniqueCreatures.CEO_state)
                        {
                        case UNIQUECREATURE_ALIVE:
                           clearmessagearea(false);
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           move(16,1);
                           addstr("The CEO is in his study.");
                           levelmap[locx][locy][locz].special=-1;
                           refresh();
                           getch();

                           for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;
                           encounter[0] = uniqueCreatures.CEO();
                           encounter[0].exists = true;
                           break;
                        case UNIQUECREATURE_DEAD:
                        case UNIQUECREATURE_LIBERAL:
                           clearmessagearea(false);
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           move(16,1);
                           addstr("The CEO's study lies empty.");
                           levelmap[locx][locy][locz].special=-1;
                           refresh();
                           getch();
                           break;
                        }
                        
                     }
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
                     if(location[cursite]->type==SITE_RESIDENTIAL_APARTMENT||
                        location[cursite]->type==SITE_RESIDENTIAL_TENEMENT ||
                        location[cursite]->type==SITE_RESIDENTIAL_APARTMENT_UPSCALE)
                     {
                        if(LCSrandom(3))break; // Rarely encounter someone in apartments. (Was LCSrandom(5), seemed too easy to burgle people all day.)
                     }
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
//removed the 'alarmed' requirement for high security buildings, on the principle that even if they didn't see you, they will presumably
//notice later on that all their stuff has been stolen or whatever.
   if(//sitealarm==1&&
      sitecrime>5+LCSrandom(95))//was 100 but that meant I could still steal everything from a building every day without anyone caring...
   {
      if(location[cursite]->renting==RENTING_NOCONTROL)
      {
         // Capture a warehouse or crack den?
         if(location[cursite]->type==SITE_INDUSTRY_WAREHOUSE||
            location[cursite]->type==SITE_BUSINESS_CRACKHOUSE)
         {
            location[cursite]->renting=RENTING_PERMANENT; // Capture safehouse for the glory of the LCS!
            location[cursite]->closed=0;
            location[cursite]->heat=100;
         }
         else
         {
            // Close down site
            location[cursite]->closed=sitecrime/10;
         }
      }

      // Out sleepers
      if(location[cursite]->renting==RENTING_CCS)
      {
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
      }
   }
   else if(((sitealarm==1&&sitecrime>10)||sitecrime>5+LCSrandom(95))&&
      (location[cursite]->renting==RENTING_NOCONTROL || location[cursite]->renting > 500))
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


