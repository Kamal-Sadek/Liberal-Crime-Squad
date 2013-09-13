#ifndef CLIPTYPE_H
#define CLIPTYPE_H

#include "includes.h"

class ClipType : public ItemType
{
   public:
      explicit ClipType(MCD_STR xmlstring);
      
      bool is_clip() const { return true; }
      
      int get_ammoamount() const { return ammo_; }
   
   private:
      
      int ammo_;
};

#endif //CLIPTYPE_H
