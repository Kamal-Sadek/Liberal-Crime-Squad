#include <externs.h>

long Vehicle::curcarid = 0;

string Vehicle::showXml() const
{
   CMarkup xml;
   xml.AddElem("vehicle");
   xml.IntoElem();
   xml.AddElem("vtypeidname", vtypeidname_);
   xml.AddElem("vtypeid", tostring(vtypeid_));
   xml.AddElem("color", color_);
   xml.AddElem("heat", tostring(heat_));
   xml.AddElem("location", tostring(location_));
   xml.AddElem("myear", tostring(myear_));
   xml.AddElem("id", tostring(id_));
   return xml.GetDoc();
}

Vehicle::Vehicle(const std::string& inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem())
   {
      std::string tag=xml.GetTagName();
      if(tag=="vtypeidname") vtypeidname_=xml.GetData();
      else if(tag=="vtypeid") vtypeid_=atoi(xml.GetData());
      else if(tag=="color") color_=xml.GetData();
      else if(tag=="heat") heat_=atoi(xml.GetData());
      else if(tag=="location") location_=atoi(xml.GetData());
      else if(tag=="myear") myear_=atoi(xml.GetData());
      else if(tag=="id") id_=atoi(xml.GetData());
   }
}

void Vehicle::init(const VehicleType& seed, const string& color, int myear)
{
   id_=curcarid++;
   heat_=0;
   location_=-1;
   vtypeidname_=seed.idname();
   vtypeid_=seed.id();
   color_=color;
   myear_=myear;
}

void Vehicle::stop_riding_me() const
{
   for(int p=0;p<len(pool);p++)
      if(pool[p]->carid==id_)
         pool[p]->carid=-1;
}

void Vehicle::stop_preferring_me() const
{
   for(int p=0;p<len(pool);p++)
      if(pool[p]->pref_carid==id_)
         pool[p]->pref_carid=-1;
}

string Vehicle::fullname(bool halffull) const
{
   string s;
   int words=0;
   if(heat_)
   {
	   s="Stolen ";
	   words++;
   }
   if(displayscolor())
   {
      s+=color_+" ";
	   words++;
   }
   if (myear_!=-1&&words<2) //don't print year if that will make the name too long.
      s+=tostring(myear_)+" ";
   if(halffull) s+=shortname();
   else s+=longname();

   return s;
}
