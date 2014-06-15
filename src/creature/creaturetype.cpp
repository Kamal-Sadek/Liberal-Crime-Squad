#include "externs.h"

// Assign a value to an Interval from a string or log error.
void assign_interval(Interval& i, const std::string& value,
                     const std::string& owner, const std::string& element)
{
   if (!i.set_interval(value))
      xmllog.log("Invalid interval for " + element + " in " + owner + ": " + value);
}

int CreatureType::number_of_creaturetypes = 0;

CreatureType::WeaponsAndClips::WeaponsAndClips(std::string weapon, int weapons, std::string clip, int clips)
 : weapontype(weapon), number_weapons(weapons),
   cliptype(clip), number_clips(clips)
{
}

CreatureType::WeaponsAndClips::WeaponsAndClips(CMarkup& xml, const string& owner)
 : number_weapons(1),
   cliptype("APPROPRIATE"), number_clips(4)
{ // The main position of the CMarkup object is expected not to be changed here.
   weapontype = xml.GetData();

   // Read in values.
   if (weapontype.empty())
   {
      while (xml.FindChildElem())
      {
         std::string element = xml.GetChildTagName();
         if (element == "type")
            weapontype = xml.GetChildData();
         else if (element == "number_weapons")
            assign_interval(number_weapons, xml.GetChildData(), owner, element);
         else if (element == "cliptype")
            cliptype = xml.GetChildData();
         else if (element == "number_clips")
            assign_interval(number_clips, xml.GetChildData(), owner, element);
         else
            xmllog.log("Unknown element for weapon in " + owner + ": "
                       + element);
      }
   }

   // Check values.
   if (weapontype != "CIVILIAN")
   {
      if (getweapontype(weapontype) == -1)
      {
         xmllog.log("Invalid weapon type for " + owner + ": " + weapontype);
         weapontype = "WEAPON_NONE";
         cliptype = "NONE";
      }
      else
      {
         const vector<attackst*>& attacks = ::weapontype[getweapontype(weapontype)]->get_attacks();

         // Find a usable clip type for the weapon.
         if (cliptype == "APPROPRIATE")
         {
            cliptype = "NONE";
            for (vector<attackst*>::const_iterator it = attacks.begin();
                 it != attacks.end();
                 ++it)
            {
               if ((*it)->uses_ammo)
               {
                  cliptype = (*it)->ammotype;
                  break;
               }
            }
         }
         // Check clip is usable by the weapon.
         else if (getcliptype(cliptype) != -1) //Must be a clip type too.
         {
            vector<attackst*>::const_iterator it = attacks.begin();
            while (it != attacks.end() && cliptype != (*it)->ammotype)
               ++it;
            if (it == attacks.end())
            {
               xmllog.log("In " + owner + ", " + cliptype +
                          "can not be used by " + weapontype + ".");
               cliptype = "NONE";
            }
         }
         // Undefined clip type.
         else
         {
            xmllog.log("Invalid clip type for " + owner + ": " + cliptype);
            cliptype = "NONE";
         }
      }
   }
}

