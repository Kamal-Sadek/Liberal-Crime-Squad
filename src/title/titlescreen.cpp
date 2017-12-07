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
#include <stdlib.h>


namespace
{

char str[100];

struct Quote
{
  std::string line[4];
};

std::vector<Quote> quote_selection =
{
  {
    "\"Unjust laws exist; shall we be content to obey them, or shall we",
    "endeavor to amend them, and obey them until we have succeeded, or",
    "shall we transgress them at once? ... I say, break the law.\"",
    "\xc4\xc4 Henry David Thoreau"
  },
  {
    "\"One has not only a legal, but a moral",
    "responsibility to obey just laws.  Conversely, one",
    "has a moral responsibility to disobey unjust laws.\"",
    "\xc4\xc4 Martin Luther King, Jr."
  },
  {
    "\"Civil disobedience is not our problem. Our problem is",
    "civil obedience... Protest beyond the law is not a",
    "departure from democracy; it is absolutely essential to it.\"",
    "\xc4\xc4 Howard Zinn"
  },
  {
    "\"We have to grasp, as Marx and Adam Smith did, that corporations are",
    "not concerned with the common good... If we don't rebel, if we're",
    "not physically in an active rebellion, then it's spiritual death.\"",
    "\xc4\xc4 Chris Hedges"
  },
  {
    "\"Civil disobedience becomes a sacred duty when the",
    "state has become lawless or corrupt... Non-cooperation",
    "with evil is as much a duty as cooperation with good.\"",
    "\xc4\xc4 Mahatma Gandhi"
  },
  {
    "\"Civil disobedience's main goal typically is to try",
    "to arouse and inspire others to join and do something.",
    "Well, sometimes that is a good tactic, sometimes not.\"",
    "\xc4\xc4 Noam Chomsky"
  },
  {
    "\"Our enemies are innovative and resourceful, and so",
    "are we.  They never stop thinking about new ways to",
    "harm our country and our people, and neither do we.\"",
    "\xc4\xc4 George W. Bush"
  },
  {
    "\"When you kill one, it is a",
    "tragedy.  When you kill ten",
    "million, it is a statistic.\"",
    "\xc4\xc4 Josef Stalin"
  },
  {
    "\"There's an old saying in Tennessee \xc4\xc4 I know it's in Texas,",
    "probably in Tennessee \xc4\xc4 that says, fool me once, shame on",
    "\xc4\xc4 shame on you. Fool me \xc4\xc4 you can't get fooled again.\"",
    "\xc4\xc4 George W. Bush"
  },
  {
    "\"The great story here for anybody willing to find it and write about",
    "it and explain it is this vast right-wing conspiracy that has been",
    "conspiring against my husband since the day he announced for President.\"",
    "\xc4\xc4 Hillary Clinton"
  },
  {
    "\"But I don't want to put the",
    "cart before the horse.  We",
    "don't have a strategy yet.\"",
    "\xc4\xc4 Barack Obama"
  },
  {
    "\"You know how dumb the average person",
    "is?  Well, by definition, half of",
    "'em are even dumber than THAT.\"",
    "\xc4\xc4 J.R. \"Bob\" Dobbs"
  },
  {
    "\"I believe we are on an irreversible",
    "trend toward more freedom and",
    "democracy \xc4\xc4 but that could change.\"",
    "\xc4\xc4 Dan Quayle"
  },
  {
    "\"We should invade their countries, kill their leaders and convert them to",
    "Christianity.  We weren't punctilious about locating and punishing only Hitler",
    "and his top officers.  We carpet-bombed German cities; we killed civilians.\"",
    "\xc4\xc4 Ann Coulter"
  },
  {
    "\"Root out the counterrevolutionaries without mercy,",
    "lock up suspicious characters in concentration camps...",
    "Shirkers will be shot, regardless of past service.\"",
    "\xc4\xc4 Leon Trotsky"
  },
  {
    "\"I want to see Fegelein at once!  If",
    "he's gone AWOL, it's desertion!  Treason!",
    "Bring me Fegelein!  Fegelein!  Fegelein!\"",
    "\xc4\xc4 Adolf Hitler"
  },
  {
    "\"My fellow Americans, I'm pleased to tell you",
    "today that I've signed legislation that will outlaw",
    "Russia forever.  We begin bombing in five minutes.\"",
    "\xc4\xc4 Ronald Reagan"
  },
  {
    "\"You go into some of these small towns... the jobs have been gone",
    "now for 25 years and nothing's replaced them... It's not surprising,",
    "then, they get bitter, they cling to guns or religion...\"",
    "\xc4\xc4 Barack Obama"
  },
  {
    "\"You don't have a peaceful revolution.",
    "You don't have a turn-the-other-cheek revolution.",
    "There's no such thing as a nonviolent revolution.\"",
    "\xc4\xc4 Malcolm X"
  },
  {
    "\"Politics is war without",
    "bloodshed, while war is",
    "politics with bloodshed.\"",
    "\xc4\xc4 Chairman Mao"
  },
  {
    "\"Don't think that I have come to bring peace to the Earth.  I did not come to",
    "bring peace, but a sword.  For I have come to turn a man against his father, a",
    "daughter against her mother, a daughter-in-law against her mother-in-law...\"",
    "\xc4\xc4 Jesus Christ"
  },
  {
    "\"What signify a few lives lost in a century or two?  The",
    "tree of liberty must be refreshed from time to time with",
    "the blood of patriots and tyrants.  It is its natural manure.\"",
    "\xc4\xc4 Thomas Jefferson"
  },
  {
    "\"Is it better to be loved or feared?  One might want to be",
    "both, but because it is hard to unite them in one person, it",
    "is far safer to be feared than loved, if you cannot be both.\"",
    "\xc4\xc4 Niccolo Machiavelli"
  },
  {
    "\"People that are really very weird",
    "can get into sensitive positions and",
    "have a tremendous impact on history.\"",
    "\xc4\xc4 Dan Quayle"
  },
  {
    "\"The supreme art of war is to subdue the enemy without",
    "fighting... If you know the enemy and know yourself",
    "you need not fear the results of a hundred battles.\"",
    "\xc4\xc4 Sun Tzu"
  },
  {
    "\"Only two things are infinite, the",
    "universe and human stupidity, and",
    "I'm not sure about the former.\"",
    "\xc4\xc4 Albert Einstein"
  },
  {
    "\"As Putin rears his head and comes into the airspace of",
    "the United States of America, where \xc4\xc4 where do they",
    "go?  It's Alaska.  It's just right over the border.\"",
    "\xc4\xc4 Sarah Palin"
  },
  {
    "\"The production of too many useful things results in too many useless people.",
    "The only antidote to mental suffering is physical pain.",
    "If anything is certain, it is that I myself am not a Marxist.\"",
    "\xc4\xc4 Karl Marx"
  },
  {
    "\"If ignorant of the almighty Time Cube Creation Truth, you",
    "deserve to be killed.  Killing you is not immoral \xc4\xc4 but",
    "justified to save life on Earth for future generations.\"",
    "\xc4\xc4 Gene Ray"
  },
  {
    "\"These hangman rope sneak deadly gangsters, judges & police, trick, trap, rob,",
    "wreck, butcher, and murder the people to keep them terrorized in gangster",
    "Frankenstein earphone radio slavery for the Communist gangster government.\"",
    "\xc4\xc4 Francis E. Dec"
  },
  {
    "\"If a political party tried to abolish Social Security, unemployment insurance,",
    "labor laws, or farm programs, you'd not hear of them again.  A tiny splinter",
    "group thinks they can do it.  Their number is negligible and they are stupid.\"",
    "\xc4\xc4 Dwight D. Eisenhower"
  },
  {
    "\"If we see that Germany is winning we ought to help",
    "Russia and if Russia is winning we ought to help Germany,",
    "and that way let them kill as many as possible.\"",
    "\xc4\xc4 Harry S. Truman"
  },
  {
    "\"Let's make America",
    "great again.\"",
    "",
    "\xc4\xc4 Donald J. Trump"
  },
};


void
title_screen()
{
  erase();

  set_color(COLOR_GREEN, COLOR_BLACK, 1);
  strcpy(str, "Liberal Crime Squad");
  move(2, 39 - ((len(str)-1)>>1));
  addstr(str);

  set_color(COLOR_WHITE, COLOR_BLACK, 1);
  strcpy(str,"Inspired by the 1983 version of Oubliette");
  move(4, 39 - ((len(str)-1)>>1));
  addstr(str);

  auto const& selection = quote_selection[LCSrandom(quote_selection.size())];
  for (int y = 0; y < 4; ++y)
  {
    move(6 + y, 39 - selection.line[y].length()/2);
    addstr(selection.line[y]);
  }

  strcpy(str, "v3.9 Copyright (C) 2002-4, Tarn Adams");
  move(11, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "A Bay 12 Games Production");
  move(12, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "http://bay12games.com/lcs/");
  move(13, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "v" PACKAGE_VERSION " Maintained by the Open Source Community");
  move(15, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "https://github.com/bregma/Liberal-Crime-Squad");
  move(16, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "http://sourceforge.net/projects/lcsgame/");
  move(17, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "http://www.bay12forums.com/smf/index.php?board=3.0");
  move(18, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "http://lcs.wikidot.com/");
  move(19, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "Press ESC now to quit. Quitting later causes your progress to be saved.");
  move(21, 39-((len(str)-1)>>1));
  addstr(str);
  strcpy(str, "Press any other key to pursue your Liberal Agenda!");
  move(23, 39-((len(str)-1)>>1));
  addstr(str);
  move(24, 79);
  addstr("+");
}


void
choose_savefile_name()
{
  erase();
  set_color(COLOR_WHITE, COLOR_BLACK, 1);
  mvaddstr(0, 0, "In what world will you pursue your Liberal Agenda?");
  set_color(COLOR_WHITE, COLOR_BLACK, 0);
  mvaddstr(1, 0, "Enter a name for the save file.");

  char savefile_temp[21];
  enter_name(2,0, savefile_temp, sizeof(savefile_temp)/sizeof(savefile_temp[0]), "");
  if (strcmp(savefile_temp, "") != 0)
    savefile_name = string(savefile_temp) + ".dat";
  else
    savefile_name = "save_" + to_string(LCSrandom(1000)) + ".dat";
}

} // anonymous namespace


