#include "externs.h"

ShopOption::ShopOption()
 : description_("UNDEFINED"), letter_(0), letter_defined_(false)
{

}

Shop::Shop(MCD_STR xmlstring)
 : allow_selling_(false), only_sell_legal_(true), increase_prices_with_illegality_(false),
   fullscreen_(false)
{
   init(xmlstring);
}

Shop::Shop(MCD_STR xmlstring, bool fullscreen, bool only_sell_legal,
           bool increase_prices_with_illegality)
 : allow_selling_(false), only_sell_legal_(only_sell_legal),
   increase_prices_with_illegality_(increase_prices_with_illegality),
   fullscreen_(fullscreen)
{
   init(xmlstring);
}

void Shop::init(const MCD_STR &xmlstring)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();
   
   while (xml.FindElem())
   {
      std::string tag = xml.GetTagName();
      
      if (tag == "only_sell_legal_items")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            only_sell_legal_ = true;
         else if (b == 0)
            only_sell_legal_ = false;
      }
      else if (tag == "fullscreen")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            fullscreen_ = true;
         else if (b == 0)
            fullscreen_ = false;
      }
      else if (tag == "allow_selling")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            allow_selling_ = true;
         else if (b == 0)
            allow_selling_ = false;
      }
      else if (tag == "increase_prices_with_illegality")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            increase_prices_with_illegality_ = true;
         else if (b == 0)
            increase_prices_with_illegality_ = false;
      }
      else if (tag == "department")
      {
         options_.push_back(new Shop(xml.GetSubDoc(), fullscreen_, only_sell_legal_,
                                     increase_prices_with_illegality_));
      }
      else if (tag == "entry")
         description_ = xml.GetData();
      else if (tag == "exit")
         exit_ = xml.GetData();
      else if (tag == "letter")
      {
         letter_ = xml.GetData()[0];
         if (97 <= letter_ && letter_ <= 122) //Check it is a letter.
            letter_defined_ = true;
         else if (65 <= letter_ && letter_ <= 90)
         {
            letter_ += 32;
            letter_defined_ = true;
         }
      }
      else if (tag == "item")
         options_.push_back(new ShopItem(xml.GetSubDoc(), only_sell_legal_,
                            increase_prices_with_illegality_));
   }


}

Shop::~Shop()
{
   for (unsigned i = 0; i < options_.size(); ++i)
      delete options_[i];
}

void Shop::enter(squadst& customers) const
{
   int buyer = 0;
   choose(customers, buyer);
}

void Shop::choose(squadst& customers, int& buyer) const
{
   if (fullscreen_)
      browse_fullscreen(customers, buyer);
   else
      browse_halfscreen(customers, buyer);
}

