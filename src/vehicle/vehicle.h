#ifndef VEHICLE_H
#define VEHICLE_H

#include "includes.h"

// These 2 declarations are for stuff that isn't in vehicle.cpp or the Vehicle class.
// They're just needed by this header and implemented elsewhere.
extern vector<VehicleType *> vehicletype;
int getvehicletype(const string &idname);

class Vehicle
{
private:
   void init(const VehicleType& seed, const string& color, int myear);

   string vtypeidname_;
   long vtypeid_; //If the xml-file is changed vtypeid_ may not be correct after loading a saved game. So it's not actually used now. -XML
   string color_;
   //bool displaycolor_;
   short heat_;
   long location_;
   int myear_;
   long id_;
   //int drivebonus_;
   //string longname_;
   //string shortname_;
   //int steal_difficultytofind_;
   //int steal_juice_;
   //int steal_extraheat_;

public:
   explicit Vehicle(const VehicleType& seed) { init(seed,pickrandom(seed.color()),seed.makeyear()); }
   Vehicle(const VehicleType& seed, const string& color, int myear){ init(seed,color,myear); }
   ~Vehicle() { stop_riding_me(); stop_preferring_me(); }

   string showXml() const;
   Vehicle(const std::string& inputXml);

   void stop_riding_me() const;
   void stop_preferring_me() const; //GETS RID OF CAR PREFERENCES FROM pool LIBERALS.

   string fullname(bool halffull=false) const;

   short get_heat() const { return heat_; }
   void add_heat(short heat) { heat_ += heat; }
   long get_location() const { return location_; }
   void set_location(long new_location) { location_ = new_location; }

   const string& vtypeidname() const { return vtypeidname_; }
   long vtypeid() const { return vtypeid_; }
   const string& color() const { return color_; }
   bool displayscolor() const { return vehicletype[getvehicletype(vtypeidname_)]->displayscolor(); }
   int myear() const { return myear_; }
   long id() const { return id_; }
   int drivebonus() const { return vehicletype[getvehicletype(vtypeidname_)]->drivebonus(); }
   const string& longname() const { return vehicletype[getvehicletype(vtypeidname_)]->longname(); }
   const string& shortname() const { return vehicletype[getvehicletype(vtypeidname_)]->shortname(); }
   int steal_difficultytofind() const { return vehicletype[getvehicletype(vtypeidname_)]->steal_difficultytofind(); }
   int steal_juice() const { return vehicletype[getvehicletype(vtypeidname_)]->steal_juice(); }
   int steal_extraheat() const { return vehicletype[getvehicletype(vtypeidname_)]->steal_extraheat(); }
   int sensealarmchance() const { return vehicletype[getvehicletype(vtypeidname_)]->sensealarmchance(); }
   int touchalarmchance() const { return vehicletype[getvehicletype(vtypeidname_)]->touchalarmchance(); }
   bool availableatshop() const { return vehicletype[getvehicletype(vtypeidname_)]->availableatshop(); }
   int price() const { return vehicletype[getvehicletype(vtypeidname_)]->price(); }
   int sleeperprice() const { return vehicletype[getvehicletype(vtypeidname_)]->sleeperprice(); }

   static long curcarid;
};

#endif //VEHICLE_H
