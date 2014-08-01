#include <externs.h>

Armor::Armor(const ArmorType& seed, int quality, int number)
 : Item(seed,number), bloody_(false), damaged_(false), quality_(quality)
{ }

Armor::Armor(const std::string& inputXml) : Item(inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem())
   {
      std::string tag=xml.GetTagName();
      if(tag=="bloody")
         bloody_=xml.GetData()=="true";
      else if(tag == "damaged")
         damaged_=xml.GetData()=="true";
      else if(tag=="quality")
         quality_=atoi(xml.GetData());
   }
}

string Armor::showXml() const
{
   CMarkup xml;
   xml.AddElem("armor");
   xml.IntoElem();

   addBaseValues(xml);

   if (bloody_)
      xml.AddElem("bloody", "true");
   else
      xml.AddElem("bloody", "false");

   if (damaged_)
      xml.AddElem("damaged", "true");
   else
      xml.AddElem("damaged", "false");

   xml.AddElem("quality", tostring(quality_));

   return xml.GetDoc();
}

string Armor::equip_title() const
{
   return equip_title(false);
}

string Armor::equip_title(bool full) const
{
   string et;
   if (full)
      et = get_name();
   else
      et = get_shortname();

   if (quality_ <= get_quality_levels())
   if (bloody_ || damaged_ || quality_ > 1)
   {
      et += "[";
      if (quality_ > 9)
         et += "X";
      else if (quality_ > 1)
         et += tostring(quality_);
      if (bloody_)
         et += "B";
      if (damaged_)
         et += "D";
      et += "]";
   }

   return et;
}

Armor* Armor::split(int number)
{
   if (number > number_)
      number = number_;

   Armor* newi = this->clone();
   newi->number_ = number;
   this->number_ -= number;

   return newi;
}

bool Armor::merge(Item& i)
{
   if(i.is_armor() && this->is_same_type(i))
   {
      Armor& a = static_cast<Armor&>(i); //cast -XML
      if (this->bloody_ == a.bloody_ && this->damaged_ == a.damaged_
          && this->quality_ == a.quality_)
      {
         this->increase_number(a.get_number());
         a.set_number(0);
         return true;
      }
   }
   return false;
}

bool Armor::decrease_quality(int decrease)
{
   quality_+=decrease;
   if (quality_ < 1)
      quality_ = 1;
   if (quality_ > armortype[getarmortype(itemtypename())]->get_quality_levels())
   {
      return false;
   }
   return true;
}

bool Armor::sort_compare_special(Item* other) const
{
   bool reorder = false;
   if (other != NULL)
   {
      int thisi = getarmortype(itemtypename());
      int otheri = getarmortype(other->get_itemtypename());
      if (thisi < otheri || otheri == -1)
         reorder = false;
      else if (thisi > otheri && otheri != -1)
         reorder = true;
      else if (other->is_armor())
      {
         Armor* a = static_cast<Armor*>(other); //cast... -XML

         if(this->quality_ < a->quality_)
            reorder = false;
         else if(this->quality_ > a->quality_)
            reorder = true;
         else if(this->damaged_ != a->damaged_) //i.e. if damaged is different
            reorder = this->damaged_;
         else if(this->bloody_ != a->bloody_ ) //i.e. if bloody is different
            reorder = this->bloody_;
         else
            reorder = false;
      }
   }
   return reorder;
}

void Armor::set_damaged(bool d)
{
   if ((d && can_get_damaged()) || !d)
      damaged_ = d;
}

void Armor::set_bloody(bool b)
{
   if ((b && can_get_bloody()) || !b)
      bloody_ = b;
}

const string ruinedName = "Tattered Rags";
const string& Armor::get_name() const
   {
      if (this->quality_ <= this->get_quality_levels())
      return armortype[getarmortype(itemtypename())]->get_name();
      return ruinedName;
   }

const string& Armor::get_shortname() const
   {
      if (this->quality_ <= this->get_quality_levels())
      return armortype[getarmortype(itemtypename())]->get_shortname();
      return ruinedName;
   } 

long Armor::get_fencevalue() const
   { 
      if (this->quality_ <= this->get_quality_levels())
         return armortype[getarmortype(itemtypename())]->get_fencevalue() / this->quality_;
      return 0;
   }

int Armor::get_make_difficulty() const
   { return armortype[getarmortype(itemtypename())]->get_make_difficulty(); }

int Armor::get_make_price() const
   { return armortype[getarmortype(itemtypename())]->get_make_price(); }

bool Armor::deathsquad_legality() const
   { return armortype[getarmortype(itemtypename())]->deathsquad_legality(); }

bool Armor::can_get_bloody() const
   { return armortype[getarmortype(itemtypename())]->can_get_bloody(); }

bool Armor::can_get_damaged() const
   { return armortype[getarmortype(itemtypename())]->can_get_damaged(); }

int Armor::get_armor(int bodypart) const
   { return armortype[getarmortype(itemtypename())]->get_armor(bodypart); }

bool Armor::has_fireprotection() const
   { return armortype[getarmortype(itemtypename())]->has_fireprotection(); }

bool Armor::covers(int bodypart) const
   { return armortype[getarmortype(itemtypename())]->covers(bodypart); }

bool Armor::conceals_face() const
   { return armortype[getarmortype(itemtypename())]->conceals_face(); }

int Armor::get_interrogation_basepower() const
   { return armortype[getarmortype(itemtypename())]->get_interrogation_basepower(); }

int Armor::get_interrogation_assaultbonus() const
   { return armortype[getarmortype(itemtypename())]->get_interrogation_assaultbonus(); }

int Armor::get_interrogation_drugbonus() const
   { return armortype[getarmortype(itemtypename())]->get_interrogation_drugbonus(); }

int Armor::get_professionalism() const
   { return armortype[getarmortype(itemtypename())]->get_professionalism(); }

int Armor::get_stealth_value() const
   { return armortype[getarmortype(itemtypename())]->get_stealth_value(); }

int Armor::get_weaponsize_concealment() const
   { return armortype[getarmortype(itemtypename())]->get_weaponsize_concealment(); }

bool Armor::conceals_weapon(const Weapon& weapon) const
   { return armortype[getarmortype(itemtypename())]->conceals_weaponsize(weapon.get_size()); }

bool Armor::conceals_weapon(const WeaponType& weapon) const
   { return armortype[getarmortype(itemtypename())]->conceals_weapon(weapon); }

bool Armor::conceals_weaponsize(int weaponsize) const
   { return armortype[getarmortype(itemtypename())]->conceals_weaponsize(weaponsize); }

//For masks:
bool Armor::is_mask() const
{
   return armortype[getarmortype(itemtypename())]->is_mask();
}

bool Armor::is_surprise_mask() const
{
   return armortype[getarmortype(itemtypename())]->is_surprise_mask();
}

const string& Armor::get_description() const
{
   return armortype[getarmortype(itemtypename())]->get_description();
}


int Armor::get_durability() const
{
   return armortype[getarmortype(itemtypename())]->get_durability();
}

int Armor::get_quality_levels() const
{
   return armortype[getarmortype(itemtypename())]->get_quality_levels();
}
