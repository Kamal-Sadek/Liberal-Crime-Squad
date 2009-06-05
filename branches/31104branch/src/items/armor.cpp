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
   case ARMOR_DEATHSQUADUNIFORM:
      return 8;
   case ARMOR_MASK:
      switch(subtype)
      {
      case MASK_ASHCROFT:
      case MASK_BARR:
      case MASK_BLAIR:
      case MASK_BUSH_BARBARA:
      case MASK_BUSH_GEORGE:
      case MASK_BUSH_GEORGE2:
      case MASK_CARTER:
      case MASK_CHENEY:
      case MASK_CLINTON_BILL:
      case MASK_CLINTON_HILLARY:
      case MASK_FALWELL:
      case MASK_FORD:
      case MASK_GORE_AL:
      case MASK_GORE_TIPPER:
      case MASK_TED_STEVENS:
      case MASK_HELMS:
      case MASK_LINCOLN:
      case MASK_KENNEDY_JOHN:
      case MASK_KENNEDY_ROBERT:
      case MASK_KISSINGER:
      case MASK_NIXON:
      case MASK_POWELL:
      case MASK_REAGAN_NANCY:
      case MASK_REAGAN_RONALD:
      case MASK_RICE:
      case MASK_ROBERTSON:
      case MASK_RUMSFELD:
      case MASK_STARR:
      case MASK_THURMOND:
      case MASK_WASHINGTON:
      case MASK_WAXMAN:
      case MASK_COLEMAN_GARY:
      case MASK_MADONNA:
      case MASK_SPEARS:
      case MASK_EMINEM:
      case MASK_AGUILERA:
      case MASK_WAHLBERG:
      case MASK_IGGYPOP:
      case MASK_CASH:
      case MASK_LORDS:
      case MASK_JACKSON_MICHAEL:
      case MASK_SHIELDS:
      case MASK_CRUTHERS:
      case MASK_KING_DON:
      default:
         return 4;
      case MASK_GUY_FAWKES:
      case MASK_JASON:
      case MASK_SATAN:
      case MASK_JESUS:
      case MASK_BINLADEN:
         return 6;
      }
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
   case ARMOR_DEATHSQUADUNIFORM:
      return 6;
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
   case ARMOR_DEATHSQUADUNIFORM:
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
   case ARMOR_DEATHSQUADUNIFORM:
      return true;
   default:
      return false;
   }
}
