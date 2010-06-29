#ifndef VEHICLE_H
#define VEHICLE_H

#include "includes.h"

class Vehicle
{
   public:
      Vehicle() {} //Used for loading save.
      Vehicle(const VehicleType& seed);
      Vehicle(const VehicleType& seed, const string& color, int myear);
      ~Vehicle()
      {
         stop_riding_me();
      }

      void stop_riding_me() const;
      
      string fullname(bool halffull=false) const;

      short& heat() { return heat_; }
      long& location() { return location_; }
      
      const string& vtypeidname() const { return vtypeidname_; }
      long vtypeid() const { return vtypeid_; }
      const string& color() const { return color_; }
      bool displayscolor() const;
      int myear() const { return myear_; }
      long id() const { return id_; }
      int drivebonus() const;
      const string& longname() const;
      const string& shortname() const;
      int steal_difficultytofind() const;
      int steal_juice() const;
      int steal_extraheat() const;
      int sensealarmchance() const;
      int touchalarmchance() const;
      bool availableatshop() const;
      int price() const;
      
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
};

#endif //VEHICLE_H
