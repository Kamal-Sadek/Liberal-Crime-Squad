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



/* checks if your liberal activity is noticed */
void noticecheck(int exclude,int difficulty)
{
   if(sitealarm)return;

   char sneak=0;
   
   int topi=0;
   for(int i=0;i<6;++i)
   {
      if(activesquad->squad[i]&&
         activesquad->squad[i]->get_skill(SKILL_STEALTH)>sneak)
      {
         sneak=activesquad->squad[i]->get_skill(SKILL_STEALTH);
         topi=i;
      }
   }

   for(int e=0;e<ENCMAX;e++)
   {
      //Prisoners shouldn't shout for help.
      if(!strcmp(encounter[e].name,"Prisoner"))continue;
      
      if(e==exclude || encounter[e].exists==false)
         continue;
      else if(activesquad->squad[topi]->skill_check(SKILL_STEALTH,difficulty))
         continue;
      else
      {
         clearmessagearea();

         set_color(COLOR_RED,COLOR_BLACK,1);
         move(16,1);
         addstr(encounter[e].name);
         addstr(" observes your Liberal activity");
         move(17,1);
         if(encounter[e].align==ALIGN_CONSERVATIVE)
            addstr("and lets forth a piercing Conservative alarm cry!");
         else
            addstr("and shouts for help!");
         
         sitealarm=1;


         refresh();
         getch();
         break;
      }
   }
}



/* checks if your liberal behavior/attack alienates anyone */
char alienationcheck(char mistake)
{
   if(location[cursite]->siege.siege)return 0;


   char alienate=0;
   char alienatebig=0;

   char sneak=0;

   int oldsitealienate=sitealienate;

   vector<int> noticer;
   for(int e=0;e<ENCMAX;e++)
   {
      // Prisoners should never be alienated by your crimes, as
      // they're happy to have you attacking their place of holding
      //if(encounter[e].type==CREATURE_PRISONER)continue;

      // ...but Prisoners are now spawned with a variety of creature
      // types, so we'll go by name instead
      if(!strcmp(encounter[e].name,"Prisoner"))continue;

      if(encounter[e].exists&&encounter[e].alive&&
         (encounter[e].align==0||(encounter[e].align==1&&mistake)))
      {
         noticer.push_back(e);
      }
   }

   if(noticer.size()>0)
   {
      int n,an;

      do
      {
         an=LCSrandom(noticer.size());
         n=noticer[an];
         noticer.erase(noticer.begin() + an);

         if(encounter[n].align==1)alienatebig=1;
         else alienate=1;
      }while(noticer.size()>0);

      if(alienatebig)sitealienate=2;
      if(alienate&&sitealienate!=2)sitealienate=1;

      if(oldsitealienate<sitealienate)
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);

         move(16,1);
         if(sitealienate==1)addstr("We've alienated the masses here!              ");
         else addstr("We've alienated absolutely everyone here!               ");
         move(17,1);
         addstr("                                                        ");

         sitealarm=1;

         for(int i=0;i<ENCMAX;i++)
         {
            if(encounter[i].exists && encounter[i].align != ALIGN_CONSERVATIVE)
            {
               if(encounter[i].align == ALIGN_MODERATE || alienatebig)
                  conservatise(encounter[i]);
            }
         }

         if(mode==GAMEMODE_CHASECAR||
                     mode==GAMEMODE_CHASEFOOT)printchaseencounter();
         else printencounter();
         refresh();
         getch();
      }
   }

   return alienate;
}


