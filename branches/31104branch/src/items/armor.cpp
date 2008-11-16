/*
    This file is a part of Liberal Crime Squad.
    All code is released under GNU General Public License.
*/

#include "includes.h"


int armorst::interrogation_basepower()
{
   switch(type)
   {
   default:
   case ARMOR_NONE:
   case ARMOR_CLOTHES:
   case ARMOR_WORKCLOTHES:
   case ARMOR_CLOWNSUIT:
   case ARMOR_PRISONER:
   case ARMOR_ELEPHANTSUIT:
   case ARMOR_DONKEYSUIT:
   case ARMOR_TOGA:
   case ARMOR_MITHRIL:
   case ARMOR_OVERALLS:
   case ARMOR_WIFEBEATER:
      return 0;
   case ARMOR_BLACKROBE:
   case ARMOR_TRENCHCOAT:
   case ARMOR_BUNKERGEAR:
   case ARMOR_CIVILLIANARMOR:
   case ARMOR_HEAVYARMOR:
   case ARMOR_SECURITYUNIFORM:
      return 2;
   case ARMOR_MASK:
   case ARMOR_CHEAPSUIT:
   case ARMOR_CHEAPDRESS:
   case ARMOR_BLACKSUIT:
   case ARMOR_SWATARMOR:
      return 4;
   case ARMOR_LABCOAT:
   case ARMOR_EXPENSIVESUIT:
   case ARMOR_EXPENSIVEDRESS:
   case ARMOR_BLACKDRESS:
   case ARMOR_BONDAGEGEAR:
   case ARMOR_POLICEUNIFORM:
   case ARMOR_POLICEARMOR:
   case ARMOR_ARMYARMOR:
   case ARMOR_MILITARY:
   case ARMOR_PRISONGUARD:
      return 6;
   }
}

int armorst::interrogation_assaultbonus()
{
   switch(type)
   {
   default:
      return 0;
   case ARMOR_CLOWNSUIT:
   case ARMOR_ELEPHANTSUIT:
   case ARMOR_DONKEYSUIT:
   case ARMOR_MASK:
   case ARMOR_BLACKSUIT:
   case ARMOR_BLACKDRESS:
   case ARMOR_BONDAGEGEAR:
   case ARMOR_POLICEUNIFORM:
   case ARMOR_POLICEARMOR:
   case ARMOR_ARMYARMOR:
   case ARMOR_MILITARY:
   case ARMOR_PRISONGUARD:
      return 4;
   }
}

int armorst::interrogation_drugbonus()
{
   switch(type)
   {
   default:
      return 0;
   case ARMOR_CLOWNSUIT:
   case ARMOR_ELEPHANTSUIT:
   case ARMOR_DONKEYSUIT:
   case ARMOR_MASK:
   case ARMOR_BONDAGEGEAR:
      return 4;
   }
}

int armorst::professionalism()
{
   switch(type)
   {
   case ARMOR_NONE:
   case ARMOR_CLOWNSUIT:
   case ARMOR_ELEPHANTSUIT:
   case ARMOR_DONKEYSUIT:
   case ARMOR_PRISONER:
   case ARMOR_TOGA:
   case ARMOR_MITHRIL:
   case ARMOR_WIFEBEATER:
   case ARMOR_MASK:
   case ARMOR_HEAVYARMOR:
   case ARMOR_SWATARMOR:
   case ARMOR_BONDAGEGEAR:
      return 1;
   default:
   case ARMOR_BUNKERGEAR:
   case ARMOR_POLICEUNIFORM:
   case ARMOR_POLICEARMOR:
   case ARMOR_ARMYARMOR:
   case ARMOR_MILITARY:
   case ARMOR_PRISONGUARD:
   case ARMOR_SECURITYUNIFORM:
   case ARMOR_CLOTHES:
   case ARMOR_WORKCLOTHES:
   case ARMOR_OVERALLS:
   case ARMOR_BLACKROBE:
   case ARMOR_TRENCHCOAT:
   case ARMOR_CIVILLIANARMOR:
   case ARMOR_LABCOAT:
      return 2;
   case ARMOR_CHEAPSUIT:
   case ARMOR_CHEAPDRESS:
   case ARMOR_BLACKSUIT:
   case ARMOR_BLACKDRESS:
      return 3;
   case ARMOR_EXPENSIVESUIT:
   case ARMOR_EXPENSIVEDRESS:
      return 4;
   }
}

bool armorst::faceconcealed()
{
   switch(type)
   {
   case ARMOR_MASK:
   case ARMOR_ELEPHANTSUIT:
   case ARMOR_DONKEYSUIT:
      return true;
   default:
      return false;
   }
}
