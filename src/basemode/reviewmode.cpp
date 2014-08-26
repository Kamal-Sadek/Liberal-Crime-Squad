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

// Note: this file is encoded in the PC-8 / Code Page 437 / OEM-US character set
// (The same character set used by Liberal Crime Squad when it is running)
// Certain special characters won't display correctly unless your text editor is
// set to use that character set, such as this e with an accent: ‚

// In Windows Notepad with the Terminal font, OEM/DOS encoding it should work fine.
// You can set this in Notepad by going to Format->Font and choosing the Terminal font,
// then choosing OEM/DOS in the Script dropdown box.

// In Notepad++ go to the Encoding menu, Character sets, Western European, OEM-US... easy!

// In Code::Blocks's editor go to Settings->Editor->the Other Settings tab and
// then pick WINDOWS-437 from the dropdown box and then choose the radio button
// to make this the default encoding and disable auto-detection of the encoding.
// Then close the file and reopen it (since Code::Blocks detects the encoding
// when it opens the file and it can't be changed after that; what we changed was
// how it detects encoding for files it opens in the future, not files already open).

// In Microsoft Visual C++, right-click the file in the Solution Explorer,
// select "Open With...", choose "C++ Source Code Editor (with encoding)",
// then choose "OEM United States - Codepage 437".

// In MS-DOS Editor (included with Windows as EDIT.COM in your system32 directory),
// the codepage will be correct already since it's running in a console window just
// like Liberal Crime Squad. Well OK, the encoding might be wrong, but then it's wrong
// in Liberal Crime Squad TOO, and to fix it, go to Control Panel, Regional and Language Settings,
// Advanced tab, and choose English (United States) from the dropdown box as the encoding
// for non-Unicode applications, then press OK.

// If you have a Linux or other UNIX-based system you are obviously smart enough
// to figure out for yourself how to open a file in OEM-US PC-8 codepage 437 in
// your favorite text editor. If you're on Mac OS X, well that's UNIX-based, figure
// it out for yourself.

#include <externs.h>

/* base - review and reorganize liberals */
void review()
{
   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      mvaddstr(0,0,"Review your Liberals and Assemble Squads");
      mvaddstr(1,0,"ÄÄÄÄSQUAD NAMEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄLOCATIONÄÄÄÄÄÄÄÄÄÄÄÄACTIVITYÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"); // 80 characters

      int n[8]={0,0,0,0,0,0,0,0},y=2;
      for(int p=0;p<len(pool);p++)
      {
         if(pool[p]->is_active_liberal()) n[0]++; // Active Liberals
         if(pool[p]->align!=ALIGN_LIBERAL && pool[p]->alive) n[1]++; // Hostages
         if(pool[p]->clinic && pool[p]->alive) n[2]++; // Hospital
         if(pool[p]->is_imprisoned()) n[3]++; // Justice System
         if(pool[p]->is_lcs_sleeper()) n[4]++; // Sleepers
         if(!pool[p]->alive) n[5]++; // The Dead
         if((pool[p]->dating || pool[p]->hiding) && pool[p]->alive) n[6]++; // Away
      }
      for(int l=0;l<len(location);l++)
      {
         consolidateloot(location[l]->loot);
         if(!location[l]->siege.siege)
            n[7]+=len(location[l]->loot); // Review and Move Equipment
      }

      for(int p=page*19;p<len(squad)+REVIEWMODENUM+1&&p<page*19+19;p++,y++)
      {
         if(p<len(squad))
         {
            if(activesquad==squad[p])set_color(COLOR_WHITE,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,0);
            mvaddchar(y,0,y+'A'-2);addstr(" - ");
            addstr(squad[p]->name);

            if(squad[p]->squad[0]!=NULL&&squad[p]->squad[0]->location!=-1)
               mvaddstr(y,31,location[squad[p]->squad[0]->location]->getname(true, true));

            if(squad[p]->squad[0]!=NULL)
            {
               std::string str=getactivity(squad[p]->activity);
               if(squad[p]->activity.type==ACTIVITY_NONE)
               {
                  bool haveact=false,multipleact=false;
                  for(int p2=0;p2<6;p2++)
                  {
                     if(squad[p]->squad[p2]==NULL) continue;
                     const std::string str2=getactivity(squad[p]->squad[p2]->activity);
                     if(haveact&&str!=str2) multipleact=true;
                     str=str2,haveact=true;
                  }
                  if(multipleact) str="Acting Individually";
               }
               mvaddstr(y,51,str);
            }
         }
         else if(p==len(squad))
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            mvaddstr(y,0,"1 - Active Liberals ("+tostring(n[0])+')');
         }
         else if(p==len(squad)+1)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            mvaddstr(y,0,"2 - Hostages ("+tostring(n[1])+')');
         }
         else if(p==len(squad)+2)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            mvaddstr(y,0,"3 - Hospital ("+tostring(n[2])+')');
         }
         else if(p==len(squad)+3)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            mvaddstr(y,0,"4 - Justice System ("+tostring(n[3])+')');
         }
         else if(p==len(squad)+4)
         {
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            mvaddstr(y,0,"5 - Sleepers ("+tostring(n[4])+')');
         }
         else if(p==len(squad)+5)
         {
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            mvaddstr(y,0,"6 - The Dead ("+tostring(n[5])+')');
         }
         else if(p==len(squad)+6)
         {
            set_color(COLOR_BLUE,COLOR_BLACK,1);
            mvaddstr(y,0,"7 - Away ("+tostring(n[6])+')');
         }
         else if(p==len(squad)+7)
         {
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            mvaddstr(y,0,"8 - Review and Move Equipment ("+tostring(n[7])+')');
         }
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      mvaddstr(21,0,"Press V to Inspect Liberal finances.");
      if(music.isEnabled())
         mvaddstr(21,38,"Press Y to turn off the Music.");
      else mvaddstr(21,38,"Press Y to turn on some Music.");
      move(22,0);
      addstr("Press a Letter to select a squad.  1-7 to view Liberal groups.");
      move(23,0);
      addpagestr();
      addstr("  Press U to Promote Liberals.");
      move(24,0);
      addstr("Press Z to Assemble a New Squad.  Press T to Assign New Bases to the Squadless.");

      int c=getkey();

      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(squad)+REVIEWMODENUM) page++;

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) return;

      if(c>='a'&&c<='s')
      {
         int sq=page*19+c-'a';
         if(sq<len(squad)&&sq>=0)
         {
            if(squad[sq]==activesquad)assemblesquad(squad[sq]);
            else activesquad=squad[sq];
         }
      }
      if(c>='1'&&c<='7') review_mode(c-'1');
      if(c=='8') equipmentbaseassign();
      if(c=='z')
      {
         assemblesquad(NULL);
         if(!activesquad&&len(squad))
            activesquad=squad[len(squad)-1];
      }
      if(c=='t') squadlessbaseassign();
      if(c=='u') promoteliberals();
      if(c=='v')
      {
          char clearformess=false;
          fundreport(clearformess);
          if(clearformess) erase();
      }
      if(c=='y') music.enableIf(!music.isEnabled());
   }
}



