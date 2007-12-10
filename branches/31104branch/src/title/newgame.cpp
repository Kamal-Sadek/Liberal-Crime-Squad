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


/* creates your founder */
void makecharacter(void)
{
   creaturest *newcr=new creaturest;
   initliberal(*newcr);

   newcr->att[ATTRIBUTE_HEART]=6;
   newcr->att[ATTRIBUTE_WISDOM]=1;
   newcr->att[ATTRIBUTE_INTELLIGENCE]=1;
   newcr->att[ATTRIBUTE_AGILITY]=4;
   newcr->att[ATTRIBUTE_STRENGTH]=4;
   newcr->att[ATTRIBUTE_HEALTH]=6;
	newcr->att[ATTRIBUTE_CHARISMA]=4;
   for(int sk=0;sk<SKILLNUM;sk++)newcr->skill[sk]=0;

   name(newcr->propername);

   for(int q=0;q<10;q++)
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Insight into a Revolution:  My Traumatic Childhood");
      set_color(COLOR_WHITE,COLOR_BLACK,0);

      switch(q)
      {
         case 0:
            move(2,0);addstr("The day I was born in 1984...");
            move(5,0);addstr("A - was the day the Sandinista Front won the elections in Nicaragua.");
            //ATTRIBUTE_CHARISMA 2
            //SKILL_PERSUASION 2
            //ATTRIBUTE_HEART 1
            move(7,0);addstr("B - was the 3rd anniversary of the assassination attempt on Ronald Reagan.");
            //SKILL_PISTOL 2
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_AGILITY 1
            //ATTRIBUTE_HEALTH 1
            move(9,0);addstr("C - was the day the Macintosh was introduced.");
            //ATTRIBUTE_INTELLIGENCE 3
            //SKILL_COMPUTERS 2

            move(12,0);
            addstr("My parents named me ");
            addstr(newcr->propername);
            addstr(".");
            break;
         case 1:
            move(2,0);addstr("When I was bad...");
            move(5,0);addstr("A - my parents argued with each other about me, but I was never punished.");
            //SKILL_PERSUASION 1
            //ATTRIBUTE_CHARISMA 2
            move(7,0);addstr("B - my father beat me.  I learned to take a punch earlier than most.");
            //SKILL_HANDTOHAND 1
            //ATTRIBUTE_HEALTH 1
            //ATTRIBUTE_STRENGTH 1
            move(9,0);addstr("C - I was sent to my room, where I studied quietly by myself, alone.");
            //ATTRIBUTE_INTELLIGENCE 1
				//SKILL_COMPUTERS 1
            //SKILL_WRITING 1
            break;
         case 2:
            move(2,0);addstr("In elementary school...");
            move(5,0);addstr("A - I was the class clown.  I even had some friends.");
            //SKILL_PERSUASION 2
            //ATTRIBUTE_CHARISMA 1
            move(7,0);addstr("B - I was unruly and often fought with the other children.");
            //SKILL_HANDTOHAND 1
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_AGILITY 1
            move(9,0);addstr("C - I was at the head of the class, and I worked very hard.");
            //ATTRIBUTE_INTELLIGENCE 1
            //SKILL_WRITING 1
            //SKILL_COMPUTERS 1
            break;
         case 3:
            move(2,0);addstr("When I turned 10...");
            move(5,0);addstr("A - my parents divorced.  Acrimoniously.  I once tripped over the paperwork!");
            //ATTRIBUTE_INTELLIGENCE 1
            //SKILL_WRITING 1
            //SKILL_LAW 1
            move(7,0);addstr("B - my parents divorced.  Violently.");
            //SKILL_CLUB 1
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_AGILITY 1
            move(9,0);addstr("C - my parents divorced.  My studies suffered but I continued working.");
            //ATTRIBUTE_INTELLIGENCE 2
            //SKILL_COMPUTERS 1
            break;
         case 4:
            move(2,0);addstr("When I hit junior high school...");
            move(5,0);addstr("A - I broke into lockers and was into punk rock.");
            //SKILL_SECURITY 1
            //ATTRIBUTE_CHARISMA 1
            move(7,0);addstr("B - I was into knives and broke things with bats.  My anger was unmanaged.");
            //SKILL_KNIFE 1
            //SKILL_CLUB 1
            move(9,0);addstr("C - I got into chess and go.  I was a total outcast.");
            //ATTRIBUTE_INTELLIGENCE 2
            break;
         case 5:
            move(2,0);addstr("Things were getting really bad...");
            move(5,0);addstr("A - when I stole my first car.  I got a few blocks before I totalled it.");
            //SKILL_DRIVING 1
            //SKILL_SECURITY 1
            //ATTRIBUTE_INTELLIGENCE 1
            move(7,0);addstr("B - and I went to live with my dad.  He had been in Nam and he still drank.");
            //SKILL_SHOTGUN 1
            //SKILL_RIFLE 1
            //ATTRIBUTE_AGILITY 1
            move(9,0);addstr("C - and I went completely goth.  I had no friends and made costumes by myself.");
            //SKILL_DISGUISE 1
            //SKILL_GARMENTMAKING 2
            break;
         case 6:
            move(2,0);addstr("Well, I knew it had reached a crescendo when...");
            move(5,0);addstr("A - I stole a cop car when I was only 14.  I went to juvie for 6 months.");
            //SKILL_DRIVING 1
            //SKILL_SECURITY 1
            //SKILL_INTELLIGENCE 1
            move(7,0);addstr("B - my mom shot her ex-husband, my dad, with a shotgun.  She got off.");
            //SKILL_SHOTGUN 1
            //SKILL_RIFLE 1
            //SKILL_PISTOL 1
            move(9,0);addstr("C - I tried sports for a quarter, desperate to fit in.");
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_AGILITY 2
            break;
         case 7:
            move(2,0);addstr("I was only 15 when I ran away, and...");
            move(5,0);addstr("A - I started robbing houses:  rich people only.  I was fed up with their crap.");
            //SKILL_SECURITY 1
            //SKILL_DISGUISE 1
            //ATTRIBUTE_INTELLIGENCE 1
            move(7,0);addstr("B - I hung out with thugs and beat the shit out of people.");
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_HEALTH 1
            //SKILL_HANDTOHAND 1
            move(9,0);addstr("C - I volunteered for a left-wing candidate. It wasn't *real*, though, you know?");
            //ATTRIBUTE_HEART 1
            //SKILL_LAW 1
            //SKILL_PERSUASION 1
            break;
         case 8:
            move(2,0);addstr("Life went on.  On my 18th birthday...");
            move(5,0);addstr("A - I stole a security uniform.");
            move(7,0);addstr("B - I bought myself an assault rifle.");
            move(9,0);addstr("C - I celebrated.  I'd saved a thousand bucks!");
            break;
         case 9:
            move(2,0);addstr("For the past few years, I've been...");
            move(5,0);addstr("A - stealing from Corporations, but I want to do more.");
            //ATTRIBUTE_INTELLIGENCE 2
            //ATTRIBUTE_AGILITY 1
            //ATTRIBUTE_CHARISMA 1
            //SKILL_DISGUISE 2
            //SKILL_SECURITY 2
            //SKILL_DRIVING 1
            move(7,0);addstr("B - a violent criminal.  Nothing can change that.");
            //SKILL_SHOTGUN 1
            //SKILL_RIFLE 1
            //SKILL_PISTOL 1
            //SKILL_KNIFE 1
            //SKILL_CLUB 1
            //ATTRIBUTE_AGILITY 2
            //ATTRIBUTE_STRENGTH 1
            //ATTRIBUTE_HEALTH 1
            move(9,0);addstr("C - volunteering for prominent liberals, but I know there's a better way.");
            //ATTRIBUTE_INTELLIGENCE 2
            //ATTRIBUTE_HEART 2
            //SKILL_LAW 2
            //SKILL_WRITING 2
            //SKILL_PERSUASION 1
            move(12,0);addstr("I live in ");
            addstr(lcityname);addstr(", and it's about to experience real change.");
            break;
      }

      refresh();

      int c;
      do
      {
         c=getch();
         translategetch(c);
      }while(c!='a'&&c!='b'&&c!='c');

      switch(q)
      {
         case 0:
            if(c=='a')
            {
               newcr->att[ATTRIBUTE_CHARISMA]+=2;
               newcr->att[ATTRIBUTE_HEART]+=1;
               newcr->skill[SKILL_PERSUASION]+=2;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_PISTOL]+=2;
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
               newcr->att[ATTRIBUTE_HEALTH]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=3;
               newcr->skill[SKILL_COMPUTERS]+=2;
            }
            break;
         case 1:
            if(c=='a')
            {
               newcr->skill[SKILL_PERSUASION]+=1;
               newcr->att[ATTRIBUTE_CHARISMA]+=2;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_HANDTOHAND]+=1;
               newcr->att[ATTRIBUTE_HEALTH]+=1;
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
					newcr->skill[SKILL_COMPUTERS]+=1;
               newcr->skill[SKILL_WRITING]+=1;
            }
            break;
         case 2:
            if(c=='a')
            {
               newcr->skill[SKILL_PERSUASION]+=2;
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_HANDTOHAND]+=1;
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
               newcr->skill[SKILL_WRITING]+=1;
               newcr->skill[SKILL_COMPUTERS]+=1;
            }
            break;
         case 3:
            if(c=='a')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
               newcr->skill[SKILL_WRITING]+=1;
               newcr->skill[SKILL_LAW]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_CLUB]+=1;
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
               newcr->skill[SKILL_COMPUTERS]+=1;
            }
            break;
         case 4:
            if(c=='a')
            {
               newcr->skill[SKILL_SECURITY]+=1;
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_KNIFE]+=1;
               newcr->skill[SKILL_CLUB]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
            }
            break;
         case 5:
            if(c=='a')
            {
               newcr->skill[SKILL_DRIVING]+=1;
               newcr->skill[SKILL_SECURITY]+=1;
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_SHOTGUN]+=1;
               newcr->skill[SKILL_RIFLE]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
            }
            if(c=='c')
            {
               newcr->skill[SKILL_DISGUISE]+=1;
               newcr->skill[SKILL_GARMENTMAKING]+=2;
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
               newcr->skill[SKILL_SHOTGUN]+=1;
               newcr->skill[SKILL_RIFLE]+=1;
               newcr->skill[SKILL_PISTOL]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=2;
            }
            break;
         case 7:
            if(c=='a')
            {
               newcr->skill[SKILL_SECURITY]+=1;
               newcr->skill[SKILL_DISGUISE]+=1;
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=1;
            }
            if(c=='b')
            {
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->att[ATTRIBUTE_HEALTH]+=1;
               newcr->skill[SKILL_HANDTOHAND]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_HEART]+=1;
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
            break;
         case 9:
            if(c=='a')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
               newcr->att[ATTRIBUTE_AGILITY]+=1;
               newcr->att[ATTRIBUTE_CHARISMA]+=1;
               newcr->skill[SKILL_DISGUISE]+=2;
               newcr->skill[SKILL_SECURITY]+=2;
               newcr->skill[SKILL_DRIVING]+=1;
            }
            if(c=='b')
            {
               newcr->skill[SKILL_SHOTGUN]+=1;
               newcr->skill[SKILL_RIFLE]+=1;
               newcr->skill[SKILL_PISTOL]+=1;
               newcr->skill[SKILL_KNIFE]+=1;
               newcr->skill[SKILL_CLUB]+=1;
               newcr->att[ATTRIBUTE_AGILITY]+=2;
               newcr->att[ATTRIBUTE_STRENGTH]+=1;
               newcr->att[ATTRIBUTE_HEALTH]+=1;
            }
            if(c=='c')
            {
               newcr->att[ATTRIBUTE_INTELLIGENCE]+=2;
               newcr->att[ATTRIBUTE_HEART]+=2;
               newcr->skill[SKILL_LAW]+=2;
               newcr->skill[SKILL_WRITING]+=2;
               newcr->skill[SKILL_PERSUASION]+=1;
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

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_POLICESTATION;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_COURTHOUSE;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_PRISON;
      newl->parent=3;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_NUCLEAR;
      newl->parent=3;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_GOVERNMENT_INTELLIGENCEHQ;
      newl->parent=3;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_CORPORATE_HEADQUARTERS;
      newl->parent=3;
      newl->needcar=1;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_CORPORATE_HOUSE;
      newl->parent=3;
      newl->needcar=1;
      initlocation(*newl);
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
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_WAREHOUSE;
      newl->parent=2;
      newl->renting=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_INDUSTRY_WAREHOUSE;
      newl->parent=2;
      newl->renting=0;
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

   newl=new locationst;
      newl->type=SITE_MEDIA_CABLENEWS;
      newl->parent=0;
      initlocation(*newl);
   location.push_back(newl);

   newl=new locationst;
      newl->type=SITE_MEDIA_AMRADIO;
      newl->parent=0;
      initlocation(*newl);
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
}



/* mostly depricated, but called once by makecharacter */
void initliberal(creaturest &cr)
{
   cr.creatureinit();

   cr.prisoner=NULL;

   cr.align=1;
   cr.type=CREATURE_BUM;
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

