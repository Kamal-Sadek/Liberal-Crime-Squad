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
// Gets the full title of an item for display, without quantity.
void get_equip_title(char *str2, itemst* item)
{
   switch (item->type)
   {
      case ITEM_WEAPON:
         getweaponfull(str2,item->weapon.type);
         if(item->weapon.ammo>0)
         {
            char num[20];
            itoa(item->weapon.ammo,num,10);
            strcat(str2," (");
            strcat(str2,num);
            strcat(str2,")");
         }
         break;

      case ITEM_ARMOR:
         getarmorfull(str2,item->armor,0);
         break;

      case ITEM_CLIP:
         getclip(str2,item->cliptype);
         break;

      case ITEM_LOOT:
         getloot(str2,item->loottype);
         break;

      case ITEM_MONEY:
         strcpy(str2,"$");
         {
            char num[20];
            itoa(item->money,num,10);
            strcat(str2,num);
         }
         break;

      default:
         strcpy(str2, "(BUG #");
         {
            char num[20];
            itoa(item->type,num,10);
            strcat(str2,num);
         }
         strcat(str2, ", report me!)");

   }
}

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
void equip(vector<itemst *> &loot,int loc)
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
      char str[200],str2[200];

      for(int l=page*18;l<loot.size()&&l<page*18+18;l++)
      {
         get_equip_title(str2, loot[l]);

         if(loot[l]->number>1)
         {
            char num[20];
            itoa(loot[l]->number,num,10);
            strcat(str2," ");
            strcat(str2,"x");
            strcat(str2,num);
         }
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
      if((page+1)*18<loot.size())
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

      if(loc!=-1)
      {
         if(location[loc]->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(22,1);
         addstr("Y - Get things from ");
         addstr(location[loc]->shortname);

         if(loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(22,40);
         addstr("Z - Stash things at ");
         addstr(location[loc]->shortname);
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(24,1);
      addstr("Enter - Done");

      refresh();

      int c=getch();
      translategetch(c);

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot < 0 || slot >= loot.size())
         {
            // Out of range.
         }
         else if (loot[slot]->type != ITEM_WEAPON
                 && loot[slot]->type != ITEM_ARMOR
                 && loot[slot]->type != ITEM_CLIP)
         {
            errmsg = "You can't equip that.";
            continue;
         }
         else
         {
            move(8,20);
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("Choose a Liberal squad member to receive it.");

            refresh();

            int c=getch();
            translategetch(c);

            if(c>='1'&&c<='6')
            {
               Creature *squaddie = activesquad->squad[c - '1'];

               if(squaddie!=NULL)
               {
                  int armok=2;
                  if((squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF)||
                     (squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF))armok--;
                  if((squaddie->wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF)||
                     (squaddie->wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF))armok--;
                  if(squaddie->special[SPECIALWOUND_NECK]!=1)armok=0;
                  if(squaddie->special[SPECIALWOUND_UPPERSPINE]!=1)armok=0;

                  if(loot[slot]->type==ITEM_WEAPON && armok)
                  {
                     if(squaddie->weapon.type==WEAPON_MOLOTOV&&
                        loot[slot]->weapon.type==WEAPON_MOLOTOV)
                     {
                        
                        if(squaddie->weapon.ammo==0)
                           squaddie->weapon.ammo=1;
                        else
                           squaddie->clip[CLIP_MOLOTOV]++;
                     }
                     else if(squaddie->weapon.type!=WEAPON_NONE)
                     {
                        itemst *newloot=new itemst;
                           newloot->type=ITEM_WEAPON;
                           newloot->weapon=squaddie->weapon;
                        loot.push_back(newloot);

                        squaddie->weapon=loot[slot]->weapon;
                     }
                     else squaddie->weapon=loot[slot]->weapon;

                     loot[slot]->number--;
                     if(loot[slot]->number==0)
                     {
                        delete loot[slot];
                        loot.erase(loot.begin() + slot);
                     }

                     //DROP ALL CLIPS THAT DON'T WORK
                     for(int cl=0;cl<CLIPNUM;cl++)
                     {
                        if(cl==ammotype(squaddie->weapon.type))continue;

                        for(int p2=0;p2<squaddie->clip[cl];p2++)
                        {
                           if(cl==CLIP_MOLOTOV)
                           {
                              itemst *newi=new itemst;
                                 newi->type=ITEM_WEAPON;
                                 newi->weapon.type=WEAPON_MOLOTOV;
                                 newi->weapon.ammo=1;
                              loot.push_back(newi);
                           }
                           else
                           {
                              itemst *newi=new itemst;
                                 newi->type=ITEM_CLIP;
                                 newi->cliptype=cl;
                              loot.push_back(newi);
                           }
                        }

                        squaddie->clip[cl]=0;
                     }

                     if(page*18>=loot.size()&&page!=0)page--;
                  }
                  else if(loot[slot]->type==ITEM_ARMOR)
                  {
                     if(squaddie->armor.type!=ARMOR_NONE)
                     {
                        itemst *newloot=new itemst;
                           newloot->type=ITEM_ARMOR;
                           newloot->armor=squaddie->armor;
                        loot.push_back(newloot);
                     }

                     squaddie->armor=loot[slot]->armor;

                     loot[slot]->number--;
                     if(loot[slot]->number==0)
                     {
                        delete loot[slot];
                        loot.erase(loot.begin() + slot);
                     }

                     if(page*18>=loot.size()&&page!=0)page--;
                  }
                  else if(loot[slot]->type==ITEM_CLIP && armok)
                  {
                     short ammo_type = ammotype(squaddie->weapon.type);
                     int space = 9 - squaddie->clip[ammo_type];

                     if (ammo_type != loot[slot]->cliptype) {
                        errmsg = (ammo_type < 0 ?
                              "Can't carry ammo without a gun." :
                              "That ammo doesn't fit.");
                        continue;
                     } else if (space < 1) {
                        errmsg = "Can't carry any more ammo.";
                        continue;
                     } else {
                        int amount = 1;
                        if (loot[slot]->number > 1)
                           amount = prompt_amount(0,
                                 MIN(loot[slot]->number, space));

                        squaddie->clip[ammo_type] += amount;
                        loot[slot]->number -= amount;

                        if(loot[slot]->number==0)
                        {
                           delete loot[slot];
                           loot.erase(loot.begin() + slot);
                        }

                        if(page*18>=loot.size()&&page!=0)page--;
                     }
                  }

                  consolidateloot(loot);
               }
            }
         }
      }
      if(c=='s')
      {
         move(8,20);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         addstr("Choose a Liberal squad member to strip down.");

         refresh();

         int c=getch();
         translategetch(c);

         if(c>='1'&&c<='6')
         {
            if(activesquad->squad[c-'1']!=NULL)
            {
               if(activesquad->squad[c-'1']->armor.type!=ARMOR_NONE)
               {
                  itemst *newi=new itemst;
                     newi->type=ITEM_ARMOR;
                     newi->armor=activesquad->squad[c-'1']->armor;
                  loot.push_back(newi);

                  activesquad->squad[c-'1']->armor.type=ARMOR_NONE;
                  activesquad->squad[c-'1']->armor.quality='1';
                  activesquad->squad[c-'1']->armor.flag=0;

                  consolidateloot(loot);
               }
            }
         }
      }

      if(c==10)return;

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
            if(activesquad->squad[p]->weapon.type!=WEAPON_NONE)
            {
               itemst *i=new itemst;
                  i->type=ITEM_WEAPON;
                  i->weapon=activesquad->squad[p]->weapon;
               loot.push_back(i);

               activesquad->squad[p]->weapon.type=WEAPON_NONE;
               activesquad->squad[p]->weapon.ammo=0;

               //DROP ALL CLIPS
               for(int c=0;c<CLIPNUM;c++)
               {
                  for(int p2=0;p2<activesquad->squad[p]->clip[c];p2++)
                  {
                     if(c==CLIP_MOLOTOV)
                     {
                        itemst *newi=new itemst;
                           newi->type=ITEM_WEAPON;
                           newi->weapon.type=WEAPON_MOLOTOV;
                           newi->weapon.ammo=1;
                        loot.push_back(newi);
                     }
                     else
                     {
                        itemst *newi=new itemst;
                           newi->type=ITEM_CLIP;
                           newi->cliptype=c;
                        loot.push_back(newi);
                     }
                  }

                  activesquad->squad[p]->clip[c]=0;
               }

               consolidateloot(loot);
            }
         }
      }

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<loot.size())page++;

   }while(1);
}