void
mode_title()
{
   title_screen();

   int c=0;

   vector<string> savefiles;

   do {

      if(c=='h') {
         viewhighscores();
         getkey();
         title_screen();
      }

      if (c == 'm')
        music.enableIf(!music.isEnabled());

      if (music.isEnabled())
        strcpy(str,"Press M to turn off the Music. Press H to view your Liberal High Score.");
      else
        strcpy(str,"Press M to turn on some Music. Press H to view your Liberal High Score.");
      move(22,39-((len(str)-1)>>1));
      addstr(str);
      if(c==ESC||c=='x') exit(0);

      c=getkey();
   } while(c=='m'||c=='h'||c=='x'||c==ESC);

   savefiles = std::move(LCSSaveFiles());

   char loaded = savefiles.size() > 0;
   bool to_delete = false;

   if(!loaded)
   {
      choose_savefile_name();
      setup_newgame();
      makecharacter();
   }
   else
   {
      int p=0, y=2, page=0;

      while(true)
      {
         erase();
         if(to_delete)
         {
            set_color(COLOR_YELLOW,COLOR_RED,1);
            mvaddstr(0,0, "Delete a Save File");
         }
         else
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            mvaddstr(0,0,"Choose a Save File");
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         mvaddstr(1,0,"\xc4\xc4\xc4\xc4Title\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4\xc4");
         for(p=page*19,y=2;p<savefiles.size()&&p<page*19+19;p++,y++)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0); //c==y+'a'-2);
            move(y,0);
            addchar(y+'A'-2);addstr(" - ");

			string &strtemp = savefiles[y-2];
            addstr(strtemp.substr(0, strtemp.find(".dat")));
         }

         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr("NEW GAME");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(22,0);
         if(!to_delete) addstr("Press a Letter to Select a Save File, V to switch to delete");
         else addstr("Press a Letter to Delete a Save File, V to switch to select");
         addstr(", or X to quit");
         move(23,0);
         addpagestr();

         c = getkey();

         //PAGE UP
         if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
         //PAGE DOWN
         if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<savefiles.size())page++;
         if(c>='a'&&c<='s')
         {
            int p=page*19+c-'a';
            if(!to_delete)
            {
               if(p<savefiles.size()) { savefile_name = savefiles[p]; break; }
               else if(p == savefiles.size())
               {
                  choose_savefile_name();
                  setup_newgame();
                  makecharacter();
                  break;
               }
            }
            else if(p<savefiles.size())
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               strcpy(str,"Are you sure you want to delete " + savefiles[p] + "? (y/n)");
               move(10,39-((len(str)-1)>>1));
               addstr(str);
               c=getkey();
               if(c=='y')
               {
                  LCSDeleteFile(savefiles[p].c_str(), LCSIO_PRE_HOME);
                  savefiles = std::move(LCSSaveFiles());
               }
               continue;
            }

         }
         else if(c=='v') to_delete = !to_delete;
         if(c==ESC||c=='x') exit(0);
      }

      load(savefile_name);

   }
   mode=GAMEMODE_BASE;
   mode_base();

   if(!disbanding) savegame(savefile_name);
}
