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

/* prompt user to enter an amount of items to equip, move, or sell */
long prompt_amount(long min, long max)
{
   printparty();

   move(8,15);
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr("     How many?          ");

   char str[100];
   enter_name(8,30,str,100,toCstring(max));

   int amount=atoi(str);
   amount=MAX(amount, min);
   amount=MIN(amount, max);

   return amount;
}

/* review squad equipment */
void equip(vector<Item *> &loot,int loc)
{
   if(activesquad==NULL) return;

   consolidateloot(loot);
   if(loc!=-1) consolidateloot(location[loc]->loot);

   int page=0;
   const char *errmsg = NULL;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Equip the Squad");

      printparty();

      if(errmsg) {
         move(8,20);
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         addstr(errmsg);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         errmsg = NULL;
      }

      int x=1,y=10;
      char str[200];
      //char str2[200];

      for(int l=page*18;l<len(loot)&&l<page*18+18;l++)
      {
         string s=loot[l]->equip_title();

         if(loot[l]->get_number()>1)
            s+=" x"+tostring(loot[l]->get_number());
         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,s);

         move(y,x);
         addstr(str);

         x+=26;
         if(x>53) x=1,y++;
      }

      //PAGE UP
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<len(loot))
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(19,1);
      addstr("Press a letter to equip a Liberal item");
      move(20,1);
      addstr("Press a number to drop that Squad member's Conservative weapon");
      move(21,1);
      addstr("S - Liberally Strip a Squad member");
      move(22,1);
      addstr("Cursors - Increase or decrease ammo allocation");

      if(loc!=-1)
      {
         if(len(location[loc]->loot)) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(23,1);
         addstr("Y - Get things from ");
         addstr(location[loc]->getname(true));

         if(len(loot)) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(23,40);
         addstr("Z - Stash things at ");
         addstr(location[loc]->getname(true));
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,1);
      addstr("Enter - Done");

      int c=getkey();

      bool increaseammo=(c==KEY_UP),decreaseammo=(c==KEY_DOWN);

      if((c>='a'&&c<='r')||increaseammo||decreaseammo)
      {
         int slot=c-'a'+page*18;
         if(increaseammo||decreaseammo) slot=-999;
         else
         {
            if(slot<0||slot>=len(loot)) continue; // Out of range.
            else if(!loot[slot]->is_weapon()
                  &&!loot[slot]->is_armor()
                  &&!loot[slot]->is_clip())
            {
               errmsg="You can't equip that.";
               continue;
            }
         }
         bool choice=true;
         if(activesquad->squad[0])
         {
            choice=false;
            for(int c=1;c<6;c++)
               if(activesquad->squad[c]) //are these slots always filled in order?
               {
                  choice=true;
                  break;
               }
         }
         int c='1';
         if(choice)
         {
            move(8,20);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            if(increaseammo)
               addstr("Choose a Liberal squad member to receive a clip.");
            else if(decreaseammo)
               addstr("Choose a Liberal squad member to drop a clip.");
            else addstr("Choose a Liberal squad member to receive it.");

            c=getkey();
         }

         if(c>='1'&&c<='6')
         {
            Creature *squaddie=activesquad->squad[c-'1'];

            if(squaddie)
            {
               if(decreaseammo)
               {
                  if(len(squaddie->clips))
                  {
                     loot.push_back(squaddie->clips.back()->split(1));
                     if(squaddie->clips.back()->empty())
                        squaddie->clips.pop_back();
                  }
                  else if(!squaddie->get_weapon().uses_ammo())
                  {
                     errmsg="No ammo to drop!";
                     continue;
                  }
                  else
                  {
                     errmsg="No spare clips!";
                     continue;
                  }
                  consolidateloot(loot);
                  continue;
               }
               if(increaseammo)
               {
                  if(!squaddie->get_weapon().uses_ammo())
                  {
                     errmsg="No ammo required!";
                     continue;
                  }
                  slot=-1;
                  for(int sl=0;sl<len(loot);sl++)
                  {
                     if(loot[sl]->is_clip() && squaddie->get_weapon().acceptable_ammo(*loot[sl]))
                     {
                        slot=sl;
                        break;
                     }
                     else if(loot[sl]->is_weapon() && loot[sl]->is_same_type(squaddie->get_weapon())) //For throwing weapons. -XML
                     {
                        Weapon* w=static_cast<Weapon*>(loot[sl]); //cast -XML
                        if(w->is_throwable())
                        {
                           slot=sl;
                           break;
                        }
                     }
                  }
                  if(slot==-1)
                  {
                     errmsg="No ammo available!";
                     continue;
                  }
               }
               int armok=2;
               if((squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
                  (squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF)) armok--;
               if((squaddie->wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
                  (squaddie->wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF)) armok--;
               if(squaddie->special[SPECIALWOUND_NECK]!=1) armok=0;
               if(squaddie->special[SPECIALWOUND_UPPERSPINE]!=1) armok=0;

               if(loot[slot]->is_weapon()&&armok)
               {
                  Weapon* w=static_cast<Weapon*>(loot[slot]); //cast -XML
                  squaddie->give_weapon(*w,&loot);

                  if(loot[slot]->empty()) delete_and_remove(loot,slot);

                  if(page*18>=len(loot)&&page!=0) page--;
               }
               else if(loot[slot]->is_armor())
               {
                  Armor* a = static_cast<Armor*>(loot[slot]); //cast -XML
                  squaddie->give_armor(*a,&loot);

                  if(loot[slot]->empty()) delete_and_remove(loot,slot);

                  if(page*18>=len(loot)&&page!=0) page--;
               }
               else if(loot[slot]->is_clip()&&armok)
               {
                  int space=9-squaddie->count_clips();

                  if(!squaddie->get_weapon().uses_ammo())
                  {  errmsg="Can't carry ammo without a gun."; continue; }
                  else if(!squaddie->get_weapon().acceptable_ammo(*loot[slot]))
                  {  errmsg="That ammo doesn't fit."; continue; }
                  else if(space<1)
                  {  errmsg="Can't carry any more ammo."; continue; }
                  else
                  {
                     int amount=1;
                     if(loot[slot]->get_number()>1&&!increaseammo)
                        amount=prompt_amount(0,MIN(loot[slot]->get_number(),space));

                     squaddie->take_clips(*loot[slot],amount);

                     if(loot[slot]->empty()) delete_and_remove(loot,slot);

                     if(page*18>=len(loot)&&page!=0) page--;
                  }
               }

               consolidateloot(loot);
            }
         }
      }
      if(c=='s')
      {
         bool choice=true;
         if(activesquad->squad[0])
         {
            choice=false;
            for(int c=1;c<6;c++)
               if(activesquad->squad[c])
               {  choice=true; break; }
         }
         int c='1';
         if(choice)
         {
            move(8,20);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Choose a Liberal squad member to strip down.");

            c=getkey();
         }
         if(c>='1'&&c<='6')
            if(activesquad->squad[c-'1'])
            {
               activesquad->squad[c-'1']->strip(&loot);
               consolidateloot(loot);
            }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) return;

      if(loc!=-1)
      {
         if(c=='y'&&len(location[loc]->loot)) moveloot(loot,location[loc]->loot);
         if(c=='z'&&len(loot)) moveloot(location[loc]->loot,loot);
      }

      if(c>='1'&&c<='6')
      {
         int p=c-'1';
         if(activesquad->squad[p]!=NULL)
         {
            activesquad->squad[p]->drop_weapons_and_clips(&loot);
            consolidateloot(loot);
         }
      }

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<len(loot)) page++;

   }
}