/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<itemst *> &dest,vector<itemst *> &source)
{
   int page=0;

   vector<int> selected;
   selected.resize(source.size());
   for(int s=0;s<selected.size();s++)selected[s]=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("Select Objects");

      printparty();

      int x=1,y=10;
      char str[200],str2[200];

      for(int l=page*18;l<source.size()&&l<page*18+18;l++)
      {
         if(selected[l])set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);

         get_equip_title(str2, source[l]);

         if(source[l]->number>1)
         {
            char num[20];
            strcat(str2," ");
            if(selected[l]>0)
            {
               itoa(selected[l],num,10);
               strcat(str2,num);
               strcat(str2,"/");
            }
            else strcat(str2,"x");
            itoa(source[l]->number,num,10);
            strcat(str2,num);
         }

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
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<source.size())
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

         if(slot>=0&&slot<source.size())
         {
            if(selected[slot])
            {
               selected[slot]=0;
            }
            else
            {
               if(source[slot]->number>1)
                  selected[slot] = prompt_amount(0, source[slot]->number);
               else selected[slot]=1;
            }
         }
      }

      if(c==10)break;

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<source.size())page++;

   }while(1);

   for(int l=source.size()-1;l>=0;l--)
   {
      if(selected[l]>0)
      {
         itemst *newit=new itemst;
            *newit=*source[l];
            newit->number=selected[l];
         dest.push_back(newit);

         source[l]->number-=selected[l];
         if(source[l]->number<=0)
         {
            delete source[l];
            source.erase(source.begin() + l);
         }
      }
   }

   // Avoid stuff jumping around the next time you equip.
   consolidateloot(dest);
}



