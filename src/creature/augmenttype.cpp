#include <externs.h>

int AugmentType::number_of_augmenttypes = 0;

AugmentType::AugmentType(const std::string& xmlstring)
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
      {
         std::string temp=xml.GetData();
         type_=augment_string_to_enum(temp);
      }
      else if(element=="attribute")
         attribute_=attribute_string_to_enum(xml.GetData());
      else if(element=="effect")
         effect_=atoi(xml.GetData());
   }

}

void AugmentType::make_augment(Augmentation& au) const
{
   au.name=name_;
   au.type=type_;
   au.attribute=attribute_;
   au.effect=effect_;
}
