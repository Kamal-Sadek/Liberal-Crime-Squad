#ifndef CLIP_H
#define CLIP_H

//#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"

class Clip : public Item
{
   public:
      explicit Clip(const ClipType& seed, int number = 1);
      virtual Clip* clone() const { return new Clip(*this); }
      explicit Clip(const std::string& inputXml);
      string showXml() const;

      bool is_clip() const { return true; }

      virtual Clip* split(int number);
      virtual bool merge(Item& i);
      virtual bool sort_compare_special(Item* other) const;

      virtual string equip_title() const;
      virtual const string& get_name() const;
      virtual long get_fencevalue() const;

      int get_ammoamount() const;

   private:

};

#endif //CLIP_H