/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<Item *> &dest,vector<Item *> &source)
{
   int page=0;

   vector<int> selected(len(source),0);

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Select Objects");

      printparty();

      int x=1,y=10;
      char str[200];

      for(int l=page*18;l<len(source)&&l<page*18+18;l++)
      {
         if(selected[l]) set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         string s=source[l]->equip_title();

         if(source[l]->get_number()>1)
         {
            s+=" ";
            if(selected[l]>0) s+=tostring(selected[l])+"/";
            else s+="x";
            s+=tostring(source[l]->get_number());
         }

         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,s);

         move(y,x);
         addstr(str);

         x+=26;
         if(x>53) x=1,y++;
      }

      //PAGE UP
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<len(source))
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,1);
      addstr("Press a letter to select an item.");
      move(24,1);
      addstr("Enter - Done");

      int c=getkey();

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<len(source))
         {
            if(selected[slot]) selected[slot]=0;
            else if(source[slot]->get_number()>1)
               selected[slot]=prompt_amount(0,source[slot]->get_number());
            else selected[slot]=1;
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<len(source)) page++;
   }

   for(int l=len(source)-1;l>=0;l--) if(selected[l]>0)
   {
      if(source[l]->get_number()<=selected[l])
      {
         dest.push_back(source[l]);
         source.erase(source.begin()+l);
      }
      else
      {
         Item* newit=source[l]->split(selected[l]);
         dest.push_back(newit);
      }
   }

   // Avoid stuff jumping around the next time you equip.
   consolidateloot(dest);
}

