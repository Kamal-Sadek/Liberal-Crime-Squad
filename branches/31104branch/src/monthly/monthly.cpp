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



/* does end of month actions */
void passmonth(char &clearformess,char canseethings)
{
   short oldlaw[LAWNUM];
   memmove(oldlaw,law,sizeof(short)*LAWNUM);
   int l, v, p;

   //TIME ADVANCE
   day=1;
   month++;
   if(month==13)
   {
      month=1;
      year++;
   }

   switch(endgamestate)
   {
   case ENDGAME_NONE:
      if(newscherrybusted && publicmood(-1)>65 && !LCSrandom(5))
         endgamestate=ENDGAME_CCS_APPEARANCE;
      break;
   case ENDGAME_CCS_APPEARANCE:
      if(!LCSrandom(12))
         endgamestate=ENDGAME_CCS_ATTACKS;
      break;
   case ENDGAME_CCS_ATTACKS:
      if(!LCSrandom(12))
         endgamestate=ENDGAME_CCS_SIEGES;
      break;
   case ENDGAME_CCS_SIEGES:
   case ENDGAME_CCS_DEFEATED:
      //if(publicmood(-1)>85&&presparty==1)
      //   endgamestate=ENDGAME_MARTIALLAW;
      break;
   }

   //CLEAR RENT EXEMPTIONS
   for(l=0;l<location.size();l++)location[l]->newrental=0;

   //YOUR PAPER AND PUBLIC OPINION AND STUFF
   vector<int> nploc;
   for(l=0;l<location.size();l++)
   {
      if((location[l]->compound_walls & COMPOUND_PRINTINGPRESS)&&
         !location[l]->siege.siege)nploc.push_back(l);
   }

   int guardianpower=0;
   if(nploc.size()>0&&!disbanding)
   {
      //DO SPECIAL EDITIONS
      int loottype=choosespecialedition(clearformess);

      if(loottype!=-1)
      {
         guardianpower+=10*nploc.size();
         printnews(loottype,nploc.size());

         if(loottype==LOOT_INTHQDISK||
            loottype==LOOT_SECRETDOCUMENTS)
         {
            for(int l=0;l<nploc.size();l++)
            {
               criminalizepool(LAWFLAG_TREASON,-1,nploc[l]);
            }
         }
      }
   }

   int libpower[VIEWNUM]={0};

   //STORIES STALE EVEN IF NOT PRINTED
   for(v=0;v<VIEWNUM;v++)public_interest[v]/=2;

   int conspower=200-attitude[VIEW_AMRADIO]-attitude[VIEW_CABLENEWS];
   
   //HAVING SLEEPERS
   for(int pl=pool.size()-1;pl>0;pl--)
   {
      if(pool[pl]->alive&&(pool[pl]->flag & CREATUREFLAG_SLEEPER))
      {
         sleepereffect(*pool[pl],clearformess,canseethings,libpower);
      }
   }

   //Manage graffiti
   for(int l=0;l<location.size();l++) // Check each location
   {
      for(int c=location[l]->changes.size()-1;c>=0;c--) // Each change to the map
      {
         if(location[l]->changes[c].flag==SITEBLOCK_GRAFFITI||
            location[l]->changes[c].flag==SITEBLOCK_GRAFFITI_CCS||
            location[l]->changes[c].flag==SITEBLOCK_GRAFFITI_OTHER) // Find changes that refer specifically to graffiti
         {
            int power;
            int align;

            if(location[l]->changes[c].flag==SITEBLOCK_GRAFFITI)
               align=1;
            if(location[l]->changes[c].flag==SITEBLOCK_GRAFFITI_CCS)
               align=-1;

            //Purge graffiti from more secure sites (or from non-secure
            //sites about once every five years), but these will
            //influence people more for the current month
            if(securityable(location[l]->type))
            {
               location[l]->changes.erase(location[l]->changes.begin()+c);
               power=5;
            }
            else
            {
               if(location[l]->renting==-2) // CCS Safehouse
               {
                  location[l]->changes[c].flag=SITEBLOCK_GRAFFITI_CCS; // Convert to CCS tags
               }
               else if(location[l]->renting==0) // LCS Permanent Safehouse
               {
                  location[l]->changes[c].flag=SITEBLOCK_GRAFFITI; // Convert to LCS tags
               }
               else
               {
                  power=1;
                  if(!LCSrandom(10))
                     location[l]->changes[c].flag=SITEBLOCK_GRAFFITI_OTHER; // Convert to other tags
                  if(!LCSrandom(10)&&endgamestate<ENDGAME_CCS_DEFEATED&&endgamestate>0)
                     location[l]->changes[c].flag=SITEBLOCK_GRAFFITI_CCS; // Convert to CCS tags
                  if(!LCSrandom(30))
                     location[l]->changes.erase(location[l]->changes.begin()+c); // Clean up
               }
            }
            if(align==1)
            {
               background_liberal_influence[VIEW_LIBERALCRIMESQUAD]+=power;
               background_liberal_influence[VIEW_CONSERVATIVECRIMESQUAD]+=power;
            }
            else if(align==-1)
            {
               background_liberal_influence[VIEW_LIBERALCRIMESQUAD]-=power;
               background_liberal_influence[VIEW_CONSERVATIVECRIMESQUAD]-=power;
            }
         }
      }
   }

   int mediabalance=0;
   int issuebalance[VIEWNUM-5];
   
   //PUBLIC OPINION NATURAL MOVES
   for(v=0;v<VIEWNUM;v++)
   {
      // Liberal essays add their power to the effect of sleepers
      libpower[v]+=background_liberal_influence[v];
      background_liberal_influence[v]=0;

      if(v==VIEW_LIBERALCRIMESQUADPOS)continue;
      if(v==VIEW_LIBERALCRIMESQUAD)continue;
      if(v==VIEW_CONSERVATIVECRIMESQUAD)continue;
      if(v!=VIEW_AMRADIO&&v!=VIEW_CABLENEWS)
      {
         issuebalance[v] = libpower[v] - conspower;
         mediabalance += issuebalance[v];

         // Heavy randomization -- balance of power just biases the roll
         int roll = issuebalance[v] + LCSrandom(400)-200;

         // If +/-50 to either side, that side wins the tug-of-war
         if(roll < -50)
         {
            change_public_opinion(v,-1,0);
         }
         else if(roll > 50)
         {
            change_public_opinion(v,1,0);
         }
         else // Else random movement
         {
            change_public_opinion(v,LCSrandom(2)*2-1,0);
         }
      }

      // AM Radio and Cable News popularity slowly shift to reflect public
      // opinion over time -- if left unchecked, their subtle influence
      // on society will become a self-perpetuating Conservative nightmare!
      else /*if(v==VIEW_AMRADIO||v==VIEW_CABLENEWS)*/
      {
         if((int)publicmood(-1)<attitude[v])change_public_opinion(v,-1);
         else change_public_opinion(v,1);
      }
   }

   /*******************************************************
   *                                                      *
   *     INTELLIGENCE REPORT - MOVE TO SEPARATE FILE      *
   *        EYES ONLY - LCS PROPERTY - TOP SECRET         *
   *******************************************************/
   /*{
      int y;
      char str[20];
      erase();
      move(0,0);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("LCS MONTHLY INTELLIGENCE REPORT");

      move(2,2);
      addstr("POLITICAL INFLUENCE");
      y=3;
      for(int i=0;i<VIEWNUM-5;i++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,1);
         getview(str,i);
         addstr(str);
         move(y,20);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("---------");

         // Calculate location for pip (with a bit of randomness for imprecision!)
         int pip=(issuebalance[i]+225)/50+LCSrandom(3)-1; 

         // Select color and limit to ends of spectrum
         if(pip<=0)     { pip=0; set_color(COLOR_RED,    COLOR_BLACK,1); }
         else if(pip<4) {        set_color(COLOR_MAGENTA,COLOR_BLACK,1); }
         else if(pip==4){        set_color(COLOR_YELLOW, COLOR_BLACK,1); }
         else if(pip<8) {        set_color(COLOR_BLUE,   COLOR_BLACK,1); }
         else           { pip=8; set_color(COLOR_GREEN,  COLOR_BLACK,1); }

         move(y++,20+pip);
         addch('O');
      }
      refresh();
      getch();
   }*/
   /*******************************************************
   *                                                      *
   *              END INTELLIGENCE REPORT                 *
   *                                                      *
   *******************************************************/

   //ELECTIONS
   if(month==11){elections(clearformess,canseethings);clearformess=1;}

   //SUPREME COURT
   if(month==6){supremecourt(clearformess,canseethings);clearformess=1;}

   //CONGRESS
   if(month==3){congress(clearformess,canseethings);clearformess=1;}
   if(month==9){congress(clearformess,canseethings);clearformess=1;}

   //DID YOU WIN?
   if(wincheck())
   {
      liberalagenda(1);
      savehighscore(END_WON);
      reset();
      viewhighscores();
      end_game();
   }

   //CONTROL LONG DISBANDS
   if(disbanding&&year-disbandtime>=50)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);

      erase();
      move(12,10);
      addstr("The Liberal Crime Squad is now just a memory.");
      refresh();
      getch();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      erase();
      move(12,12);
      addstr("The last LCS members have all been hunted down.");
      refresh();
      getch();

      set_color(COLOR_BLACK,COLOR_BLACK,1);
      erase();
      move(12,14);
      addstr("They will never see the utopia they dreamed of...");
      refresh();
      getch();

      savehighscore(END_DISBANDLOSS);
      reset();
      viewhighscores();
      end_game();
   }

   //UPDATE THE WORLD IN CASE THE LAWS HAVE CHANGED
   updateworld_laws(law,oldlaw);

   //THE SYSTEM!
   for(p=pool.size()-1;p>=0;p--)
   {
      if(disbanding)break;

      if(!pool[p]->alive)continue;
      if(pool[p]->flag & CREATUREFLAG_SLEEPER)continue;
      if(pool[p]->location==-1)continue;

      if(location[pool[p]->location]->type==SITE_GOVERNMENT_POLICESTATION)
      {
         if(clearformess)
         {
            erase();
         }
         else
         {
            makedelimiter(8,0);
         }

         if(pool[p]->flag & CREATUREFLAG_MISSING)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(8,1);
            addstr(pool[p]->name);
            addstr(" has been rehabilitated from LCS brainwashing.");

            refresh();
            getch();

            removesquadinfo(*pool[p]);
            if(pool[p]->align==1)
            {
               int boss=getpoolcreature(pool[p]->hireid);
               if(boss!=-1&&pool[boss]->juice>50)
               {
                  int juice=pool[boss]->juice-50;
                  if(juice>10)juice=10;
                  addjuice(*pool[boss],-juice);
               }
            }
            delete pool[p];
            pool.erase(pool.begin() + p);
            continue;
         }
         else if(pool[p]->flag & CREATUREFLAG_ILLEGALALIEN)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(8,1);
            addstr(pool[p]->name);
            addstr(" has been shipped out to the INS to face deportation.");

            refresh();
            getch();

            removesquadinfo(*pool[p]);
            if(pool[p]->align==1)
            {
               int boss=getpoolcreature(pool[p]->hireid);
               if(boss!=-1&&pool[boss]->juice>50)
               {
                  int juice=pool[boss]->juice-50;
                  if(juice>10)juice=10;
                  addjuice(*pool[boss],-juice);
               }
            }
            delete pool[p];
            pool.erase(pool.begin() + p);
            continue;
         }
         else
         {
            //TRY TO GET RACKETEERING CHARGE
            int copstrength=100;
            float heat=0;
            if(law[LAW_POLICEBEHAVIOR]==-2)copstrength=200;
            if(law[LAW_POLICEBEHAVIOR]==-1)copstrength=150;
            if(law[LAW_POLICEBEHAVIOR]==1)copstrength=75;
            if(law[LAW_POLICEBEHAVIOR]==2)copstrength=50;

            for(int i=0;i<LAWFLAGNUM;i++)
            {
               heat+=(pool[p]->heat)/4.0f;
            }

            copstrength=static_cast<int>(copstrength*heat);
            if(copstrength>200)copstrength=200;

            //Confession check
            if(LCSrandom(copstrength)>pool[p]->juice  +  pool[p]->attval(ATTRIBUTE_HEART)*5  -
                                      pool[p]->attval(ATTRIBUTE_WISDOM)*5  +  pool[p]->skillval(SKILL_PSYCHOLOGY)*5
                                      /*+ pool[p]->skillval(SKILL_SURVIVAL)*5*/  &&  pool[p]->hireid!=-1)
            {
               int nullify=0;
               int p2=getpoolcreature(pool[p]->hireid);

               if(pool[p2]->alive && (pool[p2]->location==-1 || location[pool[p2]->location]->type!=SITE_GOVERNMENT_PRISON))
               {
                  //Leadership check to nullify subordinate's confession
                  if(LCSrandom(pool[p2]->skillval(SKILL_LEADERSHIP)+1))nullify=1;
                  else
                  {
                     //Charge the boss with racketeering!
                     criminalize(*pool[p2],LAWFLAG_RACKETEERING);
                     //Rack up testimonies against the boss in court!
                     pool[p2]->confessions++;
                  }
               }
               if(!nullify)
               {
                  //Issue a raid on this guy's base!
                  if(pool[p]->base>=0)location[pool[p]->base]->heat+=300;

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(8,1);
                  addstr(pool[p]->name);
                  addstr(" has broken under the pressure and ratted you out!");

                  refresh();
                  getch();

                  removesquadinfo(*pool[p]);

                  int boss=getpoolcreature(pool[p]->hireid);
                  if(boss!=-1&&pool[boss]->juice>50)
                  {
                     int juice=pool[boss]->juice-50;
                     if(juice>5)juice=5;
                     addjuice(*pool[boss],-juice);
                  }

                  delete pool[p];
                  pool.erase(pool.begin() + p);
                  continue; //no trial for this person; skip to next person
               }
               //else continue to trial
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(pool[p]->name);
            addstr(" is moved to the courthouse for trial.");

            refresh();
            getch();

            for(int l=0;l<location.size();l++)
            {
               if(location[l]->type==SITE_GOVERNMENT_COURTHOUSE)
               {
                  pool[p]->location=l;
               }
            }
            pool[p]->armor.type=ARMOR_PRISONER;
         }
      }
      else if(location[pool[p]->location]->type==SITE_GOVERNMENT_COURTHOUSE)
      {
         trial(*pool[p]);
         clearformess=1;
      }
      else if(location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON)
      {
         if(prison(*pool[p]))clearformess=1;
      }
   }

   //NUKE EXECUTION VICTIMS
   for(p=pool.size()-1;p>=0;p--)
   {
      if(pool[p]->location==-1)continue;

      if(location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON&&!pool[p]->alive)
      {
         removesquadinfo(*pool[p]);
         pool[p]->alive=0;
         pool[p]->location=-1;
         //delete pool[p];
         //pool.erase(pool.begin() + p);
      }
   }

   //MUST DO AN END OF GAME CHECK HERE BECAUSE OF EXECUTIONS
   endcheck(END_EXECUTED);

   //DISPERSAL CHECK
   dispersalcheck(clearformess);

   //FUND REPORTS
   fundreport(clearformess);

   
}



