#include <externs.h>

ClipType::ClipType(MCD_STR xmlstring) : ItemType(xmlstring), ammo_(1)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();

   while(xml.FindElem()) //Loop over all the elements inside the cliptype element.
   {
      std::string element=xml.GetTagName();
      if(element=="ammo")
         ammo_=atoi(xml.GetData());
      /*else
         errorlog << "Unknown element for clip type " << idname() << ": " << element << endl;*/
   }
}
