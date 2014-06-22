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

//TODO: Log the monthly report? --Addictgamer



/* monthly - reports the guardian's power to the player */
void guardianupdate(char size, char power)
{
   erase();

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(5,2);
   if(size)
   {
      addstr("The monthly Liberal Guardian newspaper", gamelog);
   }
   else
   {
      addstr("The monthly Liberal Guardian online newsletter", gamelog);
   }

   addstr(" is published.", gamelog);
   gamelog.newline();

   move(7,2);
   if(power<0)
   {
      addstr("The only readers are Conservatives, who seem to think it's funny.", gamelog);
   }
   else if(power==0)
   {
      addstr("Unfortunately, nobody seems interested.", gamelog);
   }
   else if(power<5)
   {
      addstr("Very few people seem to be interested.", gamelog);
   }
   else if(power<50)
   {
      addstr("A fair number of people are reading it.", gamelog);
   }
   else if(power<100)
   {
      addstr("Many people are reading it.", gamelog);
   }
   else if(power<250)
   {
      addstr("The response is very strong. People are changing their minds.", gamelog);
   }
   else
   {
      addstr("The response is electric. Everyone is talking about this month's ", gamelog);
      move(8,2);
      addstr("Liberal Guardian.",  gamelog);
   }
   gamelog.nextMessage();

   getkey();

   return;
}



/* monthly - lets the player choose a special edition for the guardian */
int choosespecialedition(char &clearformess)
{
   int page=0;

   //char havetype[LOOTNUM];
   //for(int l=0;l<LOOTNUM;l++)havetype[l]=0;
   vector<bool> havetype(loottype.size(),false);
   vector<int> loottypeindex;

   //FIND ALL LOOT TYPES
   for(int loc=0;loc<(int)location.size();loc++)
   {
      if(location[loc]->renting==RENTING_NOCONTROL)continue;

      for(int l=0;l<(int)location[loc]->loot.size();l++)
      {
         if(!location[loc]->loot[l]->is_loot())continue;
         //Temporary, maybe put special edition definition into an xml file. -XML
         if(location[loc]->loot[l]->get_itemtypename()!="LOOT_CEOPHOTOS"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_CEOLOVELETTERS"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_CEOTAXPAPERS"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_INTHQDISK"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_CCS_BACKERLIST"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_CORPFILES"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_JUDGEFILES"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_RESEARCHFILES"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_PRISONFILES"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_CABLENEWSFILES"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_AMRADIOFILES"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_SECRETDOCUMENTS"&&
            location[loc]->loot[l]->get_itemtypename()!="LOOT_POLICERECORDS")continue;

         if(!havetype[getloottype(location[loc]->loot[l]->get_itemtypename())])
         {
            loottypeindex.push_back(getloottype(location[loc]->loot[l]->get_itemtypename()));
            havetype[getloottype(location[loc]->loot[l]->get_itemtypename())]=true;
         }
      }
   }
   for(int sq=0;sq<(int)squad.size();sq++)
   {
      for(int l=0;l<(int)squad[sq]->loot.size();l++)
      {
         if(!squad[sq]->loot[l]->is_loot())continue;

         if(squad[sq]->loot[l]->get_itemtypename()!="LOOT_CEOPHOTOS"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_CEOLOVELETTERS"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_CEOTAXPAPERS"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_INTHQDISK"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_CCS_BACKERLIST"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_CORPFILES"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_JUDGEFILES"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_RESEARCHFILES"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_PRISONFILES"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_CABLENEWSFILES"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_AMRADIOFILES"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_SECRETDOCUMENTS"&&
            squad[sq]->loot[l]->get_itemtypename()!="LOOT_POLICERECORDS")continue;

         if(!havetype[getloottype(squad[sq]->loot[l]->get_itemtypename())])
         {
            loottypeindex.push_back(getloottype(squad[sq]->loot[l]->get_itemtypename()));
            havetype[getloottype(squad[sq]->loot[l]->get_itemtypename())]=true;
         }
      }
   }

   if(loottypeindex.size()==0)return -1;

   clearformess=1;

   //PICK ONE
   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Do you want to run a special edition?");

      int x=1,y=10;
      char str[200];

      for(int l=page*18;l<(int)loottypeindex.size()&&l<page*18+18;l++)
      {
         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,loottype[loottypeindex[l]]->get_name());

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
      if((page+1)*18<(int)loottype.size())
      {
         move(17,53);
         addnextpagestr();
      }

      move(24,1);
      addstr("Enter - Not in this month's Liberal Guardian");

      int c=getkey();

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<(int)loottypeindex.size())
         {
            //DELETE THE ITEM
            for(int loc=0;loc<(int)location.size();loc++)
            {
               if(location[loc]->renting==RENTING_NOCONTROL)continue;

               for(int l=0;l<(int)location[loc]->loot.size();l++)
               {
                  if(!location[loc]->loot[l]->is_loot())continue;

                  if(getloottype(location[loc]->loot[l]->get_itemtypename())==loottypeindex[slot])
                  {
                     location[loc]->loot[l]->decrease_number(1);
                     if(location[loc]->loot[l]->get_number()==0)
                     	delete_and_remove(location[loc]->loot,l);
                     return loottypeindex[slot];
                  }
               }
            }
            for(int sq=0;sq<(int)squad.size();sq++)
            {
               for(int l=0;l<(int)squad[sq]->loot.size();l++)
               {
                  if(!squad[sq]->loot[l]->is_loot())continue;

                  if(getloottype(squad[sq]->loot[l]->get_itemtypename())==loottypeindex[slot])
                  {
                     squad[sq]->loot[l]->decrease_number(1);
                     if(squad[sq]->loot[l]->get_number()==0)
                        delete_and_remove(squad[sq]->loot,l);
                     return loottypeindex[slot];
                  }
               }
            }

            //WHOOPS!
            return loottypeindex[slot];
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR)
      {
         return -1;
      }

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<(int)loottype.size())page++;

   }

   return -1;
}



