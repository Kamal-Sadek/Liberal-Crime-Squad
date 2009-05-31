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


/* creates your founder */
void makecharacter(void)
{
   Creature *newcr=new Creature;
   initliberal(*newcr);
#ifdef HIGHSTATS
   newcr->att[ATTRIBUTE_HEART]=8;
   newcr->att[ATTRIBUTE_WISDOM]=1;
   newcr->att[ATTRIBUTE_INTELLIGENCE]=13;
   newcr->att[ATTRIBUTE_AGILITY]=15;
   newcr->att[ATTRIBUTE_STRENGTH]=14;
   newcr->att[ATTRIBUTE_HEALTH]=16;
	newcr->att[ATTRIBUTE_CHARISMA]=14;
#else
   newcr->att[ATTRIBUTE_HEART]=8;
   newcr->att[ATTRIBUTE_WISDOM]=1;
   newcr->att[ATTRIBUTE_INTELLIGENCE]=3;
   newcr->att[ATTRIBUTE_AGILITY]=5;
   newcr->att[ATTRIBUTE_STRENGTH]=4;
   newcr->att[ATTRIBUTE_HEALTH]=6;
	newcr->att[ATTRIBUTE_CHARISMA]=4;
#endif
   for(int sk=0;sk<SKILLNUM;sk++)newcr->skill[sk]=0;

   char first[2][80];
   char last[80];
   bool gender=(newcr->gender_conservative==GENDER_FEMALE);
   firstname(first[0], GENDER_MALE);
   firstname(first[1], GENDER_FEMALE);
   lastname(last);

   bool choices=true;

   while(1)
   {
      clear();
   
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(4,6);
      addstr("The Founder of the Liberal Crime Squad");

      move(6,2);
      addstr("FIRST NAME: ");
      addstr(first[gender]);
      move(6,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press A to have your parents reconsider)");

      move(8,2);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("LAST NAME: ");
      addstr(last);
      move(8,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press B to be born to a different family)");
      
      move(10,2);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("SEX: ");
      if(newcr->gender_conservative == GENDER_MALE)
      {
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         addstr("Male");
      }
      else
      {
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         addstr("Female");
      }
      move(10,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press C to be born as the other sex)");

      move(12,2);
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
      move(12,30);
      set_color(COLOR_BLACK,COLOR_BLACK,1);
      addstr(" (Press D to toggle childhood)");

      move(15,4);
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      addstr("Press any other key when ready to begin...");

      int c=getch();
      translategetch(c);

      if(c=='a')
      {
         firstname(first[gender],newcr->gender_conservative);
         continue;
      }
      if(c=='b')
      {
         lastname(last);
         continue;
      }
      if(c=='c')
      {
         if(newcr->gender_conservative == GENDER_FEMALE)
            newcr->gender_conservative = GENDER_MALE;
         else
            newcr->gender_conservative = GENDER_FEMALE;

         newcr->gender_liberal = newcr->gender_conservative;

         gender=newcr->gender_conservative-1;
         continue;
      }
      if(c=='d')
      {
         choices = !choices;
         continue;
      }
      break;
   }
   
   strcpy(newcr->propername,first[gender]);
   strcat(newcr->propername," ");
   strcat(newcr->propername,last);
   
   int c;
#ifdef HAVEMAPS
   bool hasmaps=true;
#else
   bool hasmaps=false;
#endif
#ifdef HAVELAWYER
   bool makelawyer=true;
#else
   bool makelawyer=false;
#endif
#ifdef ISGAY
   bool gaylawyer=true;
#else
   bool gaylawyer=false;
#endif

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
      //F - XXX

      char selection = LCSrandom(5);

      switch(q)
      {
         case 0:
            move(2,0);addstr("The day I was born in 1984...");
/*
	- The anniversary of Hitler's suicide. - SKILL_PISTOL 1 (lol)
	- Indira Gandhi Killed
*/
            
            move(5,0);
            if(choices || selection == 0)
               addstr("A - the Polish priest Popieluszko was kidnapped by government agents."); // ...who? -- LK
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
            // Sep. 4, 1984
            
            
            move(17,0);
            addstr("The doctor said I was a ");
            set_color(COLOR_WHITE,COLOR_BLACK,1);
#ifdef TRANNY
            addstr("intersex baby");
#else
            if(newcr->gender_conservative == GENDER_MALE)
               addstr("boy");
            else if(newcr->gender_conservative == GENDER_FEMALE)
               addstr("girl");
            else
               addstr("intersex baby"); // this is a bug if it happens, but hey, whatever :P
#endif
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr(".");

            move(19,0);
            addstr("My parents named me ");
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

         case 1:
            move(2,0);addstr("When I was bad...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - my parents grounded me and hid my toys, but I knew where they put them."); // Toybox
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
               addstr("E - my father lectured me endlessly, trying to make me think like him."); // Wisdom?
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
               addstr("A - I was mischevious, and always up to something.");
            //SKILL_DISGUISE 1
            //ATTRIBUTE_AGILITY 1
            //SKILL_STEALTH 1 XXX
            //SKILL_SECURITY 1 XXX
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
	    //ATTRIBUTE_AGILITY 1 XXX
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I was the class clown.  I even had some friends.");
            //SKILL_PERSUASION 1
            //ATTRIBUTE_CHARISMA 1
            break;
         case 3:
            move(2,0);addstr("When I turned 10..."); // Happy birthday!
            move(5,0);
            if(choices || selection == 0)
               addstr("A - my parents divorced.  Whenever I talked, they argued, so I stayed quiet.");
            //SKILL_STEALTH 1 -- lol
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
               addstr("D - my parents divorced.  Mom slept with the divorce lawyer."); // *Oh* mom...
            //SKILL_SEDUCTION 1
            move(13,0);
            if(choices || selection == 4)
               addstr("E - my parents divorced.  It still hurts to read my old diary."); // Dear Diary...
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
               addstr("B - I played guitar in a grunge band.  We sucked, but so did life."); // ♩ Life Sucks ♩ Then You Die ♩ Fuck The World ♩ Let's Get High ♩
            //SKILL_MUSIC 2
            //ATTRIBUTE_CHARISMA 2
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I drew things, a lot.  I was drawing a world better than this."); // A world without Conservatives...
            //SKILL_ART 2
            //ATTRIBUTE_HEART 2
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I played violent video games at home.  I was a total outcast."); // What? No acne or weight gain?
            //SKILL_COMPUTERS 2
            //ATTRIBUTE_AGILITY 2
	    //ATTRIBUTE_HEALTH -1 XXX
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I was obsessed with swords, and started lifting weights.");
            //SKILL_SWORD 2
            //ATTRIBUTE_STRENGTH 2
	    //ATTRIBUTE_HEALTH 1 XXX
            break;
         case 5:
            move(2,0);addstr("Things were getting really bad...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - when I stole my first car.  I got a few blocks before I totalled it.");
            //SKILL_DRIVING 1
            //SKILL_SECURITY 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - and I went to live with my dad.  He had been in Nam and he still drank.");
            //SKILL_SHOTGUN 1
            //SKILL_RIFLE 1
	    //SKILL_PSYCHOLOGY 1 XXX How could we forget?
            move(9,0);
            if(choices || selection == 2)
               addstr("C - and I went completely goth.  I had no friends and made costumes by myself.");
            //SKILL_TAILORING 2
	    //SKILL_WRITING -1 XXX Emo poetry
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
               addstr("B - my stepmom shot her ex-husband, my dad, with a shotgun.  She got off."); // FIXME Sounds... sexual... -- LK
            //SKILL_SHOTGUN 2
            //ATTRIBUTE_AGILITY 1
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I tried wrestling for a quarter, desperate to fit in.");
            //ATTRIBUTE_STRENGTH 1
            //SKILL_HANDTOHAND 2
	    //ATTRIBUTE_AGILITY 1 XXX As an ex-wrestler, I can vouch for this -- LK
	    //SKILL_DODGE 1 	  XXX				     And this -- LK
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
	    //SKILL_LEADERSHIP 1 XXX
            break;
         case 7:					
            move(2,0);addstr("I was only 15 when I ran away, and...");
            move(5,0);
            if(choices || selection == 0)
               addstr("A - I started robbing houses:  rich people only.  I was fed up with their crap.");
            //SKILL_SECURITY 1
            //SKILL_STEALTH 1
            //ATTRIBUTE_AGILITY 1
	    //SKILL_THEFT 1 XXX
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
	    //SKILL_COOKING 1 XXX For the occasional day as the fry cook
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I let people pay me for sex.  I needed the money to survive."); // What? No heart loss?
            //ATTRIBUTE_CHARISMA 1
            //SKILL_SEDUCTION 2
	    //SKILL_STREETSENSE 1 XXX
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
               addstr("A - I stole a security uniform.");
	    //Security Uniform
	    //SKILL_THEFT 1 XXX
	    //SKILL_STEALTH 1 XXX
            move(7,0);
            if(choices || selection == 1)
               addstr("B - I bought myself an assault rifle.");
	    //AK47
            move(9,0);
            if(choices || selection == 2)
               addstr("C - I celebrated.  I'd saved a thousand bucks!"); // Not $1000 + $7?
	    //$1000
	    //SKILL_BUSINESS 2 XXX
            move(11,0);
            if(choices || selection == 3)
               addstr("D - I went to a party and met a cool law student.  We've been dating since.");
	    //Sleeper Lawyer
	    //SKILL_SEDUCTION 1 XXX
	    //SKILL_CHARISMA 1 XXX
            move(13,0);
            if(choices || selection == 4)
               addstr("E - I managed to acquire secret maps of several major buildings downtown.");
	    //Maps
	    //SKILL_STREETSENSE 1 XXX
            break;
         case 9:
            move(2,0);addstr("For the past few years, I've been..."); // XXX: Should `few' be a number instead? -- LK
            move(5,0);
            if(choices || selection == 0)
               addstr("A - stealing from Corporations.  I know they're still keeping more secrets.");
            //ATTRIBUTE_INTELLIGENCE 2
            //ATTRIBUTE_AGILITY 2
            //SKILL_DISGUISE 2
            //SKILL_SECURITY 1
            //SKILL_STEALTH 1
            move(7,0);
            if(choices || selection == 1)
               addstr("B - a violent criminal.  Nothing can change me, or stand in my way.");
            //SKILL_RIFLE 2
            //SKILL_PISTOL 2
            //ATTRIBUTE_AGILITY 2
            //ATTRIBUTE_HEALTH 2
            move(9,0);
            if(choices || selection == 2)
               addstr("C - taking college courses.  I can see how much the country needs help.");
            //SKILL_SCIENCE 2
            //SKILL_COMPUTERS 2
            //ATTRIBUTE_INTELLIGENCE 4
            move(11,0);
            if(choices || selection == 3)
               addstr("D - surviving alone, just like anyone.  But we can't go on like this.");
            //SKILL_FIRSTAID 2
            //SKILL_STREETSENSE 2
            //ATTRIBUTE_INTELLIGENCE 1
            //ATTRIBUTE_AGILITY 1
            //ATTRIBUTE_HEALTH 2
            move(13,0);
            if(choices || selection == 4)
               addstr("E - writing my manifesto and refining my image.  I'm ready to lead.");
            //ATTRIBUTE_CHARISMA 2
            //ATTRIBUTE_INTELLIGENCE 2
            //SKILL_LAW 1
            //SKILL_LEADERSHIP 1
            //SKILL_WRITING 2
            move(17,0);
            addstr("I live in ");
            addstr(lcityname);addstr(", and it's about to experience real change.");
            break;
      }

      refresh();

      do
      {
         c=getch();
         translategetch(c);
         if(!choices)c='a'+selection;
      }while(c<'a'||c>'e');

      switch(q)
      {
         case 0:
            // Oct. 19, 1984
            if(c=='a')
            {
               newcr->att[ATTRIBUTE_AGILITY]+=2;
               newcr->birthday_month = 10;
               newcr->birthday_day = 19;
            }
            // Mar. 3, 1984
            if(c=='b')
            {
               newcr->att[ATTRIBUTE_STRENGTH]+=2;
               newcr->birthday_month = 3;
               newcr->birthday_day = 3;
            }
            // Jan. 24, 1984
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
               newcr->birthday_month = 1;
               newcr->birthday_day = 24;
            }
            // Oct. 16, 1984
            if(c=='d')
            {
               newcr->att[ATTRIBUTE_HEART]+=2;
               newcr->birthday_month = 10;
               newcr->birthday_day = 16;
            }
            // Sep. 4, 1984
            if(c=='e')
            {
               newcr->att[ATTRIBUTE_CHARISMA]+=2;
               newcr->birthday_month = 9;
               newcr->birthday_day = 4;
            }
#ifdef OLDFOUND
            newcr->age = year - 1924;
#else
            newcr->age = year - 1984;
#endif
            // Don't count this year in founder's age if starting before birthday
            if(month < newcr->birthday_month ||
               (month==newcr->birthday_month && day<newcr->birthday_day))SKILL_BUSINESS
            {
               newcr->age--;
            }
            break;
         case 1:
            if(c=='a')
            {
               newcr->skill[SKILL_SECURITY]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_HANDTOHAND]+=1;
               newcr->att[ATTRIBUTE_HEALTH]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
					newcr->skill[SKILL_WRITING]+=1;
            }
            if(c=='d')
            {
               newcr->skill[SKILL_PERSUASION]+=1;
               newcr->att[ATTRIBUTE_HEART]+=1;
            }
            if(c=='e')
            {
               newcr->skill[SKILL_PSYCHOLOGY]+=1;
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
            }
            break;
         case 2:
            if(c=='a')
            {
               newcr->skill[SKILL_DISGUISE]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_PSYCHOLOGY]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
               newcr->att[ATTRIBUTE_HEART]-=1;
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
               newcr->skill[SKILL_WRITING]+=1;
            }
            if(c=='d')
            {
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->skill[SKILL_HANDTOHAND]+=1;
            }
            if(c=='e')
            {
               newcr->skill[SKILL_PERSUASION]+=1;
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
            }
            break;
         case 3:
            if(c=='a')
            {
               newcr->skill[SKILL_STEALTH]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_HANDTOHAND]+=1;
            }
            if(c=='c')
            {
               newcr->skill[SKILL_LAW]+=1;
            }
            if(c=='d')
            {
               newcr->skill[SKILL_SEDUCTION]+=1;
            }
            if(c=='e')
            {
               newcr->att[SKILL_WRITING]+=1;
            }
            break;
         case 4:
            if(c=='a')
            {
               newcr->skill[SKILL_SCIENCE]+=2;
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_MUSIC]+=2;
               newcr->att[ATTRIBUTE_CHARISMA]+=2;
            }
            if(c=='c')
            {
               newcr->skill[SKILL_ART]+=2;
               newcr->att[ATTRIBUTE_HEART]+=2;
            }
            if(c=='d')
            {
               newcr->skill[SKILL_COMPUTERS]+=2;
               newcr->att[ATTRIBUTE_AGILITY]+=2;
            }
            if(c=='e')
            {
               newcr->att[ATTRIBUTE_STRENGTH]+=2;
               newcr->skill[SKILL_SWORD]+=2;
            }
            break;
         case 5:
            if(c=='a')
            {
               newcr->skill[SKILL_DRIVING]+=1;
               newcr->skill[SKILL_SECURITY]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_SHOTGUN]+=1;
               newcr->skill[SKILL_RIFLE]+=1;
            }
            if(c=='c')
            {
               newcr->skill[SKILL_TAILORING]+=2;
            }
            if(c=='d')
            {
               newcr->skill[SKILL_RELIGION]+=1;
               newcr->skill[SKILL_PSYCHOLOGY]+=1;
            }
            if(c=='e')
            {
               newcr->skill[SKILL_TEACHING]+=2;
            }
            break;
         case 6:
            if(c=='a')
            {
               newcr->skill[SKILL_DRIVING]+=1;
               newcr->skill[SKILL_SECURITY]+=1;
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_SHOTGUN]+=2;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->skill[SKILL_HANDTOHAND]+=2;
            }
            if(c=='d')
            {
               newcr->skill[SKILL_SEDUCTION]+=1;
               newcr->skill[SKILL_RELIGION]+=1;
               newcr->att[ATTRIBUTE_HEART]+=1;
               gaylawyer=true;
            }
            if(c=='e')
            {
               newcr->skill[SKILL_PERSUASION]+=2;
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
            }
            break;
         case 7:
            if(c=='a')
            {
               newcr->skill[SKILL_SECURITY]+=1;
               newcr->skill[SKILL_STEALTH]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
            }
            if(c=='b')
            {
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->skill[SKILL_HANDTOHAND]+=2;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
               newcr->skill[SKILL_BUSINESS]+=2;
            }
            if(c=='d')
            {
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
               newcr->skill[SKILL_SEDUCTION]+=2;
            }
            if(c=='e')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
               newcr->skill[SKILL_LAW]+=1;
               newcr->skill[SKILL_PERSUASION]+=1;
            }
            break;
         case 8:
            if(c=='a')
            {
               newcr->armor.type=ARMOR_SECURITYUNIFORM;
               newcr->armor.quality='1';
               newcr->armor.flag=0;
            }
            if(c=='b')
            {
               newcr->weapon.type=WEAPON_AUTORIFLE_AK47;
               newcr->weapon.ammo=30;
               newcr->clip[CLIP_ASSAULT]=9;
            }
            if(c=='c')
            {
               funds=1007;
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
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
               newcr->att[ATTRIBUTE_AGILITY]+=2;
               newcr->skill[SKILL_DISGUISE]+=2;
               newcr->skill[SKILL_SECURITY]+=1;
               newcr->skill[SKILL_STEALTH]+=1;
               newcr->type = CREATURE_THIEF;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_RIFLE]+=2;
               newcr->skill[SKILL_PISTOL]+=2;
               newcr->att[ATTRIBUTE_AGILITY]+=2;
               newcr->att[ATTRIBUTE_HEALTH]+=2;
               newcr->type = CREATURE_GANGMEMBER;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=4;
               newcr->skill[SKILL_SCIENCE]+=2;
               newcr->skill[SKILL_COMPUTERS]+=2;
               newcr->type = CREATURE_COLLEGESTUDENT;
            }
            if(c=='d')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
               newcr->att[ATTRIBUTE_HEALTH]+=2;
               newcr->skill[SKILL_FIRSTAID]+=2;
               newcr->skill[SKILL_STREETSENSE]+=2;
               newcr->type = CREATURE_HSDROPOUT;
            }
            if(c=='e')
            {
               newcr->att[ATTRIBUTE_CHARISMA]+=2;
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
               newcr->skill[SKILL_LAW]+=1;
               newcr->skill[SKILL_WRITING]+=2;
               newcr->skill[SKILL_LEADERSHIP]+=1;
               newcr->type = CREATURE_POLITICALACTIVIST;
            }
            break;
      }
   }

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(2,2);
   addstr("A NEW CONSERVATIVE ERA");

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(4,2);
   addstr("The Year is 2009.");
   move(6,2);
   addstr("Conservative President ");
   char president[80];
   generate_name(president,GENDER_WHITEMALEPATRIARCH);
   addstr(president);
   addstr(" ends his second term with approval");
   move(7,2);
   addstr("ratings in the high 70s, and is succeeded by hardcore Arch-Conservative");
   move(8,2);
   addstr(execname[EXEC_PRESIDENT]);
   addstr(".");

   move(10,2);
   addstr("With Conservatives sweeping into power in the House of Representatives");
   move(11,2);
   addstr("and Senate, and a Conservative majority in the Supreme Court of the");
   move(12,2);
   addstr("United States, commentators are hailing it as the beginning of a new");
   move(13,2);
   addstr("Conservative era.");

   move(15,2);
   set_color(COLOR_RED,COLOR_BLACK,1);
   addstr("President ");
   addstr(execname[EXEC_PRESIDENT]);
   addstr(" has asked the new Congress to move quickly");
   move(16,2);
   addstr("to rubber stamp his radical Arch-Conservative agenda. ");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   addstr("The left seems");
   move(17,2);
   addstr("powerless to stop this imminent trampling of Liberal Sanity and Justice.");

   move(19,2);
   addstr("In this dark time, the Liberal Crime Squad is born...");
   refresh();
   getch();

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("What is your name to the People?");
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(1,0);
   addstr("Press enter to be known by your real name instead.");
  
   move(2,0);
   enter_name(newcr->name,CREATURE_NAMELEN,newcr->propername);

   curs_set(0);
   noecho();
   raw_output(TRUE);
   keypad(stdscr,TRUE);

   pool.push_back(newcr);

   //MAKE LOCATIONS
   locationst *newl;

   newl=new locationst;
      strcpy(newl->name,"Downtown");
      strcpy(newl->shortname,"Downtown");
      newl->type=SITE_DOWNTOWN;
      newl->parent=-1;
   location.push_back(newl);

   newl=new locationst;
      strcpy(newl->name,"The Commercial Center");
      strcpy(newl->shortname,"C-District");
      newl->type=SITE_COMMERCIAL;
      newl->parent=-1;
   location.push_back(newl);

   newl=new locationst;
      strcpy(newl->name,"The University District");
      strcpy(newl->shortname,"U-District");
      newl->type=SITE_UDISTRICT;
      newl->parent=-1;
   location.push_back(newl);

   newl=new locationst;
      strcpy(newl->name,"The Industrial District");
      strcpy(newl->shortname,"I-District");
      newl->type=SITE_INDUSTRIAL;
      newl->parent=-1;
   location.push_back(newl);

   newl=new locationst;
      strcpy(newl->name,"On the Outskirts of the City");
      strcpy(newl->shortname,"Outskirts");
      newl->type=SITE_OUTOFTOWN;
      newl->parent=-1;
      newl->needcar=1;
   location.push_back(newl);
	
   newl=new locationst;
      newl->type=SITE_GOVERNMENT_POLICESTATION;
      newl->parent=0;
      initlocation(*newl);
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_COURTHOUSE;
      newl->parent=0;
      initlocation(*newl);
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_FIRESTATION;
      newl->parent=0;
      initlocation(*newl);
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_PRISON;
      newl->parent=4;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_NUCLEAR;
      newl->parent=4;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_INTELLIGENCEHQ;
      newl->parent=4;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_CORPORATE_HEADQUARTERS;
      newl->parent=4;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_CORPORATE_HOUSE;
      newl->parent=4;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_PAWNSHOP;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);
   
   newl=new locationst;
      newl->type=SITE_BUSINESS_HALLOWEEN;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CARDEALERSHIP;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_SHELTER;
      newl->parent=3;
      newl->renting=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_WAREHOUSE;
      newl->parent=3;
      newl->renting=0;
      newl->hidden=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_WAREHOUSE;
      newl->parent=3;
      newl->renting=0;
      newl->hidden=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_POLLUTER;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_POLLUTER;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_MEDIA_CABLENEWS;
      newl->parent=0;
      initlocation(*newl);
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_MEDIA_AMRADIO;
      newl->parent=0;
      initlocation(*newl);
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_APARTMENT_UPSCALE;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_APARTMENT_UPSCALE;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_APARTMENT;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_APARTMENT;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_TENEMENT;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_TENEMENT;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_HOSPITAL_UNIVERSITY;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_HOSPITAL_CLINIC;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_LABORATORY_GENETIC;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_LABORATORY_COSMETICS;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_DEPTSTORE;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_SWEATSHOP;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CRACKHOUSE;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CRACKHOUSE;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CRACKHOUSE;
      newl->parent=3;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_VEGANCOOP;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_JUICEBAR;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_INTERNETCAFE;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CIGARBAR;
      newl->parent=0;
      initlocation(*newl);
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_LATTESTAND;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_OUTDOOR_PUBLICPARK;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   /*newl=new locationst;
      newl->type=SITE_BUSINESS_ARMSDEALER;
      newl->parent=4;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);*/

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_BOMBSHELTER;
      newl->parent=3;
      newl->renting=-2;
      newl->hidden=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_BARANDGRILL;
      newl->parent=0;
      newl->renting=-2;
      newl->hidden=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_OUTDOOR_BUNKER;
      newl->parent=4;
      newl->renting=-2;
      newl->needcar=1;
      newl->hidden=1;
      initlocation(*newl);
   location.push_back(newl);

   squadst *newsq=new squadst;
      newsq->id=0;cursquadid++;
      newsq->squad[0]=newcr;
      newcr->squadid=0;
      strcpy(newsq->name,"The Liberal Crime Squad");
      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
         {
            newcr->base=l;
            newcr->location=l;

            #ifdef GIVEBLOODYARMOR
            itemst *newi=new itemst;
               newi->type=ITEM_ARMOR;
               newi->armor.flag=1;
               newi->armor.quality='1';
               newi->armor.type=ARMOR_CLOTHES;
               newi->number=1;
            location[l]->loot.push_back(newi);
            #endif
            break;
         }
      }
      newcr->juice=0;
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
      }
      else
      {
         if(newcr->gender_conservative==GENDER_MALE)
            lawyer->gender_conservative=GENDER_FEMALE;

         if(newcr->gender_conservative==GENDER_FEMALE)
            lawyer->gender_conservative=GENDER_MALE;
      }

      // Ensure the lawyer has good heart/wisdom stats
      if(lawyer->att[ATTRIBUTE_HEART]<newcr->att[ATTRIBUTE_HEART]-2)
         lawyer->att[ATTRIBUTE_HEART]=newcr->att[ATTRIBUTE_HEART]-2;
      
      lawyer->att[ATTRIBUTE_WISDOM]=1;
      
      lawyer->namecreature();
      lawyer->flag|=CREATUREFLAG_SLEEPER;
      lawyer->flag|=CREATUREFLAG_LOVESLAVE;
      lawyer->align=ALIGN_LIBERAL;
      lawyer->age=28;

      location[lawyer->worklocation]->interrogated=1;
      lawyer->hireid=newcr->id;
      pool.push_back(lawyer);
      lawyer->location=lawyer->base;
   }
}



/* mostly depricated, but called once by makecharacter */
void initliberal(Creature &cr) // XXX: Define `mostly' -- LK
{
   cr.creatureinit();

   cr.prisoner=NULL;

   cr.align=1;
   cr.type=CREATURE_POLITICALACTIVIST;
   cr.money=0;

   cr.namecreature();

   //STARTING SKILLS
   int startsknum=cr.attval(ATTRIBUTE_WISDOM);

   while(startsknum>0)
   {
      cr.skill[LCSrandom(SKILLNUM)]++;
      startsknum--;
   }
}

