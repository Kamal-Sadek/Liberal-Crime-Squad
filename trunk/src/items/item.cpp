#include "externs.h"

Item::Item(const ItemType& seed, int number)
 : number_(number)
{
   itemtypename_ = seed.get_idname();
   itemtypeid_ = seed.get_id();
   //number_ = number;
}

Item::Item(const char * inputXml)
{
   CMarkup xml;
   xml.SetDoc (inputXml);
   xml.FindElem ();
   xml.IntoElem ();
   
   while (xml.FindElem ())
   {
      std::string tag = xml.GetTagName ();
      if (tag == "itemtypename")
      {
         itemtypename_ = xml.GetData();
      }
      else if (tag == "itemtypeid")
      {
         itemtypeid_ = atoi(xml.GetData().c_str());
      }
      else if (tag == "number")
      {
         number_ = atoi(xml.GetData().c_str());
      }
   }
}

void Item::addBaseValues(CMarkup& xml) const
{
   xml.AddElem("itemtypename", itemtypename_);

   xml.AddElem("itemtypeid", itemtypeid_);
   
   xml.AddElem("number", tostring(number_));
}

/*Item* Item::split(int number)
{
   if (amount > number_)
      amount = number_;
   
   Item* newi = this->clone();
   newi->number_ = amount;
   this->number_ -= amount;
   
   return newi;
}*/

/* compares two items, used in sorting gear */
bool Item::sort_compare(Item *a,Item *b)
{
   bool reorder;

   if (a->is_weapon() && !b->is_weapon())
      reorder = false;
   else if (!a->is_weapon() && b->is_weapon())
      reorder = true;
   else if (a->is_armor() && !b->is_armor())
      reorder = false;
   else if (!a->is_armor() && b->is_armor())
      reorder = true;
   else if (a->is_clip() && !b->is_clip())
      reorder = false;
   else if (!a->is_clip() && b->is_clip())
      reorder = true;
   else if (a->is_loot() && !b->is_loot())
      reorder = false;
   else if (!a->is_loot() && b->is_loot())
      reorder = true;
   else
      reorder = a->sort_compare_special(b);
   
   return !reorder;
}

bool Item::is_same_type(const Item& i)
{
    return itemtypename_ == i.itemtypename_;
}
