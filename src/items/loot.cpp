
#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
//#include <includeEnum.h>
//#include <includeCommon.h>

/*
translateid.cpp
*/
#include "common\\translateid.h"

//#include <includeNews.h>
//#include <includeFunctions.h>
//#include <includeTitle.h>

//#include <includeExtern.h>
extern vector<LootType *> loottype;

Loot::Loot(const LootType& seed, int number) : Item(seed,number)
{ }

Loot::Loot(const std::string& inputXml) : Item(inputXml)
{
   /*CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem())
   {
      std::string tag=xml.GetTagName();
   }*/
}

string Loot::showXml() const
{
   CMarkup xml;
   xml.AddElem("loot");
   xml.IntoElem();
   addBaseValues(xml);
   return xml.GetDoc();
}

Loot* Loot::split(int number)
{
   if(number>number_) number=number_;
   Loot* newi=clone();
   newi->number_=number;
   number_-=number;
   return newi;
}

bool Loot::merge(Item& i)
{
   if(is_stackable()&&i.is_loot()&&is_same_type(i))
   {
      increase_number(i.get_number());
      i.set_number(0);
      return true;
   }
   return false;
}

bool Loot::sort_compare_special(Item* other) const
{
   if(other)
   {
      int thisi=getloottype(itemtypename());
      int otheri=getloottype(other->get_itemtypename());
      if(thisi<otheri||otheri==-1) return false;
      else if(thisi>otheri&&otheri!=-1) return true;
      else return false;
   }
   else return false;
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
