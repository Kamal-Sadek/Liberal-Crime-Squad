/**
 * Interface for the augmentations submodule.
 */
/*
 * Copyright 2015,2016 Kamal-Sadek  <kamaljalals@gmail.com>
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
#include <externs.h>
#include "tinyxml2.h"

namespace
{
  const std::string AUGMENTTYPE_XML_AUGMENTTYPE_ELEMENT{"augmenttype"};
} // anonymous namespace


int AugmentType::number_of_augmenttypes = 0;


AugmentType::
AugmentType(std::string const& xml)
: max_age_(-1)
, min_age_(-1)
, cost_(0)
, id_(number_of_augmenttypes++)
, difficulty_(5)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    doc.PrintError();
    return;
  }

  auto e = doc.FirstChildElement();
  if ((e != nullptr) && (e->Name() == AUGMENTTYPE_XML_AUGMENTTYPE_ELEMENT))
  {
    auto attr_idname = e->Attribute("idname");
    if (attr_idname == nullptr)
    {
      this->idname_ = "LACKS IDNAME "+std::to_string(this->id_);
      xmllog.log("Augment type "+std::to_string(this->id_)+" lacks idname.");
    }
    else
    {
      this->idname_ = attr_idname;
    }

    for (auto element = e->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      string tag = element->Name();
      if (tag == "name")
        this->name_ = element->GetText();
      else if (tag == "type")
         this->type_ = augment_string_to_enum(element->GetText());
      else if (tag == "attribute")
         this->attribute_ = attribute_string_to_enum(element->GetText());
      else if (tag == "effect")
         this->effect_ = std::stoi(element->GetText());
      else if (tag == "description")
         this->description_ = element->GetText();
      else if (tag == "max_age")
         this->max_age_ = std::stoi(element->GetText());
      else if (tag == "min_age")
         this->min_age_ = std::stoi(element->GetText());
      else if (tag == "cost")
         this->cost_ = std::stoi(element->GetText());
      else if (tag == "difficulty")
        this-> difficulty_ = std::stoi(element->GetText());
    }
  }
}


void AugmentType::
make_augment(Augmentation& au)
{
   au.name=get_name();
   au.type=get_type();
   au.attribute=get_attribute();
   au.effect=get_effect();
}
