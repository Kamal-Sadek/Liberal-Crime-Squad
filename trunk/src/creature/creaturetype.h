#ifndef CREATURE_TYPE_H
#define CREATURE_TYPE_H

#include "includes.h"

class CreatureType
{
   public:
      // Creates a creature type from xml.
      explicit CreatureType(const std::string& xmlstring);

      friend void makecreature(Creature &cr, short type);
      // Remakes the creature into the creature type. Depends on being called
      // from makecreature currently.
      void make_creature(Creature& cr) const;

      const std::string& get_idname() const { return idname_; }
      long get_id() const { return id_; }
      short get_type() const { return type_; }

      int get_alignment() const;
      int roll_gender() const;
      float roll_infiltration() const;
      std::string get_encounter_name() const;
      std::string get_type_name() const;

      // Gives the creature one of the creature type's possible weapons,
      // with clips if so defined.
      void give_weapon(Creature& cr) const;
      // May give a 9mm, .45 or .38 depending on gun control law and chance.
      void give_weapon_civilian(Creature& cr) const;
      // Gives the creature one of the creature type's possible armors.
      void give_armor(Creature& cr) const;

      static int number_of_creaturetypes;
   private:
      std::string idname_;
      long id_;
      short type_; // This is a CreatureTypes enum value.

      struct WeaponsAndClips
      {
         WeaponsAndClips(std::string weapon, int weapons, std::string clip, int clips);
         WeaponsAndClips(CMarkup& xml, const string& owner);
         std::string weapontype;
         Interval number_weapons;
         std::string cliptype;
         Interval number_clips;
      };
      std::vector<WeaponsAndClips> weapons_and_clips_;
      std::vector<std::string> armortypes_;
      std::string type_name_;
      std::string encounter_name_;
      Interval age_;
      bool alignment_public_mood_;
      Alignment alignment_;
      Interval attribute_points_;
      Interval attributes_[ATTNUM];
      int gender_liberal_;      // Only gender_liberal_ is used now determining
      int gender_conservative_; // starting gender.
      Interval infiltration_;
      Interval juice_;
      Interval money_;
      Interval skills_[SKILLNUM];
};

// This declaration is necessary for the inline function Creature::get_type_name() below it to work.
// It isn't part of the CreatureType class or creaturetype.cpp file, it's implemented elsewhere.
const CreatureType* getcreaturetype(const std::string& crtype);

// This would normally be inlined inside the Creature class in creature.h, but the Creature class
// has to be declared prior to the CreatureType class so we have to inline it here instead.
inline std::string Creature::get_type_name() const { return getcreaturetype(type_idname)->get_type_name(); }

#endif //CREATURE_TYPE_H
