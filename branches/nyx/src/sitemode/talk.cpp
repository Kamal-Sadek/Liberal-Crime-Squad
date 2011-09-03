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


/* bluff, date, issues */
char talk(Creature &a,int t)
{
   //BLUFFING
   if((sitealarm||location[cursite]->siege.siege)&&encounter[t].enemy())
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
      int hostages=0;
      int weaponhostage=0;
      bool cop=0;

      for(int i=0;i<6;i++)
      {
         if(activesquad->squad[i]&&
            activesquad->squad[i]->prisoner&&
            activesquad->squad[i]->prisoner->alive&&
            activesquad->squad[i]->prisoner->enemy())
         {
            hostages++;
            if (activesquad->squad[i]->get_weapon().can_threaten_hostages())
              weaponhostage++;
         }
      }
      if(encounter[t].type==CREATURE_COP||
         encounter[t].type==CREATURE_GANGUNIT||
         encounter[t].type==CREATURE_SWAT ||
         encounter[t].type==CREATURE_SECURITYGUARD ||
         encounter[t].type==CREATURE_MILITARYPOLICE)
      {
         cop=1;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(11,1);
      addstr("A - Intimidate");
      if(!hostages)set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,1);
      addstr("B - Threaten hostages");
      if(encounter[t].cantbluff!=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(13,1);
      addstr("C - Bluff");
      if(cop)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("D - Surrender to authorities");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      refresh();
      while(1)
      {
         c=getch();
         translategetch(c);
         if(c=='a')break;
         if(c=='b' && hostages)break;
         if(c=='c' && encounter[t].cantbluff!=2)break;
         if(c=='d' && cop)break;
      }

      if(c=='a')
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(a.name);
         addstr(":");
         move(17,1);
         set_color(COLOR_GREEN,COLOR_BLACK,1);

         switch(LCSrandom(3))
         {
         case 0:
            {   // Formatting the slogan so that it always has quotes around it and punctuation
               if(slogan[0]!='"')addch('"');
               addstr(slogan);
               int last=strlen(slogan);
               if(last && slogan[last-1]!='"' && slogan[last-1]!='!' && slogan[last-1]!='.' && slogan[last-1]!='?')
               {
                  addch('!');
               }
               if(last && slogan[last-1]!='"')addstr("\"");

               if(!sitestory->claimed)
                  sitestory->claimed=1;
               break;
            }
         case 1:
            addstr("Die you Conservative dogs!");
            break;
         case 2:
            addstr("We're the Liberal Crime Squad!");
            break;
         case 3:
            addstr("Praying won't help you now!");
            break;
         }
         
         refresh();
         getch();
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         
         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].exists&&encounter[e].alive&&encounter[e].enemy())
            {
               int attack = a.juice / 50 + attitude[VIEW_LIBERALCRIMESQUAD] / 10;
               int defense = encounter[e].attribute_roll(ATTRIBUTE_WISDOM);

               if(attack > defense)
               {
                  if(encounter[e].type==CREATURE_COP||
                     encounter[e].type==CREATURE_GANGUNIT||
                     encounter[e].type==CREATURE_SWAT||
                     encounter[e].type==CREATURE_DEATHSQUAD||
                     encounter[e].type==CREATURE_SOLDIER||
                     encounter[e].type==CREATURE_HARDENED_VETERAN||
                     encounter[e].type==CREATURE_CCS_ARCHCONSERVATIVE||
                     encounter[e].type==CREATURE_AGENT)
                  {
                     if(LCSrandom(3))
                     {
                        continue;
                     }
                  }
                  clearmessagearea();
                  move(16,1);
                  addstr(encounter[e].name);
                  switch(LCSrandom(6))
                  {
                  case 0:addstr(" chickens out!");break;
                  case 1:addstr(" backs off!");break;
                  case 2:addstr(" doesn't want to die!");break;
                  case 3:addstr(" is out of there!");break;
                  case 4:addstr(" has a family!");break;
                  case 5:addstr(" is too young to die!");break;
                  }
                  delenc(e,0);
                  addjuice(a,2,200); // Instant juice!
                  refresh();
                  getch();
               }
            }
         }
      }
      else if(c=='b')
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(16,1);
         addstr(a.name);
         addstr(":");
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(17,1);
         switch(LCSrandom(6))
         {
         case 0:addstr("\"Back off or the hostage dies!\"");break;
         case 1:addstr("\"Don't push the LCS!\"");
            if(!sitestory->claimed)sitestory->claimed=1;break;
         case 2:addstr("\"Hostage says you better leave!\"");break;
         case 3:addstr("\"I'll do it! I'll kill this one!\"");break;
         case 4:addstr("\"You gonna tell the family you pushed me?!\"");break;
         case 5:
                if(law[LAW_FREESPEECH]==-2)addstr("\"Don't [play] with me!\"");
                else addstr("\"Don't fuck with me!\"");
                break;
         }

         sitecrime+=5;
         criminalizeparty(LAWFLAG_KIDNAPPING);

         addjuice(a,-2,-10); // DE-juice for this shit

         
         refresh();
         getch();

         bool noretreat=false;

         if(weaponhostage)
         {
            int e;
            for(e=0;e<ENCMAX;e++)
            {
               if(encounter[e].exists&&encounter[e].alive&&
                  encounter[e].enemy()&&encounter[e].blood>70)
               {
                  if((encounter[e].type==CREATURE_DEATHSQUAD||
                     encounter[e].type==CREATURE_SOLDIER||
                     encounter[e].type==CREATURE_HARDENED_VETERAN||
                     encounter[e].type==CREATURE_CCS_ARCHCONSERVATIVE||
                     encounter[e].type==CREATURE_AGENT||
                     encounter[e].type==CREATURE_MERC||
                     encounter[e].type==CREATURE_COP||
                     encounter[e].type==CREATURE_GANGUNIT||
                     encounter[e].type==CREATURE_SWAT)&&
                     LCSrandom(5))
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     clearmessagearea();
                     move(16,1);
                     addstr(encounter[e].name);
                     addstr(":");
                     move(17,1);

                     if(encounter[e].align!=ALIGN_CONSERVATIVE)
                     {
                        set_color(COLOR_GREEN,COLOR_BLACK,1);
                        switch(LCSrandom(5))
                        {
                        case 0:addstr("\"Let them go. Think about what you're doing.\"");break;
                        case 1:addstr("\"Calm down, and let's talk about this.\"");break;
                        case 2:addstr("\"Wait! We can work this out.\"");break;
                        case 3:addstr("\"This isn't right, think about it.\"");break;
                        case 4:addstr("\"Slow down. We can work this out.\"");break;
                        }
                     }
                     else
                     {
                        set_color(COLOR_RED,COLOR_BLACK,1);
                        if(((encounter[e].type==CREATURE_DEATHSQUAD||
                           encounter[e].type==CREATURE_AGENT||
                           encounter[e].type==CREATURE_MERC||
                           encounter[e].type==CREATURE_GANGUNIT))
                           &&encounter[e].align==ALIGN_CONSERVATIVE)
                        {
                           switch(LCSrandom(5))
                           {
                           case 0:addstr("\"Hahahaha...\"");break;
                           case 1:addstr("\"You think you can scare me?\"");break;
                           case 2:addstr("\"You're not getting out of here alive.\"");break;
                           case 3:addstr("\"What's wrong?  Need your diaper changed?\"");break;
                           case 4:addstr("\"Three... two...\"");break;
                           }
                        }
                        else
                        {
                           switch(LCSrandom(5))
                           {
                           case 0:
                              if(hostages>1)
                                 addstr("\"Release your hostages, and nobody gets hurt.\"");
                              else
                                 addstr("\"Let the hostage go, and nobody gets hurt.\"");
                              break;
                           case 1:addstr("\"You got about five seconds to back down.\"");break;
                           case 2:addstr("\"You want to do this the hard way?\"");break;
                           case 3:addstr("\"Big mistake.\"");break;
                           case 4:addstr("\"Release them, and I'll let you go.\"");break;
                           }
                        }

                     }

                     refresh();
                     getch();

                     noretreat=true;
                     break;
                  }
               }
            }
            if(noretreat==false)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               clearmessagearea();
               move(16,1);
               addstr("The ploy works! The Conservatives back off.");
               for(int i=ENCMAX;i>=0;i--)
               {
                  if(encounter[i].exists&&
                     encounter[i].alive&&
                     encounter[i].align<=-1)
                  {
                     delenc(i,0);
                  }
               }
               refresh();
               getch();
            }
            else
            {
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               clearcommandarea();
               clearmessagearea();
               clearmaparea();
               move(9,1);
               addstr("How should ");
               addstr(a.name);
               addstr(" respond?");
               move(11,1);
               if(hostages>1)
                  addstr("A - Execute a hostage");
               else
                  addstr("A - Execute the hostage");
               move(12,1);
               if(hostages>1)
                  addstr("B - Offer to trade the hostages for freedom");
               else
                  addstr("B - Offer to trade the hostage for freedom");
               move(13,1);
               addstr("C - No reply");

               while(1)
               {
                  c=getch();
                  translategetch(c);
                  if(c=='a'||c=='b'||c=='b')break;
               }
               if(c=='a')
               {
                  Creature* executer=0;
                  if(a.prisoner)
                  {
                     executer=&a;
                  }
                  else for(int i=0;i<6;i++)
                  {
                     if(activesquad->squad[i] &&
                        activesquad->squad[i]->prisoner &&
                        activesquad->squad[i]->prisoner->alive &&
                        activesquad->squad[i]->prisoner->enemy())
                     {
                        executer=activesquad->squad[i];
                        break;
                     }
                  }
                  
                  move(16,1);
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  if(executer->get_weapon().is_ranged()
                     && executer->get_weapon().get_ammoamount()>0)
                  {
                     addstr("BLAM!");
                     executer->get_weapon().decrease_ammo(1); //What if it doesn't use ammo? -XML
                  }
                  else
                  {
                     addstr("CRUNCH!");
                  }
                  refresh();
                  getch();
                  move(17,1);
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr(executer->name);
                  addstr(" drops ");
                  addstr(executer->prisoner->name);
                  addstr("'s body.");

                  addjuice(*executer,-5,-50); // DE-juice for this shit
                  sitecrime+=10;
                  sitestory->crime.push_back(CRIME_KILLEDSOMEBODY);
                  criminalize(*executer,LAWFLAG_MURDER);

                  if(executer->prisoner->type==CREATURE_CORPORATE_CEO||
                     executer->prisoner->type==CREATURE_RADIOPERSONALITY||
                     executer->prisoner->type==CREATURE_NEWSANCHOR||
                     executer->prisoner->type==CREATURE_SCIENTIST_EMINENT||
                     executer->prisoner->type==CREATURE_JUDGE_CONSERVATIVE)sitecrime+=30;

                  makeloot(*executer->prisoner,groundloot);

                  refresh();
                  getch();

                  delete executer->prisoner;
                  executer->prisoner=NULL;

                  if(hostages>1&&LCSrandom(2))
                  {
                     clearmessagearea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr(encounter[e].name);
                     addstr(":");
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     move(17,1);
                     if(law[LAW_FREESPEECH]>ALIGN_ARCHCONSERVATIVE)
                        addstr("\"Fuck! \"");
                     else
                        addstr("\"[No!] \"");
                     switch(LCSrandom(5))
                     {
                     case 0:addstr("Okay, okay, you win!\"");break;
                     case 1:addstr("Don't shoot!\"");break;
                     case 2:addstr("Do you even care?!\"");break;
                     case 3:addstr("Heartless!\"");break;
                     case 4:addstr("It's not worth it!\"");break;
                     }

                     for(int i=ENCMAX;i>=0;i--)
                     {
                        if(encounter[i].exists && encounter[i].enemy() && encounter[i].alive)
                        {
                           delenc(i,0);
                        }
                     }

                     refresh();
                     getch();
                  }
               }
               else if(c=='b')
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(16,1);
                  addstr(a.name);
                  addstr(":");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(17,1);
                  switch(LCSrandom(5))
                  {
                  case 0:
                     if(hostages>1)
                        addstr("\"Back off and we'll let the hostages go.\"");
                     else
                        addstr("\"Back off and the hostage goes free.\"");
                     break;
                  case 1:addstr("\"Freedom for freedom, understand?\"");break;
                  case 2:addstr("\"Let me go in peace, okay?\"");break;
                  case 3:addstr("\"Let's make a trade, then.\"");break;
                  case 4:addstr("\"I just want out of here, yeah?\"");break;
                  }

                  refresh();
                  getch();

                  if(((encounter[e].type==CREATURE_DEATHSQUAD||
                     encounter[e].type==CREATURE_AGENT||
                     encounter[e].type==CREATURE_MERC||
                     encounter[e].type==CREATURE_GANGUNIT)&&
                     LCSrandom(2))&&encounter[e].align==ALIGN_CONSERVATIVE)
                  {
                     clearmessagearea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr(encounter[e].name);
                     addstr(":");
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     move(17,1);
                     switch(LCSrandom(5))
                     {
                     case 0:addstr("\"Do I look like a loving person?\"");break;
                     case 1:addstr("\"You don't take a hint, do you?\"");break;
                     case 2:addstr("\"I'm doing the world a favor.\"");break;
                     case 3:addstr("\"That's so pathetic...\"");break;
                     case 4:addstr("\"It's a deal.\"");break;
                     }

                     refresh();
                     getch();
                  }
                  else
                  {
                     clearmessagearea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     addstr(encounter[e].name);
                     addstr(":");
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     move(17,1);
                     switch(LCSrandom(4))
                     {
                     case 0:addstr("\"Right. Let's do it.\"");break;
                     case 1:addstr("\"No further conditions.\"");break;
                     case 2:addstr("\"Let them go, and we're done.\"");break;
                     case 3:addstr("\"No tricks, okay?\"");break;
                     }
                     refresh();
                     getch();
                     for(int i=ENCMAX;i>=0;i--)
                     {
                        if(encounter[i].exists &&
                           encounter[i].enemy() &&
                           encounter[i].alive)
                        {
                           delenc(i,0);
                        }
                     }
                     clearmessagearea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(16,1);
                     juiceparty(15,200); // Instant juice for successful hostage negotiation
                     if(hostages>1)addstr("The squad releases all hostages in the trade.");
                     else addstr("The squad releases the hostage in the trade.");
                     for(int i=0;i<6;i++)
                     {
                        if(activesquad->squad[i] &&
                           activesquad->squad[i]->prisoner &&
                           activesquad->squad[i]->prisoner->enemy())
                        {
                           delete activesquad->squad[i]->prisoner;
                           activesquad->squad[i]->prisoner=NULL;
                        }
                     }
                     refresh();
                     getch();
                  }
               }
            }
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            clearmessagearea();
            move(16,1);
            addstr(encounter[t].name);
            addstr(" isn't interested in your pathetic threats.");
            refresh();
            getch();
         }
      }
      else if(c=='c')
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
               case SIEGE_CCS:
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
               case SIEGE_FIREMEN:
                  addstr("lights a match and throws it on the ground.");
                  if((!(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_END) ||
                     !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK) ||
                     !(levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_START) ||
                     !(levelmap[locx][locy][locz].flag & SITEBLOCK_DEBRIS)) && !LCSrandom(10))
                  {
                     levelmap[locx][locy][locz].flag |= SITEBLOCK_FIRE_START;
                     move(17,1);
                     addstr("The carpet smolders, then bursts into flame.");
                     move(18,1);
                     addstr("Perhaps that was a bad idea...");
                  }
                  break;
            }
         }
         else        //Special bluff messages for various uniforms
         {
            set_color(COLOR_GREEN, COLOR_BLACK, 1);
            if(a.get_armor().get_itemtypename() == "ARMOR_POLICEUNIFORM" ||
               a.get_armor().get_itemtypename() == "ARMOR_POLICEARMOR" ||
               a.get_armor().get_itemtypename() == "SWATARMOR")
            {
               addstr("\"The situation is under control.\"");
            }
            else if (a.get_armor().get_itemtypename() == "ARMOR_BUNKERGEAR")
            {
               if(siteonfire) addstr("\"Fire! Evacuate immediately!\"");
               else addstr("\"Everything's in check.\"");
            }
               
            else if(a.get_armor().get_itemtypename() == "ARMOR_LABCOAT")
               addstr("\"Make way, I'm a doctor!\"");

            else if(a.get_armor().get_itemtypename() == "ARMOR_DEATHSQUADUNIFORM")
               addstr("\"Non-targets please leave the site.\"");

            else if(a.get_armor().get_itemtypename() == "ARMOR_MITHRIL")
            {
               addstr(a.name);
               addstr(" engraves ");
               set_color(COLOR_CYAN, COLOR_BLACK, 1);
               addstr("Elbereth");     //Fanciful multicolor message
               set_color(COLOR_GREEN, COLOR_BLACK, 1);
               addstr(" on the floor.");
            }
            else
            {
               addstr(" talks like a Conservative");
               move(17,1);
               addstr("and pretends to belong here.");
            }
         }

         refresh();
         getch();

         bool fooled=true;
         int e;
         
         for(e=0;e<ENCMAX;e++)
         {
            if(encounter[e].exists&&encounter[e].alive&&
               encounter[e].enemy())
            {
               if(encounter[e].get_attribute(ATTRIBUTE_WISDOM,true)>10)
                  fooled = a.skill_check(SKILL_DISGUISE,DIFFICULTY_CHALLENGING);
               else
                  fooled = a.skill_check(SKILL_DISGUISE,DIFFICULTY_AVERAGE);

               if(fooled == false)break;
            }
         }

         a.train(SKILL_DISGUISE,20);

         if(!fooled)
         {
            clearmessagearea();

            set_color(COLOR_RED,COLOR_BLACK,1);
            move(16,1);
            if(encounter[e].type==CREATURE_HICK)
            {
               addstr("But ");
               addstr(encounter[e].name);
               addstr(" weren't born yesterday.");
            }
            else
            {
               addstr(encounter[e].name);
               if(law[LAW_FREESPEECH]==ALIGN_ARCHCONSERVATIVE)
                  addstr(" is not fooled by that [act].");
               else addstr(" is not fooled by that crap.");
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

            refresh();
            getch();
            
            for(int e=ENCMAX-1;e>=0;e--)
            {
               if(encounter[e].exists&&encounter[e].alive&&
                  encounter[e].enemy())
               {
                  delenc(e,0);
               }
            }
         }
      }
      else
      {
         move(14,1);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("The Squad is arrested.");
         getch();

         int stolen=0;
         // Police assess stolen goods in inventory
         for(int l=0;l<activesquad->loot.size();l++)
         {
            if(activesquad->loot[l]->is_loot())
               stolen++;
         }
            
         for(int i=0;i<6;++i)
         {
            if(activesquad->squad[i])
            {
               activesquad->squad[i]->crimes_suspected[LAWFLAG_THEFT]+=stolen;
               capturecreature(*activesquad->squad[i]);
            }
            activesquad->squad[i]=NULL;
         }
         location[cursite]->siege.siege=0;
      }
      return 1;
   }
   //TALKING
   short talkmode=TALKMODE_START;

   Creature *tk=&encounter[t];

   // TALKING TO DOGS
   if(tk->type == CREATURE_GUARDDOG && tk->align != ALIGN_LIBERAL)
   {
      // Find most Heartful Liberal
      int bestp=0;
      for(int p=0; p<6; p++)
      {
         if(activesquad->squad[p] &&
            activesquad->squad[p]->get_attribute(ATTRIBUTE_HEART, true) >
            activesquad->squad[bestp]->get_attribute(ATTRIBUTE_HEART, true))
         {
            bestp = p;
         }
      }
      
      clearcommandarea();
      clearmessagearea();
      clearmaparea();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(10,1);
      addstr(pool[bestp]->name);
      addstr(" says, ");
      move(11,1);
      set_color(COLOR_GREEN,COLOR_BLACK,1);
      
      // Say something unbelievably hippie
      int statement = LCSrandom(10);
      if(activesquad->squad[bestp]->get_attribute(ATTRIBUTE_HEART, true) >= 20)
      {
         switch(statement)
         {
         case 0: addstr("\"I love dogs more than people.\""); break;
         case 1: addstr("\"Dogs are the future of humanity.\""); break;
         case 2: addstr("\"Power to the canines!\""); break;
         case 3: addstr("\"We need to recruit more dogs.\""); break;
         case 4: addstr("\"Wanna join the LCS?\""); break;
         case 5: addstr("\"Want me to untie you?\""); break;
         case 6: addstr("\"You deserve better than this.\""); break;
         case 7: addstr("\"Dogs are the best anything ever.\""); break;
         case 8: addstr("\"We should bark together!\""); break;
         case 9: addstr("\"All we are saying is give fleas a chance.\""); break;
         case 10: addstr("\"Dogs are better than humans.\""); break;
         }
      }
      else // or not
      {
         switch(statement)
         {
         case 0: addstr("\"Hi Mister Dog!\""); break;
         case 1: addstr("\"Good dog!\""); break;
         case 2: addstr("\"Hey there, boy.\""); break;
         case 3: addstr("\"Woof...?\""); break;
         case 4: addstr("\"Bark at the man for me!\""); break;
         case 5: addstr("\"Down, boy!\""); break;
         case 6: addstr("\"Don't bite me!\""); break;
         case 7: addstr("\"Hi doggy!\""); break;
         case 8: addstr("\"Hi, puppy.\""); break;
         case 9: addstr("\"OH MAN I LOVE DOGS!\""); break;
         case 10: addstr("\"Bark! Bark!\""); break;
         }
      }
      
      refresh();
      getch();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(13,1);addstr(tk->name);

      if(activesquad->squad[bestp]->get_attribute(ATTRIBUTE_HEART, true) >= 20)
      {
         addstr(" says, ");
         move(14,1);
         
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         switch(statement)
         {
         case 0: addstr("\"A human after my own heart, in more ways than one.\""); break;
         case 1: addstr("\"I don't see it, but I'll hear you out.\""); break;
         case 2: addstr("\"Down with the feline establishment!\""); break;
         case 3: addstr("\"Oh yeah? I'm a dog. What do you represent?\""); break;
         case 4: addstr("\"Maybe. Is there a dental plan?\""); break;
         case 5: addstr("\"Yes, please! This collar is painful!\""); break;
         case 6: addstr("\"Finally, a human that understands.\""); break;
         case 7: addstr("\"Heheheh, you're funny. Okay, I won't rat you out.\""); break;
         case 8: addstr("\"Barkety bark, baby. Tell me more.\""); break;
         case 9: addstr("\"We'll fight the fleas until our dying itch.\""); break;
         case 10: addstr("\"You're pandering, but I love it.\""); break;
         }

         tk->align = ALIGN_LIBERAL;
      }
      else
      {
         addstr(" says, ");
         move(14,1);
         
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         switch(statement)
         {
         case 0: addstr("\"Woof?\""); break;
         case 1: addstr("\"Bark!\""); break;
         case 2: addstr("\"Woof!\""); break;
         case 3: addstr("\"Woof!\""); break;
         case 4: addstr("\"Bark! Grr...\""); break;
         case 5: addstr("\"Rr...?\""); break;
         case 6: addstr("\"Grrr...!\""); break;
         case 7: addstr("\"Bark!\""); break;
         case 8: addstr("\"Bark!\""); break;
         case 9: addstr("\"Bark!\""); break;
         case 10: addstr("\"Your accent is atrocious.\""); break;
         }

         tk->cantbluff=1;
      }

      refresh();
      getch();

      return 1;
   }

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
         add_age(*tk);
         addstr(":");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         switch(talkmode)
         {
            case TALKMODE_RENTING:
            {
               int rent=200;
               switch(location[cursite]->type)
               {
                  case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
                  case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
               }
               if(ledger.get_funds()<rent)set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(11,1);
               addstr("A - Accept.");
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(12,1);
               addstr("B - Decline.");
               move(13,1);
               addstr("C - Threaten the landlord.");
               break;
            }
            case TALKMODE_START:
               move(11,1);
               addstr("A - Strike up a conversation about politics");
               if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
               addstr(".");
               move(12,1);
               if(tk->can_date(a))set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               addstr("B - Drop a pickup line");
               if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
               addstr(".");
               set_color(COLOR_WHITE,COLOR_BLACK,0);
               move(13,1);
               addstr("C - On second thought, don't say anything");
               if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
               addstr(".");
               if(encounter[t].type==CREATURE_LANDLORD&&
                  location[cursite]->renting==-1)
               {
                  move(14,1);
                  addstr("D - Rent a room");
                  if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
                  addstr(".");
               } else if(encounter[t].type==CREATURE_LANDLORD&&
                       location[cursite]->renting>0)
                    {
                       move(14,1);
                       addstr("D - Stop renting a room");
                       if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
                       addstr(".");
                    }

               if((encounter[t].type==CREATURE_GANGMEMBER||encounter[t].type==CREATURE_MERC))
               {
                  move(14,1);
                  addstr("D - Buy weapons");
                  if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)addstr(" while naked");
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
               int lw = LCSrandom(LAWNUM); // pick a random law to talk about

               bool succeeded         = false;
               bool you_are_stupid    = false;
               bool issue_too_liberal = false;
               
               if(!(a.attribute_check(ATTRIBUTE_INTELLIGENCE,DIFFICULTY_EASY)))
                  you_are_stupid = true;
               else if(law[lw]==ALIGN_ELITELIBERAL && newscherrybusted)
                  issue_too_liberal = true;

               clearcommandarea();clearmessagearea();clearmaparea();
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(9,1);addstr(a.name);addstr(" says,");
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               int y=10;
               move(y,1);y++;
               if(you_are_stupid)
               {
                  switch(lw)
                  {
                  case LAW_ABORTION:      addstr("\"Conservatives make women turn to coat hangers.\"");break;
                  case LAW_ANIMALRESEARCH:addstr("\"Richard Gere put a gerbil in his butt!\"");break;
                  case LAW_POLICEBEHAVIOR:
                                          if(law[LAW_FREESPEECH]==ALIGN_ARCHCONSERVATIVE)
                                              addstr("\"[The police are not doing their job very well!]\"");
                                          else
                                              addstr("\"The cops suck!\"");
                                          break;
                  case LAW_PRIVACY:       addstr("\"The government, like, knows things about you.\"");break;
                  case LAW_DEATHPENALTY:  addstr("\"They executed this one dude, and like, his head caught on fire.\"");break;
                  case LAW_NUCLEARPOWER:  addstr("\"Have you seen Godzilla?  Nuclear power is bad, yo.\"");break;
                  case LAW_POLLUTION:     addstr("\"You wanna look like the Toxic Avenger?  Oppose pollution!\"");break;
                  case LAW_LABOR:         addstr("\"Bad people wanna make babies work and stuff.\"");break;
                  case LAW_GAY:           addstr("\"Lots of people don't like homosexuals.\"");break;
                  case LAW_CORPORATE:     addstr("\"The corporations are putting you down, dude.\"");break;
                  case LAW_FREESPEECH:    addstr("\"Better watch what you say.  They've got ears everywhere.\"");break;
                  case LAW_FLAGBURNING:   addstr("\"The flag is stupid.\"");break;
                  case LAW_TAX:           addstr("\"Rich people, like, have money, man.\"");break;
                  case LAW_GUNCONTROL:    addstr("\"People like, think they need lots of guns.\"");break;
                  case LAW_WOMEN:         addstr("\"We need more women!\"");break;
                  case LAW_CIVILRIGHTS:   addstr("\"Conservatives are all racists!\"");break;
                  case LAW_DRUGS:         addstr("\"Dude, the government won't let you do drugs.\"");break;
                  case LAW_IMMIGRATION:   addstr("\"They're all trying to keep people out of the country.\"");break;
                  case LAW_ELECTIONS:     addstr("\"The politicians are just tools of the corporations!\"");break;
                  case LAW_MILITARY:      addstr("\"Patriots are idiots! Give peace a chance!\"");break;
                  case LAW_TORTURE:       addstr("\"Torture is bad!\"");break;
                  }
               }
               else if(issue_too_liberal)
               {
                  switch(lw)
                  {
                  case LAW_ABORTION:      addstr("\"Conservatives don't like abortion.\"");break;
                  case LAW_ANIMALRESEARCH:addstr("\"Animals are denied the right to vote.\"");break;
                  case LAW_POLICEBEHAVIOR:addstr("\"The police are still out there.\"");break;
                  case LAW_PRIVACY:       addstr("\"The government runs intelligence agencies.\"");break;
                  case LAW_DEATHPENALTY:  addstr("\"You can go to prison for life for serious crimes.\"");break;
                  case LAW_NUCLEARPOWER:  addstr("\"Some people support legalizing nuclear power.\"");break;
                  case LAW_POLLUTION:     addstr("\"We're still polluting a little bit.\"");break;
                  case LAW_LABOR:         addstr("\"Corporate bosses don't always give in to unions.\"");break;
                  case LAW_GAY:           addstr("\"Not everybody likes gay people.\"");break;
                  case LAW_CORPORATE:     addstr("\"There are corporations.\"");break;
                  case LAW_FREESPEECH:    addstr("\"People get mad if you swear a lot in public.\"");break;
                  case LAW_FLAGBURNING:   addstr("\"The flag code says you shouldn't make it into clothing.\"");break;
                  case LAW_TAX:           addstr("\"There's still inequality in this country.\"");break;
                  case LAW_GUNCONTROL:    addstr("\"We need to repeal the second amendment.\"");break;
                  case LAW_WOMEN:         addstr("\"Some people are sexist.\"");break;
                  case LAW_CIVILRIGHTS:   addstr("\"I knew some people that were pretty racist.\"");break;
                  case LAW_DRUGS:         addstr("\"Drugs are expensive.\"");break;
                  case LAW_IMMIGRATION:   addstr("\"All the immigrants, not everybody likes them.\"");break;
                  case LAW_ELECTIONS:     addstr("\"Some of these politicians rub me the wrong way.\"");break;
                  case LAW_MILITARY:      addstr("\"We still have a military.\"");break;
                  case LAW_TORTURE:       addstr("\"Some conservatives support torture.\"");break;
                  }
               }
               else
               {
                  switch(lw)
                  {
                  case LAW_ABORTION:
                     addstr("\"The government is systematically trying to rob women of the right");move(y,1);y++;
                     addstr("to control their own destinies.\"");
                     break;
                  case LAW_ANIMALRESEARCH:
                     addstr("\"Animals are routinely subjected to inhumane treatment in labs in this");move(y,1);y++;
                     addstr("country.\"");
                     break;
                  case LAW_POLICEBEHAVIOR:
                     addstr("\"The police regularly torture minority suspects during interrogations.\"");
                     break;
                  case LAW_PRIVACY:
                     addstr("\"Files are being kept on innocent citizens whose only crime is to");move(y,1);y++;
                     addstr("speak out against a system that is trying to farm them like beasts.\"");
                     break;
                  case LAW_DEATHPENALTY:
                     addstr("\"Over thirty innocent people have been executed over the past decade.\"");
                     break;
                  case LAW_NUCLEARPOWER:
                     addstr("\"Radioactive waste is being stored all over the country, and it poses");move(y,1);y++;
                     addstr("a serious threat to many families, even in this neighborhood.\"");
                     break;
                  case LAW_POLLUTION:
                     addstr("\"Industries that stop at nothing to become more profitable are polluting");move(y,1);y++;
                     addstr("the environment in ways that hurt not only animals, but people too.");
                     break;
                  case LAW_LABOR:
                     addstr("\"Have you noticed how people are working more and more hours for less and");move(y,1);y++;
                     addstr("less money?  It's all part of a plan to keep you enslaved, man.\"");
                     break;
                  case LAW_GAY:
                     addstr("\"Homosexuals are people like anyone else, and yet they are treated in this");move(y,1);y++;
                     addstr("country as if they are deviants fit only for cheap entertainment.\"");
                     break;
                  case LAW_CORPORATE:
                     addstr("\"Corporate executives use giant corporations as a means to become parasites");move(y,1);y++;
                     addstr("that suck wealth out of this country and put it into their pockets.\"");
                     break;
                  case LAW_FREESPEECH:
                     addstr("\"Protests and demonstrations are regularly and often brutally suppressed in");move(y,1);y++;
                     addstr("this country.  People have to watch what they write -- even what they read.\"");
                     break;
                  case LAW_FLAGBURNING:
                     addstr("\"Burning a piece of cloth is actually stigmatized in this country.");move(y,1);y++;
                     addstr("You can love freedom and still hate what our government stands for.\"");
                     break;
                  case LAW_TAX:
                     addstr("\"The tax code has been designed to perpetuate an unjust class");move(y,1);y++;
                     addstr("structure that is keeping you oppressed.\"");
                     break;
                  case LAW_GUNCONTROL:
                     addstr("\"We live in such a backwards country right now that people think it's");move(y,1);y++;
                     addstr("a right to walk around with the power to murder at any moment.\"");
                     break;
                  case LAW_WOMEN:
                     addstr("\"Sexism is still pervasive, in subtle ways, and women make much less");move(y,1);y++;
                     addstr("than they deserve for their labor.\"");
                     break;
                  case LAW_CIVILRIGHTS:
                     addstr("\"Despite our progress, this society is still strangled by its continuing");move(y,1);y++;
                     addstr("legacy of racial discrimination and inequality.\"");
                     break;
                  case LAW_DRUGS:
                     addstr("\"The government's drug policy is a mess.  We need to stop filling");move(y,1);y++;
                     addstr("prisons with drug users, and only intervene when people really need help.\"");
                     break;
                  case LAW_IMMIGRATION:
                     addstr("\"Millions of people are doing jobs most folks don't even want, and");move(y,1);y++;
                     addstr("saving their families from poverty, but we just try to kick them out.\"");
                     break;
                  case LAW_ELECTIONS:
                     addstr("\"Political favors are bought and sold for campaign contributions,");move(y,1);y++;
                     addstr("and the voting system enforces two party dominance.\"");
                     break;
                  case LAW_MILITARY:
                     addstr("\"Take a breath and think about the world we live in, that we're spending");move(y,1);y++;
                     addstr("hundreds of billions on new ways to kill people.  This has to stop!\"");
                     break;
                  case LAW_TORTURE:
                     addstr("\"In the name of the war on terror, we've sacrificed our soul by letting");move(y,1);y++;
                     addstr("the government torture and abuse human beings on our behalf.\"");
                     break;
                  }
               }
               refresh();
               getch();

               int difficulty = DIFFICULTY_VERYEASY;

               if(tk->align == ALIGN_CONSERVATIVE)
                  difficulty += 7;
               if(!(tk->talkreceptive()))
                  difficulty += 7;
               if(you_are_stupid)
                  difficulty += 5;
               if(issue_too_liberal)
                  difficulty += 5;
               if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)
                  difficulty += 5;
               
               succeeded = a.skill_check(SKILL_PERSUASION,difficulty);

               // Prisoners never accept to join you, you must liberate them instead
               if(succeeded && tk->type!=CREATURE_PRISONER)
               {
                  y++;
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(y,1);y++;addstr(tk->name);addstr(" responds,");
                  set_color(COLOR_BLUE,COLOR_BLACK,1);
                  move(y,1);y++;
                  if(tk->type==CREATURE_MUTANT&&
                     tk->get_attribute(ATTRIBUTE_INTELLIGENCE,true)<3)
                  {
                     addstr("\"Aaaahhh...\"");
                  }
                  else
                  {
                     switch(LCSrandom(10))
                     {
                     case 0: addstr("\"Dear me! Is there anything we can do?\""); break;
                     case 1: addstr("\"That *is* disturbing!   What can I do?\""); break;
                     case 2: addstr("\"Gosh!   Is there anything I can do?\""); break;
                     case 3: addstr("\"That's frightening!   What can we do?\""); break;
                     case 4: addstr("\"Oh, really?\" ");
                             refresh();
                             getch();
                             move(y,1);y++;
                             set_color(COLOR_GREEN,COLOR_BLACK,1);
                             addstr("\"Yeah, really!\"");
                             break;
                     case 5: addstr("\"Oh my Science!   We've got to do something!\""); break;
                     case 6: addstr("\"Dude... that's like... totally bumming me.\""); break;
                     case 7: addstr("\"Gadzooks! Something must be done!\"");break;
                     case 8: addstr("\"You got anything to smoke on you?\" ");
                             set_color(COLOR_WHITE,COLOR_BLACK,1);
                             addstr("*cough*");
                             break;
                     case 9: addstr("\"Lawks, I don't think we can allow that.\"");break;
                     }
                  }
                  
                  refresh();
                  getch();

                  y++;move(y,1);y++;
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr("After more discussion, ");
                  addstr(tk->name);
                  addstr(" agrees to come by later tonight.");
                  refresh();
                  getch();

                  Creature *newcr=new Creature;
                     *newcr=*tk;
                  newcr->namecreature();

                  recruitst *newrst=new recruitst;
                  newrst->recruit=newcr;
                  newrst->recruiter_id = a.id;

                  //newcr->align=1;

                  recruit.push_back(newrst);

                  delenc(t,0);
                  return 1;
               }
               else
               {
                  y++;
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(y,1);y++;addstr(tk->name);addstr(" responds,");
                  set_color(COLOR_BLUE,COLOR_BLACK,1);
                  if(tk->type==CREATURE_MUTANT&&
                     tk->get_attribute(ATTRIBUTE_INTELLIGENCE,true)<3)
                  {
                     move(y,1);y++;
                     addstr("\"Ugh.  Pfft.\"");
                  }
                  else
                  {
                     if(tk->align==ALIGN_CONSERVATIVE && you_are_stupid)
                     {
                        move(y,1);y++;
                        if(tk->type==CREATURE_GANGUNIT)
                           addstr("\"Do you want me to arrest you?\"");
                        else if(tk->type==CREATURE_DEATHSQUAD)
                           addstr("\"If you don't shut up, I'm going to shoot you.\"");
                        else
                        {
                           switch(LCSrandom(10))
                           {
                           case 0:addstr("\"Get away from me, you hippie.\"");break;
                           case 1:addstr("\"My heart aches for humanity.\"");break;
                           case 2:addstr("\"I'm sorry, but I think I'm done talking to you.\"");break;
                           case 3:addstr("\"Do you need some help finding the exit?\"");break;
                           case 4:addstr("\"People like you are the reason I'm on medication.\"");break;
                           case 5:addstr("\"Everyone is entitled to be stupid, but you abuse the privilege.\"");break;
                           case 6:addstr("\"I don't know what you're on, but I hope it's illegal.\"");break;
                           case 7:addstr("\"Don't you have a parole meeting to get to?\"");break;
                           case 8:addstr("\"Wow. Why am I talking to you again?\"");break;
                           case 9:addstr("\"Were you dropped as a child?\"");break;
                           }
                        }
                     }
                     else if(tk->align != ALIGN_LIBERAL && tk->attribute_check(ATTRIBUTE_WISDOM,DIFFICULTY_AVERAGE))
                     {
                        move(y,1);y++;
                        switch(lw)
                        {
                        case LAW_ABORTION:      addstr("\"Abortion is murder.\"");break;
                        case LAW_ANIMALRESEARCH:addstr("\"Don't pretend animals are human.\"");break;
                        case LAW_POLICEBEHAVIOR:addstr("\"Only criminals have reason to fear police.\"");break;
                        case LAW_PRIVACY:       addstr("\"National security is important.\"");break;
                        case LAW_DEATHPENALTY:  addstr("\"Some people deserve to die.\"");break;
                        case LAW_NUCLEARPOWER:  addstr("\"Nuclear power is clean.\"");break;
                        case LAW_POLLUTION:     addstr("\"It's not that bad.\"");break;
                        case LAW_LABOR:         addstr("\"Trust the free market, it hasn't failed us yet.\"");break;
                        case LAW_GAY:           addstr("\"Homosexuality is a sin.\"");break;
                        case LAW_CORPORATE:     addstr("\"Corporations are part of capitalism.\"");break;
                        case LAW_FREESPEECH:    addstr("\"Don't be offensive and you'll be fine.\"");break;
                        case LAW_FLAGBURNING:   addstr("\"That flag is the sacred symbol of our country.\"");break;
                        case LAW_TAX:           addstr("\"I want to pay lower taxes.\"");break;
                        case LAW_GUNCONTROL:    addstr("\"Without guns, we're slaves to the Government.\"");break;
                        case LAW_WOMEN:         addstr("\"Why don't you go burn a bra or something?\"");break;
                        case LAW_CIVILRIGHTS:   addstr("\"Reverse discrimination is still discrimination.\"");break;
                        case LAW_DRUGS:         addstr("\"Drugs are a terrible influence on society.\"");break;
                        case LAW_IMMIGRATION:   addstr("\"Immigration undermines our economy and culture.\"");break;
                        case LAW_ELECTIONS:     addstr("\"Unregulated campaigning is a matter of free speech.\"");break;
                        case LAW_MILITARY:      addstr("\"The military protects us and enables our way of life.\"");break;
                        case LAW_TORTURE:       addstr("\"The terrorists would do worse to us.\"");break;
                        }
                     }
                     else
                     {
                        move(y,1);y++;
                        addstr("\"Whatever.\"");
                     }
                  }
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr(" <turns away>");
                  refresh();
                  getch();

                  encounter[t].cantbluff=1;
                  return 1;
               }
               break;
            }
            case TALKMODE_RENTING:
            {
               int rent=200;
               switch(location[cursite]->type)
               {
                  case SITE_RESIDENTIAL_APARTMENT:rent=650;break;
                  case SITE_RESIDENTIAL_APARTMENT_UPSCALE:rent=1500;break;
               }
               if(c=='a' && ledger.get_funds()>=rent)
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

                  ledger.subtract_funds(rent,EXPENSE_RENT);
                  location[cursite]->renting=rent;
                  location[cursite]->newrental=1;

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
               if(c=='c')
               {
                  clearcommandarea();clearmessagearea();clearmaparea();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  Creature *armed_liberal=NULL;
                  for(int i=0;i<6;i++)
                  {
                     if(activesquad->squad[i] &&
                        activesquad->squad[i]->get_weapon().is_threatening())
                     {
                        armed_liberal=activesquad->squad[i];
                        break;
                     }
                  }
                  if(armed_liberal)
                  {
                     move(9,1);
                     addstr(armed_liberal->name);
                     addstr(" brandishes the ");
                     addstr(armed_liberal->get_weapon().get_shortname(0).c_str());
                     addstr(".");
                     refresh();
                     getch();
                     clearmessagearea();
                  }
                  move(9,1);
                  addstr(a.name);
                  addstr(" says,");
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  move(10,1);
                  addstr("\"What's the price for the Liberal Crime Squad?\"");
                  refresh();
                  getch();

                  int roll       = a.skill_roll(SKILL_PERSUASION);
                  int difficulty = DIFFICULTY_FORMIDABLE;
                  
                  if(newscherrybusted == false)
                     difficulty += 6;
                  if(armed_liberal == NULL)
                     difficulty += 6;

                  if(roll < difficulty - 1)
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(12,1);addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(13,1);
                     addstr("\"I think you'd better leave.\"");
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr(" <crosses arms>");
                     refresh();
                     getch();
                     tk->cantbluff = 1;
                     return 1;
                  }
                  else
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(12,1);addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_BLUE,COLOR_BLACK,1);
                     move(13,1);
                     addstr("\"Jesus... it's yours...\"");
                     refresh();
                     getch();

                     int rent;

                     // Either he calls the cops...
                     if(roll < difficulty) 
                     {
                        for(int i=0;i<6;i++)
                        {
                           if(activesquad->squad[i])
                              criminalize(*(activesquad->squad[i]),LAWFLAG_EXTORTION);
                        }
                        location[cursite]->siege.timeuntillocated = 2;
                        rent = 10000000; // Yeah he's kicking you out next month
                     }
                     // ...or it's yours for free
                     else
                     {
                        rent = 0;
                     }

                     location[cursite]->renting=rent;
                     location[cursite]->newrental=true;

                     basesquad(activesquad,cursite);
                     return 1;
                  }
               }
               break;
            }
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
                  
                  bool interested = tk->talkreceptive();

                  if(!interested && a.skill_check(SKILL_PERSUASION,DIFFICULTY_AVERAGE))
                     interested = true;

                  if((tk->animalgloss==ANIMALGLOSS_ANIMAL&&tk->align!=ALIGN_LIBERAL)||
                     tk->animalgloss==ANIMALGLOSS_TANK)
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(12,1);addstr(tk->name);

                     switch(tk->type)
                     {
                     case CREATURE_TANK:
                        addstr(" rumbles disinterestedly.");
                        break;
                     case CREATURE_GUARDDOG:
                        addstr(" barks.");
                        break;
                     default:
                        addstr(" doesn't understand.");
                     }
                     
                     refresh();
                     getch();

                     return 1;
                  }
                  else if(tk->type!=CREATURE_PRISONER && interested)
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
                     if(tk->type==CREATURE_PRISONER)
                     {
                        if(tk->align==ALIGN_LIBERAL)
                           addstr("\"Now's not the time!\"");
                        else
                           addstr("\"Leave me alone.\"");
                     }
                     else
                     {
                        addstr("\"No.\"");
                     }
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr(" <turns away>");
                     refresh();
                     getch();

                     return 1;
                  }
               }
               if(c=='b'&&tk->can_date(a))
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
                     case 0:addstr("\"Hey baby, you're kinda ugly.  I like that.\"");break;
                     case 1:addstr("\"I lost my phone number.  Could I have yours?\"");break;
                     case 2:addstr("\"Hey, you wanna go rub one off?\"");break;
                     case 3:addstr("\"Hot damn.  You're built like a brick shithouse, honey.\"");break;
                     case 4:addstr("\"I know I've seen you on the back of a milk carton,");
                            move(11,1);y++;
                            addstr("cuz you've been missing from my life.\"");
                            break;
                     case 5:addstr("\"I'm big where it counts.\"");break;
                     case 6:addstr("\"Daaaaaamn girl, I want to wrap your legs around my face and");
                            move(11,1);y++;
                            addstr("wear you like a feed bag!\""); // Bill Hicks
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

                  bool succeeded = false;

                  int difficulty = DIFFICULTY_HARD;

                  if(tk->type == CREATURE_CORPORATE_CEO)
                     difficulty = DIFFICULTY_HEROIC;

                  if(a.skill_check(SKILL_SEDUCTION,difficulty))
                     succeeded = true;

                  if((tk->animalgloss==ANIMALGLOSS_ANIMAL&&law[LAW_ANIMALRESEARCH]!=2)||
                     tk->animalgloss==ANIMALGLOSS_TANK)
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(12,1);addstr(tk->name);

                     switch(tk->type)
                     {
                     case CREATURE_TANK:
                        addstr(" shakes its turret a firm 'no'.");
                        break;
                     case CREATURE_GUARDDOG:
                        addstr(" says, ");
                        move(13,1);
                        set_color(COLOR_RED,COLOR_BLACK,1);
                        switch(LCSrandom(3))
                        {
                        case 0:addstr("\"No! Wrong! I'm a dog!! Jesus.\"");break;
                        case 1:addstr("\"What?! Ugh, I'm going to toss my kibble.\"");break;
                        case 2:addstr("\"Okay, you need to stop petting me now.\"");break;
                        }
                        tk->align=ALIGN_CONSERVATIVE;
                        tk->cantbluff=1;
                        break;
                     default:
                        addstr(" doesn't quite pick up on the subtext.");
                     }
                     
                     refresh();
                     getch();

                     return 1;
                  }

                  a.train(SKILL_SEDUCTION,LCSrandom(5)+2);

                  if((a.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM" // Police property on armor? -XML
                           || a.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")
                     && tk->type==CREATURE_PROSTITUTE)
                  {
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(y,1);y++;addstr(tk->name);addstr(" responds,");
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     move(y,1);y++;

                     addstr("Dirty. You know that's illegal, officer.");

                     refresh();
                     getch();

                     encounter[t].cantbluff=1;
                  }
                  else if(succeeded)
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
                        criminalize(*tk,LAWFLAG_ESCAPED);
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

                     Creature *newcr=new Creature;
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
                     set_color(COLOR_RED,COLOR_BLACK,1);
                     move(y,1);y++;
                     if(tk->type == CREATURE_CORPORATE_CEO)
                     {
                        if(a.gender_liberal != GENDER_MALE)
                        {
                           addstr("\"I'm a happily married man, sweetie.\"");
                        }
                        else
                        {
                           addstr("\"This ain't Brokeback Mountain, son.\"");
                        }
                     }
                     else
                     {
                        switch(LCSrandom(9))
                        {
                        case 0: addstr("\"Jesus...\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <turns away>");break;
                        case 1: addstr("\"Touch me and you'll regret it.\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <crosses arms>");break;
                        case 2: addstr("\"I'm.. uh.. waiting for someone.\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <turns away>");break;
                        case 3: addstr("\"Hey, look, a UFO!\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <ducks away>");break;
                        case 4: addstr("\"You're not my type. I like sane people.\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <turns away>");break;
                        case 5: addstr("\"Hahahahaha!\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <shakes head>");break;
                        case 6: addstr("\"You're disgusting.\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <turns away>");break;
                        case 7: addstr("\"Are you serious?\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <turns away>");break;
                        case 8: addstr("\"You're a pig.\"");
                           set_color(COLOR_WHITE,COLOR_BLACK,1);
                           addstr(" <turns away>");break;
                        }
                     }
                     
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

                     if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)
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

                     int rent=200;
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
                     
                     talkmode=TALKMODE_RENTING;
                  }
               }
               else if (encounter[t].type == CREATURE_LANDLORD && location[cursite]->renting > 0)
               {
                  if (c == 'd')
                  {
                     clearcommandarea();
                     clearmessagearea();
                     clearmaparea();
                     set_color(COLOR_WHITE, COLOR_BLACK, 1);
                     move(9, 1);
                     addstr(a.name);
                     addstr(" says,");
                     set_color(COLOR_GREEN, COLOR_BLACK, 1);
                     move(10, 1);
                     addstr("\"I'd like cancel my room.\"");
                     refresh();
                     getch();

                     if (a.is_naked() && a.animalgloss != ANIMALGLOSS_ANIMAL)
                     {
                        set_color(COLOR_WHITE, COLOR_BLACK, 1);
                        move(12, 1);
                        addstr(tk->name);
                        addstr(" responds,");
                        set_color(COLOR_BLUE, COLOR_BLACK, 1);
                        move(13, 1);
                        addstr("\"Put some clothes on before I call the cops.\"");
                        refresh();
                        getch();
                        return 1;
                     }

                     set_color(COLOR_WHITE, COLOR_BLACK, 1);
                     move(12, 1);
                     addstr(tk->name);
                     addstr(" responds,");
                     set_color(COLOR_BLUE, COLOR_BLACK, 1);
                     move(13, 1);
                     addstr("\"Alright. Please clear out your room.\"");
                     refresh();
                     getch();

                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(15,1);
                     addstr("<Your possessions at this location have been moved to the shelter.>");
                     refresh();
                     getch();

                     location[cursite]->renting=RENTING_NOCONTROL;

                     //MOVE ALL ITEMS AND SQUAD MEMBERS
                     int hs=0;
                     for(int l2=0;l2<location.size();l2++)
                     {
                        if(location[l2]->type==SITE_RESIDENTIAL_SHELTER)
                        {
                           hs=l2;
                           break;
                        }
                     }
                     for(int p=0;p<pool.size();p++)
                     {
                        if(pool[p]->location==cursite)pool[p]->location=hs;
                        if(pool[p]->base==cursite)pool[p]->base=hs;
                     }
                     for(int l2=0;l2<location[cursite]->loot.size();l2++)
                     {
                        location[hs]->loot.push_back(location[cursite]->loot[l2]);
                     }
                     location[cursite]->loot.clear();

                     location[cursite]->compound_walls=0;
                     location[cursite]->compound_stores=0;
                     location[cursite]->front_business=-1;

                     return 1;
                  }
               }
               if((encounter[t].type==CREATURE_GANGMEMBER||encounter[t].type==CREATURE_MERC))
               {
                  if(c=='d')
                  {
                     clearcommandarea();clearmessagearea();clearmaparea();
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     move(9,1);addstr(a.name);addstr(" says,");
                     set_color(COLOR_GREEN,COLOR_BLACK,1);
                     move(10,1);
                     addstr("\"Hey, I need a gun.\"");
                     refresh();
                     getch();

                     if(a.is_naked() && a.animalgloss!=ANIMALGLOSS_ANIMAL)
                     {
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(12,1);addstr(tk->name);addstr(" responds,");
                        set_color(COLOR_BLUE,COLOR_BLACK,1);
                        move(13,1);
                        addstr("\"Jesus...\"");
                        refresh();
                        getch();
                        return 1;
                     }
                     if(a.get_armor().get_itemtypename()=="ARMOR_POLICEUNIFORM"
                        ||a.get_armor().get_itemtypename()=="ARMOR_POLICEARMOR")
                     {
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(12,1);addstr(tk->name);addstr(" responds,");
                        set_color(COLOR_BLUE,COLOR_BLACK,1);
                        move(13,1);
                        addstr("\"I don't sell guns, officer.\"");
                        refresh();
                        getch();
                        return 1;
                     }
                     if(sitealarm!=0)
                     {
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(12,1);addstr(tk->name);addstr(" responds,");
                        set_color(COLOR_BLUE,COLOR_BLACK,1);
                        move(13,1);
                        addstr("\"We can talk when things are calm.\"");
                        refresh();
                        getch();
                        return 1;
                     }
                     switch(location[cursite]->type)
                     {
                     case SITE_OUTDOOR_BUNKER:
                     case SITE_BUSINESS_CRACKHOUSE:
                     case SITE_BUSINESS_BARANDGRILL:
                     case SITE_BUSINESS_ARMSDEALER:
                     case SITE_RESIDENTIAL_TENEMENT:
                     case SITE_RESIDENTIAL_BOMBSHELTER:
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(12,1);addstr(tk->name);addstr(" responds,");
                        set_color(COLOR_BLUE,COLOR_BLACK,1);
                        move(13,1);
                        addstr("\"What exactly do you need?\"");
                        refresh();
                        getch();
                        break;
                     default:
                        set_color(COLOR_WHITE,COLOR_BLACK,1);
                        move(12,1);addstr(tk->name);addstr(" responds,");
                        set_color(COLOR_BLUE,COLOR_BLACK,1);
                        move(13,1);
                        addstr("\"Uhhh... not a good place for this.\"");
                        refresh();
                        getch();
                        return 1;
                     }

                     armsdealer(cursite);
                  }
               }
               break;
         }
      }
   }while(1);
}

