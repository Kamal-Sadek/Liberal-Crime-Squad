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
      if(location[loc]->renting==RENTING_NOCONTROL)continue;

      for(int l=0;l<location[loc]->loot.size();l++)
      {
         if(location[loc]->loot[l]->type!=ITEM_LOOT)continue;

         if(location[loc]->loot[l]->loottype!=LOOT_CEOPHOTOS&&
            location[loc]->loot[l]->loottype!=LOOT_INTHQDISK&&
            location[loc]->loot[l]->loottype!=LOOT_CORPFILES&&
            location[loc]->loot[l]->loottype!=LOOT_JUDGEFILES&&
            location[loc]->loot[l]->loottype!=LOOT_RESEARCHFILES&&
            location[loc]->loot[l]->loottype!=LOOT_PRISONFILES&&
            location[loc]->loot[l]->loottype!=LOOT_CABLENEWSFILES&&
            location[loc]->loot[l]->loottype!=LOOT_AMRADIOFILES&&
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
            squad[sq]->loot[l]->loottype!=LOOT_JUDGEFILES&&
            squad[sq]->loot[l]->loottype!=LOOT_RESEARCHFILES&&
            squad[sq]->loot[l]->loottype!=LOOT_PRISONFILES&&
            squad[sq]->loot[l]->loottype!=LOOT_CABLENEWSFILES&&
            squad[sq]->loot[l]->loottype!=LOOT_AMRADIOFILES&&
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
               if(location[loc]->renting==RENTING_NOCONTROL)continue;

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
   if(law[LAW_FREESPEECH]==-2)offended_firemen=1;
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
               change_public_opinion(VIEW_WOMEN,50);
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
               change_public_opinion(VIEW_WOMEN,50);
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
               addstr("documenting gladiatorial matches held between prisoners by guards.");
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
      case LOOT_JUDGEFILES:
         move(6,1);
         addstr("The Liberal Guardian runs a story with evidence of a Conservative judge");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(2))
         {
            case 0:addstr("taking bribes to acquit murderers.");break;
            case 1:addstr("promising Conservative rulings in exchange for appointments.");break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         //move(9,1);
         //addstr("This is bound to get the police a little riled up.");

         change_public_opinion(VIEW_JUSTICES,50);
         //offended_cops=1;
         break;
      case LOOT_RESEARCHFILES:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring research papers");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(4))
         {
            case 0:addstr("documenting horrific animal rights abuses.");
               change_public_opinion(VIEW_ANIMALRESEARCH,50);break;
            case 1:addstr("studying the effects of torture on cats.");
               change_public_opinion(VIEW_ANIMALRESEARCH,50);break;
            case 2:addstr("covering up the accidental creation of a genetic monster.");
               change_public_opinion(VIEW_GENETICS,50);break;
            case 3:addstr("showing human test subjects dying under genetic research.");
               change_public_opinion(VIEW_GENETICS,50);break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         //move(9,1);
         //addstr("This is bound to get the police a little riled up.");

         //offended_cops=1;
         break;
      case LOOT_PRISONFILES:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring prison documents");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(4))
         {
            case 0:addstr("documenting human rights abuses by prison guards.");break;
            case 1:addstr("documenting a prison torture case.");break;
            case 2:addstr("documenting widespread corruption among prison employees.");break;
            case 3:
               addstr("documenting gladiatorial matches held between prisoners by guards.");
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         move(9,1);
         addstr("This is bound to get the police a little riled up.");

		 change_public_opinion(VIEW_PRISONS,50);
         offended_cops=1;
         break;
      case LOOT_CABLENEWSFILES:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring cable news memos");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(4))
         {
            case 0:addstr("calling their news 'the vanguard of Conservative thought'.");break;
            case 1:addstr("mandating negative coverage of Liberal politicians.");break;
            case 2:addstr("planning to drum up a false scandal about a Liberal figure.");break;
            case 3:addstr("instructing a female anchor to 'get sexier or get a new job'.");
               break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         move(9,1);
         addstr("This is bound to get the Conservative masses a little riled up.");

         change_public_opinion(VIEW_CABLENEWS,50);
         offended_cablenews=1;
         break;
      case LOOT_AMRADIOFILES:
         move(6,1);
         addstr("The Liberal Guardian runs a story featuring AM radio plans");
         move(7,1);
         
         change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
         change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
         switch(LCSrandom(4))
         {
            case 0:addstr("calling listeners 'sheep to be told what to think'.");break;
            case 1:addstr("saying 'it's okay to lie, they don't need the truth'.");break;
            case 2:addstr("planning to drum up a false scandal about a Liberal figure.");break;
         }
         move(8,1);
         addstr("The major networks and publications take it up and run it for weeks.");
         move(9,1);
         addstr("This is bound to get the Conservative masses a little riled up.");

         change_public_opinion(VIEW_AMRADIO,50);
         offended_cablenews=1;
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
      moneygained_embezzlement>0||
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

      //EMBEZZLEMENT
      if(moneygained_embezzlement>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Embezzlement. . . . . . . . . . . . . . . . . . . . . . . .");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneygained_embezzlement,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney+=moneygained_embezzlement;
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
         addstr("Groceries and Maintenance . . . . . . . . . . . . . . . . .");

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

      //CONFISCATED
      if(moneylost_confiscated>0)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addstr("Confiscated by Law Enforcement. . . . . . . . . . . . . . .");

         set_color(COLOR_RED,COLOR_BLACK,0);
         move(y,60);
         char num[20];
         itoa(moneylost_confiscated,num,10);
         addstr("$");
         addstr(num);

         y++;

         totalmoney-=moneylost_confiscated;
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
      moneygained_embezzlement=0;
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
      moneylost_confiscated=0;
   }
}



