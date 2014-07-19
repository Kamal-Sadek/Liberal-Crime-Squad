#ifndef LOOTTYPE_H
#define LOOTTYPE_H

#include "includes.h"

class LootType : public ItemType
{
   public:
      explicit LootType(MCD_STR xmlstring);
      
      bool is_loot() const { return true; }
      
      // Returns if the loot type should be stacked or not.
      bool is_stackable() const { return stackable_; }
      // Returns if the loot should be skipped when quick fencing all loot.
      bool no_quick_fencing() const { return no_quick_fencing_; }
      // Returns if the loot is usable as cloth when making clothing.
      bool is_cloth() const { return cloth_; }
      
   
   private:
      bool stackable_;
      bool no_quick_fencing_;
      bool cloth_;
};

#endif //LOOTTYPE_H
