
#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"
#include <includeEnum.h>
#include <includeCommon.h>

/*
stringconversion.cpp
*/
#include "common\\stringconversion.h"

//#include <includeNews.h>
//#include <includeFunctions.h>
//#include <includeTitle.h>

//#include <includeExtern.h>
//The game log. All events and stuff are output here.
//NO debugging out. Make a debugging log for that.
extern Log xmllog;

int AugmentType::number_of_augmenttypes = 0;

AugmentType::AugmentType(const std::string& xmlstring): max_age_(-1), min_age_(-1), cost_(0), difficulty_(5)
{
   id_=number_of_augmenttypes++;

   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();

   idname_=xml.GetAttrib("idname");
   if(!len(idname_))
   {
      idname_ = "LACKS IDNAME "+tostring(id_);
      xmllog.log("Augment type "+tostring(id_)+" lacks idname.");
   }
   xml.IntoElem();

   while(xml.FindElem())
   {
      std::string element = xml.GetTagName();

      if(element=="name")
         name_=xml.GetData();
      else if(element=="type")
         type_=augment_string_to_enum(xml.GetData());
      else if(element=="attribute")
         attribute_= getAttributeFromInt(attribute_string_to_enum(xml.GetData()));
      else if(element=="effect")
         effect_=atoi(xml.GetData());
      else if(element=="description")
         description_=xml.GetData();
      else if(element=="max_age")
         max_age_=atoi(xml.GetData());
      else if(element=="min_age")
         min_age_=atoi(xml.GetData());
      else if(element=="cost")
         cost_=atoi(xml.GetData());
      else if(element=="difficulty")
         difficulty_=atoi(xml.GetData());
   }

}

void AugmentType::make_augment(Augmentation& au)
{
   au.name=get_name();
   au.type=get_type();
   au.attribute=get_attribute();
   au.effect=get_effect();
}
