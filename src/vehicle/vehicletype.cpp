#include <externs.h>

int VehicleType::number_of_vehicletypes = 0;


VehicleType::VehicleType(MCD_STR xmlstring)
 : /*idname_("UNDEFINED"), id_(-1),*/ year_startcurrent_(true), year_start_(0), //Default values
 year_randomuptocurrent_(false), year_addrandom_(0), year_add_(0), displaycolor_(true),
 longname_("UNDEFINED"), shortname_("UNDEF"), drivebonus_(0),
 steal_difficultytofind_(1), steal_juice_(0), steal_extraheat_(0),
 sensealarmchance_(0), touchalarmchance_(0), availableatshop_(true), price_(1234), sleeperprice_(1111)
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
         drivebonus_ = atoi(xmlfile.GetData());
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
