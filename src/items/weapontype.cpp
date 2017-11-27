#include <externs.h>

WeaponType::WeaponType(MCD_STR xmlstring)
 : ItemType(xmlstring), name_sub_1_defined_(false), name_sub_2_defined_(false),
   name_future_sub_1_defined_(false), name_future_sub_2_defined_(false),
   shortname_("UNDEF"), shortname_defined_(false), shortname_future_defined_(false),
   shortname_sub_1_defined_(false), shortname_sub_2_defined_(false),
   shortname_future_sub_1_defined_(false), shortname_future_sub_2_defined_(false),
   can_take_hostages_(false), threatening_(false), can_threaten_hostages_(true),
   protects_against_kidnapping_(true),
   musical_attack_(false), instrument_(false), legality_(2), bashstrengthmod_(1),
   suspicious_(true), size_(15), can_graffiti_(false), auto_break_lock_(false)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();

   while(xml.FindElem()) //Loop over all the elements inside the weapontype element.
   {
      std::string element = xml.GetTagName();

      if (element == "shortname")
      {
         shortname_ = xml.GetData();
         shortname_defined_ = true;
      }
      else if (element == "shortname_future")
      {
         shortname_future_ = xml.GetData();
         shortname_future_defined_ = true;
      }
      else if (element == "name_sub_1")
      {
         name_sub_1_ = xml.GetData();
         name_sub_1_defined_ = true;
      }
      else if (element == "name_sub_2")
      {
         name_sub_2_ = xml.GetData();
         name_sub_2_defined_ = true;
      }
      else if (element == "name_future_sub_1")
      {
         name_future_sub_1_ = xml.GetData();
         name_future_sub_1_defined_ = true;
      }
      else if (element == "name_future_sub_2")
      {
         name_future_sub_2_ = xml.GetData();
         name_future_sub_2_defined_ = true;
      }
      else if (element == "shortname_sub_1")
      {
         shortname_sub_1_ = xml.GetData();
         shortname_sub_1_defined_ = true;
      }
      else if (element == "shortname_sub_2")
      {
         shortname_sub_2_ = xml.GetData();
         shortname_sub_2_defined_ = true;
      }
      else if (element == "shortname_future_sub_1")
      {
         shortname_future_sub_1_ = xml.GetData();
         shortname_future_sub_1_defined_ = true;
      }
      else if (element == "shortname_future_sub_2")
      {
         shortname_future_sub_2_ = xml.GetData();
         shortname_future_sub_2_defined_ = true;
      }
      else if (element == "can_take_hostages")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_take_hostages_ = true;
         else if (b == 0)
            can_take_hostages_ = false;
         /*else
            errorlog << "Invalid boolean value for weapon type " << idname()
                      << "::can_take_hostages: " << xml.GetData() << endl;*/
      }
      else if (element == "threatening")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            threatening_ = true;
         else if (b == 0)
            threatening_ = false;
         /*else
            errorlog << "Invalid boolean value for weapon type " << idname()
                      << "::threatening: " << xml.GetData() << endl;*/
      }
      else if (element == "can_threaten_hostages")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_threaten_hostages_ = true;
         else if (b == 0)
            can_threaten_hostages_ = false;
      }
      else if (element == "protects_against_kidnapping")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            protects_against_kidnapping_ = true;
         else if (b == 0)
            protects_against_kidnapping_ = false;
      }
      else if (element == "musical_attack")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            musical_attack_ = true;
         else if (b == 0)
            musical_attack_ = false;
         /*else
            errorlog << "Invalid boolean value for weapon type " << idname()
                      << "::musical_attack: " << xml.GetData() << endl;*/
      }
      else if (element == "instrument")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            instrument_ = true;
         else if (b == 0)
            instrument_ = false;
         /*else
            errorlog << "Invalid boolean value for weapon type " << idname()
                      << "::instrument: " << xml.GetData() << endl;*/
      }
      else if (element == "graffiti")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_graffiti_ = true;
         else if (b == 0)
            can_graffiti_ = false;
         /*else
            errorlog << "Invalid boolean value for weapon type " << idname()
                      << "::graffiti: " << xml.GetData() << endl;*/
      }
      else if (element == "legality")
         legality_ = atoi(xml.GetData());
      else if (element == "bashstrengthmod")
         bashstrengthmod_ = atoi(xml.GetData()) / 100.0;
      else if (element == "auto_break_locks")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            auto_break_lock_ = true;
         else if (b == 0)
            auto_break_lock_ = false;
         /*else
            errorlog << "Invalid boolean value for weapon type " << idname()
                      << "::auto_break_locks: " << xml.GetData() << endl;*/
      }
      else if (element == "suspicious")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            suspicious_ = true;
         else if (b == 0)
            suspicious_ = false;
         /*else
            errorlog << "Invalid boolean value for weapon type " << idname()
                      << "::suspicious: " << xml.GetData() << endl;*/
      }
      else if (element == "size")
         size_ = atoi(xml.GetData());
      else if (element == "attack")
      {
         attackst* attack = new attackst(xml.GetSubDoc());

         int i;
         for(i=0;i<len(attacks_)&&attack->priority>=attacks_[i]->priority;i++);
         attacks_.insert(attacks_.begin()+i,attack);
      }
      /*else
         errorlog << "Unknown element for weapon type " << idname()
                   << ": " << element << endl;*/
   }

   if (!shortname_defined_)
   {
      if ((uses_ammo() && len(name()) <= 9)
          || len(name()) <= 14)
         shortname_ = name();
   }
   else
   {
      if (len(shortname_) > 9 && uses_ammo())
         shortname_.resize(9);
      else if (len(shortname_) > 14)
         shortname_.resize(14);
   }

}