/* equipment - assign new bases to the equipment */
void equipmentbaseassign()
{
   int p=0,l=0,page_loot=0,page_loc=0,selectedbase=0;
   bool sortbytype = false;
   vector<Item *> temploot;
   map<Item *,Location *> temploot2;
   for(l=0;l<len(location);l++) for(int l2=0;l2<len(location[l]->loot);l2++)
      if(!location[l]->siege.siege)
      {
         temploot.push_back(location[l]->loot[l2]);
         temploot2[location[l]->loot[l2]]=location[l];
      }
   if(!len(temploot)) return;

   vector<int> temploc;
   for(l=0;l<len(location);l++) if(location[l]->renting>=0&&!location[l]->siege.siege)
      temploc.push_back(l);
   if(!len(temploc)) return;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds();

      move(0,0);
      addstr("Moving Equipment");
      move(1,0);
      addstr("ÄÄÄÄITEMÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄCURRENT LOCATIONÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
      move(1,51);
      addstr("NEW LOCATION");

      int y=2;
      for(p=page_loot*19;p<len(temploot)&&p<page_loot*19+19;p++,y++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(temploot[p]->equip_title());

         mvaddstr(y,25,temploot2[temploot[p]]->getname(true,true));
      }

      y=2;
      for(p=page_loc*9;p<len(temploc)&&p<page_loc*9+9;p++,y++)
      {
         if(p==selectedbase)set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,51);
         addchar(y+'1'-2);addstr(" - ");
         addstr(location[temploc[p]]->getname(true,true));
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to assign a base.  Press a Number to select a base.");
      move(23,0);
      if(sortbytype) addstr("T to sort by location.");
      else addstr("T to sort by type.");
      addstr("  Shift and a Number will move ALL items!");

      move(24,0); // location for either viewing other base pages or loot pages
      if(len(temploc)>9)
      {
         addstr(",. to view other base pages.");
         move(24,34); // we have base pages, so different location for viewing other loot pages
      }
      if(len(temploot)>19)
         addpagestr();

      int c=getkey();

      //PAGE UP (items)
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page_loot>0) page_loot--;
      //PAGE DOWN (items)
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page_loot+1)*19<len(temploot)) page_loot++;

      //PAGE UP (locations)
      if(c==','&&page_loc>0) page_loc--;
      //PAGE DOWN (locations)
      if(c=='.'&&(page_loc+1)*9<len(temploc)) page_loc++;

      //Toggle sorting method
      if(c=='t')
      {
         sortbytype=!sortbytype;
         if(sortbytype) sort(temploot.begin(),temploot.end(),Item::sort_compare);
         else
         {  //Sort by location
            temploot.clear();
            for(l=0;l<len(location);l++) for(int l2=0;l2<len(location[l]->loot);l2++)
               if(!location[l]->siege.siege) temploot.push_back(location[l]->loot[l2]);
         }
      }

      if(c>='a'&&c<='s')
      {
         int p=page_loot*19+c-'a';
         if(p<len(temploot))
            // Search through the old base's stuff for this item
            for(int l2=0;l2<len(temploot2[temploot[p]]->loot);l2++)
               // Remove it from that inventory and move it to the new one
               if(temploot2[temploot[p]]->loot[l2]==temploot[p])
               {
                  temploot2[temploot[p]]->loot.erase(temploot2[temploot[p]]->loot.begin()+l2);
                  location[temploc[selectedbase]]->loot.push_back(temploot[p]);
                  temploot2[temploot[p]]=location[temploc[selectedbase]];
               }
      }
      if(c>='1'&&c<='9')
      {
         int p=page_loc*9+c-'1';
         if(p<len(temploc)) selectedbase=p;
      }
      // Check if the player wants to move all items to a new location,
      // using Shift + a number key.
      const char upnums[]="!@#$%^&*(";
      for(int upnumi=0;upnumi<len(upnums);upnumi++)
      {
         if(c==upnums[upnumi])
         {
            // Set base location
            int basechoice=page_loc*9+upnumi;
            if(basechoice<len(temploc))
            {
               selectedbase=basechoice;
               // Search through the old base's stuff for this item
               for(int p=0;p<len(temploot);p++)
                  // Search through the old base's stuff for this item
                  for(int l2=0;l2<len(temploot2[temploot[p]]->loot);l2++)
                     // Remove it from that inventory and move it to the new one
                     if(temploot2[temploot[p]]->loot[l2]==temploot[p])
                     {
                        temploot2[temploot[p]]->loot.erase(temploot2[temploot[p]]->loot.begin()+l2);
                        location[temploc[selectedbase]]->loot.push_back(temploot[p]);
                        temploot2[temploot[p]]=location[temploc[selectedbase]];
                     }
            }
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}



/* combines multiple items of the same type into stacks */
void consolidateloot(vector<Item *> &loot)
{
   int l,l2;

   //PUT THINGS TOGETHER
   for(l=len(loot)-1;l>=1;l--) for(l2=l-1;l2>=0;l2--)
   {
      loot[l2]->merge(*loot[l]);
      if(loot[l]->empty())
      {  delete_and_remove(loot,l); break; }
   }

   sort(loot.begin(),loot.end(),Item::sort_compare);
}


/* check if the squad has a certain weapon */
char squadhasitem(squadst &sq, const string& type)
{
   if(getweapontype(type)==-1) return 0;

   for(int p=0;p<6;p++) if(sq.squad[p])
      if(sq.squad[p]->get_weapon().get_itemtypename()==type) return 1;

   for(int l=0;l<len(sq.loot);l++)
   {
      if(sq.loot[l]->get_itemtypename()!=type) continue;
      if(sq.loot[l]->is_weapon()&&sq.loot[l]->get_itemtypename()==type) return 1;
   }

   return 0;
}

