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
   creaturest *newcr=new creaturest;
   initliberal(*newcr);

   newcr->att[ATTRIBUTE_HEART]=8;
   newcr->att[ATTRIBUTE_WISDOM]=1;
   newcr->att[ATTRIBUTE_INTELLIGENCE]=3;
   newcr->att[ATTRIBUTE_AGILITY]=5;
   newcr->att[ATTRIBUTE_STRENGTH]=4;
   newcr->att[ATTRIBUTE_HEALTH]=6;
	newcr->att[ATTRIBUTE_CHARISMA]=4;
   for(int sk=0;sk<SKILLNUM;sk++)newcr->skill[sk]=0;

   name(newcr->propername);
   
   int c;
   bool hasmaps=0;
   bool makejudge=0;

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

      switch(q)
      {
         case 0:
            move(2,0);addstr("The day I was born in 1984...");
            move(5,0);addstr("A - the Polish priest Popieluszko was kidnapped by government agents.");
            //ATTRIBUTE_AGILITY 2
            // Oct. 19, 1984
            move(7,0);addstr("B - was the 3rd anniversary of the assassination attempt on Ronald Reagan.");
            //ATTRIBUTE_STRENGTH 2
            // Mar. 3, 1984
            move(9,0);addstr("C - the Macintosh was introduced.");
            //ATTRIBUTE_INTELLIGENCE 2
            // Jan. 24, 1984
            move(11,0);addstr("D - the Nobel Peace Prize went to Desmond Tutu for opposition to apartheid.");
            //ATTRIBUTE_HEART 2
            // Oct. 16, 1984
            move(13,0);addstr("E - the Sandanista Front won the elections in Nicaragua.");
            //ATTRIBUTE_CHARISMA 2
            // Sep. 4, 1984
            
            
            move(18,0);
            addstr("My parents named me ");
            addstr(newcr->propername);
            addstr(".");
            break;
         case 1:
            move(2,0);addstr("When I was bad...");
            move(5,0);addstr("A - my parents grounded me and hid my toys, but I knew where they put them.");
            //SKILL_SECURITY 1
            //ATTRIBUTE_AGILITY 1
            move(7,0);addstr("B - my father beat me.  I learned to take a punch earlier than most.");
            //SKILL_HANDTOHAND 1
            //ATTRIBUTE_HEALTH 1
            move(9,0);addstr("C - I was sent to my room, where I studied quietly by myself, alone.");
            //SKILL_WRITING 1
            //ATTRIBUTE_INTELLIGENCE 1
            move(11,0);addstr("D - my parents argued with each other about me, but I was never punished.");
            //SKILL_PERSUASION 1
            //ATTRIBUTE_HEART 1
            move(13,0);addstr("E - my father lectured me endlessly, trying to make me think like him.");
            //SKILL_INTERROGATION 1
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
            move(5,0);addstr("A - I was mischevious, and always up to something.");
            //SKILL_DISGUISE 1
            //ATTRIBUTE_AGILITY 1
            move(7,0);addstr("B - I had a lot of repressed anger.  I hurt animals.");
            //SKILL_INTERROGATION 1
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_AGILITY 1
            //ATTRIBUTE_HEART -1 <--- !
            move(9,0);addstr("C - I was at the head of the class, and I worked very hard.");
            //ATTRIBUTE_INTELLIGENCE 1
            //SKILL_WRITING 1
            move(11,0);addstr("D - I was unruly and often fought with the other children.");
            //SKILL_HANDTOHAND 1
            //ATTRIBUTE_STRENGTH 1
            move(13,0);addstr("E - I was the class clown.  I even had some friends.");
            //SKILL_PERSUASION 1
            //ATTRIBUTE_CHARISMA 1
            break;
         case 3:
            move(2,0);addstr("When I turned 10...");
            move(5,0);addstr("A - my parents divorced.  Whenever I talked, they argued, so I stayed quiet.");
            //SKILL_STEALTH 1
            move(7,0);addstr("B - my parents divorced.  Violently.");
            //SKILL_HANDTOHAND 1
            move(9,0);addstr("C - my parents divorced.  Acrimoniously.  I once tripped over the paperwork!");
            //SKILL_LAW 1
            move(11,0);addstr("D - my parents divorced.  Mom slept with the divorce lawyer.");
            //SKILL_SEDUCTION 1
            move(13,0);addstr("E - my parents divorced.  It still hurts to read my old diary.");
            //SKILL_WRITING 1
            break;
         case 4:
            move(2,0);addstr("In junior high school...");
            move(5,0);addstr("A - I was into chemistry.  I wanted to know what made the world tick.");
            //SKILL_SCIENCE 2
            //ATTRIBUTE_INTELLIGENCE 2
            move(7,0);addstr("B - I played guitar in a grunge band.  We sucked, but so did life.");
            //SKILL_MUSIC 2
            //ATTRIBUTE_CHARISMA 2
            move(9,0);addstr("C - I drew things, a lot.  I was drawing a world better than this.");
            //SKILL_ART 2
            //ATTRIBUTE_HEART 2
            move(11,0);addstr("D - I played violent video games at home.  I was a total outcast.");
            //SKILL_COMPUTERS 2
            //ATTRIBUTE_AGILITY 2
            move(13,0);addstr("E - I was obsessed with swords, and started lifting weights.");
            //SKILL_SWORD 2
            //ATTRIBUTE_STRENGTH 2
            break;
         case 5:
            move(2,0);addstr("Things were getting really bad...");
            move(5,0);addstr("A - when I stole my first car.  I got a few blocks before I totalled it.");
            //SKILL_DRIVING 1
            //SKILL_SECURITY 1
            move(7,0);addstr("B - and I went to live with my dad.  He had been in Nam and he still drank.");
            //SKILL_SHOTGUN 1
            //SKILL_RIFLE 1
            move(9,0);addstr("C - and I went completely goth.  I had no friends and made costumes by myself.");
            //SKILL_GARMENTMAKING 2
            move(11,0);addstr("D - when I was sent to religious counseling, just stressing me out more.");
            //SKILL_RELIGION 1
            //SKILL_INTERROGATION 1
            move(13,0);addstr("E - and I tried being a teacher's assistant.  It just made me a target.");
            //SKILL_TEACHING 2
            break;
         case 6:
            move(2,0);addstr("Well, I knew it had reached a crescendo when...");
            move(5,0);addstr("A - I stole a cop car when I was only 14.  I went to juvie for 6 months.");
            //SKILL_DRIVING 1
            //SKILL_SECURITY 1
            //ATTRIBUTE_INTELLIGENCE 1
            move(7,0);addstr("B - my stepmom shot her ex-husband, my dad, with a shotgun.  She got off.");
            //SKILL_SHOTGUN 2
            //ATTRIBUTE_AGILITY 1
            move(9,0);addstr("C - I tried wrestling for a quarter, desperate to fit in.");
            //ATTRIBUTE_STRENGTH 1
            //SKILL_HANDTOHAND 2
            move(11,0);addstr("D - I got caught making out, and now I needed to be 'cured' of homosexuality.");
            //SKILL_SEDUCTION 1
            //SKILL_RELIGION 1
            //ATTRIBUTE_HEART 1
            move(13,0);addstr("E - I resorted to controlling people.  Had my own clique of outcasts.");
            //SKILL_PERSUASION 2
            //ATTRIBUTE_CHARISMA 1
            break;
         case 7:
            move(2,0);addstr("I was only 15 when I ran away, and...");
            move(5,0);addstr("A - I started robbing houses:  rich people only.  I was fed up with their crap.");
            //SKILL_SECURITY 1
            //SKILL_STEALTH 1
            //ATTRIBUTE_AGILITY 1
            move(7,0);addstr("B - I hung out with thugs and beat the shit out of people.");
            //ATTRIBUTE_STRENGTH 1
            //SKILL_HANDTOHAND 2
            move(9,0);addstr("C - I got a horrible job working fast food, smiling as people fed the man.");
            //ATTRIBUTE_CHARISMA 1
            //SKILL_BUSINESS 2
            move(11,0);addstr("D - I let people pay me for sex.  I needed the money to survive.");
            //ATTRIBUTE_CHARISMA 1
            //SKILL_SEDUCTION 2
            move(13,0);addstr("E - I volunteered for a left-wing candidate. It wasn't *real*, though, you know?");
            //ATTRIBUTE_INTELLIGENCE 1
            //SKILL_LAW 1
            //SKILL_PERSUASION 1
            break;
         case 8:
            move(2,0);addstr("Life went on.  On my 18th birthday...");
            move(5,0);addstr("A - I stole a security uniform.");
            move(7,0);addstr("B - I bought myself an assault rifle.");
            move(9,0);addstr("C - I celebrated.  I'd saved a thousand bucks!");
            move(11,0);addstr("D - I went out to party and met a judge.  We've been fast friends ever since.");
            move(13,0);addstr("E - I managed to acquire secret maps of several major buildings downtown.");
            break;
         case 9:
            move(2,0);addstr("For the past few years, I've been...");
            move(5,0);addstr("A - stealing from Corporations.  I know they're still keeping more secrets.");
            //ATTRIBUTE_INTELLIGENCE 2
            //ATTRIBUTE_AGILITY 2
            //SKILL_DISGUISE 2
            //SKILL_SECURITY 1
            //SKILL_STEALTH 1
            move(7,0);addstr("B - a violent criminal.  Nothing can change me, or stand in my way.");
            //SKILL_RIFLE 2
            //SKILL_PISTOL 2
            //ATTRIBUTE_AGILITY 2
            //ATTRIBUTE_HEALTH 2
            move(9,0);addstr("C - taking college courses.  I can see how much the country needs help.");
            //SKILL_SCIENCE 2
            //SKILL_COMPUTERS 2
            //ATTRIBUTE_INTELLIGENCE 4
            move(11,0);addstr("D - surviving alone, just like anyone.  But we can't go on like this.");
            //SKILL_MEDICAL 2
            //SKILL_STREETSENSE 2
            //ATTRIBUTE_INTELLIGENCE 1
            //ATTRIBUTE_AGILITY 1
            //ATTRIBUTE_HEALTH 2
            move(13,0);addstr("E - writing my manifesto and refining my image.  I'm ready to lead.");
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
               newcr->skill[SKILL_INTERROGATION]+=1;
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
               newcr->skill[SKILL_INTERROGATION]+=1;
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
               newcr->skill[SKILL_GARMENTMAKING]+=2;
            }
            if(c=='d')
            {
               newcr->skill[SKILL_RELIGION]+=1;
               newcr->skill[SKILL_INTERROGATION]+=1;
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
               funds=1000;
            }
            if(c=='d')
            {
               makejudge=true;
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
               newcr->skill[SKILL_MEDICAL]+=2;
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

   //MAKE ORGANIZATIONS!  WOOOO!

   //-----
	int ID;
	organization *tOrg;

	for(int i = 0; i < organizationDefManager.getSize(); i++)
	{
		ID = organizationDefManager.getIDByIndex(i);
		tOrg = organizationDefManager.getInstance(ID);
		gOrgManager.addOrg(*tOrg);
	}
	//-----

   //MAKE LOCATIONS
   locationst *newl;

   newl=new locationst;
      strcpy(newl->name,"Downtown");
      strcpy(newl->shortname,"Downtown");
      newl->type=SITE_DOWNTOWN;
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
	
	//Puzz:  Temporary measures to add organizations to these blokes
   vector<int> locID;

   locID = gOrgManager.getOrgsByType("POLICE");
   newl=new locationst;
      newl->type=SITE_GOVERNMENT_POLICESTATION;
      newl->parent=0;
      initlocation(*newl);
	  newl->orgID = locID[0];
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_COURTHOUSE;
      newl->parent=0;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_FIRESTATION;
      newl->parent=0;
     newl->orgID = locID[0];
      initlocation(*newl);
     newl->orgID = locID[0];
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_PRISON;
      newl->parent=3;
      newl->needcar=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
   location.push_back(newl);

   locID = gOrgManager.getOrgsByType("GOVERMENT");

   newl=new locationst;
      newl->type=SITE_INDUSTRY_NUCLEAR;
      newl->parent=3;
      newl->needcar=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_INTELLIGENCEHQ;
      newl->parent=3;
      newl->needcar=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
   location.push_back(newl);

   locID = gOrgManager.getOrgsByType("CORPORATION");

   newl=new locationst;
      newl->type=SITE_CORPORATE_HEADQUARTERS;
      newl->parent=3;
      newl->needcar=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_CORPORATE_HOUSE;
      newl->parent=3;
      newl->needcar=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_SHELTER;
      newl->parent=2;
      newl->renting=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_WAREHOUSE;
      newl->parent=2;
      newl->renting=0;
      newl->hidden=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_WAREHOUSE;
      newl->parent=2;
      newl->renting=0;
      newl->hidden=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_WAREHOUSE;
      newl->parent=2;
      newl->renting=0;
      newl->hidden=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_POLLUTER;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_POLLUTER;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_POLLUTER;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

	locID = gOrgManager.getOrgsByType("CABLENEWS");

   newl=new locationst;
      newl->type=SITE_MEDIA_CABLENEWS;
      newl->parent=0;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
      if(hasmaps)newl->interrogated=1;
   location.push_back(newl);

   locID = gOrgManager.getOrgsByType("AMRADIO");

   newl=new locationst;
      newl->type=SITE_MEDIA_AMRADIO;
      newl->parent=0;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
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
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_APARTMENT;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_TENEMENT;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_TENEMENT;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_HOSPITAL_UNIVERSITY;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_HOSPITAL_CLINIC;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_LABORATORY_GENETIC;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_LABORATORY_COSMETICS;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_PAWNSHOP;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_DEPTSTORE;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_HALLOWEEN;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_SWEATSHOP;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_SWEATSHOP;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CRACKHOUSE;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CRACKHOUSE;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_CRACKHOUSE;
      newl->parent=2;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_VEGANCOOP;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_JUICEBAR;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_INTERNETCAFE;
      newl->parent=1;
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
      newl->type=SITE_BUSINESS_LATTESTAND;
      newl->parent=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_OUTDOOR_PUBLICPARK;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_ARMSDEALER;
      newl->parent=3;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   locID = gOrgManager.getOrgsByType("CCS");

   newl=new locationst;
      newl->type=SITE_RESIDENTIAL_BOMBSHELTER;
      newl->parent=2;
      newl->renting=-2;
      newl->hidden=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_BUSINESS_BARANDGRILL;
      newl->parent=0;
      newl->renting=-2;
      newl->hidden=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_OUTDOOR_BUNKER;
      newl->parent=3;
      newl->renting=-2;
      newl->needcar=1;
      newl->hidden=1;
	  newl->orgID = locID[0];
      initlocation(*newl);
	  newl->orgID = locID[0];
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

   if(makejudge)
   {
      creaturest* judge=new creaturest;
      makecreature(*judge,CREATURE_JUDGE_LIBERAL);
      namecreature(*judge);
      judge->flag|=CREATUREFLAG_SLEEPER;
      judge->hireid=newcr->id;
      pool.push_back(judge);
      judge->location=judge->base;
   }
}



/* mostly depricated, but called once by makecharacter */
void initliberal(creaturest &cr)
{
   cr.creatureinit();

   cr.prisoner=NULL;

   cr.align=1;
   cr.type=CREATURE_POLITICALACTIVIST;
   cr.money=0;

   namecreature(cr);

   //STARTING SKILLS
   int startsknum=cr.attval(ATTRIBUTE_WISDOM);

   while(startsknum>0)
   {
      cr.skill[LCSrandom(SKILLNUM)]++;
      startsknum--;
   }
}

