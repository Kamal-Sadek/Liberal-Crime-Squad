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
void noticecheck(int exclude)
{
   if(sitealarm)return;

   char noticed=0;
   char sneak=0;
   vector<int> noticer;
   for(int e=0;e<ENCMAX;e++)
   {
      if(e==exclude)continue;
      //if(encounter[e].type==CREATURE_PRISONER)continue;
      if(strcmp(encounter[e].name,"Prisoner")==0)continue;
      if(encounter[e].exists&&encounter[e].alive&&
         encounter[e].enemy())
      {
         noticer.push_back(e);
      }
   }
   int topi=-1;
   for(int i=0;i<6;++i)
   {
      if(activesquad->squad[i]&&
         activesquad->squad[i]->skill[SKILL_SLEIGHTOFHAND]+activesquad->squad[i]->skill[SKILL_STEALTH]>sneak)
      {
         sneak=activesquad->squad[i]->skill[SKILL_SLEIGHTOFHAND]+activesquad->squad[i]->skill[SKILL_STEALTH];
         topi=i;
      }
   }
   if(topi>=0&&noticer.size())activesquad->squad[topi]->train(SKILL_SLEIGHTOFHAND,10);

   if(noticer.size()>0)
   {
      int n,an;

      do
      {
         an=LCSrandom(noticer.size());
         n=noticer[an];
         noticer.erase(noticer.begin() + an);

         int chance=encounter[n].attval(ATTRIBUTE_WISDOM)*3+encounter[n].attval(ATTRIBUTE_INTELLIGENCE);
         if(chance>LCSrandom(21+sneak))
         {
            noticed=1;
            break;
         }
      }while(noticer.size()>0);

      if(!noticed)return;

      clearmessagearea();

      set_color(COLOR_RED,COLOR_BLACK,1);
      move(16,1);
      addstr(encounter[n].name);
      addstr(" observes your Liberal activity");
      move(17,1);
      if(encounter[n].align==ALIGN_CONSERVATIVE)
         addstr("and lets forth a piercing Conservative alarm cry!");
      else
         addstr("and shouts for help!");
      
      sitealarm=1;


      refresh();
      getch();
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
void disguisecheck(void)
{
   int weapon=0;
   bool forcecheck=false;
   int weaponar[6]={0};

   for(int i=0;i<6;i++)
   {
      if(activesquad->squad[i]==NULL)break;
      if(activesquad->squad[i]->armor.type==ARMOR_NONE)forcecheck=true;
      int thisweapon=weaponcheck(*activesquad->squad[i],cursite);
      if(thisweapon>weapon)weapon=thisweapon;
      if(thisweapon==2)weaponar[i]=1;
   }

   if(sitealarm)
   {
      for(int i=0;i<6;i++)
      {
         if(activesquad->squad[i]==NULL)break;
         // If carrying an illegal weapon and never been charged with that
         // crime, apply it here. If they are wanted for carrying an illegal
         // weapon already, it shouldn't stack on extras here or you'll accrue
         // dozens of extra charges for only one site incident
         if(weaponar[i]&&!activesquad->squad[i]->lawflag[LAWFLAG_GUNCARRY])
         {
            // Illegal weapon crimes disabled
            //criminalize(*activesquad->squad[i],LAWFLAG_GUNCARRY);
         }
      }
   }

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
      int disguise = disguiseskill();
      int stealth  = stealthskill();
      bool sneaking=false;

      if(stealth>disguise || weapon==2 || sitealarm)
      {
         disguise=stealth;
         sneaking=true;
      }

      int n,an;

      do
      {
         an=LCSrandom(noticer.size());
         n=noticer[an];
         noticer.erase(noticer.begin() + an);

         int spotchance=encounter[n].attval(ATTRIBUTE_WISDOM) * 3+
                        encounter[n].attval(ATTRIBUTE_INTELLIGENCE);
         for(int i=0;i<6;i++)
         {
            if(!sneaking)disguisepractice(i, spotchance);
            else stealthpractice(i, spotchance);
         }
         if(sitealarmtimer ?
            spotchance > (int)LCSrandom(21)+disguise :
            spotchance+sitecrime > (int)LCSrandom(21)+disguise)
         {
            noticed=1;
            break;
         }

      }while(noticer.size()>0);

      if(!noticed)return;

      clearmessagearea();

      set_color(COLOR_RED,COLOR_BLACK,1);
      move(16,1);
      addstr(encounter[n].name);
      if(sitealarmtimer!=0 && weapon<1)
      {
         if((sitetype==SITE_RESIDENTIAL_TENEMENT||
             sitetype==SITE_RESIDENTIAL_APARTMENT||
             sitetype==SITE_RESIDENTIAL_APARTMENT_UPSCALE)&&
             levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
         {
            sitealarm=1;

            addstr(" shrieks in alarm at the squad's Liberal Tresspassing!");
         }
         else
         {
            addstr(" looks at the Squad suspiciously.");

            int time=20+LCSrandom(10)-encounter[n].attval(ATTRIBUTE_INTELLIGENCE)-encounter[n].attval(ATTRIBUTE_CHARISMA);
            if(time<1)time=1;

            if(sitealarmtimer>time||sitealarmtimer==-1)sitealarmtimer=time;
         }
      }
      else
      {
         if(weapon)
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
               if(weaponar[i])
               {
                  // Illegal weapon crimes disabled
                  //criminalize(*activesquad->squad[i],LAWFLAG_GUNCARRY);
               }
            }
         }
         else
         {
            addstr(" looks at the Squad with Intolerance");
            move(17,1);
            if(encounter[n].align==ALIGN_CONSERVATIVE)
               addstr("and lets forth a piercing Conservative alarm cry!");
            else
               addstr("and shouts for help!");
         }

         sitealarm=1;
      }

      refresh();
      getch();
   }
}



