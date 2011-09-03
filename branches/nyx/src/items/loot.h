#ifndef LOOT_H
#define LOOT_H

#include "includes.h"

class Loot : public Item
{
   public:
      explicit Loot(const LootType& seed, int number = 1);
      virtual Loot* clone() const { return new Loot(*this); }
      explicit Loot(const char * inputXml);      
      string showXml () const;

      
      virtual bool is_loot() const { return true; }
      
      virtual Loot* split(int number);
      virtual bool merge(Item& i);
      virtual bool sort_compare_special(Item* other) const;
      
      virtual string equip_title() const;
      virtual const string& get_name() const;
      virtual long get_fencevalue() const;
      
      bool is_stackable() const;
      bool no_quick_fencing() const;
      bool is_cloth() const; 
   
   private:
      
};

#endif //LOOT_H
