#include "includes.h"
#include "externs.h"

long Vehicle::curcarid = 0;

string Vehicle::showXml () const
{
   char buf[256];

   CMarkup xml;
   xml.AddElem ("vehicle");
   xml.IntoElem ();
   xml.AddElem ("vtypeidname", vtypeidname_);

   snprintf (buf, 256, "%i", vtypeid_);
   xml.AddElem ("vtypeid", buf);

   xml.AddElem ("color", color_);

   snprintf (buf, 256, "%i", heat_);
   xml.AddElem ("heat", buf);

   snprintf (buf, 256, "%i", location_);
   xml.AddElem ("location", buf);

   snprintf (buf, 256, "%i", myear_);
   xml.AddElem ("myear", buf);

   snprintf (buf, 256, "%i", id_);
   xml.AddElem ("id", buf);

   return xml.GetDoc();
}

Vehicle::Vehicle(const char * inputXml)
{
   CMarkup xml;
   xml.SetDoc (inputXml);
   xml.FindElem ();
   xml.IntoElem ();
   
   while (xml.FindElem ()) {
      std::string tag = xml.GetTagName ();
      if (tag == "vtypeidname") {
	 vtypeidname_ = xml.GetData();
      }
      else if (tag == "vtypeid") {
	 vtypeid_ = atoi (xml.GetData().c_str());
      }
      else if (tag == "color") {
	 color_ = xml.GetData();
      }
      else if (tag == "heat") {
	 heat_ = atoi (xml.GetData().c_str());
      }
      else if (tag == "location") {
	 location_ = atoi (xml.GetData().c_str());
      }
      else if (tag == "myear") {
	 myear_ = atoi (xml.GetData().c_str());
      }
      else if (tag == "id") {
	 id_ = atoi (xml.GetData().c_str());
      }

   }
}


Vehicle::Vehicle(const VehicleType& seed)
{
   init(seed,seed.color()[LCSrandom(seed.color().size())],seed.makeyear());
}

Vehicle::Vehicle(const VehicleType& seed, const string& color, int myear)
{
   init(seed,color,myear);
}

void Vehicle::init(const VehicleType& seed, const string& color, int myear)
{
   id_=curcarid; curcarid++;

   heat_=0;
   location_=-1;
   vtypeidname_ = seed.idname();
   vtypeid_ = seed.id();
   color_ = color;
   myear_ = myear;
}

void Vehicle::stop_riding_me() const
{
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->carid==id_)
      {
         pool[p]->carid=-1;
      }
   }
}

void Vehicle::stop_preferring_me() const
{
   for(int p=0;p<pool.size();p++)
   {
      if(pool[p]->pref_carid==id_)
      {
         pool[p]->pref_carid=-1;
      }
   }
}

string Vehicle::fullname(bool halffull) const
{
   string s;
   int words = 0;
   if (heat_>0)
   {
	   s = "Stolen ";
	   words++;
   }
   if (displayscolor())
   {
      s += color_+" ";
	   words++;
   }
   if (myear_!=-1 && words<2)//don't print year if that will make the name too long.
   {
      s += tostring(myear_)+" ";
   }
   if (halffull)
      s += shortname();
   else
      s += longname();
      
   return s;
}

bool Vehicle::displayscolor() const
   { return vehicletype[getvehicletype(vtypeidname_)]->displayscolor(); }
int Vehicle::drivebonus() const
   { return vehicletype[getvehicletype(vtypeidname_)]->drivebonus(); }
const string& Vehicle::longname() const
   { return vehicletype[getvehicletype(vtypeidname_)]->longname(); }
const string& Vehicle::shortname() const
   { return vehicletype[getvehicletype(vtypeidname_)]->shortname(); }
int Vehicle::steal_difficultytofind() const
   { return vehicletype[getvehicletype(vtypeidname_)]->steal_difficultytofind(); }
int Vehicle::steal_juice() const
   { return vehicletype[getvehicletype(vtypeidname_)]->steal_juice(); }
int Vehicle::steal_extraheat() const
   { return vehicletype[getvehicletype(vtypeidname_)]->steal_extraheat(); }
int Vehicle::sensealarmchance() const
   { return vehicletype[getvehicletype(vtypeidname_)]->sensealarmchance(); }
int Vehicle::touchalarmchance() const
   { return vehicletype[getvehicletype(vtypeidname_)]->touchalarmchance(); }
bool Vehicle::availableatshop() const
   { return vehicletype[getvehicletype(vtypeidname_)]->availableatshop(); }
int Vehicle::price() const
   { return vehicletype[getvehicletype(vtypeidname_)]->price(); }

