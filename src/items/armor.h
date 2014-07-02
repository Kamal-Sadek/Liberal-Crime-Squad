#ifndef ARMOR_H
#define ARMOR_H

#include "includes.h"

class Armor : public Item
{
   public:
      explicit Armor(const ArmorType& seed, int quality = 1, int number = 1);
      virtual Armor* clone() const { return new Armor(*this); }
      explicit Armor(const std::string& inputXml);
      string showXml() const;

      virtual bool is_armor() const { return true; }

      virtual string equip_title() const;
      string equip_title(bool full) const;

      virtual Armor* split(int number);
      virtual bool merge(Item& i);
      virtual bool sort_compare_special(Item* other) const;

      int get_quality() const { return quality_; }
      //void decrease_quality(int decrease = 1);

      void set_damaged(bool d);
      bool is_damaged() const { return damaged_; }
      void set_bloody(bool b);
      bool is_bloody() const { return bloody_; }
      void clean_and_repair() { damaged_ = false; bloody_ = false; }

      bool is_good_for_sale() const { return !bloody_ && !damaged_; }

      const string& get_name() const;
      long get_fencevalue() const;
      int get_make_difficulty() const;
      int get_make_price() const;
      bool deathsquad_legality() const;

      bool can_get_bloody() const;
      bool can_get_damaged() const;

      int get_armor(int bodypart) const;
      bool has_fireprotection() const;
      bool covers(int bodypart) const;
      bool conceals_face() const;
      int get_stealth_value() const;
      const string& get_shortname() const;

      int get_interrogation_basepower() const;
      int get_interrogation_assaultbonus() const;
      int get_interrogation_drugbonus() const;

      int get_professionalism() const;

      int get_weaponsize_concealment() const;
      bool conceals_weapon(const Weapon& weapon) const;
      bool conceals_weapon(const WeaponType& weapon) const;
      bool conceals_weaponsize(int weaponsize) const;
      /*const vector<string>& get_appropriate_weapons() const
         { return armortype[getarmortype(itemtypename())]->get_appropriate_weapons(); }
      const string& get_appropriate_weapon(int index) const
         { return armortype[getarmortype(itemtypename())]->get_appropriate_weapon(index); }*/

      //For masks:
      bool is_mask() const;
      bool is_surprise_mask() const; //Should perhaps be decided by the shop and not the mask. -XML
      const string& get_description() const;


   private:

      bool bloody_;
      bool damaged_;

      int quality_;
};

#endif //ARMOR_H