/* monthly - guardian - prints liberal guardian special editions */
void printnews(short li,short newspaper)
{
   if(law[LAW_FREESPEECH]==-2)offended_firemen=1;
   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);

   if(loottype[li]->get_idname()=="LOOT_CEOPHOTOS") // Tmp -XML
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring photos of a major CEO ", gamelog);
      move(7,1);
      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(10))
      {
         case 0:
            addstr("engaging in lewd behavior with animals.", gamelog);
            change_public_opinion(VIEW_ANIMALRESEARCH,15);
            break;
         case 1:addstr("digging up graves and sleeping with the dead.", gamelog);break;
         case 2:
            addstr("participating in a murder.", gamelog);
            change_public_opinion(VIEW_POLICEBEHAVIOR,15);
            change_public_opinion(VIEW_JUSTICES,10);
            break;
         case 3:addstr("engaging in heavy bondage.  A cucumber was involved in some way.", gamelog);break;
         case 4:addstr("tongue-kissing an infamous dictator.", gamelog);break;
         case 5:
            addstr("making out with an FDA official overseeing the CEO's products.", gamelog);
            change_public_opinion(VIEW_GENETICS,10);
            change_public_opinion(VIEW_POLLUTION,10);
            break;
         case 6:addstr("castrating himself.", gamelog);break;
         case 7:addstr("waving a Nazi flag at a supremacist rally.", gamelog);break;
         case 8:
            addstr("torturing an employee with a hot iron.", gamelog);
            change_public_opinion(VIEW_SWEATSHOPS,10);
            break;
         case 9:addstr("playing with feces and urine.", gamelog);break;
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("This is bound to get the Corporations a little riled up.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_CEOSALARY,50);
      change_public_opinion(VIEW_CORPORATECULTURE,50);
      offended_corps=1;
   }
   else if(loottype[li]->get_idname()=="LOOT_CEOLOVELETTERS")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring love letters from a major CEO ", gamelog);
      move(7,1);
      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(8))
      {
         case 0:
            addstr("addressed to his pet dog.  Yikes.", gamelog);
            change_public_opinion(VIEW_ANIMALRESEARCH,15);
            break;
         case 1:
            addstr("to the judge that acquit him in a corruption trial.", gamelog);
            change_public_opinion(VIEW_JUSTICES,15);
            break;
         case 2:
            addstr("to an illicit gay lover.", gamelog);
            change_public_opinion(VIEW_GAY,15);
            break;
         case 3:addstr("to himself.  They're very steamy.", gamelog);break;
         case 4:
            addstr("implying that he has enslaved his houseservants.", gamelog);
            change_public_opinion(VIEW_SWEATSHOPS,10);
            break;
         case 5:
            addstr("to the FDA official overseeing the CEO's products.", gamelog);
            change_public_opinion(VIEW_GENETICS,10);
            change_public_opinion(VIEW_POLLUTION,10);
            break;
         case 6:addstr("that seem to touch on every fetish known to man.", gamelog);break;
         case 7:addstr("promising someone company profits in exchange for sexual favors.", gamelog);break;
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("This is bound to get the Corporations a little riled up.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_CEOSALARY,50);
      change_public_opinion(VIEW_CORPORATECULTURE,50);
      offended_corps=1;
   }
   else if(loottype[li]->get_idname()=="LOOT_CEOTAXPAPERS")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring a major CEO's tax papers ", gamelog);
      move(7,1);
      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(1))
      {
         default:
            addstr("showing that he has engaged in consistent tax evasion.", gamelog);
            change_public_opinion(VIEW_TAXES,25);
            break;
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("This is bound to get the Corporations a little riled up.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_CEOSALARY,50);
      change_public_opinion(VIEW_CORPORATECULTURE,50);
      offended_corps=1;
   }
   else if(loottype[li]->get_idname()=="LOOT_CORPFILES")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring Corporate files ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,newspaper*10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,newspaper*10);
      switch(LCSrandom(5))
      {
         case 0:
            addstr("describing a genetic monster created in a lab.", gamelog);
            change_public_opinion(VIEW_GENETICS,50);
            break;
         case 1:
            addstr("with a list of gay employees entitled \"Homo-workers\".", gamelog);
            change_public_opinion(VIEW_GAY,50);
            break;
         case 2:
            addstr("containing a memo: \"Terminate the pregnancy, I terminate you.\"", gamelog);
            change_public_opinion(VIEW_WOMEN,50);
            break;
         case 3:
            addstr("cheerfully describing foreign corporate sweatshops.", gamelog);
            change_public_opinion(VIEW_SWEATSHOPS,50);
            break;
         case 4:
            addstr("describing an intricate tax scheme.", gamelog);
            change_public_opinion(VIEW_TAXES,50);
            break;
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("This is bound to get the Corporations a little riled up.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_CEOSALARY,50);
      change_public_opinion(VIEW_CORPORATECULTURE,50);
      offended_corps=1;
   }
   else if(loottype[li]->get_idname()=="LOOT_CCS_BACKERLIST")
   {
      move(5,1);
      addstr("The Liberal Guardian runs more than one thousand pages of documents about ", gamelog);
      gamelog.newline();
      move(6,1);
      addstr("the CCS organization, also revealing in extreme detail the names and ", gamelog);
      gamelog.newline();
      move(7,1);
      addstr("responsibilities of Conservative Crime Squad sympathizers and supporters", gamelog);
      gamelog.newline();
      move(8,1);
      addstr("in the state and federal governments. Sections precisely document the", gamelog);
      gamelog.newline();
      move(9,1);
      addstr("extensive planning to create an extra-judicial death squad that would be", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("above prosecution, and could hunt down law-abiding Liberals and act", gamelog);
      gamelog.newline();
      move(11,1);
      addstr("as a foil when no other enemies were present to direct public energy", gamelog);
      gamelog.newline();
      move(12,1);
      addstr("against.", gamelog);

      move(14,1);
      addstr("The scandal reaches into the heart of the Conservative leadership in the", gamelog);
      gamelog.newline();
      move(15,1);
      addstr("country, and the full ramifications of this revelation may not be felt", gamelog);
      gamelog.newline();
      move(16,1);
      addstr("for months. One thing is clear, however, from the immediate public reaction", gamelog);
      gamelog.newline();
      move(17,1);
      addstr("toward the revelations, and the speed with which even AM Radio and Cable", gamelog);
      gamelog.newline();
      move(18,1);
      addstr("News denounce the CCS.", gamelog);
      gamelog.newline();

      move(20,1);
      addstr("This is the beginning of the end for the Conservative Crime Squad.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_INTELLIGENCE,50);
      change_public_opinion(VIEW_CONSERVATIVECRIMESQUAD,100);
      ccsexposure = CCSEXPOSURE_EXPOSED;
   }
   else if(loottype[li]->get_idname()=="LOOT_INTHQDISK"
           || loottype[li]->get_idname()=="LOOT_SECRETDOCUMENTS")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring CIA and other intelligence files ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(6))
      {
         case 0:addstr("documenting the overthrow of a government.", gamelog);break;
         case 1:
            addstr("documenting the planned assassination of a Liberal federal judge.", gamelog);
            change_public_opinion(VIEW_JUSTICES,50);
            break;
         case 2:addstr("containing private information on innocent citizens.", gamelog);break;
         case 3:
            addstr("documenting \"harmful speech\" made by innocent citizens.", gamelog);
            change_public_opinion(VIEW_FREESPEECH,50);
            break;
         case 4:
            addstr("used to keep tabs on gay citizens.", gamelog);
            change_public_opinion(VIEW_GAY,50);
            break;
         case 5:
            addstr("documenting the infiltration of a pro-choice group.", gamelog);
            change_public_opinion(VIEW_WOMEN,50);
            break;
              }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("This is bound to get the Government a little riled up.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_INTELLIGENCE,50);
      offended_cia=1;
   }
   else if(loottype[li]->get_idname()=="LOOT_POLICERECORDS")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring police records ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(6))
      {
         case 0:
            addstr("documenting human rights abuses by the force.", gamelog);
            change_public_opinion(VIEW_TORTURE,15);
            break;
         case 1:
            addstr("documenting a police torture case.", gamelog);
            change_public_opinion(VIEW_TORTURE,50);
            break;
         case 2:
            addstr("documenting a systematic invasion of privacy by the force.", gamelog);
            change_public_opinion(VIEW_INTELLIGENCE,15);
            break;
         case 3:
            addstr("documenting a forced confession.", gamelog);
            break;
         case 4:
            addstr("documenting widespread corruption in the force.", gamelog);
            break;
         case 5:
            addstr("documenting gladiatorial matches held between prisoners by guards.", gamelog);
            change_public_opinion(VIEW_DEATHPENALTY,50);
            change_public_opinion(VIEW_PRISONS,20);
            break;
      }
      move(9,1);
      gamelog.newline();
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_POLICEBEHAVIOR,50);
   }
   else if(loottype[li]->get_idname()=="LOOT_JUDGEFILES")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story with evidence of a Conservative judge ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(2))
      {
         case 0:addstr("taking bribes to acquit murderers.", gamelog);break;
         case 1:addstr("promising Conservative rulings in exchange for appointments.", gamelog);break;
      }
      gamelog.newline();
      move(8,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_JUSTICES,50);
   }
   else if(loottype[li]->get_idname()=="LOOT_RESEARCHFILES")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring research papers ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(4))
      {
         case 0:addstr("documenting horrific animal rights abuses.", gamelog);
            change_public_opinion(VIEW_ANIMALRESEARCH,50);break;
         case 1:addstr("studying the effects of torture on cats.", gamelog);
            change_public_opinion(VIEW_ANIMALRESEARCH,50);break;
         case 2:addstr("covering up the accidental creation of a genetic monster.", gamelog);
            change_public_opinion(VIEW_GENETICS,50);break;
         case 3:addstr("showing human test subjects dying under genetic research.", gamelog);
            change_public_opinion(VIEW_GENETICS,50);break;
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.nextMessage();
   }
   else if(loottype[li]->get_idname()=="LOOT_PRISONFILES")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring prison documents ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      change_public_opinion(VIEW_PRISONS,50);
      switch(LCSrandom(4))
      {
         case 0:addstr("documenting human rights abuses by prison guards.", gamelog);break;
         case 1:
            addstr("documenting a prison torture case.", gamelog);
            change_public_opinion(VIEW_TORTURE,50);
            break;
         case 2:addstr("documenting widespread corruption among prison employees.", gamelog);break;
         case 3:
            addstr("documenting gladiatorial matches held between prisoners by guards.", gamelog);
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_DEATHPENALTY,50);
   }
   else if(loottype[li]->get_idname()=="LOOT_CABLENEWSFILES")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring cable news memos ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(4))
      {
         case 0:addstr("calling their news 'the vanguard of Conservative thought'.", gamelog);break;
         case 1:addstr("mandating negative coverage of Liberal politicians.", gamelog);break;
         case 2:addstr("planning to drum up a false scandal about a Liberal figure.", gamelog);break;
         case 3:addstr("instructing a female anchor to 'get sexier or get a new job'.", gamelog);
            break;
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("This is bound to get the Conservative masses a little riled up.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_CABLENEWS,50);
      offended_cablenews=1;
   }
   else if(loottype[li]->get_idname()=="LOOT_AMRADIOFILES")
   {
      move(6,1);
      addstr("The Liberal Guardian runs a story featuring AM radio plans ", gamelog);
      move(7,1);

      change_public_opinion(VIEW_LIBERALCRIMESQUAD,10);
      change_public_opinion(VIEW_LIBERALCRIMESQUADPOS,10);
      switch(LCSrandom(3))
      {
         case 0:addstr("calling listeners 'sheep to be told what to think'.", gamelog);break;
         case 1:addstr("saying 'it's okay to lie, they don't need the truth'.", gamelog);break;
         case 2:addstr("planning to drum up a false scandal about a Liberal figure.", gamelog);break;
      }
      gamelog.newline();
      move(9,1);
      addstr("The major networks and publications take it up and run it for weeks.", gamelog);
      gamelog.newline();
      move(10,1);
      addstr("This is bound to get the Conservative masses a little riled up.", gamelog);
      gamelog.nextMessage();

      change_public_opinion(VIEW_AMRADIO,50);
      offended_cablenews=1;
   }

   getkey();
}



