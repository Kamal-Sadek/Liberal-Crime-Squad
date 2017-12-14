/**
 * Interface for the Shop class.
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
#ifndef SHOP_H
#define SHOP_H

#include "includes.h"

class Shop;

class ShopOption
{
   friend class Shop;

   public:
      ShopOption();
      virtual ~ShopOption() { }

   protected:

      //Returns if the option should be displayed.
      virtual bool display() const
      {
         return true;
      }

      //Returns if the option can be chosen.
      virtual bool is_available() const
      {
         return true;
      }

      virtual const std::string get_description_halfscreen() const
      {
         return description_;
      }

      virtual const std::string get_description_fullscreen() const
      {
         return description_;
      }

      std::string description_;
      char letter_;
      bool letter_defined_;
      char showletter()
      {
         if (letter_ == '!')
            return letter_;
         else
            return letter_-32;
      }
      virtual void choose(squadst& customers, int& buyer) const = 0;
};

class Shop : public ShopOption
{
   public:
      Shop(std::string const& xmlstring,
           bool fullscreen=false,
           bool only_sell_legal=true,
           bool increase_prices_with_illegality=false);
      ~Shop();

      //This function is used to start the shop interface.
      void enter(squadst& customers) const;

   protected:
      virtual void choose(squadst& customers, int& buyer) const;
      virtual bool is_available() const;

   private:
      void init(std::string const& xmlstring);

      void browse_fullscreen(squadst& customers, int& buyer) const;
      void browse_halfscreen(squadst& customers, int& buyer) const;
      void sell_loot(squadst& customers) const;
      int fenceselect(squadst& customers) const;
      void choose_buyer(squadst& customers, int& buyer) const;
      void maskselect(Creature &buyer) const;

      bool allow_selling_;
      bool only_sell_legal_;
      bool increase_prices_with_illegality_;
      std::vector<ShopOption*> options_;
      bool fullscreen_;
      std::string exit_;
      bool sell_masks_;

      class ShopItem : public ShopOption
      {
         public:
            ShopItem(std::string const& xmlstring, bool only_sell_legal,
                     bool increase_price_with_illegality);

            virtual bool display() const;

         protected:
            virtual void choose(squadst& customers, int& buyer) const;
            virtual bool is_available() const;
            virtual const std::string get_description_halfscreen() const;
            virtual const std::string get_description_fullscreen() const;

         private:
            bool can_afford() const;
            bool legal() const;
            bool valid_item() const;
            std::string itemtypename_; //Have pointer to ItemType instead? -XML
            enum itemclassenum //Could be solved better without enum. -XML
            {
               WEAPON,
               CLIP,
               ARMOR,
               LOOT
            };
            itemclassenum itemclass_;

            int price_;
            int adjusted_price() const;
            int sleeperprice_;
            bool only_sell_legal_;
            bool increase_price_with_illegality_;
            bool description_defined_;
            const std::string& get_description() const;

            //Item* make_item() const;

      };
};

#endif //SHOP_H
