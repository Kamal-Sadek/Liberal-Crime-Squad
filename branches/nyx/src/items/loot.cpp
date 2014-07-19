#include "externs.h"

Loot::Loot(const LootType& seed, int number)
 : Item(seed,number)
{
   
}

Loot::Loot(const char * inputXml)
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

string Loot::showXml () const
{
   CMarkup xml;
   xml.AddElem("loot");
   xml.IntoElem();

   addBaseValues(xml);

   return xml.GetDoc();
}

Loot* Loot::split(int number)
{
   if (number > number_)
      number = number_;
   
   Loot* newi = this->clone();
   newi->number_ = number;
   this->number_ -= number;
   
   return newi;
}

bool Loot::merge(Item& i)
{
   if (is_stackable() && i.is_loot() && this->is_same_type(i))
   {
      number_ += i.get_number();
      i.set_number(0);
      return true;
   }
   return false;
}

bool Loot::sort_compare_special(Item* other) const
{
   bool reorder = false;
   if (other != NULL)
   {
      int thisi = getloottype(itemtypename());
      int otheri = getloottype(other->get_itemtypename());
      if (thisi < otheri || otheri == -1)
         reorder = false;
      else if (thisi > otheri && otheri != -1)
         reorder = true;
   }
   return reorder;
}

string Loot::equip_title() const
   { return loottype[getloottype(itemtypename())]->get_name(); }
   
const string& Loot::get_name() const
   { return loottype[getloottype(itemtypename())]->get_name(); }

long Loot::get_fencevalue() const
   { return loottype[getloottype(itemtypename())]->get_fencevalue(); }

bool Loot::is_stackable() const
   { return loottype[getloottype(itemtypename())]->is_stackable(); }
bool Loot::no_quick_fencing() const
   { return loottype[getloottype(itemtypename())]->no_quick_fencing(); }
bool Loot::is_cloth() const
   { return loottype[getloottype(itemtypename())]->is_cloth(); }