/* returns the difficulty of seeing through your squad's disguise */
int disguiseskill(void)
{
   int lowest=10000;
   int highest=0;
   int bonus = 15; // bonus for having few party members

   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         bonus -= 5; // bonus decreases for each party member

         // Dead people are a dead giveaway
         if(!activesquad->squad[p]->alive)return 0;

         // As are hostages
         if(activesquad->squad[p]->prisoner!=NULL)return 0;

         int skill=activesquad->squad[p]->attval(ATTRIBUTE_INTELLIGENCE)+
                   activesquad->squad[p]->attval(ATTRIBUTE_CHARISMA)+
                   activesquad->squad[p]->skill[SKILL_DISGUISE]*3;

         //ALSO NEED APPROPRIATE UNIFORM
         char uniformed=hasdisguise(*activesquad->squad[p],sitetype);

         if(!uniformed)
         {
            skill=0;
            //NAKEDNESS MAJOR PENALTY
            if(activesquad->squad[p]->armor.type==ARMOR_NONE)skill=-100;
         }
         else
         {
            //activesquad->squad[p]->train(SKILL_DISGUISE,5);
         }

         if(activesquad->squad[p]->armor.quality!='1')
         {
            if(activesquad->squad[p]->armor.quality=='2')skill--;
            else if(activesquad->squad[p]->armor.quality=='3')skill>>=1;
            else if(activesquad->squad[p]->armor.quality=='4')skill>>=2;
         }

         if(activesquad->squad[p]->armor.flag & ARMORFLAG_BLOODY)
         {
            skill>>=1;
         }

         if(activesquad->squad[p]->armor.flag & ARMORFLAG_DAMAGED)
         {
            skill>>=1;
         }

         if(lowest>skill)lowest=skill;
         if(highest<skill)highest=skill;
      }
   }

   return lowest + highest/4 + bonus;
}

/* practices p's stealth skill */
void disguisepractice(int p, int diff)  //diff is the difficulty that the Conservative sets for the disguise roll
{
	if(activesquad->squad[p]!=NULL)
	{
	    if(!activesquad->squad[p]->alive)return;

	    if(activesquad->squad[p]->prisoner!=NULL)return;
		
		//spread is how overwhelmed your disguise ability is by the Conservative
		int spread = diff-(15+ // magic number replacing your stats -- high stats shouldn't be punished here, low shouldn't be rewarded
                         activesquad->squad[p]->skill[SKILL_DISGUISE]*3);

      if(hasdisguise(*activesquad->squad[p],sitetype))
      {
         if(spread>10)
         {
	         activesquad->squad[p]->train(SKILL_DISGUISE,10);  //getting crushed isn't a great way to learn
         }
         else if(spread>0)
         {
  		      activesquad->squad[p]->train(SKILL_DISGUISE,spread);
	      }
      }
	}
}


/* returns the difficulty of spotting the squad if it is sneaking */
int stealthskill(void)
{
   int lowest=10000;
   int highest=0;
   int bonus = 20; // party size bonus, higher for fewer party members

   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         bonus -= 10; // less bonus for more people

         if(!activesquad->squad[p]->alive)return 0;

         if(activesquad->squad[p]->prisoner!=NULL)return 0;

         int skill=activesquad->squad[p]->skill[SKILL_STEALTH]*3+1;

         if(lowest>skill)lowest=skill;
         if(highest<skill)highest=skill;
      }
   }

   return lowest + bonus;
}

