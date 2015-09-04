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
      Augmentation():type(-1),name(""),attribute(-1),
         effect(-1),value(-1),max_age(-1),min_age(-1) { }
      Augmentation(const std::string& inputXml);
      string showXml() const;
      std::string name;
      short type;
      int attribute;
      int effect;
      int value;
      int max_age;
      int min_age;
      static std::string get_name(int augmentation_type);
      static int get_associated_attribute(int augmentation_type);
};

#endif
