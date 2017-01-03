
#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
//#include <includeEnum.h>
//#include <includeCommon.h>

/*
stringconversion.cpp
*/
#include "common\\stringconversion.h"

//#include <includeNews.h>
//#include <includeFunctions.h>
//#include <includeTitle.h>

//#include <includeExtern.h>

Item::Item(const ItemType& seed, int number) : number_(number)
{
   itemtypename_=seed.get_idname();
   itemtypeid_=seed.get_id();
}

Item::Item(const std::string& inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem())
   {
      std::string tag=xml.GetTagName();
      if(tag=="itemtypename") itemtypename_=xml.GetData();
      else if(tag=="itemtypeid") itemtypeid_=atoi(xml.GetData());
      else if(tag=="number") number_=atoi(xml.GetData());
   }
}

void Item::addBaseValues(CMarkup& xml) const
{
   xml.AddElem("itemtypename",itemtypename_);
   xml.AddElem("itemtypeid",itemtypeid_);
   xml.AddElem("number",tostring(number_));
}

/*Item* Item::split(int number)
{
   if(amount>number_) amount=number_;
   Item* newi=this->clone();
   newi->number_=amount;
   this->number_-=amount;
   return newi;
}*/

/* compares two items (in descending order, hence the flipped arguments), used in sorting gear */
bool Item::sort_compare(Item *b,Item *a)
{
   if(a->is_weapon()&&!b->is_weapon()) return false;
   else if(!a->is_weapon()&&b->is_weapon()) return true;
   else if(a->is_armor()&&!b->is_armor()) return false;
   else if(!a->is_armor()&&b->is_armor()) return true;
   else if(a->is_clip()&&!b->is_clip()) return false;
   else if(!a->is_clip()&&b->is_clip()) return true;
   else if(a->is_loot()&&!b->is_loot()) return false;
   else if(!a->is_loot()&&b->is_loot()) return true;
   else return a->sort_compare_special(b);
}

const char* Item::aan() const
{
   switch(get_name()[0])
   {
   case 'a': case 'A':
   case 'e': case 'E':
   case 'i': case 'I':
   case 'o': case 'O':
   case 'u': case 'U':
      return "an";
   default:
      return "a";
   }
}
