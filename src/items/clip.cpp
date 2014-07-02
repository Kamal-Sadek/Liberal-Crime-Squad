#include "externs.h"

Clip::Clip(const ClipType& seed, int number)
 : Item(seed,number)
{

}

Clip::Clip(const std::string& inputXml)
 : Item(inputXml)
{
   /*CMarkup xml;
   xml.SetDoc (inputXml);
   xml.FindElem ();
   xml.IntoElem ();

   while (xml.FindElem ())
   {
      std::string tag = xml.GetTagName ();

   }*/
}

string Clip::showXml () const
{
   CMarkup xml;
   xml.AddElem("clip");
   xml.IntoElem();

   addBaseValues(xml);

   return xml.GetDoc();
}

Clip* Clip::split(int number)
{
   if (number > number_)
      number = number_;

   Clip* newi = this->clone();
   newi->number_ = number;
   this->number_ -= number;

   return newi;
}

bool Clip::merge(Item& i)
{
   if (i.is_clip() && this->itemtypename() == i.get_itemtypename())
   {
      number_ += i.get_number();
      i.set_number(0);
      return true;
   }
   return false;
}

bool Clip::sort_compare_special(Item* other) const
{
   bool reorder = false;
   if (other != NULL)
   {
      int thisi = getcliptype(itemtypename());
      int otheri = getcliptype(other->get_itemtypename());
      if (thisi < otheri || otheri == -1)
         reorder = false;
      else if (thisi > otheri && otheri != -1)
         reorder = true;
   }
   return reorder;
}

string Clip::equip_title() const
   { return cliptype[getcliptype(itemtypename())]->get_name(); }

const string& Clip::get_name() const
   { return cliptype[getcliptype(itemtypename())]->get_name(); }

long Clip::get_fencevalue() const
   { return cliptype[getcliptype(itemtypename())]->get_fencevalue(); }

int Clip::get_ammoamount() const
   { return cliptype[getcliptype(itemtypename())]->get_ammoamount(); }


