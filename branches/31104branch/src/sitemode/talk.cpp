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



/* bluff, date, issues */
char talk(creaturest &a,int t)
{
   //BLUFFING
   if((sitealarm||location[cursite]->siege.siege)&&encounter[t].align==-1)
   {
      clearcommandarea();
      clearmessagearea();
      clearmaparea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(9,1);
      addstr(a.name);
      addstr(" talks to ");
      addstr(encounter[t].name);
      addstr(":");

      int c=0;

      if(encounter[t].type==CREATURE_COP||
         encounter[t].type==CREATURE_GANGUNIT||
         encounter[t].type==CREATURE_SWAT)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(11,1);
         addstr("A - Bluff");
         move(12,1);
         addstr("B - Give up");
         while(c!='a'&&c!='b')
         {
            c=getch();
            translategetch(c);
         }
      }

      if(c!='b')
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         if(location[cursite]->siege.siege)
         {
            addstr(a.name);
            addstr(" ");
            switch(location[cursite]->siege.siegetype)
            {
               case SIEGE_POLICE:
                  addstr("pretends to be part of a police raid.");
                  break;
               case SIEGE_CIA:
                  addstr("pretends to be a Secret Agent.");
                  break;
               case SIEGE_HICKS:
                  switch(LCSrandom(2))
                  {
                     case 0:
                        addstr("pretends to be Mountain");
                        move(17,1);
                        addstr("like Patrick Swayze in Next of Kin.");
                        break;
                     case 1:
                        addstr("squeals like Ned Beatty");
                        move(17,1);
                        addstr("in Deliverance.");
                        break;
                  }
                  break;
               case SIEGE_CORPORATE:
                  addstr("pretends to be a mercenary.");
                  break;
            }
         }
         else
         {
            addstr("talks like a Conservative");
            move(17,1);
            addstr("and pretends to belong here.");
         }

         refresh();
         getch();

         vector<int> noticer;
         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].exists&&encounter[e].alive&&
               encounter[e].align==-1)
            {
               noticer.push_back(e);
            }
         }

         if(noticer.size()>0)
         {
            int disguise=disguiseskill();
            int weapon=0;
            for(int i=0;i>0;i--)
            {
               if(activesquad->squad[i]==NULL)break;
               weapon+=weaponcheck(*activesquad->squad[i],cursite);
            }

            char noticed=0,bluff=0;
            int n,an;

            do
            {
               an=LCSrandom(noticer.size());
               n=noticer[an];
               noticer.erase(noticer.begin() + an);

               int chance=encounter[n].attval(ATTRIBUTE_WISDOM)*3+
                  encounter[n].attval(ATTRIBUTE_INTELLIGENCE);

               if(chance+10*weapon+sitecrime > (int)LCSrandom(21)+disguise)
               {
                  noticed=1;
                  break;
               }

            }while(noticer.size()>0);

            //NOW MUST BLUFF
            if(!noticed)
            {
               short aroll=LCSrandom(21)+a.attval(ATTRIBUTE_CHARISMA)+
                  a.attval(ATTRIBUTE_WISDOM)*3+LCSrandom(a.skill[SKILL_PERSUASION]*2+1);

               int maxtroll=0,troll;
               for(int e=0;e<ENCMAX;e++)
               {
                  if(encounter[e].exists&&encounter[e].alive&&
                     encounter[e].align==-1)
                  {
                     troll=encounter[e].attval(ATTRIBUTE_WISDOM)*3+
                        encounter[e].attval(ATTRIBUTE_INTELLIGENCE);
                     if(troll>maxtroll)
                     {
                        n=e;
                        maxtroll=troll;
                     }
                  }
               }

               maxtroll+=LCSrandom(21);
               a.skill_ip[SKILL_PERSUASION]+=(maxtroll>>2)+1;

               if(maxtroll>aroll)
               {
                  clearmessagearea();

                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(16,1);
                  if(encounter[n].type==CREATURE_HICK)
                  {
                     addstr("But ");
                     addstr(encounter[n].name);
                     addstr(" weren't born yesterday.");
                  }
                  else
                  {
                     addstr(encounter[n].name);
                     addstr(" is not fooled by that crap.");
                  }

                  refresh();
                  getch();
               }
               else
               {
                  clearmessagearea();

                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(16,1);
                  addstr("The Enemy is fooled and departs.");
                  bluff=1;

                  refresh();
                  getch();
               }
            }
            else
            {
               clearmessagearea();

               if(!location[cursite]->siege.siege&&!sitealarm)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(16,1);
                  addstr(encounter[n].name);
                  addstr(" looks at the Squad with Intolerance");
                  move(17,1);
                  addstr("and lets forth a piercing Conservative alarm cry!");

                  sitealarm=1;
               }
               else
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(16,1);
                  addstr(encounter[n].name);
                  addstr(" looks at the Squad and isn't fooled.");
               }

               refresh();
               getch();
            }

            if(bluff)
            {
               for(int e=ENCMAX-1;e>=0;e--)
               {
                  if(encounter[e].exists&&encounter[e].alive&&
                     encounter[e].align==-1)
                  {
                     delenc(e,0);
                  }
               }
            }
         }
      }
      else
      {
         move(14,1);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("The police arrest the Squad.");
         getch();
         for(int i=0;i<6;++i)
         {
            if(activesquad->squad[i])capturecreature(*activesquad->squad[i]);
            activesquad->squad[i]=NULL;
         }
      }
      return 1;
   }

   //TALKING
   short talkmode=TALKMODE_START;

   creaturest *tk=&encounter[t];

   do
   {
      clearcommandarea();
      clearmessagearea();
      clearmaparea();

      int c='a';
      if(talkmode!=TALKMODE_ISSUES)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(9,1);
         addstr(a.name);
         addstr(" talks to ");
         addstr(tk->name);
         addstr(":");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         switch(talkmode)
         {
            case TALKMODE_RENTING:
               move(11,1);
               addstr("A - Accept.");
               move(12,1);
               addstr("B - Decline.");
               break;
            case TALKMODE_START:
               move(11,1);
               addstr("A - Strike up a conversation about the issues");
               if(a.armor.type==ARMOR_NONE)addstr(" while naked");
               addstr(".");
               move(12,1);
               if(!kid(*tk)&&!kid(a))set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               addstr("B - Drop a pickup line");
               if(a.armor.type==ARMOR_NONE)addstr(" while naked");
               addstr(".");
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(13,1);
               addstr("C - On second thought, don't say anything");
               if(a.armor.type==ARMOR_NONE)addstr(" while naked");
               addstr(".");
               if(encounter[t].type==CREATURE_LANDLORD&&
                  location[cursite]->renting==-1)
               {
                  move(14,1);
                  addstr("D - Rent a room");
                  if(a.armor.type==ARMOR_NONE)addstr(" while naked");
                  addstr(".");
               }
               break;
         }

         c=getch();
         translategetch(c);
      }

      if(c>='a'&&c<='g')
      {
         switch(talkmode)
         {
            case TALKMODE_ISSUES:
            {
               clearcommandarea();
               clearmessagearea();
               clearmaparea();

               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(9,1);
               addstr("What issue will ");
               addstr(a.name);
               addstr(" discuss");
               if(a.armor.type==ARMOR_NONE)addstr(" while naked");
               addstr("?");

               int lw=-1;
               char str[50];
               int x=1,y=11;
               for(int l2=0;l2<LAWNUM;l2++)
               {
                  if(law[l2]<2)set_color(COLOR_WHITE,COLOR_BLACK,1);
                  else set_color(COLOR_BLACK,COLOR_BLACK,1);
                  move(y,x);
                  addch(l2+'A');
                  addstr(" - ");
                  getlaw(str,l2);
                  addstr(str);

                  y++;
                  if(y==17)
                  {
                     y=11;
                     x+=30;
                  }
               }

               do
               {
                  refresh();
                  int c=getch();
                  translategetch(c);
                  if(c>='a'&&c<='z')
                  {
                     lw=c-'a';
                     if(lw<LAWNUM)
                     {
                        if(law[lw]<2)break;
                     }
                  }
                  if(c==10||c==27||c==32){lw=-1;break;}
               }while(1);

               if(lw!=-1)
               {
                  short aroll=LCSrandom(21)+a.attval(ATTRIBUTE_CHARISMA)+
                     a.attval(ATTRIBUTE_HEART)+LCSrandom(a.skill[SKILL_PERSUASION]*2+1);
                  char badthing=0;
                  if(aroll<15)badthing=1;
                  if(a.armor.type==ARMOR_NONE)aroll-=30;
                  short troll=LCSrandom(21)+tk->attval(ATTRIBUTE_CHARISMA)+
                     tk->attval(ATTRIBUTE_WISDOM);
                  a.skill_ip[SKILL_PERSUASION]+=LCSrandom(2)+1;

                  clearcommandarea();clearmessagearea();clearmaparea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(9,1);addstr(a.name);addstr(" says,");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  int y=10;
                  move(y,1);y++;
                  if(badthing)
                  {
                     switch(lw)
                     {
                        case LAW_ABORTION:
                           addstr("\"Conservatives make women turn to coat hangers.\"");
                           troll+=tk->skill[SKILL_LAW];
                           troll+=tk->skill[SKILL_RELIGION];
                           break;
                        case LAW_ANIMALRESEARCH:
                           addstr("\"Richard Gere put a gerbil in his butt!\"");
                           troll+=tk->skill[SKILL_SCIENCE];
                           break;
                        case LAW_POLICEBEHAVIOR:
                           addstr("\"The cops suck!\"");
                           troll+=tk->skill[SKILL_LAW];
                           troll-=tk->skill[SKILL_GANGSTERISM];
                           break;
                        case LAW_PRIVACY:
                           addstr("\"The government, like, knows things about you.\"");
                           troll+=tk->skill[SKILL_LAW];
                           break;
                        case LAW_DEATHPENALTY:
                           addstr("\"They executed this one dude, and like, his head caught on fire.\"");
                           troll+=tk->skill[SKILL_LAW];
                           break;
                        case LAW_NUCLEARPOWER:
                           addstr("\"Have you seen Godzilla?  Nuclear power is bad, yo.\"");
                           troll+=tk->skill[SKILL_SCIENCE];
                           break;
                        case LAW_POLLUTION:
                           addstr("\"You wanna look like the Toxic Avenger?  Oppose pollution!\"");
                           troll+=tk->skill[SKILL_SCIENCE];
                           break;
                        case LAW_LABOR:
                           addstr("\"Bad people wanna make babies work and stuff.\"");
                           troll+=tk->skill[SKILL_BUSINESS];
                           break;
                        case LAW_GAY:
                           addstr("\"Lots of people don't like homosexuals.\"");
                           troll+=tk->skill[SKILL_RELIGION];
                           break;
                        case LAW_CORPORATE:
                           addstr("\"The corporations are putting you down, dude.\"");
                           troll+=tk->skill[SKILL_BUSINESS];
                           break;
                        case LAW_FREESPEECH:
                           addstr("\"Better watch what you say.  They've got ears everywhere.\"");
                           troll+=tk->skill[SKILL_LAW];
                           break;
                        case LAW_FLAGBURNING:
                           addstr("\"The flag is stupid.\"");
                           troll+=tk->skill[SKILL_LAW];
                           break;
                        case LAW_TAX:
									addstr("\"Rich people, like, have money, man.\"");
                           troll+=tk->skill[SKILL_BUSINESS];
									break;
                        case LAW_GUNCONTROL:
                           addstr("\"People get shot, yo.\"");
                           troll+=tk->skill[SKILL_LAW];
                           troll+=tk->skill[SKILL_GANGSTERISM];
                           break;
                     }
                  }
                  else
                  {
                     switch(lw)
                     {
                        case LAW_ABORTION:
                           addstr("\"The government is systematically trying to rob women of the right");
                           move(y,1);y++;
                           addstr("to control their own destinies.\"");
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           if(tk->skill[SKILL_RELIGION])troll+=tk->skill[SKILL_RELIGION]-a.skill[SKILL_RELIGION];
                           break;
                        case LAW_ANIMALRESEARCH:
                           addstr("\"Animals are routinely subjected to inhumane treatment in labs in this");
                           move(y,1);y++;
                           addstr("country.\"");
                           if(tk->skill[SKILL_SCIENCE])troll+=tk->skill[SKILL_SCIENCE]-a.skill[SKILL_SCIENCE];
                           if(tk->skill[SKILL_RELIGION])troll+=tk->skill[SKILL_RELIGION]-a.skill[SKILL_RELIGION];
                           break;
                        case LAW_POLICEBEHAVIOR:
                           addstr("\"The police regularly torture minority suspects during interrogations.\"");
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           if(tk->skill[SKILL_GANGSTERISM])troll-=tk->skill[SKILL_GANGSTERISM]+a.skill[SKILL_GANGSTERISM];
                           break;
                        case LAW_PRIVACY:
                           addstr("\"Files are being kept on innocent citizens whose only crime is to");
                           move(y,1);y++;
                           addstr("speak out against a system that is trying to farm them like beasts.\"");
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           break;
                        case LAW_DEATHPENALTY:
                           addstr("\"Over thirty innocent people have been executed over the past decade.\"");
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           if(tk->skill[SKILL_RELIGION])troll-=tk->skill[SKILL_RELIGION]+a.skill[SKILL_RELIGION];
                           break;
                        case LAW_NUCLEARPOWER:
                           addstr("\"Radioactive waste is being stored all over the country, and it poses");
                           move(y,1);y++;
                           addstr("a serious threat to many families, even in this neighborhood.\"");
                           if(tk->skill[SKILL_SCIENCE])troll+=tk->skill[SKILL_SCIENCE]-a.skill[SKILL_SCIENCE];
                           break;
                        case LAW_POLLUTION:
                           addstr("\"Industries that stop at nothing to become more profitable are polluting");
                           move(y,1);y++;
                           addstr("the environment in ways that hurt not only animals, but people too.");
                           if(tk->skill[SKILL_SCIENCE])troll+=tk->skill[SKILL_SCIENCE]-a.skill[SKILL_SCIENCE];
                           if(tk->skill[SKILL_BUSINESS])troll+=tk->skill[SKILL_BUSINESS]-a.skill[SKILL_BUSINESS];
                           break;
                        case LAW_LABOR:
                           addstr("\"Have you noticed how people are working more and more hours for less and");
                           move(y,1);y++;
                           addstr("less money?  It's all part of a plan to keep you enslaved, man.\"");
                           if(tk->skill[SKILL_BUSINESS])troll+=tk->skill[SKILL_BUSINESS]-a.skill[SKILL_BUSINESS];
                           break;
                        case LAW_GAY:
                           addstr("\"Homosexuals are people like anyone else, and yet they are treated in this");
                           move(y,1);y++;
                           addstr("country as if they are deviants fit only for cheap entertainment.\"");
                           if(tk->skill[SKILL_RELIGION])troll+=tk->skill[SKILL_RELIGION]-a.skill[SKILL_RELIGION];
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           break;
                        case LAW_CORPORATE:
                           addstr("\"Corporate executives use giant corporations as a means to become parasites");
                           move(y,1);y++;
                           addstr("that suck wealth out of this country and put it into their pockets.\"");
                           if(tk->skill[SKILL_BUSINESS])troll+=tk->skill[SKILL_BUSINESS]-a.skill[SKILL_BUSINESS];
                           break;
                        case LAW_FREESPEECH:
                           addstr("\"Protests and demonstrations are regularly and often brutally suppressed in\"");
                           move(y,1);y++;
                           addstr("this country.  People have to watch what they write -- even what they read.\"");
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           break;
                        case LAW_FLAGBURNING:
                           addstr("\"Burning a piece of cloth is actually stigmatized in this country.");
                           move(y,1);y++;
                           addstr("You love freedom and still hate our government stands for.\"");
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           break;
                        case LAW_TAX:
									addstr("\"The tax code has been designed to perpetuate an unjust class");
									move(y,1);y++;
									addstr("structure that is keeping you oppressed.\"");
                           if(tk->skill[SKILL_BUSINESS])troll+=tk->skill[SKILL_BUSINESS]-a.skill[SKILL_BUSINESS];
									break;
                        case LAW_GUNCONTROL:
                           addstr("\"We live in such a backwards country right now that people think it's");
                           move(y,1);y++;
                           addstr("a right to walk around with the power to murder at any moment.\"");
                           if(tk->skill[SKILL_LAW])troll+=tk->skill[SKILL_LAW]-a.skill[SKILL_LAW];
                           if(tk->skill[SKILL_GANGSTERISM])troll+=tk->skill[SKILL_GANGSTERISM]-a.skill[SKILL_GANGSTERISM];
                           break;
                     }
                  }
                  refresh();
                  getch();

                  if(aroll>troll)
                  {
                     y++;
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(y,1);y++;addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(y,1);y++;
                     if(tk->type==CREATURE_MUTANT&&
                        tk->attval(ATTRIBUTE_INTELLIGENCE)<3)
                     {
                        addstr("\"Aaaahhh...\"");
                     }
                     else addstr("\"That *is* disturbing!   What can I do?\"");
                     refresh();
                     getch();

                     y++;move(y,1);y++;
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr("After more discussion, ");
                     addstr(tk->name);
                     addstr(" agrees to come by later tonight.");
                     refresh();
                     getch();

                     creaturest *newcr=new creaturest;
                        *newcr=*tk;
                     namecreature(*newcr);

                     newcr->location=a.location;
                     newcr->base=a.base;
                     newcr->hireid=a.id;
                     newcr->align=1;

                     pool.push_back(newcr);
                     stat_recruits++;

                     delenc(t,0);
                     return 1;
                  }
                  else
                  {
                     y++;
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(y,1);y++;addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(y,1);y++;
                     if(tk->type==CREATURE_MUTANT&&
                        tk->attval(ATTRIBUTE_INTELLIGENCE)<3)
                     {
                        addstr("\"Ugh.  Pfft.\"");
                     }
                     else addstr("\"Whatever.\"");
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr(" <turns away>");
                     refresh();
                     getch();

                     encounter[t].cantbluff=1;
                     return 1;
                  }
               }
               else
               {
                  clearcommandarea();clearmessagearea();clearmaparea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(9,1);addstr(a.name);addstr(" says,");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(10,1);addstr("\"Nevermind.\"");
                  refresh();
                  getch();

                  return 1;
               }
               break;
            }
            case TALKMODE_RENTING:
               if(c=='a')
               {
                  clearcommandarea();clearmessagearea();clearmaparea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(9,1);addstr(a.name);addstr(" says,");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(10,1);
                  addstr("\"I'll take it.\"");
                  refresh();
                  getch();

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(12,1);addstr(tk->name);addstr(" responds,");
                  set_color(COLOR_BLUE,COLOR_BLACK,1);
                  move(13,1);
                  addstr("\"Rent is due by the third of every month.");
                  move(14,1);
                  addstr("We'll start next month.\"");
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr(" <turns away>");
                  refresh();
                  getch();

                  long rent=200;
                  switch(location[cursite]->type)
                  {
                     case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
                     case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
                  }

                  funds-=rent;
                  stat_spent+=rent;
                  location[cursite]->renting=rent;
                  location[cursite]->newrental=1;
                  moneylost_rent+=rent;

                  basesquad(activesquad,cursite);
                  return 1;
               }
               if(c=='b')
               {
                  clearcommandarea();clearmessagearea();clearmaparea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(9,1);addstr(a.name);addstr(" says,");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(10,1);
                  addstr("\"Whoa, I was looking for something cheaper.\"");
                  refresh();
                  getch();

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(12,1);addstr(tk->name);addstr(" responds,");
                  set_color(COLOR_BLUE,COLOR_BLACK,1);
                  move(13,1);
                  addstr("\"Not my problem...\"");
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr(" <turns away>");
                  refresh();
                  getch();

                  return 1;
               }
               break;
            case TALKMODE_START:
               if(c=='a')
               {
                  clearcommandarea();clearmessagearea();clearmaparea();

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(9,1);addstr(a.name);addstr(" says,");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(10,1);
                  addstr("\"Do you want to hear something disturbing?\"");
                  refresh();
                  getch();

                  if(talkreceptive(*tk))
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(12,1);addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(13,1);
                     addstr("\"What?\"");
                     refresh();
                     getch();

                     talkmode=TALKMODE_ISSUES;
                     break;
                  }
                  else
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(12,1);addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(13,1);
                     addstr("\"No.\"");
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr(" <turns away>");
                     refresh();
                     getch();

                     return 1;
                  }
               }
               if(c=='b'&&!kid(*tk)&&!kid(a))
               {
                  int y=12;
                  clearcommandarea();clearmessagearea();clearmaparea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(9,1);addstr(a.name);addstr(" says,");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(10,1);
                  int line;
                  if(law[LAW_FREESPEECH]==-2)
                  {
                     line=LCSrandom(3);
                     switch(line)
                     {
                        case 0:addstr("\"[What church do you go to?]\"");break;
                        case 1:addstr("\"[Will you marry me?]\"");break;
                        case 2:addstr("\"[Do you believe in abstinence education?]\"");break;
                     }
                  }
                  else
                  {
                     line=LCSrandom(44);
                  switch(line)
                  {
//LIMIT             :-----------------------------------------------------------------------------:
case 0:addstr("\"Hey baby, you're kinda ugly.  I like that.\"");break;
case 1:addstr("\"I lost my phone number.  Could I have yours?\"");break;
case 2:addstr("\"Hey, you wanna go rub one off?\"");break;
case 3:addstr("\"Hot damn.  You're built like a brick shithouse, honey.\"");break;
case 4:
       addstr("\"I know I've seen you on the back of a milk carton,");
       move(11,1);y++;
       addstr("cuz you've been missing from my life.\"");
       break;
case 5:addstr("\"I'm big where it counts.\"");break;
case 6:addstr("\"Daaaaaamn girl, I want to wrap your legs around my face and");
       move(11,1);y++;
       addstr("wear you like a feed bag!\"");
       break;
case 7:addstr("\"Let's play squirrel.  I'll bust a nut in your hole.\"");break;
case 8:addstr("\"You know, if I were you, I'd have sex with me.\"");break;
case 9:addstr("\"You don't sweat much for a fat chick.\"");break;
case 10:addstr("\"Fuck me if I'm wrong but you want to kiss me, right?\"");break;
case 11:addstr("\"Your parents must be retarded, because you are special.\"");break;
case 12:addstr("\"Let's play trains...  you can sit on my face and I will chew chew chew.\"");break;
case 13:addstr("\"Is it hot in here or is it just you?\"");break;
case 14:addstr("\"I may not be Fred Flintstone, but I can make your bed rock!\"");break;
case 15:addstr("\"What do you say we go behind a rock and get a little boulder?\"");break;
case 16:addstr("\"Do you have stars on your panties?  Your ass is outta this world!\"");break;
case 17:addstr("\"Those pants would look great on the floor of my bedroom.\"");break;
case 18:addstr("\"If I said you had a nice body, would you hold it against me?\"");break;
case 19:addstr("\"Are you tired?  You've been running around in my thoughts all day.\"");break;
case 20:addstr("\"If I could change the alphabet baby, I would put the U and I together!\"");break;
case 21:addstr("\"Your lips look sweet.  Can I taste them?\"");break;
case 22:addstr("\"Nice shoes.  Wanna fuck?\"");break;
case 23:addstr("\"Your sexuality makes me nervous and this frustrates me.\"");break;
case 24:addstr("\"Are you Jamaican?  Cuz Jamaican me horny.\"");break;
case 25:addstr("\"Hey pop tart, fancy coming in my toaster of love?\"");break;
case 26:addstr("\"Wanna play army?  You lie down and I'll blow you away.\"");break;
case 27:addstr("\"Can I lick your forehead?\"");break;
case 28:addstr("\"I have a genital rash.  Will you rub this ointment on me?\"");break;
case 29:addstr("\"What's your sign?\"");break;
case 30:addstr("\"Do you work for the post office?");
      move(11,1);y++;
      addstr("Because I could have sworn you were checking out my package.\"");
      break;
case 31:addstr("\"I'm not the most attractive person in here,");
      move(11,1);y++;
      addstr("but I'm the only one talking to you.\"");
      break;
case 32:addstr("\"Hi.  I suffer from amnesia.  Do I come here often?\"");break;
case 33:addstr("\"I'm new in town.  Could you give me directions to your apartment?\"");break;
case 34:addstr("\"Stand still so I can pick you up!\"");break;
case 35:addstr("\"Your daddy must have been a baker, cuz you've got a nice set of buns.\"");break;
case 36:addstr("\"If you were a laser, you'd be set on 'stunning'.\"");break;
case 37:addstr("\"Is that a keg in your pants?  Cuz I'd love to tap that ass.\"");break;
case 38:addstr("\"If I could be anything, I'd love to be your bathwater.\"");break;
case 39:addstr("\"Stop, drop and roll, baby.  You are on fire.\"");break;
case 40:addstr("\"Do you want to see something swell?\"");break;
case 41:addstr("\"Excuse me.  Do you want to fuck or should I apologize?\"");break;
case 42:addstr("\"Say, did we go to different schools together?\"");break;
case 43:addstr("\"You smell...  Let's go take a shower.\"");break;
                  }
                  }
                  refresh();
                  getch();

                  short aroll=LCSrandom(21)+a.attval(ATTRIBUTE_CHARISMA)*2+LCSrandom(a.skill[SKILL_SEDUCTION]*2+1);
                  if(a.armor.type==ARMOR_NONE)aroll-=30;
                  short troll=LCSrandom(21)+tk->attval(ATTRIBUTE_CHARISMA)+tk->attval(ATTRIBUTE_WISDOM);
                  a.skill_ip[SKILL_SEDUCTION]+=LCSrandom(5)+2;

                  if(aroll>troll)
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(y,1);y++;addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(y,1);y++;

                     if(law[LAW_FREESPEECH]==-2)
                     {
                     switch(line)
                     {
                        case 0:addstr("\"[I go to your church.]\"");break;
                        case 1:addstr("\"[Yes.]\"");break;
                        case 2:addstr("\"[Yes.  Yes, I do.]\"");break;
                     }
                     }
                     else
                  {
                  switch(line)
                  {
//LIMIT             :-----------------------------------------------------------------------------:
case 0:addstr("\"You're not so cute yourself.  Wanna get a room?\"");break;
case 1:addstr("\"How sweet!  You can call me tonight...\"");break;
case 2:addstr("\"You bet, baby.\"");break;
case 3:addstr("\"He he, I'll let that one slide.  Besides, I like country folk...\"");break;
case 4:addstr("\"That's sick.  I can do sick tonight.\"");break;
case 5:addstr("\"Oooo, let me see!\"");break;
case 6:addstr("\"Wow, looks like I'm going to have to reward creativity tonight!\"");break;
case 7:addstr("\"Winter's coming.  You'd better bust more than one.\"");break;
case 8:addstr("\"But you're not, so the pleasure's all mine.\"");break;
case 9:addstr("\"Just wait until tonight, baby.\"");break;
case 10:addstr("\"You're wrong.\"");break;
case 11:addstr("\"I can drool on you if you like it that way.\"");break;
case 12:addstr("\"Oooo, all aboard baby!\"");break;
case 13:addstr("\"Not as hot as we'll be tonight you slut.\"");break;
case 14:addstr("\"Goober.  You wanna hook up tonight?\"");break;
case 15:addstr("\"Oooo, we should get stoned too!  He he.\"");break;
case 16:addstr("\"You'll have to whip out your rocket to get some.  Let's do it.\"");break;
case 17:addstr("\"So would my underwear.\"");break;
case 18:addstr("\"Yeah, and you're going to repay me tonight.\"");break;
case 19:addstr("\"Then stop *thinking* about it and come over tonight.\"");break;
case 20:addstr("\"As long as you put a condom between them, I'm all for it.\"");break;
case 21:addstr("\"Sure, but you can't use your mouth.\"");break;
case 22:addstr("\"I hope you don't have a foot fetish, but I'm game.\"");break;
case 23:addstr("\"My sex could do even more.\"");break;
case 24:addstr("\"Let me invite you to visit my island paradise.  Tonight.\"");break;
case 25:addstr("\"Oh, man...  just don't tell anybody I'm seeing you.\"");break;
case 26:addstr("\"I hope we're shooting blanks, soldier.  I'm out of condoms.\"");break;
case 27:addstr("\"You can lick all my decals off, baby.\"");break;
case 28:addstr("\"Only if I'm not allowed to use my hands.\"");break;
case 29:addstr("\"The one that says 'Open All Night'.\"");break;
case 30:addstr("\"It looks like a letter bomb to me.  Let me blow it up.\"");break;
case 31:addstr("\"Hey, I could do better.  But I'm feeling cheap tonight.\"");break;
case 32:addstr("\"Yeah.  I hope you remember the lube this time.\"");break;
case 33:addstr("\"But if we use a hotel, you won't get shot by an angry spouse tonight.\"");break;
case 34:addstr("\"I think you'll appreciate the way I move after tonight.\"");break;
case 35:addstr("\"They make a yummy bedtime snack.\"");break;
case 36:addstr("\"Oh..  oh, God.  I can't believe I'm going to date a Trekkie.\"");break;
case 37:addstr("\"Oh, it isn't safe for you to drive like that.  You'd better stay the night.\"");break;
case 38:addstr("\"Come over tonight and I can show you what it's like.\"");break;
case 39:addstr("\"I'll stop, drop and roll if you do it with me.\"");break;
case 40:addstr("\"I'd rather feel something swell.\"");break;
case 41:addstr("\"You can apologize later if it isn't any good.\"");break;
case 42:addstr("\"Yeah, and we tonight can try different positions together.\"");break;
case 43:addstr("\"Don't you like it dirty?\"");break;
                  }
                  }
                     refresh();
                     getch();

                     y++;
                     move(y,1);
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(y,1);y++;
                     addstr(a.name);
                     addstr(" and ");
                     addstr(tk->name);
                     addstr(" make plans for tonight");
                     if(tk->type==CREATURE_PRISONER)
                     {
                        addstr(", and ");
                        move(y,1);y++;
                        addstr(tk->name);
                        addstr(" breaks for the exit");
                        tk->lawflag[LAWFLAG_ESCAPED]++;
                     }
                     addstr(".  ");
                     refresh();
                     getch();

                     int olddate=0;
                     datest *newd=NULL;
                     for(int d=0;d<date.size();d++)
                     {
                        if(date[d]->mac_id==a.id)
                        {
                           newd=date[d];
                           break;
                        }
                     }

                     if(newd==NULL)
                     {
                        newd=new datest;
                           newd->mac_id=a.id;
                        date.push_back(newd);
                     }

                     creaturest *newcr=new creaturest;
                     *newcr=*tk;

                     newcr->location=a.location;
                     newcr->base=a.base;

                     newd->date.push_back(newcr);

                     delenc(t,0);
                  }
                  else
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(y,1);y++;addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(y,1);y++;
                     addstr("\"Jesus...\"");
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr(" <turns away>");
                     refresh();
                     getch();

                     encounter[t].cantbluff=1;
                  }

                  return 1;
               }
               if(c=='c')return 0;
               if(encounter[t].type==CREATURE_LANDLORD&&
                  location[cursite]->renting==-1)
               {
                  if(c=='d')
                  {
                     clearcommandarea();clearmessagearea();clearmaparea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(9,1);addstr(a.name);addstr(" says,");
                     set_color(COLOR_GREEN,COLOR_BLACK,1);
                     move(10,1);
                     addstr("\"I'd like to rent a room.\"");
                     refresh();
                     getch();

                     if(a.armor.type==ARMOR_NONE)
                     {
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(12,1);addstr(tk->name);addstr(" responds,");
                        set_color(COLOR_BLUE,COLOR_BLACK,1);
                        move(13,1);
                        addstr("\"Put some clothes on before I call the cops.\"");
                        refresh();
                        getch();
                        return 1;
                     }

                     long rent=200;
                     switch(location[cursite]->type)
                     {
                        case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
                        case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
                     }

                     char num[20];
                     itoa(rent,num,10);

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(12,1);addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(13,1);
                     addstr("\"It'll be $");
                     addstr(num);
                     addstr(" a month.");
                     move(14,1);
                     addstr("I'll need $");
                     addstr(num);
                     addstr(" now as a security deposit.\"");
                     refresh();
                     getch();

                     if(funds>=rent)
                     {
                        talkmode=TALKMODE_RENTING;
                     }
                     else
                     {
                        clearcommandarea();clearmessagearea();clearmaparea();
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(9,1);addstr(a.name);addstr(" says,");
                        set_color(COLOR_GREEN,COLOR_BLACK,1);
                        move(10,1);
                        addstr("\"Whoa, I was looking for something cheaper.\"");
                        refresh();
                        getch();

                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(12,1);addstr(tk->name);addstr(" responds,");
                        set_color(COLOR_BLUE,COLOR_BLACK,1);
                        move(13,1);
                        addstr("\"Not my problem...\"");
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        addstr(" <turns away>");
                        refresh();
                        getch();
                     }
                  }
               }
               break;
         }
      }
   }while(1);
}



