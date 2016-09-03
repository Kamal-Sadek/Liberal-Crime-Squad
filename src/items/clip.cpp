
#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
#include <includeEnum.h>
#include <includeCommon.h>

/*
translateid.cpp
*/
#include "common\\translateid.h"

//#include <includeNews.h>
//#include <includeFunctions.h>
//#include <includeTitle.h>

//#include <includeTalk.h>
#include <includeExternDefault.h>
//#include <includeExternPolitics.h>
//#include <includeExternStat.h>

Clip::Clip(const ClipType& seed, int number) : Item(seed,number)
{ }

Clip::Clip(const std::string& inputXml) : Item(inputXml)
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

string Clip::showXml() const
{
   CMarkup xml;
   xml.AddElem("clip");
   xml.IntoElem();
   addBaseValues(xml);
   return xml.GetDoc();
}

Clip* Clip::split(int number)
{
   if(number>number_) number=number_;
   Clip* newi=this->clone();
   newi->number_=number;
   this->number_-=number;
   return newi;
}

bool Clip::merge(Item& i)
{
   if(i.is_clip() && this->is_same_type(i))
   {
      this->increase_number(i.get_number());
      i.set_number(0);
      return true;
   }
   return false;
}

bool Clip::sort_compare_special(Item* other) const
{
   if(other)
   {
      int thisi=getcliptype(itemtypename());
      int otheri=getcliptype(other->get_itemtypename());
      if(thisi<otheri||otheri==-1) return false;
      else if(thisi>otheri&&otheri!=-1) return true;
      else return false;
   }
   else return false;
}

string Clip::equip_title() const
{ return cliptype[getcliptype(itemtypename())]->get_name(); }

const string& Clip::get_name() const
{ return cliptype[getcliptype(itemtypename())]->get_name(); }

long Clip::get_fencevalue() const
{ return cliptype[getcliptype(itemtypename())]->get_fencevalue(); }

int Clip::get_ammoamount() const
{ return cliptype[getcliptype(itemtypename())]->get_ammoamount(); }
