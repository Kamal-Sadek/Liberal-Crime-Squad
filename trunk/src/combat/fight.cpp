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


bool goodguyattack = false;

/* attack handling for each side as a whole */
void youattack(void)
{
   short wasalarm=sitealarm;

   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].enemy())
         encounter[e].cantbluff=2;
   }

   sitealarm=1;

   goodguyattack = true;

   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(!activesquad->squad[p]->alive)continue;

      int thisweapon = weaponcheck(*activesquad->squad[p],activesquad->squad[p]->weapon.type);

      if((thisweapon==-1||thisweapon==2) &&           // Illegal weapon
          activesquad->squad[p]->weapon.ranged() &&   // Ranged
          activesquad->squad[p]->weapon.ammo!=0)      // Not out of ammo
      {
         // Illegal weapon crimes disabled
         //criminalize(*activesquad->squad[p],LAWFLAG_GUNUSE); // Criminalize for firing illegal weapon
      }

      vector<int> dangerous_enemies;
      vector<int> enemies;
      vector<int> non_enemies;

      for(int e=0;e<ENCMAX;e++)
      {
         if(encounter[e].alive&&encounter[e].exists)
         {
            if(encounter[e].enemy())
            {
               if(encounter[e].weapon.type!=WEAPON_NONE &&
                  encounter[e].blood>=40)
               {
                  dangerous_enemies.push_back(e);
               }
               else
               {
                  enemies.push_back(e);
               }
            }
            else non_enemies.push_back(e);
         }
      }

      if(dangerous_enemies.size()+enemies.size()==0)return;

      int target;
      // Roll 1dX-1, where X is the number of "dangerous enemies", plus one if there are "other enemies"
      target=LCSrandom(dangerous_enemies.size()+!!enemies.size());
      // If the result is less than the number of "dangerous enemies", the result indicates which of these to shoot at
      if(target!=dangerous_enemies.size())
         target=dangerous_enemies[target];
      // Else, roll again on the list of "other enemies" to pick one of them to shoot at
      else
         target=enemies[LCSrandom(enemies.size())];

      char mistake=0;
      // Less likely to accidentally hit bystanders,
      // and never hit the wrong person if not using a ranged
      // weapon
      if(non_enemies.size()>0 && !LCSrandom(60) && activesquad->squad[p]->weapon.ranged()) 
      {
         target=non_enemies[LCSrandom(non_enemies.size())];
         mistake=1;
         addjuice(*activesquad->squad[p],-5);
      }

      char actual;
      short beforeblood=encounter[target].blood;
      if(encounter[target].align==1)mistake=1;
      attack(*activesquad->squad[p],encounter[target],mistake,actual);
      

      if(actual)
      {
         if(mistake)
         {
            alienationcheck(mistake);
            sitestory->crime.push_back(CRIME_ATTACKED_MISTAKE);
            sitecrime+=10;
            addjuice(*(activesquad->squad[p]),-1);
         }
         else
         {
            sitecrime+=3;
            addjuice(*(activesquad->squad[p]),2);
         }
         sitestory->crime.push_back(CRIME_ATTACKED);
         // Charge with assault if (a) first strike, or (b) hit enemy
         if(!wasalarm||beforeblood>encounter[target].blood)
         {
            if(activesquad->squad[p]->weapon.type==WEAPON_NONE)
               criminalize(*activesquad->squad[p],LAWFLAG_ASSAULT);
            else
               criminalize(*activesquad->squad[p],LAWFLAG_ARMEDASSAULT);
         }
      }

      if(!encounter[target].alive)delenc(target,1);
   }

   //COVER FIRE
   if(location[cursite]->siege.siege)
   {
      for(int p=0;p<pool.size();p++)
      {
         if(!pool[p]->alive)continue;
         if(pool[p]->align!=1)continue;
         if(pool[p]->squadid!=-1)continue;
         if(pool[p]->location!=cursite)continue;
         // Juice check to engage in cover fire
         // 10% chance for every 10 juice, starting at
         // 10% chance for 0 juice -- caps out at 100%
         // chance to fire at 90 juice
         if(LCSrandom(10)-pool[p]->juice/10>0)continue;

         if(rangedweapon(pool[p]->weapon))
         {
            char conf=0;
            if(pool[p]->weapon.ammo>0)conf=1;
            if(ammotype(pool[p]->weapon.type)!=-1)
            {
               if(pool[p]->clip[ammotype(pool[p]->weapon.type)]>0)conf=1;
            }

            if(conf)
            {
               vector<int> goodtarg;
               vector<int> badtarg;

               for(int e=0;e<ENCMAX;e++)
               {
                  if(encounter[e].alive&&encounter[e].exists)
                  {
                     if(encounter[e].enemy())goodtarg.push_back(e);
                     else badtarg.push_back(e);
                  }
               }

               if(goodtarg.size()==0)return;

               int target=goodtarg[LCSrandom(goodtarg.size())];

               char mistake=0;

               if(badtarg.size()>0 && !LCSrandom(10))
               {
                  target=badtarg[LCSrandom(badtarg.size())];
                  mistake=1;
               }

               char actual;
               attack(*pool[p],encounter[target],mistake,actual);

               if(actual)
               {
                  if(mistake)
                  {
                     alienationcheck(mistake);
                     sitestory->crime.push_back(CRIME_ATTACKED_MISTAKE);
                     sitecrime+=10;
                     addjuice(*(pool[p]),-10);
                  }

                  
                  if(pool[p]->weapon.type==WEAPON_NONE)
                     criminalize(*pool[p],LAWFLAG_ASSAULT);
                  else
                     criminalize(*pool[p],LAWFLAG_ARMEDASSAULT);
               }

               if(!encounter[target].alive)delenc(target,1);
            }
         }
      }
   }
}



void enemyattack(void)
{
   goodguyattack = false;

   bool armed=false;
   for(int i=0;i<6;i++)
   {
      if(activesquad->squad[i]==NULL)break;
      if(activesquad->squad[i]->weapon.type!=WEAPON_NONE)
         armed=true;
   }

#ifdef NOENEMYATTACK
   return;
#endif

   int e2, e;
   char printed;
   for(e=0;e<ENCMAX;e++)
   {
      if(!encounter[e].exists)continue;
      if(!encounter[e].alive)continue;

      if(sitealarm==1&&encounter[e].type==CREATURE_BOUNCER&&encounter[e].align!=ALIGN_LIBERAL)
         conservatise(encounter[e]);
      if(encounter[e].enemy())
         encounter[e].cantbluff=2;

      if(mode!=GAMEMODE_CHASECAR)
      {
         // Encountered creature will flee if:
         // (a) Non-Conservative, and not recently converted via music or some other mechanism
         // (b) Conservative, no juice, unarmed, non-tank/animal, enemy is armed, and fails a morale check based in part on injury level
         // (c) Conservative, and lost more than 55% blood
         // (d) There's a fire, they are not firefighters, and they fail a random check
         // Encountered creatures will never flee if they are tanks, animals, or so hurt they can't move
         char fire=0;
         if(mode==GAMEMODE_SITE)
         {
            if(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_START ||
               levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_END)
            {
               fire=1;
            }
            else if(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK)
            {
               fire=2;
            }
         }

         if(((!encounter[e].enemy()||
            (encounter[e].juice==0&&encounter[e].weapon.type==WEAPON_NONE&&armed&&encounter[e].blood<signed(70+LCSrandom(61))))
            &&!(encounter[e].flag & CREATUREFLAG_CONVERTED))||encounter[e].blood<45
            ||((fire*LCSrandom(5)>=3)&&!(encounter[e].type==CREATURE_FIREFIGHTER)))
         {
            if(!incapacitated(encounter[e],0,printed)&&encounter[e].animalgloss==ANIMALGLOSS_NONE)
            {
               if(printed)
               {
                  printparty();
                  if(mode==GAMEMODE_CHASECAR||
                     mode==GAMEMODE_CHASEFOOT)printchaseencounter();
                  else printencounter();

                  refresh();
                  getch();
               }

               clearmessagearea();

               move(16,1);
               addstr(encounter[e].name);
               if((encounter[e].wound[BODYPART_LEG_RIGHT] & WOUND_NASTYOFF)||
                  (encounter[e].wound[BODYPART_LEG_RIGHT] & WOUND_CLEANOFF)||
                  (encounter[e].wound[BODYPART_LEG_LEFT] & WOUND_NASTYOFF)||
                  (encounter[e].wound[BODYPART_LEG_LEFT] & WOUND_CLEANOFF)||
                  (encounter[e].blood<45))
               {
                  switch(LCSrandom(9))
                  {
                     case 0:addstr(" crawls off moaning...");break;
                     case 1:addstr(" crawls off whimpering...");break;
                     case 2:addstr(" crawls off trailing blood...");break;
                     case 3:addstr(" crawls off screaming...");break;
                     case 4:addstr(" crawls off crying...");break;
                     case 5:addstr(" crawls off sobbing...");break;
                     case 6:addstr(" crawls off whispering...");break;
                     case 7:addstr(" crawls off praying...");break;
                     case 8:addstr(" crawls off cursing...");break;
                  }
               }
               else
               {
                  switch(LCSrandom(7))
                  {
                     case 0:addstr(" makes a break for it!");break;
                     case 1:addstr(" escapes crying!");break;
                     case 2:addstr(" runs away!");break;
                     case 3:addstr(" gets out of there!");break;
                     case 4:addstr(" runs hollering!");break;
                     case 5:addstr(" bolts out of there!");break;
                     case 6:addstr(" runs away screaming!");break;
                  }
               }

               delenc(e,0);
               e--;

               printparty();
               if(mode==GAMEMODE_CHASECAR||
                     mode==GAMEMODE_CHASEFOOT)printchaseencounter();
               else printencounter();

               refresh();
               getch();
            }
            continue;
         }
      }

      vector<int> goodtarg;
      vector<int> badtarg;

      if(encounter[e].enemy())
      {
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)
            {
               if(activesquad->squad[p]->alive)goodtarg.push_back(p);
            }
         }
      }
      else
      {
         for(e2=0;e2<ENCMAX;e2++)
         {
            if(!encounter[e2].exists)continue;
            if(!encounter[e2].alive)continue;
            if(encounter[e2].align!=-1)continue;

            goodtarg.push_back(e2);
         }
      }

      for(e2=0;e2<ENCMAX;e2++)
      {
         if(!encounter[e2].exists)continue;
         if(!encounter[e2].alive)continue;
         if(encounter[e2].enemy())continue;

         badtarg.push_back(e2);
      }

      if(goodtarg.size()==0)return;

      int target=goodtarg[LCSrandom(goodtarg.size())];

      char canmistake=1;

      int encnum=0;
      for(e2=0;e2<ENCMAX;e2++)if(encounter[e2].exists)encnum++;

      if((encounter[e].type==CREATURE_SCIENTIST_EMINENT||
         encounter[e].type==CREATURE_JUDGE_LIBERAL||
         encounter[e].type==CREATURE_JUDGE_CONSERVATIVE||
         encounter[e].type==CREATURE_CORPORATE_CEO||
         encounter[e].type==CREATURE_RADIOPERSONALITY||
         encounter[e].type==CREATURE_NEWSANCHOR)&&encnum<ENCMAX)canmistake=0;

      char actual;
      if(canmistake)
      {
         if(encounter[e].enemy())
         {
            if(activesquad->squad[target]->prisoner!=NULL && !LCSrandom(2))
            {
               attack(encounter[e],*activesquad->squad[target]->prisoner,1,actual);
               if(!activesquad->squad[target]->prisoner->alive)
               {
                  if(activesquad->squad[target]->prisoner->squadid==-1)
                  {
                     clearmessagearea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr(activesquad->squad[target]->name);
                     addstr(" drops ");
                     addstr(activesquad->squad[target]->prisoner->name);
                     addstr("'s body.");

                     if(activesquad->squad[target]->prisoner->type==CREATURE_CORPORATE_CEO||
                        activesquad->squad[target]->prisoner->type==CREATURE_RADIOPERSONALITY||
                        activesquad->squad[target]->prisoner->type==CREATURE_NEWSANCHOR||
                        activesquad->squad[target]->prisoner->type==CREATURE_SCIENTIST_EMINENT||
                        activesquad->squad[target]->prisoner->type==CREATURE_JUDGE_CONSERVATIVE)sitecrime+=30;

                     makeloot(*activesquad->squad[target]->prisoner,groundloot);

                     refresh();
                     getch();

                     delete activesquad->squad[target]->prisoner;
                     activesquad->squad[target]->prisoner=NULL;
                  }
               }
               continue;
            }
         }

         if(!LCSrandom(10)&&badtarg.size()>0)
         {
            target=badtarg[LCSrandom(badtarg.size())];
            if(encounter[target].flag & CREATUREFLAG_CONVERTED)
               attack(encounter[e],encounter[target],0,actual);
            else
               attack(encounter[e],encounter[target],1,actual);
            if(!encounter[target].alive)delenc(target,1);
            continue;
         }
      }

      if(encounter[e].enemy())
         attack(encounter[e],*activesquad->squad[target],0,actual);
      else
         attack(encounter[e],encounter[target],0,actual);
   }
}



