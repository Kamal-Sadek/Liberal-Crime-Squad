/*
 * Copyright (c) 2002,2003,2004 by Tarn Adams
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/*
 * This file was created by Chris Johnson (grundee@users.sourceforge.net)
 * by copying code from game.cpp into monthly/endgame.cpp.
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
#include <sstream>

vector<Creature *> activatable_liberals()
{
   vector<Creature *> temppool;
   for(int p=0;p<len(pool);p++) if(pool[p]->is_active_liberal())
   {
      if(pool[p]->squadid!=-1)
      {
         int sq=getsquad(pool[p]->squadid);
         if(sq!=-1) if(squad[sq]->activity.type!=ACTIVITY_NONE) continue;
      }
      temppool.push_back(pool[p]);
   }
   return temppool;
}

/* base - activate the uninvolved */
void activate()
{
   vector<Creature *> temppool = activatable_liberals();

   if(!len(temppool)) return;

   sortliberals(temppool,activesortingchoice[SORTINGCHOICE_ACTIVATE]);

   int page=0;

   while(true)
   {
      music.play(MUSIC_ACTIVATE);
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds();

      move(0,0);
      addstr("Activate Uninvolved Liberals");
      move(1,0);
      addstr("ÄÄÄÄCODE NAMEÄÄÄÄÄÄÄÄÄÄÄÄSKILLÄÄÄHEALTHÄÄÄLOCATIONÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
      move(1,57);
      addstr("ACTIVITY");

      int y=2;
      for(int p=page*19;p<len(temppool)&&p<page*19+19;p++,y++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         int skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=temppool[p]->get_skill(sk);
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
         addstr(location[temppool[p]->location]->getname(true, true));

         move(y,57);
         // Let's add some color here...
         set_activity_color(temppool[p]->activity.type);
         addstr(getactivity(temppool[p]->activity));
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to Assign an Activity.");
      move(23,0);
      addpagestr();
      addstr(" T to sort people.");
      move(24,0);
      addstr("Press Z to assign simple tasks in bulk.");

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(temppool)) page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+c-'a';
         if(p<len(temppool)) activate(temppool[p]);
      }

      if(c=='t')
      {
         sorting_prompt(SORTINGCHOICE_ACTIVATE);
         sortliberals(temppool,activesortingchoice[SORTINGCHOICE_ACTIVATE],true);
      }

      if(c=='z') activatebulk();

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}

int classlist=0;
void listclasses(Creature *cr)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(10,40);
   addstr("Classes cost $60 a day. Study what?");
   if(classlist==0)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_DEBATING);
      move(12,40);
      addstr("1 - Public Policy");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_BUSINESS);
      move(13,40);
      addstr("2 - Economics");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_PSYCHOLOGY);
      move(14,40);
      addstr("3 - Psychology");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_LAW);
      move(15,40);
      addstr("4 - Criminal Law");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_SCIENCE);
      move(16,40);
      addstr("5 - Physics");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(17,40);
      addstr("6 - Other classes");
   }
   else if(classlist==1)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_DRIVING);
      move(12,40);
      addstr("1 - Drivers Ed");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_FIRST_AID);
      move(13,40);
      addstr("2 - First Aid");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_ART);
      move(14,40);
      addstr("3 - Painting");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_DISGUISE);
      move(15,40);
      addstr("4 - Theatre");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_MARTIAL_ARTS);
      move(16,40);
      addstr("5 - Kung Fu");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(17,40);
      addstr("6 - Other classes");
   }
   else if(classlist==2)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_GYMNASTICS);
      move(12,40);
      addstr("1 - Gymnastics");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_WRITING);
      move(13,40);
      addstr("2 - Writing");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_TEACHING);
      move(14,40);
      addstr("3 - Education");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_MUSIC);
      move(15,40);
      addstr("4 - Music");

      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_LOCKSMITHING);
      move(16,40);
      addstr("5 - Locksmithing");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(17,40);
      addstr("6 - Other classes");
   }
   else if(classlist==3)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STUDY_COMPUTERS);
      move(12,40);
      addstr("1 - Computers");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(17,40);
      addstr("6 - Other classes");
   }
}

void updateclasschoice(Creature *cr, char choice)
{
   if(choice=='6')
   {
      classlist=(classlist+1)%4;
      listclasses(cr);
   }
   else
   {
      if(classlist==0)
      {
         switch(choice)
         {
            case '1':cr->activity.type=ACTIVITY_STUDY_DEBATING;break;
            case '2':cr->activity.type=ACTIVITY_STUDY_BUSINESS;break;
            case '3':cr->activity.type=ACTIVITY_STUDY_PSYCHOLOGY;break;
            case '4':cr->activity.type=ACTIVITY_STUDY_LAW;break;
            case '5':cr->activity.type=ACTIVITY_STUDY_SCIENCE;break;
         }
      }
      else if(classlist==1)
      {
         switch(choice)
         {
            case '1':cr->activity.type=ACTIVITY_STUDY_DRIVING;break;
            case '2':cr->activity.type=ACTIVITY_STUDY_FIRST_AID;break;
            case '3':cr->activity.type=ACTIVITY_STUDY_ART;break;
            case '4':cr->activity.type=ACTIVITY_STUDY_DISGUISE;break;
            case '5':cr->activity.type=ACTIVITY_STUDY_MARTIAL_ARTS;break;
         }
      }
      else if(classlist==2)
      {
         switch(choice)
         {
            case '1':cr->activity.type=ACTIVITY_STUDY_GYMNASTICS;break;
            case '2':cr->activity.type=ACTIVITY_STUDY_WRITING;break;
            case '3':cr->activity.type=ACTIVITY_STUDY_TEACHING;break;
            case '4':cr->activity.type=ACTIVITY_STUDY_MUSIC;break;
            case '5':cr->activity.type=ACTIVITY_STUDY_LOCKSMITHING;break;
         }
      }
      else if(classlist==3)
      {
         switch(choice)
         {
            case '1':cr->activity.type=ACTIVITY_STUDY_COMPUTERS;break;
         }
      }
      listclasses(cr);
   }
}