/* combines multiple items of the same type into stacks */
void consolidateloot(vector<itemst *> &loot)
{
   int l;
   int l2;

   //PUT THINGS TOGETHER
   for(l=loot.size()-1;l>=1;l--)
   {
      for(l2=l-1;l2>=0;l2--)
      {
         if(loot[l]->type==loot[l2]->type)
         {
            char conf=0;
            switch(loot[l]->type)
            {
               case ITEM_WEAPON:
                  if(loot[l]->weapon.type==loot[l2]->weapon.type&&
                     loot[l]->weapon.ammo==loot[l2]->weapon.ammo)conf=1;
                  break;
               case ITEM_ARMOR:
                  if(loot[l]->armor.type==loot[l2]->armor.type&&
                     loot[l]->armor.subtype==loot[l2]->armor.subtype&&
                     loot[l]->armor.quality==loot[l2]->armor.quality&&
                     loot[l]->armor.flag==loot[l2]->armor.flag)conf=1;
                  break;
               case ITEM_CLIP:
                  if(loot[l]->cliptype==loot[l2]->cliptype)conf=1;
                  break;
               case ITEM_LOOT:
                  if(loot[l]->loottype==loot[l2]->loottype&&
                     loot[l]->loottype!=LOOT_CEOPHOTOS&&
                     loot[l]->loottype!=LOOT_INTHQDISK&&
                     loot[l]->loottype!=LOOT_CORPFILES&&
                     loot[l]->loottype!=LOOT_JUDGEFILES&&
                     loot[l]->loottype!=LOOT_RESEARCHFILES&&
                     loot[l]->loottype!=LOOT_PRISONFILES&&
                     loot[l]->loottype!=LOOT_CABLENEWSFILES&&
                     loot[l]->loottype!=LOOT_AMRADIOFILES&&
                     loot[l]->loottype!=LOOT_SECRETDOCUMENTS&&
                     loot[l]->loottype!=LOOT_POLICERECORDS)conf=1;
                  break;
               case ITEM_MONEY:
                  conf=1;
                  break;
            }
            if(conf)
            {
               if(loot[l2]->type==ITEM_MONEY)loot[l2]->money+=loot[l]->money;
               else loot[l2]->number+=loot[l]->number;
               delete loot[l];
               loot.erase(loot.begin() + l);
               break;
            }
         }
      }
   }

   //NOW DO A LAME-ASS SORT
   itemst *pt;
   for(l=loot.size()-2;l>=0;l--)
   {
      for(l2=l+1;l2<loot.size();l2++)
      {
         if(itemcompare(loot[l],loot[l2])>=0)
         {
            pt=loot[l];
            loot.erase(loot.begin() + l);
            loot.insert(loot.begin()+l2-1,pt);
            break;
         }
      }
      if(l2==loot.size())
      {
         pt=loot[l];
         loot.erase(loot.begin() + l);
         loot.push_back(pt);
      }
   }
}