void Shop::browse_halfscreen(squadst& customers, int& buyer) const
{
   unsigned page = 0;
   const unsigned max_entries_per_page = 20;

   std::vector<ShopOption*> available_options = options_;

   available_options.erase (remove_if (available_options.begin(), 
				       available_options.end(),
				       not1 (mem_fun (&ShopOption::display))),
			    available_options.end());

   int partysize = 0;
   for (int p = 0; p < 6; ++p)
   {
      if(customers.squad[p] != NULL)
      {
         partysize++;
      }
   }

   do
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);

      locheader();
      printparty();

      move(8,45);
      addstr("Buyer: ");
      addstr(customers.squad[buyer]->name);
      
      //Write wares and prices
      int yline = 10;
      int column = 1;
      int taken_letters = 0;
      for (unsigned p = page * (max_entries_per_page - 1);
           p < available_options.size() && p < page * (max_entries_per_page - 1) + max_entries_per_page;
           ++p)
      {
         if (available_options[p]->is_available())
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         else
            set_color(COLOR_BLACK,COLOR_BLACK,1);
         if (column == 1)
            move(yline,1);
         else
            move(yline,40);
         
         if (available_options[p]->letter_defined_)
            addch(available_options[p]->letter_-32);
         else
         {
            bool done = false;
            while (taken_letters < 27 && !done)
            {
               done = true;
               for (unsigned i = 0; i < available_options.size(); ++i)
               {
                  if (available_options[i]->letter_defined_ && 'a' + taken_letters == available_options[i]->letter_)
                  {
                     available_options[p]->letter_ = 'a' + taken_letters;
                     //available_options[p]->letter_defined_ = true;
                     ++taken_letters;
                     done = false;
                     break;
                  }
               }
            }
            addch('A' + taken_letters);
            ++taken_letters;
         }
         
         addstr(" - ");
         addstr(available_options[p]->get_description_halfscreen().c_str());
         if (column == 1)
            column = 2;
         else
         {
            ++yline;
            column = 1;
         }
      }
      if (column == 2)
         ++yline;
      
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(yline,1);
      addstr("E - Look over Equipment");
      ++yline;
      
      if (allow_selling_)
      {
         move(yline,1);
         if (location[activesquad->squad[0]->base]->loot.size() > 0)
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         else
            set_color(COLOR_BLACK,COLOR_BLACK,1);
         addstr("S - Sell something");
         ++yline;
      }
      
      ++yline;

      if (party_status != -1)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(yline,1);
      addstr("0 - Show the squad's Liberal status");
      
      if (partysize > 0 && (party_status == -1 || partysize > 1))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(yline++,40);
      addstr("# - Check the status of a squad Liberal");

      if (partysize >= 2)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(yline,1);
      addstr("B - Choose a buyer");
      
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(yline,40);
      addstr("Enter - ");
      addstr(exit_.c_str());

      

      int c = getch();
      translategetch(c);
      
      for (unsigned i = 0; i < available_options.size(); ++i)
      {
         if (c == available_options[i]->letter_)
         {
            available_options[i]->choose(customers, buyer);
            break;
         }
      }
      
      if(c == 'e' && customers.squad[0]->location != -1)
         equip(location[customers.squad[0]->location]->loot, -1);
      else if (c == 's' && allow_selling_
               && location[customers.squad[0]->base]->loot.size() > 0)
         sell_loot(customers);
      else if (c == 'b')
         choose_buyer(customers, buyer);
      else if (c == '0')
         party_status=-1;
      else if ( c >= '1' && c <= '6' && activesquad != NULL)
      {
         if (customers.squad[c-'1'] != NULL)
         {
            if (party_status == c - '1')
               fullstatus(party_status);
            else
               party_status = c - '1';
         }
      }
      else if (c == 10)
         break;

   } while (true);
}