void activate(Creature *cr)
{
   int hostagecount=0,state=0,oldstate=0,choice=0;
   char havedead=0;
   for(int p=0;p<len(pool);p++)
   {
      if (pool[p]->alive && pool[p]->align != Alignment::LIBERAL && pool[p]->location==cr->location)
        hostagecount++;
      if (!pool[p]->alive)
        havedead=1;
   }

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds();

      move(0,0);
      if(cr->income)
      {
         addstr(cr->name);
         addstr(" made $");
         addstr(cr->income);
         addstr(" yesterday. What now?");
      }
      else
      {
         addstr("Taking Action: What will ");
         addstr(cr->name);
         addstr(" be doing today?");
      }

      printcreatureinfo(cr);

      makedelimiter();

      if(!state) switch(cr->activity.type)
      {
      case ACTIVITY_COMMUNITYSERVICE:
      case ACTIVITY_TROUBLE:
      case ACTIVITY_GRAFFITI:
      case ACTIVITY_POLLS:
      case ACTIVITY_DOS_ATTACKS:
      case ACTIVITY_HACKING:
      case ACTIVITY_WRITE_LETTERS:
      case ACTIVITY_WRITE_GUARDIAN:
         state='a';
         break;
      case ACTIVITY_DONATIONS:
      case ACTIVITY_SELL_TSHIRTS:
      case ACTIVITY_SELL_ART:
      case ACTIVITY_SELL_MUSIC:
         state='b';
         break;
      case ACTIVITY_SELL_DRUGS:
      case ACTIVITY_PROSTITUTION:
      case ACTIVITY_CCFRAUD:
      case ACTIVITY_DOS_RACKET:
         state='c';
         break;
      case ACTIVITY_RECRUITING:
      case ACTIVITY_MAKE_ARMOR:
      case ACTIVITY_REPAIR_ARMOR:
      case ACTIVITY_STEALCARS:
      case ACTIVITY_WHEELCHAIR:
      case ACTIVITY_AUGMENT:
         state='d';
         break;
      case ACTIVITY_TEACH_POLITICS:
      case ACTIVITY_TEACH_COVERT:
      case ACTIVITY_TEACH_FIGHTING:
         state='t';
         break;
      case ACTIVITY_HOSTAGETENDING:
         state='i';
         break;
      case ACTIVITY_STUDY_DEBATING:
      case ACTIVITY_STUDY_MARTIAL_ARTS:
      case ACTIVITY_STUDY_DRIVING:
      case ACTIVITY_STUDY_PSYCHOLOGY:
      case ACTIVITY_STUDY_FIRST_AID:
      case ACTIVITY_STUDY_LAW:
      case ACTIVITY_STUDY_DISGUISE:
      case ACTIVITY_STUDY_SCIENCE:
      case ACTIVITY_STUDY_BUSINESS:
      //case ACTIVITY_STUDY_COOKING:
      case ACTIVITY_STUDY_GYMNASTICS:
      case ACTIVITY_STUDY_WRITING:
      case ACTIVITY_STUDY_ART:
      case ACTIVITY_STUDY_MUSIC:
      case ACTIVITY_STUDY_TEACHING:
      case ACTIVITY_STUDY_LOCKSMITHING:
      case ACTIVITY_STUDY_COMPUTERS:
         state='l';
         break;
      case ACTIVITY_CLINIC:
         state='m';
         break;
      case ACTIVITY_HEAL:
         state='h';
         break;
      case ACTIVITY_BURY:
         state='z';
         break;
      case ACTIVITY_NONE:
         state='x';
         break;
      }
      oldstate=state;

      set_color(COLOR_WHITE,COLOR_BLACK,state=='a');
      mvaddstr(10,1,"A - Engaging in Liberal Activism");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='b');
      mvaddstr(11,1,"B - Legal Fundraising");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='c');
      mvaddstr(12,1,"C - Illegal Fundraising");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='d');
      mvaddstr(13,1,"D - Recruitment and Acquisition");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='t');
      mvaddstr(14,1,"T - Teaching Other Liberals");

      if(hostagecount>0)set_color(COLOR_WHITE,COLOR_BLACK,state=='i');
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      mvaddstr(15,1,"I - Tend to a Conservative hostage");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='l');
      mvaddstr(16,1,"L - Learn in the University District");

      if(clinictime(*cr)) set_color(COLOR_WHITE,COLOR_BLACK,state=='m');
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      mvaddstr(17,1,"M - Move to the Free Clinic");

      if(cr->get_skill(SKILL_FIRSTAID)!=0)
         set_color(COLOR_WHITE,COLOR_BLACK,state=='h');
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      mvaddstr(18,1,"H - Heal Liberals");

      if(havedead)set_color(COLOR_WHITE,COLOR_BLACK,state=='z');
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      mvaddstr(19,1,"Z - Dispose of bodies");

      siegest *siege=NULL;
      if(selectedsiege!=-1) siege=&location[selectedsiege]->siege;
      if(activesquad) if(activesquad->squad[0]->location!=-1) siege=&location[activesquad->squad[0]->location]->siege;
      char sieged=0;
      if(siege) sieged=siege->siege;
      /*char underattack=0;
      if(siege&&sieged) underattack=siege->underattack;*/

      if(!sieged)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      mvaddstr(20,1,"E - Equip this Liberal");

      if(state=='a'||state=='b'||state=='c'||state=='d')
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(20,40,"? - Help");
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      mvaddstr(21,40,"Enter - Confirm Selection");

      set_color(COLOR_WHITE,COLOR_BLACK,state=='x');
      mvaddstr(21,1,"X - Nothing for Now");

      switch(state)
      {
      case 'a':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_COMMUNITYSERVICE);
         mvaddstr(10,40,"1 - Community Service");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TROUBLE);
         mvaddstr(11,40,"2 - Liberal Disobedience");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_GRAFFITI);
         mvaddstr(12,40,"3 - Graffiti");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_POLLS);
         mvaddstr(13,40,"4 - Search Opinion Polls");

         //set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_DOS_ATTACKS);
         //mvaddstr(14,40,"5 - Harass Websites");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_HACKING);
         mvaddstr(14,40,"5 - Hacking");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_WRITE_LETTERS);
         mvaddstr(15,40,"6 - Write to Newspapers");

         if(cr->location!=-1&&
            location[cr->location]->compound_walls & COMPOUND_PRINTINGPRESS)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_WRITE_GUARDIAN);
            mvaddstr(16,40,"7 - Write for The Liberal Guardian");
         }
         break;
      case 'b':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_DONATIONS);
         mvaddstr(10,40,"1 - Solicit Donations");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_TSHIRTS);
         move(11,40);
         if(cr->get_skill(SKILL_TAILORING)>=8)
            addstr("2 - Sell Liberal T-Shirts");
         else if(cr->get_skill(SKILL_TAILORING)>=4)
            addstr("2 - Sell Embroidered Shirts");
         else
            addstr("2 - Sell Tie-Dyed T-Shirts");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_ART);
         move(12,40);
         if(cr->get_skill(SKILL_ART)>=8)
            addstr("3 - Sell Liberal Art");
         else if(cr->get_skill(SKILL_ART)>=4)
            addstr("3 - Sell Paintings");
         else
            addstr("3 - Sell Portrait Sketches");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_MUSIC);
         move(13,40);
         if(cr->get_skill(SKILL_MUSIC)>8)
            addstr("4 - Play Liberal Music");
         else
            addstr("4 - Play Street Music");
         break;
      case 'c':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SELL_DRUGS);
         mvaddstr(10,40,"1 - Sell Brownies");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_PROSTITUTION);
#ifndef ZEROMORAL
         if(cr->age < 18)
            set_color(COLOR_BLACK, COLOR_BLACK, 1);    //Grayed out for minors
