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



/* handles end of round stuff for everyone */
void creatureadvance(void)
{
   int e;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(!activesquad->squad[p]->alive)continue;

      advancecreature(*activesquad->squad[p]);
      if(activesquad->squad[p]->prisoner!=NULL)
      {
         advancecreature(*activesquad->squad[p]->prisoner);
         if(!activesquad->squad[p]->prisoner->alive)
         {
            if(activesquad->squad[p]->prisoner->squadid==-1)
            {
               clearmessagearea();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->name);
               addstr(" drops ");
               addstr(activesquad->squad[p]->prisoner->name);
               addstr("'s body.");

               makeloot(*activesquad->squad[p]->prisoner,groundloot);

               refresh();
               getch();

               sitecrime+=10;
               sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
               //criminalizeparty(LAWFLAG_MURDER);
               //<-- might not die from squad's attacks

               if(activesquad->squad[p]->prisoner->type==CREATURE_CORPORATE_CEO||
                  activesquad->squad[p]->prisoner->type==CREATURE_RADIOPERSONALITY||
                  activesquad->squad[p]->prisoner->type==CREATURE_NEWSANCHOR||
                  activesquad->squad[p]->prisoner->type==CREATURE_SCIENTIST_EMINENT||
                  activesquad->squad[p]->prisoner->type==CREATURE_JUDGE_CONSERVATIVE)sitecrime+=30;

               delete activesquad->squad[p]->prisoner;
               activesquad->squad[p]->prisoner=NULL;
            }
         }
      }
   }

   if(location[cursite]->siege.siege)
   {
      for(int p=0;p<pool.size();p++)
      {
         if(!pool[p]->alive)continue;
         if(pool[p]->squadid!=-1)continue;
         if(pool[p]->location!=cursite)continue;

         advancecreature(*pool[p]);
      }

      autopromote(cursite);
   }

   for(e=0;e<ENCMAX;e++)
   {
      if(!encounter[e].exists)continue;
      if(!encounter[e].alive)continue;

      advancecreature(encounter[e]);
   }

   if(mode!=GAMEMODE_CHASECAR)
   {
      //TAKE THE INJURED WITH YOU
      squadgrab_immobile(0);

      //TAKE THE DEAD WITH YOU
      squadgrab_immobile(1);
   }

   for(e=ENCMAX-1;e>=0;e--)
   {
      if(!encounter[e].exists)continue;
      if(!encounter[e].alive)delenc(e,1);
   }

   if(mode==GAMEMODE_SITE)
   {
      if(sitealarm)postalarmtimer++;

      //Attacking government agencies results in the rapid
      //deployment of overwhelming force to the site -- especially
      //if there are a lot of heavily armed people already on site
      
      //Police Station -- SWAT teams are on site already and
      //only need to suit up to respond almost instantly
      if(location[cursite]->type==SITE_GOVERNMENT_POLICESTATION && 
         postalarmtimer<75 && sitealarm)postalarmtimer=75;
      //Courthouse -- Big police response, not far to come
      if(location[cursite]->type==SITE_GOVERNMENT_COURTHOUSE &&
         postalarmtimer<65 && sitealarm)postalarmtimer=65;
      //Prison -- Big police response but has to get from downtown
      if(location[cursite]->type==SITE_GOVERNMENT_PRISON &&
         postalarmtimer<50 && sitealarm)postalarmtimer=50;
      //Intelligence HQ -- Agents are deadly, but the site is not
      //is not designed to be called to action that quickly
      if(location[cursite]->type==SITE_GOVERNMENT_INTELLIGENCEHQ &&
         postalarmtimer<50 && sitealarm)postalarmtimer=50;

      if(sitealarmtimer>0&&!sitealarm)
      {
         sitealarmtimer--;
         if(sitealarmtimer<=0)
         {
            sitealarmtimer=0;

            clearmessagearea();

            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("The Squad smells Conservative panic.");

            if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
            else printencounter();

            refresh();
            getch();
         }
      }

      for(int z=0;z<MAPZ;z++)
      {
         bool stairs=0; // Will check if higher levels are accessible

         for(int y=0;y<MAPY;y++)
         {
            for(int x=0;x<MAPX;x++)
            {
               if(levelmap[x][y][z].flag & SITEBLOCK_EXIT)
                  continue;

               if(levelmap[x][y][z].special!=-1)
               {
                  if(levelmap[x][y][z].special & SPECIAL_STAIRS_UP)
                     stairs=1;
               }

               // Extinguish ending fires
               if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_END)
               {
                  if(!LCSrandom(15))
                  {
                     levelmap[x][y][z].flag &= ~SITEBLOCK_FIRE_END;
                     levelmap[x][y][z].flag |= SITEBLOCK_DEBRIS;
                  }
               }

               // Cool/spread peak fires
               if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_PEAK)
               {
                  siteonfire=1;
                  if(!LCSrandom(10))
                  {
                     levelmap[x][y][z].flag &= ~SITEBLOCK_FIRE_PEAK;
                     levelmap[x][y][z].flag |= SITEBLOCK_FIRE_END;
                  }
                  else if(!LCSrandom(4)) // Spread fire
                  {
                     int dir=LCSrandom(4); // Random initial direction
                     int tries=0; // Will try all four directions before giving up

                     while(tries<4)
                     {
                        int xmod=0;
                        int ymod=0;
                        switch(dir)
                        {
                        case 0:xmod=-1;break;
                        case 1:xmod=1;break;
                        case 2:ymod=-1;break;
                        case 3:ymod=1;break;
                        }
                        // Check if the tile is a valid place to spread fire to
                        if(x+xmod<MAPX&&x+xmod>=0&&y+ymod<MAPY&&y+ymod>=0&&
                           !(levelmap[x+xmod][y+ymod][z].flag & SITEBLOCK_FIRE_START)&&
                           !(levelmap[x+xmod][y+ymod][z].flag & SITEBLOCK_DEBRIS)&&
                           !(levelmap[x+xmod][y+ymod][z].flag & SITEBLOCK_FIRE_PEAK)&&
                           !(levelmap[x+xmod][y+ymod][z].flag & SITEBLOCK_FIRE_END)&&
                           !(levelmap[x+xmod][y+ymod][z].flag & SITEBLOCK_EXIT))
                        {
                           // Spread it
                           levelmap[x+xmod][y+ymod][z].flag |= SITEBLOCK_FIRE_START;
                           break;
                        }
                        // Else try another direction
                        tries++;
                        dir++;dir%=4;
                     }
                     if(tries==5) // If all four directions unacceptable, spread upward
                     {
                        // Check if up is valid
                        if(z<MAPZ&&
                           !(levelmap[x][y][z+1].flag & SITEBLOCK_FIRE_START)&&
                           !(levelmap[x][y][z+1].flag & SITEBLOCK_DEBRIS)&&
                           !(levelmap[x][y][z+1].flag & SITEBLOCK_FIRE_PEAK)&&
                           !(levelmap[x][y][z+1].flag & SITEBLOCK_FIRE_END))
                        {
                           // Spread it
                           levelmap[x][y][z+1].flag |= SITEBLOCK_FIRE_START;
                        }
                        // Else give up
                     }
                  }
               }

               // Aggrivate starting fires
               if(levelmap[x][y][z].flag & SITEBLOCK_FIRE_START)
               {
                  if(!LCSrandom(5))
                  {
                     sitechangest change(x,y,z,SITEBLOCK_DEBRIS);
                     location[cursite]->changes.push_back(change);
                     levelmap[x][y][z].flag&=~SITEBLOCK_BLOCK;
                     levelmap[x][y][z].flag&=~SITEBLOCK_DOOR;
                     levelmap[x][y][z].flag&=~SITEBLOCK_FIRE_START;
                     levelmap[x][y][z].flag|=SITEBLOCK_FIRE_PEAK;
                     sitecrime+=5;
                  }
               }
            }
         }

         // If no stairs to the next level were found, don't continue to that level
         if(!stairs)break;
      }
   }
}



