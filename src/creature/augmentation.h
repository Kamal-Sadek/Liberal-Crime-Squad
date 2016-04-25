#ifndef AUGMENTATION_H
#define AUGMENTATION_H

#include "includes.h"

enum Augmentations
{
   AUGMENTATION_HEAD,
   AUGMENTATION_BODY,
   AUGMENTATION_ARMS,
   AUGMENTATION_LEGS,
   AUGMENTATION_SKIN,
   AUGMENTATIONNUM
};

class Augmentation
{
   public:
      Augmentation():type(-1),name(""),attribute(-1), effect(-1),
         value(-1) { }
      Augmentation(const std::string& inputXml);
      string showXml() const;
      std::string name;
      short type;
      int attribute;
      int effect;
      int value;
      static std::string get_name(int augmentation_type);
      static int get_associated_attribute(int augmentation_type);
};

#endif
