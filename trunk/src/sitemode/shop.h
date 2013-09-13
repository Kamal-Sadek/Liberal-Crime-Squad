#ifndef SHOP_H
#define SHOP_H

#include "includes.h"


class Shop;

class ShopOption
{
   friend class Shop;

   public:
      ShopOption();
      virtual ~ShopOption() {}

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
      Shop(MCD_STR xmlstring);
      ~Shop();
      
      //This function is used to start the shop interface.
      void enter(squadst& customers) const;
   
   protected:
      virtual void choose(squadst& customers, int& buyer) const;
      virtual bool is_available() const;

   private:
      Shop(MCD_STR xmlstring, bool fullscreen, bool only_sell_legal,
           bool increase_prices_with_illegality);
      void init(const MCD_STR &xmlstring);
      
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
            ShopItem(MCD_STR xmlstring, bool only_sell_legal,
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
