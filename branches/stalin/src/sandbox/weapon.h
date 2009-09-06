
#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

enum Weapons
{
   WEAPON_NONE,
   WEAPON_BASEBALLBAT,
   WEAPON_CROWBAR,
   WEAPON_KNIFE,
   WEAPON_SHANK,
   WEAPON_SYRINGE,
   WEAPON_REVOLVER_22,
   WEAPON_REVOLVER_44,
   WEAPON_SEMIPISTOL_9MM,
   WEAPON_SEMIPISTOL_45,
   WEAPON_AUTORIFLE_M16,
   WEAPON_AUTORIFLE_AK47,
   WEAPON_SEMIRIFLE_AR15,
   WEAPON_CARBINE_M4,
   WEAPON_SMG_MP5,
   WEAPON_SHOTGUN_PUMP,
   WEAPON_SWORD,
   WEAPON_CHAIN,
   WEAPON_NIGHTSTICK,
   WEAPON_GAVEL,
   WEAPON_DAISHO,
   WEAPON_HAMMER,
   WEAPON_MAUL,
   WEAPON_CROSS,
   WEAPON_STAFF,
   WEAPON_PITCHFORK,
   WEAPON_TORCH,
   WEAPON_GUITAR,
   WEAPON_SPRAYCAN,
   WEAPONNUM
};

enum ClipType
{
   CLIP_9,
   CLIP_45,
   CLIP_ASSAULT,
   CLIP_SMG,
   CLIP_22,
   CLIP_44,
   CLIP_BUCKSHOT,
   CLIPNUM
};

class Weapon
{
   short type;
   short ammo;

   bool ranged(void)
   {
      switch(type)
      {
         case WEAPON_REVOLVER_22:
         case WEAPON_REVOLVER_44:
         case WEAPON_SEMIPISTOL_9MM:
         case WEAPON_SEMIPISTOL_45:
         case WEAPON_AUTORIFLE_M16:
         case WEAPON_AUTORIFLE_AK47:
         case WEAPON_SEMIRIFLE_AR15:
         case WEAPON_CARBINE_M4:
         case WEAPON_SMG_MP5:
         case WEAPON_SHOTGUN_PUMP:
            return 1;
      }
      return 0;
   }
};

#endif //WEAPON_H_INCLUDED