/* rename various buildings according to the new laws */
void updateworld_laws(short *law,short *oldlaw)
{
   if((law[LAW_DEATHPENALTY]==-2||oldlaw[LAW_DEATHPENALTY]==-2)&&
      law[LAW_DEATHPENALTY]!=oldlaw[LAW_DEATHPENALTY])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_PRISON) // Prison or re-ed camp?
         {
            initlocation(*location[l]);
         }
         if(location[l]->type==SITE_GOVERNMENT_COURTHOUSE) // Courthouse or judge hall?
         {
            initlocation(*location[l]);
         }
      }
   }

   if((law[LAW_POLICEBEHAVIOR]==-2||oldlaw[LAW_POLICEBEHAVIOR]==-2)&&
      law[LAW_POLICEBEHAVIOR]!=oldlaw[LAW_POLICEBEHAVIOR])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_PRISON) // Prison or re-ed camp?
         {
            initlocation(*location[l]);
         }
         if(location[l]->type==SITE_GOVERNMENT_INTELLIGENCEHQ) // Intelligence HQ or ministry of love?
         {
            initlocation(*location[l]);
         }
      }
   }

   if((law[LAW_FREESPEECH]==-2||oldlaw[LAW_FREESPEECH]==-2)&&
      law[LAW_FREESPEECH]!=oldlaw[LAW_FREESPEECH])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_FIRESTATION) // Fire station or Fireman HQ?
         {
            initlocation(*location[l]);
         }
      }
   }

   if((law[LAW_PRIVACY]==-2||oldlaw[LAW_PRIVACY]==-2)&&
      law[LAW_PRIVACY]!=oldlaw[LAW_PRIVACY])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_INTELLIGENCEHQ) // Intelligence HQ or min. of love?
         {
            initlocation(*location[l]);
         }
      }
   }

   if((law[LAW_CORPORATE]==-2||oldlaw[LAW_CORPORATE]==-2)&&
      law[LAW_CORPORATE]!=oldlaw[LAW_CORPORATE])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_CORPORATE_HOUSE) // CEO house or CEO Castle?
         {
            initlocation(*location[l]);
         }
      }
   }

   if((law[LAW_TAX]==-2||oldlaw[LAW_TAX]==-2)&&
      law[LAW_TAX]!=oldlaw[LAW_TAX])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_CORPORATE_HOUSE) // CEO house or CEO Castle?
         {
            initlocation(*location[l]);
         }
      }
   }
}




