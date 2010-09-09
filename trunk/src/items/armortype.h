#ifndef ARMORTYPE_H
#define ARMORTYPE_H

class ArmorType : public ItemType
{
   public:
      explicit ArmorType(MCD_STR xmlstring);
      ArmorType(const ArmorType& base, MCD_STR xmlstring);
      
      bool is_armor() const { return true; }
      
      int get_make_difficulty() const { return make_difficulty_; }
      int get_make_price() const { return make_price_; }
      // Returns if making the armor should only be possible when death penalty
      // and police behavior laws are arch-conservative, ie when death squads
      // roam.
      bool deathsquad_legality() const { return deathsquad_legality_; }

      // Returns armor (protection) provided by armor type on specified body
      // part. Use the bodyparts enum values for the parameter.
      int get_armor(int bodypart) const;
      bool has_fireprotection() const { return fireprotection_; }
      // Returns if the armor type covers the specified body part.
      // Use the bodyparts enum values for the parameter.
      bool covers(int bodypart) const;
      // Returns if the armor type conceals the wearer's face. (Not actually used anywhere.)
      bool conceals_face() const { return conceal_face_; }

      const string& get_shortname() const;

      int get_interrogation_basepower() const { return interrogation_basepower_; }
      int get_interrogation_assaultbonus() const { return interrogation_assaultbonus_; }
      int get_interrogation_drugbonus() const { return interrogation_drugbonus_; }

      // Returns value for professional appearance. Used when soliciting donations.
      int get_professionalism() const { return professionalism_; }

      // Returns the largest weapon size concealed by the armor type.
      int get_weaponsize_concealment() const { return conceal_weaponsize_; }
      // Checks if the armor type conceals the given weapon.
      bool conceals_weapon(const WeaponType& weapon) const { return conceals_weaponsize(weapon.get_size()); }
      bool conceals_weaponsize(int weaponsize) const;
      /*const vector<string>& get_appropriate_weapons() const { return appropriate_weapon_; }
      const string& get_appropriate_weapon(int index) const;*/
      
      
      //For masks:
      bool is_mask() const { return mask_; }
      bool is_surprise_mask() const { return surprise_mask_; }
      const string& get_description() const { return description_; }
      
      
   
   private:
      
      void init(const MCD_STR& xmlstring);
      
      int make_difficulty_;
      int make_price_;
      bool deathsquad_legality_;

      int armor_body_;
      int armor_head_;
      int armor_limbs_;
      bool fireprotection_;
      bool cover_head_;
      bool cover_body_;
      bool cover_arms_;
      bool cover_legs_;
      bool conceal_face_;

      string shortname_;
      string shortname_future_;
      bool shortname_defined_;
      bool shortname_future_defined_;
      

      int interrogation_basepower_;
      int interrogation_assaultbonus_;
      int interrogation_drugbonus_;

      int professionalism_;

      int conceal_weaponsize_;
      //vector<string> appropriate_weapon_;
      
      //For masks:
      bool mask_;
      bool surprise_mask_;
      string description_;
};

#endif //ARMORTYPE_H
