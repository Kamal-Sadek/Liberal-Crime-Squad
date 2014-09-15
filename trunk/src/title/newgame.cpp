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

#include <externs.h>

/* select new game options */
void setup_newgame()
{
   music.play(MUSIC_NEWGAME);
   bool classicmode   = false;
   bool strongccs     = false;
   bool nightmarelaws = false;
   multipleCityMode   = false;
   stalinmode         = false;

   clear();
   while(true)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(4,6);
      addstr("New Game of Liberal Crime Squad: Advanced Gameplay Options");
      move(7,0);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(classicmode)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" A - Classic Mode: No Conservative Crime Squad.");
      move(9,0);
      if(!classicmode)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      if(strongccs)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" B - We Didn't Start The Fire: The CCS starts active and extremely strong.");
      move(11,0);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(nightmarelaws)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" C - Nightmare Mode: Liberalism is forgotten. Is it too late to fight back?");
      move(13,0);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(multipleCityMode)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" D - National LCS: Advanced play across multiple cities.");
      move(15,0);
      if(nocourtpurge)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" E - Marathon Mode: Prevent Liberals from amending the Constitution.");
      #ifdef ALLOWSTALIN
      move(17,0);
      if(stalinmode)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" F - Stalinist Mode: Enable Stalinist Comrade Squad (not fully implemented).");
      move(21,4);
      #else // ALLOWSTALIN
      move(19,4);
      #endif // ALLOWSTALIN
      addstr("Press any other key to continue...");

      int c=getkey();

      if(c=='a')
      {
         classicmode=!classicmode;
         continue;
      }
      if(c=='b')
      {
         strongccs=!strongccs;
         continue;
      }
      if(c=='c')
      {
         nightmarelaws=!nightmarelaws;
         continue;
      }
      if(c=='d')
      {
          multipleCityMode=!multipleCityMode;
          continue;
      }
      if(c=='e')
      {
          nocourtpurge=!nocourtpurge;
          notermlimit=!notermlimit;
          continue;
      }
      #ifdef ALLOWSTALIN
      if(c=='f')
      {
          stalinmode=!stalinmode;
          continue;
      }
      #endif
      break;
   }
   if(nightmarelaws)
   {
      for(int l=0;l<LAWNUM;l++)
         law[l]=ALIGN_ARCHCONSERVATIVE;
      for(int a=0;a<VIEWNUM-3;a++)
         attitude[a]=LCSrandom(20);
      for(int s=0;s<SENATENUM;s++)
      {
         if(s<55) senate[s]=ALIGN_ARCHCONSERVATIVE;
         else if(s<70) senate[s]=ALIGN_CONSERVATIVE;
         else if(s<80) senate[s]=ALIGN_MODERATE;
         else if(s<97) senate[s]=ALIGN_LIBERAL;
         else senate[s]=ALIGN_ELITELIBERAL;
      }

      for(int h=0;h<HOUSENUM;h++)
      {
         if(h<220) house[h]=ALIGN_ARCHCONSERVATIVE;
         else if(h<350) house[h]=ALIGN_CONSERVATIVE;
         else if(h<400) house[h]=ALIGN_MODERATE;
         else if(h<425) house[h]=ALIGN_LIBERAL;
         else house[h]=ALIGN_ELITELIBERAL;
      }

      for(int c=0;c<COURTNUM;c++)
      {
         if(c<5) court[c]=ALIGN_ARCHCONSERVATIVE;
         else if(c<7) court[c]=ALIGN_CONSERVATIVE;
         else if(c<8) court[c]=ALIGN_MODERATE;
         else if(c<8) court[c]=ALIGN_LIBERAL;
         else court[c]=ALIGN_ELITELIBERAL;
         do
         {
            if(court[c]==ALIGN_ARCHCONSERVATIVE)
               generate_name(courtname[c],GENDER_WHITEMALEPATRIARCH);
            else generate_name(courtname[c]);
         } while(len(courtname[c])>20);
      }
   }
   if(classicmode)
      endgamestate=ENDGAME_CCS_DEFEATED;
   else if(strongccs)
   {
      endgamestate=ENDGAME_CCS_ATTACKS;
      //attitude[VIEW_POLITICALVIOLENCE]=90;
   }

   clear();
   while(true)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(4,6);
      addstr("New Game of Liberal Crime Squad: Your Agenda");
      move(7,0);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(wincondition==WINCONDITION_ELITE)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" A - No Compromise Classic - I will make all our laws Elite Liberal!");
      move(9,0);
      if(!classicmode)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      if(wincondition==WINCONDITION_EASY)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" B - Democrat Mode - Most laws must be Elite Liberal, some can be Liberal.");
      move(13,4);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Press any other key to continue...");

      int c=getkey();

      if(c=='a')
      {
         wincondition=WINCONDITION_ELITE;
         continue;
      }
      if(c=='b')
      {
         wincondition=WINCONDITION_EASY;
         continue;
      }
      break;
   }

   clear();
   while(true)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(4,6);
      addstr("New Game of Liberal Crime Squad: Field Learning");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(5,6);
      addstr("(affects Security, Stealth, Disguise, & Driving)");
      move(8,0);
      if(fieldskillrate==FIELDSKILLRATE_FAST)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" A - Fast skills - Grinding is Conservative!");
      move(10,0);
      if(fieldskillrate==FIELDSKILLRATE_CLASSIC)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" B - Classic - Excellence requires practice.");
      move(12,0);
      if(fieldskillrate==FIELDSKILLRATE_HARD)
         addstr("[X]");
      else addstr("[ ]");
      addstr(" C - Hard Mode - Learn from the best, or face arrest!");
      move(16,4);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Press any other key to continue...");

      int c=getkey();

      if(c=='a')
      {
         fieldskillrate=FIELDSKILLRATE_FAST;
         continue;
      }
      if(c=='b')
      {
         fieldskillrate=FIELDSKILLRATE_CLASSIC;
         continue;
      }
      if(c=='c')
      {
         fieldskillrate=FIELDSKILLRATE_HARD;
         continue;
      }
      break;
   }
}