/* practices p's stealth skill */
void stealthpractice(int p, int diff)  //diff is the difficulty that the Conservative sets for the disguise roll
{
	if(activesquad->squad[p]!=NULL)
	{
	    if(!activesquad->squad[p]->alive)return;

	    if(activesquad->squad[p]->prisoner!=NULL)return;
	    
		//spread is how overwhelmed your stealth ability is by the Conservative
		int spread = diff-(activesquad->squad[p]->skill[SKILL_STEALTH]*3);
			
		if(spread>10)
		{
			activesquad->squad[p]->train(SKILL_STEALTH,10);    //getting crushed isn't a great way to learn
		}
		else if (spread>0)
		{
			activesquad->squad[p]->train(SKILL_STEALTH,spread);
		}
	}
}

/* checks if a creature's weapon is suspicious or illegal */
char weaponcheck(creaturest &cr,short type)
{
   bool suspicious=0;  // Does the weapon look at all suspicious?
   char illegal=0;     // Is the weapon illegal?
   char incharacter=0; // Is the weapon in character for the clothing the LCS is wearing?
   char concealed=0;   // Is the weapon concealed under clothing?

   //CHECK SUSPICIOUSNESS
   switch(cr.weapon.type)
   {
   case WEAPON_NONE:
   case WEAPON_CROSS:
   case WEAPON_SYRINGE:
   case WEAPON_GAVEL:
   case WEAPON_GUITAR:
   case WEAPON_SPRAYCAN:
   case WEAPON_NIGHTSTICK:
   case WEAPON_CHAIN:
      suspicious=0;
      break;
   default:
      suspicious=1;
      break;
   }

   //CHECK CONCEALMENT
   switch(cr.weapon.type)
   {
   case WEAPON_SPRAYCAN:
   case WEAPON_CHAIN:
   case WEAPON_SYRINGE:
   case WEAPON_GAVEL:
   case WEAPON_CROSS:
   case WEAPON_SHANK:
   case WEAPON_KNIFE:
   case WEAPON_CROWBAR:
   case WEAPON_REVOLVER_22:
   case WEAPON_REVOLVER_44:
   case WEAPON_SEMIPISTOL_9MM:
   case WEAPON_SEMIPISTOL_45:
      if(cr.armor.type!=ARMOR_NONE)concealed=1;
      break;
   case WEAPON_SHOTGUN_PUMP:
   case WEAPON_SMG_MP5:
      if(cr.armor.type==ARMOR_TRENCHCOAT)concealed=1;
      break;
   default:
      concealed=0;
      break;
   }

   //CHECK UNIFORM
   incharacter=0;
   switch(cr.weapon.type)
   {
   case WEAPON_SYRINGE:
      if(cr.armor.type==ARMOR_LABCOAT)incharacter=CREATURE_SCIENTIST_LABTECH;
      break;
   case WEAPON_GAVEL:
      if(cr.armor.type==ARMOR_BLACKROBE)incharacter=CREATURE_JUDGE_LIBERAL;
      break;
   case WEAPON_REVOLVER_22:
   case WEAPON_REVOLVER_44:
   case WEAPON_SEMIPISTOL_9MM:
   case WEAPON_SEMIPISTOL_45:
   case WEAPON_NIGHTSTICK:
      if(cr.armor.type==ARMOR_SECURITYUNIFORM)
      {
         incharacter=CREATURE_SECURITYGUARD;
      }
      //break; // no break
   case WEAPON_SHOTGUN_PUMP:
      // Police
      if(cr.armor.type==ARMOR_POLICEUNIFORM||
         cr.armor.type==ARMOR_POLICEARMOR)
      {
         incharacter=CREATURE_COP;
      }
      break;
   case WEAPON_SMG_MP5:
   case WEAPON_CARBINE_M4:
      // Police, military, or, in extreme times, security
      if(cr.armor.type==ARMOR_SWATARMOR)
      {
         incharacter=CREATURE_SWAT;
      }
      if(cr.armor.type==ARMOR_SECURITYUNIFORM&&law[LAW_GUNCONTROL]==-2)
      {
         incharacter=CREATURE_SECURITYGUARD;
      }
      if(cr.armor.type==ARMOR_MILITARY||
         cr.armor.type==ARMOR_ARMYARMOR)
      {
         incharacter=CREATURE_SOLDIER;
      }
      break;
   case WEAPON_AUTORIFLE_M16:
   case WEAPON_SEMIRIFLE_AR15:
      // Military
      if(cr.armor.type==ARMOR_MILITARY||cr.armor.type==ARMOR_ARMYARMOR)
      {
         incharacter=CREATURE_SOLDIER;
      }
      if(cr.armor.type==ARMOR_DEATHSQUADUNIFORM)
      {
         incharacter=CREATURE_DEATHSQUAD;
      }
      break;
   case WEAPON_AXE:
      if(cr.armor.type==ARMOR_BUNKERGEAR)
      {
         incharacter=CREATURE_FIREFIGHTER;
      }
      break;
   case WEAPON_FLAMETHROWER:
      if(cr.armor.type==ARMOR_BUNKERGEAR&&law[LAW_FREESPEECH]==-2)
      {
         incharacter=CREATURE_FIREFIGHTER;
      }
      break;
   }

   //CHECK LEGALITY
   switch(cr.weapon.type)
   {
   case WEAPON_SHOTGUN_PUMP:
      illegal=0;
   case WEAPON_REVOLVER_22:
      if(law[LAW_GUNCONTROL]==1)illegal=0;
      else illegal=1;
   case WEAPON_SEMIPISTOL_9MM:
   case WEAPON_SEMIPISTOL_45:
   case WEAPON_REVOLVER_44:
      if(law[LAW_GUNCONTROL]==0)illegal=0;
      else illegal=1;
   case WEAPON_SEMIRIFLE_AR15:
   case WEAPON_AUTORIFLE_M16:
      if(law[LAW_GUNCONTROL]==-1)illegal=0;
      else illegal=1;
   case WEAPON_SMG_MP5:
   case WEAPON_CARBINE_M4:
   case WEAPON_AUTORIFLE_AK47:
      if(law[LAW_GUNCONTROL]==-2)illegal=0;
      else illegal=1;
      break;
   case WEAPON_FLAMETHROWER:
      illegal=1;
      break;
   default:
      illegal=0;
   }

   // If your disguise is inappropriate to the current location,
   // then being in character isn't sufficient
   if(hasdisguise(cr,location[cursite]->type)==false)
   {
      incharacter=-1;
   }

   // For now, you can only bluff your way out of toting weapons
   // if you're in the middle of a siege where the place is getting
   // stormed by people carrying such weapons
   if(location[cursite]->siege.siege==false)
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
char hasdisguise(creaturest &cr,short type)
{
   char uniformed=0;

   if(location[cursite]->siege.siege)
   {
      switch(location[cursite]->siege.siegetype)
      {
         case SIEGE_CIA:
         {
            if(cr.armor.type==ARMOR_BLACKSUIT)uniformed=1;
            break;
         }
         case SIEGE_CORPORATE:
         {
            if(cr.armor.type==ARMOR_MILITARY)uniformed=1;
            if(cr.armor.type==ARMOR_ARMYARMOR)uniformed=1;
            break;
         }
         case SIEGE_HICKS:
         {
            if(cr.armor.type==ARMOR_CLOTHES)uniformed=1;
            if(cr.armor.type==ARMOR_OVERALLS)uniformed=1;
            if(cr.armor.type==ARMOR_WIFEBEATER)uniformed=1;
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
            if(cr.armor.type==ARMOR_SWATARMOR&&
               location[cursite]->siege.escalationstate==0)uniformed=1;
            if(cr.armor.type==ARMOR_MILITARY&&
               location[cursite]->siege.escalationstate>0)uniformed=1;
            if(cr.armor.type==ARMOR_ARMYARMOR&&
               location[cursite]->siege.escalationstate>0)uniformed=1;
            break;
         }
         case SIEGE_FIREMEN:
         {
            if(cr.armor.type==ARMOR_BUNKERGEAR)uniformed=1;
            break;
         }
      }
   }
   else
   {
      if(cr.armor.type!=ARMOR_NONE&&cr.armor.type!=ARMOR_HEAVYARMOR)uniformed=1;

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
               if(cr.armor.type==ARMOR_LABCOAT)uniformed=1;
               if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_POLICESTATION:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(law[LAW_POLICEBEHAVIOR]==-2 && law[LAW_DEATHPENALTY]==-2 &&
                  cr.armor.type==ARMOR_DEATHSQUADUNIFORM)uniformed=1;
               if(cr.armor.type==ARMOR_POLICEUNIFORM)uniformed=1;
               if(cr.armor.type==ARMOR_POLICEARMOR)uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_COURTHOUSE:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.armor.type==ARMOR_BLACKROBE)uniformed=1;
               if(cr.armor.type==ARMOR_BLACKSUIT)uniformed=1;
               if(cr.armor.type==ARMOR_BLACKDRESS)uniformed=1;
               if(cr.armor.type==ARMOR_CHEAPSUIT)uniformed=1;
               if(cr.armor.type==ARMOR_CHEAPDRESS)uniformed=1;
               if(cr.armor.type==ARMOR_EXPENSIVESUIT)uniformed=1;
               if(cr.armor.type==ARMOR_EXPENSIVEDRESS)uniformed=1;
               if(cr.armor.type==ARMOR_POLICEUNIFORM)uniformed=1;
               if(cr.armor.type==ARMOR_POLICEARMOR)uniformed=1;
               if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
               if(law[LAW_POLICEBEHAVIOR]==-2 && law[LAW_DEATHPENALTY]==-2 &&
                  cr.armor.type==ARMOR_DEATHSQUADUNIFORM)uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_PRISON:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(law[LAW_DEATHPENALTY]==-2&&
                   law[LAW_POLICEBEHAVIOR]==-2)
               {
                  if(cr.armor.type==ARMOR_LABCOAT)uniformed=1;
               }
               else if(cr.armor.type==ARMOR_PRISONGUARD)uniformed=1;
               if(cr.armor.type==ARMOR_PRISONER)uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_INTELLIGENCEHQ:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.armor.type==ARMOR_BLACKSUIT)uniformed=1;
               if(cr.armor.type==ARMOR_BLACKDRESS)uniformed=1;
               if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            }
            break;
         case SITE_GOVERNMENT_FIRESTATION:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.armor.type==ARMOR_BUNKERGEAR)uniformed=1;
               if(cr.armor.type==ARMOR_WORKCLOTHES)uniformed=1;
               if(cr.armor.type==ARMOR_OVERALLS)uniformed=1;
               if(location[cursite]->highsecurity)
               {
                  if(cr.armor.type==ARMOR_POLICEUNIFORM)uniformed=1;
                  if(cr.armor.type==ARMOR_POLICEARMOR)uniformed=1;
               }
            }
            break;
         case SITE_BUSINESS_CIGARBAR:
            uniformed=0;
            if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            if(cr.armor.type==ARMOR_EXPENSIVESUIT)uniformed=1;
            if(cr.armor.type==ARMOR_CHEAPSUIT)uniformed=1;
            if(cr.armor.type==ARMOR_EXPENSIVEDRESS)uniformed=1;
            if(cr.armor.type==ARMOR_CHEAPDRESS)uniformed=1;
            if(cr.armor.type==ARMOR_BLACKSUIT)uniformed=1;
            if(cr.armor.type==ARMOR_BLACKDRESS)uniformed=1;
            break;
         case SITE_INDUSTRY_SWEATSHOP:
            uniformed=0;
            if(cr.armor.type==ARMOR_NONE)uniformed=1;
            if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            break;
         case SITE_INDUSTRY_POLLUTER:
            uniformed=0;
            if(cr.armor.type==ARMOR_WORKCLOTHES)uniformed=1;
            if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            break;
         case SITE_INDUSTRY_NUCLEAR:
            uniformed=0;
            if(cr.armor.type==ARMOR_LABCOAT)uniformed=1;
            if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            break;
         case SITE_CORPORATE_HEADQUARTERS:
            uniformed=0;
            if(cr.armor.type==ARMOR_EXPENSIVESUIT)uniformed=1;
            if(cr.armor.type==ARMOR_CHEAPSUIT)uniformed=1;
            if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            if(cr.armor.type==ARMOR_EXPENSIVEDRESS)uniformed=1;
            if(cr.armor.type==ARMOR_CHEAPDRESS)uniformed=1;
            break;
         case SITE_CORPORATE_HOUSE:
            uniformed=0;
            if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
            break;
         case SITE_MEDIA_AMRADIO:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
               if(cr.armor.type==ARMOR_EXPENSIVESUIT)uniformed=1;
               if(cr.armor.type==ARMOR_CHEAPSUIT)uniformed=1;
               if(cr.armor.type==ARMOR_EXPENSIVEDRESS)uniformed=1;
               if(cr.armor.type==ARMOR_CHEAPDRESS)uniformed=1;
            }
            break;
         case SITE_MEDIA_CABLENEWS:
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
            {
               uniformed=0;
               if(cr.armor.type==ARMOR_SECURITYUNIFORM)uniformed=1;
               if(cr.armor.type==ARMOR_EXPENSIVESUIT)uniformed=1;
               if(cr.armor.type==ARMOR_EXPENSIVEDRESS)uniformed=1;
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
