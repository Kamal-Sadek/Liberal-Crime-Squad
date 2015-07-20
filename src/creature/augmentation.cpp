#include <externs.h>

std::string Augmentation::get_name(int au)
{
   switch(au)
   {
      case AUGMENTATION_HEAD: return "Head";
      case AUGMENTATION_BODY: return "Chest";
      case AUGMENTATION_ARMS: return "Arms";
      case AUGMENTATION_LEGS: return "Legs";
      case AUGMENTATION_SKIN: return "Skin";
      default: return "Conservative Swine! (Bug)";
   }
}

std::string Augmentation::showXml() const
{
   CMarkup xml;
   xml.AddElem("augmentation");
   xml.IntoElem();
   xml.AddElem("name", name);
   xml.AddElem("type", type);
   xml.AddElem("attribute", attribute);
   xml.AddElem("effect", effect);
   xml.AddElem("value", value);

   return xml.GetDoc();
}

Augmentation::Augmentation(const std::string& inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();

   while(xml.FindElem())
   {
      std::string tag = xml.GetTagName();

      if(tag == "name")
         name = xml.GetData();
      else if(tag == "type")
         type = atoi(xml.GetData());
      else if(tag == "attribute")
         attribute = atoi(xml.GetData());
      else if(tag == "effect")
         effect = atoi(xml.GetData());
      else if(tag == "value")
         value = atoi(xml.GetData());
   }
}

