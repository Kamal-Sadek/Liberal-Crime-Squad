#include "externs.h"

ArmorType::ArmorType(MCD_STR xmlstring)
 : ItemType(xmlstring),
   make_difficulty_(0), make_price_(0), deathsquad_legality_(false),
   can_get_bloody_(true), can_get_damaged_(true),
   armor_body_(0), armor_head_(0), armor_limbs_(0), fireprotection_(false),
   cover_head_(false), cover_body_(true), cover_arms_(true), cover_legs_(true),
   conceal_face_(false), stealth_value_(0),
   shortname_("UNDEF"), shortname_defined_(false), shortname_future_defined_(false),
   interrogation_basepower_(0), interrogation_assaultbonus_(0), interrogation_drugbonus_(0),
   professionalism_(2), conceal_weaponsize_(5),
   mask_(false), surprise_mask_(false)
{
   init(xmlstring);         
}

ArmorType::ArmorType(const ArmorType& base, MCD_STR xmlstring)
 : ItemType(base, xmlstring)
{
   make_difficulty_ = base.make_difficulty_;
   make_price_ = base.make_price_;
   deathsquad_legality_ = base.deathsquad_legality_;
   armor_body_ = base.armor_body_;
   armor_head_ = base.armor_head_;
   armor_limbs_ = base.armor_limbs_;
   fireprotection_ = base.fireprotection_;
   cover_head_ = base.cover_head_;
   cover_body_ = base.cover_body_;
   cover_arms_ = base.cover_arms_;
   cover_legs_ = base.cover_legs_;
   conceal_face_ = base.conceal_face_;
   shortname_ = base.shortname_;
   shortname_future_ = base.shortname_future_;
   shortname_defined_ = base.shortname_defined_;
   shortname_future_defined_ = base.shortname_future_defined_;   
   interrogation_basepower_ = base.interrogation_basepower_;
   interrogation_assaultbonus_ = base.interrogation_assaultbonus_;
   interrogation_drugbonus_ = base.interrogation_drugbonus_;
   professionalism_ = base.professionalism_;
   conceal_weaponsize_ = base.conceal_weaponsize_;
   stealth_value_ = base.stealth_value_;
   mask_ = base.mask_;
   surprise_mask_ = base.surprise_mask_;
   description_ = base.description_;
   
   init(xmlstring);
}