#endif
         mvaddstr(11,40,"2 - Prostitution");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_CCFRAUD);
         mvaddstr(12,40,"3 - Steal Credit Card Numbers");

         /*set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_DOS_RACKET);
         mvaddstr(13,40,"4 - Electronic Protection Racket");*/
         break;
      case 'd':
         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_RECRUITING);
         mvaddstr(10,40,"1 - Recruiting");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_MAKE_ARMOR);
         mvaddstr(11,40,"2 - Make Clothing");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_REPAIR_ARMOR);
         mvaddstr(12,40,"3 - Repair Clothing");

         if(cr->canwalk()) {
            set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_STEALCARS);
            mvaddstr(13,40,"4 - Steal a Car");
         } else {
            if(!(cr->flag&CREATUREFLAG_WHEELCHAIR)) set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_WHEELCHAIR);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            mvaddstr(13,40,"4 - Procure a Wheelchair");
         }

         if(cr->get_skill(SKILL_SCIENCE)!=0)
            set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_AUGMENT);
         else
            set_color(COLOR_BLACK,COLOR_BLACK,1);
         mvaddstr(14,40,"5 - Augment a Liberal");

         break;
      case 't':
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,40);
         addstr("Teach Liberals About What?");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_POLITICS);
         move(12,40);
         addstr("1 - Political Activism");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_COVERT);
         move(13,40);
         addstr("2 - Infiltration");

         set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_TEACH_FIGHTING);
         move(14,40);
         addstr("3 - Urban Warfare");
         break;
      case 'l':
         listclasses(cr);
         break;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      switch(cr->activity.type)
      {
      case ACTIVITY_COMMUNITYSERVICE:
         move(22,3);
         addstr(cr->name);
         addstr(" will help the elderly, local library, anything");
         move(23,1);
         addstr("  that is Liberal.");
         break;
      case ACTIVITY_TROUBLE:
         move(22,3);
         addstr(cr->name);
         addstr(" will create public disturbances. ");
         break;
      case ACTIVITY_GRAFFITI:
         move(22,3);
         addstr(cr->name);
         addstr(" will spray political graffiti. Art and Heart will");
         move(23,1);
         addstr("  enhance the liberal effect.");
         break;
      case ACTIVITY_POLLS:
         move(22,3);
         addstr(cr->name);
         addstr(" will search the internet for public opinion polls.");
         move(23,1);
         addstr("  Polls will give an idea on how the liberal agenda is going. Computers");
         move(24,1);
         addstr("  and intelligence will provide better results.");
         break;
      case ACTIVITY_DOS_ATTACKS:
         move(22,3);
         addstr(cr->name);
         addstr(" will harass Conservative websites. Computer skill");
         move(23,1);
         addstr("  will give greater effect.");
         break;
      case ACTIVITY_HACKING:
         move(22,3);
         addstr(cr->name);
         addstr(" will harass websites and hack private networks.");
         move(23,1);
         addstr("  Computer skill and intelligence will give more frequent results.");
         move(24,1);
         addstr("  Multiple hackers will increase chances of both success and detection.");
         break;
      case ACTIVITY_WRITE_LETTERS:
         move(22,3);
         addstr(cr->name);
         addstr(" will write letters to newspapers about current events.");
         break;
      case ACTIVITY_WRITE_GUARDIAN:
         move(22,3);
         addstr(cr->name);
         addstr(" will write articles for the LCS's newspaper.");
         break;
      case ACTIVITY_DONATIONS:
         move(22,3);
         addstr(cr->name);
         addstr(" will walk around and ask for donations to the LCS.");
         move(23,1);
         addstr("  Based on persuasion, public's view on the cause, and how well dressed the");
         move(24,1);
         addstr("  activist is.");
         break;
      case ACTIVITY_SELL_TSHIRTS:
         move(22,3);
         addstr(cr->name);
         if(cr->get_skill(SKILL_TAILORING)>=8)
            addstr(" will print and distribute shirts with Liberal slogans.");
         else if(cr->get_skill(SKILL_TAILORING)>=4)
            addstr(" will embroider shirts and sell them on the street.");
         else addstr(" will tie-dye T-shirts and sell them on the street.");
         break;
      case ACTIVITY_SELL_ART:
         move(22,3);
         addstr(cr->name);
         if(cr->get_skill(SKILL_ART)>=8)
            addstr(" will create and sell paintings embracing the Liberal agenda.");
         else if(cr->get_skill(SKILL_ART)>=4)
            addstr(" will make pretty paintings and sell them on the streets.");
         else addstr(" will sketch people and sell portraits back to them.");
         break;
      case ACTIVITY_SELL_MUSIC:
         move(22,3);
         addstr(cr->name);
         addstr(" will go out into the streets and drum on buckets,");
         move(23,1);
         addstr("  or play guitar if one is equipped.");
         break;
      case ACTIVITY_RECRUITING:
         move(22,3);
         addstr(cr->name);
         addstr(" will try to find someone to join the LCS.");
         break;
      case ACTIVITY_STEALCARS:
         move(22,3);
         addstr(cr->name);
         addstr(" will try to find and steal a car.");
         break;
      case ACTIVITY_SELL_DRUGS:
         move(22,3);
         addstr(cr->name);
         addstr(" will bake and sell special adult brownies that open");
         move(23,1);
         addstr("  magical shimmering doorways to the adamantium pits.");
         break;
      case ACTIVITY_PROSTITUTION:
         move(22,3);
         addstr(cr->name);
         addstr(" will trade sex for money.");
         break;
      case ACTIVITY_CCFRAUD:
         move(22,3);
         addstr(cr->name);
         addstr(" will commit credit card fraud online.");
         break;
      case ACTIVITY_DOS_RACKET:
         move(22,3);
         addstr(cr->name);
         addstr(" will demand money in exchange for not bringing down");
         move(23,1);
         addstr("major websites.");
         break;
      case ACTIVITY_TEACH_POLITICS:
         move(22,1);
         addstr("  Skills Trained: Writing, Persuasion, Law, Street Sense, Science,");
         move(23,1);
         addstr("    Religion, Business, Music, Art");
         move(24,1);
         addstr("  Classes cost up to $20/day to conduct. All Liberals able will attend.");
         break;
      case ACTIVITY_TEACH_COVERT:
         move(22,1);
         addstr("  Skills Trained: Computers, Security, Stealth, Disguise, Tailoring,");
         move(23,1);
         addstr("    Seduction, Psychology, Driving");
         move(24,1);
         addstr("  Classes cost up to $60/day to conduct. All Liberals able will attend.");
         break;
      case ACTIVITY_TEACH_FIGHTING:
         move(22,1);
         addstr("  Skills Trained: All Weapon Skills, Martial Arts, Dodge, First Aid");
         move(24,1);
         addstr("  Classes cost up to $100/day to conduct. All Liberals able will attend.");
         break;
      case ACTIVITY_AUGMENT:
         move(22,3);
         addstr(cr->name);
         addstr(" will augment another Liberal to make them");
         move(23,1);
         addstr("physically superior.");
         break;
      case ACTIVITY_STUDY_DEBATING:
      case ACTIVITY_STUDY_MARTIAL_ARTS:
      case ACTIVITY_STUDY_DRIVING:
      case ACTIVITY_STUDY_PSYCHOLOGY:
      case ACTIVITY_STUDY_FIRST_AID:
      case ACTIVITY_STUDY_LAW:
      case ACTIVITY_STUDY_DISGUISE:
      case ACTIVITY_STUDY_SCIENCE:
      case ACTIVITY_STUDY_BUSINESS:
      //case ACTIVITY_STUDY_COOKING:
      case ACTIVITY_STUDY_GYMNASTICS:
      case ACTIVITY_STUDY_WRITING:
      case ACTIVITY_STUDY_ART:
      case ACTIVITY_STUDY_MUSIC:
      case ACTIVITY_STUDY_TEACHING:
      case ACTIVITY_STUDY_LOCKSMITHING:
      case ACTIVITY_STUDY_COMPUTERS:
         move(22,3);
         addstr(cr->name);
         addstr(" will attend classes in the University District");
         move(23,1);
         addstr("  at a cost of $60 a day.");
         break;
      }

      int c=getkey();

      if(c>='a'&&c<='z') state=c;
      if((c>='a'&&c<='z') || (c>='0'&&c<='9'))
      {
         choice=c;
         switch(state)
         {
         case 'a':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_COMMUNITYSERVICE;break;
            case '2':cr->activity.type=ACTIVITY_TROUBLE;break;
            case '3':cr->activity.type=ACTIVITY_GRAFFITI;
               cr->activity.arg=-1;
               break;
            case '4':cr->activity.type=ACTIVITY_POLLS;break;
            //case '5':cr->activity.type=ACTIVITY_DOS_ATTACKS;break;
            case '5':cr->activity.type=ACTIVITY_HACKING;break;
            case '6':cr->activity.type=ACTIVITY_WRITE_LETTERS;break;
            case '7':
               if(cr->location!=-1&&
                  location[cr->location]->compound_walls & COMPOUND_PRINTINGPRESS)
               {
                  cr->activity.type=ACTIVITY_WRITE_GUARDIAN;break;
               }
            default:
               if(cr->get_attribute(ATTRIBUTE_WISDOM,true)>7||cr->juice<0)
                  cr->activity.type=ACTIVITY_COMMUNITYSERVICE;
               else if(cr->get_attribute(ATTRIBUTE_WISDOM,true)>4)
                  cr->activity.type=ACTIVITY_TROUBLE;
               else
               {
                  if(cr->get_skill(SKILL_COMPUTERS)>2)
                     cr->activity.type=ACTIVITY_HACKING;
                  else if(cr->get_skill(SKILL_ART)>1)
                  {
                     cr->activity.type=ACTIVITY_GRAFFITI;
                     cr->activity.arg=-1;
                  }
                  else
                     cr->activity.type=ACTIVITY_TROUBLE;
               }
            }
            break;
         case 'b':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_DONATIONS;break;
            case '2':cr->activity.type=ACTIVITY_SELL_TSHIRTS;break;
            case '3':cr->activity.type=ACTIVITY_SELL_ART;break;
            case '4':cr->activity.type=ACTIVITY_SELL_MUSIC;break;
            default:
               if(cr->get_weapon().is_instrument())
                  cr->activity.type=ACTIVITY_SELL_MUSIC;
               else if(cr->get_skill(SKILL_ART)>1)
                  cr->activity.type=ACTIVITY_SELL_ART;
               else if(cr->get_skill(SKILL_TAILORING)>1)
                  cr->activity.type=ACTIVITY_SELL_TSHIRTS;
               else if(cr->get_skill(SKILL_MUSIC)>1)
                  cr->activity.type=ACTIVITY_SELL_MUSIC;
               else cr->activity.type=ACTIVITY_DONATIONS;
            }
            break;
         case 'c':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_SELL_DRUGS;break;
            case '2':
