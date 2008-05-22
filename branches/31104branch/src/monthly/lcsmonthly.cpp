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



/* monthly - reports the guardian's power to the player */
void guardianupdate(char size, char power)
{
   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(5,2);
   if(size)
   {
      addstr("The monthly Liberal Guardian newspaper");
   }
   else
   {
      addstr("The monthly Liberal Guardian online newsletter");
   }

   addstr(" is published.");

   move(7,2);
   if(power<0)
   {
      addstr("The only readers are Conservatives, who seem to think it's funny.");
   }
   else if(power==0)
   {
      addstr("Unfortunately, nobody seems interested.");
   }
   else if(power<5)
   {
      addstr("Very few people seem to be interested.");
   }
   else if(power<50)
   {
      addstr("A fair number of people are reading it.");
   }
   else if(power<100)
   {
      addstr("Many people are reading it.");
   }
   else if(power<250)
   {
      addstr("The response is very strong. People are changing their minds.");
   }
   else
   {
      addstr("The response is electric. Everyone is talking about this month's");
      move(8,2);
      addstr("Liberal Guardian.");
   }

   getch();
   return;
}



/* monthly - lets the player choose a special edition for the guardian */
int choosespecialedition(char &clearformess)
{
   int page=0;

   char havetype[LOOTNUM];
   for(int l=0;l<LOOTNUM;l++)havetype[l]=0;
   vector<int> loottype;

   //FIND ALL LOOT TYPES
   for(int loc=0;loc<location.size();loc++)
   {
      if(location[loc]->renting==-1)continue;

      for(int l=0;l<location[loc]->loot.size();l++)
      {
         if(location[loc]->loot[l]->type!=ITEM_LOOT)continue;

         if(location[loc]->loot[l]->loottype!=LOOT_CEOPHOTOS&&
            location[loc]->loot[l]->loottype!=LOOT_INTHQDISK&&
            location[loc]->loot[l]->loottype!=LOOT_CORPFILES&&
            location[loc]->loot[l]->loottype!=LOOT_SECRETDOCUMENTS&&
            location[loc]->loot[l]->loottype!=LOOT_POLICERECORDS)continue;

         if(!havetype[location[loc]->loot[l]->loottype])
         {
            loottype.push_back(location[loc]->loot[l]->loottype);
            havetype[location[loc]->loot[l]->loottype]=1;
         }
      }
   }
   for(int sq=0;sq<squad.size();sq++)
   {
      for(int l=0;l<squad[sq]->loot.size();l++)
      {
         if(squad[sq]->loot[l]->type!=ITEM_LOOT)continue;

         if(squad[sq]->loot[l]->loottype!=LOOT_CEOPHOTOS&&
            squad[sq]->loot[l]->loottype!=LOOT_INTHQDISK&&
            squad[sq]->loot[l]->loottype!=LOOT_CORPFILES&&
            squad[sq]->loot[l]->loottype!=LOOT_SECRETDOCUMENTS&&
            squad[sq]->loot[l]->loottype!=LOOT_POLICERECORDS)continue;

         if(!havetype[squad[sq]->loot[l]->loottype])
         {
            loottype.push_back(squad[sq]->loot[l]->loottype);
            havetype[squad[sq]->loot[l]->loottype]=1;
         }
      }
   }

   if(loottype.size()==0)return -1;

   clearformess=1;
   
   //PICK ONE
   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Do you want to run a special edition?");

      int x=1,y=10;
      char str[200],str2[200];

      for(int l=page*18;l<loottype.size()&&l<page*18+18;l++)
      {
         getloot(str2,loottype[l]);
         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,str2);

         move(y,x);
         addstr(str);

         x+=26;
         if(x>53)
         {
            x=1;
            y++;
         }
      }

      //PAGE UP
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<loottype.size())
      {
         move(17,53);
         addnextpagestr();
      }

      move(24,1);
      addstr("X - Not in this month's Liberal Guardian");

      refresh();

      int c=getch();
      translategetch(c);

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<loottype.size())
         {
            //DELETE THE ITEM
            for(int loc=0;loc<location.size();loc++)
            {
               if(location[loc]->renting==-1)continue;

               for(int l=0;l<location[loc]->loot.size();l++)
               {
                  if(location[loc]->loot[l]->type!=ITEM_LOOT)continue;

                  if(location[loc]->loot[l]->loottype==loottype[slot])
                  {
                     delete location[loc]->loot[l];
                     location[loc]->loot.erase(location[loc]->loot.begin() + l);
                     return loottype[slot];
                  }
               }
            }
            for(int sq=0;sq<squad.size();sq++)
            {
               for(int l=0;l<squad[sq]->loot.size();l++)
               {
                  if(squad[sq]->loot[l]->type!=ITEM_LOOT)continue;

                  if(squad[sq]->loot[l]->loottype==loottype[slot])
                  {
                     delete squad[sq]->loot[l];
                     squad[sq]->loot.erase(squad[sq]->loot.begin() + l);
                     return loottype[slot];
                  }
               }
            }

            //WHOOPS!
            return loottype[slot];
         }
      }

      if(c=='x')
      {
         return -1;
      }

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<loottype.size())page++;

   }while(1);

   return -1;
}



