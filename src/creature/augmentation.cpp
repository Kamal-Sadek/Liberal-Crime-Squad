/**
 * Implementation of the augmentations submodule.
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
#include "creature/augmentation.h"
#include "tinyxml2.h"


namespace
{
  const std::string AUGMENTATION_XML_AUGMENTATION_ELEMENT{"augmentation"};
  const std::string AUGMENTATION_XML_NAME_ELEMENT{"name"};
  const std::string AUGMENTATION_XML_TYPE_ELEMENT{"type"};
  const std::string AUGMENTATION_XML_ATTRIBUTE_ELEMENT{"attribute"};
  const std::string AUGMENTATION_XML_EFFECT_ELEMENT{"effect"};
  const std::string AUGMENTATION_XML_VALUE_ELEMENT{"value"};
} // anonymous namespace


std::string Augmentation::
get_name(int au)
{
   switch(au)
   {
      case AUGMENTATION_HEAD: return "Head";
      case AUGMENTATION_BODY: return "Body";
      case AUGMENTATION_ARMS: return "Arms";
      case AUGMENTATION_LEGS: return "Legs";
      case AUGMENTATION_SKIN: return "Skin";
      default: return "Conservative Swine! (Bug)";
   }
}

std::string Augmentation::
showXml() const
{
  return std::string{
    "<"+AUGMENTATION_XML_AUGMENTATION_ELEMENT+">"
      "<"+AUGMENTATION_XML_NAME_ELEMENT+">" + this->name + "</"+AUGMENTATION_XML_NAME_ELEMENT+">"
      "<"+AUGMENTATION_XML_TYPE_ELEMENT+">" + std::to_string(this->type) + "</"+AUGMENTATION_XML_TYPE_ELEMENT+">"
      "<"+AUGMENTATION_XML_ATTRIBUTE_ELEMENT+">" + std::to_string(this->attribute) + "</"+AUGMENTATION_XML_ATTRIBUTE_ELEMENT+">"
      "<"+AUGMENTATION_XML_EFFECT_ELEMENT+">" + std::to_string(this->effect) + "</"+AUGMENTATION_XML_EFFECT_ELEMENT+">"
      "<"+AUGMENTATION_XML_VALUE_ELEMENT+">" + std::to_string(this->value) + "</"+AUGMENTATION_XML_VALUE_ELEMENT+">"
    "</"+AUGMENTATION_XML_AUGMENTATION_ELEMENT+">"
  };
}

Augmentation::
Augmentation(std::string const& xml)
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError err = doc.Parse(xml.c_str());
  if (err != tinyxml2::XML_SUCCESS)
  {
    doc.PrintError();
    return;
  }

  auto e = doc.FirstChildElement();
  if ((e != nullptr) && (e->Name() == AUGMENTATION_XML_AUGMENTATION_ELEMENT))
  {
    for (auto element = e->FirstChildElement(); element; element = element->NextSiblingElement())
    {
      std::string tag = element->Name();

      if (tag == AUGMENTATION_XML_NAME_ELEMENT)
      {
        char const* val = element->GetText();
        if (val != nullptr)
          this->name = val;
      }
      else if (tag == AUGMENTATION_XML_TYPE_ELEMENT)
        this->type = std::stoi(element->GetText());
      else if (tag == AUGMENTATION_XML_ATTRIBUTE_ELEMENT)
        this->attribute = std::stoi(element->GetText());
      else if (tag == AUGMENTATION_XML_EFFECT_ELEMENT)
        this->effect = std::stoi(element->GetText());
      else if (tag == AUGMENTATION_XML_VALUE_ELEMENT)
        this->value = std::stoi(element->GetText());
    }
  }
}