#ifndef ZEROMORAL
               if(cr->age >= 18)
#endif
                  cr->activity.type=ACTIVITY_PROSTITUTION;break;
            case '3':cr->activity.type=ACTIVITY_CCFRAUD;break;
            //case '4':cr->activity.type=ACTIVITY_DOS_RACKET;break;
            default:
               if(cr->get_skill(SKILL_COMPUTERS)>1)
                  cr->activity.type=ACTIVITY_CCFRAUD;
#ifndef ZEROMORAL
               else if(cr->get_skill(SKILL_SEDUCTION)>1 && cr->age >= 18)
#else
               else if(cr->get_skill(SKILL_SEDUCTION)>1)
#endif
                  cr->activity.type=ACTIVITY_PROSTITUTION;
               else cr->activity.type=ACTIVITY_SELL_DRUGS;
            }
            break;
         case 'd':
            switch(choice)
            {
            case '1': { // Pick type to recruit
               activityst oact=cr->activity;
               cr->activity.type=ACTIVITY_NONE;
               recruitSelect(*cr);
               if(cr->activity.type==ACTIVITY_RECRUITING) break;
               else cr->activity=oact;
               break; }
            case '2': { // Pick clothing to make
               activityst oact=cr->activity;
               cr->activity.type=ACTIVITY_NONE;
               select_makeclothing(cr);
               if(cr->activity.type==ACTIVITY_MAKE_ARMOR) break;
               else cr->activity=oact;
               break; }
            case '3':cr->activity.type=ACTIVITY_REPAIR_ARMOR;break;
            case '4':
               if(cr->canwalk())
                  cr->activity.type=ACTIVITY_STEALCARS;
               else if(!(cr->flag & CREATUREFLAG_WHEELCHAIR))
                  cr->activity.type=ACTIVITY_WHEELCHAIR;
               break;
            case '5': {
               if(cr->get_skill(SKILL_SCIENCE)!=0) {
                  activityst oact=cr->activity;
                  cr->activity.type=ACTIVITY_NONE;
                  select_augmentation(cr);
                  if(cr->activity.type==ACTIVITY_AUGMENT) break;
                  else cr->activity=oact;
               }
               break; }
            }
            break;
         case 't':
            switch(choice)
            {
            case '1':cr->activity.type=ACTIVITY_TEACH_POLITICS;break;
            case '2':cr->activity.type=ACTIVITY_TEACH_COVERT;break;
            case '3':cr->activity.type=ACTIVITY_TEACH_FIGHTING;break;
            default:
               switch(cr->type)
               {
               // this first block are creatures with All Weapon Skills, Martial Arts, Dodge, and First Aid
               case CREATURE_ATHLETE:
               case CREATURE_BOUNCER: // for fighting skills
               case CREATURE_COP: // for fighting skills
               case CREATURE_CCS_ARCHCONSERVATIVE: // for fighting skills
               case CREATURE_CCS_MOLOTOV: // for fighting skills
               case CREATURE_CCS_SNIPER: // for fighting skills
               case CREATURE_CCS_VIGILANTE: // for fighting skills
               case CREATURE_DEATHSQUAD: // for fighting skills
               case CREATURE_DOCTOR: // for First Aid
               case CREATURE_FIREFIGHTER: // for fighting skills
               case CREATURE_GANGMEMBER: // for fighting skills
               case CREATURE_GANGUNIT: // for fighting skills
               case CREATURE_GUARDDOG:
               case CREATURE_GENETIC:
               case CREATURE_HARDENED_VETERAN: // for fighting skills
               case CREATURE_HICK:
               case CREATURE_MARTIALARTIST: // for fighting skills
               case CREATURE_MERC: // for fighting skills
               case CREATURE_MILITARYOFFICER: // for fighting skills
               case CREATURE_MILITARYPOLICE: // for fighting skills
               case CREATURE_MUTANT:
               case CREATURE_NURSE: // for First Aid
               case CREATURE_PRISONGUARD: // for fighting skills
               case CREATURE_SEAL: // for fighting skills
               case CREATURE_SECURITYGUARD: // for fighting skills
               case CREATURE_SOLDIER: // for fighting skills
               case CREATURE_SWAT: // for fighting skills
               case CREATURE_TANK: // fpr fighting skills
               case CREATURE_VETERAN: // for fighting skills
                  cr->activity.type=ACTIVITY_TEACH_FIGHTING;
                  break;
               // this second block are creatures with Computers, Security, Stealth, Disguise, Tailoring, Seduction, Psychology, & Driving
               case CREATURE_ACTOR: // for Disguise
               case CREATURE_AGENT: // for Driving & Psychology
               case CREATURE_AMATEURMAGICIAN:
               case CREATURE_BIKER: // for Driving
               case CREATURE_BUM:
               case CREATURE_CONSTRUCTIONWORKER: // for Driving
               case CREATURE_CRACKHEAD:
               case CREATURE_EDUCATOR: // for Psychology & Driving
               case CREATURE_FASHIONDESIGNER: // for Tailoring
               case CREATURE_GARBAGEMAN: // for Driving
               case CREATURE_HSDROPOUT:
               case CREATURE_LOCKSMITH: // for Security
               case CREATURE_MAILMAN:
               case CREATURE_PLUMBER:
               case CREATURE_PRISONER:
               case CREATURE_PROGRAMMER: // for Computers
               case CREATURE_PROSTITUTE: // for Seduction
               case CREATURE_PSYCHOLOGIST: // for Psychology
               case CREATURE_SECRET_SERVICE: // for Driving & Psychology
               case CREATURE_SEWERWORKER:
               case CREATURE_TAXIDRIVER: // for Driving
               case CREATURE_THIEF: // for Disguise, Security, & Stealth
               case CREATURE_TRUCKER: // for Driving
               case CREATURE_WORKER_FACTORY_CHILD:
               case CREATURE_WORKER_SERVANT:
               case CREATURE_WORKER_SWEATSHOP: // for Tailoring
                  cr->activity.type=ACTIVITY_TEACH_COVERT;
                  break;
               // this third block are creatures with Writing, Persuasion, Law, Street Sense, Science, Religion, Business, Music, & Art
               case CREATURE_AUTHOR: // for Writing & Persuasion
               case CREATURE_BANK_MANAGER: // for Business
               case CREATURE_BANK_TELLER:
               case CREATURE_CAMERAMAN: // for Art
               case CREATURE_CARSALESMAN: // for Business & Persuasion
               case CREATURE_CHEF:
               case CREATURE_CLERK:
               case CREATURE_COLLEGESTUDENT: // for Art, Music, Science, & Writing
               case CREATURE_CORPORATE_CEO: // for Business
               case CREATURE_CORPORATE_MANAGER: // for Business
               case CREATURE_CRITIC_ART: // for Writing, Persuasion, & Art
               case CREATURE_CRITIC_MUSIC: // for Writing, Persuasion, & Music
               case CREATURE_DANCER: // for Art & Music
               case CREATURE_ENGINEER: // for Science
               case CREATURE_FASTFOODWORKER:
            case CREATURE_BAKER:
            case CREATURE_BARISTA:
            case CREATURE_BARTENDER:
            case CREATURE_FOOTBALLCOACH: // for Persuasion
               case CREATURE_HAIRSTYLIST: // for Art
               case CREATURE_HIPPIE: // for Art & Music
               case CREATURE_JOURNALIST: // for Writing & Persuasion
               case CREATURE_JUDGE_CONSERVATIVE: // for Law & Writing
               case CREATURE_JUDGE_LIBERAL: // for Law & Writing
               case CREATURE_JUROR:
               case CREATURE_LANDLORD: // for Business
               case CREATURE_LAWYER: // for Law & Persuasion
               case CREATURE_MATHEMATICIAN: // for Science
               case CREATURE_MUSICIAN: // for Music
               case CREATURE_NEWSANCHOR:
               case CREATURE_NUN: // for Religion
               case CREATURE_OFFICEWORKER: // for Business
               case CREATURE_PAINTER: // for Art
               case CREATURE_PHOTOGRAPHER: // for Art
               case CREATURE_POLITICALACTIVIST: // for Persuasion, Law, & Writing
               case CREATURE_POLITICIAN: // for Law & Persuasion
               case CREATURE_PRIEST: // for Religion
               case CREATURE_RADIOPERSONALITY: // for Persuasion
               case CREATURE_RETIREE:
               case CREATURE_SCIENTIST_EMINENT: // for Science
               case CREATURE_SCIENTIST_LABTECH: // for Science
               case CREATURE_SCULPTOR: // for Art
               case CREATURE_SOCIALITE: // for Persuasion, Art, & Music
               case CREATURE_TEACHER:
               case CREATURE_TEENAGER:
               case CREATURE_TELEMARKETER: // for Persuasion
               case CREATURE_WORKER_FACTORY_NONUNION:
               case CREATURE_WORKER_FACTORY_UNION:
               case CREATURE_WORKER_JANITOR:
               case CREATURE_WORKER_SECRETARY: // for Writing
               case CREATURE_YOGAINSTRUCTOR:
               default:
                  cr->activity.type=ACTIVITY_TEACH_POLITICS;
                  break;
               }
               break;
            }
            break;
         case 'i':
            if(hostagecount>0)
            {
               activityst oact=cr->activity;
               cr->activity.type=ACTIVITY_NONE;
               select_tendhostage(cr);
               if(cr->activity.type==ACTIVITY_HOSTAGETENDING) break;
               else cr->activity=oact;
            }
            state=oldstate;
            break;
         case 'l':
            updateclasschoice(cr, choice);
            break;
         case 'm':
            if(clinictime(*cr)) cr->activity.type=ACTIVITY_CLINIC;
            else state=oldstate;
            break;
         case 'h':
            if(cr->get_skill(SKILL_FIRSTAID)) cr->activity.type=ACTIVITY_HEAL;
            else state=oldstate;
            break;
         case 'z':
            if(havedead) cr->activity.type=ACTIVITY_BURY;
            else state=oldstate;
            break;
         case 'e':
            if(!sieged)
            {
               //create a temp squad containing just this liberal
               int oldsquadid = cr->squadid;
               squadst *oldactivesquad = activesquad;
               activesquad=new squadst;
               strcpy(activesquad->name, "Temporary Squad");
               activesquad->id=cursquadid;
               activesquad->squad[0]=cr;
               cr->squadid = activesquad->id;
               //go to equipment screen
               equip(location[activesquad->squad[0]->location]->loot,-1);
               //once you're done, restore original squad status.
               delete activesquad;
               activesquad = oldactivesquad;
               cr->squadid = oldsquadid;
            }
            state=oldstate;
            break;
         /*case 'w':
            if(location[cr->location]->compound_walls==COMPOUND_PRINTINGPRESS)
            {
               activityst oact=cr->activity;
               cr->activity.type=ACTIVITY_NONE;
               if(select_view(cr,cr->activity.arg))
               {
                  cr->activity.type=ACTIVITY_WRITE_GUARDIAN;
                  break;
               }
               else cr->activity=oact;
            }
            state=oldstate;
            break;*/
         case 'x':
            cr->activity.type=ACTIVITY_NONE;
            break;
         default:
            state=oldstate;
            break;
         }
      }

      // Enter pressed
      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
      // ? Pressed
      if(c=='?')
         if(state=='a'||state=='b'||state=='c'||state=='d')
            HelpActivities(cr->activity.type); // Call activity help pages
   }
}


