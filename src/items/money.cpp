
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

//#include <includeTalk.h>
//#include <includeExternDefault.h>
//#include <includeExternPolitics.h>
//#include <includeExternStat.h>


Money::Money(const std::string& inputXml) : Item(inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem()) if(xml.GetTagName()=="amount") amount_=atoi(xml.GetData());
}

string Money::showXml() const
{
   CMarkup xml;
   xml.AddElem("money");
   xml.IntoElem();
   addBaseValues(xml);
   xml.AddElem("amount",tostring(amount_));
   return xml.GetDoc();
}

Money* Money::split(int number)
{
   flatten();
   if(number>amount_) number=amount_;
   Money* newi=clone();
   newi->amount_=number;
   amount_-=number;
   return newi;
}

bool Money::merge(Item& i)
{
   flatten();
   if(i.is_money()&&is_same_type(i))
   {
      Money& m=static_cast<Money&>(i); //cast -XML
      m.flatten();
      take_all_from(m);
      m.set_number(0);
      return true;
   }
   else return false;
}

bool Money::sort_compare_special(Item* other) const
{ return other&&!other->is_money(); }

string Money::equip_title() const
{ return "$"+tostring(amount_); }
