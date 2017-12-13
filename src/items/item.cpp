/**
 * Implementation of the Weapon class.
 */
/*
 * Copyright 2010 Carlos Gustavos  <blomkvist>
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
#include "items/item.h"
#include <externs.h>
#include <sstream>
#include "tinyxml2.h"


namespace
{
  const std::string ITEM_XML_TYPENAME_ELEMENT{"itemtypename"};
  const std::string ITEM_XML_TYPEID_ELEMENT{"itemtypeid"};
  const std::string ITEM_XML_NUMBER_ELEMENT{"number"};
} // anonymous namespace


Item::Item(const ItemType& seed, int number) : number_(number)
{
   itemtypename_=seed.get_idname();
   itemtypeid_=seed.get_id();
}

Item::Item(const std::string& xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    std::ostringstream ostr;
    ostr << "error " << doc.ErrorID() << " parsing item XML"
         << " at line " << doc.GetErrorLineNum() << ": "
         << doc.GetErrorStr1() << " / " << doc.GetErrorStr2();
    addstr(ostr.str(), xmllog);
    getkey();
    return;
  }

  auto toplevel = doc.FirstChildElement();
  if (toplevel != nullptr)
  {
    for (auto element = toplevel->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();
      if (tag == ITEM_XML_TYPENAME_ELEMENT)
         this->itemtypename_ = element->GetText();
      else if (tag == ITEM_XML_TYPEID_ELEMENT)
         this->itemtypeid_ = std::stoi(element->GetText());
      else if (tag == ITEM_XML_NUMBER_ELEMENT)
         this->number_ = std::stoi(element->GetText());
    }
  }
}


std::string Item::
showXml() const
{
  return std::string{
    "<" + this->item_class() + ">"
       "<" + ITEM_XML_TYPENAME_ELEMENT + ">" + itemtypename_ + "</" + ITEM_XML_TYPENAME_ELEMENT + ">"
       "<" + ITEM_XML_TYPEID_ELEMENT + ">" + std::to_string(itemtypeid_) + "</" + ITEM_XML_TYPEID_ELEMENT + ">"
       "<" + ITEM_XML_NUMBER_ELEMENT + ">" + std::to_string(number_) + "</" + ITEM_XML_NUMBER_ELEMENT + ">"
       + this->xml_details() +
    "</" + this->item_class() + ">"
  };
}


std::string Item::
xml_details() const
{ return ""; }


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