void activatebulk()
{
   vector<Creature *> temppool = activatable_liberals();

   if(!len(temppool)) return;

   int page=0,selectedactivity=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds();

      move(0,0);
      addstr("Activate Uninvolved Liberals");
      move(1,0);
      addstr("ÄÄÄÄCODE NAMEÄÄÄÄÄÄÄÄÄÄÄÄCURRENT ACTIVITYÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
      move(1,51);
      addstr("BULK ACTIVITY");

      if(selectedactivity==0) set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,51);
      addstr("1 - Liberal Activism");
      if(selectedactivity==1) set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(3,51);
      addstr("2 - Legal Fundraising");
      if(selectedactivity==2) set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(4,51);
      addstr("3 - Illegal Fundraising");
      if(selectedactivity==3) set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(5,51);
      addstr("4 - Checking Polls");
      if(selectedactivity==4) set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(6,51);
      addstr("5 - Stealing Cars");
      if(selectedactivity==5) set_color(COLOR_WHITE,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(7,51);
      addstr("6 - Community Service");

      int y=2;
      for(int p=page*19;p<len(temppool)&&p<page*19+19;p++,y++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         move(y,25);
         set_activity_color(temppool[p]->activity.type);
         addstr(getactivity(temppool[p]->activity));
         /*if(temppool[p]->activity.type==ACTIVITY_TROUBLE)
         {
         addstr(" ($");
         addstr(temppool[p]->activity.arg);
         addstr(")");
         }*/
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to Assign an Activity.  Press a Number to select an Activity.");
      move(23,0);
      addpagestr();

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
            switch(selectedactivity)
            {
            case 0: //Activism
               if(temppool[p]->get_attribute(ATTRIBUTE_WISDOM,true)>7||temppool[p]->juice<0)
                  temppool[p]->activity.type=ACTIVITY_COMMUNITYSERVICE;
               else if(temppool[p]->get_attribute(ATTRIBUTE_WISDOM,true)>4)
                  temppool[p]->activity.type=ACTIVITY_TROUBLE;
               else
               {
                  if(temppool[p]->get_skill(SKILL_COMPUTERS)>2)
                     temppool[p]->activity.type=ACTIVITY_HACKING;
                  else if(temppool[p]->get_skill(SKILL_ART)>1)
                  {
                     temppool[p]->activity.type=ACTIVITY_GRAFFITI;
                     temppool[p]->activity.arg=-1;
                  }
                  else temppool[p]->activity.type=ACTIVITY_TROUBLE;
               }
               break;
            case 1: //Fundraising
               if(temppool[p]->get_weapon().is_instrument())
                  temppool[p]->activity.type=ACTIVITY_SELL_MUSIC;
               else if(temppool[p]->get_skill(SKILL_ART)>1)
                  temppool[p]->activity.type=ACTIVITY_SELL_ART;
               else if(temppool[p]->get_skill(SKILL_TAILORING)>1)
                  temppool[p]->activity.type=ACTIVITY_SELL_TSHIRTS;
               else if(temppool[p]->get_skill(SKILL_MUSIC)>1)
                  temppool[p]->activity.type=ACTIVITY_SELL_MUSIC;
               else temppool[p]->activity.type=ACTIVITY_DONATIONS;
               break;
            case 2: //Illegal Fundraising
               if(temppool[p]->get_skill(SKILL_COMPUTERS)>1)
                  temppool[p]->activity.type=ACTIVITY_CCFRAUD;
#ifndef ZEROMORAL
               else if(temppool[p]->get_skill(SKILL_SEDUCTION)>1 && temppool[p]->age >=18)
#else
               else if(temppool[p]->get_skill(SKILL_SEDUCTION)>1)
#endif
                  temppool[p]->activity.type=ACTIVITY_PROSTITUTION;
               else
                  temppool[p]->activity.type=ACTIVITY_SELL_DRUGS;
               break;
            case 3: //Check polls
               temppool[p]->activity.type=ACTIVITY_POLLS;
               break;
            case 4: //Steal cars
               temppool[p]->activity.type=ACTIVITY_STEALCARS;
               break;
            case 5: //Volunteer
               temppool[p]->activity.type=ACTIVITY_COMMUNITYSERVICE;
               break;
            }
         }
      }
      if(c>='1'&&c<='6')
         selectedactivity=c-'1';

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}