/* checks if conservatives see through your disguise */
void disguisecheck(int timer)
{
   int weapon=0;
   int squadsize=0;
   bool forcecheck=false;
   int weaponar[6]={0};

   bool spotted = false;
   int blew_it = -1;

   // Only start to penalize the player's disguise/stealth checks after the first turn.
   timer--;

   for(int i=0;i<6;i++)
   {
      if(activesquad->squad[i]==NULL)break;
      squadsize++;
      if(activesquad->squad[i]->is_naked() &&
         activesquad->squad[i]->animalgloss!=ANIMALGLOSS_ANIMAL)forcecheck=true;
      int thisweapon=weaponcheck(*activesquad->squad[i]);
      if(thisweapon>weapon)weapon=thisweapon;
      if(thisweapon==2)weaponar[i]=1;
   }

   // Nothing suspicious going on here
   if(sitealarmtimer==-1 && weapon<1 && !forcecheck)
   {
      if(!disguisesite(location[cursite]->type)&&
         !(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED))return;
   }

   char noticed=0;
   vector<int> noticer;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].type==CREATURE_PRISONER)continue;
      if(encounter[e].exists&&encounter[e].alive&&
         encounter[e].enemy())
      {
         noticer.push_back(e);
      }
   }

   if(noticer.size()>0)
   {
      int n,an;

      do
      {
         an=LCSrandom(noticer.size());
         n=noticer[an];
         noticer.erase(noticer.begin() + an);

         int difficulty;

         // Determine difficulty based on enemy type
         switch(encounter[n].type)
         {
         default:
            difficulty = DIFFICULTY_VERYEASY;
            break;
         case CREATURE_SWAT:
         case CREATURE_COP:
         case CREATURE_GANGUNIT:
         case CREATURE_DEATHSQUAD:
            difficulty = DIFFICULTY_EASY;
            break;
         case CREATURE_PRISONGUARD:
         case CREATURE_BOUNCER:
         case CREATURE_AGENT:
         case CREATURE_SECURITYGUARD:
            difficulty = DIFFICULTY_AVERAGE;
            break;
         case CREATURE_CORPORATE_CEO:
         case CREATURE_JUDGE_CONSERVATIVE:
         case CREATURE_CCS_ARCHCONSERVATIVE:
         case CREATURE_SCIENTIST_EMINENT:
            difficulty = DIFFICULTY_HARD;
            break;
         case CREATURE_GUARDDOG:
            difficulty = DIFFICULTY_HEROIC;
            break;
         }

         // Increase difficulty if Conservatives suspicious...
         if(sitealarmtimer==1)
            difficulty += 3;

         // Make the attempt!
         for(int i=0;i<6;i++)
         {
            if(activesquad->squad[i]==NULL)break;

            // Try to sneak.
            if(!spotted)
            {
               int result = activesquad->squad[i]->skill_roll(SKILL_STEALTH);
               result -= timer;
               // Sneaking with a party is hard
               if(result < (difficulty + squadsize - 1))
                  spotted = true;
            }

            // Spotted! Act casual.
            if(spotted)
            {
               // Guns of death and doom are not very casual.
               if(weaponcheck(*activesquad->squad[i]) == 2)
               {
                  noticed = true;
                  break;
               }
               else
               {
                  int result = activesquad->squad[i]->skill_roll(SKILL_DISGUISE);
                  result -= timer;

                  if(result<difficulty)
                  {
                     // That was not very casual, dude.
                     if(result<0)blew_it=i;
                     
                     noticed = true;
                     break;
                  }
               }
            }
         }
         if(noticed)break;
      }while(noticer.size()>0);

      // Give feedback on the Liberal Performance
      if(!spotted)
      {
         for(int i=0;i<6;i++)
         {
            if(activesquad->squad[i] == NULL) break;
            activesquad->squad[i]->train(SKILL_STEALTH, 10);
         }

         if(timer == 0)
         {
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(16,1);

            if(squadsize > 1)
               addstr("The squad ");
            else
               addstr(activesquad->squad[0]->name);
            addstr(" fades into the shadows.");

            getch();
         }
      }
      else
      {
         if(blew_it == -1)
         {
            int i;
            for(i=0;i<6;i++)
            {
               if(activesquad->squad[i] == NULL) break;
               if(hasdisguise(*(activesquad->squad[i])))
                  activesquad->squad[i]->train(SKILL_DISGUISE, 10);
            }
         }

         if(blew_it != -1 && LCSrandom(2))
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);

            addstr(activesquad->squad[blew_it]->name);
            switch(LCSrandom(5))
            {
            case 0: addstr(" coughs."); break;
            case 1: addstr(" accidentally mumbles the slogan."); break;
            case 2: addstr(" paces uneasily."); break;
            case 3: addstr(" stares at the Conservatives."); break;
            case 4: addstr(" laughs nervously."); break;
            }
            
            getch();
         }
         else if(!noticed)
         {
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(16,1);

            if(squadsize > 1)
               addstr("The squad");
            else
               addstr(activesquad->squad[0]->name);
            addstr(" acts natural.");

            getch();
         }
      }

      if(!noticed)return;

      clearmessagearea();

      set_color(COLOR_RED,COLOR_BLACK,1);
      move(16,1);
      addstr(encounter[n].name);
      if(sitealarmtimer!=0 && weapon<1 && encounter[n].type!=CREATURE_GUARDDOG)
      {
         if((sitetype==SITE_RESIDENTIAL_TENEMENT||
             sitetype==SITE_RESIDENTIAL_APARTMENT||
             sitetype==SITE_RESIDENTIAL_APARTMENT_UPSCALE)&&
             levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
         {
            sitealarm=1;

            addstr(" shouts in alarm at the squad's Liberal Trespassing!");
         }
         else
         {
            addstr(" looks at the Squad suspiciously.");

            int time;

            time = 20 + LCSrandom(10) - encounter[n].get_attribute(ATTRIBUTE_INTELLIGENCE,true)
                                      - encounter[n].get_attribute(ATTRIBUTE_WISDOM,true);
            
            if(time<1)time=1;

            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
            else
            {
               if(sitealarmtimer>5) sitealarmtimer-= 5;
               if(sitealarmtimer<=5)sitealarmtimer = 0;
            }
         }
      }
      else
      {
         if(weapon&&encounter[n].type!=CREATURE_GUARDDOG)
         {
            addstr(" sees the Squad's Liberal Weapons");
            move(17,1);
            if(encounter[n].align==ALIGN_CONSERVATIVE)
               addstr("and lets forth a piercing Conservative alarm cry!");
            else
               addstr("and shouts for help!");

            for(int i=0;i<6;i++)
            {
               if(activesquad->squad[i]==NULL)break;
               //if(weaponar[i])
               //{
                  // Illegal weapon crimes disabled
                  //criminalize(*activesquad->squad[i],LAWFLAG_GUNCARRY);
               //}
            }
         }
         else
         {
            addstr(" looks at the Squad with Intolerance");
            move(17,1);
            if(encounter[n].align==ALIGN_CONSERVATIVE)
            {
               if(encounter[n].type==CREATURE_GUARDDOG)
                  addstr("and launches into angry Conservative barking!");
               else
                  addstr("and lets forth a piercing Conservative alarm cry!");
               
            }
            else
               addstr("and shouts for help!");
         }

         sitealarm=1;
      }

      refresh();
      getch();
   }
}

