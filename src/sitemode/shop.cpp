/**
 * Implementation of the Shop class.
 */
/*
 * Copyright 2010 Carlos Gustavos  <blomkvist >
 * Copyright 2014 Rich McGrew (yetisyny)
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
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
#include "sitemode/shop.h"

#include <algorithm>
#include <externs.h>
#include <functional>
#include <sstream>
#include <string>
#include "tinyxml2.h"


namespace
{
  const std::string SHOP_XML_SHOP_ELEMENT{"shop"};
  const std::string SHOP_SML_DEPARTMENT_ELEMENT{"department"};
} // anonymous namespace


ShopOption::ShopOption() : description_("UNDEFINED"), letter_(0), letter_defined_(false)
{ }


Shop::Shop(std::string const& xmlstring, bool fullscreen, bool only_sell_legal,
           bool increase_prices_with_illegality)
: allow_selling_(false)
, only_sell_legal_(only_sell_legal)
, increase_prices_with_illegality_(increase_prices_with_illegality)
, fullscreen_(fullscreen)
, sell_masks_(false)
{
   init(xmlstring);
}


void Shop::
init(std::string const& xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing shop XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if ((toplevel != nullptr)
   && (toplevel->Name() == SHOP_XML_SHOP_ELEMENT || toplevel->Name() == SHOP_SML_DEPARTMENT_ELEMENT))
  {
    auto attr_name = toplevel->Attribute("name");
    if (attr_name)
    {
    }

    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();

      if (tag == "only_sell_legal_items")
      {
        int b = stringtobool(element->GetText());
        if (b==1||b==0)
            this->only_sell_legal_ = b;
      }
      else if (tag == "fullscreen")
      {
         int b = stringtobool(element->GetText());
         if (b==1||b==0)
           this->fullscreen_ = b;
      }
      else if (tag == "allow_selling")
      {
         int b = stringtobool(element->GetText());
         if (b==1||b==0)
           this->allow_selling_ = b;
      }
      else if (tag == "increase_prices_with_illegality")
      {
         int b = stringtobool(element->GetText());
         if (b==1||b==0)
           this->increase_prices_with_illegality_ = b;
      }
      else if (tag == "department")
      {
        tinyxml2::XMLPrinter printer;
        element->Accept(&printer);
        this->options_.push_back(new Shop(printer.CStr(),
                                          this->fullscreen_,
                                          this->only_sell_legal_,
                                          this->increase_prices_with_illegality_));
      }
      else if (tag == "entry")
        this->description_ = element->GetText();
      else if (tag == "exit")
        this->exit_ = element->GetText();
      else if (tag == "sell_masks")
      {
         int b = stringtobool(element->GetText());
         if (b==1||b==0)
           this->sell_masks_ = b;
      }
      else if (tag == "letter")
      {
         this->letter_ = element->GetText()[0];
         if (97<=letter_ && letter_<= 122) //Check it is a letter.
          this->letter_defined_ = true;
         else if (65<=letter_ && letter_<=90)
         {
            this->letter_+=32;
            this->letter_defined_=true;
         }
         else if (letter_ == '!') //Allow special character.
            this->letter_defined_ = true;
      }
      else if (tag == "item")
      {
        tinyxml2::XMLPrinter printer;
        element->Accept(&printer);
        this->options_.push_back(new ShopItem(printer.CStr(),
                                              this->only_sell_legal_,
                                              this->increase_prices_with_illegality_));
      }
    }
  }
}

Shop::~Shop()
{
   delete_and_clear(options_);
}

void Shop::enter(squadst& customers) const
{
   music.play(MUSIC_SHOPPING);
   int buyer=0;
   choose(customers,buyer);
}

void Shop::choose(squadst& customers, int& buyer) const
{
   if(fullscreen_) browse_fullscreen(customers,buyer);
   else browse_halfscreen(customers,buyer);
}

void Shop::browse_halfscreen(squadst& customers, int& buyer) const
{
   int page=0,partysize=squadsize(&customers);

   std::vector<ShopOption*> available_options=options_;

   available_options.erase(remove_if(available_options.begin(),
				               available_options.end(),
				               not1(mem_fun(&ShopOption::display))),
			                  available_options.end());

   while(true)
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);

      locheader();
      printparty();

      move(8,45);
      addstr("Buyer: ");
      addstr(customers.squad[buyer]->name);

      //Write wares and prices
      int y=10,x=1,taken_letters=0;
      for(int p=page*19;p<len(available_options)&&p<page*19+20;p++)
      {
         if(available_options[p]->is_available())
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         if(x==1) move(y,1);
         else move(y,40);

         if(available_options[p]->letter_defined_)
            addchar(available_options[p]->showletter());
         else
         {
            // Find an available letter to use for this ware.
            bool done=false;
            while(taken_letters<27&&!done)
            {
               done=true;
               if('a'+taken_letters=='b'|| // Letters used by the shop UI are disallowed.
                  'a'+taken_letters=='e'||
                 ('a'+taken_letters=='s'&&allow_selling_)||
                 ('a'+taken_letters=='m'&&sell_masks_))
               {
                  taken_letters++;
                  done=false;
                  continue;
               }
               for(int i=0;i<len(available_options);i++)
               {
                  if(available_options[i]->letter_defined_&&
                     'a'+taken_letters==available_options[i]->letter_)
                  {
                     taken_letters++;
                     done=false;
                     break;
                  }
               }
            }
            available_options[p]->letter_='a'+taken_letters;
            addchar('A'+taken_letters++);
         }

         addstr(" - ");
         addstr(available_options[p]->get_description_halfscreen());
         if(x==1) x=2;
         else y++,x=1;
      }
      if(sell_masks_)
      {
         move(y,1+(x-1)*39);
         if(ledger.get_funds()>=15) set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         addstr("M - Buy a Mask                ($15)");
      }
      if(x==2) y++;

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(y++,1);
      addstr("E - Look over Equipment");

      if(allow_selling_)
      {
         move(y++,1);
         if(len(location[customers.squad[0]->base]->loot))
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         addstr("S - Sell something");
      }

      if(party_status!=-1) set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(++y,1);
      addstr("0 - Show the squad's Liberal status");

      if(partysize>0&&(party_status==-1||partysize>1))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(y++,40);
      addstr("# - Check the status of a squad Liberal");

      if(partysize>=2) set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(y,1);
      addstr("B - Choose a buyer");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(y,40);
      addstr("Enter - ");
      addstr(exit_);

      int c=getkey();

      for(int i=0;i<len(available_options);i++)
         if(c==available_options[i]->letter_)
         { available_options[i]->choose(customers,buyer); break; }

      if(c=='e' && customers.squad[0]->base!=-1)
         equip(location[customers.squad[0]->base]->loot,-1);
      else if(c=='s'&&allow_selling_&&len(location[customers.squad[0]->base]->loot))
         sell_loot(customers);
      else if(c=='m'&&sell_masks_&&ledger.get_funds()>=15)
         maskselect(*customers.squad[buyer]);
      else if(c=='b') choose_buyer(customers,buyer);
      else if(c=='0') party_status=-1;
      else if(c>='1'&&c<='6')
      {
         if(customers.squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')
               fullstatus(party_status);
            else party_status=c-'1';
         }
      }
      else if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}

void Shop::browse_fullscreen(squadst& customers, int& buyer) const
{
   int page=0;

   std::vector<ShopOption*> available_options=options_;
   available_options.erase(remove_if(available_options.begin(),
				               available_options.end(),
				               not1(mem_fun(&ShopOption::display))),
			                  available_options.end());

   while(true)
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("What will ");
      addstr(customers.squad[buyer]->name);
      addstr(" buy?");
      move(1,0);
      addstr("ÄÄÄÄPRODUCT NAMEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄPRICEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");

      //Write wares and prices
      for(int p=page*19,y=2;p<len(available_options)&&p<page*19+19;p++,y++)
      {
         if(available_options[p]->is_available())
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(y,0);
         addchar('A'+y-2);
         addstr(" - ");
         addstr(available_options[p]->get_description_fullscreen());
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select an option"); //allow customize "option"? -XML
      move(23,0);
      addpagestr();
      move(24,0);
      addstr("Enter - "+std::string(customers.squad[buyer]->name)+" "+exit_);

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(available_options)) page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+c-'a';
         if(p<len(available_options)&&available_options[p]->is_available())
            available_options[p]->choose(customers, buyer);
         break;
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }
}

void Shop::sell_loot(squadst& customers) const
{
   int partysize=squadsize(&customers);

   while(true)
   {
      erase();

      locheader();
      printparty();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,1);
      addstr("E - Look over Equipment");

      if(len(location[customers.squad[0]->base]->loot))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(10,40);
      addstr("F - Pawn Selectively");

      if(len(location[customers.squad[0]->base]->loot))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,1);
      addstr("W - Pawn all Weapons");

      if(len(location[customers.squad[0]->base]->loot))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,40);
      addstr("A - Pawn all Ammunition");

      if(len(location[customers.squad[0]->base]->loot))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,1);
      addstr("C - Pawn all Clothes");

      if(len(location[customers.squad[0]->base]->loot))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,40);
      addstr("L - Pawn all Loot");

      if(party_status!=-1)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,1);
      addstr("0 - Show the squad's Liberal status");

      if(partysize&&(party_status==-1||partysize>1))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,40);
      addstr("# - Check the status of a squad Liberal");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(16,40);
      addstr("Enter - Done pawning");

      int c=getkey();

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;

      if(c=='e'&&customers.squad[0]->base!=-1)
         equip(location[customers.squad[0]->base]->loot,-1);

      if(c=='w'||c=='a'||c=='c')
      {
         move(18,1);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         switch(c)
         {
         case 'w': addstr("Really sell all weapons? (Y)es to confirm.           "); break;
         case 'a': addstr("Really sell all ammo? (Y)es to confirm.              "); break;
         case 'c': addstr("Really sell all clothes? (Y)es to confirm.           "); break;
         }

         if(getkey()!='y') c=0; //no sale
      }

      if((c=='w'||c=='c'||c=='l'||c=='a'||c=='f')&&
          len(location[customers.squad[0]->base]->loot))
      {
         int fenceamount=0;

         if(c=='f') fenceamount=fenceselect(customers);
         else
         {
            for(int l=len(location[customers.squad[0]->base]->loot)-1;l>=0;l--)
            {
               if (c == 'w' && location[customers.squad[0]->base]->loot[l]->is_weapon()
                   && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                 * location[customers.squad[0]->base]->loot[l]->get_number();
                  delete_and_remove(location[customers.squad[0]->base]->loot,l);
               }
               else if (c == 'c' && location[customers.squad[0]->base]->loot[l]->is_armor()
                        && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                 * location[customers.squad[0]->base]->loot[l]->get_number();
                  delete_and_remove(location[customers.squad[0]->base]->loot,l);
               }
               else if (c == 'a' && location[customers.squad[0]->base]->loot[l]->is_clip()
                        && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                 * location[customers.squad[0]->base]->loot[l]->get_number();
                  delete_and_remove(location[customers.squad[0]->base]->loot,l);
               }
               else if (c == 'l' && location[customers.squad[0]->base]->loot[l]->is_loot()
                        && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  Loot* a = static_cast<Loot*>(location[customers.squad[0]->base]->loot[l]); //cast -XML
                  if(!a->no_quick_fencing())
                  {
                     fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                    * location[customers.squad[0]->base]->loot[l]->get_number();
                     delete_and_remove(location[customers.squad[0]->base]->loot,l);
                  }
               }
            }
         }

         if(fenceamount)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr("You add $");
            addstr(fenceamount);
            addstr(" to Liberal Funds.");

            getkey();

            ledger.add_funds(fenceamount,INCOME_PAWN);
         }
      }
   }
}

int Shop::fenceselect(squadst& customers) const
{
   int ret=0,page=0;

   consolidateloot(location[customers.squad[0]->base]->loot);

   vector<int> selected(len(location[customers.squad[0]->base]->loot),0);

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("What will you sell?");

      if(ret)
      {
         move(0,30);
         addstr("Estimated Liberal Amount: $");
         addstr(ret);
      }

      printparty();

      int x=1,y=10;
      std::string outstr,itemstr;

      for(int l=page*18;l<len(location[customers.squad[0]->base]->loot)&&l<page*18+18;l++)
      {
         if (selected[l])
            set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         itemstr = location[customers.squad[0]->base]->loot[l]->equip_title();
         if (location[customers.squad[0]->base]->loot[l]->get_number() > 1)
         {
            if(selected[l])
               itemstr += " " + std::to_string(selected[l]) + "/";
            else itemstr += " x";
            itemstr += std::to_string(location[customers.squad[0]->base]->loot[l]->get_number());
         }

         outstr = static_cast<char>(l-page*18+'A');
         outstr += " - " + itemstr;

         move(y,x);
         addstr(outstr);

         x+=26;
         if(x>53) x=1,y++;
      }

      //PAGE UP
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if (page > 0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page + 1) * 18 < len(location[customers.squad[0]->base]->loot))
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,1);
      addstr("Press a letter to select an item to sell.");
      move(24,1);
      addstr("Enter - Done");

      int c=getkey();

      if (c >= 'a' && c <= 'r')
      {
         int slot = c - 'a' + page * 18;

         if(slot >= 0 && slot < len(location[customers.squad[0]->base]->loot))
         {
            if (selected[slot])
            {
               ret -= location[customers.squad[0]->base]->loot[slot]->get_fencevalue() * selected[slot];
               selected[slot] = 0;
            }
            else
            {
               if (!location[customers.squad[0]->base]->loot[slot]->is_good_for_sale())
               {
                  printparty();

                  move(8,15);
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr(" You can't sell damaged goods.");

                  getkey();
               }
               else
               {
                  if(location[customers.squad[0]->base]->loot[slot]->get_number()>1)
                     selected[slot]=prompt_amount(0,location[customers.squad[0]->base]->loot[slot]->get_number());
                  else selected[slot]=1;
                  ret+=location[customers.squad[0]->base]->loot[slot]->get_fencevalue() * selected[slot];
               }
            }
         }
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)
         page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&
         (page+1)*18<len(location[customers.squad[0]->base]->loot))
         page++;

   }

   for(int l=len(location[customers.squad[0]->base]->loot)-1;l>=0;l--)
   {
      if(selected[l])
      {
         location[customers.squad[0]->base]->loot[l]->decrease_number(selected[l]);
         if(location[customers.squad[0]->base]->loot[l]->empty())
            delete_and_remove(location[customers.squad[0]->base]->loot,l);
      }
   }

   return ret;
}

void Shop::choose_buyer(squadst& customers, int& buyer) const
{
   party_status=-1;

   int partysize=squadsize(&customers);

   if(partysize<=1) return;

   while(true)
   {
      printparty();

      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose a Liberal squad member to SPEND.");

      int c=getkey();

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) return;

      if(c>='1'&&c<=partysize+'1'-1)
      {
         buyer=c-'1';
         return;
      }
   }
}

void Shop::maskselect(Creature &buyer) const
{
   short maskindex=-1;

   std::vector<int> masktype;
   for(int a=0;a<len(armortype);a++)
   {
      if(armortype[a]->is_mask()&&!armortype[a]->is_surprise_mask())
         masktype.push_back(a);
   }

   int page=0;

   while(true)
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Which mask will ");
      addstr(buyer.name);
      addstr(" buy?");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("ÄÄÄÄPRODUCT NAMEÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄDESCRIPTIONÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");

      for(int p=page*19,y=2;p<len(masktype)&&p<page*19+19;p++,y++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addchar(y+'A'-2);addstr(" - ");
         addstr(armortype[masktype[p]]->get_name());
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,39);
         addstr(armortype[masktype[p]]->get_description());
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Mask");
      move(23,0);
      addpagestr();
      move(24,0);
      addstr("Z - Surprise ");
      addstr(buyer.name);
      addstr(" With a Random Mask");

      int c=getkey();

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0) page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<len(masktype)) page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+c-'a';
         if(p<len(masktype))
         {
            maskindex=masktype[p];
            break;
         }
      }
      if(c=='z')
      {
         for(int i=0;i<len(armortype);i++)
            if(armortype[i]->is_mask()&&armortype[i]->is_surprise_mask())
               masktype.push_back(i);
         maskindex=pickrandom(masktype);
         break;
      }

      if(c=='x'||c==ENTER||c==ESC||c==SPACEBAR) break;
   }

   if (maskindex != -1 && ledger.get_funds() >= 15)
   {
      Armor a = Armor(*armortype[maskindex]);
      buyer.give_armor(a, &location[buyer.base]->loot);
      ledger.subtract_funds(15,EXPENSE_SHOPPING);
   }
}

bool Shop::is_available() const
{  //Disable shop (department) if it has nothing to sell.
   bool r = false;
   for(int i=0;i<len(options_)&&!(r = options_[i]->display());i++);
   return r;
}

Shop::ShopItem::
ShopItem(std::string const& xml, bool only_sell_legal, bool increase_price_with_illegality)
: price_(0)
, only_sell_legal_(only_sell_legal)
, increase_price_with_illegality_(increase_price_with_illegality)
, description_defined_(false)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing shop XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();

      if (tag == "class")
      {
        std::string item_class = element->GetText();
        if (item_class == "WEAPON")
          this->itemclass_ = WEAPON;
        else if (item_class == "CLIP")
          this->itemclass_ = CLIP;
        else if (item_class == "ARMOR")
          this->itemclass_ = ARMOR;
        else if (item_class == "LOOT")
          this->itemclass_ = LOOT;
      }
      else if (tag == "type")
        this->itemtypename_ = element->GetText();
      else if (tag == "description")
      {
        this->description_ = element->GetText();
        this->description_defined_ = true;
      }
      else if (tag == "price")
        this->price_ = std::stoi(element->GetText());
      else if (tag == "sleeperprice")
        this->sleeperprice_ = std::stoi(element->GetText());
      else if (tag == "letter")
      {
         this->letter_ = element->GetText()[0];
         if (97 <= this->letter_ && this->letter_ <= 122) //Check it is a letter.
            this->letter_defined_ = true;
         else if (65 <= this->letter_ && this->letter_ <= 90)
         {
            this->letter_ += 32;
            this->letter_defined_ = true;
         }
         else if (letter_ == '!') //Allow special character.
            this->letter_defined_ = true;
      }
    }
  }
}


void Shop::ShopItem::choose(squadst& customers, int& buyer) const
{
   if(!is_available()) return;
   ledger.subtract_funds(adjusted_price(), EXPENSE_SHOPPING);
   switch(itemclass_)
   {
   case WEAPON: {
      Weapon* i=new Weapon(*weapontype[getweapontype(itemtypename_)]);
      customers.squad[buyer]->give_weapon(*i,&location[customers.squad[0]->base]->loot);
      if(i->empty()) delete i;
      else location[customers.squad[0]->base]->loot.push_back(i);
      break; }
   case CLIP: {
      Clip* i=new Clip(*cliptype[getcliptype(itemtypename_)]);
      customers.squad[buyer]->take_clips(*i,1);
      if(i->empty()) delete i;
      else location[customers.squad[0]->base]->loot.push_back(i);
      break; }
   case ARMOR: {
      Armor* i=new Armor(*armortype[getarmortype(itemtypename_)]);
      customers.squad[buyer]->give_armor(*i,&location[customers.squad[0]->base]->loot);
      if(i->empty()) delete i;
      else location[customers.squad[0]->base]->loot.push_back(i);
      break; }
   case LOOT: {
      Loot* i=new Loot(*loottype[getloottype(itemtypename_)]);
      location[customers.squad[0]->base]->loot.push_back(i);
      break; }
   }
}

bool Shop::ShopItem::is_available() const
{
   return valid_item()&&(!only_sell_legal_||legal())&&can_afford();
}

const std::string Shop::ShopItem::get_description_halfscreen() const
{
   std::string r=get_description();
   r.resize(26,' ');
   r+="($"+std::to_string(adjusted_price())+")";
   return r;
}

const std::string Shop::ShopItem::get_description_fullscreen() const
{
   std::string r=get_description();
   r.resize(35,' ');
   r+="$"+std::to_string(adjusted_price());
   return r;
}

bool Shop::ShopItem::display() const
{
   return(valid_item()&&(!only_sell_legal_||legal()));
}

bool Shop::ShopItem::can_afford() const
{
   return(adjusted_price()<=ledger.get_funds());
}

bool Shop::ShopItem::legal() const
{
   bool r=true;
   switch(itemclass_)
   {
   case WEAPON: r=weapontype[getweapontype(itemtypename_)]->is_legal(); break;
   case CLIP:
      // Decide if clip is legal by looping through all weapons and
      // testing if there exists a weapon such that it is legal and
      // it can take this clip. If no legal weapon can take this type
      // of clip, the clip is implicitly illegal as well.
      r=false;
      for(int i=0;i<len(weapontype)&&!r;i++)
         if(weapontype[i]->acceptable_ammo(itemtypename_)&&weapontype[i]->is_legal())
            r=true;
      break;
   case ARMOR:       //Can't be illegal.
   case LOOT: break; //Can't be illegal.
   }
   return r;
}

bool Shop::ShopItem::valid_item() const
{
   int i=-1;
   switch(itemclass_)
   {
   case WEAPON: i=getweapontype(itemtypename_); break;
   case CLIP:   i=getcliptype(  itemtypename_); break;
   case ARMOR:  i=getarmortype( itemtypename_); break;
   case LOOT:   i=getloottype(  itemtypename_); break;
   }
   return(i!=-1);
}

int Shop::ShopItem::adjusted_price() const
{
   int p=price_;
   if(increase_price_with_illegality_&&itemclass_==WEAPON&&valid_item())
      for (int i=weapontype[getweapontype(itemtypename_)]->get_legality();
           i < to_index(law[LAW_GUNCONTROL])-2;
           i++)
         p*=2;
   return p;
}

const std::string& Shop::ShopItem::get_description() const
{
   if(description_defined_) return description_;
   else switch(itemclass_)
   {
   case WEAPON: return weapontype[getweapontype(itemtypename_)]->get_name();
   case CLIP:   return cliptype[  getcliptype(  itemtypename_)]->get_name();
   case ARMOR:  return armortype[ getarmortype( itemtypename_)]->get_name();
   case LOOT:   return loottype[  getloottype(  itemtypename_)]->get_name();
   default:     return description_; // Will be "UNDEFINED"
   }
}
