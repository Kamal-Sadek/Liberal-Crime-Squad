#include "externs.h"

Weapon::Weapon(const WeaponType& seed, int number)
 : Item(seed, number), ammo_(0)
{
   
}

Weapon::Weapon(const char * inputXml)
 : Item(inputXml)
{
   CMarkup xml;
   xml.SetDoc (inputXml);
   xml.FindElem ();
   xml.IntoElem ();
   
   while (xml.FindElem ())
   {
      std::string tag = xml.GetTagName ();
      
      if (tag == "loaded_cliptype")
         loaded_cliptype_ = xml.GetData();
      else if (tag == "ammo")
         ammo_ = atoi(xml.GetData().c_str());
   }
}

string Weapon::showXml () const
{
   CMarkup xml;
   xml.AddElem("weapon");
   xml.IntoElem();

   addBaseValues(xml);
   
   xml.AddElem("loaded_cliptype", loaded_cliptype_);
   
   xml.AddElem("ammo", tostring(ammo_));

   return xml.GetDoc();
}

string Weapon::equip_title() const
{
   string et = weapontype[getweapontype(itemtypename())]->get_name();
   if (ammo_ > 0)
      et += " (" + tostring(ammo_) + ")";
   return et;
}

bool Weapon::reload(Clip& clip)
{
   if (acceptable_ammo(clip) && !clip.empty())
   {
      loaded_cliptype_ = clip.get_itemtypename();
      ammo_ = clip.get_ammoamount();
      clip.decrease_number(1);
      return true;
   }
   else
      return false;
}

/*void Weapon::take_from(Item& i, int amount)
{
   if
}*/

Weapon* Weapon::split(int number)
{
   if (number > number_)
      number = number_;
   
   Weapon* newi = this->clone();
   newi->number_ = number;
   this->number_ -= number;
   
   return newi;
}

bool Weapon::merge(Item& i)
{
   if (i.is_weapon() && i.get_itemtypename() == this->itemtypename())
   {
      Weapon& w = static_cast<Weapon&>(i); //cast -XML
      if ((this->loaded_cliptype_ == w.loaded_cliptype_ && this->ammo_ == w.ammo_)
          || (this->ammo_ == 0 && w.ammo_ == 0))
      {
         number_ += w.number_;
         w.number_ = 0;
         return true;
      }
   }
   return false;   
}

bool Weapon::sort_compare_special(Item* other) const
{
   bool reorder = false;
   if (other != NULL)
   {
      int thisi = getweapontype(itemtypename());
      int otheri = getweapontype(other->get_itemtypename());
      if (thisi < otheri || otheri == -1)
         reorder = false;
      else if (thisi > otheri && otheri != -1)
         reorder = true;
      else if (other->is_weapon())
      {
         Weapon* w = static_cast<Weapon*>(other); //cast -XML
         reorder = (this->ammo_ < w->ammo_);
      }
   }
   return reorder;
}


const attackst* Weapon::get_attack(bool force_ranged, bool force_melee, bool force_no_reload) const
{
   const vector<attackst*>& attacks = weapontype[getweapontype(itemtypename())]->get_attacks();
   for (unsigned i = 0; i < attacks.size(); ++i)
   {
      if (force_ranged && !attacks[i]->ranged)
         continue;
      
      if (force_melee && attacks[i]->ranged)
         continue;
      
      if (force_no_reload && attacks[i]->uses_ammo && ammo_ == 0)
         continue;
      
      if (attacks[i]->uses_ammo && attacks[i]->ammotype != loaded_cliptype_ && ammo_ != 0)
         continue;

      
      return attacks[i];
   }
   return NULL;
}

bool Weapon::acceptable_ammo(const Item& c) const
{
   if (c.is_clip())
      return weapontype[getweapontype(itemtypename())]->acceptable_ammo(c.get_itemtypename());
   else
      return false;
}

const string& Weapon::get_name() const
   { return weapontype[getweapontype(itemtypename())]->get_name(); }

const string& Weapon::get_name(unsigned subtype) const
   { return weapontype[getweapontype(itemtypename())]->get_name(subtype); }
   
const string& Weapon::get_shortname(unsigned subtype) const
   { return weapontype[getweapontype(itemtypename())]->get_shortname(subtype); }

long Weapon::get_fencevalue() const
   { return weapontype[getweapontype(itemtypename())]->get_fencevalue(); }
   
bool Weapon::can_take_hostages() const
   { return weapontype[getweapontype(itemtypename())]->can_take_hostages(); }
   
bool Weapon::is_threatening() const
   { return weapontype[getweapontype(itemtypename())]->is_threatening(); }
   
bool Weapon::has_musical_attack() const
   { return weapontype[getweapontype(itemtypename())]->has_musical_attack(); }
   
bool Weapon::is_instrument() const
   { return weapontype[getweapontype(itemtypename())]->is_instrument(); }
   
int Weapon::get_legality() const
   { return weapontype[getweapontype(itemtypename())]->get_legality(); }
   
float Weapon::get_bashstrengthmod() const
   { return weapontype[getweapontype(itemtypename())]->get_bashstrengthmod(); }
   
bool Weapon::is_suspicious() const
   { return weapontype[getweapontype(itemtypename())]->is_suspicious(); }
   
int Weapon::get_size() const
   { return weapontype[getweapontype(itemtypename())]->get_size(); }
   
bool Weapon::can_graffiti() const
   { return weapontype[getweapontype(itemtypename())]->can_graffiti(); }
   
bool Weapon::uses_ammo() const
   { return weapontype[getweapontype(itemtypename())]->uses_ammo(); }
   
bool Weapon::acceptable_ammo(const Clip& c) const
   { return weapontype[getweapontype(itemtypename())]->acceptable_ammo(c.get_itemtypename()); }
   
bool Weapon::acceptable_ammo(const ClipType& c) const
   { return weapontype[getweapontype(itemtypename())]->acceptable_ammo(c); }
   
bool Weapon::is_ranged() const
   { return weapontype[getweapontype(itemtypename())]->is_ranged(); }
   
bool Weapon::is_throwable() const
   { return weapontype[getweapontype(itemtypename())]->is_throwable(); }
   
bool Weapon::auto_breaks_locks() const
   { return weapontype[getweapontype(itemtypename())]->auto_breaks_locks(); }

bool Weapon::is_legal() const
   { return weapontype[getweapontype(itemtypename())]->is_legal(); }