/* monthly - guardian - prints liberal guardian special editions */
void printnews(short l,short newspaper)
{
   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   switch(l)
   {
      case LOOT_CEOPHOTOS:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring photos of a major CEO");
         move(7,1);
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(10))
         {
            case 0:
               addstr("engaging in lewd behavior with animals.");
               change_public_opinion(VIEW_ANIMALRESEARCH,15);
               break;
            case 1:addstr("digging up graves and sleeping with the dead.");break;
            case 2:
               addstr("participating in a murder.");
               change_public_opinion(VIEW_POLICEBEHAVIOR,15);
               change_public_opinion(VIEW_JUSTICES,10);
               break;
            case 3:addstr("engaging in heavy bondage.  A cucumber was involved in some way.");break;
            case 4:addstr("tongue-kissing an infamous dictator.");break;
            case 5:
               addstr("making out with an FDA official overseeing the CEO's products.");
               change_public_opinion(VIEW_GENETICS,10);
               change_public_opinion(VIEW_POLLUTION,10);
               break;
            case 6:addstr("castrating himself.");break;
            case 7:addstr("waving a Nazi flag at a supremacist rally.");break;
            case 8:
               addstr("torturing an employee with a hot iron.");
               change_public_opinion(VIEW_SWEATSHOPS,10);
               break;
            case 9:addstr("playing with feces and urine.");break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         move(9,1);
         addstr("This is bound to get the Corporations a little riled up.");

         change_public_opinion(VIEW_CEOSALARY,50);
         change_public_opinion(VIEW_CORPORATECULTURE,50);
         offended_corps=1;
         break;
      case LOOT_CORPFILES:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring Corporate files");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,newspaper*10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,newspaper*10);
         switch(LCSrandom(5))
         {
            case 0:
               addstr("describing a genetic monster created in a lab.");
               change_public_opinion(VIEW_GENETICS,50);
               break;
            case 1:
               addstr("with a list of gay employees entitled \"Homo-workers\".");
               change_public_opinion(VIEW_GAY,50);
               break;
            case 2:
               addstr("containing a memo: \"Terminate the pregnancy, I terminate you.\"");
               change_public_opinion(VIEW_ABORTION,50);
               break;
            case 3:
               addstr("cheerfully describing foreign corporate sweatshops.");
               change_public_opinion(VIEW_SWEATSHOPS,50);
               break;
				case 4:
					addstr("describing an intricate tax scheme.");
					change_public_opinion(VIEW_TAXES,50);
					break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         move(9,1);
         addstr("This is bound to get the Corporations a little riled up.");

         change_public_opinion(VIEW_CEOSALARY,50);
         change_public_opinion(VIEW_CORPORATECULTURE,50);
         offended_corps=1;
         break;
      case LOOT_INTHQDISK:
      case LOOT_SECRETDOCUMENTS:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring CIA and other intelligence files");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(6))
         {
            case 0:addstr("documenting the overthrow of a government.");break;
            case 1:
               addstr("documenting the planned assassination of a Liberal federal judge.");
               change_public_opinion(VIEW_JUSTICES,50);
               break;
            case 2:addstr("containing private information on innocent citizens.");break;
            case 3:
               addstr("documenting \"harmful speech\" made by innocent citizens.");
               change_public_opinion(VIEW_FREESPEECH,50);
               break;
            case 4:
               addstr("used to keep tabs on gay citizens.");
               change_public_opinion(VIEW_GAY,50);
               break;
            case 5:
               addstr("documenting the infiltration of a pro-choice group.");
               change_public_opinion(VIEW_ABORTION,50);
               break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         move(9,1);
         addstr("This is bound to get the Government a little riled up.");

         change_public_opinion(VIEW_INTELLIGENCE,50);
         offended_cia=1;
         break;
      case LOOT_POLICERECORDS:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring police records");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(6))
         {
            case 0:addstr("documenting human rights abuses by the force.");break;
            case 1:addstr("documenting a police torture case.");break;
            case 2:
               addstr("documenting a systematic invasion of privacy by the force.");
               change_public_opinion(VIEW_INTELLIGENCE,15);
               break;
            case 3:addstr("documenting a forced confession.");break;
            case 4:addstr("documenting widespread corruption in the force.");break;
            case 5:
               addstr("documenting gladiatory matches held between prisoners by guards.");
               change_public_opinion(VIEW_PRISONS,50);
               break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         move(9,1);
         addstr("This is bound to get the police a little riled up.");

         change_public_opinion(VIEW_POLICEBEHAVIOR,50);
         offended_cops=1;
         break;
   }
   refresh();
   getch();
}



/* monthly - LCS finances report */
void fundreport(char &clearformess)
{
   if(disbanding)return;

   //MUST HAVE CATEGORIES FOR ALL FUND CHANGES
   if(moneygained_donate>0||
      moneygained_brownies>0||
      moneylost_trouble>0||
      moneylost_manufacture>0||
      moneylost_rent>0||
      moneylost_legal>0||
      moneygained_goods>0||
      moneygained_ccfraud>0||
      moneygained_hustling>0||
      moneygained_thievery>0||
      moneylost_goods>0||
      moneylost_food>0||
      moneylost_training>0||
      moneylost_compound>0||
      moneylost_hostage>0)
   {
      clearformess=1;
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(0,0);
      addstr("Liberal Crime Squad:   Monthly Action Report");

      int y=2;

      long totalmoney=0;

      //DONATIONS
      if(moneygained_donate>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Donations . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_donate,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_donate;
      }

      //BROWNIES
      if(moneygained_brownies>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Brownies. . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_brownies,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_brownies;
      }

      //CCFRAUD
      if(moneygained_ccfraud>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Credit Card Fraud . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_ccfraud,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_ccfraud;
      }

      //HUSTLING
      if(moneygained_hustling>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Hustling. . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_hustling,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_hustling;
      }

      //HUSTLING
      if(moneygained_extortion>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Extortion . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_extortion,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_extortion;
      }

      //THIEVERY
      if(moneygained_thievery>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Thievery. . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_thievery,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_thievery;
      }

      //SALE
      if(moneygained_goods>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Sale of Goods . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_goods,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_goods;
      }

      //PURCHASE
      if(moneylost_goods>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Purchase of Goods . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_goods,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_goods;
      }

      //TROUBLE
      if(moneylost_trouble>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Liberal Disobedience. . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_trouble,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_trouble;
      }

      //RENT
      if(moneylost_rent>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Rent. . . . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_rent,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_rent;
      }

      //Training
      if(moneylost_training>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Training. . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_training,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_training;
      }

      //MANUFACTURE
      if(moneylost_manufacture>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Manufacture . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_manufacture,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_manufacture;
      }

      //LEGAL
      if(moneylost_legal>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Legal Fees. . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_legal,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_legal;
      }

      //Food
      if(moneylost_food>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Groceries Subsidy . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_food,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_food;
      }

      //Dating
      if(moneylost_dating>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Dating. . . . . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_dating,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_dating;
      }

      //COMPOUND
      if(moneylost_compound>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Infrastructure. . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_compound,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_compound;
      }

      //HOSTAGES
      if(moneylost_hostage>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Hostage Tending . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_hostage,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_hostage;
      }

      //TOTAL
      y++;
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(y,0);
      addstr("Total:");

      if(totalmoney>0)set_color(COLOR_GREEN,COLOR_BLACK,1);
      else if(totalmoney<0)set_color(COLOR_RED,COLOR_BLACK,1);
      else set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(y,60);
      char num[20];
      if(totalmoney<0)itoa(-totalmoney,num,10);
      else itoa(totalmoney,num,10);
      addstr("$");
      addstr(num);

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,0);
      addstr("Press any key to reflect on the report.");

      refresh();
      getch();

      moneygained_donate=0;
      moneygained_brownies=0;
      moneygained_goods=0;
      moneygained_ccfraud=0;
      moneygained_hustling=0;
      moneygained_extortion=0;
      moneygained_thievery=0;
      moneylost_goods=0;
      moneylost_trouble=0;
      moneylost_manufacture=0;
      moneylost_rent=0;
      moneylost_training=0;
      moneylost_legal=0;
      moneylost_food=0;
      moneylost_dating=0;
      moneylost_compound=0;
      moneylost_hostage=0;
   }
}



/* monthly - sleeper behavior */
/**********************************************************************
** *JDS*
** ----- The sleeper system has been completely reworked.
** - Sleepers no longer directly inflence the issues. They now affect
** the broad "liberal power" stats across many issues, which are used
** as a kind of monthly liberal roll akin to AM Radio and Cable News.
** - Each sleeper can affect one or more issue, throwing their power
** into the "abstracted debate" on that issue.
** - After all of the sleepers have contributed to the liberal power
** stats, a roll is made on each issue to see whether the liberals
** make background progress on those issues. 
** - Several sleepers have special abilities. Lawyers and Judges, as
** always, can aid your people in the legal system. Police officers,
** corporate managers, CEOs, and agents can all now leak secret
** documents of the appropriate types, and they will make a check
** each month. This will only happen if the homeless shelter is not
** under siege, and "canseethings" is enabled (eg, you're not in prison
** or disbanded or some other situation where your sleeper can't get
** in touch with anyone in your squad).
** - News Anchors and Radio Personalities remain the two most powerful
** sleepers.
**********************************************************************/
void sleepereffect(creaturest &cr,char &clearformess,char canseethings,int *libpower)
{
   int power=(cr.attval(ATTRIBUTE_CHARISMA)+
               cr.attval(ATTRIBUTE_HEART)+cr.attval(ATTRIBUTE_INTELLIGENCE)+
               cr.skill[SKILL_PERSUASION]);

   // Profession specific skills
   switch(cr.type)
   {
      case CREATURE_CRITIC_ART:
         power+=cr.skill[SKILL_WRITING];
      case CREATURE_PAINTER:
      case CREATURE_SCULPTOR:
         power+=cr.skill[SKILL_ART];
         break;
      case CREATURE_CRITIC_MUSIC:
         power+=cr.skill[SKILL_WRITING];
      case CREATURE_MUSICIAN:
         power+=cr.skill[SKILL_MUSIC];
         break;
      case CREATURE_AUTHOR:
      case CREATURE_JOURNALIST:
         power+=cr.skill[SKILL_WRITING];
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         power+=cr.skill[SKILL_WRITING];
      case CREATURE_LAWYER:
         power+=cr.skill[SKILL_LAW];
         break;
      case CREATURE_SCIENTIST_LABTECH:
      case CREATURE_SCIENTIST_EMINENT:
         power+=cr.skill[SKILL_SCIENCE];
         break;
      case CREATURE_CORPORATE_CEO:
      case CREATURE_CORPORATE_MANAGER:
         power+=cr.skill[SKILL_BUSINESS];
         break;
      case CREATURE_PRIEST:
      case CREATURE_NUN:
         power+=cr.skill[SKILL_RELIGION];
         break;
      case CREATURE_EDUCATOR:
         power+=cr.skill[SKILL_INTERROGATION];
         break;
   }

   // Adjust power for super sleepers
   switch(cr.type)
   {
      case CREATURE_CORPORATE_CEO:
      case CREATURE_DEATHSQUAD:
      case CREATURE_EDUCATOR:
         power*=4;
         break;
      case CREATURE_SCIENTIST_EMINENT:
      case CREATURE_ACTOR:
      case CREATURE_GANGUNIT:
         power*=3;
         break;
      default:
         power*=2;
         break;
   }

   int homes=-1; // find homeless shelter
   for(int l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
      {
         homes=l;
      }
   }
   switch(cr.type)
   {
      /* Cultural leaders block - small influence on everything */
      case CREATURE_RADIOPERSONALITY:
         libpower[VIEW_AMRADIO]+=power;
         break;
      case CREATURE_NEWSANCHOR:
         libpower[VIEW_CABLENEWS]+=power;
         break;
      case CREATURE_PAINTER:
      case CREATURE_SCULPTOR:
      case CREATURE_AUTHOR:
      case CREATURE_JOURNALIST:
      case CREATURE_MUSICIAN:
      case CREATURE_CRITIC_ART:
      case CREATURE_CRITIC_MUSIC:
      case CREATURE_ACTOR:
      case CREATURE_PRIEST:
         for(int i=0;i<VIEWNUM;i++)
         {
            libpower[i]+=power/4;
         }
         break;
      /* Legal block - influences an array of social issues */
      case CREATURE_LAWYER:
      case CREATURE_JUDGE_CONSERVATIVE:
         libpower[VIEW_ABORTION]+=power;
         libpower[VIEW_GAY]+=power;
         libpower[VIEW_DEATHPENALTY]+=power;
         libpower[VIEW_FREESPEECH]+=power;
         libpower[VIEW_JUSTICES]+=power;
         libpower[VIEW_INTELLIGENCE]+=power;
         libpower[VIEW_ANIMALRESEARCH]+=power;
         break;
      /* Scientists block */
      case CREATURE_SCIENTIST_LABTECH:
      case CREATURE_SCIENTIST_EMINENT:
         libpower[VIEW_NUCLEARPOWER]+=power;
         libpower[VIEW_ANIMALRESEARCH]+=power;
         libpower[VIEW_GENETICS]+=power;
         break;
      /* Corporate block */
      case CREATURE_CORPORATE_CEO:
         // CEO can leak corporate files to you
         if(!LCSrandom(10)&&!location[homes]->siege.siege&&canseethings) 
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_CORPFILES;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret corporate documents.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         
         libpower[VIEW_CEOSALARY]+=power;
         libpower[VIEW_TAXES]+=power;
         libpower[VIEW_CORPORATECULTURE]+=power;
         break;
      case CREATURE_CORPORATE_MANAGER:
         // Corporate manager can leak corporate files to you
         if(!LCSrandom(70)&&!location[homes]->siege.siege&&canseethings) 
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_CORPFILES;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret corporate documents.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         libpower[VIEW_CEOSALARY]+=power;
         libpower[VIEW_TAXES]+=power;
         libpower[VIEW_CORPORATECULTURE]+=power;
         break;
      /* Law enforcement block */
      case CREATURE_DEATHSQUAD:
         libpower[VIEW_DEATHPENALTY]+=power; // No break
      case CREATURE_SWAT:
      case CREATURE_COP:
      case CREATURE_GANGUNIT:
         // Cops can leak police files to you
         if(!LCSrandom(70)&&!location[homes]->siege.siege&&canseethings)
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_POLICERECORDS;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret police records.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         libpower[VIEW_POLICEBEHAVIOR]+=power;
         break;
      /* Prison block */
      case CREATURE_EDUCATOR:
         libpower[VIEW_DEATHPENALTY]+=power; // no break intended
      case CREATURE_PRISONGUARD:
      case CREATURE_PRISONER:
         libpower[VIEW_PRISONS]+=power;
         break;
      /* Intelligence block */
      case CREATURE_AGENT:
         // Agents can leak intelligence files to you
         if(!LCSrandom(20)&&!location[homes]->siege.siege&&canseethings) 
         {
            itemst *it=new itemst;
            it->type=ITEM_LOOT;
            it->loottype=LOOT_SECRETDOCUMENTS;
            location[homes]->loot.push_back(it);

            erase();
            move(6,1);
            addstr("Sleeper ");
            addstr(cr.name);
            addstr(" has leaked secret intelligence files.");
            move(7,1);
            addstr("They are stashed at the homeless shelter.");
         }
         libpower[VIEW_INTELLIGENCE]+=power;
         break;
      /* No influence at all block - for people were liberal anyway, or have no way of doing any good */
      case CREATURE_WORKER_FACTORY_CHILD:
      case CREATURE_GENETIC:
      case CREATURE_GUARDDOG:
      case CREATURE_JUROR:
      case CREATURE_BUM:
      case CREATURE_CRACKHEAD:
      case CREATURE_TANK:
      case CREATURE_HIPPIE: // too liberal to be a proper sleeper
      case CREATURE_WORKER_FACTORY_UNION: // same
      case CREATURE_JUDGE_LIBERAL: // more again
      case CREATURE_POLITICALACTIVIST: // ??!?!? SAME
      case CREATURE_MUTANT:
         return;
      /* Talk up LCS block -- includes everyone else */
      default:
         libpower[VIEW_LIBERALCRIMESQUAD]+=power;
         libpower[VIEW_LIBERALCRIMESQUADPOS]+=power;
   }
}
