#include <externs.h>

int AugmentType::number_of_augmenttypes = 0;

AugmentType::AugmentType(const std::string& xmlstring): max_age_(-1), min_age_(-1), cost_(0)
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
         attribute_=attribute_string_to_enum(xml.GetData());
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
   }

}

void AugmentType::make_augment(Augmentation& au)
{
   au.name=get_name();
   au.type=get_type();
   au.attribute=get_attribute();
   au.effect=get_effect();
   au.max_age=get_max_age();
   au.min_age=get_min_age();
   au.cost=get_cost();
}