/* checks if a creature's weapon is suspicious or illegal */
char weaponcheck(Creature &cr)
{
   bool suspicious=cr.get_weapon().is_suspicious();  // Does the weapon look at all suspicious?
   char illegal=0;     // Is the weapon illegal?
   char incharacter=0; // Is the weapon in character for the clothing the LCS is wearing?
   bool concealed=cr.weapon_is_concealed();   // Is the weapon concealed under clothing?

   //CHECK UNIFORM
   incharacter=0;
   if(cr.get_weapon().get_itemtypename() == "WEAPON_SYRINGE"
      && cr.get_armor().get_itemtypename() == "ARMOR_LABCOAT")
      incharacter=CREATURE_SCIENTIST_LABTECH;
   else if(cr.get_weapon().get_itemtypename() == "WEAPON_GAVEL"
           && cr.get_armor().get_itemtypename() == "ARMOR_BLACKROBE")
      incharacter=CREATURE_JUDGE_LIBERAL;
   else if((cr.get_weapon().get_itemtypename() == "WEAPON_REVOLVER_38"
            || cr.get_weapon().get_itemtypename() == "WEAPON_REVOLVER_44"
            || cr.get_weapon().get_itemtypename() == "WEAPON_DESERT_EAGLE"
            || cr.get_weapon().get_itemtypename() == "WEAPON_SEMIPISTOL_9MM"
            || cr.get_weapon().get_itemtypename() == "WEAPON_SEMIPISTOL_45"
            || cr.get_weapon().get_itemtypename() == "WEAPON_NIGHTSTICK"
            || cr.get_weapon().get_itemtypename() == "WEAPON_MP5_SMG")
            && (cr.get_armor().get_itemtypename() == "ARMOR_SECURITYUNIFORM"))
      incharacter=CREATURE_SECURITYGUARD;
   else if(cr.get_weapon().get_itemtypename() == "WEAPON_REVOLVER_38"
            || cr.get_weapon().get_itemtypename() == "WEAPON_REVOLVER_44"
            || cr.get_weapon().get_itemtypename() == "WEAPON_DESERT_EAGLE"
            || cr.get_weapon().get_itemtypename() == "WEAPON_SEMIPISTOL_9MM"
            || cr.get_weapon().get_itemtypename() == "WEAPON_SEMIPISTOL_45"
            || cr.get_weapon().get_itemtypename() == "WEAPON_NIGHTSTICK"
            || cr.get_weapon().get_itemtypename() == "WEAPON_SHOTGUN_PUMP")
   {
      if(cr.get_armor().get_itemtypename() == "ARMOR_POLICEUNIFORM"
         || (cr.get_armor().get_itemtypename() == "ARMOR_POLICEARMOR"))
         incharacter=CREATURE_COP;
      else if(cr.get_armor().get_itemtypename() == "MASK_CHENEY")
         incharacter=CREATURE_HICK;
   }
   else if(cr.get_weapon().get_itemtypename() == "WEAPON_SMG_MP5"
           || cr.get_weapon().get_itemtypename() == "WEAPON_CARBINE_M4")
   {
      // Police, military, or, in extreme times, security
      if(cr.get_armor().get_itemtypename() == "ARMOR_SWATARMOR")
      {
         incharacter=CREATURE_SWAT;
      }
      if(cr.get_armor().get_itemtypename() == "ARMOR_SECURITYUNIFORM"&&law[LAW_GUNCONTROL]==-2)
      {
         incharacter=CREATURE_SECURITYGUARD;
      }
      if(cr.get_armor().get_itemtypename() == "ARMOR_MILITARY"
         || cr.get_armor().get_itemtypename() == "ARMOR_ARMYARMOR")
      {
         incharacter=CREATURE_SOLDIER;
      }
   }
   else if(cr.get_weapon().get_itemtypename() == "WEAPON_AUTORIFLE_M16"
           || cr.get_weapon().get_itemtypename() == "WEAPON_SEMIRIFLE_AR15")
   {
      // Military
      if(cr.get_armor().get_itemtypename() == "ARMOR_MILITARY"
         ||cr.get_armor().get_itemtypename() == "ARMOR_ARMYARMOR")
      {
         incharacter=CREATURE_SOLDIER;
      }
      if(cr.get_armor().get_itemtypename() == "ARMOR_DEATHSQUADUNIFORM")
      {
         incharacter=CREATURE_DEATHSQUAD;
      }
   }
   else if(cr.get_weapon().get_itemtypename() == "WEAPON_AXE"
           && cr.get_armor().get_itemtypename() == "ARMOR_BUNKERGEAR")
   {
      incharacter=CREATURE_FIREFIGHTER;
   }
   else if(cr.get_weapon().get_itemtypename() == "WEAPON_FLAMETHROWER"
           && cr.get_armor().get_itemtypename() == "ARMOR_BUNKERGEAR"&&law[LAW_FREESPEECH]==-2)
   {
      incharacter=CREATURE_FIREFIGHTER;
   }

   //CHECK LEGALITY
   if (cr.get_weapon().get_legality() < law[LAW_GUNCONTROL])
      illegal = 1;
   else
      illegal = 0;


   // If your disguise is inappropriate to the current location,
   // then being in character isn't sufficient
   if(hasdisguise(cr)==false)
   {
      incharacter=-1;
   }

   if(suspicious)
   {
      if(incharacter!=-1||concealed)
      {
         // if(illegal) return -1; else // OK, but busted if you shoot it
            return 0;  // OK
      }
      else // if(illegal)
         return 2;
      //else return 1;
   }
   return 0;
}


