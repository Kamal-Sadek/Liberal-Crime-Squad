#include <externs.h>

int VehicleType::number_of_vehicletypes = 0;


VehicleType::VehicleType(MCD_STR xmlstring)
 : /*idname_("UNDEFINED"), id_(-1),*/ year_startcurrent_(true), year_start_(0), //Default values
 year_randomuptocurrent_(false), year_addrandom_(0), year_add_(0), displaycolor_(true),
 longname_("UNDEFINED"), shortname_("UNDEF"), 
 steal_difficultytofind_(1), steal_juice_(0), steal_extraheat_(0),
 sensealarmchance_(0), touchalarmchance_(0), availableatshop_(true), price_(1234), sleeperprice_(1111), 
 armormidpoint_(50), lowarmormin_(4), lowarmormax_(6), higharmormin_(0), higharmormax_(2),
 drivebonus_(0), drivebonus_factor_(1), drivebonus_limit1_(8), drivebonus_limit2_(99), 
 dodgebonus_(0), dodgebonus_factor_(1), dodgebonus_limit1_(8), dodgebonus_limit2_(99), 
 attackbonus_driver_(-2), attackbonus_passenger_(0)
{
   id_ = number_of_vehicletypes++;

   CMarkup xmlfile;
   xmlfile.SetDoc(xmlstring);
   xmlfile.FindElem();

   idname_ = xmlfile.GetAttrib("idname");
   if (idname_ == "")
      idname_ = "LACKS IDNAME " + tostring(id_);

   xmlfile.IntoElem();

   while(xmlfile.FindElem()) //Loop over all the elements inside the vehicletype element.
   {
      std::string element = xmlfile.GetTagName();

      if (element == "year")
      {
         xmlfile.IntoElem();

         while(xmlfile.FindElem())
         {
            element = xmlfile.GetTagName();

            if (element == "start_at_current_year")
            {
               int b = stringtobool(xmlfile.GetData());
               if (b == 1)
                  year_startcurrent_ = true;
               else if (b == 0)
                  year_startcurrent_ = false;
               /*else
                  std::cerr << "Invalid boolean value for vehicle type " << idname
                            << "::year::start_at_current_year: " << xmlfile.GetData() << std::endl;*/
            }
            else if (element == "start_at_year")
               year_start_ = atoi(xmlfile.GetData());
            else if (element == "add_random_up_to_current_year")
            {
               int b = stringtobool(xmlfile.GetData());
               if (b == 1)
                  year_randomuptocurrent_ = true;
               else if (b == 0)
                  year_randomuptocurrent_ = false;
               /*else
                  std::cerr << "Invalid boolean value for vehicle type " << idname
                            << "::year::add_random_up_to_current_year: " << xmlfile.GetData() << std::endl;*/
            }
            else if (element == "add_random")
               year_addrandom_ = atoi(xmlfile.GetData());
            else if (element == "add")
               year_add_ = atoi(xmlfile.GetData());
            /*else
               std::cerr << "Unknown element for vehicle type " << idname << "::year: "
                         << element << std::endl;*/
         }

         xmlfile.OutOfElem();
      }
      else if (element == "colors")
      {
         xmlfile.IntoElem();
         //std::string color;
         while(xmlfile.FindElem())
         {
            element = xmlfile.GetTagName();

            if (element == "color")
            {
               color_.push_back(xmlfile.GetData());
            }
            else if (element == "display_color")
            {
               int b = stringtobool(xmlfile.GetData());
               if (b == 1)
                  displaycolor_ = true;
               else if (b == 0)
                  displaycolor_ = false;
               /*else
                  std::cerr << "Invalid boolean value for vehicle type " << idname
                            << "::colors::display_color: " << xmlfile.GetData() << std::endl;*/
            }
            /*else
               std::cerr << "Unknown element for vehicle type " << idname << "::colors: "
                         << element << std::endl;*/
         }
         xmlfile.OutOfElem();
      }
      else if (element == "drivebonus")
      {
         xmlfile.IntoElem();
         while(xmlfile.FindElem())
         {
            element = xmlfile.GetTagName();

            if (element == "base")
               drivebonus_ = atoi(xmlfile.GetData());
            else if (element == "skillfactor")
               drivebonus_factor_ = atof(xmlfile.GetData());
            else if (element == "softlimit")
               drivebonus_limit1_ = atoi(xmlfile.GetData());
            else if (element == "hardlimit")
               drivebonus_limit2_ = atoi(xmlfile.GetData());
            /*else
               std::cerr << "Unknown element for vehicle type " << idname << "::stealing: "
                         << element << std::endl;*/
         }
         xmlfile.OutOfElem();
      }
      else if (element == "dodgebonus")
      {
         xmlfile.IntoElem();
         while(xmlfile.FindElem())
         {
            element = xmlfile.GetTagName();

            if (element == "base")
               dodgebonus_ = atoi(xmlfile.GetData());
            else if (element == "skillfactor")
               dodgebonus_factor_ = atof(xmlfile.GetData());
            else if (element == "softlimit")
               dodgebonus_limit1_ = atoi(xmlfile.GetData());
            else if (element == "hardlimit")
               dodgebonus_limit2_ = atoi(xmlfile.GetData());
            /*else
               std::cerr << "Unknown element for vehicle type " << idname << "::stealing: "
                         << element << std::endl;*/
         }
         xmlfile.OutOfElem();
      }
      else if (element == "attackbonus")
      {
         xmlfile.IntoElem();
         while(xmlfile.FindElem())
         {
            element = xmlfile.GetTagName();

            if (element == "driver")
               attackbonus_driver_ = atoi(xmlfile.GetData());
            else if (element == "passenger")
               attackbonus_passenger_ = atoi(xmlfile.GetData());
            /*else
               std::cerr << "Unknown element for vehicle type " << idname << "::stealing: "
                         << element << std::endl;*/
         }
         xmlfile.OutOfElem();
      }
      else if (element == "longname")
         longname_ = xmlfile.GetData();
      else if (element == "shortname")
      {
         shortname_ = xmlfile.GetData();
         if (len(shortname_) > 7)
            shortname_.resize(7); //Only seven characters allowed for shortname_.
      }
      else if (element == "stealing")
      {
         xmlfile.IntoElem();
         while(xmlfile.FindElem())
         {
            element = xmlfile.GetTagName();

            if (element == "difficulty_to_find")
               steal_difficultytofind_ = atoi(xmlfile.GetData());
            else if (element == "juice")
               steal_juice_ = atoi(xmlfile.GetData());
            else if (element == "extra_heat")
               steal_extraheat_ = atoi(xmlfile.GetData());
            else if (element == "sense_alarm_chance")
               sensealarmchance_ = atoi(xmlfile.GetData());
            else if (element == "touch_alarm_chance")
               touchalarmchance_ = atoi(xmlfile.GetData());
            /*else
               std::cerr << "Unknown element for vehicle type " << idname << "::stealing: "
                         << element << std::endl;*/
         }
         xmlfile.OutOfElem();
      }
      else if (element == "armor")
      {
         xmlfile.IntoElem();
         while(xmlfile.FindElem())
         {
            element = xmlfile.GetTagName();

            if (element == "low_armor_min")
               lowarmormin_ = atoi(xmlfile.GetData());
            else if (element == "low_armor_max")
               lowarmormax_ = atoi(xmlfile.GetData());
            else if (element == "high_armor_min")
               higharmormin_ = atoi(xmlfile.GetData());
            else if (element == "high_armor_max")
               higharmormax_ = atoi(xmlfile.GetData());
            else if (element == "armor_midpoint")
               armormidpoint_ = atoi(xmlfile.GetData());
            /*else
               std::cerr << "Unknown element for vehicle type " << idname << "::stealing: "
                         << element << std::endl;*/
         }
         xmlfile.OutOfElem();
      }
      else if (element == "available_at_dealership")
      {
         int b = stringtobool(xmlfile.GetData());
         if (b == 1)
            availableatshop_ = true;
         else if (b == 0)
            availableatshop_ = false;
         /*else
            std::cerr << "Invalid boolean value for vehicle type " << idname
                      << "::available_at_dealership: " << xmlfile.GetData() << std::endl;*/
      }
      else if (element == "price")
         price_ = atoi(xmlfile.GetData());
      else if (element == "sleeperprice")
         sleeperprice_ = atoi(xmlfile.GetData());
      /*else
         std::cerr << "Unknown element for vehicle type " << idname << ": " << element
                   << std::endl;*/
   }

   if (len(color_) == 0)
      color_.push_back("Translucent"); //Default.

   //xmlfile.OutOfElem();
}