/* compares two items, used in sorting gear */
char itemcompare(itemst *a,itemst *b)
{
   if(a->type<b->type)return 1;
   else if(a->type>b->type)return -1;
   else
   {
      switch(a->type)
      {
         case ITEM_WEAPON:
            if(a->weapon.type<b->weapon.type)return 1;
            else if(a->weapon.type>b->weapon.type)return -1;
            else
            {
               if(a->weapon.ammo>b->weapon.ammo)return 1;
               else if(a->weapon.ammo<b->weapon.ammo)return -1;
            }
            break;
         case ITEM_ARMOR:
            if(a->armor.type==b->armor.type)
            {
               if(a->armor.subtype==b->armor.subtype)
               {
                  if(a->armor.quality==b->armor.quality)
                  {
                     if(a->armor.flag<b->armor.flag)return 1;
                     else if(a->armor.flag>b->armor.flag)return -1;
                  }
                  else if(a->armor.quality<b->armor.quality)return 1;
                  else return -1;
               }
               else if(a->armor.subtype<b->armor.subtype)return 1;
               else return -1;
            }
            else if(a->armor.type<b->armor.type)return 1;
            else return -1;
            break;
         case ITEM_CLIP:
            if(a->cliptype<b->cliptype)return 1;
            else return -1;
            break;
         case ITEM_LOOT:
            if(a->loottype<b->loottype)return 1;
            else return -1;
            break;
      }
   }

   return 0;
}



/* returns the type of ammo used by the given weapon, if any */
short ammotype(int type)
{
   switch(type)
   {
      case WEAPON_REVOLVER_22:
         return CLIP_22;
      case WEAPON_REVOLVER_44:
         return CLIP_44;
      case WEAPON_SMG_MP5:
         return CLIP_SMG;
      case WEAPON_SEMIPISTOL_9MM:
         return CLIP_9;
      case WEAPON_SEMIPISTOL_45:
         return CLIP_45;
      case WEAPON_CARBINE_M4:
      case WEAPON_SEMIRIFLE_AR15:
      case WEAPON_AUTORIFLE_M16:
      case WEAPON_AUTORIFLE_AK47:
         return CLIP_ASSAULT;
      case WEAPON_SHOTGUN_PUMP:
         return CLIP_BUCKSHOT;
      case WEAPON_MOLOTOV:
         return CLIP_MOLOTOV;
      case WEAPON_FLAMETHROWER:
         return CLIP_GASOLINE;
   }

   return -1;
}



/* check if a weapon is ranged */
char rangedweapon(weaponst &w)
{
   switch(w.type)
   {
      case WEAPON_REVOLVER_22:
      case WEAPON_REVOLVER_44:
      case WEAPON_SEMIPISTOL_9MM:
      case WEAPON_SEMIPISTOL_45:
      case WEAPON_SEMIRIFLE_AR15:
      case WEAPON_SMG_MP5:
      case WEAPON_CARBINE_M4:
      case WEAPON_AUTORIFLE_M16:
      case WEAPON_AUTORIFLE_AK47:
      case WEAPON_SHOTGUN_PUMP:
      case WEAPON_MOLOTOV:
      case WEAPON_FLAMETHROWER:
         return 1;
   }

   return 0;
}



/* check if the squad has a certain weapon */
char squadhasitem(squadst &sq,int type,int subtype)
{
   for(int p=0;p<6;p++)
   {
      if(sq.squad[p]!=NULL)
      {
         switch(type)
         {
            case ITEM_WEAPON:
               if(sq.squad[p]->weapon.type==subtype)return 1;
               break;
         }
      }
   }

   for(int l=0;l<sq.loot.size();l++)
   {
      if(sq.loot[l]->type!=type)continue;
      switch(type)
      {
         case ITEM_WEAPON:
            if(sq.loot[l]->weapon.type==subtype)return 1;
            break;
      }
   }

   return 0;
}