attackst::attackst(MCD_STR xmlstring)
 : priority(1), ranged(false), thrown(false), ammotype("UNDEF"), uses_ammo(false),
   attack_description("assaults"), hit_description("striking"),
   always_describe_hit(false), can_backstab(false), hit_punctuation("."),
   skill(SKILL_CLUB), accuracy_bonus(0), number_attacks(1),
   successive_attacks_difficulty(0), strength_min(5), strength_max(10),
	random_damage(1), fixed_damage(1), bruises(false), tears(false), cuts(false),
	burns(false), shoots(false), bleeding(false), severtype(0), damages_armor(false),
	armorpiercing(0), no_damage_reduction_for_limbs_chance(0)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();

   while(xml.FindElem()) //Loop over all the elements inside the vehicletype element.
   {
      std::string element = xml.GetTagName();

      if (element == "priority")
         priority = atoi(xml.GetData());
      else if (element == "ranged")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            ranged = true;
         else if (b == 0)
            ranged = false;
         /*else
            errorlog << "Invalid boolean value for attack::ranged " << xml.GetData() << endl;*/
      }
      else if (element == "thrown")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            thrown = true;
         else if (b == 0)
            thrown = false;
         /*else
            errorlog << "Invalid boolean value for attack::thrown " << xml.GetData() << endl;*/
      }
      else if (element == "can_backstab")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_backstab = true;
         else if (b == 0)
            can_backstab = false;
      }
      else if (element == "ammotype")
      {
         ammotype = xml.GetData();
         uses_ammo = true;
      }
      else if (element == "attack_description")
         attack_description = xml.GetData();
      else if (element == "hit_description")
         hit_description = xml.GetData();
      else if (element == "always_describe_hit")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            always_describe_hit = true;
         else if (b == 0)
            always_describe_hit = false;
         /*else
            errorlog << "Invalid boolean value for attack::always_describe_hit: " << xml.GetData() << endl;*/
      }
      else if (element == "hit_punctuation")
         hit_punctuation = xml.GetData();
      else if (element == "skill")
      {
         int s = skill_string_to_enum(xml.GetData());
         if (s != -1)
            skill = s;
         /*else
            errorlog << "Invalid skill name for attack::skill: " << xml.GetData() << endl; */
      }
      else if (element == "accuracy_bonus")
         accuracy_bonus = atoi(xml.GetData());
      else if (element == "number_attacks")
         number_attacks = atoi(xml.GetData());
      else if (element == "successive_attacks_difficulty")
         successive_attacks_difficulty = atoi(xml.GetData());
      else if (element == "strength_min")
         strength_min = atoi(xml.GetData());
      else if (element == "strength_max")
         strength_max = atoi(xml.GetData());
      else if (element == "random_damage")
         random_damage = atoi(xml.GetData());
      else if (element == "fixed_damage")
         fixed_damage = atoi(xml.GetData());
      else if (element == "bruises")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            bruises = true;
         else if (b == 0)
            bruises = false;
         /*else
            errorlog << "Invalid boolean value for attack::bruises: " << xml.GetData() << endl;*/
      }
      else if (element == "tears")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            tears = true;
         else if (b == 0)
            tears = false;
         /*else
            errorlog << "Invalid boolean value for attack::tears: " << xml.GetData() << endl;*/
      }
      else if (element == "cuts")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            cuts = true;
         else if (b == 0)
            cuts = false;
         /*else
            errorlog << "Invalid boolean value for attack::cuts: " << xml.GetData() << endl;*/
      }
      else if (element == "burns")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            burns = true;
         else if (b == 0)
            burns = false;
         /*else
            errorlog << "Invalid boolean value for attack::burns: " << xml.GetData() << endl;*/
      }
      else if (element == "shoots")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            shoots = true;
         else if (b == 0)
            shoots = false;
         /*else
            errorlog << "Invalid boolean value for attack::shoots: " << xml.GetData() << endl;*/
      }
      else if (element == "bleeding")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            bleeding = true;
         else if (b == 0)
            bleeding = false;
         /*else
            errorlog << "Invalid boolean value for attack::bleeding: " << xml.GetData() << endl;*/
      }
      else if (element == "severtype")
      {
         int s = severtype_string_to_enum(xml.GetData());
         if (s != -1)
            severtype = s;
         /*else
            errorlog << "Invalid severtype for attack::severtype: " << xml.GetData() << endl; */
      }
      else if (element == "damages_armor")
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            damages_armor = true;
         else if (b == 0)
            damages_armor = false;
         /*else
            errorlog << "Invalid boolean value for attack::damages_armor: " << xml.GetData() << endl;*/
      }
      else if (element == "armorpiercing")
         armorpiercing = atoi(xml.GetData());
      else if (element == "no_damage_reduction_for_limbs_chance")
         no_damage_reduction_for_limbs_chance = atoi(xml.GetData());
      else if (element == "critical")
      {
         xml.IntoElem();

         while(xml.FindElem())
         {
            element = xml.GetTagName();

            if (element == "chance")
               critical.chance = atoi(xml.GetData());
            else if (element == "hits_required")
               critical.hits_required = atoi(xml.GetData());
            else if (element == "random_damage")
            {
               critical.random_damage = atoi(xml.GetData());
               critical.random_damage_defined = true;
            }
            else if (element == "fixed_damage")
            {
               critical.fixed_damage = atoi(xml.GetData());
               critical.fixed_damage_defined = true;
            }
            else if (element == "severtype")
            {
               int s = severtype_string_to_enum(xml.GetData());
               if (s != -1)
               {
                  critical.severtype = s;
                  critical.severtype_defined = true;
               }
               /*else
                  errorlog << "Invalid severtype for attack::critical::severtype: " << xml.GetData() << endl; */
            }
            /*else
               errorlog << "Unknown element for attack::critical: " << element << endl; */
         }

         xml.OutOfElem();
      }
      else if (element == "fire")
      {
         xml.IntoElem();

         while(xml.FindElem())
         {
            element = xml.GetTagName();

            if (element == "chance")
               fire.chance = atoi(xml.GetData());
            else if (element == "chance_causes_debris")
               fire.chance_causes_debris = atoi(xml.GetData());
            /*else
               errorlog << "Unknown element for attack::fire: " << element << endl; */
         }

         xml.OutOfElem();
      }
      /*else
         errorlog << "Unknown element for attack: " << element << endl; */
   }

   if (!bruises && !tears && !cuts && !burns && !shoots)
      bruises = true; //If no type specified, then bruise.
}

