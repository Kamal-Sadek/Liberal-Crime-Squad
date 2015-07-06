#include <externs.h>

std::string Augmentation::get_name(int au)
{
   switch(au)
   {
      case AUGMENTATION_HEAD: return "Head";
      case AUGMENTATION_BODY: return "Chest";
      case AUGMENTATION_ARMS: return "Arms";
      case AUGMENTATION_LEGS: return "Legs";
      case AUGMENTATION_SKIN: return "Skin";
      default: return "Conservative Swine! (Bug)";
   }
}
