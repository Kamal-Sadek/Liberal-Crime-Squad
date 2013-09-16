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



// Helper function for equip and moveloot.
// Prompts for how many items to equip / move.
long prompt_amount(long min, long max)
{
   printparty();

   move(8,15);
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   addstr("     How many?          ");

   refresh();

   char str[100];

   keypad(stdscr,FALSE);
   raw_output(FALSE);
   echo();
   curs_set(1);
   mvgetstr(8,30,str);
   curs_set(0);
   noecho();
   raw_output(TRUE);
   keypad(stdscr,TRUE);

   int amount;
   //If no amount entered, assume the maximum
   //amount is desired
   if(str[0])
   {
      amount = atoi(str);
      amount = MAX(amount, min);
      amount = MIN(amount, max);
   }
   else
   {
      amount = max;
   }


   return amount;
}

/* review squad equipment */
void equip(vector<Item *> &loot,int loc)
{
   if(activesquad==NULL)return;

   consolidateloot(loot);
   if(loc!=-1)consolidateloot(location[loc]->loot);

   int page=0;
   const char *errmsg = NULL;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Equip the Squad");

      printparty();

      if (errmsg != NULL) {
         move(8,20);
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         addstr(errmsg);
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         errmsg = NULL;
      }

      int x=1,y=10;
      char str[200];
      //char str2[200];

      for(int l=page*18;l<(int)loot.size()&&l<page*18+18;l++)
      {
         string s = loot[l]->equip_title();

         if(loot[l]->get_number()>1)
         {
            s += " x" + tostring(loot[l]->get_number());
         }
         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,s.c_str());

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
      if((page+1)*18<(int)loot.size())
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
         if(location[loc]->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(23,1);
         addstr("Y - Get things from ");
         addstr(location[loc]->getname(true));

         if(loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(23,40);
         addstr("Z - Stash things at ");
         addstr(location[loc]->getname(true));
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,1);
      addstr("Enter - Done");

      refresh();

      int c=getch();
      translategetch(c);
      bool increaseammo = (c==KEY_UP);
      bool decreaseammo = (c==KEY_DOWN);

      if((c>='a'&&c<='r')||increaseammo||decreaseammo)
      {
         int slot=c-'a'+page*18;
         if (increaseammo||decreaseammo)
         {
            slot=-999;
         }
         else
         {
            if(slot < 0 || slot >= (int)loot.size())
            {
               // Out of range.
               continue;
            }
            else if (!loot[slot]->is_weapon()
                    && !loot[slot]->is_armor()
                    && !loot[slot]->is_clip())
            {
               errmsg = "You can't equip that.";
               continue;
            }
         }
         bool choice = true;
         if (activesquad->squad[0])
         {
            choice = false;
            for (int c=1; c<6; c++)
            {
               if (activesquad->squad[c]) //are these slots always filled in order?
               {
                  choice=true;
                  break;
               }
            }
         }
         int c;
         if (choice)
         {
            move(8,20);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            if (increaseammo)
            {
               addstr("Choose a Liberal squad member to receive a clip.");
            }
            else if (decreaseammo)
            {
               addstr("Choose a Liberal squad member to drop a clip.");
            }
            else
            {
               addstr("Choose a Liberal squad member to receive it.");
            }

            refresh();

            c=getch();
            translategetch(c);
         }
         else
         {
            c='1'; //only one member so no need to choose.
         }

         if(c>='1'&&c<='6')
         {
            Creature *squaddie = activesquad->squad[c - '1'];

            if(squaddie!=NULL)
            {
               if (decreaseammo)
               {
                  if (!squaddie->clips.empty())
                  {
                     loot.push_back(squaddie->clips.back()->split(1));
                     if (squaddie->clips.back()->empty())
                        squaddie->clips.pop_back();
                  }
                  else if (!squaddie->get_weapon().uses_ammo())
                  {
                     errmsg = "No ammo to drop!";
                     continue;
                  }
                  else
                  {
                     errmsg = "No spare clips!";
                     continue;
                  }
                  consolidateloot(loot);
                  continue;
               }
               if (increaseammo)
               {
                  if (!squaddie->get_weapon().uses_ammo())
                  {
                     errmsg = "No ammo required!";
                     continue;
                  }
                  slot = -1;
                  for (int sl = 0; sl < (int)loot.size(); sl++)
                  {
                     if ((loot[sl]->is_clip() && squaddie->get_weapon().acceptable_ammo(*loot[sl])))
                     {
                        slot = sl;
                        break;
                     }
                     else if (loot[sl]->is_weapon() && loot[sl]->is_same_type(squaddie->get_weapon())) //For throwing weapons. -XML
                     {
                        Weapon* w = static_cast<Weapon*>(loot[sl]); //cast -XML
                        if (w->is_throwable())
                        {
                           slot = sl;
                           break;
                        }
                     }
                  }
                  if (slot == -1)
                  {
                     errmsg = "No ammo available!";
                     continue;
                  }
               }
               int armok=2;
               if((squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
                  (squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
               if((squaddie->wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
                  (squaddie->wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
               if(squaddie->special[SPECIALWOUND_NECK]!=1)armok=0;
               if(squaddie->special[SPECIALWOUND_UPPERSPINE]!=1)armok=0;

               if(loot[slot]->is_weapon() && armok)
               {
                  Weapon* w = static_cast<Weapon*>(loot[slot]); //cast -XML
                  squaddie->give_weapon(*w,&loot);

                  if(loot[slot]->empty())
                     delete_and_remove(loot,slot);

                  if(page*18>=(int)loot.size()&&page!=0)page--;
               }
               else if(loot[slot]->is_armor())
               {
                  Armor* a = static_cast<Armor*>(loot[slot]); //cast -XML
                  squaddie->give_armor(*a,&loot);

                  if(loot[slot]->empty())
                     delete_and_remove(loot,slot);

                  if(page*18>=(int)loot.size()&&page!=0)page--;
               }
               else if(loot[slot]->is_clip() && armok)
               {
                  int space = 9 - squaddie->count_clips();

                  if (!squaddie->get_weapon().uses_ammo())
                  {
                     errmsg = "Can't carry ammo without a gun.";
                     continue;
                  }
                  else if (!squaddie->get_weapon().acceptable_ammo(*loot[slot]))
                  {
                     errmsg = "That ammo doesn't fit.";
                     continue;
                  } else if (space < 1) {
                     errmsg = "Can't carry any more ammo.";
                     continue;
                  } else {
                     int amount = 1;
                     if (loot[slot]->get_number() > 1)
                     {
                        if (increaseammo)
                        {
                           amount = 1;
                        }
                        else
                        {
                           amount = prompt_amount(0,
                              MIN(loot[slot]->get_number(), space));
                        }
                     }

                     squaddie->take_clips(*loot[slot],amount);

                     if(loot[slot]->empty())
                        delete_and_remove(loot,slot);

                     if(page*18>=(int)loot.size()&&page!=0)page--;
                  }
               }

               consolidateloot(loot);
            }
         }
      }
      if(c=='s')
      {
         bool choice = true;
         if (activesquad->squad[0])
         {
            choice = false;
            for (int c=1; c<6; c++)
            {
               if (activesquad->squad[c])
               {
                  choice=true;
                  break;
               }
            }
         }
         int c;
         if (choice)
         {
            move(8,20);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Choose a Liberal squad member to strip down.");

            refresh();

            c=getch();
            translategetch(c);
         }
         else
         {
            c='1'; //only one member so no need to choose.
         }
         if(c>='1'&&c<='6')
         {
            if(activesquad->squad[c-'1']!=NULL)
            {
               activesquad->squad[c-'1']->strip(&loot);
               consolidateloot(loot);
            }
         }
      }

      if(c==10||c==ESC)return;

      if(loc!=-1)
      {
         if(c=='y'&&location[loc]->loot.size()>0)moveloot(loot,location[loc]->loot);
         if(c=='z'&&loot.size()>0)moveloot(location[loc]->loot,loot);
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
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<(int)loot.size())page++;

   }while(1);
}



/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<Item *> &dest,vector<Item *> &source)
{
   int page=0;

   vector<int> selected(source.size(),0);

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Select Objects");

      printparty();

      int x=1,y=10;
      char str[200];
      //char str2[200];

      for(int l=page*18;l<(int)source.size()&&l<page*18+18;l++)
      {
         if(selected[l])set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         string s = source[l]->equip_title();

         if(source[l]->get_number()>1)
         {
            s += " ";
            if(selected[l]>0)
            {
               s += tostring(selected[l]) + "/";
            }
            else
               s += "x";
            s += tostring(source[l]->get_number());
         }

         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,s.c_str());

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
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<(int)source.size())
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,1);
      addstr("Press a letter to select an item.");
      move(24,1);
      addstr("Enter - Done");

      refresh();

      int c=getch();
      translategetch(c);

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<(int)source.size())
         {
            if(selected[slot])
            {
               selected[slot]=0;
            }
            else
            {
               if(source[slot]->get_number()>1)
                  selected[slot] = prompt_amount(0, source[slot]->get_number());
               else selected[slot]=1;
            }
         }
      }

      if(c==10||c==ESC)break;

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<(int)source.size())page++;

   }while(1);

   for(int l=source.size()-1;l>=0;l--)
   {
      if(selected[l]>0)
      {
         if(source[l]->get_number() <= selected[l])
         {
            dest.push_back(source[l]);
            source.erase(source.begin() + l);
         }
         else
         {
            Item* newit=source[l]->split(selected[l]);
            dest.push_back(newit);
         }
      }
   }

   // Avoid stuff jumping around the next time you equip.
   consolidateloot(dest);
}

