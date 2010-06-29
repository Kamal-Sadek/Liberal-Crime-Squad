#include "includes.h"
#include "externs.h"

Vehicle::Vehicle(const VehicleType& seed)
{
   init(seed,seed.color()[LCSrandom(seed.color().size())],seed.makeyear());
}

Vehicle::Vehicle(const VehicleType& seed, const string& color, int myear)
{
   init(seed,color,year);
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

string Vehicle::fullname(bool halffull) const
{
   string s;
   int words = 0;
   if (heat_>0)
   {
	   s = "Stolen ";
	   words++;
   }
   else
	   s = "";
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