attackst::criticalst::criticalst()
 : chance(0), hits_required(1), random_damage(1), random_damage_defined(false),
   fixed_damage(1), fixed_damage_defined(false), severtype(0), severtype_defined(false)
{

}

attackst::firest::firest()
 : chance(0), chance_causes_debris(0)
{

}

WeaponType::~WeaponType()
{
   delete_and_clear(attacks_);
}

const string& WeaponType::get_name(unsigned subtype) const
{
   if (subtype == 0)
      return get_name();
   else if (subtype == 1)
   {
      if (name_future_sub_1_defined_ && year >= 2100)
         return name_future_sub_1_;
      else if (name_sub_1_defined_)
         return name_sub_1_;
      else
         return get_name();
   }
   else if (subtype == 2)
   {
      if (name_future_sub_2_defined_ && year >= 2100)
         return name_future_sub_2_;
      else if (name_sub_2_defined_)
         return name_sub_2_;
      else
         return get_name();
   }
   else
      return get_name(); //return "INVALID SUBTYPE"; //Reference to temporary. -XML
}

const string& WeaponType::get_shortname(unsigned subtype) const
{
   if (subtype > 2)
      return shortname_; //return "INVALID SUBTYPE"; //Reference to temporary. -XML
   else if (subtype == 1)
   {
      if (shortname_future_sub_1_defined_ && year >= 2100)
         return shortname_future_sub_1_;
      else if (year >= 2100 && name_future_sub_1_defined_ && len(name_future_sub_1_) <= 14) //Too long for ammo using weapons. -XML
         return name_future_sub_1_;
      else if (shortname_sub_1_defined_)
         return shortname_sub_1_;
      else if (name_sub_1_defined_ && len(name_sub_1_) <= 14) //Too long for ammo using weapons. -XML
         return name_sub_1_;
   }
   else if (subtype == 2)
   {
      if (shortname_future_sub_2_defined_ && year >= 2100)
         return shortname_future_sub_2_;
      else if (year >= 2100 && name_future_sub_2_defined_ && len(name_future_sub_2_) <= 14) //Too long for ammo using weapons. -XML
         return name_future_sub_2_;
      else if (shortname_sub_2_defined_)
         return shortname_sub_2_;
      else if (name_sub_2_defined_ && len(name_sub_2_) <= 14) //Too long for ammo using weapons. -XML
         return name_sub_2_;
   }

   if (shortname_future_defined_ && year >= 2100)
      return shortname_future_;
   else if (year >= 2100 && name_future_defined() && len(name_future()) <= 14) //Too long for ammo using weapons. -XML
      return name_future();
   else //if (shortname_defined_)
      return shortname_;
   /*else if (len(name()) <= 14) //Too long for ammo using weapons. -XML
      return name();
   else
      return "UNDEF";*/
}

bool WeaponType::uses_ammo() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->uses_ammo)
        return true;
   return false;
}

bool WeaponType::acceptable_ammo(const string& clipname) const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->ammotype==clipname)
        return true;
   return false;
}

bool WeaponType::is_ranged() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->ranged)
        return true;
   return false;
}

bool WeaponType::is_throwable() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->thrown)
        return true;
   return false;
}

bool WeaponType::is_legal() const
{
   return legality_ >= to_index(law[LAW_GUNCONTROL])-2;
}