/* are they interested in talking about the issues? */
char talkreceptive(creaturest &cr)
{
   if(cr.align==-1)return 0;

   switch(cr.type)
   {
      case CREATURE_WORKER_SERVANT:
      case CREATURE_WORKER_JANITOR:
      case CREATURE_WORKER_SWEATSHOP:
      case CREATURE_WORKER_FACTORY_CHILD:
      case CREATURE_TEENAGER:
      case CREATURE_SEWERWORKER:
      case CREATURE_COLLEGESTUDENT:
      case CREATURE_MUSICIAN:
      case CREATURE_MATHEMATICIAN:
      case CREATURE_TEACHER:
      case CREATURE_HSDROPOUT:
      case CREATURE_BUM:
      case CREATURE_GANGMEMBER:
      case CREATURE_CRACKHEAD:
      case CREATURE_FASTFOODWORKER:
      case CREATURE_TELEMARKETER:
      case CREATURE_PROSTITUTE:
      case CREATURE_GARBAGEMAN:
      case CREATURE_PLUMBER:
      case CREATURE_AMATEURMAGICIAN:
      case CREATURE_HIPPIE:
      case CREATURE_RETIREE:
      case CREATURE_HAIRSTYLIST:
      case CREATURE_CLERK:
      case CREATURE_MUTANT:
         return 1;
   }

   return 0;
}



/* is the character too young to be dating? */
char kid(creaturest &cr)
{
   switch(cr.type)
   {
      case CREATURE_WORKER_FACTORY_CHILD:
         return 1;
   }

   return 0;
}