int VehicleType::makeyear() const
{
   int myear = 0;

   if (year_startcurrent_)
      myear = year;
   else
      myear = year_start_;

   if (year_randomuptocurrent_)
      myear += LCSrandom(year-year_start_+1);

   if (year_addrandom_ > 0)
      myear += LCSrandom(year_addrandom_);
   else if (year_addrandom_ < 0)
      myear -= LCSrandom(-year_addrandom_);

   myear += year_add_;

   return myear;
}


int VehicleType::gethitlocation(int bodypart)
{
   switch(bodypart)
   {
   case BODYPART_HEAD:
      return CARPART_WINDOW;
      break;
   case BODYPART_BODY:
   case BODYPART_ARM_RIGHT:
   case BODYPART_ARM_LEFT:
      return (LCSrandom(100) < armormidpoint_) ? CARPART_BODY : CARPART_WINDOW;
      break;
   case BODYPART_LEG_RIGHT:
   case BODYPART_LEG_LEFT:
      return CARPART_BODY;
      break;
   }
   
   return CARPART_WINDOW;
}
string VehicleType::getpartname(int location)
{
   return location==CARPART_WINDOW ? "window":"body";
}
int VehicleType::armorbonus(int location)
{
   switch(location)
   {
   case CARPART_BODY:
      return LCSrandom(lowarmormax_-lowarmormin_+1)+lowarmormin_;
   case CARPART_WINDOW:
      return LCSrandom(higharmormax_-higharmormin_+1)+higharmormin_;
   }
   return 0;
}

int VehicleType::modifieddriveskill(int skillLevel)
{
   int score = (skillLevel+drivebonus_)*drivebonus_factor_;
   if (score < drivebonus_limit1_)
      return score;
   if (score > drivebonus_limit1_)
      score = (score + drivebonus_limit1_)/2; // half credit after limit1
   return (score > drivebonus_limit2_) ? drivebonus_limit2_ : score;
}
int VehicleType::modifieddodgeskill(int skillLevel)
{
   int score = (skillLevel+dodgebonus_)*dodgebonus_factor_;
   if (score < dodgebonus_limit1_)
      return score;
   if (score > dodgebonus_limit1_)
      score = (score + dodgebonus_limit1_)/2; // half credit after limit1
   return (score > dodgebonus_limit2_) ? dodgebonus_limit2_ : score;
}