void ArmorType::init(const MCD_STR& xmlstring)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   
   xml.IntoElem();

   while (xml.FindElem()) //Loop over all the elements inside the armortype element.
   {
      std::string element = xml.GetTagName();

      if (element == "make_difficulty")
         make_difficulty_ = atoi(xml.GetData().c_str());
      else if (element == "make_price")
         make_price_ = atoi(xml.GetData().c_str());
      else if (element == "deathsquad_legality")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            deathsquad_legality_ = true;
         else if (b == 0)
            deathsquad_legality_ = false;
         /*else
            errorlog << "Invalid boolean value for armor type " << idname()
                      << "::deathsquad_legality: " << xml.GetData() << std::endl;*/
      }
      else if (element == "can_get_bloody")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_get_bloody_ = true;
         else if (b == 0)
            can_get_bloody_ = false;
      }
      else if (element == "can_get_damaged")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_get_damaged_ = true;
         else if (b == 0)
            can_get_damaged_ = false;
      }
      else if (element == "armor")
      {
         xml.IntoElem();
         
         while (xml.FindElem())
         {
            element = xml.GetTagName();
            
            if (element == "body")
               armor_body_ = atoi(xml.GetData().c_str());
            else if (element == "head")
               armor_head_ = atoi(xml.GetData().c_str());
            else if (element == "limbs")
               armor_limbs_ = atoi(xml.GetData().c_str());
            else if (element == "fireprotection")
            {
               int b = stringtobool(xml.GetData());
               if (b == 1)
                  fireprotection_ = true;
               else if (b == 0)
                  fireprotection_ = false;
               /*else
                  errorlog << "Invalid boolean value for armor type " << idname()
                            << "::armor::fireprotection: " << xml.GetData() << std::endl;*/
            }
            /*else
               errorlog << "Unknown element for armor type " << idname()
                         << "::armor: " << element << endl;*/
         }
         xml.OutOfElem();
      }
      else if (element == "body_covering")
      {
         xml.IntoElem();
         
         while (xml.FindElem())
         {
            element = xml.GetTagName();
            
            if (element == "body")
            {
               int b = stringtobool(xml.GetData());
               if (b == 1)
                  cover_body_ = true;
               else if (b == 0)
                  cover_body_ = false;
               /*else
                  errorlog << "Invalid boolean value for armor type " << idname()
                            << "::body_covering::body: " << xml.GetData() << std::endl;*/
            }
            else if (element == "head")
            {
               int b = stringtobool(xml.GetData());
               if (b == 1)
                  cover_head_ = true;
               else if (b == 0)
                  cover_head_ = false;
               /*else
                  errorlog << "Invalid boolean value for armor type " << idname()
                            << "::body_covering::head: " << xml.GetData() << std::endl;*/
            }
            else if (element == "arms")
            {
               int b = stringtobool(xml.GetData());
               if (b == 1)
                  cover_arms_ = true;
               else if (b == 0)
                  cover_arms_ = false;
               /*else
                  errorlog << "Invalid boolean value for armor type " << idname()
                            << "::body_covering::arms: " << xml.GetData() << std::endl;*/
            }
            else if (element == "legs")
            {
               int b = stringtobool(xml.GetData());
               if (b == 1)
                  cover_legs_ = true;
               else if (b == 0)
                  cover_legs_ = false;
               /*else
                  errorlog << "Invalid boolean value for armor type " << idname()
                            << "::body_covering::legs: " << xml.GetData() << std::endl;*/
            }
            else if (element == "conceals_face")
            {
               int b = stringtobool(xml.GetData());
               if (b == 1)
                  conceal_face_ = true;
               else if (b == 0)
                  conceal_face_ = false;
               /*else
                  errorlog << "Invalid boolean value for armor type " << idname()
                            << "::body_covering::conceal_face: " << xml.GetData() << std::endl;*/
            }
            /*else
               errorlog << "Unknown element for armor type " << idname()
                         << "::armor: " << element << endl;*/
         }
         xml.OutOfElem();
      }
      else if (element == "shortname")
      {
         shortname_ = xml.GetData();
         shortname_defined_ = true;
         if (shortname_.length() > 14)
            shortname_.resize(14);
      }
      else if (element == "interrogation")
      {
         xml.IntoElem();
         
         while (xml.FindElem())
         {
            if (element == "basepower")
               interrogation_basepower_ = atoi(xml.GetData().c_str());
            else if (element == "assaultbonus")
               interrogation_assaultbonus_ = atoi(xml.GetData().c_str());
            else if (element == "drugbonus")
               interrogation_drugbonus_ = atoi(xml.GetData().c_str());
            /*else
             errorlog << "Unknown element for armor type " << idname()
                         << "::interrogation: " << element << endl;*/
         }
         
         xml.OutOfElem();
      }
      else if (element == "professionalism")
         professionalism_ = atoi(xml.GetData().c_str());
      else if (element == "conceal_weapon_size")
         conceal_weaponsize_ = atoi(xml.GetData().c_str());
      else if (element == "stealth_value")
         stealth_value_ = atoi(xml.GetData().c_str());
      else if (element == "mask")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            mask_ = true;
         else if (b == 0)
            mask_ = false;
      }
      else if (element == "surprise")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            surprise_mask_ = true;
         else if (b == 0)
            surprise_mask_ = false;
      }
      else if (element == "description")
         description_ = xml.GetData();
      /*else
         errorlog << "Unknown element for armor type " << idname() << ": " << element << endl;*/
   }
   
   if (!shortname_defined_ && name().length() <= 14)
      shortname_ = name();
}

int ArmorType::get_armor(int bodypart) const
{
   if(covers(bodypart))
   {
      switch(bodypart)
      {
         case BODYPART_HEAD:
            return armor_head_;

         case BODYPART_BODY:
            return armor_body_;

         case BODYPART_ARM_RIGHT:
         case BODYPART_ARM_LEFT:
            return armor_limbs_;

         case BODYPART_LEG_RIGHT:
         case BODYPART_LEG_LEFT:
            return armor_limbs_;
      }
   }
   
   return 0;
}

bool ArmorType::covers(int bodypart) const
{
   switch(bodypart)
   {
      case BODYPART_HEAD: return cover_head_;
      case BODYPART_BODY: return cover_body_;
      case BODYPART_ARM_RIGHT:
      case BODYPART_ARM_LEFT: return cover_arms_;
      case BODYPART_LEG_RIGHT:
      case BODYPART_LEG_LEFT: return cover_legs_;
   }
   
   return false;
}

const string& ArmorType::get_shortname() const
{
   if (shortname_future_defined_ && year >= 2100)
      return shortname_future_;
   else if (year >= 2100 && name_future_defined() && name_future().length() <= 14)
      return name_future();
   else //if (shortname_defined_)
      return shortname_;
   /*else if (name().length() <= 14)
      return name();
   else
      return "UNDEF";*/
}

bool ArmorType::conceals_weaponsize(int weaponsize) const
{
   return (conceal_weaponsize_ >= weaponsize);
}

/*const string& get_appropriate_weapon(int index) const
{
   if (index < appropriate_weapon_.size())
      return appropriate_weapon_[index];
   else
      return "";
}*/