/* equipment - assign new bases to the equipment */
void equipmentbaseassign(void)
{
   int p=0,l=0,page_loot=0,page_loc=0,selectedbase=0;
   bool sortbytype = false;
   vector<Item *> temploot;
   map<Item *,Location *> temploot2;
   for(l=0;l<(int)location.size();l++)
      for(int l2=0;l2<(int)location[l]->loot.size();l2++)
         if (!location[l]->siege.siege)
         {
            temploot.push_back(location[l]->loot[l2]);
            temploot2[location[l]->loot[l2]]=location[l];
         }

   vector<int> temploc;
   for(l=0;l<(int)location.size();l++)
      if(location[l]->renting>=0)
         if (!location[l]->siege.siege)
            temploc.push_back(l);
   if(temploc.size()==0)return;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      printfunds(0,1,"Money: ");

      move(0,0);
      addstr("Moving Equipment");
      move(1,0);
      addstr("----ITEM-----------------CURRENT LOCATION---------------------------------------");
      move(1,51);
      addstr("NEW LOCATION");

      int y=2;
      //char str[80];
      for(p=page_loot*19;p<(int)temploot.size()&&p<page_loot*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         addstr(temploot[p]->equip_title().c_str());

         mvaddstr(y,25,temploot2[temploot[p]]->getname(true,true));

         y++;
      }

      y=2;
      for(p=page_loc*9;p<(int)temploc.size()&&p<page_loc*9+9;p++)
      {
         if(p==selectedbase)set_color(COLOR_WHITE,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,51);
         addch(y+'1'-2);addstr(" - ");
         addstr(location[temploc[p]]->getname(true,true));

         y++;
      }


      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to assign a base.  Press a Number to select a base.");
      move(23,0);
      if(sortbytype)
      {
         addstr("T to sort by location.");
      }
      else
      {
         addstr("T to sort by type.");
      }
      addstr("  Shift and a Number will move ALL items!");

      if(temploot.size()>19)
      {
         move(23,34);
         addpagestr();
      }
      if(temploc.size()>9)
      {
         move(24,0);
         addstr(",. to view other base pages.");
      }

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page_loot>0)page_loot--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page_loot+1)*19<(int)temploot.size())page_loot++;

      //PAGE UP
      if(c==','&&page_loc>0)page_loc--;
      //PAGE DOWN
      if(c=='.'&&(page_loc+1)*9<(int)temploc.size())page_loc++;

      //Toggle sorting method
      if(c=='t')
      {
         sortbytype = !sortbytype;
         if(sortbytype)
         {
            sort(temploot.begin(),temploot.end(),Item::sort_compare);
         }
         else
         {
            //Sort by location
            temploot.clear();
            for(l=0;l<(int)location.size();l++)
            {
               for(int l2=0;l2<(int)location[l]->loot.size();l2++)
               {
                  if (!location[l]->siege.siege)
                  {
                     temploot.push_back(location[l]->loot[l2]);
                  }
               }
            }
         }
      }

      if(c>='a'&&c<='s')
      {
         int p=page_loot*19+(int)(c-'a');
         if(p<(int)temploot.size())
         {
            // Search through the old base's stuff for this item
            for(int l2=0;l2<(int)temploot2[temploot[p]]->loot.size();l2++)
            {
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
      if(c>='1'&&c<='9')
      {
         int p=page_loc*9+(int)(c-'1');
         if(p<(int)temploc.size())
         {
            selectedbase=p;
         }
      }
      // Check if the player wants to move all items to a new location,
      // using Shift + a number key.
      const char upnums[] = {'!', '@', '#', '$', '%', '^', '&', '*', '('};
      for(int upnumi=0; upnumi < (int)sizeof(upnums); upnumi++)
      {
         if(c==upnums[upnumi])
         {
            // Set base location
            int basechoice=page_loc*9+upnumi;
            if(basechoice<(int)temploc.size())
            {
               selectedbase=basechoice;
               // Search through the old base's stuff for this item
               for(int p=0;p<(int)temploot.size();p++)
               {
                  // Search through the old base's stuff for this item
                  for(int l2=0;l2<(int)temploot2[temploot[p]]->loot.size();l2++)
                  {
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
         }
      }

      if(c==10||c==ESC)break;
   }while(1);
}



/* combines multiple items of the same type into stacks */
void consolidateloot(vector<Item *> &loot)
{
   int l;
   int l2;

   //PUT THINGS TOGETHER
   for(l=loot.size()-1;l>=1;l--)
   {
      for(l2=l-1;l2>=0;l2--)
      {
         loot[l2]->merge(*loot[l]);

         if (loot[l]->empty())
         {
            delete_and_remove(loot,l);
            break;
         }
      }
   }

   std::sort(loot.begin(),loot.end(),Item::sort_compare);
}


/* check if the squad has a certain weapon */
char squadhasitem(squadst &sq, const string& type)
{
   for(int p=0;p<6;p++)
   {
      if(sq.squad[p]!=NULL)
      {
         int index;
         index = getweapontype(type);
         if (index != -1)
         {
            if (sq.squad[p]->get_weapon().get_itemtypename() == type)
               return 1;
         }
      }
   }

   for(int l=0;l<(int)sq.loot.size();l++)
   {
      if(sq.loot[l]->get_itemtypename()!=type)continue;

      int index;
      index = getweapontype(type);
      if (index != -1)
      {
         if(sq.loot[l]->is_weapon() && sq.loot[l]->get_itemtypename()==type)
            return 1;
      }
   }

   return 0;
}