enum recruits
{
   RECRUITS_GANG,
   RECRUITS_NONE
};

/* creates your founder */
void makecharacter()
{
   Creature *newcr=new Creature;
   newcr->align=1;

#ifdef BLIND
   newcr->special[SPECIALWOUND_RIGHTEYE]=1;
   newcr->special[SPECIALWOUND_LEFTEYE]=1;
#endif
#ifdef SPINE
   newcr->special[SPECIALWOUND_UPPERSPINE]=1;
   newcr->special[SPECIALWOUND_LOWERSPINE]=1;
#endif
#ifdef NOFACE
   newcr->special[SPECIALWOUND_TONGUE]=1;
   newcr->special[SPECIALWOUND_RIGHTEYE]=1;
   newcr->special[SPECIALWOUND_LEFTEYE]=1;
   newcr->special[SPECIALWOUND_NOSE]=1;
#endif
#ifdef NOWALK
   newcr->special[SPECIALWOUND_UPPERSPINE]=1;
   newcr->special[SPECIALWOUND_LOWERSPINE]=1;
   newcr->special[SPECIALWOUND_NECK]=1;
   newcr->wound[BODYPART_LEG_RIGHT]=1;
   newcr->wound[BODYPART_LEG_LEFT]=1;
#endif
#ifdef INTERNAL
   newcr->special[SPECIALWOUND_RIGHTLUNG]=1;
   newcr->special[SPECIALWOUND_LEFTLUNG]=1;
   newcr->special[SPECIALWOUND_HEART]=1;
   newcr->special[SPECIALWOUND_LIVER]=1;
   newcr->special[SPECIALWOUND_STOMACH]=1;
   newcr->special[SPECIALWOUND_LEFTKIDNEY]=1;
   newcr->special[SPECIALWOUND_RIGHTKIDNEY]=1;
   newcr->special[SPECIALWOUND_SPLEEN]=1;
#endif

   newcr->set_attribute(ATTRIBUTE_HEART,8);
   newcr->set_attribute(ATTRIBUTE_WISDOM,1);
   newcr->set_attribute(ATTRIBUTE_INTELLIGENCE,3);
   newcr->set_attribute(ATTRIBUTE_AGILITY,5);
   newcr->set_attribute(ATTRIBUTE_STRENGTH,4);
   newcr->set_attribute(ATTRIBUTE_HEALTH,6);
   newcr->set_attribute(ATTRIBUTE_CHARISMA,4);
   for(int sk=0;sk<SKILLNUM;sk++)newcr->set_skill(sk,0);

   char first[3][80];
   char last[80];
   bool male = LCSrandom(2); // whether or not starting gender is male
   char gender = newcr->gender_liberal = newcr->gender_conservative = (male ? GENDER_MALE : GENDER_FEMALE);
   do {
      firstname(first[0], GENDER_NEUTRAL);
      firstname(first[1], GENDER_MALE);
      firstname(first[2], GENDER_FEMALE);
      lastname(last);
   } while(strcmp(first[0],last) == 0 && strcmp(first[1],last) == 0 && strcmp(first[2],last) == 0);

   {
      Armor a(*armortype[getarmortype("ARMOR_CLOTHES")]);
      newcr->give_armor(a,NULL);
   }

   bool choices=true;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(4,6);
      addstr("The Founder of the Liberal Crime Squad");

      move(7,2);
      addstr("FIRST NAME: ");
      addstr(first[(int)gender]);
      move(7,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press A to have your parents reconsider)");

      move(9,2);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("LAST NAME: ");
      addstr(last);
      move(9,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press B to be born to a different family)");

      move(11,2);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("SEX: ");
      if(newcr->gender_conservative == GENDER_MALE)
      {
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         addstr("Male");
      }
      else if(newcr->gender_conservative == GENDER_FEMALE)
      {
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         addstr("Female");
      }
      else
      {
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         addstr("It's Complicated");
      }
      move(11,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press C to change your sex at birth)");

      move(13,2);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("HISTORY: ");
      if(choices)
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         addstr("Let Me Choose");
      }
      else
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr("Let Fate Decide");
      }
      move(13,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press D to toggle childhood)");

      if(!multipleCityMode)
      {
         move(15,2);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("CITY: ");
         addstr(lcityname);
         move(15,30);
         set_color(COLOR_BLACK,COLOR_BLACK,1);
         addstr(" (Press E to relocate)");
      }

      move(19-multipleCityMode*2,4);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Press any other key when ready to begin...");

      int c=getkey();

      if(c=='a')
      {
         do {
            firstname(first[(int)gender],newcr->gender_conservative);
         } while(strcmp(first[(int)gender],last) == 0);
         continue;
      }
      if(c=='b')
      {
         do {
            lastname(last);
         } while(strcmp(first[0],last) == 0 && strcmp(first[1],last) == 0 && strcmp(first[2],last) == 0);
         continue;
      }
      if(c=='c')
      {
         if((newcr->gender_conservative == GENDER_FEMALE && !male) ||
            (newcr->gender_conservative == GENDER_NEUTRAL && male))
            newcr->gender_conservative = GENDER_MALE;
         else if((newcr->gender_conservative == GENDER_MALE && !male) ||
            (newcr->gender_conservative == GENDER_FEMALE && male))
            newcr->gender_conservative = GENDER_NEUTRAL;
         else
            newcr->gender_conservative = GENDER_FEMALE;

         gender = newcr->gender_liberal = newcr->gender_conservative;
         continue;
      }
      if(c=='d')
      {
         choices = !choices;
         continue;
      }
      if(c=='e' && !multipleCityMode)
      {
         strcpy(lcityname,cityname());
         continue;
      }
      break;
   }

   strcpy(newcr->propername,first[(int)gender]);
   strcat(newcr->propername," ");
   strcat(newcr->propername,last);

   int c;
   bool hasmaps=false;
   bool makelawyer=false;
   bool gaylawyer=false;
   Vehicle * startcar = NULL;
   char recruits = RECRUITS_NONE;
   char base = SITE_RESIDENTIAL_SHELTER;

   for(int q=0;q<10;q++)
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Insight into a Revolution:  My Traumatic Childhood");
      set_color(COLOR_WHITE,COLOR_BLACK,0);

      //A - Thief
      //B - Fighter
      //C - Student
      //D - Generalist/Sleepers
      //E - Recruiter

      char selection = LCSrandom(5);

      switch(q)
      {

/*
        - The anniversary of Hitler's suicide. - SKILL_PISTOL 1 (lol)
        - Indira Gandhi Killed
*/

         case 0:
            move(2,0);addstr("The day I was born in 1984...");

            move(5,0);
            if(choices || selection == 0)
               addstr("A - the Polish priest Popieluszko was kidnapped by government agents.");
            //ATTRIBUTE_AGILITY 2
            // Oct. 19, 1984
            move(7,0);
            if(choices || selection == 1)
               addstr("B - was the 3rd anniversary of the assassination attempt on Ronald Reagan.");
            //ATTRIBUTE_STRENGTH 2
            // Mar. 3, 1984
            move(9,0);
            if(choices || selection == 2)
               addstr("C - the Macintosh was introduced.");
            //ATTRIBUTE_INTELLIGENCE 2
            // Jan. 24, 1984
            move(11,0);
            if(choices || selection == 3)
               addstr("D - the Nobel Peace Prize went to Desmond Tutu for opposition to apartheid.");
            //ATTRIBUTE_HEART 2
            // Oct. 16, 1984
            move(13,0);
            if(choices || selection == 4)
               addstr("E - the Sandanista Front won the elections in Nicaragua.");
            //ATTRIBUTE_CHARISMA 2
            // Sept. 4, 1984
                        //move(14,0);
                        //if(choices || selection == 5)
               //addstr("F - the United Nations condemned Iraq's use of chemical weapons.");
            //ATTRIBUTE_HEALTH 2
                        //March 30, 1984

            move(17,0);
            addstr("The doctor said I was ");
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            if(newcr->gender_conservative == GENDER_MALE)
               addstr("a boy");
            else if(newcr->gender_conservative == GENDER_FEMALE)
               addstr("a girl");
            else
               addstr("an intersex baby");
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr(".");

            move(19,0);
            addstr("My parents ");
            if(newcr->gender_conservative == GENDER_NEUTRAL)
            {
               addstr("insisted otherwise.");
               move(20,0);
               addstr("They ");
            }

            addstr("named me ");
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr(newcr->propername);
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr(".");

            break;

                // My first memory was...
                        // my father burning my back with a cigarette
                // When he was really into the sauce...
                        //
                // XXX: Needs an option to have the founder have been in the Army -- LK
                // XXX: Something I forgot.

         case 1:
            move(2,0);addstr("When I was bad...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - my parents grounded me and hid my toys, but I knew where they put them."); // Toy box
            //SKILL_SECURITY 1
            //ATTRIBUTE_AGILITY 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - my father beat me.  I learned to take a punch earlier than most.");
            //SKILL_HANDTOHAND 1
            //ATTRIBUTE_HEALTH 1
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I was sent to my room, where I studied quietly by myself, alone.");
            //SKILL_WRITING 1
            //ATTRIBUTE_INTELLIGENCE 1
            move(11,0);
            if(choices || selection == 3)
               addstr("D - my parents argued with each other about me, but I was never punished.");
            //SKILL_PERSUASION 1
            //ATTRIBUTE_HEART 1
            move(13,0);
            if(choices || selection == 4)
               addstr("E - my father lectured me endlessly, trying to make me think like him.");
            //SKILL_PSYCHOLOGY 1
            //ATTRIBUTE_CHARISMA 1

            /*switch(c)
            {
            case 'e':
               move(17,4);
               addstr("\"The worst has happened. Someone wanted to kill and killed");
               move(18,4);
               addstr("not only the a man, not only a Pole, not only a priest.");
               move(19,4);
               addstr("Someone wanted to kill the hope that it is possible to avoid");
               move(20,4);
               addstr("violence in Polish political life.\"");
               move(21,8);
               addstr("- Solidarity Leader Lech Walesa");
               break;
            default:
               break;
            }*/
            break;
         case 2:
            move(2,0);addstr("In elementary school...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - I was mischievous, and always up to something.");
            //SKILL_DISGUISE 1
            //ATTRIBUTE_AGILITY 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - I had a lot of repressed anger.  I hurt animals.");
            //SKILL_PSYCHOLOGY 1
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_AGILITY 1
            //ATTRIBUTE_HEART -1 <--- !
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I was at the head of the class, and I worked very hard.");
            //ATTRIBUTE_INTELLIGENCE 1
            //SKILL_WRITING 1
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I was unruly and often fought with the other children.");
            //SKILL_HANDTOHAND 1
            //ATTRIBUTE_STRENGTH 1
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I was the class clown.  I even had some friends.");
            //SKILL_PERSUASION 1
            //ATTRIBUTE_CHARISMA 1
            break;
         case 3:
            move(2,0);addstr("When I turned 10...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - my parents divorced.  Whenever I talked, they argued, so I stayed quiet.");
            //SKILL_STEALTH 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - my parents divorced.  Violently.");
            //SKILL_HANDTOHAND 1
            move(9,0);
            if(choices || selection == 2)
               addstr("C - my parents divorced.  Acrimoniously.  I once tripped over the paperwork!");
            //SKILL_LAW 1
            move(11,0);
            if(choices || selection == 3)
               addstr("D - my parents divorced.  Mom slept with the divorce lawyer.");
            //SKILL_SEDUCTION 1
            move(13,0);
            if(choices || selection == 4)
               addstr("E - my parents divorced.  It still hurts to read my old diary.");
            //SKILL_WRITING 1
            break;
         case 4:
            move(2,0);addstr("In junior high school...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - I was into chemistry.  I wanted to know what made the world tick.");
            //SKILL_SCIENCE 2
            //ATTRIBUTE_INTELLIGENCE 2
            move(7,0);
            if(choices || selection == 1)
               addstr("B - I played guitar in a grunge band.  We sucked, but so did life.");
            //SKILL_MUSIC 2
            //ATTRIBUTE_CHARISMA 2
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I drew things, a lot.  I was drawing a world better than this.");
            //SKILL_ART 2
            //ATTRIBUTE_HEART 2
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I played violent video games at home.  I was a total outcast.");
            //SKILL_COMPUTERS 2
            //ATTRIBUTE_AGILITY 2
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I was obsessed with swords, and started lifting weights.");
            //SKILL_SWORD 2
            //ATTRIBUTE_STRENGTH 2
            break;
         case 5:
            move(2,0);addstr("Things were getting really bad...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - when I stole my first car.  I got a few blocks before I totaled it.");
            //SKILL_DRIVING 1
            //SKILL_SECURITY 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - and I went to live with my dad.  He had been in Nam and he still drank.");
            //SKILL_SHOTGUN 1
            //SKILL_RIFLE 1
            //SKILL_PSYCHOLOGY 1
            move(9,0);
            if(choices || selection == 2)
               addstr("C - and I went completely goth.  I had no friends and made costumes by myself.");
            //SKILL_TAILORING 2
            move(11,0);
            if(choices || selection == 3)
               addstr("D - when I was sent to religious counseling, just stressing me out more.");
            //SKILL_RELIGION 1
            //SKILL_PSYCHOLOGY 1
            move(13,0);
            if(choices || selection == 4)
               addstr("E - and I tried being a teacher's assistant.  It just made me a target.");
            //SKILL_TEACHING 2
            break;
         case 6:
            move(2,0);addstr("Well, I knew it had reached a crescendo when...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - I stole a cop car when I was only 14.  I went to juvie for 6 months.");
            //SKILL_DRIVING 1
            //SKILL_SECURITY 1
            //ATTRIBUTE_INTELLIGENCE 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - my step mom shot her ex-husband, my dad, with a shotgun.  She got off.");//XXX: Sounds... Sexual...
            //SKILL_SHOTGUN 2
            //ATTRIBUTE_AGILITY 1
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I tried wrestling for a quarter, desperate to fit in.");
            //ATTRIBUTE_STRENGTH 1
            //SKILL_HANDTOHAND 2
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I got caught making out, and now I needed to be 'cured' of homosexuality.");
            //SKILL_SEDUCTION 1
            //SKILL_RELIGION 1
            //ATTRIBUTE_HEART 1
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I resorted to controlling people.  Had my own clique of outcasts.");
            //SKILL_PERSUASION 2
            //ATTRIBUTE_CHARISMA 1
            break;
         case 7:
            move(2,0);addstr("I was only 15 when I ran away, and...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - I started robbing houses:  rich people only.  I was fed up with their crap.");
            //SKILL_SECURITY 1
            //SKILL_STEALTH 1
            //ATTRIBUTE_AGILITY 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - I hung out with thugs and beat the shit out of people.");
            //ATTRIBUTE_STRENGTH 1
            //SKILL_HANDTOHAND 2
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I got a horrible job working fast food, smiling as people fed the man.");
            //ATTRIBUTE_CHARISMA 1
            //SKILL_BUSINESS 2
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I let people pay me for sex.  I needed the money to survive.");
            //ATTRIBUTE_HEART -1
            //ATTRIBUTE_CHARISMA 2
            //SKILL_SEDUCTION 2
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I volunteered for a left-wing candidate. It wasn't *real*, though, you know?");
            //ATTRIBUTE_INTELLIGENCE 1
            //SKILL_LAW 1
            //SKILL_PERSUASION 1
            break;
         case 8:
            move(2,0);addstr("Life went on.  On my 18th birthday...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - I got my hands on a sports car. The owner must have been pissed.");
            move(7,0);
            if(choices || selection == 1)
               addstr("B - I bought myself an assault rifle.");
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I celebrated.  I'd saved a thousand bucks!");
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I went to a party and met a cool law student.  We've been dating since.");//XXX: No Seduction?
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I managed to acquire secret maps of several major buildings downtown.");
            break;
         case 9:
            move(2,0);addstr("For the past few years, I've been...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - stealing from Corporations.  I know they're still keeping more secrets.");
            //ATTRIBUTE_INTELLIGENCE 2
            //ATTRIBUTE_AGILITY 2
            //SKILL_SECURITY 2
            //SKILL_STEALTH 2
            // +Downtown apartment
            // +$500 (one month rent)
            move(7,0);
            if(choices || selection == 1)
               addstr("B - a violent criminal.  Nothing can change me, or stand in my way.");
            //SKILL_RIFLE 2
            //SKILL_PISTOL 2
            //SKILL_STREETSENSE 2
            //ATTRIBUTE_AGILITY 2
            //ATTRIBUTE_HEALTH 2
            //ATTRIBUTE_STRENGTH 2
            // +Crack house (with stockpiled rations)
            // +A crew (four gang members with knives and pistols)
            move(9,0);
            if(choices || selection == 2)
               addstr("C - taking college courses.  I can see how much the country needs help.");
            //SKILL_SCIENCE 2
            //SKILL_COMPUTERS 2
            //SKILL_WRITING 2
            //SKILL_TEACHING 2
            //SKILL_BUSINESS 1
            //SKILL_LAW 1
            //ATTRIBUTE_INTELLIGENCE 4
            // +University apartment
            // +$200 (one month rent)
            move(11,0);
            if(choices || selection == 3)
               addstr("D - surviving alone, just like anyone.  But we can't go on like this.");
            //SKILL_FIRSTAID 2
            //SKILL_STREETSENSE 2
            //ATTRIBUTE_INTELLIGENCE 1
            //ATTRIBUTE_AGILITY 1
            //ATTRIBUTE_HEALTH 2
            // +Homeless shelter
            // +1 all stats (except Wisdom)
            move(13,0);
            if(choices || selection == 4)
               addstr("E - writing my manifesto and refining my image.  I'm ready to lead.");
            //ATTRIBUTE_CHARISMA 2
            //ATTRIBUTE_INTELLIGENCE 2
            //SKILL_LAW 1
            //SKILL_PERSUASION 2
            //SKILL_WRITING 1
            // +Industrial apartment
            // +$100 (one month rent)
            // +50 juice
            move(17,0);
            addstr("I live in ");
            if(!multipleCityMode) addstr(lcityname);
            else addstr("Seattle, WA");
            addstr(", and it's about to experience real change.");
            break;
      }

      do
      {
         c=getkey();
         if(!choices) c='a'+selection;
      } while(c<'a'||c>'e');

      switch(q)
      {
         case 0:
            // Oct. 19, 1984
            if(c=='a')
            {
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+2);
               newcr->birthday_month = 10;
               newcr->birthday_day = 19;
            }
            // Mar. 3, 1984
            if(c=='b')
            {
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+2);
               newcr->birthday_month = 3;
               newcr->birthday_day = 3;
            }
            // Jan. 24, 1984
            if(c=='c')
            {
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+2);
               newcr->birthday_month = 1;
               newcr->birthday_day = 24;
            }
            // Oct. 16, 1984
            if(c=='d')
            {
               newcr->adjust_attribute(ATTRIBUTE_HEART,+2);
               newcr->birthday_month = 10;
               newcr->birthday_day = 16;
            }
            // Sep. 4, 1984
            if(c=='e')
            {
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+2);
               newcr->birthday_month = 9;
               newcr->birthday_day = 4;
            }
            newcr->age = year - 1984;
            // Don't count this year in founder's age if starting before birthday
            if(month < newcr->birthday_month ||
               (month==newcr->birthday_month && day<newcr->birthday_day))
            {
               newcr->age--;
            }
            break;
         case 1:
            if(c=='a')
            {
               newcr->set_skill(SKILL_SECURITY,newcr->get_skill(SKILL_SECURITY)+(1));
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+1);
            }
            if(c=='b')
            {
               newcr->set_skill(SKILL_HANDTOHAND,newcr->get_skill(SKILL_HANDTOHAND)+(1));
               newcr->adjust_attribute(ATTRIBUTE_HEALTH,+1);
            }
            if(c=='c')
            {
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+1);
               newcr->set_skill(SKILL_WRITING,newcr->get_skill(SKILL_WRITING)+(1));
            }
            if(c=='d')
            {
               newcr->set_skill(SKILL_PERSUASION,newcr->get_skill(SKILL_PERSUASION)+(1));
               newcr->adjust_attribute(ATTRIBUTE_HEART,+1);
            }
            if(c=='e')
            {
               newcr->set_skill(SKILL_PSYCHOLOGY,newcr->get_skill(SKILL_PSYCHOLOGY)+(1));
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+1);
            }
            break;
         case 2:
            if(c=='a')
            {
               newcr->set_skill(SKILL_DISGUISE,newcr->get_skill(SKILL_DISGUISE)+(1));
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+1);
            }
            if(c=='b')
            {
               newcr->set_skill(SKILL_PSYCHOLOGY,newcr->get_skill(SKILL_PSYCHOLOGY)+(1));
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+1);
               newcr->adjust_attribute(ATTRIBUTE_HEART,-1);
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+1);
            }
            if(c=='c')
            {
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+1);
               newcr->set_skill(SKILL_WRITING,newcr->get_skill(SKILL_WRITING)+(1));
            }
            if(c=='d')
            {
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+1);
               newcr->set_skill(SKILL_HANDTOHAND,newcr->get_skill(SKILL_HANDTOHAND)+(1));
            }
            if(c=='e')
            {
               newcr->set_skill(SKILL_PERSUASION,newcr->get_skill(SKILL_PERSUASION)+(1));
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+1);
            }
            break;
         case 3:
            if(c=='a')
            {
               newcr->set_skill(SKILL_STEALTH,newcr->get_skill(SKILL_STEALTH)+(1));
            }
            if(c=='b')
            {
               newcr->set_skill(SKILL_HANDTOHAND,newcr->get_skill(SKILL_HANDTOHAND)+(1));
            }
            if(c=='c')
            {
               newcr->set_skill(SKILL_LAW,newcr->get_skill(SKILL_LAW)+(1));
            }
            if(c=='d')
            {
               newcr->set_skill(SKILL_SEDUCTION,newcr->get_skill(SKILL_SEDUCTION)+(1));
            }
            if(c=='e')
            {
               newcr->set_skill(SKILL_WRITING,newcr->get_skill(SKILL_WRITING)+1);
            }
            break;
         case 4:
            if(c=='a')
            {
               newcr->set_skill(SKILL_SCIENCE,newcr->get_skill(SKILL_SCIENCE)+(2));
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+2);
            }
            if(c=='b')
            {
               newcr->set_skill(SKILL_MUSIC,newcr->get_skill(SKILL_MUSIC)+(2));
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+2);
            }
            if(c=='c')
            {
               newcr->set_skill(SKILL_ART,newcr->get_skill(SKILL_ART)+(2));
               newcr->adjust_attribute(ATTRIBUTE_HEART,+2);
            }
            if(c=='d')
            {
               newcr->set_skill(SKILL_COMPUTERS,newcr->get_skill(SKILL_COMPUTERS)+(2));
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+2);
            }
            if(c=='e')
            {
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+2);
               newcr->set_skill(SKILL_SWORD,newcr->get_skill(SKILL_SWORD)+(2));
            }
            break;
         case 5:
            if(c=='a')
            {
               newcr->set_skill(SKILL_DRIVING,newcr->get_skill(SKILL_DRIVING)+(1));
               newcr->set_skill(SKILL_SECURITY,newcr->get_skill(SKILL_SECURITY)+(1));
            }
            if(c=='b')
            {
               newcr->set_skill(SKILL_SHOTGUN,newcr->get_skill(SKILL_SHOTGUN)+(1));
               newcr->set_skill(SKILL_RIFLE,newcr->get_skill(SKILL_RIFLE)+(1));
               newcr->set_skill(SKILL_PSYCHOLOGY,newcr->get_skill(SKILL_PSYCHOLOGY)+(1));
            }
            if(c=='c')
            {
               newcr->set_skill(SKILL_TAILORING,newcr->get_skill(SKILL_TAILORING)+(2));
            }
            if(c=='d')
            {
               newcr->set_skill(SKILL_RELIGION,newcr->get_skill(SKILL_RELIGION)+(1));
               newcr->set_skill(SKILL_PSYCHOLOGY,newcr->get_skill(SKILL_PSYCHOLOGY)+(1));
            }
            if(c=='e')
            {
               newcr->set_skill(SKILL_TEACHING,newcr->get_skill(SKILL_TEACHING)+(2));
            }
            break;
         case 6:
            if(c=='a')
            {
               newcr->set_skill(SKILL_DRIVING,newcr->get_skill(SKILL_DRIVING)+(1));
               newcr->set_skill(SKILL_SECURITY,newcr->get_skill(SKILL_SECURITY)+(1));
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+1);
            }
            if(c=='b')
            {
               newcr->set_skill(SKILL_SHOTGUN,newcr->get_skill(SKILL_SHOTGUN)+(2));
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+1);
            }
            if(c=='c')
            {
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+1);
               newcr->set_skill(SKILL_HANDTOHAND,newcr->get_skill(SKILL_HANDTOHAND)+(2));
            }
            if(c=='d')
            {
               newcr->set_skill(SKILL_SEDUCTION,newcr->get_skill(SKILL_SEDUCTION)+(1));
               newcr->set_skill(SKILL_RELIGION,newcr->get_skill(SKILL_RELIGION)+(1));
               newcr->adjust_attribute(ATTRIBUTE_HEART,+1);
               gaylawyer=true;
            }
            if(c=='e')
            {
               newcr->set_skill(SKILL_PERSUASION,newcr->get_skill(SKILL_PERSUASION)+(2));
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+1);
            }
            break;
         case 7:
            if(c=='a')
            {
               newcr->set_skill(SKILL_SECURITY,newcr->get_skill(SKILL_SECURITY)+(1));
               newcr->set_skill(SKILL_STEALTH,newcr->get_skill(SKILL_STEALTH)+(1));
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+1);
            }
            if(c=='b')
            {
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+1);
               newcr->set_skill(SKILL_HANDTOHAND,newcr->get_skill(SKILL_HANDTOHAND)+(2));
            }
            if(c=='c')
            {
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+1);
               newcr->set_skill(SKILL_BUSINESS,newcr->get_skill(SKILL_BUSINESS)+(2));
            }
            if(c=='d')
            {
               newcr->adjust_attribute(ATTRIBUTE_HEART,-1);
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+2);
               newcr->set_skill(SKILL_SEDUCTION,newcr->get_skill(SKILL_SEDUCTION)+(2));
            }
            if(c=='e')
            {
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+1);
               newcr->set_skill(SKILL_LAW,newcr->get_skill(SKILL_LAW)+(1));
               newcr->set_skill(SKILL_PERSUASION,newcr->get_skill(SKILL_PERSUASION)+(1));
            }
            break;
         case 8:
            if(c=='a')
            {
               startcar = new Vehicle(*vehicletype[getvehicletype("SPORTSCAR")]);
               // Add heat to the starting vehicle because it is stolen.
               // This is so that you can't immediately sell it at full price.
               startcar->add_heat(10);
               vehicle.push_back(startcar);
               newcr->pref_carid = startcar->id();
            }
            if(c=='b')
            {
               Weapon neww(*weapontype[getweapontype("WEAPON_AUTORIFLE_AK47")]);
               Clip newc(*cliptype[getcliptype("CLIP_ASSAULT")],9);
               newcr->give_weapon(neww,NULL);
               newcr->take_clips(newc,9);
            }
            if(c=='c')
            {
               ledger.force_funds(1000);
            }
            if(c=='d')
            {
               makelawyer=true;
            }
            if(c=='e')
            {
               hasmaps=true;
            }
            break;
         case 9:
            if(c=='a')
            {
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+2);
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+2);
               newcr->set_skill(SKILL_SECURITY,newcr->get_skill(SKILL_SECURITY)+(2));
               newcr->set_skill(SKILL_STEALTH,newcr->get_skill(SKILL_STEALTH)+(2));
               newcr->type = CREATURE_THIEF;
               newcr->type_idname = "CREATURE_THIEF";
               base = SITE_RESIDENTIAL_APARTMENT_UPSCALE;
               ledger.force_funds(ledger.get_funds()+500);

               Armor newa(*armortype[getarmortype("ARMOR_BLACKCLOTHES")]);
               newcr->give_armor(newa,NULL);
            }
            if(c=='b')
            {
               newcr->set_skill(SKILL_RIFLE,newcr->get_skill(SKILL_RIFLE)+(2));
               newcr->set_skill(SKILL_PISTOL,newcr->get_skill(SKILL_PISTOL)+(2));
               newcr->set_skill(SKILL_STREETSENSE,newcr->get_skill(SKILL_STREETSENSE)+(2));
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+2);
               newcr->adjust_attribute(ATTRIBUTE_HEALTH,+2);
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+2);
               newcr->type = CREATURE_GANGMEMBER;
               newcr->type_idname = "CREATURE_GANGMEMBER";
               base = SITE_BUSINESS_CRACKHOUSE;
               recruits = RECRUITS_GANG;
            }
            if(c=='c')
            {
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+4);
               newcr->set_skill(SKILL_SCIENCE,newcr->get_skill(SKILL_SCIENCE)+(2));
               newcr->set_skill(SKILL_COMPUTERS,newcr->get_skill(SKILL_COMPUTERS)+(2));
               newcr->set_skill(SKILL_WRITING,newcr->get_skill(SKILL_WRITING)+(2));
               newcr->set_skill(SKILL_TEACHING,newcr->get_skill(SKILL_TEACHING)+(2));
               newcr->set_skill(SKILL_BUSINESS,newcr->get_skill(SKILL_BUSINESS)+(1));
               newcr->set_skill(SKILL_LAW,newcr->get_skill(SKILL_LAW)+(1));
               newcr->type = CREATURE_COLLEGESTUDENT;
               newcr->type_idname = "CREATURE_COLLEGESTUDENT";
               base = SITE_RESIDENTIAL_APARTMENT;
               ledger.force_funds(ledger.get_funds()+200);
            }
            if(c=='d')
            {
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+1);
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+1);
               newcr->adjust_attribute(ATTRIBUTE_HEALTH,+2);
               newcr->set_skill(SKILL_FIRSTAID,newcr->get_skill(SKILL_FIRSTAID)+(2));
               newcr->set_skill(SKILL_STREETSENSE,newcr->get_skill(SKILL_STREETSENSE)+(2));
               newcr->type = CREATURE_HSDROPOUT;
               newcr->type_idname = "CREATURE_HSDROPOUT";
               base = SITE_RESIDENTIAL_SHELTER;

               newcr->adjust_attribute(ATTRIBUTE_HEART,+1);
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+1);
               newcr->adjust_attribute(ATTRIBUTE_AGILITY,+1);
               newcr->adjust_attribute(ATTRIBUTE_STRENGTH,+1);
               newcr->adjust_attribute(ATTRIBUTE_HEALTH,+1);
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+1);
            }
            if(c=='e')
            {
               newcr->adjust_attribute(ATTRIBUTE_CHARISMA,+2);
               newcr->adjust_attribute(ATTRIBUTE_INTELLIGENCE,+2);
               newcr->set_skill(SKILL_LAW,newcr->get_skill(SKILL_LAW)+(1));
               newcr->set_skill(SKILL_WRITING,newcr->get_skill(SKILL_WRITING)+(1));
               newcr->set_skill(SKILL_PERSUASION,newcr->get_skill(SKILL_PERSUASION)+(2));
               newcr->type = CREATURE_POLITICALACTIVIST;
               newcr->type_idname = "CREATURE_POLITICALACTIVIST";
               base = SITE_RESIDENTIAL_TENEMENT;
               ledger.force_funds(ledger.get_funds()+50);
               newcr->juice+=50;
            }
            break;
      }
   }

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(2,2);
   addstr("A NEW CONSERVATIVE ERA", gamelog);
   gamelog.newline();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(4,2);
   addstr("The Year is ", gamelog);
   addstr(year, gamelog);
   addstr(".", gamelog);
   move(6,2);
   gamelog.newline();
   addstr("Conservative President ", gamelog);
   char president[80];
   generate_name(president,GENDER_WHITEMALEPATRIARCH);
   addstr(president, gamelog);
   addstr(" ends his second term with approval", gamelog);
   move(7,2);
   gamelog.newline();
   addstr("ratings in the high 70s, and is succeeded by hardcore Arch-Conservative", gamelog);
   move(8,2);
   gamelog.newline();
   addstr(execname[EXEC_PRESIDENT], gamelog);
   addstr(".", gamelog);
   gamelog.nextMessage();

   move(10,2);
   addstr("With Conservatives sweeping into power in the House of Representatives", gamelog);
   move(11,2);
   gamelog.newline();
   addstr("and Senate, and a Conservative majority in the Supreme Court of the", gamelog);
   move(12,2);
   gamelog.newline();
   addstr("United States, commentators are hailing it as the beginning of a new", gamelog);
   move(13,2);
   gamelog.newline();
   addstr("Conservative era.", gamelog);
   gamelog.nextMessage();

   move(15,2);
   set_color(COLOR_RED,COLOR_BLACK,1);
   addstr("President ", gamelog);
   addstr(execname[EXEC_PRESIDENT], gamelog);
   addstr(" has asked the new Congress to move quickly", gamelog);
   move(16,2);
   gamelog.newline();
   addstr("to rubber stamp his radical Arch-Conservative agenda. ", gamelog);
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("The left seems", gamelog);
   gamelog.newline();
   move(17,2);
   addstr("powerless to stop this imminent trampling of Liberal Sanity and Justice.", gamelog);
   gamelog.nextMessage();

   move(19,2);
   addstr("In this dark time, the Liberal Crime Squad is born...", gamelog);
   gamelog.nextMessage();

   getkey();

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("What is your name to the People?");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(1,0);
   addstr("Press enter to be known by your real name instead.");

   enter_name(2,0,newcr->name,CREATURE_NAMELEN,newcr->propername);

   pool.push_back(newcr);

   make_world(hasmaps);

   squadst *newsq=new squadst;
   newsq->id=0;cursquadid++;
   newsq->squad[0]=newcr;
   newcr->squadid=0;
   strcpy(newsq->name,"The Liberal Crime Squad");
   for(int l=0;l<len(location);l++)
   {
      if(location[l]->type==base)
      {
         newcr->base=l;
         newcr->location=l;
         if(startcar) startcar->set_location(l);
         switch(base)
         {
         case SITE_RESIDENTIAL_APARTMENT_UPSCALE:location[l]->renting=500;break;
         case SITE_RESIDENTIAL_APARTMENT:location[l]->renting=200;break;
         case SITE_RESIDENTIAL_TENEMENT:location[l]->renting=100;break;
         case SITE_BUSINESS_CRACKHOUSE:
            location[l]->renting=RENTING_PERMANENT;
            location[l]->compound_stores+=100;
            break;
         }
         location[l]->newrental=1;

         switch(recruits)
         {
         case RECRUITS_GANG:
            for(int i=0;i<4;i++)
            {
               Creature* recruit = new Creature;
               makecreature(*recruit,CREATURE_GANGMEMBER);
               if(recruit->get_weapon().get_itemtypename() == "WEAPON_AUTORIFLE_AK47" ||
                  recruit->get_weapon().get_itemtypename() == "WEAPON_SMG_MP5" ||
                 !recruit->is_armed())
               {
                  Weapon w(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")]);
                  recruit->give_weapon(w,NULL);
                  Clip c(*cliptype[getcliptype("CLIP_9")],4);
                  recruit->take_clips(c,4);
                  recruit->reload(false);
               }

               recruit->align=ALIGN_LIBERAL;
               recruit->set_attribute(ATTRIBUTE_HEART,
                                      recruit->get_attribute(ATTRIBUTE_HEART,false)+
                                      recruit->get_attribute(ATTRIBUTE_WISDOM,false)/2);
               recruit->set_attribute(ATTRIBUTE_WISDOM,
                                      recruit->get_attribute(ATTRIBUTE_WISDOM,false)/2);

               recruit->namecreature();
               strcpy(recruit->name,recruit->propername);

               recruit->location=l;
               recruit->base=l;

               recruit->hireid=newcr->id;

               newsq->squad[i+1]=recruit;
               recruit->squadid=newsq->id;
               pool.push_back(recruit);
            }
            break;
         }

         #ifdef GIVEBLOODYARMOR
         Armor *newa= new Armor(*armortype[getarmortype("ARMOR_CLOTHES")]);
         newa->set_bloody(true);
         location[l]->loot.push_back(newi);
         #endif

         #ifdef HIGHFUNDS
         ledger.force_funds(100000);
         #endif

         break;
      }
   }
   //newcr->juice=0;
   squad.push_back(newsq);
   activesquad=newsq;

   if(makelawyer)
   {
      Creature* lawyer=new Creature;
      makecreature(*lawyer,CREATURE_LAWYER);
      // Make sure lawyer is of the appropriate gender for dating the main character;
      // opposite sex by default, same sex if the option was chosen that mentions
      // homosexuality
      if(gaylawyer)
      {
         lawyer->gender_conservative=lawyer->gender_liberal=newcr->gender_conservative;

         // neutral founder gets neutral partner
      }
      else
      {
         if(newcr->gender_conservative==GENDER_MALE)
            lawyer->gender_liberal=lawyer->gender_conservative=GENDER_FEMALE;

         if(newcr->gender_conservative==GENDER_FEMALE)
            lawyer->gender_liberal=lawyer->gender_conservative=GENDER_MALE;

         // neutral founder gets random partner
      }

      // Ensure the lawyer has good heart/wisdom stats
      if(lawyer->get_attribute(ATTRIBUTE_HEART,false)<newcr->get_attribute(ATTRIBUTE_HEART,false)-2)
         lawyer->adjust_attribute(ATTRIBUTE_HEART,-2);

      lawyer->set_attribute(ATTRIBUTE_WISDOM,1);

      lawyer->namecreature();
      lawyer->flag|=CREATUREFLAG_SLEEPER;
      lawyer->flag|=CREATUREFLAG_LOVESLAVE;
      lawyer->align=ALIGN_LIBERAL;
      lawyer->infiltration=0.3f;
      lawyer->age=28;

      location[lawyer->worklocation]->mapped=1;
      lawyer->hireid=newcr->id;
      pool.push_back(lawyer);
      lawyer->location=lawyer->base=lawyer->worklocation;
   }

   uniqueCreatures.initialize();
}
