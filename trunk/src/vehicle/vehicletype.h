#ifndef VEHICLE_TYPE_H
#define VEHICLE_TYPE_H

#include "includes.h"

class VehicleType
{
   public:
      VehicleType(MCD_STR xmlstring);
   
      const string& idname() const { return idname_; }
      long id() const { return id_; }
      
      int makeyear() const;

      const vector<string>& color() const { return color_; }
      bool displayscolor() const { return displaycolor_; }
      const string& longname() const { return longname_; }
      const string& shortname() const { return shortname_; }
      int drivebonus() const { return drivebonus_; }
      
      int steal_difficultytofind() const { return steal_difficultytofind_; }
      int steal_juice() const { return steal_juice_; }
      int steal_extraheat() const { return steal_extraheat_; }
      int sensealarmchance() const { return sensealarmchance_; }
      int touchalarmchance() const { return touchalarmchance_; }
      
      bool availableatshop() const { return availableatshop_; }
      int price() const { return price_; }
      
      static int number_of_vehicletypes;
      
   private:
      string idname_;
      long id_;
      
      bool year_startcurrent_;
      int year_start_;
      bool year_randomuptocurrent_;
      int year_addrandom_;
      int year_add_;
      
      vector<string> color_;
      bool displaycolor_;
      string longname_;
      string shortname_;
      int drivebonus_;
      
      int steal_difficultytofind_;
      int steal_juice_;
      int steal_extraheat_;
      int sensealarmchance_;
      int touchalarmchance_;
      
      bool availableatshop_;
      int price_;
      
      int stringtobool(const std::string &s); //Should find better way than this. -XML
};

#endif //VEHICLE_TYPE_H
