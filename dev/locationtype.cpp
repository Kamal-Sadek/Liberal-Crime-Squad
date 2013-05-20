#include "locationtype.h"

int LocationType::random_loot() const
{
   int total_weight = 0;
   int i;
   for(i=0;i<loot_list_.size();i++)
   {
      total_weight += loot_list_[i].weight;
   }
   if(!total_weight) return getloottype(-1);

   int selection = LCSrandom(total_weight);
   for(i=0;i<loot_list_.size();i++)
   {
      selection -= loot_list_[i].weight;
      if(selection < 0) return getloottype(loot_list_[i].type);
   }

}