void Shop::browse_fullscreen(squadst& customers, int& buyer) const
{
   unsigned page = 0;
   
   available_options.erase (remove_if (available_options.begin(), 
				       available_options.end(),
				       not1 (mem_fun (&ShopOption::display))),
			    available_options.end());


   do
   {
      erase();
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("What will ");
      addstr(customers.squad[buyer]->name);
      addstr(" buy?");
      move(1,0);
      addstr("----PRODUCT NAME-----------------------PRICE------------------------------------");
      int yline = 2;

      //Write wares and prices
      for(unsigned p = page * 19; p < available_options.size () && p < page * 19 + 19; p++)
      {
         if (available_options[p]->is_available())
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         else
            set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(yline,0);
         addch('A' + yline - 2);
         addstr(" - ");
         addstr(available_options[p]->get_description_fullscreen().c_str());
         ++yline;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select an option"); //allow customize "option"? -XML
      move(23,0);
      addpagestr();
      move(24,0);
      addstr(("Enter - " + std::string(customers.squad[buyer]->name) + " " + exit_).c_str());

      refresh();

      int c = getch();
      translategetch(c);

      //PAGE UP
      if ((c == interface_pgup || c == KEY_UP || c == KEY_LEFT) && page > 0)
         page--;
      //PAGE DOWN
      if ((c == interface_pgdn || c == KEY_DOWN || c == KEY_RIGHT)
          && (page + 1) * 19 < available_options.size())
         page++;

      if (c >= 'a' && c <= 's')
      {
         int p = page * 19 + static_cast<int>(c-'a');
         if (p < available_options.size() && available_options[p]->is_available())
            available_options[p]->choose(customers, buyer);
         break;
      }

      if (c == 10)
         break;
   } while (true);
}

void Shop::sell_loot(squadst& customers) const
{
   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(customers.squad[p]!=NULL)
      {
         partysize++;
      }
   }
   
   do
   {
      erase();
   
      locheader();
      printparty();
   
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,1);
      addstr("E - Look over Equipment");
   
      if (location[activesquad->squad[0]->base]->loot.size() > 0)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(10,40);
      addstr("F - Pawn Selectively");
   
      if (location[activesquad->squad[0]->base]->loot.size() > 0)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,1);
      addstr("W - Pawn all Weapons");
   
      if (location[activesquad->squad[0]->base]->loot.size() > 0)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,40);
      addstr("A - Pawn all Ammunition");
   
      if (location[activesquad->squad[0]->base]->loot.size() > 0)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,1);
      addstr("C - Pawn all Clothes");
   
      if (location[activesquad->squad[0]->base]->loot.size() > 0)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,40);
      addstr("L - Pawn all Loot");
      
      if (party_status != -1)
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,1);
      addstr("0 - Show the squad's Liberal status");
      
      if (partysize > 0 && (party_status == -1 || partysize > 1))
         set_color(COLOR_WHITE,COLOR_BLACK,0);
      else
         set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,40);
      addstr("# - Check the status of a squad Liberal");
      
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(16,40);
      addstr("Enter - Done pawning");
      
      int c = getch();
      translategetch(c);
      
      if (c == 10)
         break;
   
      if (c == 'e' && customers.squad[0]->location != -1) //Should it be base rather than location? -XML
         equip(location[customers.squad[0]->location]->loot, -1);
   
      if (c == 'w' || c == 'a' || c == 'c')
      {
         move(18,1);
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         switch (c)
         {
            case 'w':
               addstr("Really sell all weapons? (Y)es to confirm.           ");
               break;
            case 'a':
               addstr("Really sell all ammo? (Y)es to confirm.              ");
               break;
            case 'c':
               addstr("Really sell all clothes? (Y)es to confirm.           ");
               break;
         }
         int c2 = getch();
         translategetch(c2);
         if (c2 != 'y')
         {
            c = 0;//no sale
         }
      }
   
      if((c == 'w' || c == 'c' || c == 'l' || c == 'a' || c == 'f') &&
         location[customers.squad[0]->base]->loot.size() > 0)
      {
         int fenceamount=0;
   
         if (c == 'f')
            fenceamount = fenceselect(customers);
         else
         {
            for (int l = location[customers.squad[0]->base]->loot.size() - 1; l >= 0; l--)
            {
               if (c == 'w' && location[customers.squad[0]->base]->loot[l]->is_weapon()
                   && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                 * location[customers.squad[0]->base]->loot[l]->get_number();
                  delete location[customers.squad[0]->base]->loot[l];
                  location[customers.squad[0]->base]->loot.erase(location[customers.squad[0]->base]->loot.begin() + l);
               }
               else if (c == 'c' && location[customers.squad[0]->base]->loot[l]->is_armor()
                        && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                 * location[customers.squad[0]->base]->loot[l]->get_number();
                  delete location[customers.squad[0]->base]->loot[l];
                  location[customers.squad[0]->base]->loot.erase(location[customers.squad[0]->base]->loot.begin() + l);
               }
               else if (c == 'a' && location[customers.squad[0]->base]->loot[l]->is_clip()
                        && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                 * location[customers.squad[0]->base]->loot[l]->get_number();
                  delete location[customers.squad[0]->base]->loot[l];
                  location[customers.squad[0]->base]->loot.erase(location[customers.squad[0]->base]->loot.begin() + l);
               }
               else if (c == 'l' && location[customers.squad[0]->base]->loot[l]->is_loot()
                        && location[customers.squad[0]->base]->loot[l]->is_good_for_sale())
               {
                  Loot* a = static_cast<Loot*>(location[customers.squad[0]->base]->loot[l]); //cast -XML
                  if(!a->no_quick_fencing())
                  {
                     fenceamount += location[customers.squad[0]->base]->loot[l]->get_fencevalue()
                                    * location[customers.squad[0]->base]->loot[l]->get_number();
                     delete location[customers.squad[0]->base]->loot[l];
                     location[customers.squad[0]->base]->loot.erase(location[customers.squad[0]->base]->loot.begin() + l);
                  }
               }
            }
         }
   
         if(fenceamount > 0)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            move(8,1);
            addstr("You add $");
            addstr(tostring(fenceamount).c_str());
            addstr(" to Liberal Funds.");
   
            refresh();
            getch();
   
            ledger.add_funds(fenceamount,INCOME_PAWN);
         }
      }
   } while (true);
}