/* base - activate - hostages */
void select_tendhostage(Creature *cr)
{
   vector<Creature *> temppool;

   for(int p=0;p<len(pool);p++)
   {
      if (pool[p]->align != Alignment::LIBERAL &&
          pool[p]->alive &&
          pool[p]->location==cr->location)
      {
         temppool.push_back(pool[p]);
      }
   }

   if(!len(temppool))return;
   if(len(temppool)==1)
   {
      cr->activity.type=ACTIVITY_HOSTAGETENDING;
      cr->activity.arg=temppool[0]->id;
      return;
   }

   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Which hostage will ");
      addstr(cr->name);
      addstr(" be watching over?");
      move(1,0);
      addstr("ÄÄÄÄCODE NAMEÄÄÄÄÄÄÄÄÄÄÄÄSKILLÄÄÄHEALTHÄÄÄLOCATIONÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
      move(1,57);
      addstr("DAYS IN CAPTIVITY");

      int y=2;
      for(int p=page*19;p<len(temppool)&&p<page*19+19;p++,y++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(temppool[p]->name);

         char bright=0;
         int skill=0;
         for(int sk=0;sk<SKILLNUM;sk++)
         {
            skill+=temppool[p]->get_skill(sk);
            if(temppool[p]->get_skill_ip(sk)>=100+(10*temppool[p]->get_skill(sk))&&
               temppool[p]->get_skill(sk)<temppool[p]->skill_cap(sk,true))bright=1;
         }

         set_color(COLOR_WHITE,COLOR_BLACK,bright);

         move(y,25);
         addstr(skill);

         printhealthstat(*temppool[p],y,33,TRUE);

         if(mode==REVIEWMODE_JUSTICE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(y,42,location[temppool[p]->location]->getname(true, true));

         move(y,57);
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         addstr(temppool[p]->joindays);
         addstr(" ");
         if(temppool[p]->joindays>1)addstr("Days");
         else addstr("Day");
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Conservative");
      move(23,0);
      addpagestr();

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
            cr->activity.type=ACTIVITY_HOSTAGETENDING;
            cr->activity.arg=temppool[p]->id;
            return;
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}


long select_hostagefundinglevel(Creature *cr,Creature *hs)
{
   long flevel=-1;

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   printfunds();
   move(0,0);
   addstr("Select a Funding Level for this Operation:");

   move(2,0);
   addstr("A - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" is just on watch duty.");

   move(3,0);
   addstr("B - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" will turn the prisoner over time.");

   move(4,0);
   addstr("C - $20 per day.  Enough for some props.");

   move(5,0);
   addstr("D - $50 per day.  ");
   addstr(cr->name);
   addstr(" will go for a thrifty indoctrination.");

   move(6,0);
   addstr("E - $100 per day.  ");
   addstr(cr->name);
   addstr(" needs enough freedom to be creative.");

   move(7,0);
   addstr("F - $500 per day.  It is imperative that the Conservative be turned quickly.");

   move(8,0);
   addstr("K - This Conservative has become a liability and needs to be terminated.");

   move(10,0);
   addstr("Enter - On second thought, this isn't a job for ");
   addstr(cr->name);
   addstr(".");

   do
   {
      int c=getkey();

      if(c=='a')flevel=0;
      if(c=='b')flevel=1;
      if(c=='c')flevel=20;
      if(c=='d')flevel=50;
      if(c=='e')flevel=100;
      if(c=='f')flevel=500;
      if(c=='k')flevel=666;
      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   } while(flevel==-1);

   return flevel;
}

struct recruitData
{
   int type;
   char* name;
   int difficulty;
   recruitData(int type_,char* name_,int difficulty_) : type(type_),name(name_),difficulty(difficulty_) { }
};

recruitData recruitable_creatures[] = {
   recruitData(CREATURE_VETERAN, (char*)"Army Veteran", 4),
   recruitData(CREATURE_ATHLETE, (char*)"Athlete", 4),
   recruitData(CREATURE_COLLEGESTUDENT, (char*)"College Student", 1),
   recruitData(CREATURE_PROGRAMMER, (char*)"Computer Programmer", 4),
   recruitData(CREATURE_DANCER, (char*)"Dancer", 4),
   recruitData(CREATURE_DOCTOR, (char*)"Doctor", 4),
   recruitData(CREATURE_FASHIONDESIGNER, (char*)"Fashion Designer", 6),
   recruitData(CREATURE_GANGMEMBER, (char*)"Gang Member", 2),
   recruitData(CREATURE_HIPPIE, (char*)"Hippie", 1),
   recruitData(CREATURE_JOURNALIST, (char*)"Journalist", 4),
   recruitData(CREATURE_JUDGE_LIBERAL, (char*)"Judge", 6),
   recruitData(CREATURE_LAWYER, (char*)"Lawyer", 4),
   recruitData(CREATURE_LOCKSMITH, (char*)"Locksmith", 6),
   recruitData(CREATURE_MARTIALARTIST, (char*)"Martial Artist", 4),
   recruitData(CREATURE_MUSICIAN, (char*)"Musician", 4),
   recruitData(CREATURE_MUTANT, (char*)"Mutant", 4),
   recruitData(CREATURE_PROSTITUTE, (char*)"Prostitute", 2),
   recruitData(CREATURE_PSYCHOLOGIST, (char*)"Psychologist", 4),
   recruitData(CREATURE_TAXIDRIVER, (char*)"Taxi Driver", 4),
   recruitData(CREATURE_TEACHER, (char*)"Teacher", 4)
};

// Return the difficulty of tracking this character type down, for the
// purpose of the activation menu. 0 is trivial, 10 is impossible.
int recruitFindDifficulty(int creatureType)
{
   for(int i=0; i<len(recruitable_creatures); i++)
      if(recruitable_creatures[i].type == creatureType)
         return recruitable_creatures[i].difficulty;
   return 10; // No recruitData; assume impossible to recruit
}

char* recruitName(int creatureType)
{
   for(int i=0; i<len(recruitable_creatures); i++)
      if(recruitable_creatures[i].type == creatureType)
         return recruitable_creatures[i].name;
   return (char*)"missingno";
}

void recruitSelect(Creature &cr)
{
   // Number of recruitable creatures
   int options = len(recruitable_creatures);
   for(int i=0; i<options; i++)
   {
      // Dynamic difficulty for certain creatures, recalculated each time the function is called
      if(recruitable_creatures[i].type == CREATURE_MUTANT)
      {
         if(law[LAW_NUCLEARPOWER] == Alignment::ARCH_CONSERVATIVE && law[LAW_POLLUTION] == Alignment::ARCH_CONSERVATIVE)
            recruitable_creatures[i].difficulty = 2;
         else if(law[LAW_NUCLEARPOWER] == Alignment::ARCH_CONSERVATIVE || law[LAW_POLLUTION] == Alignment::ARCH_CONSERVATIVE)
            recruitable_creatures[i].difficulty = 6;
         else
            recruitable_creatures[i].difficulty = 9;
      }
   }

   int page=0;
   while(true)
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("What type of person will ");
      addstr(cr.name);
      addstr(" try to meet and recruit today?");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("ÄÄÄÄTYPEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄDIFFICULTY TO ARRANGE MEETINGÄÄ");

      int y=2,difficulty;
      for(int p=page*19;p<options&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(recruitable_creatures[p].name);

         move(y,49);
         difficulty=recruitable_creatures[p].difficulty;
         switch(difficulty)
         {
         case 0:
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr("Simple");
            break;
         case 1:
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            addstr("Very Easy");
            break;
         case 2:
            set_color(COLOR_CYAN,COLOR_BLACK,0);
            addstr("Easy");
            break;
         case 3:
            set_color(COLOR_BLUE,COLOR_BLACK,1);
            addstr("Below Average");
            break;
         case 4:
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Average");
            break;
         case 5:
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("Above Average");
            break;
         case 6:
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr("Hard");
            break;
         case 7:
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            addstr("Very Hard");
            break;
         case 8:
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            addstr("Extremely Difficult");
            break;
         case 9:
            set_color(COLOR_RED,COLOR_BLACK,0);
            addstr("Nearly Impossible");
            break;
         default:
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("Impossible");
            break;
         }

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Profession");
      move(23,0);
      addpagestr();

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<options)page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<options)
         {
            cr.activity.type = ACTIVITY_RECRUITING;
            cr.activity.arg = recruitable_creatures[p].type;
            break;
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }

   return;
}

void show_victim_status(Creature *victim)
{
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   mvaddstr(2,55,"Status:");
   printhealthstat(*victim,2,66,true);
   printwoundstat(*victim,4,55);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   mvaddstr(11,55,"Heart: ");mvaddstr(11,66,victim->get_attribute(ATTRIBUTE_HEART,true));
   mvaddstr(12,55,"Age: ");mvaddstr(12,66,victim->age);
}

vector<string>& split_string(const string &s, char delim, vector<string> &elems) {
   ostringstream oss;
   for(char c:s) {
      if(c==' ')
      {
         elems.push_back(oss.str());
         oss.str(string());
      }
      else if(c=='\n')
      {
         elems.push_back(oss.str());
         elems.push_back("");
         oss.str(string());
      }
      else oss<<c;
   }
   elems.push_back(oss.str());

   return elems;
}

void select_augmentation(Creature *cr) //TODO: Finish and general cleanup
{
   Creature *victim = 0;
   int culloc=cr->location;
   vector<Creature *> temppool;

   for(int p=0;p<len(pool);p++) {
      if(pool[p] == cr) continue;
      if(pool[p]->is_active_liberal() && (pool[p]->location==culloc))
      {
         temppool.push_back(pool[p]);
      }
   }

   int cur_step=0,page=0,c=0,aug_c=0;
   vector<AugmentType *> aug_type;
   AugmentType *selected_aug;

   while(true)
   {
      erase();

      int y,p;

      switch(cur_step) {

         case 0: //PAGE 0, selecting a liberal
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(0,0,"Select a Liberal to perform experiments on");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(1,0,"ÄÄÄÄNAMEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄHEALTHÄÄÄÄÄÄÄÄÄÄÄÄHEARTÄÄÄÄÄÄÄÄAGEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
         for(p=page*19,y=2;p<len(temppool)&&p<page*19+19;p++,y++)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0); //c==y+'a'-2);
            move(y,0);
            addchar(y+'A'-2);addstr(" - ");
            addstr(temppool[p]->name);
            mvaddstr(y,49,temppool[p]->get_attribute(ATTRIBUTE_HEART,true));
            mvaddstr(y,62,temppool[p]->age);
            printhealthstat(*temppool[p],y,31,TRUE);
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(22,0);
         addstr("Press a Letter to select a Liberal");
         move(23,0);
         addpagestr();

         c = getkey();

         //PAGE UP
         if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
         //PAGE DOWN
         if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(temppool))page++;

         if(c>='a'&&c<='s')
         {
            int p=page*19+c-'a';
            if(p<len(temppool))
            {
               victim = temppool[p];
               cur_step = 1;
            }
            else
               victim = 0;
         }

         if(c=='x'||c==ESC||c==SPACEBAR||c==ENTER) return;

         break;



         case 1: //PAGE 1, selecting an augmentation
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(0,0,"Subject: ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(victim->name);addstr(", ");addstr(gettitle(*victim));
         //mvaddstr(1,0,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");

         show_victim_status(victim);

         mvaddstr(2,1,"Select an Augmentation");
         for(p=page*19,y=4;p<AUGMENTATIONNUM&&p<page*19+19;p++,y++)
         {
            bool already_augmented = victim->get_augmentation(y-4).type!=-1;
            if(already_augmented) set_color(COLOR_BLACK,COLOR_BLACK,1);
            else set_color(COLOR_WHITE,COLOR_BLACK,aug_c==y+'a'-4);
            move(y,1);
            addchar(y+'A'-4);addstr(" - ");
            addstr(Augmentation::get_name(y-4));
         }

         if(aug_c>='a'&&aug_c<='e'&&c>='a'&&c<='e')
         {
            aug_type.clear();
            if(victim->get_augmentation(aug_c-'a').type==-1) //False if already augmented on that bodypart.
            {
               for(int x=0,y=5;x<augmenttype.size();x++)
               {
                  if(augmenttype[x]->get_type()==aug_c-'a'&&
                        (augmenttype[x]->get_max_age()==-1||victim->age<=augmenttype[x]->get_max_age())&&
                        (augmenttype[x]->get_min_age()==-1||victim->age>=augmenttype[x]->get_min_age())&&
                        augmenttype[x]->get_cost() <= ledger.get_funds())
                        //TODO: Make it so that if you don't have money, it just grays it out, not just not show it
                     aug_type.push_back(augmenttype[x]);
               }
            }
         }

         set_color(COLOR_WHITE,COLOR_BLACK,0);

         for(int x=0,y=4;x<aug_type.size();x++,y++)
         {
            //set_color(COLOR_WHITE,COLOR_BLACK,c==y+'1'-5);
            mvaddchar(y,26,y+'1'-4);addstr(" - ");
            addstr(aug_type[x]->get_name());
         }

         //Checks to see if valid input, and moves to next screen
         if(aug_c>='a'&&aug_c<='e'&&c>='1'&&c<='0'+aug_type.size())
         {
            cur_step=2;
            selected_aug=aug_type[c-'1'];
            break;
         }

         c = getkey();
         if(c>='a'&&c<='e') aug_c=c;
         else if(c==ESC)return;
         else if(c=='x'||c==SPACEBAR||c==ENTER) {cur_step=0;aug_type.clear();aug_c=0;}
         break;



         case 2: //PAGE 2, confirm your choices
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(0,0,"Subject: ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(victim->name);addstr(", ");addstr(gettitle(*victim));

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(2,0,"Augmentation: ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(selected_aug->get_name());

         show_victim_status(victim);

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(4, 0, "Effect: ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         string selected_attribute = attribute_enum_to_string(selected_aug->get_attribute());
         addstr((char)(toupper(selected_attribute.at(0))) +
            selected_attribute.substr(1) +
            (selected_aug->get_effect() >= 0 ? " +" : " ") +
            std::to_string(selected_aug->get_effect()));

         set_color(COLOR_WHITE,COLOR_BLACK,1);
         mvaddstr(5, 0, "Chance at Success: ");
         int skills = cr->get_skill(SKILL_SCIENCE) + 0.5 * cr->get_skill(SKILL_FIRSTAID);
         int difficulty = selected_aug->get_difficulty();
         set_color(COLOR_WHITE, COLOR_BLACK, 0);
         addstr(to_string(100 * skills / difficulty));

         mvaddstr(7,0,"Description");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(8,0,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");

         vector<string> desc;
         split_string(selected_aug->get_description(),' ',desc);

         int chars_left=50;
         int line = 9;
         for(int i=0;i<desc.size();i++)
         {
            if(desc[i].length()>50) continue;
            else if(desc[i] == "")
            {
               line++;
               chars_left=50;
               continue;
            }
            else if(chars_left<0||desc[i].length()>chars_left)
            {
               line++;
               chars_left=50;
               i--;
               continue;
            }
            else if(desc[i].length()<=chars_left)
            {
               mvaddstr(line,50-chars_left,desc[i]);
               chars_left-=(desc[i].length()+1);
            }
         }

         mvaddstr(23,1,"Are you sure? (y/n)");

         c = getkey();
         if(c=='y')
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            mvaddstr(23,1,"Press any key to return");
            move(21,1);

            int blood_saved = 10*cr->get_skill(SKILL_SCIENCE) + 15*cr->get_skill(SKILL_FIRSTAID);
            if(blood_saved>100) blood_saved = 100;
            victim->blood-=100 - blood_saved;

            if(skills < difficulty &&
               LCSrandom((double)100 * difficulty / skills) < 100)
            {
               unsigned char* wound = nullptr;

               switch(selected_aug->get_type())
               {
               case AUGMENTATION_HEAD:
                  wound=&victim->wound[BODYPART_HEAD];
                  victim->blood-=100;
                  break;

               case AUGMENTATION_BODY:
                  wound=&victim->wound[BODYPART_BODY];
                  victim->blood-=100;
                  break;

               case AUGMENTATION_ARMS:
                  if(LCSrandom(2))
                     wound=&victim->wound[BODYPART_ARM_LEFT];
                  else
                     wound=&victim->wound[BODYPART_ARM_RIGHT];
                  victim->blood-=25;
                  break;

               case AUGMENTATION_LEGS:
                  if(LCSrandom(2))
                     wound=&victim->wound[BODYPART_LEG_LEFT];
                  else
                     wound=&victim->wound[BODYPART_LEG_RIGHT];
                  victim->blood-=25;
                  break;

               case AUGMENTATION_SKIN:
                  if(LCSrandom(2))
                     wound=&victim->wound[BODYPART_HEAD];
                  else
                     wound=&victim->wound[BODYPART_BODY];
                  victim->blood-=50;
                  break;
               }

               *wound|=WOUND_NASTYOFF;

               if(victim->blood>0)
               {
                  set_color(COLOR_RED,COLOR_BLACK,1);
                  addstr(string(victim->name)+" has been horribly disfigured", gamelog);
               }
            }

            else //It was successful... but not without some injuries
            {
               unsigned char* wound = nullptr;

               switch(selected_aug->get_type())
               {
               case AUGMENTATION_HEAD:
                  wound=&victim->wound[BODYPART_HEAD];
                  break;

               case AUGMENTATION_BODY:
                  wound=&victim->wound[BODYPART_BODY];
                  break;

               case AUGMENTATION_ARMS:
                  if(LCSrandom(2))
                     wound=&victim->wound[BODYPART_ARM_RIGHT];
                  else
                     wound=&victim->wound[BODYPART_ARM_LEFT];
                  break;

               case AUGMENTATION_LEGS:
                  if(LCSrandom(2))
                     wound=&victim->wound[BODYPART_LEG_RIGHT];
                  else
                     wound=&victim->wound[BODYPART_LEG_LEFT];
                  break;

               case AUGMENTATION_SKIN:
                  wound=&victim->wound[BODYPART_HEAD];
                  break;
               }

               *wound|=WOUND_BLEEDING;
               *wound|=WOUND_BRUISED;

               selected_aug->make_augment(victim->get_augmentation(selected_aug->get_type()));
               victim->adjust_attribute(selected_aug->get_attribute(), selected_aug->get_effect());
               cr->train(SKILL_SCIENCE,15);
               addjuice(*cr,10,1000);

               set_color(COLOR_GREEN,COLOR_BLACK,1);
               addstr(string(victim->name)+" has been augmented with "+selected_aug->get_name(), gamelog);
            }

            if(victim->blood<=0) //Lost too much blood, you killed 'em
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               victim->die();
               addstr(string(victim->name)+" has been brutally murdered by "+cr->name, gamelog);
            }

            gamelog.nextMessage();
            show_victim_status(victim);
            getkey();
            return;
         }

         else if(c==ESC)return;
         else if(c=='x'||c==SPACEBAR||c==ENTER||c=='n') {cur_step=1;selected_aug=nullptr;}
         break;
      }
   }
}

/* base - activate - make clothing */
void select_makeclothing(Creature *cr)
{
   vector<int> armortypei;
   for(int a=0;a<len(armortype);a++)
   {
      if(armortype[a]->get_make_difficulty() == 0)
         continue;

      if(armortype[a]->deathsquad_legality()
         && (law[LAW_POLICEBEHAVIOR] != Alignment::ARCH_CONSERVATIVE || law[LAW_DEATHPENALTY] != Alignment::ARCH_CONSERVATIVE))
         continue;

      armortypei.push_back(a);
   }

   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Which will ");
      addstr(cr->name);
      addstr(" try to make?   (Note: Half Cost if you have cloth)");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("ÄÄÄÄNAMEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄDIFFICULTYÄÄÄÄÄÄÄÄÄÄÄÄÄCOSTÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");

      int y=2,difficulty;
      for(int p=page*19;p<len(armortypei)&&p<page*19+19;p++,y++)
      {
         difficulty=armor_makedifficulty(*armortype[armortypei[p]],cr);
         if(difficulty<0) difficulty=0;

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(armortype[armortypei[p]]->get_name());

         move(y,37);
         switch(difficulty)
         {
         case 0:
            set_color(COLOR_GREEN,COLOR_BLACK,1);
            addstr("Simple");
            break;
         case 1:
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            addstr("Very Easy");
            break;
         case 2:
            set_color(COLOR_CYAN,COLOR_BLACK,0);
            addstr("Easy");
            break;
         case 3:
            set_color(COLOR_BLUE,COLOR_BLACK,1);
            addstr("Below Average");
            break;
         case 4:
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Average");
            break;
         case 5:
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("Above Average");
            break;
         case 6:
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr("Hard");
            break;
         case 7:
            set_color(COLOR_MAGENTA,COLOR_BLACK,0);
            addstr("Very Hard");
            break;
         case 8:
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            addstr("Extremely Difficult");
            break;
         case 9:
            set_color(COLOR_RED,COLOR_BLACK,0);
            addstr("Nearly Impossible");
            break;
         default:
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("Impossible");
            break;
         }

         set_color(COLOR_GREEN,COLOR_BLACK,1);
         string price = '$'+std::to_string(armortype[armortypei[p]]->get_make_price());
         move(y,64-len(price));
         addstr(price);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Type of Clothing");
      move(23,0);
      addpagestr();

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(armortypei))page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+c-'a';
         if(p<len(armortypei))
         {
            cr->activity.type=ACTIVITY_MAKE_ARMOR;
            cr->activity.arg=armortypei[p]; //Use id name of armor type instead? -XML
            return;
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}

int armor_makedifficulty(Armor& type, Creature *cr)
{
   return armor_makedifficulty(*armortype[getarmortype(type.get_itemtypename())], cr);
}

int armor_makedifficulty(ArmorType& type,Creature *cr) //Make class method? -XML
{
   int basedif=type.get_make_difficulty()-cr->get_skill(SKILL_TAILORING)+3;
   return MAX(basedif,0);
}


/* base - activate - trouble */
long select_troublefundinglevel(Creature *cr)
{
   long flevel=-1;

   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   printfunds();
   move(0,0);
   addstr("Select a Funding Level for this Operation:");

   move(2,0);
   addstr("A - Don't spend anything.  ");
   addstr(cr->name);
   addstr(" just needs something constructive to do.");

   move(3,0);
   addstr("B - $20 per day.  Some minor purchases are needed.");

   move(4,0);
   addstr("C - $50 per day.  Disobedience costs money.");

   move(5,0);
   addstr("D - $100 per day.  Enough to be really disobedient.");

   move(6,0);
   addstr("E - $500 per day.  The Machine will buckle under the weight of");
   move(7,0);
   addstr("    ");
   addstr(cr->name);
   addstr("'s Numerous and Varied Liberal Acts.");

   move(9,0);
   addstr("Enter - On second thought, this isn't a job for ");
   addstr(cr->name);
   addstr(".");

   do
   {
      int c=getkey();

      if(c=='a')flevel=0;
      if(c=='b')flevel=20;
      if(c=='c')flevel=50;
      if(c=='d')flevel=100;
      if(c=='e')flevel=500;
      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   } while(flevel==-1);

   return flevel;
}



/* base - activate - select a topic to write about */
char select_view(Creature *cr,int &v)
{
   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Write a news story if the LCS makes the news on the selected topic today, or");
      move(1,0);
      addstr("write editorials if there is no current news but there is public interest.");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,0);
      addstr("ÄÄÄÄTOPICÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄINTERESTÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");

      int y=3,x=0;
      for(int p=page*18;p<VIEWNUM-3&&p<page*18+18;p++,y++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,x);
         addchar((p-page*18)+'A');addstr(" - ");
         addstr(getview(p,false));

         move(y,44);
         if(public_interest[p]>100)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("Extremely Controversial");
         }
         else if(public_interest[p]>50)
         {
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr("Dinner Table Topic");
         }
         else if(public_interest[p]>10)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Significant Interest");
         }
         else if(public_interest[p]>0)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("Minor Discussion");
         }
         else
         {
            set_color(COLOR_BLACK,COLOR_BLACK,1);
            addstr("Exhausted");
         }
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Topic");
      move(23,0);
      addpagestr();

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*16<VIEWNUM-3) page++;

      if(c>='a'&&c<='a'+18)
      {
         int p=page*18+c-'a';
         if(p<VIEWNUM-3)
         {
            v=p;
            return 1;
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }

   return 0;
}

