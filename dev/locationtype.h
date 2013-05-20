#ifndef LOCATION_TYPE_H
#define LOCATION_TYPE_H

#include "includes.h"

struct LootEntry
{
   string type;
   int weight;
};

class LocationType
{
public:
   string id() const { return id_; }
   string city() const { return city_; }
   bool is_city() const { return id == city; }

   bool use_random_name_as_prefix() const { return use_random_name_as_prefix_; }
   string longname() const { return longname_; }
   string shortname() const { return shortname_; }

   bool default_safehouse() const { return default_safehouse_; }
   int rent() const { return rent_; }
   int base_heat_protection() const { return base_heat_protection_; }
   bool capturable_safehouse() const { return capturable_safehouse_; }

   bool can_cause_trouble() const { return can_cause_trouble_; }
   vector<LootEntry> loot_list() const { return loot_list_; }
   int random_loot() const;
   int news_notoriety() const { return news_notoriety_; }

   bool prison_min_sec() const { return prison_min_sec_; }
   bool prison_mid_sec() const { return prison_mid_sec_; }
   bool prison_max_sec() const { return prison_max_sec_; }
   bool prison() const { return (prison_min_sec() || prison_mid_sec() || prison_max_sec()); }

   bool police_station() const { return police_station_; }
   bool courthouse() const { return courthouse_; }
   bool justice_system() const { return (courthouse() || police_station() || prison()); }

   bool hospital() const { return (healing_power_ > 0); }
   bool clinic() const { return clinic_; }
   int healing_power() const { return healing_power_; }

   bool is_shop() const { return (shop_.length() || car_dealership_); }
   string shop() const { return shop_; }
   bool car_dealership() const { return car_dealership_; }

   bool ccs_safehouse() const { return ccs_safehouse_; }
   bool supports_high_security() const { return supports_high_security_; }
   bool stay_on_high_security() const { return stay_on_high_security_; }
   int door_lock_quality() const { return door_lock_quality_; }

   string pursuit_cartype() const { return pursuit_cartype_; }
   int extra_pursuers() const { return extra_pursuers_; }
   string pursuit_creaturetype() const { return pursuit_creaturetype_; }
private:
   string id_;
   string city_;

   bool use_random_name_as_prefix_;
   string longname_;
   string shortname_;

   bool default_safehouse_;
   int rent_;
   int base_heat_protection_;
   bool capturable_safehouse_;

   bool can_cause_trouble_;
   vector<LootEntry> loot_list_;
   int news_notoriety_;

   bool prison_min_sec_;
   bool prison_mid_sec_;
   bool prison_max_sec_;
   bool police_station_;
   bool courthouse_;

   bool clinic_;
   int healing_power_;

   string shop_;
   bool car_dealership_;

   bool ccs_safehouse_;
   bool supports_high_security_;
   bool stay_on_high_security_;
   int door_lock_quality_;

   string pursuit_cartype_;
   int extra_pursuers_;
   string pursuit_creaturetype_;
};

#endif //LOCATION_TYPE_H
