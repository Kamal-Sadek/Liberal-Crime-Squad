#ifndef AUGMENTATION_H
#define AUGMENTATION_H

#include "includes.h"

class Augmentation
{
   public:
   	Augmentation() { }
   	Augmentation(const std::string& inputXml);
      string showXml() const;
      short type;
      std::string name;
      int attribute;
      int effect;
   	int value;
      int get_type() const { return type; }
   	static std::string get_name(int augmentation_type);
   	static int get_associated_attribute(int augmentation_type);
};

#endif
