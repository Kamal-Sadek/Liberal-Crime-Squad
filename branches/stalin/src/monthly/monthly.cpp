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
      if((attitude[VIEW_POLITICALVIOLENCE]>60||publicmood(-1)>60))
      {
         endgamestate=ENDGAME_CCS_APPEARANCE;
         attitude[VIEW_CONSERVATIVECRIMESQUAD]=100-attitude[VIEW_POLITICALVIOLENCE];
      }
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
   int issuebalance[VIEWNUM-6];
   int stimulus=0;
   double cost=0;
   double tax=0;
   
   //PUBLIC OPINION NATURAL MOVES
   for(v=0;v<VIEWNUM;v++)
   {
      // Liberal essays add their power to the effect of sleepers
      libpower[v]+=background_liberal_influence[v];
      background_liberal_influence[v]=0;

      if(v==VIEW_LIBERALCRIMESQUADPOS)continue;
      if(v==VIEW_LIBERALCRIMESQUAD)continue;
      if(v==VIEW_POLITICALVIOLENCE)
      {
         change_public_opinion(VIEW_POLITICALVIOLENCE,-1,0);
         continue;
      }
      if(v==VIEW_CONSERVATIVECRIMESQUAD)continue;
      if(v!=VIEW_AMRADIO&&v!=VIEW_CABLENEWS&&v!=VIEW_STALIN&&v!=VIEW_DEBT&&v!=VIEW_ECONOMY)
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
      else if(v==VIEW_AMRADIO||v==VIEW_CABLENEWS)
      {
         if((int)publicmood(-1)<attitude[v])change_public_opinion(v,-1);
         else change_public_opinion(v,1);
      }
	   else if (v==VIEW_STALIN)
	   {
         if(VIEW_STALIN<=30)
         {
            change_public_opinion(VIEW_WOMEN,-2);
            change_public_opinion(VIEW_POLICEBEHAVIOR,-2);
            change_public_opinion(VIEW_DRUGS,-2);
            change_public_opinion(VIEW_GAY,-2);
            change_public_opinion(VIEW_DEATHPENALTY,-2);
            change_public_opinion(VIEW_FREESPEECH,-2);
            change_public_opinion(VIEW_STALIN,-4);
            change_public_opinion(VIEW_NUCLEARPOWER,-2);
            change_public_opinion(VIEW_CIVILRIGHTS,-2);
            change_public_opinion(VIEW_INTELLIGENCE,-2);
            change_public_opinion(VIEW_SWEATSHOPS,2);
            change_public_opinion(VIEW_CEOSALARY,2);
            change_public_opinion(VIEW_CORPORATECULTURE,2);
            change_public_opinion(VIEW_TAXES,2);
			change_public_opinion(VIEW_RELIGION,2);
            change_public_opinion(VIEW_POLLUTION,-2);
            change_public_opinion(VIEW_GUNCONTROL,2);
            change_public_opinion(VIEW_POLITICALVIOLENCE,2);
         }
         if(VIEW_STALIN<=50)
         {		  
            change_public_opinion(VIEW_WOMEN,-1);
            change_public_opinion(VIEW_POLICEBEHAVIOR,-1);
            change_public_opinion(VIEW_DRUGS,-1);
            change_public_opinion(VIEW_GAY,-1);
            change_public_opinion(VIEW_DEATHPENALTY,-1);
            change_public_opinion(VIEW_FREESPEECH,-1);
            change_public_opinion(VIEW_STALIN,-2);
            change_public_opinion(VIEW_NUCLEARPOWER,-1);
            change_public_opinion(VIEW_CIVILRIGHTS,-1);
            change_public_opinion(VIEW_INTELLIGENCE,-1);
            change_public_opinion(VIEW_SWEATSHOPS,1);
            change_public_opinion(VIEW_CEOSALARY,1);
            change_public_opinion(VIEW_TAXES,1);
            change_public_opinion(VIEW_POLLUTION,-1);
			change_public_opinion(VIEW_RELIGION,1);
            change_public_opinion(VIEW_GUNCONTROL,1);
            change_public_opinion(VIEW_POLITICALVIOLENCE,1);
         }
         else
         {
            change_public_opinion(VIEW_STALIN,-1);
         }
	   }
	   else if (v==VIEW_ECONOMY)
	   {
		   //Low Taxes and High Welfare postively affect the economy. Welfare is more effective than taxes (liberal bias
		   //after all)), but you'll need both to have a good economy. Corporate Regulations are a little random.
		   //At C+, when times are good, they are good, but when times are bad, heads start to roll. At L+, Corporate greed is
		   //reined in, but growth is sluggish as a result. Pollution regulations impose a cost on businesses, and always 
		   //negatively affect the economy. I was considering having immigration also postively effect the economy
		   //(cheap labor supply), but then the American people would kinda be seen as stupid for supporting 
		   //a policy that would actually continue an economic recession.
		   switch(l)
		   {
		   case LAW_TAX:
		       if(law[l]==ALIGN_ARCHCONSERVATIVE)stimulus+=10;
               else if(law[l]==ALIGN_CONSERVATIVE)stimulus+=8;
               else if(law[l]==ALIGN_MODERATE)stimulus+=6;
               else if(law[l]==ALIGN_LIBERAL)stimulus+=3;
               else if(law[l]==ALIGN_ELITELIBERAL)stimulus+=0;
			   break;
		   case LAW_WELFARE:
			   if(law[l]==ALIGN_ARCHCONSERVATIVE)stimulus+=0;
               else if(law[l]==ALIGN_CONSERVATIVE)stimulus+=3;
               else if(law[l]==ALIGN_MODERATE)stimulus+=6;
               else if(law[l]==ALIGN_LIBERAL)stimulus+=9;
               else if(law[l]==ALIGN_ELITELIBERAL)stimulus+=11;
			   break;
		   case LAW_CORPORATE:
			   if(law[l]==ALIGN_ARCHCONSERVATIVE)stimulus+=(LCSrandom(15)-LCSrandom(8));
               else if(law[l]==ALIGN_CONSERVATIVE)stimulus+=(LCSrandom(10)-LCSrandom(5));
               else if(law[l]==ALIGN_MODERATE)stimulus+=(LCSrandom(5)-LCSrandom(3));
               else if(law[l]==ALIGN_LIBERAL)stimulus+=(3-LCSrandom(5));
               else if(law[l]==ALIGN_ELITELIBERAL)stimulus+=(1-LCSrandom(6));
			   break;
		   case LAW_POLLUTION:
			   if(law[l]==ALIGN_ARCHCONSERVATIVE)stimulus+=0;
               else if(law[l]==ALIGN_CONSERVATIVE)stimulus+=-2;
               else if(law[l]==ALIGN_MODERATE)stimulus+=-5;
               else if(law[l]==ALIGN_LIBERAL)stimulus+=-8;
               else if(law[l]==ALIGN_ELITELIBERAL)stimulus+=-12;
			   break;
		   }

         int roll = cost + LCSrandom(400)-200;
         // Heavy randomization -- stimulus just biases the roll
         // If +/-50 to either side, that side wins the tug-of-war
         if(roll < -50)
         {
            change_public_opinion(VIEW_DEBT,-LCSrandom(5),0);
         }
         else if(roll > 50)
         {
            change_public_opinion(VIEW_DEBT,LCSrandom(5),0);
         }
         else // Else random movement
         {
            change_public_opinion(VIEW_ECONOMY,LCSrandom(2)*2-1,0);
         }
	  //Economy affects a whole slew of issues. For the most part, the economy is "self-correcting",
	  //altough it is rather sluggish, and needs a little 'assistance'. In a poor economy, people want to pay lower taxes,
	  //kick out migrant workers who might be stealing American jobs, end job-stifling pollution regulations,
	  //hand out money to poor people, purge corporations of greedy CEOes, and support political terrorism.
	  //Every action (except for kicking out migrant workers, and committing terrorism) will make the economy
	  //better off in the long term. In a good economy, people are willing to consider pollution regulations
	  //and allow immigrants to return, but they also will support C+ corporate goverence, cut back on
	  //welfare payments (after all...it's a good economy!), and no longer support terrorism.
	  //Sorry for the long comment. I have to explain everything I'm doing here.---SC
		   if(VIEW_ECONOMY>=70)
		   {
			   change_public_opinion(VIEW_TAXES,-2);
			   change_public_opinion(VIEW_POLLUTION,-2);
			   change_public_opinion(VIEW_CORPORATECULTURE,2);
			   change_public_opinion(VIEW_CEOSALARY,2);
			   change_public_opinion(VIEW_POLITICALVIOLENCE,2);
			   change_public_opinion(VIEW_IMMIGRATION,-1);
			   change_public_opinion(VIEW_WELFARE,1);
		   }
		   if(VIEW_ECONOMY>=50)
		   {
			   change_public_opinion(VIEW_TAXES,-1);
			   change_public_opinion(VIEW_POLLUTION,-1);
			   change_public_opinion(VIEW_CORPORATECULTURE,1);
			   change_public_opinion(VIEW_CEOSALARY,1);
			   change_public_opinion(VIEW_POLITICALVIOLENCE,1);
		   }		   	
		   if(VIEW_ECONOMY>=30)
		   {
			   change_public_opinion(VIEW_POLLUTION,1);
			   change_public_opinion(VIEW_CORPORATECULTURE,-1);
			   change_public_opinion(VIEW_CEOSALARY,-1);
			   change_public_opinion(VIEW_POLITICALVIOLENCE,-1);
		   }
		   if(VIEW_ECONOMY<30)
		   {
			   change_public_opinion(VIEW_POLLUTION,2);
			   change_public_opinion(VIEW_CORPORATECULTURE,-2);
			   change_public_opinion(VIEW_CEOSALARY,-2);
			   change_public_opinion(VIEW_POLITICALVIOLENCE,-2);
			   change_public_opinion(VIEW_WELFARE,-1);
			   change_public_opinion(VIEW_IMMIGRATION,1);
		   }
	   }
	   else if (v==VIEW_DEBT)
	   {
		   		   switch(l)
		   {
			   //The amount of tax revenue depends on the health of the national economy.
		   case LAW_TAX:
		       if(law[l]==ALIGN_ARCHCONSERVATIVE)tax+=((100-attitude[VIEW_ECONOMY])*0.10);
               else if(law[l]==ALIGN_CONSERVATIVE)tax+=((100-attitude[VIEW_ECONOMY])*0.25);
               else if(law[l]==ALIGN_MODERATE)tax+=((100-attitude[VIEW_ECONOMY])*0.35);
               else if(law[l]==ALIGN_LIBERAL)tax+=((100-attitude[VIEW_ECONOMY])*0.5);
               else if(law[l]==ALIGN_ELITELIBERAL)tax+=((100-attitude[VIEW_ECONOMY])*0.75);
			   break;
		   case LAW_WELFARE:
			   if(law[l]==ALIGN_ARCHCONSERVATIVE)cost+=0;
               else if(law[l]==ALIGN_CONSERVATIVE)cost+=15;
               else if(law[l]==ALIGN_MODERATE)cost+=30;
               else if(law[l]==ALIGN_LIBERAL)cost+=45;
               else if(law[l]==ALIGN_ELITELIBERAL)cost+=60;
			   break;
			   //Pollution regulations are actually pretty cheap, for the government at least.
			   //That being said, I'm only looking at EPA and Superfund costs, which compose
			   //only a small part of the national budget. However, it might be "unbalancing",
			   //since Military Upkeep is more expensive that Pollution.---SC
		   case LAW_POLLUTION:
			   if(law[l]==ALIGN_ARCHCONSERVATIVE)cost+=0;
               else if(law[l]==ALIGN_CONSERVATIVE)cost+=0;
               else if(law[l]==ALIGN_MODERATE)cost+=1;
               else if(law[l]==ALIGN_LIBERAL)cost+=3;
               else if(law[l]==ALIGN_ELITELIBERAL)cost+=6;
			   break;
		   case LAW_MILITARY:
			   if(law[l]==ALIGN_ARCHCONSERVATIVE)cost+=30;
               else if(law[l]==ALIGN_CONSERVATIVE)cost+=20;
               else if(law[l]==ALIGN_MODERATE)cost+=10;
               else if(law[l]==ALIGN_LIBERAL)cost+=5;
               else if(law[l]==ALIGN_ELITELIBERAL)cost+=0;
			   break;
		   }
	    int deficit = (int)(tax - cost);
		int roll = deficit + LCSrandom(400)-200;
         // Heavy randomization -- stimulus just biases the roll
         // If +/-50 to either side, that side wins the tug-of-war
         if(roll < -50)
         {
            change_public_opinion(VIEW_DEBT,-LCSrandom(5),0);
         }
         else if(roll > 50)
         {
            change_public_opinion(VIEW_DEBT,LCSrandom(5),0);
         }
         else // Else random movement
         {
            change_public_opinion(VIEW_DEBT,LCSrandom(2)*2-1,0);
         }
	//The National Debt also affect a whole slew of issues as well. If the National Debt is high,
	   //people will raise taxes and cut welfare payments. If the national debt is too high, people
	   //will begin considering ending Pollution regulations and cutting back on the US Military.
	   //When there is a surplus, there will be demand to give that money back to Americans, either
	   //via Welfare payments or Tax Cuts. If the National Debt is low enough, pork barrel projects win,
	   //and there will be a push to throw all that excess money to enviromentalists and militarists.---SC
		   if(VIEW_DEBT>=70)
		   {
			   change_public_opinion(VIEW_TAXES,2);
			   change_public_opinion(VIEW_WELFARE,-2);
			   change_public_opinion(VIEW_POLLUTION,-1);
			   change_public_opinion(VIEW_MILITARY,1);
		   }
		   if(VIEW_DEBT>=50)
		   {
			   change_public_opinion(VIEW_TAXES,1);
			   change_public_opinion(VIEW_WELFARE,-1);
		   }
		   if(VIEW_DEBT>=30)
		   {
			   change_public_opinion(VIEW_TAXES,-1);
			   change_public_opinion(VIEW_WELFARE,1);
		   }
		   else
		   {
			   change_public_opinion(VIEW_TAXES,-2);
			   change_public_opinion(VIEW_WELFARE,2);
			   change_public_opinion(VIEW_POLLUTION,1);
			   change_public_opinion(VIEW_MILITARY,-1);
		   }
	   }
   }

   // Seduction monthly experience stipends for those liberals
   // who have been getting it on with their love slaves/masters
   // in the background
   for(int p=0;p<pool.size();p++)
   {
      pool[p]->train(SKILL_SEDUCTION,loveslaves(*pool[p])*5);
      if(pool[p]->flag & CREATUREFLAG_LOVESLAVE)
         pool[p]->train(SKILL_SEDUCTION,5);
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
      addstr("ESTIMATED POLITICAL INFLUENCE");
      y=3;
      for(int i=0;i<VIEWNUM-6;i++)
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
         else if(pip<8) {        set_color(COLOR_CYAN,   COLOR_BLACK,1); }
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
         else if(pool[p]->flag & CREATUREFLAG_ILLEGALALIEN && law[LAW_IMMIGRATION]!=2)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(8,1);
            addstr(pool[p]->name);
            addstr(" has been shipped out to the INS to face ");
            if(law[LAW_IMMIGRATION]==-2 && law[LAW_DEATHPENALTY]==-2)
               addstr("execution.");
            else
               addstr("deportation.");

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

   if(law[LAW_GUNCONTROL]==2&&oldlaw[LAW_GUNCONTROL]<2)
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_BUSINESS_PAWNSHOP) // Do they mention guns in the title?
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
      if((law[LAW_DRUGS]==2||oldlaw[LAW_DRUGS]==2)&&
      law[LAW_DRUGS]!=oldlaw[LAW_DRUGS])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_BUSINESS_CRACKHOUSE) // Crack House, or Recreational Drugs Center?
         {
            initlocation(*location[l]);
         }
      }
	  }
	  if((law[LAW_NUCLEARPOWER]==2||oldlaw[LAW_NUCLEARPOWER]==2)&&
      law[LAW_NUCLEARPOWER]!=oldlaw[LAW_NUCLEARPOWER])
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_INDUSTRY_NUCLEAR) // Nuclear Power Plant, or Nuclear Waste Center?
         {
            initlocation(*location[l]);
         }
      }
   }
}