CreatureType::CreatureType(const std::string& xmlstring)
 : age_(18,57), alignment_public_mood_(true),
   attribute_points_(40),
   gender_liberal_(GENDER_RANDOM), gender_conservative_(GENDER_RANDOM),
   infiltration_(0), juice_(0), money_(20,40)
{
   for (int i = 0; i < ATTNUM; ++i)
      attributes_[i].set_interval(1, 10);

   id_ = number_of_creaturetypes++;

   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();

   idname_ = xml.GetAttrib("idname");
   if (idname_.empty())
   {
      idname_ = "LACKS IDNAME " + tostring(id_);
      xmllog.log("Creature type " + tostring(id_) + " lacks idname.");
   }
   type_ = creaturetype_string_to_enum(idname_);

   xml.IntoElem();
   // Loop over all the elements inside the creaturetype element.
   while (xml.FindElem())
   {
      std::string element = xml.GetTagName();

      if (element == "alignment")
      {
         std::string alignment = xml.GetData();
         if (alignment == "PUBLIC MOOD")
            alignment_public_mood_ = true;
         else if (alignment == "LIBERAL")
         {
            alignment_ = ALIGN_LIBERAL;
            alignment_public_mood_ = false;
         }
         else if (alignment == "MODERATE")
         {
            alignment_ = ALIGN_MODERATE;
            alignment_public_mood_ = false;
         }
         else if (alignment == "CONSERVATIVE")
         {
            alignment_ = ALIGN_CONSERVATIVE;
            alignment_public_mood_ = false;
         }
         else
            xmllog.log("Invalid alignment for " + idname_ + ": " + alignment);
      }
      else if (element == "age")
      {
         std::string age = xml.GetData();
         if (age == "DOGYEARS")
            age_.set_interval(2,6);
         else if (age == "CHILD")
            age_.set_interval(7,10);
         else if (age == "TEENAGER")
            age_.set_interval(14,17);
         else if (age == "YOUNGADULT")
            age_.set_interval(18,35);
         else if (age == "MATURE")
            age_.set_interval(20,59);
         else if (age == "GRADUATE")
            age_.set_interval(26,59);
         else if (age == "MIDDLEAGED")
            age_.set_interval(35,59);
         else if (age == "SENIOR")
            age_.set_interval(65,94);
         else
            assign_interval(age_, age, idname_, element);
      }
      else if (element == "attribute_points")
         assign_interval(attribute_points_, xml.GetData(), idname_, element);
      else if (element == "attributes")
      {
         while (xml.FindChildElem())
         {
            int attribute = attribute_string_to_enum(xml.GetChildTagName());
            if (attribute != -1)
               assign_interval(attributes_[attribute], xml.GetChildData(), idname_, element);
            else
               xmllog.log("Unknown attribute in " + idname_ + ": " + xml.GetTagName());
         }
      }
      else if (element == "juice")
         assign_interval(juice_, xml.GetData(), idname_, element);
      else if (element == "gender")
      {
         int gender = gender_string_to_enum(xml.GetData());
         if (gender != -1 && gender != GENDER_WHITEMALEPATRIARCH)
            gender_liberal_ = gender_conservative_ = gender;
         else
            xmllog.log("Invalid gender for " + idname_ + ": " + xml.GetData());
      }
      else if (element == "infiltration")
         assign_interval(infiltration_, xml.GetData(), idname_, element);
      else if (element == "money")
         assign_interval(money_, xml.GetData(), idname_, element);
      else if (element == "skills")
      {
         while (xml.FindChildElem())
         {
            int skill = skill_string_to_enum(xml.GetChildTagName());
            if (skill != -1)
               assign_interval(skills_[skill], xml.GetChildData(), idname_, element);
            else
               xmllog.log("Unknown skill for " + idname_ + ": " + xml.GetChildTagName());
         }
      }
      else if (element == "armor")
      {
         if (getarmortype(xml.GetData()) != -1)
            armortypes_.push_back(xml.GetData());
         else
            xmllog.log("Invalid armor type for " + idname_ + ": " + xml.GetData());;
      }
      else if (element == "weapon")
      {
         //xml.SavePos("creature");
         weapons_and_clips_.push_back(WeaponsAndClips(xml, idname_));
         //xml.RestorePos("creature");
      }
      else if (element == "encounter_name")
         encounter_name_ = xml.GetData();
      else if (element == "type_name")
         type_name_ = xml.GetData();
      else
         xmllog.log("Unknown element for " + idname_ + ": " + element);
   }

   if (type_name_.empty())
   {
      xmllog.log("type_name not defined for " + idname_ + ".");
      type_name_ = "UNDEFINED";
   }
   // If no weapon type has been given then use WEAPON_NONE.
   if (weapons_and_clips_.empty())
      weapons_and_clips_.push_back(WeaponsAndClips("WEAPON_NONE", 1, "NONE", 0));
   // If no armor type has been given then use ARMOR_NONE.
   if (armortypes_.empty())
      armortypes_.push_back("ARMOR_NONE");
}

void CreatureType::make_creature(Creature& cr) const
{
   cr.type_idname = idname_;
   cr.align = get_alignment();
   cr.age = age_.roll();
   cr.juice = juice_.roll();
   cr.gender_liberal = cr.gender_conservative = roll_gender();
   cr.infiltration = roll_infiltration();
   cr.money = money_.roll();
   strcpy(cr.name, get_encounter_name());
   for (int i = 0; i < SKILLNUM; i++)
      cr.set_skill(i, skills_[i].roll());

   give_armor(cr);
   give_weapon(cr);
}

int CreatureType::get_alignment() const
{
   if (alignment_public_mood_)
   {
      int mood = publicmood(-1);
      int a = ALIGN_CONSERVATIVE;
      if (LCSrandom(100) < mood) ++a;
      if (LCSrandom(100) < mood) ++a;
      return a;
   }
   else
      return alignment_;
}

