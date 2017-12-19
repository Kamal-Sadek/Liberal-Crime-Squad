/**
 * Implementation of the Money class.
 */
/*
 * Copyright 2010, 2013 Carlos Gustavos  <blomkvist>
 * Copyright 2014 Rich McGrew (yetisyny)
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
#include "items/money.h"
#include <externs.h>
#include <sstream>
#include "tinyxml2.h"


namespace
{
  const std::string MONEY_XML_MONEY_ELEMENT{"money"};
  const std::string MONEY_XML_AMOUNT_ELEMENT{"amount"};
} // anonymous namespace


Money::
Money(const std::string& xml)
: Item(xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing money XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if ((toplevel != nullptr) && toplevel->Name() == MONEY_XML_MONEY_ELEMENT)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == MONEY_XML_AMOUNT_ELEMENT)
         this->amount_ = std::stoi(element->GetText());
    }
  }
}


std::string Money::
item_class() const
{ return MONEY_XML_MONEY_ELEMENT; }


std::string Money::
xml_details() const
{
  return std::string{
    "<" + MONEY_XML_AMOUNT_ELEMENT + ">" + std::to_string(this->amount_) + "</" + MONEY_XML_AMOUNT_ELEMENT + ">"
  };
}


Money* Money::
split(int number)
{
   flatten();
   if(number>amount_) number=amount_;
   Money* newi=clone();
   newi->amount_=number;
   amount_-=number;
   return newi;
}


bool Money::
merge(Item& i)
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


bool Money::
sort_compare_special(Item* other) const
{ return other&&!other->is_money(); }

std::string Money::
equip_title() const
{ return "$"+std::to_string(amount_); }