/* attack handling for an individual creature and its target */
void attack(Creature &a,Creature &t,char mistake,char &actual,bool force_melee)
{
   actual=0;

   char str[200],str2[200];

   clearmessagearea();
   if (goodguyattack)
   {
      set_color(COLOR_GREEN,COLOR_BLACK,1);
   }
   else
   {
      set_color(COLOR_RED,COLOR_BLACK,1);
   }

   //INCAPACITATED
   char incaprint;
   a.forceinc=0;
   if(incapacitated(a,0,incaprint))
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

      a.forceinc=1;

      return;
   }

   //SPECIAL ATTACK!
   int encnum=0;
   for(int e=0;e<ENCMAX;e++)if(encounter[e].exists)encnum++;

   if(!force_melee &&
      (((a.type==CREATURE_COP&&a.align==ALIGN_MODERATE&&a.enemy())||
      a.type==CREATURE_SCIENTIST_EMINENT||
      a.type==CREATURE_JUDGE_LIBERAL||
      a.type==CREATURE_JUDGE_CONSERVATIVE||
      a.type==CREATURE_CORPORATE_CEO||
      a.type==CREATURE_RADIOPERSONALITY||
      a.type==CREATURE_NEWSANCHOR||
      a.weapon.type==WEAPON_GUITAR)&&!mistake&&
      (a.weapon.type==WEAPON_GUITAR||a.weapon.type==WEAPON_NONE||a.align!=1)))
   {
      if(a.align==1||encnum<ENCMAX)
      {
         specialattack(a, t, actual);
         return;
      }
   }

   if(mode==GAMEMODE_CHASECAR&&                                   // If in a car
      (!a.weapon.ranged()||force_melee||                    // And either using a melee weapon
       (a.weapon.ammo==0&&                                  // Or, out of ammo and...
        a.clip[ammotype(a.weapon.type)]==0)))               // ...no clips left
   {
      return;                                               // Then bail, they can't fight
   }

   // Grenade type weapons can't be used in melee!
   if(weaponskill(a.weapon.type)==SKILL_THROWING && force_melee)
      return;

   //RELOAD
   if(ammotype(a.weapon.type)!=-1&&a.weapon.ammo==0&&!force_melee)
   {
      if(a.clip[ammotype(a.weapon.type)]>0)
      {
         switch(ammotype(a.weapon.type))
         {
            case CLIP_9:a.weapon.ammo+=15;break;
            case CLIP_45:a.weapon.ammo+=15;break;
            case CLIP_ASSAULT:a.weapon.ammo+=30;break;
            case CLIP_SMG:a.weapon.ammo+=15;break;
            case CLIP_38:a.weapon.ammo+=6;break;
            case CLIP_44:a.weapon.ammo+=6;break;
            case CLIP_50AE:a.weapon.ammo+=7;break;
            case CLIP_BUCKSHOT:a.weapon.ammo+=6;break;
            case CLIP_MOLOTOV:a.weapon.ammo+=1;break;
            case CLIP_GASOLINE:a.weapon.ammo+=2;break;
         }

         a.clip[ammotype(a.weapon.type)]--;

         strcpy(str,a.name);
         if(a.weapon.type!=WEAPON_MOLOTOV)
            strcat(str," reloads.");
         else
            strcat(str," readies another molotov.");

         move(16,1);
         addstr(str);

         printparty();
         if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
         else printencounter();

         refresh();
         getch();

         return;
      }
      else if(a.weapon.type==WEAPON_MOLOTOV) a.weapon.type = WEAPON_NONE;
   }

   bool melee=true;

   strcpy(str,a.name);
   strcat(str," ");
   if(mistake)strcat(str,"MISTAKENLY ");
   switch(a.weapon.type)
   {
      case WEAPON_NONE:
      {
         if(!a.animalgloss)
         {
            if(!LCSrandom(a.get_skill(SKILL_HANDTOHAND)+1))
               strcat(str,"punches");
            else if(!LCSrandom(a.get_skill(SKILL_HANDTOHAND)))
               strcat(str,"swings at");
            else if(!LCSrandom(a.get_skill(SKILL_HANDTOHAND)-1))
               strcat(str,"grapples with");
            else if(!LCSrandom(a.get_skill(SKILL_HANDTOHAND)-2))
               strcat(str,"kicks");
            else if(!LCSrandom(a.get_skill(SKILL_HANDTOHAND)-3))
               strcat(str,"strikes at");
            else if(!LCSrandom(a.get_skill(SKILL_HANDTOHAND)-4))
               strcat(str,"jump kicks");
            else
               strcat(str,"gracefully strikes at");
         }
         else
         {
            if(a.specialattack==ATTACK_CANNON)
            {
               strcat(str,"shoots at");
               melee=false;
            }
            else if(a.specialattack==ATTACK_FLAME)strcat(str,"breathes fire at");
            else if(a.specialattack==ATTACK_SUCK)strcat(str,"stabs");
            else strcat(str,"claws at");
         }
         break;
      }
      case WEAPON_SWORD:
      case WEAPON_KNIFE:
      case WEAPON_SHANK:
         strcat(str,"slashes at");break;
      case WEAPON_AXE:
         strcat(str,"chops at");break;
      case WEAPON_SYRINGE:
         strcat(str,"pokes at");break;
      case WEAPON_REVOLVER_38:
      case WEAPON_REVOLVER_44:
      case WEAPON_DESERT_EAGLE:
      case WEAPON_SEMIPISTOL_9MM:
      case WEAPON_SEMIPISTOL_45:
      case WEAPON_SEMIRIFLE_AR15:
      case WEAPON_SMG_MP5:
      case WEAPON_CARBINE_M4:
      case WEAPON_AUTORIFLE_M16:
      case WEAPON_AUTORIFLE_AK47:
      case WEAPON_SHOTGUN_PUMP:
         if(a.weapon.ammo>0&&!force_melee)
         {
            strcat(str,"shoots at");
            melee=false;
         }
         else strcat(str,"swings at");
         break;
      case WEAPON_MOLOTOV:
         strcat(str,"hurls a molotov at");
         melee=false;
         break;
      case WEAPON_CHAIN:
      case WEAPON_NIGHTSTICK:
      case WEAPON_CROWBAR:
      case WEAPON_BASEBALLBAT:
      case WEAPON_GAVEL:
      case WEAPON_DAISHO:
      case WEAPON_HAMMER:
      case WEAPON_MAUL:
      case WEAPON_CROSS:
      case WEAPON_STAFF:
      case WEAPON_TORCH:
      case WEAPON_SPRAYCAN:
      case WEAPON_GUITAR:
         strcat(str,"swings at");break;
      case WEAPON_PITCHFORK:
         strcat(str,"stabs");break;
      case WEAPON_FLAMETHROWER:
         if(a.weapon.ammo>0&&!force_melee)
         {
            strcat(str,"streams fire at");
            melee=false;
         }
         else strcat(str,"swings at");break;
      
   }
   strcat(str," ");
   strcat(str,t.name);
   move(16,1);
   addstr(str);

   strcpy(str,"");

   if(a.weapon.type!=WEAPON_NONE &&
      a.weapon.type!=WEAPON_MOLOTOV)
   {
      strcat(str," with a ");
      getweaponfull(str2,a.weapon.type,1);
      strcat(str,str2);
      //strcat(str," and ");
   }
   else
   {
      //strcat(str,"and ");
   }
   strcat(str,"!");
   addstr(str);

   refresh();
   getch();

   if (goodguyattack)
   {
      set_color(COLOR_GREEN,COLOR_BLACK,1);
   }
   else
   {
      set_color(COLOR_RED,COLOR_BLACK,1);
   }
   strcpy(str,a.name);

   int bonus=0; // Accuracy bonus or penalty that does NOT affect damage or counterattack chance

   //SKILL EFFECTS
   int wsk=weaponskill(a.weapon.type);
   if(rangedweapon(a.weapon) &&
      ((a.weapon.type!=WEAPON_MOLOTOV &&
        a.weapon.ammo==0) || force_melee))
   {
      wsk=SKILL_CLUB; // Club people with out-of-ammo guns
   }// TODO: Bigger gun should mean more melee damage.

   // Basic roll
   int aroll = a.skill_roll(wsk);

   int droll = t.skill_roll(SKILL_DODGE) / 2;
   t.train(SKILL_DODGE,aroll*2);
   a.train(wsk,droll*2+5);

   // Hostages interfere with attack
   if(t.prisoner!=NULL)bonus-=LCSrandom(10);
   if(a.prisoner!=NULL)aroll-=LCSrandom(10);

   //Injured people suck at attacking, are like fish in a barrel to attackers
   healthmodroll(aroll,a);
   healthmodroll(droll,t);

   // If in a foot chance, double the debilitating effect of injuries
   if(mode==GAMEMODE_CHASEFOOT)
   {
      healthmodroll(aroll,a);
      healthmodroll(droll,t);
   }

   // Prevent negative rolls
   if(aroll<0)aroll=0;
   if(droll<0)droll=0;

   if(!melee)
   {
      // Weapon accuracy bonuses and pentalties
      if(a.weapon.type==WEAPON_SHOTGUN_PUMP)bonus=2;
      if(a.weapon.type==WEAPON_SMG_MP5)bonus=2;
      if(a.weapon.type==WEAPON_CARBINE_M4)bonus=2;
      //if(a.weapon.type==WEAPON_AUTORIFLE_M16)bonus=1;
      //if(a.weapon.type==WEAPON_AUTORIFLE_AK47)bonus=1;
   }

   //USE BULLETS
   int bursthits=0; // *JDS* Used for fully automatic weapons; tracks multiple hits

   int removeweapon=0; // Used by molotov to remove the weapon at the end of the turn if needed

   if(a.weapon.type==WEAPON_NONE)
   {
      // Martial arts multi-strikes
      bursthits=1+LCSrandom(a.get_skill(SKILL_HANDTOHAND)/3+1);
      if(bursthits>5)bursthits=5;
      if(a.animalgloss)bursthits=1; // Whoops, must be human to use martial arts fanciness
   }

   if(a.weapon.ammo>0 && !force_melee)
   {
      switch(a.weapon.type)
      {
      case WEAPON_MOLOTOV:
         a.weapon.ammo--;
         if(a.clip[CLIP_MOLOTOV]==0)
            removeweapon=1;

         if(mode==GAMEMODE_SITE)
         {
            sitechangest change(locx,locy,locz,SITEBLOCK_DEBRIS);
            location[cursite]->changes.push_back(change);

            // Fire!
            if(!(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_END) ||
               !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK) ||
               !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_START) ||
               !(levelmap[locx][locy][locz].flag & SITEBLOCK_DEBRIS))
            {
               levelmap[locx][locy][locz].flag|=SITEBLOCK_FIRE_START;
               sitecrime+=3;
               addjuice(a,3);
               criminalizeparty(LAWFLAG_ARSON);
               sitestory->crime.push_back(CRIME_ARSON);
            }
         }
         break;
      case WEAPON_FLAMETHROWER:
         a.weapon.ammo--;
         if(mode==GAMEMODE_SITE)
         {
            if((!(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_END) ||
                !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK) ||
                !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_START) ||
                !(levelmap[locx][locy][locz].flag & SITEBLOCK_DEBRIS)) &&!LCSrandom(3))
            {
               levelmap[locx][locy][locz].flag|=SITEBLOCK_FIRE_START;
               sitecrime+=3;
               addjuice(a,3);
               criminalizeparty(LAWFLAG_ARSON);
               sitestory->crime.push_back(CRIME_ARSON);
            }
         }
         break;
      case WEAPON_SHOTGUN_PUMP:
      case WEAPON_REVOLVER_38:
      case WEAPON_REVOLVER_44:
      case WEAPON_DESERT_EAGLE:
      case WEAPON_SEMIPISTOL_9MM:
      case WEAPON_SEMIPISTOL_45:
      case WEAPON_SEMIRIFLE_AR15:
         a.weapon.ammo--;
         break;
      // *JDS* automatic weapons fire and maybe hit with three shots
      case WEAPON_SMG_MP5:
         for(int i=0;i<3&&a.weapon.ammo;i++)
         {
            a.weapon.ammo--;
            // Each shot in a burst is increasingly less likely to hit
            if(aroll+bonus-i*4>droll)bursthits++;
         }
         //if(bursthits==0)bursthits=1; // Not needed with the way MP5 bursts are calculated now
         break;
      case WEAPON_CARBINE_M4:
      case WEAPON_AUTORIFLE_M16: 
      case WEAPON_AUTORIFLE_AK47:
         for(int i=0;i<3&&a.weapon.ammo;i++)
         {
            a.weapon.ammo--;
            // Each shot in a burst is increasingly less likely to hit
            if(aroll+bonus-i*3>droll)bursthits++;
         }
         break;
      }
   }

   //HIT!
   if(aroll+bonus>droll)
   {
      strcat(str," hits the ");
      int w;
      bool canhit=false;

      for(w=0;w<BODYPARTNUM;w++)
      {
         if(!(t.wound[w]&WOUND_CLEANOFF) && !(t.wound[w]&WOUND_NASTYOFF))
         {
            canhit=true;
            break;
         }
      }
      
      do
      {
         int offset=0;
         if(aroll>droll+5 || mode==GAMEMODE_CHASECAR)
            offset=4;  // NICE SHOT; MORE LIKELY TO HIT BODY/HEAD or it's a car chase and we don't want to hit the car too much
         if(aroll>droll+10)
            offset=8;  // NO LIMB HITS HERE YOU AWESOME PERSON
         if(aroll>droll+15)
            offset=12; // BOOM AUTOMATIC HEADSHOT MOTHA******
         //Weighted location roll:
         //200% chance to hit body
         //50% chance to hit head
         w=offset+LCSrandom(13-offset);

         switch(w)
         {
         case 12:w=BODYPART_HEAD;break;
         case 11:w=BODYPART_BODY;break;
         case 10:w=BODYPART_BODY;break;
         case 9:w=BODYPART_BODY;break;
         case 8:w=BODYPART_BODY;break;
         case 7:w=BODYPART_ARM_RIGHT;break;
         case 6:w=BODYPART_ARM_LEFT;break;
         case 5:w=BODYPART_LEG_RIGHT;break;
         case 4:w=BODYPART_LEG_LEFT;break;
         case 3:w=BODYPART_ARM_RIGHT;break;
         case 2:w=BODYPART_ARM_LEFT;break;
         case 1:w=BODYPART_LEG_RIGHT;break;
         case 0:w=BODYPART_LEG_LEFT;break;
         }
      }while(((t.wound[w]&WOUND_CLEANOFF) || (t.wound[w]&WOUND_NASTYOFF)) && canhit==true);

      if(t.animalgloss==ANIMALGLOSS_TANK)
      {
         switch(w)
         {
            case BODYPART_HEAD:strcat(str,"turret");break;
            case BODYPART_BODY:strcat(str,"front");break;
            case BODYPART_ARM_RIGHT:strcat(str,"right side");break;
            case BODYPART_ARM_LEFT:strcat(str,"left side");break;
            case BODYPART_LEG_RIGHT:strcat(str,"right tread");break;
            case BODYPART_LEG_LEFT:strcat(str,"left tread");break;
         }
      }
      else if(t.animalgloss==ANIMALGLOSS_ANIMAL) // FIXME: What about Six-legged Pigs?
      {
         switch(w)
         {
            case BODYPART_HEAD:strcat(str,"head");break;
            case BODYPART_BODY:strcat(str,"body");break;
            case BODYPART_ARM_RIGHT:strcat(str,"right front leg");break;
            case BODYPART_ARM_LEFT:strcat(str,"left front leg");break;
            case BODYPART_LEG_RIGHT:strcat(str,"right rear leg");break;
            case BODYPART_LEG_LEFT:strcat(str,"left rear leg");break;
         }
      }
      else
      {
         switch(w)
         {
            case BODYPART_HEAD:strcat(str,"head");break;
            case BODYPART_BODY:strcat(str,"body");break;
            case BODYPART_ARM_RIGHT:strcat(str,"right arm");break;
            case BODYPART_ARM_LEFT:strcat(str,"left arm");break;
            case BODYPART_LEG_RIGHT:
               if(mode!=GAMEMODE_CHASECAR)
               {
                  strcat(str,"right leg");
               }
               else
               {
                  strcat(str,"car");
                  aroll=-20;
               }
               break;
            case BODYPART_LEG_LEFT:
               if(mode!=GAMEMODE_CHASECAR)
               {
                  strcat(str,"left leg");
               }
               else
               {
                  strcat(str,"car");
               }
               break;
         }
      }

      // show multiple hits
      if(bursthits>1)
      {
         if(a.weapon.type==WEAPON_NONE)
            strcat(str,", striking");

         switch(bursthits)
         {
         case 1: break;
         case 2: strcat(str," twice"); break;
         case 3: strcat(str," three times"); break;
         case 4: strcat(str," four times"); break;
         case 5: strcat(str," five times"); break;
         }

      }

      if(a.weapon.type==WEAPON_MOLOTOV)
      {
         strcat(str,", exploding");
      }

      char damtype=0;
      int damamount=0;
      char strengthmin=1;
      char strengthmax=1;
      int severtype=-1;

      char damagearmor=0;
      char armorpiercing=0;

      switch(a.weapon.type) // *JDS* removed sever types from many weapons (all guns except shotgun, knives...)
      {
         case WEAPON_NONE:
             
            while(bursthits)
            {
               damamount+=LCSrandom(5+a.get_skill(SKILL_HANDTOHAND))+1+a.get_skill(SKILL_HANDTOHAND);
               bursthits--;
            }
            if(!a.animalgloss)damtype|=WOUND_BRUISED;
            else
            {
               if(a.specialattack==ATTACK_CANNON)
               {
                  damamount=LCSrandom(500)+500;
                  damtype|=WOUND_BURNED;
                  damtype|=WOUND_TORN;
                  damtype|=WOUND_SHOT;
               }
               else if(a.specialattack==ATTACK_FLAME)damtype|=WOUND_BURNED;
               else if(a.specialattack==ATTACK_SUCK)damtype|=WOUND_CUT;
               else damtype|=WOUND_TORN;
               severtype=WOUND_NASTYOFF;
            }
            strengthmin=5;
            strengthmax=10;

            break;
         case WEAPON_MAUL:
            damtype|=WOUND_BRUISED;
            damamount=LCSrandom(41)+5;
            strengthmin=8;
            strengthmax=18;
            
            break;
         case WEAPON_BASEBALLBAT:
            damtype|=WOUND_BRUISED;
            damamount=LCSrandom(41)+5;
            strengthmin=6;
            strengthmax=12;
             
            break;
         case WEAPON_PITCHFORK:
            damtype|=WOUND_CUT;
            damtype|=WOUND_BLEEDING;
            damamount=LCSrandom(61)+10;
            strengthmin=6;
            strengthmax=12;
            damagearmor=1;
            armorpiercing=1;
             
            break;
         case WEAPON_TORCH:
            damtype|=WOUND_BURNED;
            damamount=LCSrandom(11)+5;
            damagearmor=1;
             
            break;
         case WEAPON_SHANK:
            damtype|=WOUND_CUT;
            damtype|=WOUND_BLEEDING;
            damamount=LCSrandom(31)+10;
            strengthmin=1;
            strengthmax=4;
            damagearmor=1;
            armorpiercing=1;
             
            break;
         case WEAPON_KNIFE:
            damtype|=WOUND_CUT;
            damtype|=WOUND_BLEEDING;
            damamount=LCSrandom(61)+10;
            strengthmin=1;
            strengthmax=4;
            damagearmor=1;
            armorpiercing=2;
             
            break;
         case WEAPON_SYRINGE:
            damtype|=WOUND_CUT;
            damamount=LCSrandom(4)+1;
            strengthmin=1;
            strengthmax=2;
            armorpiercing=4;
            
            break;
         case WEAPON_REVOLVER_38:
            if(a.weapon.ammo>0)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               damamount=LCSrandom(141)+10;
               //severtype=WOUND_NASTYOFF; *JDS* no dismemberment from revolvers
               damagearmor=1;
               armorpiercing=3;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(6)+5;
               strengthmin=2;
               strengthmax=6;
                
            }
            break;
         case WEAPON_REVOLVER_44:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               damamount=LCSrandom(276)+10;
               //severtype=WOUND_NASTYOFF; *JDS* no dismemberment from revolvers
               damagearmor=1;
               armorpiercing=4;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(6)+5;
               strengthmin=2;
               strengthmax=6;
                
            }
            break;
         case WEAPON_DESERT_EAGLE:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               damamount=LCSrandom(301)+10;
               severtype=WOUND_NASTYOFF; // We're just going to go ahead and continue
               damagearmor=1;            // the mythology of the Deagle by letting it
               armorpiercing=4;          // blow limbs off... --Fox
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(6)+5;
               strengthmin=3;
               strengthmax=8;
                
            }
            break;
         case WEAPON_SEMIPISTOL_9MM:
         case WEAPON_SEMIPISTOL_45:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               damamount=LCSrandom(181)+10;
               //severtype=WOUND_NASTYOFF; *JDS* no dismemberment from semi-automatics
               damagearmor=1;
               armorpiercing=4;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(6)+5;
               strengthmin=2;
               strengthmax=6;
                
            }
            break;
         case WEAPON_SEMIRIFLE_AR15:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;

               damamount=LCSrandom(251)+10;

               damagearmor=1;
               armorpiercing=7;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(21)+5;
               strengthmin=6;
               strengthmax=12;
                
            }
            break;
         case WEAPON_MOLOTOV:
            damtype|=WOUND_BURNED;
            damamount=LCSrandom(101)+25;
            damagearmor=1;
            armorpiercing=7;
            break;
         case WEAPON_FLAMETHROWER:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_BURNED;
               damamount=LCSrandom(201)+50;
               damagearmor=1;
               armorpiercing=9;
            } else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(21)+5;
               strengthmin=12;
               strengthmax=18;
                
            }
            break;
         case WEAPON_AUTORIFLE_AK47:
         case WEAPON_AUTORIFLE_M16:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               if(bursthits==3)severtype=WOUND_NASTYOFF; //*JDS* dismemberment from assault rifles requires 3 hits

               damamount=0;
               while(bursthits)
               {
                  damamount+=LCSrandom(251)+10;
                  bursthits--;
               }
               damagearmor=1;
               armorpiercing=7;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(21)+5;
               strengthmin=6;
               strengthmax=12;
                
            }
            break;
         case WEAPON_CARBINE_M4:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               if(bursthits==3)severtype=WOUND_NASTYOFF; //*JDS* dismemberment from assault rifles requires 3 hits

               damamount=0;
               while(bursthits)
               {
                  damamount+=LCSrandom(221)+10;
                  bursthits--;
               }
               damagearmor=1;
               armorpiercing=7;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(25)+5;
               strengthmin=6;
               strengthmax=12;
                
            }
            break;
         case WEAPON_SMG_MP5:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               //if(bursthits==3)severtype=WOUND_NASTYOFF; //*JDS* dismemberment from assault rifles requires 3 hits

               damamount=0;
               while(bursthits)
               {
                  damamount+=LCSrandom(181)+10;
                  bursthits--;
               }
               damagearmor=1;
               armorpiercing=4;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(21)+5;
               strengthmin=5;
               strengthmax=10;
                
            }
            break;
         case WEAPON_SHOTGUN_PUMP:
            if(a.weapon.ammo>0 && !force_melee)
            {
               damtype|=WOUND_SHOT;
               damtype|=WOUND_BLEEDING;
               if(!LCSrandom(3))
               {
                  damamount=LCSrandom(301)+10;
                  severtype=WOUND_NASTYOFF; // *JDS* dismemberment OK with shotgun
               }
               else
               {
                  damamount=LCSrandom(201)+10;
               }
               
               
               damagearmor=1;
            }
            else
            {
               damtype|=WOUND_BRUISED;
               damamount=LCSrandom(21)+5;
               strengthmin=6;
               strengthmax=12;
                
            }
            break;
         case WEAPON_DAISHO:
         case WEAPON_SWORD:
         case WEAPON_AXE:
            damtype|=WOUND_CUT;
            damtype|=WOUND_BLEEDING;
            damamount=LCSrandom(101)+10;
            strengthmin=6;
            strengthmax=12;
            severtype=WOUND_CLEANOFF;
            damagearmor=1;
            armorpiercing=2;
             
            break;
         case WEAPON_HAMMER:
            damtype|=WOUND_BRUISED;
            damtype|=WOUND_BLEEDING;
            damamount=LCSrandom(21)+10;
            strengthmin=8;
            strengthmax=18;
            break;
         case WEAPON_CROWBAR:
            damtype|=WOUND_BRUISED;
            damtype|=WOUND_BLEEDING;
            damamount=LCSrandom(21)+10;
            strengthmin=4;
            strengthmax=8;
            break;
         case WEAPON_STAFF:
            damtype|=WOUND_BRUISED;
            damamount=LCSrandom(21)+5;
            strengthmin=6;
            strengthmax=12;
            break;
         case WEAPON_GUITAR:
         case WEAPON_NIGHTSTICK:
            damtype|=WOUND_BRUISED;
            damamount=LCSrandom(21)+5;
            strengthmin=4;
            strengthmax=8;
            break;
         case WEAPON_GAVEL:
            damtype|=WOUND_BRUISED;
            damamount=LCSrandom(6)+5;
            strengthmin=1;
            strengthmax=4;
            break;
         case WEAPON_CHAIN:
            damtype|=WOUND_BRUISED;
            damamount=LCSrandom(11)+5;
            strengthmin=1;
            strengthmax=6;
            break;
         case WEAPON_SPRAYCAN:
         case WEAPON_CROSS:
            damtype|=WOUND_BRUISED;
            damamount=LCSrandom(11)+5;
            strengthmin=1;
            strengthmax=4;
            break;
      }

      // Coarse combat lethality reduction.
      damamount/=4;

      if(t.squadid!=-1&&t.hireid==-1) // Plot Armor: if the founder is hit, inflict
         damamount/=2;                // 1/4 damage, because founders are cool

      int mod=0;

      if(strengthmax>strengthmin)
      {
         // Melee attacks: Maximum strength bonus, minimum
         // strength to deliver full damage
         int strength = a.attribute_roll(ATTRIBUTE_STRENGTH);
         if(strength>strengthmax)strength = strengthmax;
         mod+=strength-strengthmin;
      }

      //SKILL BONUS FOR GOOD ROLL
      mod+=aroll-droll-5;

      //DO THE HEALTH MOD ON THE WOUND
      mod -= t.attribute_roll(ATTRIBUTE_HEALTH)-5;

      //Health and poor accuracy will only avoid critical hits, not stop low-damage attacks
      if(mod<0)mod = 0;


      damagemod(t,damtype,damamount,w,armorpiercing,mod);

      
      // Temporary debug output for the damage roll
      #ifdef SHOWMECHANICS
      {
         char str[200];
         clearmessagearea();
         move(16,1);
         sprintf(str,"(ATK %d, DEF %d, DAMMOD %d, DAMAGE %d, AP %d)",aroll,droll,mod,damamount,armorpiercing);
         addstr(str);
         refresh();
         getch();
      }
      #endif

      if(mode==GAMEMODE_CHASECAR && (w == BODYPART_LEG_LEFT || w == BODYPART_LEG_RIGHT))
      {
         damamount=0; // no damage to shots to the car body
      }   

      if(damamount>0)
      {
         Creature *target=0;

         

         if(t.squadid!=-1&&t.hireid==-1&& //if the founder is hit...
            (damamount>t.blood||damamount>=10)&& //and lethal or potentially crippling damage is done...
            (w==BODYPART_HEAD||w==BODYPART_BODY)) //to a critical bodypart...
         {
            //Oh Noes!!!! Find a liberal to jump in front of the bullet!!!
            for(int i=0;i<6;i++)
            {
               if(activesquad->squad[i]==NULL)break;
               if(activesquad->squad[i]==&t)break;
               if(activesquad->squad[i]->get_attribute(ATTRIBUTE_HEART,true)>8&&
                  activesquad->squad[i]->get_attribute(ATTRIBUTE_AGILITY,true)>4)
               {
                  target=activesquad->squad[i];
                  
                  clearmessagearea();
			      if (goodguyattack) set_color(COLOR_GREEN,COLOR_BLACK,1);
				  else set_color(COLOR_RED,COLOR_BLACK,1);

                  move(16,1);
                  addstr(target->name);
                  if(!t.alive)addstr(" misguidedly ");
                  else addstr(" heroically ");
                  addstr(" shields ");
                  addstr(t.name);
                  if(!t.alive)addstr("'s corpse");
                  addstr("!");
                  
                  addjuice(*target,10);//Instant juice!! Way to take the bullet!!
                  
                  refresh();
                  getch();
                  break;
               }
            }
         }
         if(!target)target=&t;//If nobody jumps in front of the attack, 

         target->wound[w]|=damtype;

         int severamount=100;

         switch(w)
         {
            case BODYPART_HEAD:severamount=100;break;
            case BODYPART_BODY:severamount=1000;break;
            case BODYPART_ARM_RIGHT:severamount=200;break;
            case BODYPART_ARM_LEFT:severamount=200;break;
            case BODYPART_LEG_RIGHT:severamount=400;break;
            case BODYPART_LEG_LEFT:severamount=400;break;
         }

         if(severtype!=-1 && damamount>=severamount)
         {
            target->wound[w]|=(char)severtype;
         }

         if(w!=BODYPART_HEAD && w!=BODYPART_BODY && target->blood-damamount<=0 &&
            target->blood>0)
         {
            do
            {
               if(a.weapon.type==WEAPON_MOLOTOV && !LCSrandom(3))
                  break;
               else
                  damamount>>=1;
            }while(target->blood-damamount<=0);
         }


         
         if(damagearmor)armordamage(target->armor,w);

         target->blood-=damamount;

         levelmap[locx][locy][locz].flag|=SITEBLOCK_BLOODY;

         if((target->wound[BODYPART_HEAD] & WOUND_CLEANOFF)||
            (target->wound[BODYPART_BODY] & WOUND_CLEANOFF)||
            (target->wound[BODYPART_HEAD] & WOUND_NASTYOFF)||
            (target->wound[BODYPART_BODY] & WOUND_NASTYOFF)||
            target->blood<=0)
         {
            if((target->wound[BODYPART_HEAD] & WOUND_NASTYOFF)||
               (target->wound[BODYPART_BODY] & WOUND_NASTYOFF))bloodblast(target->armor);

            char alreadydead=!target->alive;
            
            if(!alreadydead)
            {
               target->die();

               if(t.align==-a.align)
                  addjuice(a,5+t.juice/20); // Instant juice
               else
                  addjuice(a,-(5+t.juice/20));

               if(target->squadid!=-1)
               {
                  if(target->align==1)stat_dead++;
               }
               else if(target->enemy()&&(t.animalgloss!=ANIMALGLOSS_ANIMAL||law[LAW_ANIMALRESEARCH]==2))
               {
                  stat_kills++;
                  if(location[cursite]->siege.siege)location[cursite]->siege.kills++;
                  if(location[cursite]->renting==RENTING_CCS)ccs_siege_kills++;
               }
               if(target->squadid==-1 &&
                  (target->animalgloss!=ANIMALGLOSS_ANIMAL||law[LAW_ANIMALRESEARCH]==2))
               {
                  sitecrime+=10;
                  sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
                  if(a.squadid!=-1)
                     criminalizeparty(LAWFLAG_MURDER);
               }
            }

            if(target->wound[BODYPART_HEAD] & WOUND_CLEANOFF)strcat(str," CUTTING IT OFF!");
            else if(target->wound[BODYPART_BODY] & WOUND_CLEANOFF)strcat(str," CUTTING IT IN HALF!");
            else if(target->wound[BODYPART_HEAD] & WOUND_NASTYOFF)strcat(str," BLOWING IT APART!");
            else if(target->wound[BODYPART_BODY] & WOUND_NASTYOFF)strcat(str," BLOWING IT IN HALF!");
            else if(a.weapon.type!=WEAPON_MOLOTOV)strcat(str,".");
            else strcat(str,"!");
            move(17,1);
            //set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(str);

            refresh();
            getch();

            if(!alreadydead)
            {
               severloot(t,groundloot);

               clearmessagearea();
			   if (goodguyattack) set_color(COLOR_GREEN,COLOR_BLACK,1);
			   else set_color(COLOR_RED,COLOR_BLACK,1);

               adddeathmessage(*target);

               refresh();
               getch();

               if(target->prisoner!=NULL)
               {
                  freehostage(t,1);
               }
            }
         }
         else
         {
            if(target->wound[w] & WOUND_CLEANOFF)strcat(str," CUTTING IT OFF!");
            else if(target->wound[w] & WOUND_NASTYOFF)strcat(str," BLOWING IT OFF!");
            else strcat(str,".");
            
            move(17,1);
            //set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(str);

            if(target->wound[w] & WOUND_NASTYOFF)bloodblast(target->armor);

            printparty();
            if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
            else printencounter();

            refresh();
            getch();

            //SPECIAL WOUNDS
            if(!(target->wound[w] & (WOUND_CLEANOFF|WOUND_NASTYOFF))&&
               !target->animalgloss)
            {
               char heavydam=0;
               char breakdam=0;
               char pokedam=0;
               if(damamount>=12) //JDS -- 2x damage needed
               {
                  if(damtype & WOUND_SHOT)heavydam=1;
                  if(damtype & WOUND_BURNED)heavydam=1;
                  if(damtype & WOUND_TORN)heavydam=1;
                  if(damtype & WOUND_CUT)heavydam=1;
               }

               if(damamount>=10) //JDS -- 2x damage needed
               {
                  if(damtype & WOUND_SHOT)pokedam=1;
                  if(damtype & WOUND_TORN)pokedam=1;
                  if(damtype & WOUND_CUT)pokedam=1;
               }

               if((damtype & WOUND_BRUISED)&&damamount>=50)breakdam=1;
               if((damtype & WOUND_SHOT)&&damamount>=50)breakdam=1;
               if((damtype & WOUND_TORN)&&damamount>=50)breakdam=1;
               if((damtype & WOUND_CUT)&&damamount>=50)breakdam=1;

               if(w==BODYPART_HEAD)
               {
                  clearmessagearea();
                  if (goodguyattack) set_color(COLOR_GREEN,COLOR_BLACK,1);
				  else set_color(COLOR_RED,COLOR_BLACK,1);

                  switch(LCSrandom(7))
                  {
                     case 0:
                        if((target->special[SPECIALWOUND_RIGHTEYE]||
                           target->special[SPECIALWOUND_LEFTEYE]||
                           target->special[SPECIALWOUND_NOSE])&&
                           heavydam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s face is blasted off!");
                           else if(damtype & WOUND_BURNED)addstr("'s face is burned away!");
                           else if(damtype & WOUND_TORN)addstr("'s face is torn off!");
                           else if(damtype & WOUND_CUT)addstr("'s face is cut away!");
                           else addstr("'s face is removed!");
                           refresh();
                           getch();
                           target->special[SPECIALWOUND_RIGHTEYE]=0;
                           target->special[SPECIALWOUND_LEFTEYE]=0;
                           target->special[SPECIALWOUND_NOSE]=0;
                           if(target->blood>20)target->blood=20;
                        }
                        break;
                     case 1:
                        if(target->special[SPECIALWOUND_TEETH]>0)
                        {
                           int teethminus=LCSrandom(TOOTHNUM)+1;
                           if(teethminus>target->special[SPECIALWOUND_TEETH])
                              teethminus=target->special[SPECIALWOUND_TEETH];
                           char num[20];
                           itoa(teethminus,num,10);

                           move(16,1);
                           if(teethminus>1)
                           {
                              addstr(num);
                              addstr(" of ");
                              addstr(target->name);
                              addstr("'s teeth are ");
                           }
                           else if(target->special[SPECIALWOUND_TEETH]>1)
                           {
                              addstr("One of ");
                              addstr(target->name);
                              addstr("'s teeth is ");
                           }
                           else
                           {
                              addstr(target->name);
                              addstr("'s last tooth is ");
                           }

                           if(damtype & WOUND_SHOT)addstr("shot out!");
                           else if(damtype & WOUND_BURNED)addstr("burned away!");
                           else if(damtype & WOUND_TORN)addstr("gouged out!");
                           else if(damtype & WOUND_CUT)addstr("cut out!");
                           else addstr("knocked out!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_TEETH]-=teethminus;
                        }
                        break;
                     case 2:
                        if(target->special[SPECIALWOUND_RIGHTEYE]&&
                           heavydam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s right eye is blasted out!");
                           else if(damtype & WOUND_BURNED)addstr("'s right eye is burned away!");
                           else if(damtype & WOUND_TORN)addstr("'s right eye is torn out!");
                           else if(damtype & WOUND_CUT)addstr("'s right eye is poked out!");
                           else addstr("'s right eye is removed!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_RIGHTEYE]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 3:
                        if(target->special[SPECIALWOUND_LEFTEYE]&&heavydam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s left eye is blasted out!");
                           else if(damtype & WOUND_BURNED)addstr("'s left eye is burned away!");
                           else if(damtype & WOUND_TORN)addstr("'s left eye is torn out!");
                           else if(damtype & WOUND_CUT)addstr("'s left eye is poked out!");
                           else addstr("'s left eye is removed!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_LEFTEYE]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 4:
                        if(target->special[SPECIALWOUND_TONGUE]&&heavydam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s tongue is blasted off!");
                           else if(damtype & WOUND_BURNED)addstr("'s tongue is burned away!");
                           else if(damtype & WOUND_TORN)addstr("'s tongue is torn out!");
                           else if(damtype & WOUND_CUT)addstr("'s tongue is cut off!");
                           else addstr("'s tongue is removed!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_TONGUE]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 5:
                        if(target->special[SPECIALWOUND_NOSE]&&
                           heavydam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s nose is blasted off!");
                           else if(damtype & WOUND_BURNED)addstr("'s nose is burned away!");
                           else if(damtype & WOUND_TORN)addstr("'s nose is torn off!");
                           else if(damtype & WOUND_CUT)addstr("'s nose is cut off!");
                           else addstr("'s nose is removed!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_NOSE]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 6:
                        if(target->special[SPECIALWOUND_NECK]&&
                           breakdam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s neck bones are shattered!");
                           else addstr("'s neck is broken!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_NECK]=0;
                           if(target->blood>20)target->blood=20;
                        }
                        break;
                  }
               }
               if(w==BODYPART_BODY)
               {
                  clearmessagearea();
                  //set_color(COLOR_MAGENTA,COLOR_BLACK,1);
		          if (goodguyattack) set_color(COLOR_GREEN,COLOR_BLACK,1);
				  else set_color(COLOR_RED,COLOR_BLACK,1);


                  switch(LCSrandom(11))
                  {
                     case 0:
                        if(target->special[SPECIALWOUND_UPPERSPINE]&&
                           breakdam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s upper spine is shattered!");
                           else addstr("'s upper spine is broken!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_UPPERSPINE]=0;
                           if(target->blood>20)target->blood=20;
                        }
                        break;
                     case 1:
                        if(target->special[SPECIALWOUND_LOWERSPINE]&&
                           breakdam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s lower spine is shattered!");
                           else addstr("'s lower spine is broken!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_LOWERSPINE]=0;
                           if(target->blood>20)target->blood=20;
                        }
                        break;
                     case 2:
                        if(target->special[SPECIALWOUND_RIGHTLUNG]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s right lung is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s right lung is torn!");
                           else addstr("'s right lung is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_RIGHTLUNG]=0;
                           if(target->blood>20)target->blood=20;
                        }
                        break;
                     case 3:
                        if(target->special[SPECIALWOUND_LEFTLUNG]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s left lung is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s left lung is torn!");
                           else addstr("'s left lung is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_LEFTLUNG]=0;
                           if(target->blood>20)target->blood=20;
                        }
                        break;
                     case 4:
                        if(target->special[SPECIALWOUND_HEART]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s heart is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s heart is torn!");
                           else addstr("'s heart is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_HEART]=0;
                           if(target->blood>3)target->blood=3;
                        }
                        break;
                     case 5:
                        if(target->special[SPECIALWOUND_LIVER]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s liver is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s liver is torn!");
                           else addstr("'s liver is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_LIVER]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 6:
                        if(target->special[SPECIALWOUND_STOMACH]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s stomach is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s stomach is torn!");
                           else addstr("'s stomach is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_STOMACH]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 7:
                        if(target->special[SPECIALWOUND_RIGHTKIDNEY]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s right kidney is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s right kidney is torn!");
                           else addstr("'s right kidney is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_RIGHTKIDNEY]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 8:
                        if(target->special[SPECIALWOUND_LEFTKIDNEY]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s left kidney is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s left kidney is torn!");
                           else addstr("'s left kidney is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_LEFTKIDNEY]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 9:
                        if(target->special[SPECIALWOUND_SPLEEN]&&
                           pokedam)
                        {
                           move(16,1);
                           addstr(target->name);
                           if(damtype & WOUND_SHOT)addstr("'s spleen is blasted!");
                           else if(damtype & WOUND_TORN)addstr("'s spleen is torn!");
                           else addstr("'s spleen is punctured!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_SPLEEN]=0;
                           if(target->blood>50)target->blood=50;
                        }
                        break;
                     case 10:
                        if(target->special[SPECIALWOUND_RIBS]>0&&
                           breakdam)
                        {
                           int ribminus=LCSrandom(RIBNUM)+1;
                           if(ribminus>target->special[SPECIALWOUND_RIBS])ribminus=target->special[SPECIALWOUND_RIBS];
                           char num[20];
                           itoa(ribminus,num,10);

                           move(16,1);
                           if(ribminus>1)
                           {
                              addstr(num);
                              addstr(" of ");
                              addstr(target->name);
                              addstr("'s ribs are ");
                           }
                           else if(target->special[SPECIALWOUND_RIBS]>1)
                           {
                              addstr("One of ");
                              addstr(target->name);
                              addstr("'s rib is ");
                           }
                           else
                           {
                              addstr(target->name);
                              addstr("'s last unbroken rib is ");
                           }

                           if(damtype & WOUND_SHOT)addstr("shot apart!");
                           else addstr("broken!");
                           refresh();
                           getch();

                           target->special[SPECIALWOUND_RIBS]-=ribminus;
                        }
                        break;
                  }
               }

               severloot(*target,groundloot);
            }

            //set_color(COLOR_WHITE,COLOR_BLACK,1);            
         }
      }
      else
      {
         strcat(str," to no effect.");
         move(17,1);
         addstr(str);

         printparty();
         if(mode==GAMEMODE_CHASECAR||
                  mode==GAMEMODE_CHASEFOOT)printchaseencounter();
         else printencounter();

         refresh();
         getch();
      }
   }
   else
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);

      if(melee && aroll<droll-10 && t.blood>70 && t.animalgloss==ANIMALGLOSS_NONE && weaponskill(t.weapon.type)!=SKILL_THROWING)
      {

         strcpy(str,t.name);
         strcat(str," knocks the blow aside and counters!");
         move(17,1);
         addstr(str);

         refresh();
         getch();

		 goodguyattack = !goodguyattack;
         char actual_dummy;
         attack(t,a,0,actual_dummy,true);
      }
      else
      {
         strcat(str," misses.");
         move(17,1);
         addstr(str);
         printparty();
         if(mode==GAMEMODE_CHASECAR||
                     mode==GAMEMODE_CHASEFOOT)printchaseencounter();
         else printencounter();

         refresh();
         getch();
      }
   }

   if(removeweapon)
      a.weapon.type=WEAPON_NONE;
   actual=1;
   return;
}



/* modifies a combat roll based on the creature's critical injuries */
void healthmodroll(int &aroll,Creature &a)
{
   if(a.special[SPECIALWOUND_RIGHTEYE]!=1)aroll-=LCSrandom(2);
   if(a.special[SPECIALWOUND_LEFTEYE]!=1)aroll-=LCSrandom(2);
   if(a.special[SPECIALWOUND_RIGHTEYE]!=1&&
      a.special[SPECIALWOUND_LEFTEYE]!=1)aroll-=LCSrandom(20);
   if(a.special[SPECIALWOUND_RIGHTLUNG]!=1)aroll-=LCSrandom(8);
   if(a.special[SPECIALWOUND_LEFTLUNG]!=1)aroll-=LCSrandom(8);
   if(a.special[SPECIALWOUND_HEART]!=1)aroll-=LCSrandom(10);
   if(a.special[SPECIALWOUND_LIVER]!=1)aroll-=LCSrandom(5);
   if(a.special[SPECIALWOUND_STOMACH]!=1)aroll-=LCSrandom(5);
   if(a.special[SPECIALWOUND_RIGHTKIDNEY]!=1)aroll-=LCSrandom(5);
   if(a.special[SPECIALWOUND_LEFTKIDNEY]!=1)aroll-=LCSrandom(5);
   if(a.special[SPECIALWOUND_SPLEEN]!=1)aroll-=LCSrandom(4);
   if(a.special[SPECIALWOUND_LOWERSPINE]!=1)aroll-=LCSrandom(100);
   if(a.special[SPECIALWOUND_UPPERSPINE]!=1)aroll-=LCSrandom(200);
   if(a.special[SPECIALWOUND_NECK]!=1)aroll-=LCSrandom(300);
   if(a.special[SPECIALWOUND_RIBS]<RIBNUM)aroll-=LCSrandom(5);
   if(a.special[SPECIALWOUND_RIBS]<RIBNUM/2)aroll-=LCSrandom(5);
   if(a.special[SPECIALWOUND_RIBS]==0)aroll-=LCSrandom(5);
}



/* adjusts attack damage based on armor, other factors */
void damagemod(Creature &t,char &damtype,int &damamount,
               char hitlocation,char armorpenetration,int &mod)
{
   int armor=0;
   int head_armor=0;
   int limb_armor=0;

   switch(t.armor.type)
   {
      default:armor=0;break;
      case ARMOR_BUNKERGEAR:armor=2;head_armor=2;limb_armor=2;break;
      case ARMOR_CIVILLIANARMOR:armor=5;break;
      case ARMOR_POLICEARMOR:armor=7;break;
      case ARMOR_SWATARMOR:armor=7;head_armor=5;limb_armor=2;break;
      case ARMOR_ARMYARMOR:armor=8;head_armor=6;break;
      case ARMOR_HEAVYARMOR:armor=10;head_armor=8;limb_armor=4;break;
   }

   
   if(t.animalgloss==ANIMALGLOSS_TANK)
   {
      if(damtype!=WOUND_BURNED)damamount=0;
      else armor=10;
   }
   else if(hitlocation==BODYPART_HEAD)armor=head_armor;
   else if(hitlocation!=BODYPART_BODY)armor=limb_armor;

   if(t.armor.quality>'1')
      armor-=t.armor.quality-'1';
   if(t.armor.flag & ARMORFLAG_DAMAGED)
      armor-=1;

   if(armor<0)armor=0; // Possible from second-rate clothes

   int mod2=armor+LCSrandom(armor+1)-armorpenetration;
   if(mod2>0)mod-=mod2*2;

   if(mod>10)mod=10; // Cap damage multiplier (every 5 points adds 1x damage)
   
   if(mod<=-8)damamount>>=6;
   else if(mod<=-6)damamount>>=5;
   else if(mod<=-4)damamount>>=4;
   else if(mod<=-3)damamount>>=3;
   else if(mod<=-2)damamount>>=2;
   else if(mod<=-1)damamount>>=1;
   else if(mod>=0)damamount=(int)((float)damamount * (1.0f + 0.2f*mod));

   // Firefighter's bunker gear reduces fire damage by 3/4
   if((damtype & WOUND_BURNED)&&(t.armor.type==ARMOR_BUNKERGEAR))
   {
      // Damaged gear isn't as effective as undamaged gear
      if(t.armor.flag & ARMORFLAG_DAMAGED)
         damamount>>=1; // Only half as much damage reduction
      else
         damamount>>=2; // Full damage reduction
   }

   if(damamount<0)damamount=0;
}


void specialattack(Creature &a, Creature &t, char &actual)
{
   int resist=0;
   char str[200];

   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);

   strcpy(str,a.name);
   strcat(str," ");

   int attack=0;
   if(a.align!=1)
   {
      attack=a.attribute_roll(ATTRIBUTE_WISDOM)+t.get_attribute(ATTRIBUTE_WISDOM,false);
   }
   else if(a.align==1)
   {
      attack=a.attribute_roll(ATTRIBUTE_HEART)+t.get_attribute(ATTRIBUTE_HEART,false);
   }

   switch(a.type)
   {
      case CREATURE_COP:
         switch(LCSrandom(7))
         {
            case 0:strcat(str,"reasons with ");
                   strcat(str,t.name);break;
            case 1:strcat(str,"promises a fair trial to ");
                   strcat(str,t.name);break;
            case 2:strcat(str,"offers a kind ear to ");
                   strcat(str,t.name);break;
            case 3:strcat(str,"urges cooperation from ");
                   strcat(str,t.name);break;
            case 4:strcat(str,"offers a hug to ");
                   strcat(str,t.name);break;
            case 5:strcat(str,"suggests counseling to ");
                   strcat(str,t.name);break;
            case 6:strcat(str,"gives a teddy bear to ");
                   strcat(str,t.name);break;
         }
         strcat(str,"!");

         resist=t.attribute_roll(ATTRIBUTE_HEART);

         attack+=a.skill_roll(SKILL_PERSUASION);
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
      case CREATURE_JUDGE_LIBERAL:
         switch(LCSrandom(4))
         {
            case 0:strcat(str,"debates the death penalty with");break;
            case 1:strcat(str,"debates gay rights with");break;
            case 2:strcat(str,"debates free speech with");break;
            case 3:strcat(str,"debates the Second Amendment with");break;
         }
         strcat(str," ");
         strcat(str,t.name);
         strcat(str,"!");
         if(t.align==1)
         {
            resist=t.skill_roll(SKILL_LAW)+
                   t.attribute_roll(ATTRIBUTE_HEART);
         }
         else
         {
            resist=t.skill_roll(SKILL_LAW)+
                   t.attribute_roll(ATTRIBUTE_WISDOM);
         }
         attack+=a.skill_roll(SKILL_LAW);
         break;
      case CREATURE_SCIENTIST_EMINENT:
         switch(LCSrandom(3))
         {
            case 0:strcat(str,"debates scientific ethics with");break;
            case 1:if(a.align==-1)strcat(str,"explains the benefits of research to");
                   else strcat(str,"explains ethical research to");
               break;
            case 2:strcat(str,"discusses the scientific method with");break;
         }
         strcat(str," ");
         strcat(str,t.name);
         strcat(str,"!");
         if(t.align==1)
         {
            resist=t.skill_roll(SKILL_SCIENCE)+
                   t.attribute_roll(ATTRIBUTE_HEART);
         }
         else
         {
            resist=t.skill_roll(SKILL_SCIENCE)+
                   t.attribute_roll(ATTRIBUTE_WISDOM);
         }
         attack+=a.skill_roll(SKILL_SCIENCE);
         break;
      case CREATURE_CORPORATE_CEO:
         if(a.align==-1)
         {
            switch(LCSrandom(10))
            {
               case 0:strcat(str,"explains the derivatives market to");break;
               case 1:strcat(str,"justifies voodoo economics to");break;
               case 2:strcat(str,"extols the Reagan presidency to");break;
               case 3:strcat(str,"argues about tax cuts with");break;
               case 4:strcat(str,"explains Conservative philosophy to");break;
               case 5:strcat(str,"extends a dinner invitation to");break;
               case 6:strcat(str,"offers a VP position to");break;
               case 7:strcat(str,"shows a $1000 bill to");break;
               case 8:strcat(str,"debates fiscal policy with");break;
               case 9:strcat(str,"offers stock options to");break;
            }
         }
         else
         {
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"debates fiscal policy with");break;
               case 1:strcat(str,"derides voodoo economics to");break;
               case 2:strcat(str,"dismisses the Reagan presidency to");break;
               case 3:strcat(str,"argues about tax cuts with");break;
               case 4:strcat(str,"explains Liberal philosophy to");break;
            }
         }
         strcat(str," ");
         strcat(str,t.name);
         strcat(str,"!");
         if(t.align==1)
         {
            resist=t.skill_roll(SKILL_BUSINESS)+
                   t.attribute_roll(ATTRIBUTE_HEART);
         }
         else
         {
            resist=t.skill_roll(SKILL_BUSINESS)+
                   t.attribute_roll(ATTRIBUTE_WISDOM);
         }
         attack+=a.skill_roll(SKILL_BUSINESS);
         break;
      case CREATURE_RADIOPERSONALITY:
      case CREATURE_NEWSANCHOR:
         switch(LCSrandom(5))
         {
            case 0:strcat(str,"winks at");break;
            case 1:strcat(str,"smiles at");break;
            case 2:strcat(str,"smirks at");break;
            case 3:strcat(str,"chats warmly with");break;
            case 4:strcat(str,"yells slogans at");break;
         }
         strcat(str," ");
         strcat(str,t.name);
         strcat(str,"!");
         if(t.align==1)
         {
            resist=t.attribute_roll(ATTRIBUTE_HEART);
         }
         else
         {
            resist=t.attribute_roll(ATTRIBUTE_WISDOM);
         }
         attack+=a.attribute_roll(ATTRIBUTE_CHARISMA);
         break;
      default:
         if(a.weapon.type==WEAPON_GUITAR)
         {
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"plays a song for");break;
               case 1:strcat(str,"sings to");break;
               case 2:strcat(str,"strums the guitar at");break;
               case 3:if(a.align==1)strcat(str,"plays protest songs at");
                      else strcat(str,"plays country songs at");
                      break;
               case 4:strcat(str,"rocks out at");break;
            }
            strcat(str," ");
            strcat(str,t.name);
            strcat(str,"!");

            attack=a.skill_roll(SKILL_MUSIC);

            if(t.align==1)
            {
               resist=t.attribute_roll(ATTRIBUTE_HEART);
            }
            else
            {
               resist=t.attribute_roll(ATTRIBUTE_WISDOM);
            }
            if(resist>0)
               a.train(SKILL_MUSIC,LCSrandom(resist)+1);
            else
               a.train(SKILL_MUSIC,1);
         }
         break;
   }

   move(16,1);
   addstr(str);

   if(t.animalgloss==ANIMALGLOSS_TANK||(t.animalgloss==ANIMALGLOSS_ANIMAL&&law[LAW_ANIMALRESEARCH]!=2))
   {
      switch(t.animalgloss)
      {
      case ANIMALGLOSS_TANK:
      case ANIMALGLOSS_ANIMAL:
      default:
         move(17,1);
         addstr(t.name);
         addstr(" is immune to the attack!");
      }
   }
   else if(a.enemy() && t.flag & CREATUREFLAG_BRAINWASHED)
   {
      move(17,1);
      addstr(t.name);
      addstr(" is not subject to Conservative thinking!");
   }
   else if(attack>resist)
   {
      t.stunned+=(attack-resist)/4;
      if(a.enemy())
      {
         if(t.juice>=100)
         {
            move(17,1);
            addstr(t.name);
            addstr(" loses juice!");
            addjuice(t,-50);
         }
         else if(LCSrandom(15)>t.get_attribute(ATTRIBUTE_WISDOM,true) || t.get_attribute(ATTRIBUTE_WISDOM,true) < t.get_attribute(ATTRIBUTE_HEART,true))
         {
            move(17,1);
            addstr(t.name);
            addstr(" becomes Wiser!");
            t.adjust_attribute(ATTRIBUTE_WISDOM,+1);
         }
         else if(t.align==ALIGN_LIBERAL && t.flag & CREATUREFLAG_LOVESLAVE)
         {
            move(17,1);
            addstr(t.name);
            addstr(" can't bear to leave!");
         }
         else
         {
            if(a.align==-1)
            {
               move(17,1);
               addstr(t.name);
               addstr(" is turned Conservative");
               t.stunned=0;
               if(t.prisoner!=NULL)
               {
                  freehostage(t,0);
               }
               addstr("!");
            }
            else
            {
               move(17,1);
               addstr(t.name);
               addstr(" doesn't want to fight anymore");
               t.stunned=0;
               if(t.prisoner!=NULL)
               {
                  freehostage(t,0);
               }
               addstr("!");
            }

            for(int e=0;e<ENCMAX;e++)
            {
               if(encounter[e].exists==0)
               {
                  encounter[e]=t;
                  encounter[e].exists=1;
                  if(a.align==-1)conservatise(encounter[e]);
                  encounter[e].cantbluff=2;
                  encounter[e].squadid=-1;
                  break;
               }
            }

            char flipstart=0;
            for(int p=0;p<6;p++)
            {
               if(activesquad->squad[p]==&t)
               {
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
                  flipstart=1;
               }
               if(flipstart&&p<5)activesquad->squad[p]=activesquad->squad[p+1];
            }
            if(flipstart)activesquad->squad[5]=NULL;
         }
      }
      else
      {
         if(t.juice>=100)
         {
            move(17,1);
            addstr(t.name);
            addstr(" seems less badass!");
            addjuice(t,-50);
         }
         else if(!t.attribute_check(ATTRIBUTE_HEART,DIFFICULTY_AVERAGE) ||
            t.get_attribute(ATTRIBUTE_HEART,true) < t.get_attribute(ATTRIBUTE_WISDOM,true))
         {
            move(17,1);
            addstr(t.name);
            addstr("'s Heart swells!");
            t.adjust_attribute(ATTRIBUTE_HEART,+1);
         }
         else
         {
            move(17,1);
            addstr(t.name);
            addstr(" has turned Liberal!");
            t.stunned=0;

            liberalize(t);
            t.flag|=CREATUREFLAG_CONVERTED;
            t.cantbluff=0;
         }
      }
   }
   else
   {
      move(17,1);
      addstr(t.name);
      addstr(" remains strong.");
   }

   printparty();
   if(mode==GAMEMODE_CHASECAR||
            mode==GAMEMODE_CHASEFOOT)printchaseencounter();
   else printencounter();

   refresh();
   getch();

   return;
}


/* destroys armor, masks, drops weapons based on severe damage */
void severloot(Creature &cr,vector<itemst *> &loot)
{
   int armok=2;
   if((cr.wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
      (cr.wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
   if((cr.wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
      (cr.wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
   if(cr.special[SPECIALWOUND_NECK]!=1)armok=0;
   if(cr.special[SPECIALWOUND_UPPERSPINE]!=1)armok=0;

   if(cr.weapon.type!=WEAPON_NONE && armok==0)
   {
      clearmessagearea();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(16,1);
      addstr("The ");
      char str[200];
      getweaponfull(str,cr.weapon.type,1);
      addstr(str);
      addstr(" slips from");
      move(17,1);
      addstr(cr.name);
      addstr("'s grasp.");
      refresh();
      getch();

      if(mode==GAMEMODE_SITE)
      {
         itemst *newi=new itemst;
            newi->type=ITEM_WEAPON;
            newi->weapon=cr.weapon;
         loot.push_back(newi);
      }

      cr.weapon.type=WEAPON_NONE;
      cr.weapon.ammo=0;

      for(int c=0;c<CLIPNUM;c++)
      {
         if(mode==GAMEMODE_SITE)
         {
            for(int p=0;p<cr.clip[c];p++)
            {
               if(c==CLIP_MOLOTOV)
               {
                  itemst *newi=new itemst;
                     newi->type=ITEM_WEAPON;
                     newi->weapon.type=WEAPON_MOLOTOV;
                     newi->weapon.ammo=1;
                  loot.push_back(newi);
               }
               else
               {
                  itemst *newi=new itemst;
                     newi->type=ITEM_CLIP;
                     newi->cliptype=c;
                  loot.push_back(newi);
               }
            }
         }

         cr.clip[c]=0;
      }
   }

   if(((cr.wound[BODYPART_BODY] & WOUND_CLEANOFF)||
      (cr.wound[BODYPART_BODY] & WOUND_NASTYOFF))&&
      cr.armor.type!=ARMOR_NONE&&
      cr.armor.type!=ARMOR_MASK)
   {
      clearmessagearea();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(16,1);
      addstr(cr.name);
      addstr("'s ");
      char str[80];
      getarmorfull(str,cr.armor.type,cr.armor.subtype);
      addstr(str);
      addstr(" has been destroyed.");
      refresh();
      getch();

      cr.armor.type=ARMOR_NONE;
      cr.armor.quality='1';
      cr.armor.flag=0;
   }

   if((cr.wound[BODYPART_HEAD] & WOUND_NASTYOFF)&&
      cr.armor.type==ARMOR_MASK)
   {
      clearmessagearea();
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      move(16,1);
      addstr(cr.name);
      addstr("'s ");
      char str[80];
      getarmorfull(str,cr.armor.type,cr.armor.subtype);
      addstr(str);
      addstr(" has been destroyed.");
      refresh();
      getch();

      cr.armor.type=ARMOR_NONE;
      cr.armor.quality='1';
      cr.armor.flag=0;
   }
}



/* damages the selected armor if it covers the body part specified */
void armordamage(armorst &armor,int bp)
{
   switch(armor.type)
   {
      case ARMOR_NONE:
         return;
      case ARMOR_HEAVYARMOR:
      case ARMOR_SWATARMOR:
      case ARMOR_SECURITYUNIFORM:
      case ARMOR_POLICEUNIFORM:
      case ARMOR_DEATHSQUADUNIFORM:
      case ARMOR_BONDAGEGEAR:
      case ARMOR_MILITARY:
      case ARMOR_BUNKERGEAR:
         armor.flag|=ARMORFLAG_DAMAGED;
         break;
      case ARMOR_ARMYARMOR:
         if(bp==BODYPART_BODY||bp==BODYPART_HEAD)armor.flag|=ARMORFLAG_DAMAGED;
      case ARMOR_MASK:
         if(bp==BODYPART_HEAD)armor.flag|=ARMORFLAG_DAMAGED;
         break;
      case ARMOR_CIVILLIANARMOR:
      case ARMOR_POLICEARMOR:
      case ARMOR_TOGA:
      case ARMOR_MITHRIL:
      case ARMOR_WIFEBEATER:
         if(bp==BODYPART_BODY)armor.flag|=ARMORFLAG_DAMAGED;
         break;
      case ARMOR_OVERALLS:
         if(bp==BODYPART_BODY)armor.flag|=ARMORFLAG_DAMAGED;
         if(bp==BODYPART_LEG_RIGHT)armor.flag|=ARMORFLAG_DAMAGED;
         if(bp==BODYPART_LEG_LEFT)armor.flag|=ARMORFLAG_DAMAGED;
         break;
      default:
         if(bp!=BODYPART_HEAD)armor.flag|=ARMORFLAG_DAMAGED;
         break;
   }
}



/* blood explosions */
void bloodblast(armorst &armor)
{
   //GENERAL
   if(armor.type!=ARMOR_NONE)armor.flag|=ARMORFLAG_BLOODY;

   if(mode!=GAMEMODE_SITE)return;

   levelmap[locx][locy][locz].flag|=SITEBLOCK_BLOODY2;

   //HIT EVERYTHING
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(activesquad->squad[p]->armor.type!=ARMOR_NONE)
      {
         if(!LCSrandom(2))activesquad->squad[p]->armor.flag|=ARMORFLAG_BLOODY;
      }
   }

   for(int e=0;e<ENCMAX;e++)
   {
      if(!encounter[e].exists)continue;
      if(encounter[e].armor.type!=ARMOR_NONE)
      {
         if(!LCSrandom(2))encounter[e].armor.flag|=ARMORFLAG_BLOODY;
      }
   }

   //REFRESH THE SCREEN
   printsitemap(locx,locy,locz);
   refresh();
}



/* kills the specified creature from the encounter, dropping loot */
void delenc(short e,char loot)
{
   //MAKE GROUND LOOT
   if(mode!=GAMEMODE_SITE)loot=0;
   if(loot)makeloot(encounter[e],groundloot);

   //BURY IT
   for(int en=e;en<ENCMAX;en++)
   {
      if(!encounter[en].exists)break;
      if(en<ENCMAX-1)encounter[en]=encounter[en+1];
   }
   encounter[ENCMAX-1].exists=0;
}



/* generates the loot dropped by a creature when it dies */
void makeloot(Creature &cr,vector<itemst *> &loot)
{
   if(cr.weapon.type!=WEAPON_NONE)
   {
      itemst *newi=new itemst;
         newi->type=ITEM_WEAPON;
         newi->weapon=cr.weapon;
      loot.push_back(newi);

      cr.weapon.type=WEAPON_NONE;
      cr.weapon.ammo=0;
   }

   if(cr.armor.type!=ARMOR_NONE)
   {
      itemst *newi=new itemst;
         newi->type=ITEM_ARMOR;
         newi->armor=cr.armor;
      loot.push_back(newi);

      cr.armor.type=ARMOR_NONE;
      cr.armor.quality='1';
      cr.armor.flag=0;
   }

   if(cr.money>0 && mode == GAMEMODE_SITE)
   {
      itemst *newi=new itemst;
         newi->type=ITEM_MONEY;
         newi->money=cr.money;
      loot.push_back(newi);

      cr.money=0;
   }

   for(int c=0;c<CLIPNUM;c++)
   {
      for(int p=0;p<cr.clip[c];p++)
      {
         if(c==CLIP_MOLOTOV)
         {
            itemst *newi=new itemst;
               newi->type=ITEM_WEAPON;
               newi->weapon.type=WEAPON_MOLOTOV;
               newi->weapon.ammo=1;
            loot.push_back(newi);
         }
         else
         {
            itemst *newi=new itemst;
               newi->type=ITEM_CLIP;
               newi->cliptype=c;
            loot.push_back(newi);
         }
      }

      cr.clip[c]=0;
   }
}



/* abandoned liberal is captured by conservatives */
void capturecreature(Creature &t)
{
   t.activity.type=ACTIVITY_NONE;

   t.weapon.ammo=0;
   t.weapon.type=0;
   t.armor.type=ARMOR_CLOTHES;
   t.armor.subtype=0;
   t.armor.quality='1';
   t.armor.flag=0;

   freehostage(t,2); // situation 2 = no message; this may want to be changed to 0 or 1
   if(t.prisoner)
   {
      if(t.prisoner->squadid==-1)
         delete t.prisoner;
      t.prisoner=NULL; // Stop hauling people
   }
   if(t.flag & CREATUREFLAG_JUSTESCAPED)
   {
      t.location=cursite;
      if(sitetype==SITE_GOVERNMENT_PRISON||
         sitetype==SITE_GOVERNMENT_COURTHOUSE)
      {
         t.armor.type=ARMOR_PRISONER;
      }
      if(sitetype==SITE_GOVERNMENT_PRISON)
      {
         // Clear criminal record?
         t.heat=0;
         for(int i=0;i<LAWFLAGNUM;i++)
         {
            t.crimes_suspected[i]=0;
         }
      }
   }
   else
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
         {
            t.location=l;
         }
      }
   }

   t.squadid=-1;
}



/* checks if the creature can fight and prints flavor text if they can't */
char incapacitated(Creature &a,char noncombat,char &printed)
{
   printed=0;

   if(a.animalgloss==ANIMALGLOSS_TANK)
   {
      if(a.blood<=20||(a.blood<=50&&(LCSrandom(2)||a.forceinc)))
      {
         a.forceinc=0;
         if(noncombat)
         {
            clearmessagearea();
            
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(16,1);
            addstr("The ");
            addstr(a.name);
            switch(LCSrandom(3))
            {
               case 0:addstr(" smokes...");
                  break;
               case 1:addstr(" smolders.");
                  break;
               case 2:addstr(" burns...");
                  break;
            }

            printed=1;
         }
         return 1;
      }

      return 0;
   }

   if(a.animalgloss==ANIMALGLOSS_ANIMAL)
   {
      if(a.blood<=20||(a.blood<=50&&(LCSrandom(2)||a.forceinc)))
      {
         a.forceinc=0;
         if(noncombat)
         {
            clearmessagearea();
            set_color(COLOR_WHITE,COLOR_BLACK,1);

            move(16,1);
            addstr("The ");
            addstr(a.name);
            switch(LCSrandom(3))
            {
               case 0:addstr(" yelps in pain...");
                  break;
               case 1:
                  if(law[LAW_FREESPEECH]==-2)addstr(" [makes a stinky].");
                  else addstr(" soils the floor.");
                  break;
               case 2:addstr(" yowls pitifully...");
                  break;
            }

            printed=1;
         }
         return 1;
      }

      return 0;
   }

   if(a.blood<=20||(a.blood<=50&&(LCSrandom(2)||a.forceinc)))
   {
      a.forceinc=0;
      if(noncombat)
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(16,1);
         addstr(a.name);
         switch(LCSrandom(54))
         {
            case 0:
               addstr(" desperately cries out to Jesus.");
               break;
            case 1:
               if(law[LAW_FREESPEECH]==-2)addstr(" [makes a stinky].");
               else addstr(" soils the floor.");
               break;
            case 2:
               addstr(" whimpers in a corner.");
               break;
            case 3:
               addstr(" begins to weep.");
               break;
            case 4:
               addstr(" vomits.");
               break;
            case 5:
               addstr(" chortles...");
               break;
            case 6:
               addstr(" screams in pain.");
               break;
            case 7:
               addstr(" asks for mother.");
               break;
            case 8:
               addstr(" prays softly...");
               break;
            case 9:
               addstr(" clutches at the wounds.");
               break;
            case 10:
               addstr(" reaches out and moans.");
               break;
            case 11:
               addstr(" hollers in pain.");
               break;
            case 12:
               addstr(" groans in agony.");
               break;
            case 13:
               addstr(" begins hyperventilating.");
               break;
            case 14:
               addstr(" shouts a prayer.");
               break;
            case 15:
               addstr(" coughs up blood.");
               break;
            case 16:
               if(mode!=GAMEMODE_CHASECAR)addstr(" stumbles against a wall.");
               else addstr(" leans against the door.");
               break;
            case 17:
               addstr(" begs for forgiveness.");
               break;
            case 18:
               addstr(" shouts \"Why have you forsaken me?\"");
               break;
            case 19:
               addstr(" murmurs \"Why Lord?   Why?\"");
               break;
            case 20:
               addstr(" whispers \"Am I dead?\"");
               break;
            case 21:
               if(law[LAW_FREESPEECH]==-2)addstr(" [makes a mess], moaning.");
               else addstr(" pisses on the floor, moaning.");
               break;
            case 22:
               addstr(" whispers incoherently.");
               break;
            case 23:
               if(a.special[SPECIALWOUND_RIGHTEYE]&&
                  a.special[SPECIALWOUND_LEFTEYE])
               {
                  addstr(" stares off into space.");
               }
               else if(a.special[SPECIALWOUND_RIGHTEYE]||
                  a.special[SPECIALWOUND_LEFTEYE])
               {
                  addstr(" stares into space with one empty eye.");
               }
               else
               {
                  addstr(" stares out with hollow sockets.");
               }
               break;
            case 24:
               addstr(" cries softly.");
               break;
            case 25:
               addstr(" yells until the scream cracks dry.");
               break;
            case 26:
               if(a.special[SPECIALWOUND_TEETH]>1)addstr("'s teeth start chattering.");
               else if(a.special[SPECIALWOUND_TEETH]==1)addstr("'s tooth starts chattering.");
               else addstr("'s gums start chattering.");
               break;
            case 27:
               addstr(" starts shaking uncontrollably.");
               break;
            case 28:
               addstr(" looks strangely calm.");
               break;
            case 29:
               addstr(" nods off for a moment.");
               break;
            case 30:
               addstr(" starts drooling.");
               break;
            case 31:
               addstr(" seems lost in memories.");
               break;
            case 32:
               addstr(" shakes with fear.");
               break;
            case 33:
               addstr(" murmurs \"I'm so afraid...\"");
               break;
            case 34:
               addstr(" cries \"It can't be like this...\"");
               break;
            case 35:
               if(a.type==CREATURE_TEENAGER||
                  a.type==CREATURE_WORKER_FACTORY_CHILD)
               {
                  addstr(" cries \"Mommy!\"");
               }
               else addstr(" murmurs \"What about my children?\"");
               break;
            case 36:
               addstr(" shudders quietly.");
               break;
            case 37:
               addstr(" yowls pitifully.");
               break;
            case 38:
               addstr(" begins losing faith in God.");
               break;
            case 39:
               addstr(" muses quietly about death.");
               break;
            case 40:
               addstr(" asks for a blanket.");
               break;
            case 41:
               addstr(" shivers softly.");
               break;
            case 42:
               if(law[LAW_FREESPEECH]==-2)addstr(" [makes a mess].");
               else addstr(" vomits up a clot of blood.");
               break;
            case 43:
               if(law[LAW_FREESPEECH]==-2)addstr(" [makes a mess].");
               else addstr(" spits up a cluster of bloody bubbles.");
               break;
            case 44:
               addstr(" pleads for mercy.");
               break;
            case 45:
               addstr(" quietly asks for coffee.");
               break;
            case 46:
               addstr(" looks resigned.");
               break;
            case 47:
               addstr(" scratches at the air.");
               break;
            case 48:
               addstr(" starts to giggle uncontrollably.");
               break;
            case 49:
               addstr(" wears a look of pain.");
               break;
            case 50:
               addstr(" questions God.");
               break;
            case 51:
               addstr(" whispers \"Mama baby.  Baby loves mama.\"");
               break;
            case 52:
               addstr(" asks for childhood toys frantically.");
               break;
            case 53:
               addstr(" murmurs \"But I go to church...\"");
               break;
         }

         printed=1;
      }

      return 1;
   }
   else if(a.stunned)
   {
      if(noncombat)
      {
         a.stunned--;
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(16,1);
         addstr(a.name);
         switch(LCSrandom(11))
         {
            case 0:
               addstr(" seems hesitant.");
               break;
            case 1:
               addstr(" is caught in self-doubt.");
               break;
            case 2:
               addstr(" looks around uneasily.");
               break;
            case 3:
               addstr(" begins to weep.");
               break;
            case 4:
               addstr(" asks \"Is this right?\"");
               break;
            case 5:
               addstr(" asks for guidance.");
               break;
            case 6:
               addstr(" is caught in indecision.");
               break;
            case 7:
               addstr(" feels numb.");
               break;
            case 8:
               addstr(" prays softly.");
               break;
            case 9:
               addstr(" searches for the truth.");
               break;
            case 10:
               addstr(" tears up.");
               break;
         }

         printed=1;
      }
      return 1;
   }

   if(a.special[SPECIALWOUND_NECK]==2||
      a.special[SPECIALWOUND_UPPERSPINE]==2)
   {
      if(!noncombat)
      {
         clearmessagearea();
         set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(16,1);
         addstr(a.name);
         switch(LCSrandom(5))
         {
            case 0:
               addstr(" looks on with authority.");
               break;
            case 1:
               addstr(" waits patiently.");
               break;
            case 2:
               addstr(" sits in thought.");
               break;
            case 3:
               addstr(" breathes slowly.");
               break;
            case 4:
               addstr(" considers the situation.");
               break;
         }

         printed=1;
      }

      return 1;
   }

   return 0;
}



/* describes a character's death */
void adddeathmessage(Creature &cr)
{
   set_color(COLOR_YELLOW,COLOR_BLACK,1);

   move(16,1);
   char str[200];

   if((cr.wound[BODYPART_HEAD] & WOUND_CLEANOFF)||
      (cr.wound[BODYPART_HEAD] & WOUND_NASTYOFF))
   {
      strcpy(str,cr.name);
      switch(LCSrandom(4))
      {
         case 0:
            strcat(str," reaches once where there");
            addstr(str);
            move(17,1);
            if(mode!=GAMEMODE_CHASECAR)
            {
               addstr("is no head, and falls.");
            }
            else addstr("is no head, and slumps over.");
            break;
         case 1:
            if(mode!=GAMEMODE_CHASECAR)
            {
               strcat(str," stands headless for a");
            }
            else strcat(str," sits headless for a");
            addstr(str);
            move(17,1);
            addstr("moment then crumples over.");
            break;
         case 2:
            strcat(str," squirts ");
            if(law[LAW_FREESPEECH]==-2)strcat(str,"[red water]");
            else strcat(str,"blood");
            strcat(str," out of the ");
            addstr(str);
            move(17,1);
            if(mode!=GAMEMODE_CHASECAR)
            {
               addstr("neck and runs down the hall.");
            }
            else addstr("neck and falls to the side.");
            break;
         case 3:
            strcat(str," sucks a last breath through");
            addstr(str);
            move(17,1);
            addstr("the neck hole, then is quiet.");
            break;
      }
   }
   else if((cr.wound[BODYPART_BODY] & WOUND_CLEANOFF)||
      (cr.wound[BODYPART_BODY] & WOUND_NASTYOFF))
   {
      strcpy(str,cr.name);
      switch(LCSrandom(2))
      {
         case 0:strcat(str," breaks into pieces.");break;
         case 1:strcat(str," falls apart and is dead.");break;
      }
      addstr(str);
   }
   else
   {
      strcpy(str,cr.name);
      switch(LCSrandom(11))
      {
         case 0:
            strcat(str," cries out one last time");
            addstr(str);
            move(17,1);
            addstr("then is quiet.");
            break;
         case 1:
            strcat(str," gasps a last breath and");
            addstr(str);
            move(17,1);
            if(law[LAW_FREESPEECH]==-2)addstr("[makes a mess].");
            else addstr("soils the floor.");
            break;
         case 2:
            strcat(str," murmur quietly, breathing softly.");
            addstr(str);
            move(17,1);
            addstr("Then all is silent.");
            break;
         case 3:
            strcat(str," shouts \"FATHER!  Why have you");
            addstr(str);
            move(17,1);
            addstr("forsaken me?\" and dies in a heap.");
            break;
         case 4:
            strcat(str," cries silently for mother,");
            addstr(str);
            move(17,1);
            addstr("breathing slowly, then not at all.");
            break;
         case 5:
            strcat(str," breathes heavily, coughing up");
            addstr(str);
            move(17,1);
            addstr("blood...  then is quiet.");
            break;
         case 6:
            strcat(str," silently drifts away, and");
            addstr(str);
            move(17,1);
            addstr("is gone.");
            break;
         case 7:
            strcat(str," sweats profusely, murmurs");
            addstr(str);
            move(17,1);
            if(law[LAW_FREESPEECH]==-2)addstr("something [good] about Jesus, and dies.");
            else addstr("something about Jesus, and dies.");
            break;
         case 8:
            strcat(str," whines loudly, voice crackling,");
            addstr(str);
            move(17,1);
            addstr("then curls into a ball, unmoving.");
            break;
         case 9:
            strcat(str," shivers silently, whispering");
            addstr(str);
            move(17,1);
            addstr("a prayer, then all is still.");
            break;
		 case 10:
			strcat(str," speaks these final words:");
			addstr(str);
            move(17,1);
			switch (cr.align)
			{
			case ALIGN_LIBERAL:
			case ALIGN_ELITELIBERAL:
				addstr(slogan);
				break;
			case ALIGN_MODERATE:
				addstr("\"A plague on both your houses...\"");
				break;
			default:
				addstr("\"Better dead than liberal...\"");
				break;
			}
      }
   }
}



/* pushes people into the current squad (used in a siege) */
void autopromote(int loc)
{
   if(activesquad==NULL)return;

   int partysize=0;
   int partydead=0;
   int p;
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)partysize++;
      else continue;

      if(!activesquad->squad[p]->alive)partydead++;
   }

   int libnum=0;
   for(int pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->location!=loc)continue;
      if(pool[pl]->alive&&pool[pl]->align==1)libnum++;
   }

   if(partysize==6&&partydead==0)return;
   if(partysize==libnum)return;

   char conf;
   for(p=0;p<6;p++)
   {
      conf=0;
      if(activesquad->squad[p]==NULL)conf=1;
      else if(!activesquad->squad[p]->alive)conf=1;

      if(conf)
      {
         for(int pl=0;pl<pool.size();pl++)
         {
            if(pool[pl]->location!=loc)continue;
            if(pool[pl]->alive&&pool[pl]->squadid==-1&&
               pool[pl]->align==1)
            {
               if(activesquad->squad[p]!=NULL)activesquad->squad[p]->squadid=-1;
               activesquad->squad[p]=pool[pl];
               activesquad->squad[p]->squadid=activesquad->id;
               break;
            }
         }
      }
   }
}