int CreatureType::roll_gender() const
{
   int gender = LCSrandom(2)+1; // Male or female.
   switch (gender_liberal_)
   {
      case GENDER_NEUTRAL: return GENDER_NEUTRAL;
      case GENDER_MALE:    return GENDER_MALE;
      case GENDER_FEMALE:  return GENDER_FEMALE;
      case GENDER_MALE_BIAS:
      {
         if ( law[LAW_WOMEN] == -2 ||
             (law[LAW_WOMEN] == -1 && LCSrandom(25)) ||
             (law[LAW_WOMEN] == 0  && LCSrandom(10)) ||
             (law[LAW_WOMEN] == 1  && LCSrandom(4)))
               return GENDER_MALE;
      }
      case GENDER_FEMALE_BIAS:
      {
         if ( law[LAW_WOMEN] == -2 ||
             (law[LAW_WOMEN] == -1 && LCSrandom(25)) ||
             (law[LAW_WOMEN] == 0  && LCSrandom(10)) ||
             (law[LAW_WOMEN] == 1  && LCSrandom(4)))
               return GENDER_FEMALE;
      }
   }
   return gender;
}

float CreatureType::roll_infiltration() const
{
   return infiltration_.roll()/100.0f;
}

std::string CreatureType::get_encounter_name() const
{
   if (!encounter_name_.empty())
      return encounter_name_;
   else
      return get_type_name();
}

std::string CreatureType::get_type_name() const
{
   switch (type_) // Hardcoded special cases.
   {
      case CREATURE_WORKER_SERVANT:
         if (law[LAW_LABOR] == -2 && law[LAW_CORPORATE] == -2)
            return "Slave";
	 break;
      case CREATURE_WORKER_JANITOR:
         if (law[LAW_LABOR] == 2)
            return "Custodian";
	 break;
      case CREATURE_WORKER_SWEATSHOP:
         if (law[LAW_LABOR] == 2 && law[LAW_IMMIGRATION] == 2)
            return "Migrant Worker";
	 break;
      case CREATURE_CARSALESMAN:
         if (law[LAW_WOMEN] == -2)
            return "Car Salesman";
	 break;
      case CREATURE_FIREFIGHTER:
         if (law[LAW_FREESPEECH] == -2)
            return "Fireman";
	 break;
   }
   return type_name_;
}

void CreatureType::give_weapon(Creature& cr) const
{
   int i = LCSrandom(weapons_and_clips_.size());
   const WeaponsAndClips& wc = weapons_and_clips_[i];

   if (wc.weapontype == "CIVILIAN")
      give_weapon_civilian(cr);
   else if (wc.weapontype != "WEAPON_NONE")
   {
      Weapon w(*weapontype[getweapontype(wc.weapontype)], wc.number_weapons.roll());
      w.set_number(min(w.get_number(), 10L));
      while (!w.empty())
         cr.give_weapon(w, NULL);
      if (wc.cliptype != "NONE")
      {
         int n = wc.number_clips.roll();
         cr.take_clips(*cliptype[getcliptype(wc.cliptype)], n);
         cr.reload(false);
      }
   }
}

void CreatureType::give_weapon_civilian(Creature& cr) const
{
   if (law[LAW_GUNCONTROL] == -1 && !LCSrandom(30))
   {
      cr.give_weapon(*weapontype[getweapontype("WEAPON_REVOLVER_38")], NULL);
      cr.take_clips(*cliptype[getcliptype("CLIP_38")], 4);
      cr.reload(false);
   }
   else if (law[LAW_GUNCONTROL] == -2)
   {
      if (!LCSrandom(10))
      {
         cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_9MM")], NULL);
         cr.take_clips(*cliptype[getcliptype("CLIP_9")], 4);
         cr.reload(false);
      }
      else if (!LCSrandom(9))
      {
         cr.give_weapon(*weapontype[getweapontype("WEAPON_SEMIPISTOL_45")], NULL);
         cr.take_clips(*cliptype[getcliptype("CLIP_45")], 4);
         cr.reload(false);
      }
   }
}

void CreatureType::give_armor(Creature& cr) const
{
   int n = LCSrandom(armortypes_.size());
   if (armortypes_[n] != "ARMOR_NONE")
      cr.give_armor(*armortype[getarmortype(armortypes_[n])], NULL);
}