int Shop::fenceselect(squadst& customers) const
{
   int ret=0;

   consolidateloot(location[customers.squad[0]->base]->loot);

   int page=0;

   vector<int> selected(location[customers.squad[0]->base]->loot.size(),0);

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("What will you sell?");

      if (ret > 0)
      {
         move(0,30);
         addstr("Estimated Liberal Amount: $");
         addstr(tostring(ret).c_str());
      }

      printparty();

      int x = 1, y = 10;
      std::string outstr;
      std::string itemstr;

      for (int l = page * 18;
           l < location[customers.squad[0]->base]->loot.size() && l < page * 18 + 18;
           l++)
      {
         if (selected[l])
            set_color(COLOR_GREEN,COLOR_BLACK,1);
         else
            set_color(COLOR_WHITE,COLOR_BLACK,0);
         itemstr = location[customers.squad[0]->base]->loot[l]->equip_title();
         if (location[customers.squad[0]->base]->loot[l]->get_number() > 1)
         {
            if(selected[l] > 0)
            {
               itemstr += " " + tostring(selected[l]) + "/";
            }
            else
               itemstr += " x";
            itemstr += tostring(location[customers.squad[0]->base]->loot[l]->get_number());
         }

         outstr = static_cast<char>(l - page * 18 + 'A');
         outstr += " - " + itemstr;

         move(y,x);
         addstr(outstr.c_str());

         x += 26;
         if (x > 53)
         {
            x = 1;
            y++;
         }
      }

      //PAGE UP
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if (page > 0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page + 1) * 18 < location[customers.squad[0]->base]->loot.size())
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,1);
      addstr("Press a letter to select an item to sell.");
      move(24,1);
      addstr("X - Done");

      refresh();

      int c = getch();
      translategetch(c);

      if (c >= 'a' && c <= 'r')
      {
         int slot = c - 'a' + page * 18;

         if(slot >= 0 && slot < location[customers.squad[0]->base]->loot.size())
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

                  refresh();
                  getch();
               }
               else
               {
                  if (location[customers.squad[0]->base]->loot[slot]->get_number() > 1)
                  {
                     selected[slot] = 1;

                     printparty();

                     move(8,15);
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr("       How many?          ");

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

                     selected[slot] = atoi(str);
                     if (selected[slot] < 0)
                        selected[slot] = 0;
                     else if (selected[slot] > location[customers.squad[0]->base]->loot[slot]->get_number())
                        selected[slot]=location[customers.squad[0]->base]->loot[slot]->get_number();
                  }
                  else
                     selected[slot]=1;
                  ret += location[customers.squad[0]->base]->loot[slot]->get_fencevalue() * selected[slot];
               }
            }
         }
      }

      if(c == 'x')
         break;

      //PAGE UP
      if((c == interface_pgup || c == KEY_UP || c == KEY_LEFT) && page > 0)
         page--;
      //PAGE DOWN
      if((c == interface_pgdn || c == KEY_DOWN || c == KEY_RIGHT)
         && (page + 1) * 18 < location[customers.squad[0]->base]->loot.size())
         page++;

   } while (true);

   for (int l = location[customers.squad[0]->base]->loot.size() - 1; l >= 0; l--)
   {
      if(selected[l] > 0)
      {
         location[customers.squad[0]->base]->loot[l]->decrease_number(selected[l]);
         if(location[customers.squad[0]->base]->loot[l]->get_number() <= 0)
         {
            delete location[customers.squad[0]->base]->loot[l];
            location[customers.squad[0]->base]->loot.erase(location[customers.squad[0]->base]->loot.begin() + l);
         }
      }
   }

   return ret;
}

void Shop::choose_buyer(squadst& customers, int& buyer) const
{
   party_status = -1;

   int partysize = 0;
   for (int p = 0; p < 6; p++)
   {
      if (customers.squad[p] != NULL)
         partysize++;
   }

   if (partysize <= 1)
      return;

   do
   {
      printparty();

      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose a Liberal squad member to SPEND.");

      refresh();

      int c = getch();
      translategetch(c);

      if (c == 10)
         return;

      if (c >= '1' && c <= partysize + '1' - 1)
      {
         buyer = c - '1';
         return;
      }
   } while (true);
}

bool Shop::is_available() const
{
   //Disable shop (department) if it has nothing to sell.
   bool r = false;
   for (unsigned i = 0; i < options_.size() && !(r = options_[i]->display()); ++i);
   
   return r;
}

Shop::ShopItem::ShopItem(MCD_STR xmlstring, bool only_sell_legal,
                         bool increase_price_with_illegality)
 : price_(0), only_sell_legal_(only_sell_legal),
   increase_price_with_illegality_(increase_price_with_illegality),
   description_defined_(false)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();
   
   while (xml.FindElem())
   {
      std::string tag = xml.GetTagName();
      
      if (tag == "class")
      {
         if (xml.GetData() == "WEAPON")
            itemclass_ = WEAPON;
         else if (xml.GetData() == "CLIP")
            itemclass_ = CLIP;
         else if (xml.GetData() == "ARMOR")
            itemclass_ = ARMOR;
         else if (xml.GetData() == "LOOT")
            itemclass_ = LOOT;
      }
      else if (tag == "type")
         itemtypename_ = xml.GetData();
      else if (tag == "description")
      {
         description_ = xml.GetData();
         description_defined_ = true;
      }
      else if (tag == "price")
         price_ = atoi(xml.GetData().c_str());
      else if (tag == "letter")
      {
         letter_ = xml.GetData()[0];
         if (97 <= letter_ && letter_ <= 122) //Check it is a letter.
            letter_defined_ = true;
         else if (65 <= letter_ && letter_ <= 90)
         {
            letter_ += 32;
            letter_defined_ = true;
         }
      }
   }
}

