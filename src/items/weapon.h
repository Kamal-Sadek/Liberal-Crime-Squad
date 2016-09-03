#ifndef WEAPON_H
#define WEAPON_H

//#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"

class Weapon : public Item
{
   public:
      explicit Weapon(const WeaponType& seed, int number = 1);
      virtual Weapon* clone() const { return new Weapon(*this); }
      explicit Weapon(const std::string& inputXml);
      string showXml() const;

      virtual string equip_title() const;

      virtual Weapon* split(int number);
      virtual bool merge(Item& i);
      virtual bool sort_compare_special(Item* other) const;

      const string& get_name() const;
      const string& get_name(unsigned subtype) const;
      const string& get_shortname(unsigned subtype = 0) const;
      long get_fencevalue() const;
      bool can_take_hostages() const;
      bool is_threatening() const;
      bool can_threaten_hostages() const;
      bool protects_against_kidnapping() const;
      bool has_musical_attack() const;
      bool is_instrument() const;
      int get_legality() const;
      float get_bashstrengthmod() const;
      bool is_suspicious() const;
      int get_size() const;
      bool can_graffiti() const;
      bool uses_ammo() const;
      bool acceptable_ammo(const Item& c) const;
      bool acceptable_ammo(const Clip& c) const;
      bool acceptable_ammo(const ClipType& c) const;
      bool is_ranged() const;
      bool is_throwable() const;
      bool auto_breaks_locks() const;
      bool is_legal() const;

      // Tries to reload the weapon with clip. If the weapon is reloaded, clip's
      // number is reduced by one.
      bool reload(Clip& clip);
      int get_ammoamount() const { return ammo_; }
      //void set_ammo(int b) { ammo_ = b; }
      void decrease_ammo(int d) { ammo_ -= d; }
      const string& get_loaded_cliptypename() const { return loaded_cliptype_; }
      const attackst* get_attack(bool force_ranged, bool force_melee, bool force_no_reload) const;

      virtual bool is_weapon() const { return true; }


   private:
      string loaded_cliptype_;

      int ammo_;
};


#endif //WEAPON_H