/* monthly - LCS finances report */
void fundreport(char &clearformess)
{
   if(disbanding)return;

   clearformess=1;
   erase();

   int y=2;
   int totalmoney=0;
   bool showledger = false;
   char entryname[80];
   char num[20];

   for(int i=0;i<INCOMETYPENUM;i++)
   {
      if(ledger.income[i] != 0)
      {
         showledger = true;
         move(y,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ");

         set_color(COLOR_GREEN,COLOR_BLACK,0);
         itoa(ledger.income[i],num,10);
         addstr("$");
         addstr(num);

         switch(i)
         {
         case INCOME_DONATIONS:
            strcpy(entryname,"Donations");
            break;
         case INCOME_BROWNIES:
            strcpy(entryname,"Brownies");
            break;
         case INCOME_PAWN:
            strcpy(entryname,"Pawning Goods");
            break;
         case INCOME_SKETCHES:
            strcpy(entryname,"Drawing Sales");
            break;
         case INCOME_TSHIRTS:
            strcpy(entryname,"T-Shirt Sales");
            break;
         case INCOME_BUSKING:
            strcpy(entryname,"Street Music");
            break;
         case INCOME_CARS:
            strcpy(entryname,"Car Sales");
            break;
         case INCOME_CCFRAUD:
            strcpy(entryname,"Credit Card Fraud");
            break;
         case INCOME_PROSTITUTION:
            strcpy(entryname,"Prostitution");
            break;
         case INCOME_HUSTLING:
            strcpy(entryname,"Hustling");
            break;
         case INCOME_EXTORTION:
            strcpy(entryname,"Extortion");
            break;
         case INCOME_THIEVERY:
            strcpy(entryname,"Thievery");
            break;
         case INCOME_EMBEZZLEMENT:
            strcpy(entryname,"Embezzlement");
            break;
         default:
            strcpy(entryname,"Other Income");
            break;
         }
         move(y,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(entryname);
         y++;

         totalmoney+=ledger.income[i];
         ledger.income[i]=0;
      }
   }

   for(int i=0;i<EXPENSETYPENUM;i++)
   {
      if(ledger.expense[i] != 0)
      {
         showledger = true;
         move(y,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ");

         set_color(COLOR_RED,COLOR_BLACK,0);
         itoa(ledger.expense[i],num,10);
         addstr("$");
         addstr(num);

         switch(i)
         {
         case EXPENSE_SHOPPING:
            strcpy(entryname,"Purchasing Goods");
            break;
         case EXPENSE_TROUBLEMAKING:
            strcpy(entryname,"Activism");
            break;
         case EXPENSE_RENT:
            strcpy(entryname,"Rent");
            break;
         case EXPENSE_TRAINING:
            strcpy(entryname,"Training");
            break;
         case EXPENSE_MANUFACTURE:
            strcpy(entryname,"Manufacturing");
            break;
         case EXPENSE_LEGAL:
            strcpy(entryname,"Legal Fees");
            break;
         case EXPENSE_FOOD:
            strcpy(entryname,"Groceries");
            break;
         case EXPENSE_RECRUITMENT:
            strcpy(entryname,"Recruitment");
            break;
         case EXPENSE_DATING:
            strcpy(entryname,"Dating");
            break;
         case EXPENSE_COMPOUND:
            strcpy(entryname,"Safehouse Investments");
            break;
         case EXPENSE_HOSTAGE:
            strcpy(entryname,"Hostage Tending");
            break;
         case EXPENSE_CONFISCATED:
            strcpy(entryname,"Confiscated");
            break;
         case EXPENSE_TSHIRTS:
            strcpy(entryname,"T-Shirt Materials");
            break;
         case EXPENSE_SKETCHES:
            strcpy(entryname,"Drawing Materials");
            break;
         case EXPENSE_CARS:
            strcpy(entryname,"New Cars");
            break;
         case EXPENSE_TRAVEL:
            strcpy(entryname,"Travel");
            break;
         default:
            strcpy(entryname,"Other Expenses");
            break;
         }
         move(y,0);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr(entryname);
         y++;

         totalmoney-=ledger.expense[i];
         ledger.expense[i]=0;
      }
   }

   if(showledger == true)
   {
      set_color(COLOR_WHITE,COLOR_BLACK,1);

      move(0,0);
      addstr("Liberal Crime Squad:   Monthly Action Report");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(++y,0);
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

      getkey();
   }
}



