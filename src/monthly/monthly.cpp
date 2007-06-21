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
      /*short power=nploc.size();
      
      //CRIMINALIZE THE PRINTERS
      long writingpower=0;
      for(int l=0;l<nploc.size();l++)
      {
         criminalizepool(LAWFLAG_SPEECH,-1,nploc[l]);
         for(int p=0;p<pool.size();p++)
         {
            if(pool[p]->location==nploc[l]&&
               pool[p]->alive&&
               pool[p]->align==1)
            {
               writingpower+=LCSrandom(pool[p]->skill[SKILL_PERSUASION]+
                  pool[p]->skill[SKILL_WRITING]*3+
                  pool[p]->attval(ATTRIBUTE_INTELLIGENCE)+
                  pool[p]->attval(ATTRIBUTE_CHARISMA)+
                  pool[p]->attval(ATTRIBUTE_HEART)*2+1);
               pool[p]->skill_ip[SKILL_WRITING]+=LCSrandom(2)+1;
               pool[p]->skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;
            }
         }
      }*/

      /*
      //DO EMPHASIS OF SITE EVENTS
      int vpower,val;
      for(int v=0;v<VIEWNUM;v++)
      {
         val=0; // *JDS* Newspaper's emphasis strength starts at 0, increases with writing power
         if(LCSrandom(100)<writingpower)val++;
         if(LCSrandom(100)<writingpower)val++;
         if(LCSrandom(100)<writingpower)val++;
         if(LCSrandom(1000)<writingpower)val++;
         if(LCSrandom(1000)<writingpower)val++;
         if(LCSrandom(1000)<writingpower)val++;
         if(LCSrandom(10000)<writingpower)val++;
         if(LCSrandom(10000)<writingpower)val++;
         if(LCSrandom(10000)<writingpower)val++;
         if(LCSrandom(10000)<writingpower)val++;
         if(val>newspaper_topicwork[v])val=newspaper_topicwork[v];
         vpower=power*val;
         if(vpower>0)
         {
            change_public_opinion(v,vpower,1,0);
         }
      }
      */

      //DO SPECIAL EDITIONS
      int loottype=choosespecialedition(clearformess);

      if(loottype!=-1)
      {
         guardianpower+=10*nploc.size();
         printnews(loottype,nploc.size());

         for(int p=0;p<pool.size();p++)
         {
            if(pool[p]->alive&&pool[p]->align==1&&
               pool[p]->clinic==0&&pool[p]->dating==0&&
               pool[p]->hiding==0)
            {
               if(pool[p]->location!=-1)
               {
                  if(location[pool[p]->location]->type!=SITE_GOVERNMENT_PRISON&&
                     location[pool[p]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
                     location[pool[p]->location]->type!=SITE_GOVERNMENT_POLICESTATION)
                  {
                     if(loottype==LOOT_INTHQDISK||
                        loottype==LOOT_SECRETDOCUMENTS)addjuice(*pool[p],100);
                     else addjuice(*pool[p],50);
                  }
               }
            }
         }

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
   int computernum=0;
   /*

   for(int dude=0;dude<pool.size();dude++)
   {
      if(pool[dude]->alive&&pool[dude]->align==1&&
         pool[dude]->clinic==0&&pool[dude]->dating==0&&
         pool[dude]->hiding==0&&pool[dude]->skill[SKILL_COMPUTERS])
      {
         if(pool[dude]->location!=-1)
         {
            if(location[pool[dude]->location]->type!=SITE_GOVERNMENT_PRISON&&
               location[pool[dude]->location]->type!=SITE_GOVERNMENT_COURTHOUSE&&
               location[pool[dude]->location]->type!=SITE_GOVERNMENT_POLICESTATION)
            {
               computernum++;
            }
         }
      }
   }*/

   if(blogpost.size()&&!disbanding)
   {
      blogpostst* bp=blogpost.back();
      while(blogpost.size())
      {
         if(bp->type==REPORT_NEWS)
         {
            guardianpower+=bp->power;
            libpower[bp->issue]+=bp->power;
         }
         else
         {
            // Persuasive arguments tend to popularize the LCS
            if(bp->power>=3)
            {
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,2,0);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,1,0);
            }
            else if(bp->power>0&&(attitude[VIEW_LIBERALCRIMESQUAD]<20||!LCSrandom(20)))
            {
               change_public_opinion(VIEW_LIBERALCRIMESQUAD,1,0);
               change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,LCSrandom(2),0);
            }

            if(bp->type==REPORT_ATTACK)
            {
               // Offensive or libellous attack articles by shoddy writers
               // on topics that the public largely disagrees with the LCS
               // on tend to scandalize the public and hurt the cause for
               // free speech (not to mention offend their view of the LCS)
               if(bp->power<0&&attitude[bp->issue]<50)
               {
                  change_public_opinion(VIEW_FREESPEECH,(attitude[bp->issue]-50)/25,0);
                  change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,(attitude[bp->issue]-50)/10,0);
               }
               // But they are more effective
               bp->power+=1;
            }

            // Modify power by lcs popularity
            bp->power+=(attitude[VIEW_LIBERALCRIMESQUADPOS]-50)/26;
            bp->power*=static_cast<long>((attitude[VIEW_LIBERALCRIMESQUAD]*(nploc.size()+1))/100.0f);
            
            if(bp->issue<VIEWNUM-2)
            {
               if(bp->power>0)
               {
                  if(attitude[bp->issue]<30+newspaper_topicwork[bp->issue]+LCSrandom(20))
                  {
                     guardianpower+=bp->power;
                     libpower[bp->issue]+=3*bp->power;
                  }
               }
               else
               {
                  if(attitude[bp->issue]>70-LCSrandom(20))
                  {
                     guardianpower+=bp->power;
                     libpower[bp->issue]+=3*bp->power;
                  }
               }
            }
            else 
            {
               guardianpower+=bp->power;
               libpower[bp->issue]+=3*bp->power;
            }
         }
         delete bp;
         blogpost.pop_back();
         bp=blogpost.back();
      }
      /*for(int i=0;i<VIEWNUM;i++)
      {
         libpower[i]+=guardianpower;
      }*/
      clearformess=1;

      guardianupdate(nploc.size(),guardianpower);
   }

   //STORIES STALE EVEN IF NOT PRINTED
   for(v=0;v<VIEWNUM;v++)newspaper_topicwork[v]=0;

   int conspower=200-attitude[VIEW_AMRADIO]-attitude[VIEW_CABLENEWS];
   
   //HAVING SLEEPERS
   for(int pl=0;pl<pool.size();pl++)
   {
      if(pool[pl]->alive&&(pool[pl]->flag & CREATUREFLAG_SLEEPER))
      {
         sleepereffect(*pool[pl],clearformess,canseethings,libpower);
      }
   }
   
   //PUBLIC OPINION NATURAL MOVES
   for(v=0;v<VIEWNUM;v++)
   {
      
      if(v==VIEW_LIBERALCRIMESQUADPOS)continue;
      if(v==VIEW_LIBERALCRIMESQUAD)continue;
      if(v!=VIEW_AMRADIO&&v!=VIEW_CABLENEWS)
      {
         //DRIFTS DOWN DEPENDING ON AM RADIO AND CABLE NEWS
         
         
         if(LCSrandom(200)>conspower)
         {
            change_public_opinion(v,LCSrandom(2)*2-1,0);
         }
         else
         {
            change_public_opinion(v,-1,0);

            //Crushing conservative media silences liberal influence
            if(LCSrandom(200)<conspower/2)
            {
               libpower[v]-=75;
            }
         }
      }
      if(LCSrandom(200)<libpower[v])
      {
         change_public_opinion(v,1,0);
      }
      
      if(v==VIEW_AMRADIO||v==VIEW_CABLENEWS)
      {
         if(!LCSrandom(3))change_public_opinion(v,-1,0);
      }
   }

   //ELECTIONS
   if(month==11){elections(clearformess,canseethings);clearformess=1;}

   //SUPREME COURT
   if(month==6){supremecourt(clearformess,canseethings);clearformess=1;}

   //CONGRESS
   if(month==3){congress(clearformess,canseethings);clearformess=1;}
   if(month==9){congress(clearformess,canseethings);clearformess=1;}

   //REAGANIFY?
   if(publicmood(-1)<=8)
   {
      reaganify(canseethings);
      clearformess=1;
   }

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
      addstr("You disappeared safely, but you hadn't done enough.");
      refresh();
      getch();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      erase();
      move(12,12);
      addstr("The Conservatives have made the world in their image.");
      refresh();
      getch();

      set_color(COLOR_BLACK,COLOR_BLACK,1);
      erase();
      move(12,14);
      addstr("They'll round the last of you up eventually.  All is lost.");
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

         if(pool[p]->flag & CREATUREFLAG_ILLEGALALIEN)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            move(8,1);
            addstr(pool[p]->name);
            addstr(" has been shipped out to the INS to face deportation.");

            refresh();
            getch();

            removesquadinfo(*pool[p]);
            delete pool[p];
            pool.erase(pool.begin() + p);
            continue;
         }
         else
         {
            //TRY TO GET RACKETEERING CHARGE
            int copstrength=100;
            if(law[LAW_POLICEBEHAVIOR]==-2)copstrength=200;
            if(law[LAW_POLICEBEHAVIOR]==-1)copstrength=150;
            if(law[LAW_POLICEBEHAVIOR]==1)copstrength=75;
            if(law[LAW_POLICEBEHAVIOR]==2)copstrength=50;

            if(LCSrandom(copstrength)>pool[p]->juice+pool[p]->attval(ATTRIBUTE_HEART,0)*5&&pool[p]->hireid!=-1)
            {
               if(pool[p]->hireid!=-1)
               {
                  for(int p2=0;p2<pool.size();p2++)
                  {
                     if(pool[p2]->alive==1&&pool[p2]->id==pool[p]->hireid)
                     {
                        char conf=0;
                        if(pool[p2]->location==-1)conf=1;
                        else if(location[pool[p2]->location]->type!=SITE_GOVERNMENT_PRISON)conf=1;

                        if(conf)
                        {
                           pool[p2]->lawflag[LAWFLAG_RACKETEERING]++;
                        }
                     }
                  }
               }

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(pool[p]->name);
               addstr(" has broken under the pressure and ratted you out!");

               refresh();
               getch();

               removesquadinfo(*pool[p]);
               delete pool[p];
               pool.erase(pool.begin() + p);
               continue;
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(pool[p]->name);
               addstr(" is moved to the courthouse for trial.");

               refresh();
               getch();
            }

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
         delete pool[p];
         pool.erase(pool.begin() + p);
      }
   }

   //MUST DO AN END OF GAME CHECK HERE BECAUSE OF EXECUTIONS
   endcheck(END_EXECUTED);

   //DISPERSAL CHECK
   dispersalcheck(clearformess);

   //FUND REPORTS
   fundreport(clearformess);

   //HEAL CLINIC PEOPLE AND TRAIN
   for(p=0;p<pool.size();p++)
   {
      if(disbanding)break;

      if(pool[p]->clinic>0)
      {
         pool[p]->clinic--;

         for(int w=0;w<BODYPARTNUM;w++)
         {
            if((pool[p]->wound[w] & WOUND_NASTYOFF)||
               (pool[p]->wound[w] & WOUND_CLEANOFF))
            {
               pool[p]->wound[w]=(char)WOUND_CLEANOFF;
            }
            else pool[p]->wound[w]=0;
         }

         if(pool[p]->special[SPECIALWOUND_RIGHTLUNG]!=1)
         {
            pool[p]->special[SPECIALWOUND_RIGHTLUNG]=1;
            if(LCSrandom(2))
            {
               pool[p]->att[ATTRIBUTE_HEALTH]--;
               if(pool[p]->att[ATTRIBUTE_HEALTH]<=0)
               {
                  pool[p]->att[ATTRIBUTE_HEALTH]=1;
               }
            }
         }
         if(pool[p]->special[SPECIALWOUND_LEFTLUNG]!=1)
         {
            pool[p]->special[SPECIALWOUND_LEFTLUNG]=1;
            if(LCSrandom(2))
            {
               pool[p]->att[ATTRIBUTE_HEALTH]--;
               if(pool[p]->att[ATTRIBUTE_HEALTH]<=0)
               {
                  pool[p]->att[ATTRIBUTE_HEALTH]=1;
               }
            }
         }
         if(pool[p]->special[SPECIALWOUND_HEART]!=1)
         {
            pool[p]->special[SPECIALWOUND_HEART]=1;
            if(LCSrandom(3))
            {
               pool[p]->att[ATTRIBUTE_HEALTH]--;
               if(pool[p]->att[ATTRIBUTE_HEALTH]<=0)
               {
                  pool[p]->att[ATTRIBUTE_HEALTH]=1;
               }
            }
         }
         pool[p]->special[SPECIALWOUND_LIVER]=1;
         pool[p]->special[SPECIALWOUND_STOMACH]=1;
         pool[p]->special[SPECIALWOUND_RIGHTKIDNEY]=1;
         pool[p]->special[SPECIALWOUND_LEFTKIDNEY]=1;
         pool[p]->special[SPECIALWOUND_SPLEEN]=1;
         pool[p]->special[SPECIALWOUND_RIBS]=RIBNUM;
         if(!pool[p]->special[SPECIALWOUND_NECK])
         {
            pool[p]->special[SPECIALWOUND_NECK]=2;
         }
         if(!pool[p]->special[SPECIALWOUND_UPPERSPINE])
         {
            pool[p]->special[SPECIALWOUND_UPPERSPINE]=2;
         }
         if(!pool[p]->special[SPECIALWOUND_LOWERSPINE])
         {
            pool[p]->special[SPECIALWOUND_LOWERSPINE]=2;
         }

         if(pool[p]->blood<=20&&pool[p]->clinic<=2)pool[p]->blood=50;
         if(pool[p]->blood<=50&&pool[p]->clinic<=1)pool[p]->blood=75;

         if(pool[p]->clinic==0)
         {
            pool[p]->blood=100;
            if(clearformess)
            {
               erase();
            }
            else
            {
               makedelimiter(8,0);
            }

            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr(pool[p]->name);
            addstr(" has left ");
            addstr(location[pool[p]->location]->name);
            addstr(".");

            int hs=-1;
            for(int l=0;l<location.size();l++)
            {
               if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
               {
                  hs=l;
                  break;
               }
            }
            if (hs==-1)
            {
               //TODO: Error unable to find location
               hs=0;
            }

            if(location[pool[p]->base]->siege.siege)
            {
               pool[p]->base=hs;
            }

            pool[p]->location=pool[p]->base;

            refresh();
            getch();
         }
      }
   }
}



/* rename prison according to the new laws (add more buildings to this) */
void updateworld_laws(short *law,short *oldlaw)
{
   //RENAME PRISONS
   if(law[LAW_DEATHPENALTY]==-2&&
      law[LAW_POLICEBEHAVIOR]==-2&&
      (oldlaw[LAW_DEATHPENALTY]>-2||
      oldlaw[LAW_POLICEBEHAVIOR]>-2))
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_PRISON)
         {
            initlocation(*location[l]);
         }
      }
   }
   //RENAME CAMPS
   else if(oldlaw[LAW_DEATHPENALTY]==-2&&
      oldlaw[LAW_POLICEBEHAVIOR]==-2&&
      (law[LAW_DEATHPENALTY]>-2||
      law[LAW_POLICEBEHAVIOR]>-2))
   {
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_GOVERNMENT_PRISON)
         {
            initlocation(*location[l]);
         }
      }
   }
}