/* checks if a creature's uniform is appropriate to the location */
char hasdisguise(Creature &cr)
{
   short type = -1;
   if(cursite>=0)type = location[cursite]->type;
   char uniformed=0;

   // Never uniformed in battle colors
   //if(activesquad->stance==SQUADSTANCE_BATTLECOLORS)
   //   return 0;

   if(location[cursite]->siege.siege)
   {
      switch(location[cursite]->siege.siegetype)
      {
         case SIEGE_CIA:
         {
            if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKSUIT")uniformed=1;
            break;
         }
         case SIEGE_CORPORATE:
         {
            if(cr.get_armor().get_itemtypename()=="ARMOR_MILITARY")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_ARMYARMOR")uniformed=1;
            break;
         }
         case SIEGE_HICKS:
         {
            if(cr.get_armor().get_itemtypename()=="ARMOR_CLOTHES")uniformed=2;
            if(cr.get_armor().get_itemtypename()=="ARMOR_OVERALLS")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_WIFEBEATER")uniformed=1;
            break;
         }
         case SIEGE_CCS:
         {
            // CCS has trained in anticipation of this tactic
            // There is no fooling them
            // (They pull this shit all the time in their own sieges)
            uniformed=0;
            break;
         }
         case SIEGE_POLICE:
         {
            if(cr.get_armor().get_itemtypename()=="ARMOR_SWATARMOR"&&
               location[cursite]->siege.escalationstate==0)uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_MILITARY"&&
               location[cursite]->siege.escalationstate>0)uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_ARMYARMOR"&&
               location[cursite]->siege.escalationstate>0)uniformed=1;
            break;
         }
         case SIEGE_FIREMEN:
         {
            if(cr.get_armor().get_itemtypename()=="ARMOR_BUNKERGEAR")uniformed=1;
            break;
         }
      }
   }
   else
   {
      if((!cr.is_naked()||cr.animalgloss==ANIMALGLOSS_ANIMAL)
         &&cr.get_armor().get_itemtypename()!="ARMOR_HEAVYARMOR")uniformed=1;

      switch(type)
      {
         
         case SITE_INDUSTRY_WAREHOUSE:
         case SITE_RESIDENTIAL_SHELTER:
            uniformed=1;
            break;
         case SITE_LABORATORY_COSMETICS:
         case SITE_LABORATORY_GENETIC:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.get_armor().get_itemtypename()=="ARMOR_LABCOAT")uniformed=1;
               if(location[cursite]->highsecurity)
               {
                  if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
               }
               else
               {
                  if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=2;
               }
            }
            break;
         case SITE_GOVERNMENT_POLICESTATION:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(law[LAW_POLICEBEHAVIOR]==-2 && law[LAW_DEATHPENALTY]==-2 &&
                  cr.get_armor().get_itemtypename()=="ARMOR_DEATHSQUADUNIFORM")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_COURTHOUSE:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKROBE")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKSUIT")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKDRESS")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPSUIT")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPDRESS")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVESUIT")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVEDRESS")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")uniformed=1;
               if(law[LAW_POLICEBEHAVIOR]==-2 && law[LAW_DEATHPENALTY]==-2 &&
                  cr.get_armor().get_itemtypename()=="ARMOR_DEATHSQUADUNIFORM")uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_PRISON:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(law[LAW_DEATHPENALTY]==-2&&
                   law[LAW_POLICEBEHAVIOR]==-2)
               {
                  if(cr.get_armor().get_itemtypename()=="ARMOR_LABCOAT")uniformed=1;
               }
               else if(cr.get_armor().get_itemtypename()=="ARMOR_PRISONGUARD")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_PRISONER")uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_ARMYBASE:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.get_armor().get_itemtypename()=="ARMOR_MILITARY")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_ARMYARMOR")uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_INTELLIGENCEHQ:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKSUIT")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKDRESS")uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_FIRESTATION:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.get_armor().get_itemtypename()=="ARMOR_BUNKERGEAR")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_WORKCLOTHES")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_OVERALLS")uniformed=1;
               if(location[cursite]->highsecurity)
               {
                  if(cr.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM")uniformed=1;
                  if(cr.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")uniformed=1;
               }
            }
            break;
         case SITE_BUSINESS_CIGARBAR:
            uniformed=0;
            if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVESUIT")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPSUIT")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVEDRESS")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPDRESS")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKSUIT")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_BLACKDRESS")uniformed=1;
            break;
         case SITE_INDUSTRY_SWEATSHOP:
            uniformed=0;
            if(cr.is_naked())uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
            break;
         case SITE_INDUSTRY_POLLUTER:
            uniformed=0;
            if(cr.get_armor().get_itemtypename()=="ARMOR_WORKCLOTHES")uniformed=1;
            if(location[cursite]->highsecurity)
            {
               if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
            }
            break;
         case SITE_INDUSTRY_NUCLEAR:
            uniformed=0;
            if(cr.get_armor().get_itemtypename()=="ARMOR_LABCOAT")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
            break;
         case SITE_CORPORATE_HEADQUARTERS:
            uniformed=0;
            if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVESUIT")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPSUIT")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVEDRESS")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPDRESS")uniformed=1;
            break;
         case SITE_CORPORATE_HOUSE:
            uniformed=0;
            if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVESUIT")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVEDRESS")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
            if(cr.get_armor().get_itemtypename()=="ARMOR_SERVANTUNIFORM")uniformed=1;
            if(location[cursite]->highsecurity)
            {
               if(cr.get_armor().get_itemtypename()=="ARMOR_MILITARY")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_ARMYARMOR")uniformed=1;
            }
            break;
         case SITE_MEDIA_AMRADIO:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVESUIT")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPSUIT")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVEDRESS")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_CHEAPDRESS")uniformed=1;
            }
            break;
         case SITE_MEDIA_CABLENEWS:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.get_armor().get_itemtypename()=="ARMOR_SECURITYUNIFORM")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVESUIT")uniformed=1;
               if(cr.get_armor().get_itemtypename()=="ARMOR_EXPENSIVEDRESS")uniformed=1;
            }
            break;
         case SITE_RESIDENTIAL_TENEMENT:
         case SITE_RESIDENTIAL_APARTMENT:
         case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)uniformed=0;
            break;
         default:
            break;
      }
   }

   if(!uniformed)
   {
      if(cr.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM"||
         cr.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")
      {
         uniformed=2;
      }
      if(law[LAW_POLICEBEHAVIOR]==-2 && law[LAW_DEATHPENALTY]==-2 &&
            cr.get_armor().get_itemtypename()=="ARMOR_DEATHSQUADUNIFORM")
      {
         uniformed=2;
      }
   }

   return uniformed;
}



/* returns true if the entire site is not open to public */
char disguisesite(long type)
{
   switch(type)
   {
      case SITE_LABORATORY_COSMETICS:
      case SITE_LABORATORY_GENETIC:
      case SITE_GOVERNMENT_PRISON:
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
      case SITE_INDUSTRY_SWEATSHOP:
      case SITE_INDUSTRY_POLLUTER:
      case SITE_INDUSTRY_NUCLEAR:
      case SITE_CORPORATE_HEADQUARTERS:
      case SITE_CORPORATE_HOUSE:
      case SITE_BUSINESS_CIGARBAR:
         return 1;
   }

   return 0;
}
