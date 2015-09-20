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

char str[100];

void title() {
   //title screen
   erase();

   gamelog.log("title");

   set_color(COLOR_GREEN,COLOR_BLACK,1);
   strcpy(str,"Liberal Crime Squad");
   move(2,39-((len(str)-1)>>1));
   addstr(str);

   set_color(COLOR_WHITE,COLOR_BLACK,1);
   strcpy(str,"Inspired by the 1983 version of Oubliette");
   move(4,39-((len(str)-1)>>1));
   addstr(str);

   switch(LCSrandom(32))
   {
   case 0:
      strcpy(str,"\"Unjust laws exist; shall we be content to obey them, or shall we");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"endeavor to amend them, and obey them until we have succeeded, or");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"shall we transgress them at once? ... I say, break the law.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Henry David Thoreau");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 1:
      strcpy(str,"\"One has not only a legal, but a moral");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"responsibility to obey just laws.  Conversely, one");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"has a moral responsibility to disobey unjust laws.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Martin Luther King, Jr.");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 2:
      strcpy(str,"\"Civil disobedience is not our problem. Our problem is");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"civil obedience... Protest beyond the law is not a");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"departure from democracy; it is absolutely essential to it.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Howard Zinn");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 3:
      strcpy(str,"\"We have to grasp, as Marx and Adam Smith did, that corporations are");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"not concerned with the common good... If we don't rebel, if we're");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"not physically in an active rebellion, then it's spiritual death.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Chris Hedges");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 4:
      strcpy(str,"\"Civil disobedience becomes a sacred duty when the");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"state has become lawless or corrupt... Non-cooperation");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"with evil is as much a duty as cooperation with good.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Mahatma Gandhi");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 5:
      strcpy(str,"\"Civil disobedience's main goal typically is to try");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"to arouse and inspire others to join and do something.");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"Well, sometimes that is a good tactic, sometimes not.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Noam Chomsky");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 6:
      strcpy(str,"\"Our enemies are innovative and resourceful, and so");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"are we.  They never stop thinking about new ways to");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"harm our country and our people, and neither do we.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ George W. Bush");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 7:
      strcpy(str,"\"When you kill one, it is a");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"tragedy.  When you kill ten");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"million, it is a statistic.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Josef Stalin");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 8:
      strcpy(str,"\"There's an old saying in Tennessee ÄÄ I know it's in Texas,");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"probably in Tennessee ÄÄ that says, fool me once, shame on");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ shame on you. Fool me ÄÄ you can't get fooled again.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ George W. Bush");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 9:
      strcpy(str,"\"The great story here for anybody willing to find it and write about");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"it and explain it is this vast right-wing conspiracy that has been");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"conspiring against my husband since the day he announced for President.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Hillary Clinton");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 10:
      strcpy(str,"\"But I don't want to put the");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"cart before the horse.  We");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"don't have a strategy yet.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Barack Obama");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 11:
      strcpy(str,"\"You know how dumb the average person");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"is?  Well, by definition, half of");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"'em are even dumber than THAT.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ J.R. \"Bob\" Dobbs");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 12:
      strcpy(str,"\"I believe we are on an irreversible");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"trend toward more freedom and");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"democracy ÄÄ but that could change.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Dan Quayle");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 13:
      strcpy(str,"\"We should invade their countries, kill their leaders and convert them to");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"Christianity.  We weren't punctilious about locating and punishing only Hitler");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"and his top officers.  We carpet-bombed German cities; we killed civilians.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Ann Coulter");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 14:
      strcpy(str,"\"Root out the counterrevolutionaries without mercy,");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"lock up suspicious characters in concentration camps...");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"Shirkers will be shot, regardless of past service.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Leon Trotsky");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 15:
      strcpy(str,"\"I want to see Fegelein at once!  If");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"he's gone AWOL, it's desertion!  Treason!");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"Bring me Fegelein!  Fegelein!  Fegelein!\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Adolf Hitler");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 16:
      strcpy(str,"\"My fellow Americans, I'm pleased to tell you");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"today that I've signed legislation that will outlaw");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"Russia forever.  We begin bombing in five minutes.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Ronald Reagan");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 17:
      strcpy(str,"\"You go into some of these small towns... the jobs have been gone");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"now for 25 years and nothing's replaced them... It's not surprising,");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"then, they get bitter, they cling to guns or religion...\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Barack Obama");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 18:
      strcpy(str,"\"You don't have a peaceful revolution.");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"You don't have a turn-the-other-cheek revolution.");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"There's no such thing as a nonviolent revolution.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Malcolm X");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 19:
      strcpy(str,"\"Politics is war without");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"bloodshed, while war is");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"politics with bloodshed.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Chairman Mao");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 20:
      strcpy(str,"\"Don't think that I have come to bring peace to the Earth.  I did not come to");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"bring peace, but a sword.  For I have come to turn a man against his father, a");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"daughter against her mother, a daughter-in-law against her mother-in-law...\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Jesus Christ");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 21:
      strcpy(str,"\"What signify a few lives lost in a century or two?  The");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"tree of liberty must be refreshed from time to time with");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"the blood of patriots and tyrants.  It is its natural manure.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Thomas Jefferson");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 22:
      strcpy(str,"\"Is it better to be loved or feared?  One might want to be");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"both, but because it is hard to unite them in one person, it");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"is far safer to be feared than loved, if you cannot be both.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Niccolo Machiavelli");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 23:
      strcpy(str,"\"People that are really very weird");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"can get into sensitive positions and");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"have a tremendous impact on history.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Dan Quayle");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 24:
      strcpy(str,"\"The supreme art of war is to subdue the enemy without");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"fighting... If you know the enemy and know yourself");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"you need not fear the results of a hundred battles.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Sun Tzu");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 25:
      strcpy(str,"\"Only two things are infinite, the");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"universe and human stupidity, and");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"I'm not sure about the former.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Albert Einstein");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 26:
      strcpy(str,"\"As Putin rears his head and comes into the airspace of");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"the United States of America, where ÄÄ where do they");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"go?  It's Alaska.  It's just right over the border.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Sarah Palin");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 27:
      strcpy(str,"\"The production of too many useful things results in too many useless people.");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"The only antidote to mental suffering is physical pain.");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"If anything is certain, it is that I myself am not a Marxist.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Karl Marx");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 28:
      strcpy(str,"\"If ignorant of the almighty Time Cube Creation Truth, you");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"deserve to be killed.  Killing you is not immoral ÄÄ but");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"justified to save life on Earth for future generations.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Gene Ray");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 29:
      strcpy(str,"\"These hangman rope sneak deadly gangsters, judges & police, trick, trap, rob,");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"wreck, butcher, and murder the people to keep them terrorized in gangster");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"Frankenstein earphone radio slavery for the Communist gangster government.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Francis E. Dec");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 30:
      strcpy(str,"\"If a political party tried to abolish Social Security, unemployment insurance,");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"labor laws, or farm programs, you'd not hear of them again.  A tiny splinter");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"group thinks they can do it.  Their number is negligible and they are stupid.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Dwight D. Eisenhower");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   case 31:
      strcpy(str,"\"If we see that Germany is winning we ought to help");
      move(6,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"Russia and if Russia is winning we ought to help Germany,");
      move(7,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"and that way let them kill as many as possible.\"");
      move(8,39-((len(str)-1)>>1));
      addstr(str);
      strcpy(str,"ÄÄ Harry S. Truman");
      move(9,39-((len(str)-1)>>1));
      addstr(str);
      break;
   }

   strcpy(str,"v3.9 Copyright (C) 2002-4, Tarn Adams");
   move(11,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"A Bay 12 Games Production");
   move(12,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://bay12games.com/lcs/");
   move(13,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"v" PACKAGE_VERSION " Maintained by the Open Source Community");
   move(15,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"https://github.com/Kamal-Sadek/Liberal-Crime-Squad");
   move(16,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://sourceforge.net/projects/lcsgame/");
   move(17,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://www.bay12games.com/forum/index.php?board=3.0");
   move(18,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"http://lcs.wikidot.com/");
   move(19,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press ESC now to quit. Quitting later causes your progress to be saved.");
   move(21,39-((len(str)-1)>>1));
   addstr(str);
   strcpy(str,"Press any other key to pursue your Liberal Agenda!");
   move(23,39-((len(str)-1)>>1));
   addstr(str);
   move(24,79);
   addstr("+");
}

void mode_title()
{
   title();

   int c=0;

   do {

      if(c=='h') {
         viewhighscores();
         getkey();
         title();
      }

      if(c=='m') music.enableIf(!music.isEnabled());
      if(music.isEnabled()) strcpy(str,"Press M to turn off the Music. Press H to view your Liberal High Score.");
      else strcpy(str,"Press M to turn on some Music. Press H to view your Liberal High Score.");
      move(22,39-((len(str)-1)>>1));
      addstr(str);
      if(c==ESC||c=='x') end_game();

      c=getkey();
   } while(c=='m'||c=='h'||c=='x'||c==ESC);

   char loaded = load();
   if(!loaded)
   {
      setup_newgame();
      makecharacter();
   }
   mode=GAMEMODE_BASE;
   mode_base();

   savegame();
}