/* handles end of round stuff for one creature */
void advancecreature(Creature &cr)
{
   if(!cr.alive)return;

   char incaprint;
   if(incapacitated(cr,1,incaprint))
   {
      if(incaprint)
      {
         printparty();
         if(mode==GAMEMODE_CHASECAR||
            mode==GAMEMODE_CHASEFOOT)printchaseencounter();
         else printencounter();

         refresh();
         getch();
      }
   }

   int bleed=0;
   int topmedicalskill=0;
   Creature* topmedical=NULL;
   for(int i=0;i<6;++i)
   {
      if(activesquad->squad[i]&&
         activesquad->squad[i]->alive&&
         activesquad->squad[i]->stunned==0&&
         activesquad->squad[i]->blood>40&&
         activesquad->squad[i]->id!=cr.id&&
         activesquad->squad[i]->skillval(SKILL_FIRSTAID)>topmedicalskill)
      {
         topmedical=activesquad->squad[i];
         topmedicalskill=activesquad->squad[i]->skillval(SKILL_FIRSTAID);
      }
   }

   for(int w=0;w<BODYPARTNUM;w++)
   {
      if(cr.wound[w] & WOUND_BLEEDING)
      {
         if(LCSrandom(500)<cr.attval(ATTRIBUTE_HEALTH)/*+cr.skillval(SKILL_SURVIVAL)*5*/)
         {
            cr.wound[w]^=WOUND_BLEEDING;
         }
         else if(cr.squadid!=-1&&topmedical&&LCSrandom(40)<topmedicalskill)
         {
            clearmessagearea();
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(16,1);
            addstr(topmedical->name);
            addstr(" was able to slow the bleeding of");
            move(17,1);
            addstr(cr.name);
            addstr("'s wounds.");
            topmedical->train(SKILL_FIRSTAID,max(int(50-topmedicalskill*2),0));
            cr.wound[w]^=WOUND_BLEEDING;
            refresh();
            getch();
         }
         else bleed++;
      }
   }

   if(mode==GAMEMODE_SITE && LCSrandom(3) &&
      ((levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK) ||
       (levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_END)))
   {
      int burndamage=0;
      clearmessagearea();

      if(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK)
      {
         burndamage=LCSrandom(40);
      }
      else
      {
         burndamage=LCSrandom(20);
      }

      // Firefighter's bunker gear reduces burn damage
      if(cr.armor.type==ARMOR_BUNKERGEAR)
      {
         // Base effect is 3/4 damage reduction, the denominator
         // increases with low quality or damaged gear
         int denom=4;

         // Damaged gear
         if(cr.armor.flag & ARMORFLAG_DAMAGED)
            denom+=2;
         // Shoddy quality gear
         denom+=cr.armor.quality - '1';

         // Apply damage reduction
         burndamage = static_cast<int>(burndamage * (3.0/denom));
      }


      cr.blood-=burndamage;

      char str[200];

      if(cr.blood<=0)
      {
         cr.blood=0;
         cr.alive=0;

         if(cr.squadid!=-1)
         {
            if(cr.align==1)stat_dead++;
         }
         else if(cr.align==-1&&(cr.animalgloss!=ANIMALGLOSS_ANIMAL||law[LAW_ANIMALRESEARCH]==2))
         {
            stat_kills++;
            if(location[cursite]->siege.siege)location[cursite]->siege.kills++;
         }
         if(cr.squadid==-1)
         {
            sitecrime+=10;
            sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
            criminalizeparty(LAWFLAG_MURDER);
            //<-- people dying in fire? probably your fault for starting it
         }
         adddeathmessage(cr);

         refresh();
         getch();

         if(cr.prisoner!=NULL)
         {
            freehostage(cr,1);
         }
      }
      else
      {
         set_color(COLOR_RED,COLOR_BLACK,0);
         move(16,1);
         strcpy(str,cr.name);
         strcat(str," is burned!");
         addstr(str);

         refresh();
         getch();
      }
   }

   if(bleed>0)
   {
      clearmessagearea();

      cr.blood-=bleed;

      levelmap[locx][locy][locz].flag|=SITEBLOCK_BLOODY;

      if(cr.armor.type!=ARMOR_NONE)cr.armor.flag|=ARMORFLAG_BLOODY;

      if(cr.blood<=0)
      {
         cr.blood=0;
         cr.alive=0;

         if(cr.squadid!=-1)
         {
            if(cr.align==1)stat_dead++;
         }
         else if(cr.align==-1&&(cr.animalgloss!=ANIMALGLOSS_ANIMAL||law[LAW_ANIMALRESEARCH]==2))
         {
            stat_kills++;
            if(location[cursite]->siege.siege)location[cursite]->siege.kills++;
         }
         if(cr.squadid==-1)
         {
            sitecrime+=10;
            sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
            //criminalizeparty(LAWFLAG_MURDER);
            //<-- might not die from squad attacking
         }
         adddeathmessage(cr);

         refresh();
         getch();

         if(cr.prisoner!=NULL)
         {
            freehostage(cr,1);
         }
      }
      else
      {
         //set_color(COLOR_RED,COLOR_BLACK,0);
         //move(16,1);
         //strcpy(str,cr.name);
         //strcat(str," bleeds.");
         //addstr(str);

         //refresh();
         //getch();
      }
   }
}