void review_mode(short mode)
{
   vector<Creature *> temppool;
   Creature *swap = NULL;
   int swapPos = 0;

   for(int p=0;p<len(pool);p++)
   {
      switch(mode)
      {
      case REVIEWMODE_LIBERALS:
         if(pool[p]->is_active_liberal())
            temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_HOSTAGES:
         if(pool[p]->align!=ALIGN_LIBERAL && pool[p]->alive)
            temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_CLINIC:
         if(pool[p]->clinic && pool[p]->alive)
            temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_JUSTICE:
         if(pool[p]->is_imprisoned())
            temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_SLEEPERS:
         if(pool[p]->is_lcs_sleeper())
            temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_DEAD:
         if(!pool[p]->alive)
            temppool.push_back(pool[p]);
         break;
      case REVIEWMODE_AWAY:
         if((pool[p]->dating || pool[p]->hiding) && pool[p]->alive)
            temppool.push_back(pool[p]);
         break;
      }
   }

   if(!len(temppool)) return;

   sortliberals(temppool,activesortingchoice[reviewmodeenum_to_sortingchoiceenum(mode)]);

   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      switch(mode)
      {
      case REVIEWMODE_LIBERALS:
         addstr("Active Liberals");
         break;
      case REVIEWMODE_HOSTAGES:
         addstr("Conservative Automatons in Captivity");
         break;
      case REVIEWMODE_CLINIC:
         addstr("Liberals in CLINICS");
         break;
      case REVIEWMODE_JUSTICE:
         addstr("Liberals and the Justice System");
         break;
      case REVIEWMODE_SLEEPERS:
         addstr("Sleepers");
         break;
      case REVIEWMODE_DEAD:
         addstr("Liberal Martyrs and Dead Bodies");
         break;
      case REVIEWMODE_AWAY:
         addstr("Liberals that are Away");
         break;
      }
      move(1,0);
      addstr("ÄÄÄÄCODE NAMEÄÄÄÄÄÄÄÄÄÄÄÄSKILLÄÄÄHEALTHÄÄÄLOCATIONÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"); // 80 characters

      move(1,57);
      switch(mode)
      {
      case REVIEWMODE_LIBERALS:
         addstr("SQUAD / ACTIVITY");
         break;
      case REVIEWMODE_HOSTAGES:
         addstr("DAYS IN CAPTIVITY");
         break;
      case REVIEWMODE_JUSTICE:
         addstr("MONTHS LEFT");
         break;
      case REVIEWMODE_CLINIC:
         addstr("PROGNOSIS");
         break;
      case REVIEWMODE_SLEEPERS:
         addstr("PROFESSION");
         break;
      case REVIEWMODE_DEAD:
         addstr("DAYS SINCE PASSING");
         break;
      case REVIEWMODE_AWAY:
         addstr("DAYS UNTIL RETURN");
         break;
      }

      int y=2;
      for(int p=page*19;p<len(temppool)&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddchar(y,0,y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         int skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(int)temppool[p]->get_skill(sk);
            if(temppool[p]->get_skill_ip(sk)>=100+(10*temppool[p]->get_skill(sk))&&
               temppool[p]->get_skill(sk)<temppool[p]->skill_cap(sk,true))bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         addstr(skill);

         printhealthstat(*temppool[p],y,33,TRUE);

         if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,42);
         if(temppool[p]->location==-1) addstr("Away");
         else addstr(location[temppool[p]->location]->getname(true, true));

         move(y,57);
         switch(mode)
         {
         case REVIEWMODE_LIBERALS:
            {
               char usepers=1;
               if(temppool[p]->squadid!=-1)
               {
                  int sq=getsquad(temppool[p]->squadid);
                  if(sq!=-1)
                  {
                     if(squad[sq]->activity.type!=ACTIVITY_NONE)
                     {
                        set_color(COLOR_GREEN,COLOR_BLACK,1);
                        addstr("SQUAD");
                        usepers=0;
                     }
                  }
               }
               if(usepers)
               {  // Let's add some color here...
                  set_activity_color(temppool[p]->activity.type);
                  addstr(getactivity(temppool[p]->activity));
               }
               break;
            }
         case REVIEWMODE_HOSTAGES:
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr(temppool[p]->joindays);
               addstr(" ");
               if(temppool[p]->joindays>1)addstr("Days");
               else addstr("Day");
               break;
            }
         case REVIEWMODE_JUSTICE:
            {
               if(temppool[p]->deathpenalty&&temppool[p]->sentence!=0&&
                  location[temppool[p]->location]->type==SITE_GOVERNMENT_PRISON)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  addstr("DEATH ROW: ");
                  addstr(temppool[p]->sentence);
                  addstr(" ");
                  if(temppool[p]->sentence>1)addstr("Months");
                  else addstr("Month");
               }
               else if(temppool[p]->sentence<=-1&&
                        location[temppool[p]->location]->type==SITE_GOVERNMENT_PRISON)
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  if(temppool[p]->sentence<-1)
                  {
                     addstr(-(temppool[p]->sentence));
                     addstr(" Life Sentences");
                  }
                  else
                     addstr("Life Sentence");
               }
               else if(temppool[p]->sentence!=0&&
                        location[temppool[p]->location]->type==SITE_GOVERNMENT_PRISON)
               {
                  set_color(COLOR_YELLOW,COLOR_BLACK,1);
                  addstr(temppool[p]->sentence);
                  addstr(" ");
                  if(temppool[p]->sentence>1)addstr("Months");
                  else addstr("Month");
               }
               else
               {
                  set_color(COLOR_BLACK,COLOR_BLACK,1);
                  addstr("ÄÄÄÄÄÄÄ"); // 7 characters
               }
               break;
            }
         case REVIEWMODE_CLINIC:
            {
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               addstr("Out in ");
               addstr(temppool[p]->clinic);
               addstr(" ");
               if(temppool[p]->clinic>1)addstr("Months");
               else addstr("Month");
               break;
            }
         case REVIEWMODE_SLEEPERS:
            {
               if(temppool[p]->align==-1)set_color(COLOR_RED,COLOR_BLACK,1);
               else if(temppool[p]->align==0)set_color(COLOR_WHITE,COLOR_BLACK,1);
               else set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr(temppool[p]->get_type_name());
               break;
            }
         case REVIEWMODE_DEAD:
            {
               set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               addstr(temppool[p]->deathdays);
               addstr(" ");
               if(temppool[p]->deathdays>1)addstr("Days");
               else addstr("Day");
               break;
            }
         case REVIEWMODE_AWAY:
            {
               set_color(COLOR_CYAN,COLOR_BLACK,1);
               if(temppool[p]->hiding!=-1)
               {
                  addstr(temppool[p]->dating+temppool[p]->hiding);
                  addstr(" ");
                  if(temppool[p]->dating+temppool[p]->hiding>1)
                     addstr("Days");
                  else addstr("Day");
               }
               else addstr("<No Contact>");
               break;
            }
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to View Status.        Z - ");
      if(swap) { addstr("Place "); addstr(swap->name); }
      else addstr("Reorder Liberals");
      move(23,0);
      addpagestr();
      addstr(" T to sort people.");

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(temppool)) page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<len(temppool))
         {
            int page=0;
            //const int pagenum=2;
            while(true)
            {
               erase();

               move(0,0);
               if(temppool[p]->align!=1)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  addstr("Profile of an Automaton");
               }
               else
               {
                  set_color(COLOR_GREEN,COLOR_BLACK,1);
                  addstr("Profile of a Liberal");
               }

               if(page==0) printliberalstats(*temppool[p]);
               else if(page==1) printliberalskills(*temppool[p]);
               else if(page==2) printliberalcrimes(*temppool[p]);

               // Add removal of squad members member
               move(22,0);

               if(temppool[p]->is_active_liberal() &&
                  temppool[p]->hireid !=-1)  // If alive and not own boss? (suicide?)
               {
                  addstr("R - Remove member");
                  int boss = getpoolcreature(temppool[p]->hireid);
                  if(pool[boss]->location==temppool[p]->location)
                     addstr("         K - Kill member");
               }

               move(23,0);

               if(temppool[p]->align!=1) addstr("Press N to change this Automaton's Code Name");
               else addstr("N - Change Code Name      G - Fix Gender Label");
               if(len(temppool)>1) addstr("    LEFT/RIGHT - View Others");
               move(24,0);
               addstr("Press any other key to continue the Struggle");
               addstr("    UP/DOWN  - More Info");

               int c=getkey();

               if(len(temppool)>1&&((c==KEY_LEFT)||(c==KEY_RIGHT)))
               {
                  int sx=1;
                  if(c==KEY_LEFT) sx=-1;
                  p=(p+len(temppool)+sx)%len(temppool);
                  continue;
               }

               if(c==KEY_DOWN)
               {
                  page++;
                  if(page>2) page=0;
                  continue;
               }

               if(c==KEY_UP)
               {
                  page--;
                  if(page<0) page=2;
                  continue;
               }

               if(c=='n')
               {
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  mvaddstr(23,0,"What is the new code name?                                                      "); // 80 characters
                  mvaddstr(24,0,"                                                                                "); // 80 spaces

                  enter_name(24,0,temppool[p]->name,CREATURE_NAMELEN,temppool[p]->propername);
               }
               else if(c=='g' && temppool[p]->align==1)
               {
                  temppool[p]->gender_liberal++;
                  if(temppool[p]->gender_liberal > 2)
                     temppool[p]->gender_liberal = 0;
               }
               else if(c=='r' && temppool[p]->is_active_liberal() &&
                                 temppool[p]->hireid != -1)  // If alive and not own boss? (suicide?)
               {
                  int boss = getpoolcreature(temppool[p]->hireid);

                  move(22,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addstr("Do you want to permanently release this squad member from the LCS?              "); // 80 characters

                  move(23,0);
                  addstr("If the member has low heart they may go to the police.                          "); // 80 characters

                  move(24,0);
                  addstr("  C - Confirm       Any other key to continue                                   "); // 80 characters

                  int c=getkey();

                  if(c=='c')
                  {
                     // Release squad member
                     move(22,0);
                     addstr(temppool[p]->name, gamelog);
                     addstr(" has been released.                                                             ", gamelog); // 80 characters
                     move(23,0);
                     gamelog.newline(); //New line.
                     addstr("                                                                                "); // 80 spaces
                     move(24,0);
                     addstr("                                                                                "); // 80 spaces

                     getkey();

                     // Chance of member going to police if boss has criminal record and
                     // if they have low heart
                     // TODO: Do law check against other members?
                     if(temppool[p]->get_attribute(ATTRIBUTE_HEART,true) < temppool[p]->get_attribute(ATTRIBUTE_WISDOM,true)+ LCSrandom(5)
                        && iscriminal(*pool[boss]))
                     {
                        set_color(COLOR_CYAN,COLOR_BLACK,1);
                        move(22,0);
                        addstr("A Liberal friend tips you off on ", gamelog);
                        addstr(temppool[p]->name, gamelog);
                        addstr("'s whereabouts.", gamelog);
                        move(24,0);
                        gamelog.newline(); //New line.
                        addstr("The Conservative traitor has ratted you out to the police, and sworn", gamelog);
                        move(25,0);
                        gamelog.newline(); //New line.
                        addstr("to testify against ", gamelog);
                        addstr(pool[boss]->name, gamelog);
                        addstr(" in court.", gamelog);

                        criminalize(*pool[boss],LAWFLAG_RACKETEERING);
                        pool[boss]->confessions++;

                        // TODO: Depending on the crime increase heat or make seige

                        if(location[pool[boss]->base]->heat > 20)
                           location[pool[boss]->base]->siege.timeuntillocated=3;
                        else
                           location[pool[boss]->base]->heat += 20;
                     }
                     gamelog.nextMessage(); //Write out buffer to prepare for next message.

                     // Remove squad member
                     removesquadinfo(*temppool[p]);
                     cleangonesquads();

                     delete_and_remove(temppool,p,pool,getpoolcreature(temppool[p]->id));
                     break;
                  }
               }
               else if(c=='k' && temppool[p]->is_active_liberal() &&
                                 temppool[p]->hireid != -1)  // If alive and not own boss? (suicide?)
               {
                  // Kill squad member
                  int boss = getpoolcreature(temppool[p]->hireid);
                  if(pool[boss]->location!=temppool[p]->location) break;

                  move(22,0);
                  set_color(COLOR_WHITE,COLOR_BLACK,0);
                  addstr("Confirm you want to have "); // 25 characters (25+55=80)
                  addstr(pool[boss]->name);
                  addstr(" kill this squad member?                               "); // 55 characters (25+55=80)
                  move(23,0);
                  addstr("Killing your squad members is Not a Liberal Act.                                "); // 80 characters
                  move(24,0);
                  addstr("  C - Confirm       Any other key to continue                                   "); // 80 characters

                  int c=getkey();

                  if(c=='c')
                  {
                     temppool[p]->die();
                     cleangonesquads();
                     stat_kills++;

                     move(22,0);
                     addstr(pool[boss]->name, gamelog);
                     addstr(" executes ", gamelog); // 10 characters (10+4+66=80)
                     addstr(temppool[p]->name, gamelog);
                     addstr(" by ", gamelog); // 4 characters (10+4+66=80)
                     switch(LCSrandom(3))
                     {
                     case 0:addstr("strangling to death.                                              ", gamelog);break; // 66 characters (10+4+66=80)
                     case 1:addstr("beating to death.                                                 ", gamelog);break; // 66 characters (10+4+66=80)
                     case 2:addstr("cold execution.                                                   ", gamelog);break; // 66 characters (10+4+66=80)
                     }
                     move(23,0);
                     addstr("                                                                                "); // 80 spaces
                     move(24,0);
                     addstr("                                                                                "); // 80 spaces

                     getkey();

                     move(22,0);
                     if(boss!=-1)
                     {
                        if(LCSrandom(pool[boss]->get_attribute(ATTRIBUTE_HEART,false))>LCSrandom(3))
                        {
                           set_color(COLOR_GREEN,COLOR_BLACK,1);
                           gamelog.newline(); //New line.
                           addstr(pool[boss]->name, gamelog);
                           addstr(" feels sick to the stomach afterward and                                        ", gamelog); // 80 characters
                           pool[boss]->adjust_attribute(ATTRIBUTE_HEART,-1);
                           move(23,0); // this sentence probably takes more than 80 characters so use 2 lines and break it here
                           gamelog.newline(); //New line.
                           switch(LCSrandom(4))
                           {
                           case 0:addstr("throws up in a trash can.                                                       ", gamelog);break; // 80 characters
                           case 1:addstr("gets drunk, eventually falling asleep.                                          ", gamelog);break; // 80 characters
                           case 2:addstr("curls up in a ball, crying softly.                                              ", gamelog);break; // 80 characters
                           case 3:addstr("shoots up and collapses in a heap on the floor.                                 ", gamelog);break; // 80 characters
                           }
                           move(24,0);
                           gamelog.newline(); //New line.
                           addstr(pool[boss]->name, gamelog);
                           addstr(" has lost heart.                                                                ", gamelog); // 80 characters
                           getkey();
                        }
                        else if(!LCSrandom(3))
                        {
                           gamelog.newline(); //New line here too.
                           set_color(COLOR_CYAN,COLOR_BLACK,1);
                           addstr(pool[boss]->name, gamelog);
                           addstr(" grows colder.                                                                  ", gamelog); // 80 characters
                           pool[boss]->adjust_attribute(ATTRIBUTE_WISDOM,+1);
                           move(24,0);
                           gamelog.newline(); //New line.
                           addstr(pool[boss]->name, gamelog);
                           addstr(" has gained wisdom.                                                             ", gamelog); // 80 characters
                           getkey();
                        }
                     }
                     gamelog.nextMessage(); //Write buffer out to prepare for next message.

                     break;
                  }
               }
               else break;
            }
         }
      }

      if(c=='t')
      {
         sorting_prompt(reviewmodeenum_to_sortingchoiceenum(mode));
         sortliberals(temppool,activesortingchoice[reviewmodeenum_to_sortingchoiceenum(mode)],true);
      }

      // Reorder squad
      if(c=='z')
      {
         if(len(temppool)<=1) continue;

         move(22,0);
         addstr("                                                                                "); // 80 spaces
         move(23,0);
         addstr("                                                                                "); // 80 spaces

         move(22,8);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("Choose squad member to replace ");

         if(!swap) {
            int c=getkey();

            if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;

            if(c<'a'||c>'s') continue; // Not within correct range

            // Get first member to swap
            int p=page*19+c-'a';

            if(p<len(temppool)) swap=temppool[swapPos=p];
         }
         else { // non-null swap
            addstr(swap->name);
            addstr(" with");

            int c=getkey();

            if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;

            if(c<'a'||c>'s') continue; // Not within correct range

            Creature *swap2 = NULL;

            int p=page*19+c-'a';

            if(p<len(temppool) && temppool[p] != swap)
            {
               swap2=temppool[p];

               for(int i = 0; i < len(pool); i++)
                  if(pool[i]->id == swap->id)
                  {
                     pool.erase(pool.begin() + i);
                     break;
                  }

               for(int i = 0; i < len(pool); i++)
                  if(pool[i]->id == swap2->id)
                  {
                     pool.insert(pool.begin() + i + (swapPos < p), swap);
                     break;
                  }

               temppool.erase(temppool.begin() + swapPos);
               temppool.insert(temppool.begin() + p, swap);

               swap = NULL;
            }
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}



/* base - review - assemble a squad */
void assemblesquad(squadst *cursquad)
{
   int culloc=-1,p;
   if(cursquad!=NULL) culloc=cursquad->squad[0]->location;

   char newsquad=0;
   if(cursquad==NULL)
   {
      cursquad=new squadst;
      cursquad->id=cursquadid;
      cursquadid++;
      newsquad=1;
   }

   vector<Creature *> temppool;
   for(p=0;p<len(pool);p++)
      if(pool[p]->is_active_liberal() &&
        (pool[p]->location==culloc || culloc==-1))
      {
         temppool.push_back(pool[p]);
      }

   sortliberals(temppool,activesortingchoice[SORTINGCHOICE_ASSEMBLESQUAD]);

   //BUILD LIST OF BASES FOR EACH SQUAD IN CASE IT ENDS UP EMPTY
   //THEN WILL DROP ITS LOOT THERE
   vector<int> squadloc;
   squadloc.resize(len(squad));
   for(int sl=0;sl<len(squad);sl++)
   {
      squadloc[sl]=squad[sl]->squad[0]->location;
      if(squadloc[sl]!=-1) if(location[squadloc[sl]]->renting==RENTING_NOCONTROL)
         squadloc[sl]=-1;
   }

   int page=0,partysize;

   while(true)
   {
      partysize=squadsize(cursquad);

      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      if(partysize<6)addstr("Assemble the squad!");
      else addstr("The squad is full.");

      if(newsquad)
      {
         move(0,71);
         addstr("New Squad");
      }
      else
      {
         move(0,73-len(cursquad->name));
         addstr("Squad: ");
         addstr(cursquad->name);
      }

      move(1,0);
      addstr("ÄÄÄÄCODE NAMEÄÄÄÄÄÄÄÄÄÄÄÄSKILLÄÄÄHEALTHÄÄÄÄÄÄÄÄÄÄÄPROFESSIONÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"); // 80 characters

      int y=2;
      for(p=page*19;p<len(temppool)&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddchar(y,0,y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         int skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=(int)temppool[p]->get_skill(sk);
            if(temppool[p]->get_skill_ip(sk)>=100+(10*temppool[p]->get_skill(sk))&&
               temppool[p]->get_skill(sk)<temppool[p]->skill_cap(sk,true)) bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         addstr(skill);

         printhealthstat(*temppool[p],y,33,FALSE);

         if(temppool[p]->squadid==cursquad->id)
         {
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            move(y,75);
            addstr("SQUAD");
         }
         else if(temppool[p]->squadid!=-1)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,0);
            move(y,75);
            addstr("SQUAD");
         }
         else if(cursquad->squad[0]!=NULL)
         {
            if(cursquad->squad[0]->location!=temppool[p]->location)
            {
               set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(y,75);
               addstr("AWAY");
            }
         }

         if(temppool[p]->align==-1) set_color(COLOR_RED,COLOR_BLACK,1);
         else if(temppool[p]->align==0) set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(y,50);
         addstr(temppool[p]->get_type_name());
         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to add or remove a Liberal from the squad.");
      move(23,0);
      addpagestr();
      addstr(" T to sort people.");
		move(23,50);
      addstr("V - View a Liberal");
      move(24,0);
      if(partysize>0) addstr("Enter - The squad is ready.");
      else addstr("Enter - I need no squad!");
      if(partysize>0) set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(24,40);
      addstr("9 - Dissolve the squad.");

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(temppool)) page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+c-'a';
         if(p<len(temppool))
         {
            char conf=1;
            if(cursquad->squad[0]!=NULL)
            {
               if(cursquad->squad[0]->location!=temppool[p]->location)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  move(22,0);
                  addstr("                                                                                "); // 80 spaces
                  move(23,0);
                  addstr("           Liberals must be in the same location to form a Squad.               "); // 80 characters
                  move(24,0);
                  addstr("                                                                                "); // 80 spaces

                  getkey();

                  conf=0;
               }
            }
            if(!temppool[p]->canwalk()&&
               !(temppool[p]->flag & CREATUREFLAG_WHEELCHAIR))
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               move(22,0);
               addstr("                                                                                "); // 80 spaces
               move(23,0);
               addstr("                Squad Liberals must be able to move around.                     "); // 80 characters
               move(24,0);
               addstr("                                                                                "); // 80 spaces

               getkey();

               conf=0;
            }
            if(conf)
            {
               if(temppool[p]->squadid==cursquad->id)
               {
                  bool flipstart=0;
                  for(int pt=0;pt<6;pt++)
                  {
                     if(cursquad->squad[pt]==temppool[p])
                     {
                        flipstart=1;
                        cursquad->squad[pt]->squadid=-1;
                     }
                     if(flipstart&&pt<5)cursquad->squad[pt]=cursquad->squad[pt+1];
                  }
                  if(flipstart)cursquad->squad[5]=NULL;
               }
               else if(partysize<6)
               {
                  for(int pt=0;pt<6;pt++)
                  {
                     if(cursquad->squad[pt]==NULL)
                     {
                        removesquadinfo(*temppool[p]);
                        cursquad->squad[pt]=temppool[p];
                        temppool[p]->squadid=cursquad->id;
                        break;
                     }
                  }
               }
            }
         }
      }
      if(c=='t')
      {
         sorting_prompt(SORTINGCHOICE_ASSEMBLESQUAD);
         sortliberals(temppool,activesortingchoice[SORTINGCHOICE_ASSEMBLESQUAD],true);
      }
		if(c=='v')
		{
			move(22,0);
			set_color(COLOR_WHITE,COLOR_BLACK,1);
			addstr("Press a Letter to view Liberal details.                                         "); // 80 characters
			move(23,0);
			addstr("                                                                                "); // 80 spaces
			move(24,0);
			addstr("                                                                                "); // 80 spaces
			int c2=getkey();
			if(c2>='a'&&c2<='s')
			{
				int p=page*19+c2-'a';
            if(p<len(temppool))
            {
               //Create a temporary squad from which to view this character - even if they already have a squad.
               squadst *oldactivesquad = activesquad;
               int oldSquadID = temppool[p]->squadid;
               //create a temp squad containing just this liberal
               activesquad=new squadst;
               strcpy(activesquad->name, "Temporary Squad");
               activesquad->id=cursquadid;
               activesquad->squad[0]=temppool[p];
               temppool[p]->squadid = activesquad->id;
               fullstatus(0);
               delete_and_nullify(activesquad);
               temppool[p]->squadid = oldSquadID;
               activesquad = oldactivesquad;
            }
			}
		}
      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR)
      {
         //CHECK IF GOOD (either has at least one Liberal or is empty)
         bool good=true; // Start off at true for empty squads

         for(int p=0;p<6;p++)
            if(cursquad->squad[p])
            {  // It is good if either there is at least one Liberal, or if the squad is completely empty
               if(cursquad->squad[p]->align==1)
               {  good=true; break; } // We found a Liberal, it's good
               else good=false; // We found a non-Liberal, this is bad unless we can find a Liberal too
            }

         if(good) break;
         else
         {  // At this point we have a non-empty squad, none of whose members are Liberal
            set_color(COLOR_RED,COLOR_BLACK,1);

            move(22,0);
            addstr("                                                                                "); // 80 spaces
            move(23,0);
            addstr("You cannot form a Squad with only Conservatives!                                "); // 80 characters
            move(24,0);
            addstr("                                                                                "); // 80 spaces

            getkey();
         }
      }
      if(c=='9')
      {
         for(int p=0;p<6;p++)
         {
            if(cursquad->squad[p]!=NULL)
            {
               cursquad->squad[p]->squadid=-1;
               cursquad->squad[p]=NULL;
            }
         }
      }
   }

   //FINALIZE NEW SQUADS
   bool hasmembers=squadsize(cursquad)>0;

   if(newsquad)
   {
      if(hasmembers)
      {
         move(22,0);
         addstr("                                                                                "); // 80 spaces
         move(23,0);
         addstr("What shall we designate this Liberal squad?                                     "); // 80 characters
         move(24,0);
         addstr("                                                                                "); // 80 spaces

         enter_name(24,0,cursquad->name,SQUAD_NAMELEN,"The Liberal Crime Squad");

         squad.push_back(cursquad);
      }
      else delete cursquad;
   }

   //NUKE ALL EMPTY SQUADS
   for(int sq=len(squad)-1;sq>=0;sq--)
   {
      hasmembers=false;

      for(int p=0;p<6;p++)
         if(squad[sq]->squad[p]!=NULL)
         {  hasmembers=true; break; }

      if(!hasmembers && mode==GAMEMODE_BASE)
      {
         if(squadloc[sq]!=-1)
            location[squadloc[sq]]->getloot(squad[sq]->loot);

         if(activesquad==squad[sq])activesquad=NULL;
         delete_and_remove(squad,sq);
      }
   }
}