void Shop::ShopItem::choose(squadst& customers, int& buyer) const
{
   if (is_available())
   {
      ledger.subtract_funds(adjusted_price(), EXPENSE_SHOPPING);
      switch (itemclass_)
      {
         case WEAPON:
         {
            Weapon* i = new Weapon(*weapontype[getweapontype(itemtypename_)]);
            customers.squad[buyer]->give_weapon(*i, &location[customers.squad[0]->base]->loot);
            if (i->empty())
               delete i;
            else
               location[customers.squad[0]->base]->loot.push_back(i);
            break;
         }
         case CLIP:
         {
            Clip* i = new Clip(*cliptype[getcliptype(itemtypename_)]);
            customers.squad[buyer]->take_clips(*i, 1);
            if (i->empty())
               delete i;
            else
               location[customers.squad[0]->base]->loot.push_back(i);
            break;
         }
         case ARMOR:
         {
            Armor* i = new Armor(*armortype[getarmortype(itemtypename_)]);
            customers.squad[buyer]->give_armor(*i, &location[customers.squad[0]->base]->loot);
            if (i->empty())
               delete i;
            else
               location[customers.squad[0]->base]->loot.push_back(i);
            break;
         }
         case LOOT:
         {
            Loot* i = new Loot(*loottype[getloottype(itemtypename_)]);
            location[customers.squad[0]->base]->loot.push_back(i);
            break;
         }
      }
   }
}

bool Shop::ShopItem::is_available() const
{
   return valid_item() && (!only_sell_legal_ || legal()) && can_afford();
}

const std::string Shop::ShopItem::get_description_halfscreen() const
{
   std::string r = get_description();
   r.resize(26, ' ');
   r += "($" + tostring(adjusted_price()) + ")";
   return r;
}

const std::string Shop::ShopItem::get_description_fullscreen() const
{
   std::string r = get_description();
   r.resize(35, ' ');
   r += "$" + tostring(adjusted_price());
   return r;
}

bool Shop::ShopItem::display() const
{
   return (valid_item() && (!only_sell_legal_ || legal()));
}

bool Shop::ShopItem::can_afford() const
{
   return (adjusted_price() <= ledger.get_funds());
}

bool Shop::ShopItem::legal() const
{
   bool r = true;
   switch (itemclass_)
   {
      case WEAPON: r = weapontype[getweapontype(itemtypename_)]->is_legal(); break;
      /*case CLIP:   r = getcliptype(itemtypename_); break; //Can't be illegal.
      case ARMOR:  r = getarmortype(itemtypename_); break;
      case LOOT:   r = getloottype(itemtypename_); break;*/
   }
   
   return r;
}

bool Shop::ShopItem::valid_item() const
{
   int i;
   switch (itemclass_)
   {
      case WEAPON: i = getweapontype(itemtypename_); break;
      case CLIP: i = getcliptype(itemtypename_); break;
      case ARMOR: i = getarmortype(itemtypename_); break;
      case LOOT: i = getloottype(itemtypename_); break;
   }
   return (i != -1);
}

int Shop::ShopItem::adjusted_price() const
{
   int p = price_;
   if (increase_price_with_illegality_ && itemclass_ == WEAPON && valid_item())
   {
      for (int i = weapontype[getweapontype(itemtypename_)]->get_legality();
           i < law[LAW_GUNCONTROL]; ++i)
      {
         p *= 2;
      }
   }
   
   return p;
}

const std::string& Shop::ShopItem::get_description() const
{
   if (description_defined_)
   {
      return description_;
   }
   else
   {
      switch (itemclass_)
      {
         case WEAPON: return weapontype[getweapontype(itemtypename_)]->get_name();
         case CLIP  : return cliptype  [getcliptype  (itemtypename_)]->get_name();
         case ARMOR : return armortype [getarmortype (itemtypename_)]->get_name();
         case LOOT  : return loottype  [getloottype  (itemtypename_)]->get_name();
      }
   }
}
