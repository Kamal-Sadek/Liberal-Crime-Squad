#ifndef AUGMENT_TYPE_H
#define AUGMENT_TYPE_H

#include "includes.h"

class AugmentType {
   public:
      explicit AugmentType(const std::string& xmlstring);

      void make_augment(Augmentation& au) const;

      const std::string get_idname() const { return idname_; }
      const std::string get_name() const { return name_; }
      long get_id() const { return id_; }
      short get_type() const { return type_; }
      int get_effect() const { return effect_; }

		static int number_of_augmenttypes;
	private:
		std::string idname_;
      std::string name_;
      int attribute_;
      int effect_;
      long id_;
	   short type_;
};

#endif