/* base - review - assign new bases to the squadless */
void squadlessbaseassign()
{
   int p=0,l=0,page_lib=0,page_loc=0,selectedbase=0;
   vector<Creature *> temppool;
   for(p=0;p<len(pool);p++) if(pool[p]->is_active_liberal()&&pool[p]->squadid==-1) temppool.push_back(pool[p]);

   if(!len(temppool)) return;
   sortliberals(temppool,activesortingchoice[SORTINGCHOICE_BASEASSIGN]);

   vector<int> temploc;
   for(l=0;l<len(location);l++) if(location[l]->renting>=0&&!location[l]->siege.siege) temploc.push_back(l);
   if(!len(temploc)) return;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds();

      move(0,0);
      addstr("New Bases for Squadless Liberals");
      move(1,0);
      addstr("ÄÄÄÄCODE NAMEÄÄÄÄÄÄÄÄÄÄÄÄCURRENT BASEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"); // 80 characters
      move(1,51);
      addstr("NEW BASE");

      int y=2;
      for(p=page_lib*19;p<len(temppool)&&p<page_lib*19+19;p++,y++)
      {
         // Red name if location under siege
         if(temppool[p]->base == temppool[p]->location &&
            location[temppool[p]->base]->siege.siege)
            set_color(COLOR_RED,COLOR_BLACK,1);
         else if(multipleCityMode && location[temppool[p]->base]->city != location[temploc[selectedbase]]->city)
            set_color(COLOR_BLACK,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddchar(y,0,y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         mvaddstr(y,25,location[temppool[p]->base]->getname(true, true));
         if(location[temppool[p]->base]->siege.siege)
            addstr(" <Under Siege>");
      }

      y=2;
      for(p=page_loc*9;p<len(temploc)&&p<page_loc*9+9;p++,y++)
      {
         int color = COLOR_WHITE;

         if(p==selectedbase)set_color(color,COLOR_BLACK,1);
         else set_color(color,COLOR_BLACK,0);
         mvaddchar(y,51,y+'1'-2);addstr(" - ");
         addstr(location[temploc[p]]->getname(true, true));
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      mvaddstr(21,0,"Press a Letter to assign a Base.  Press a Number to select a Base.");
      mvaddstr(22,0,"Liberals must be moved in squads to transfer between cities.");
      if(len(temppool)>19)
      {
         move(23,0);
         addpagestr();
      }
      if(len(temploc)>9)
      {
         move(24,0);
         addstr(",. to view other Base pages.");
      }
      move(23,35);
      addstr("T to sort people.");

      int c=getkey();

      //PAGE UP (people)
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page_lib>0) page_lib--;
      //PAGE DOWN (people)
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page_lib+1)*19<len(temppool)) page_lib++;

      //PAGE UP (locations)
      if(c==','&&page_loc>0) page_loc--;
      //PAGE DOWN (locations)
      if(c=='.'&&(page_loc+1)*9<len(temploc)) page_loc++;

      if(c>='a'&&c<='s')
      {
         int p=page_lib*19+c-'a';

         // Assign new base, IF the selected letter is a liberal, AND the Liberal is not under siege or in a different city
         if(p<len(temppool)
            && !(temppool[p]->base == temppool[p]->location && location[temppool[p]->base]->siege.siege)
            && !(multipleCityMode && location[temppool[p]->base]->city != location[temploc[selectedbase]]->city))
         {
            temppool[p]->base=temploc[selectedbase];
         }
      }
      if(c>='1'&&c<='9')
      {
         int p=page_loc*9+c-'1';
         if(p<len(temploc)) selectedbase=p;
      }
      if(c=='t')
      {
         sorting_prompt(SORTINGCHOICE_BASEASSIGN);
         sortliberals(temppool,activesortingchoice[SORTINGCHOICE_BASEASSIGN],true);
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}

// prints a formatted name, used by promoteliberals
static void printname(Creature &cr)
{
   int bracketcolor=-1,namecolor,brightness;

   if(cr.hiding)
      bracketcolor=COLOR_BLACK;

   // Determine bracket color, if any, based on location
   if(cr.location!=-1)
   {
      switch(location[cr.location]->type)
      {
      case SITE_GOVERNMENT_POLICESTATION:
      case SITE_GOVERNMENT_COURTHOUSE:
         if(!(cr.flag & CREATUREFLAG_SLEEPER))
            bracketcolor=COLOR_YELLOW;
         break;
      case SITE_GOVERNMENT_PRISON:
         if(!(cr.flag & CREATUREFLAG_SLEEPER))
            bracketcolor=COLOR_RED;
         break;
      default:
         break;
      }
   }

   // Determine name color, based on recruitment style
   if(cr.flag & CREATUREFLAG_LOVESLAVE)
      namecolor=COLOR_MAGENTA;
   else if(cr.flag & CREATUREFLAG_BRAINWASHED)
      namecolor=COLOR_YELLOW;
   else namecolor=COLOR_WHITE;

   // Determine name brightness, based on subordinates left
   /*if(subordinatesleft(cr))
   brightness=1;
   else*/
   brightness=0;

   // add bracket (if used)
   if(bracketcolor!=-1)
   {
      set_color(bracketcolor,COLOR_BLACK,1);
      addstr("[");
   }

   if(cr.flag & CREATUREFLAG_SLEEPER)
   {
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr("[");
   }

   // add name
   set_color(namecolor,COLOR_BLACK,brightness);
   addstr(cr.name);

   // add close bracket (if used)
   if(cr.flag & CREATUREFLAG_SLEEPER)
   {
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr("]");
   }
   if(bracketcolor!=-1)
   {
      set_color(bracketcolor,COLOR_BLACK,1);
      addstr("]");
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
}

/* base - review - promote liberals */
void promoteliberals()
{
   #define PAGELENGTH 19
   vector<Creature *> temppool;
   vector<int> level;
   for(int p=0;p<len(pool);p++)
      if(pool[p]->alive&&pool[p]->align==1)
         temppool.push_back(pool[p]);

   if(!len(temppool)) return;

   //SORT
   sortbyhire(temppool,level);

   //PROMOTE
   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds();

      move(0,0);
      addstr("Promote the Elite Liberals");
      move(1,0);
      addstr("ÄÄÄÄCODE NAMEÄÄÄÄÄÄÄÄÄÄÄÄÄÄCURRENT CONTACTÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"); // 80 characters
      move(1,54);
      addstr("CONTACT AFTER PROMOTION");

      int y=2;

      for(int p=page*PAGELENGTH;p<len(temppool)&&p<page*PAGELENGTH+PAGELENGTH;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddchar(y,0,y+'A'-2);addstr(" - ");

         move(y,27);
         int p2 = 0;

         for(p2=0;p2<len(pool);p2++)
         {
            int p3 = 0;
            if(pool[p2]->alive==1&&pool[p2]->id==temppool[p]->hireid)
            {
               printname(*pool[p2]);

               move(y,54);
               for(p3=0;p3<len(pool);p3++)
               {
                  if(pool[p3]->alive==1&&pool[p3]->id==pool[p2]->hireid)
                  {
                     if(temppool[p]->flag&CREATUREFLAG_LOVESLAVE)
                        addstr("<Refuses Promotion>");
                     else if(!subordinatesleft(*pool[p3])&&!(temppool[p]->flag&CREATUREFLAG_BRAINWASHED))
                        addstr("<Can't Lead More>");
                     else
                        printname(*pool[p3]);
                     break;
                  }
               }

               break;
            }
         }
         if(p2==len(pool)) addstr("<LCS Leader>");

         move(y++,4+level[p]);
         printname(*temppool[p]);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(21,0);
      addstr("Recruited/");
      set_color(COLOR_MAGENTA,COLOR_BLACK,0);
      addstr("Seduced");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("/");
      set_color(COLOR_YELLOW,COLOR_BLACK,0);
      addstr("Enlightened");
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      addstr("   [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Arrested");
      set_color(COLOR_YELLOW,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_RED,COLOR_BLACK,1);
      addstr(" [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("In Jail");
      set_color(COLOR_RED,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("In Hiding");
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr(" [");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Sleeper");
      set_color(COLOR_BLUE,COLOR_BLACK,1);
      addstr("]");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a letter to promote a Liberal. You cannot promote Liberals in hiding.");
      move(23,0);
      addstr("Enlightened Liberals follow anyone. Seduced Liberals follow only their lover.");
      if(len(temppool)>PAGELENGTH)
      {
         move(24,0);
         addpagestr();
      }

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*PAGELENGTH<len(temppool)) page++;

      if(c>='a'&&c<='a'+PAGELENGTH)
      {
         int p=page*PAGELENGTH+(int)(c-'a');
         // *JDS* can't promote liberals in hiding OR loveslaves
         if(p<len(temppool)&&!temppool[p]->hiding&&!(temppool[p]->flag&CREATUREFLAG_LOVESLAVE))
         {
            for(int p2=0;p2<len(pool);p2++)
            {
               if(pool[p2]->alive==1&&pool[p2]->id==temppool[p]->hireid)
               {
                  addstr(pool[p2]->name);

                  for(int p3=0;p3<len(pool);p3++)
                  {
                     // Can't promote if new boss can't accept more subordinates
                     if(pool[p3]->alive==1&&pool[p3]->id==pool[p2]->hireid&&
                        (temppool[p]->flag&CREATUREFLAG_BRAINWASHED||subordinatesleft(*pool[p3])))
                     {
                        temppool[p]->hireid=pool[p2]->hireid;
                        sortbyhire(temppool,level);
                        break;
                     }
                  }
                  break;
               }
            }
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}



void sortbyhire(vector<Creature *> &temppool,vector<int> &level)
{
   vector<Creature *> newpool;
   level.clear();

   for(int i=len(temppool)-1;i>=0;i--)
      if(temppool[i]->hireid==-1)
      {
         newpool.insert(newpool.begin(),temppool[i]);
         level.insert(level.begin(),0);
         temppool.erase(temppool.begin() + i);
      }

   bool changed;
   do
   {
      changed=false;

      for(int i=0;i<len(newpool);i++)
         for(int j=len(temppool)-1;j>=0;j--)
            if(temppool[j]->hireid==newpool[i]->id)
            {
               newpool.insert(newpool.begin()+i+1,temppool[j]);
               level.insert(level.begin()+i+1,level[i]+1);
               temppool.erase(temppool.begin() + j);
               changed=true;
            }
   } while(changed);

   temppool.clear();
   for(int p=0;p<len(newpool);p++)
      temppool.push_back(newpool[p]);
}
