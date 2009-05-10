/*

Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
                                                                                      //
This file is part of Liberal Crime Squad.                                             //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/

/*
	This file was created by Chris Johnson (grundee@users.sourceforge.net)
	by copying code from game.cpp.
	To see descriptions of files and functions, see the list at 
	the bottom of includes.h in the top src folder.
*/

//#include <includes.h>
#include <externs.h>

void getactivity(char *str,activityst &act)
{
   strcpy(str,"");

   switch(act.type)
   {
      case ACTIVITY_HOSTAGETENDING:
      {
         strcat(str,"Tending to ");
         int pl=getpoolcreature(act.arg);
         if(pl!=-1)
         {
            strcat(str,pool[pl]->name);
         }
         break;
      }
      case ACTIVITY_REPAIR_ARMOR:
         strcat(str,"Repairing Clothing");
         break;
      case ACTIVITY_WHEELCHAIR:
         strcat(str,"Procuring a Wheelchair");
         break;
      case ACTIVITY_STEALCARS:
         strcat(str,"Stealing a Car");
         break;
      case ACTIVITY_POLLS:
         strcat(str,"Gathering Opinion Info");
         break;
      case ACTIVITY_MAKE_ARMOR:
      {
         strcat(str,"Making ");
         char str2[80];
         getarmor(str2,act.arg);
         strcat(str,str2);
         break;
      }
      case ACTIVITY_TROUBLE:
         strcat(str,"Causing Trouble");
         break;
      case ACTIVITY_PROSTITUTION:
         strcat(str,"Prostituting");
         break;
      case ACTIVITY_COMMUNITYSERVICE:
         strcat(str,"Volunteering");
         break;
      case ACTIVITY_GRAFFITI:
         strcat(str,"Making Graffiti");
         break;
      case ACTIVITY_CCFRAUD:
         strcat(str,"Credit Card Fraud");
         break;
      case ACTIVITY_DOS_RACKET:
         strcat(str,"Extorting Websites");
         break;
      case ACTIVITY_DOS_ATTACKS:
         strcat(str,"Attacking Websites");
         break;
      case ACTIVITY_HACKING:
         strcat(str,"Hacking Networks");
         break;
      case ACTIVITY_SELL_TSHIRTS:
         strcat(str,"Selling T-Shirts");
         break;
      case ACTIVITY_SELL_ART:
         strcat(str,"Selling Art");
         break;
      case ACTIVITY_TEACH_GENERALED:
         strcat(str,"Teaching General ED");
         break;
      case ACTIVITY_TEACH_POLITICS:
         strcat(str,"Teaching Politics");
         break;
      case ACTIVITY_TEACH_SURVIVAL:
         strcat(str,"Teaching Survival");
         break;
      case ACTIVITY_TEACH_FIGHTING:
         strcat(str,"Teaching Fighting");
         break;
      case ACTIVITY_TEACH_COVERT:
         strcat(str,"Teaching Covert Ops");
         break;
      case ACTIVITY_SELL_MUSIC:
         strcat(str,"Selling Music");
         break;
      case ACTIVITY_BURY:
         strcat(str,"Disposing of Bodies");
         break;
      case ACTIVITY_DONATIONS:
         strcat(str,"Soliciting Donations");
         break;
      case ACTIVITY_SELL_DRUGS:
         strcat(str,"Selling Brownies");
         break;
      case ACTIVITY_VISIT:
      {
         strcat(str,"Going to ");
         strcat(str,location[act.arg]->shortname);
         break;
      }
      case ACTIVITY_HEAL:
         strcat(str,"Tending to Injuries");
         break;
      case ACTIVITY_NONE:
         strcat(str,"Laying Low");
         break;
      case ACTIVITY_WRITE_LETTERS:
      case ACTIVITY_WRITE_GUARDIAN:
         strcat(str,"Writing");
         break;
      case ACTIVITY_CLINIC:
         strcat(str,"Going to Free CLINIC");
         break;
      case ACTIVITY_SLEEPER_LIBERAL:
         strcat(str,"Promoting Liberalism");
         break;
      case ACTIVITY_SLEEPER_CONSERVATIVE:
         strcat(str,"Spouting Conservatism");
         break;
      case ACTIVITY_SLEEPER_SPY:
         strcat(str,"Snooping Around");
         break;
      case ACTIVITY_SLEEPER_RECRUIT:
         strcat(str,"Recruiting Sleepers");
         break;
      case ACTIVITY_SLEEPER_JOINLCS:
         strcat(str,"Quitting Job");
         break;
      case ACTIVITY_SLEEPER_SCANDAL:
         strcat(str,"Creating a Scandal");
         break;
      case ACTIVITY_SLEEPER_EMBEZZLE:
         strcat(str,"Embezzling Funds");
         break;
      case ACTIVITY_SLEEPER_STEAL:
         strcat(str,"Stealing Equipment");
         break;
   }
}



void getweapon(char *str,int type)
{
   strcpy(str,"");

   if(year<2100)
   {
      switch(type)
      {
         //LIMIT ----------------------------------------:----: (-5 for guns)
         case WEAPON_NONE:         strcpy(str,"None");break;
         case WEAPON_CROWBAR:      strcpy(str,"Crowbar");break;
         case WEAPON_BASEBALLBAT:   strcpy(str,"Baseball Bat");break;
         case WEAPON_KNIFE:         strcpy(str,"Knife");break;
         case WEAPON_SHANK:         strcpy(str,"Shank");break;
         case WEAPON_SYRINGE:      strcpy(str,"Syringe");break;
         case WEAPON_REVOLVER_38:   strcpy(str,".38 Rev.");break;
         case WEAPON_REVOLVER_44:   strcpy(str,".44 Mag.");break;
         case WEAPON_SEMIPISTOL_9MM:   strcpy(str,"9mm Semi.");break;
         case WEAPON_SEMIPISTOL_45:   strcpy(str,".45 Semi.");break;
         case WEAPON_SEMIRIFLE_AR15:   strcpy(str,"AR-15");break;
         case WEAPON_SMG_MP5:       strcpy(str,"MP5");break;
         case WEAPON_CARBINE_M4:    strcpy(str,"M4");break;
         case WEAPON_AUTORIFLE_M16:   strcpy(str,"M16");break;
         case WEAPON_AUTORIFLE_AK47:   strcpy(str,"AK-47");break;
         case WEAPON_SHOTGUN_PUMP:      strcpy(str,"Shotgun");break;
         case WEAPON_SWORD:         strcpy(str,"Sword");break;
         case WEAPON_DAISHO:         strcpy(str,"Daisho");break;
         case WEAPON_HAMMER:         strcpy(str,"Hammer");break;
         case WEAPON_MAUL:         strcpy(str,"Maul");break;
         case WEAPON_CROSS:         strcpy(str,"Cross");break;
         case WEAPON_STAFF:         strcpy(str,"Staff");break;
         case WEAPON_CHAIN:         strcpy(str,"Chain");break;
         case WEAPON_NIGHTSTICK:    strcpy(str,"Nightstick");break;
         case WEAPON_GAVEL:         strcpy(str,"Gavel");break;
         case WEAPON_PITCHFORK:     strcpy(str,"Pitchfork");break;
         case WEAPON_TORCH:         strcpy(str,"Torch");break;
         case WEAPON_GUITAR:        strcpy(str,"Guitar");break;
         case WEAPON_SPRAYCAN:      strcpy(str,"Spraypaint");break;
         case WEAPON_MOLOTOV:       strcpy(str,"Molotov");break;
         case WEAPON_AXE:           strcpy(str,"Axe");break;
         case WEAPON_FLAMETHROWER:  strcpy(str,"M2A1-7");break;
      }
   }
   else
   {
      switch(type)
      {
         //LIMIT ----------------------------------------:----: (-5 for guns)
         case WEAPON_NONE:         strcpy(str,"None");break;
         case WEAPON_CROWBAR:      strcpy(str,"Prybar");break;
         case WEAPON_BASEBALLBAT:   strcpy(str,"Spaceball Bat");break;
         case WEAPON_KNIFE:         strcpy(str,"Vibro-Knife");break;
         case WEAPON_SHANK:         strcpy(str,"Shank");break;
         case WEAPON_SYRINGE:       strcpy(str,"Injector");break;
         case WEAPON_REVOLVER_38:   strcpy(str,".38 Slug");break;
         case WEAPON_REVOLVER_44:   strcpy(str,".44 Slug");break;
         case WEAPON_SEMIPISTOL_9MM:strcpy(str,"9mm Las.");break;
         case WEAPON_SEMIPISTOL_45: strcpy(str,".45 Las.");break;
         case WEAPON_SEMIRIFLE_AR15:strcpy(str,"X15 Semi");break;
         case WEAPON_SMG_MP5:       strcpy(str,"Prince SMG");break;
         case WEAPON_CARBINE_M4:    strcpy(str,"J4 Laser");break;
         case WEAPON_AUTORIFLE_M16: strcpy(str,"X15 Full");break;
         case WEAPON_AUTORIFLE_AK47:strcpy(str,"P74 War");break;
         case WEAPON_SHOTGUN_PUMP:  strcpy(str,"Burstgun");break;
         case WEAPON_SWORD:         strcpy(str,"Light Sword");break;
         case WEAPON_DAISHO:        strcpy(str,"Twin Sword");break;
         case WEAPON_HAMMER:        strcpy(str,"Hammer");break;
         case WEAPON_MAUL:          strcpy(str,"Maul");break;
         case WEAPON_CROSS:         strcpy(str,"Cross");break;
         case WEAPON_STAFF:         strcpy(str,"Staff");break;
         case WEAPON_CHAIN:         strcpy(str,"CyberArm");break;
         case WEAPON_NIGHTSTICK:    strcpy(str,"Shock Stick");break;
         case WEAPON_GAVEL:         strcpy(str,"Gavel");break;
         case WEAPON_PITCHFORK:     strcpy(str,"Pitchfork");break;
         case WEAPON_TORCH:         strcpy(str,"Flare");break;
         case WEAPON_GUITAR:        strcpy(str,"Keytar");break;
         case WEAPON_SPRAYCAN:      strcpy(str,"Holopaint");break;
         case WEAPON_MOLOTOV:       strcpy(str,"Molotov");break;
         case WEAPON_AXE:           strcpy(str,"Laser Axe");break;
         case WEAPON_FLAMETHROWER:  strcpy(str,"Plasmagun");break;
      }
   }
}

void getweaponfull(char *str,int type,int subtype)
{
   strcpy(str,"");

   if(year<2100)
   {
      switch(type)
      {
         //LIMIT -----------------------------------------------:----: (-5 for guns)
         case WEAPON_NONE:          strcpy(str,"None");break;
         case WEAPON_CROWBAR:       strcpy(str,"Crowbar");break;
         case WEAPON_BASEBALLBAT:   strcpy(str,"Baseball Bat");break;
         case WEAPON_KNIFE:         strcpy(str,"Knife");break;
         case WEAPON_SHANK:         strcpy(str,"Shank");break;
         case WEAPON_SYRINGE:       strcpy(str,"Syringe");break;
         case WEAPON_REVOLVER_38:   strcpy(str,".38 Revolver");break;
         case WEAPON_REVOLVER_44:   strcpy(str,".44 Magnum");break;
         case WEAPON_SEMIPISTOL_9MM:strcpy(str,"9mm Semiauto");break;
         case WEAPON_SEMIPISTOL_45: strcpy(str,".45 Semiauto");break;
         case WEAPON_SEMIRIFLE_AR15:strcpy(str,"AR15");break;
         case WEAPON_SMG_MP5:       strcpy(str,"MP5 SMG");break;
         case WEAPON_CARBINE_M4:    strcpy(str,"M4 Carbine");break;
         case WEAPON_AUTORIFLE_M16: strcpy(str,"M16");break;
         case WEAPON_AUTORIFLE_AK47:strcpy(str,"AK47");break;
         case WEAPON_SHOTGUN_PUMP:  strcpy(str,"Shotgun");break;
         case WEAPON_SWORD:         strcpy(str,"Sword of Morfiegor");break;
         case WEAPON_DAISHO:
            switch(subtype)
            {
            case 0: strcpy(str,"Daisho");break;
            case 1: strcpy(str,"Katana");break;
            case 2: strcpy(str,"Wakizashi");break;
            }
            break;
         case WEAPON_HAMMER:         strcpy(str,"Dwarven Hammer");break;
         case WEAPON_MAUL:          strcpy(str,"Maul of Anrin");break;
         case WEAPON_CROSS:         strcpy(str,"Silver Cross");break;
         case WEAPON_STAFF:         strcpy(str,"Wizard's Staff");break;
         case WEAPON_CHAIN:         strcpy(str,"Chain");break;
         case WEAPON_NIGHTSTICK:    strcpy(str,"Nightstick");break;
         case WEAPON_GAVEL:         strcpy(str,"Gavel");break;
         case WEAPON_PITCHFORK:     strcpy(str,"Pitchfork");break;
         case WEAPON_TORCH:         strcpy(str,"Torch");break;
         case WEAPON_GUITAR:        strcpy(str,"Guitar");break;
         case WEAPON_SPRAYCAN:      strcpy(str,"Spraypaint");break;
         case WEAPON_MOLOTOV:       strcpy(str,"Molotov Bomb");break;
         case WEAPON_AXE:           strcpy(str,"Axe");break;
         case WEAPON_FLAMETHROWER:  strcpy(str,"M2 Flamethrower");break;
      }
   }
   else
   {
      switch(type)
      {
      case WEAPON_NONE:          strcpy(str,"None");break;
      case WEAPON_BASEBALLBAT:   strcpy(str,"Spaceball Bat");break;
      case WEAPON_KNIFE:         strcpy(str,"Vibro-Knife");break;
      case WEAPON_SHANK:         strcpy(str,"Cyber Shank");break;
      case WEAPON_SYRINGE:      strcpy(str,"Medical Injector");break;
      case WEAPON_REVOLVER_38:   strcpy(str,".38 Slug Pistol");break;
      case WEAPON_REVOLVER_44:   strcpy(str,".44 Heavy Slug Pistol");break;
      case WEAPON_SEMIPISTOL_9MM:   strcpy(str,"9mm Laser Pistol");break;
      case WEAPON_SEMIPISTOL_45:   strcpy(str,".45 Laser Pistol");break;
      case WEAPON_SEMIRIFLE_AR15:   strcpy(str,"X15 Personal Laser");break;
      case WEAPON_SMG_MP5:       strcpy(str,"Beijing Prince SMG");break;
      case WEAPON_CARBINE_M4:    strcpy(str,"J4 Laser Carbine");break;
      case WEAPON_AUTORIFLE_M16:   strcpy(str,"X15 Military Laser");break;
      case WEAPON_AUTORIFLE_AK47:   strcpy(str,"P74 War Laser");break;
      case WEAPON_SHOTGUN_PUMP:      strcpy(str,"Plasma Shotgun");break;
      case WEAPON_SWORD:         strcpy(str,"Light Sword");break;
      case WEAPON_DAISHO:
         switch(subtype)
         {
         case 0: strcpy(str,"Twin Swords of Liberalism");break;
         case 1: strcpy(str,"Retribution Sword");break;
         case 2: strcpy(str,"Justice Blade");break;
         }
         break;
      case WEAPON_HAMMER:        strcpy(str,"Dwarven Hammer");break;
      case WEAPON_MAUL:          strcpy(str,"Maul of Anrin");break;
      case WEAPON_CROSS:         strcpy(str,"Silver Cross");break;
      case WEAPON_STAFF:         strcpy(str,"Wizard's Staff");break;
      case WEAPON_CHAIN:         strcpy(str,"Industrial Cyber-Arm");break;
      case WEAPON_NIGHTSTICK:    strcpy(str,"Electro-Shock Stick");break;
      case WEAPON_GAVEL:         strcpy(str,"Laser Gavel");break;
      case WEAPON_PITCHFORK:     strcpy(str,"Space Pitchfork");break;
      case WEAPON_CROWBAR:       strcpy(str,"Forcefield Prybar");break;
      case WEAPON_TORCH:         strcpy(str,"Burning Flare");break;
      case WEAPON_GUITAR:        strcpy(str,"Cyber-Keytar");break;
      case WEAPON_SPRAYCAN:      strcpy(str,"Holo-Paint");break;
      case WEAPON_MOLOTOV:       strcpy(str,"Molotov Bomb");break;
      case WEAPON_AXE:           strcpy(str,"Laser Axe");break;
      case WEAPON_FLAMETHROWER:  strcpy(str,"Plasma Gun");break;
      }
   }
}


void getarmor(char *str,int type,int subtype)
{
   switch(type)
   {
      //LIMIT ---------------------------------------------:
      case ARMOR_NONE:         strcpy(str,"Naked");break;
      case ARMOR_CLOTHES:         strcpy(str,"Clothes");break;
      case ARMOR_OVERALLS:      strcpy(str,"Overalls");break;
      case ARMOR_WIFEBEATER:      strcpy(str,"Wife Beater");break;
      case ARMOR_TRENCHCOAT:      strcpy(str,"Trenchcoat");break;
      case ARMOR_WORKCLOTHES:      strcpy(str,"Work Clothes");break;
      case ARMOR_SECURITYUNIFORM:   strcpy(str,"Security Unif.");break;
      case ARMOR_POLICEUNIFORM:   strcpy(str,"Police Uniform");break;
      case ARMOR_CHEAPSUIT:      strcpy(str,"Cheap Suit");break;
      case ARMOR_EXPENSIVESUIT:   strcpy(str,"Expensive Suit");break;
      case ARMOR_BLACKSUIT:      strcpy(str,"Black Suit");break;
      case ARMOR_CHEAPDRESS:      strcpy(str,"Cheap Dress");break;
      case ARMOR_EXPENSIVEDRESS:   strcpy(str,"Expensve Dress");break;
      case ARMOR_BLACKDRESS:      strcpy(str,"Black Dress");break;
      case ARMOR_LABCOAT:         strcpy(str,"Lab Coat");break;
      case ARMOR_BLACKROBE:      strcpy(str,"Black Robe");break;
      case ARMOR_CLOWNSUIT:      strcpy(str,"Clown Suit");break;
      case ARMOR_ELEPHANTSUIT:      strcpy(str,"Elephant Suit");break;
      case ARMOR_DONKEYSUIT:      strcpy(str,"Donkey Suit");break;
      case ARMOR_BONDAGEGEAR:      strcpy(str,"Bondage Gear");break;
      case ARMOR_MASK:
         switch(subtype)
         {
      case MASK_ASHCROFT:         strcpy(str,"Ashcroft Mask");break;
      case MASK_BARR:            strcpy(str,"Bob Barr Mask");break;
      case MASK_BLAIR:         strcpy(str,"Blair Mask");break;
      case MASK_BUSH_BARBARA:      strcpy(str,"Brb. Bush Mask");break;
      case MASK_BUSH_GEORGE:      strcpy(str,"G HW Bush Mask");break;
      case MASK_BUSH_GEORGE2:      strcpy(str,"G W. Bush Mask");break;
      //LIMIT ---------------------------------------------:
      case MASK_CARTER:         strcpy(str,"Carter Mask");break;
      case MASK_CHENEY:         strcpy(str,"Cheney Mask");break;
      case MASK_CLINTON_BILL:      strcpy(str,"B Clinton Mask");break;
      case MASK_CLINTON_HILLARY:   strcpy(str,"H Clinton Mask");break;
      case MASK_FALWELL:         strcpy(str,"Falwell Mask");break;
      case MASK_FORD:            strcpy(str,"Gld. Ford Mask");break;
      case MASK_GORE_AL:         strcpy(str,"Al Gore Mask");break;
      case MASK_GORE_TIPPER:      strcpy(str,"Tpr. Gore Mask");break;
      case MASK_GUY_FAWKES:      strcpy(str,"G. Fawkes Mask");break;
      case MASK_TED_STEVENS:      strcpy(str,"T Stevens Mask");break;
      case MASK_HELMS:         strcpy(str,"Helms Mask");break;
      case MASK_JASON:         strcpy(str,"Jason Mask");break;
      case MASK_LINCOLN:         strcpy(str,"Lincoln Mask");break;
      case MASK_KENNEDY_JOHN:      strcpy(str,"JFK Mask");break;
      case MASK_KENNEDY_ROBERT:   strcpy(str,"RFK Mask");break;
      case MASK_KISSINGER:      strcpy(str,"Kissinger Mask");break;
      case MASK_NIXON:         strcpy(str,"Nixon Mask");break;
      case MASK_POWELL:         strcpy(str,"Powell Mask");break;
      case MASK_REAGAN_NANCY:      strcpy(str,"N. Reagan Mask");break;
      case MASK_REAGAN_RONALD:   strcpy(str,"R. Reagan Mask");break;
      case MASK_RICE:            strcpy(str,"Cn. Rice Mask");break;
      case MASK_ROBERTSON:      strcpy(str,"Robertson Mask");break;
      case MASK_RUMSFELD:         strcpy(str,"Rumsfeld Mask");break;
      case MASK_SATAN:         strcpy(str,"Satan Mask");break;
      case MASK_STARR:         strcpy(str,"Ken Starr Mask");break;
      case MASK_THURMOND:         strcpy(str,"Thurmond Mask");break;
      case MASK_WASHINGTON:      strcpy(str,"Washngton Mask");break;
      case MASK_WAXMAN:         strcpy(str,"Waxman Mask");break;
      //LIMIT ---------------------------------------------:
      case MASK_JESUS:         strcpy(str,"Jesus Mask");break;
      case MASK_COLEMAN_GARY:      strcpy(str,"G Coleman Mask");break;
      case MASK_MADONNA:         strcpy(str,"Madonna Mask");break;
      case MASK_SPEARS:         strcpy(str,"B Spears Mask");break;
      case MASK_EMINEM:         strcpy(str,"Eminem Mask");break;
      case MASK_AGUILERA:         strcpy(str,"C Aguilra Mask");break;
      case MASK_WAHLBERG:         strcpy(str,"M Wahlbrg Mask");break;
      case MASK_IGGYPOP:         strcpy(str,"Iggy Pop Mask");break;
      case MASK_CASH:            strcpy(str,"Jny. Cash Mask");break;
      case MASK_BINLADEN:         strcpy(str,"bin Laden Mask");break;
      case MASK_LORDS:         strcpy(str,"Tr. Lords Mask");break;
      case MASK_SHIELDS:         strcpy(str,"Br. Shlds Mask");break;
      case MASK_JACKSON_MICHAEL:   strcpy(str,"M Jackson Mask");break;
      case MASK_CRUTHERS:         strcpy(str,"Cruthers Mask");break;
      case MASK_KING_DON:         strcpy(str,"Don King Mask");break;
      //LIMIT ---------------------------------------------:
      default:
                           strcpy(str,"Mask");
         }
         break;
      case ARMOR_MILITARY:      strcpy(str,"Army Uniform");break;
      case ARMOR_PRISONGUARD:      strcpy(str,"Guard Uniform");break;
      case ARMOR_PRISONER:      strcpy(str,"Orange Jmpsuit");break;
      case ARMOR_TOGA:         strcpy(str,"Toga");break;
      case ARMOR_MITHRIL:         strcpy(str,"Mithril Mail");break;
      case ARMOR_CIVILLIANARMOR:       strcpy(str,"Body Armor");break;
      case ARMOR_SWATARMOR:            strcpy(str,"SWAT Armor");break;
      case ARMOR_POLICEARMOR:          strcpy(str,"PoliceBdyArmor");break;
      case ARMOR_ARMYARMOR:            strcpy(str,"Army BodyArmor");break;
      case ARMOR_HEAVYARMOR:           strcpy(str,"HeavyBodyArmor");break;
      case ARMOR_BUNKERGEAR:           strcpy(str,"Bunker Gear");break;
      case ARMOR_DEATHSQUADUNIFORM:    strcpy(str,"DSquad Uniform");break;
   }
}


void getarmorfull(char *str,int type,int subtype)
{
   switch(type)
   {
      //LIMIT --------------------------------------------------:
      case ARMOR_NONE:         strcpy(str,"Naked");break;
      case ARMOR_CLOTHES:         strcpy(str,"Clothes");break;
      case ARMOR_OVERALLS:      strcpy(str,"Overalls");break;
      case ARMOR_WIFEBEATER:      strcpy(str,"Wife Beater");break;
      case ARMOR_TRENCHCOAT:      strcpy(str,"Trenchcoat");break;
      case ARMOR_WORKCLOTHES:      strcpy(str,"Work Clothes");break;
      case ARMOR_SECURITYUNIFORM:   strcpy(str,"Security Uniform");break;
      case ARMOR_POLICEUNIFORM:   strcpy(str,"Police Uniform");break;
      case ARMOR_CHEAPSUIT:      strcpy(str,"Cheap Suit");break;
      case ARMOR_EXPENSIVESUIT:   strcpy(str,"Expensive Suit");break;
      case ARMOR_BLACKSUIT:      strcpy(str,"Black Suit");break;
      case ARMOR_CHEAPDRESS:      strcpy(str,"Cheap Dress");break;
      case ARMOR_EXPENSIVEDRESS:   strcpy(str,"Expensve Dress");break;
      case ARMOR_BLACKDRESS:      strcpy(str,"Black Dress");break;
      case ARMOR_LABCOAT:         strcpy(str,"Lab Coat");break;
      case ARMOR_BLACKROBE:      strcpy(str,"Black Robe");break;
      case ARMOR_CLOWNSUIT:      strcpy(str,"Clown Suit");break;
      case ARMOR_ELEPHANTSUIT:      strcpy(str,"Elephant Suit");break;
      case ARMOR_DONKEYSUIT:      strcpy(str,"Donkey Suit");break;
      case ARMOR_BONDAGEGEAR:      strcpy(str,"Bondage Gear");break;
      case ARMOR_MASK:
         switch(subtype)
         {
      case MASK_ASHCROFT:         strcpy(str,"John Ashcroft Mask");break;
      case MASK_BARR:            strcpy(str,"Bob Barr Mask");break;
      case MASK_BLAIR:         strcpy(str,"Tony Blair Mask");break;
      case MASK_BUSH_BARBARA:      strcpy(str,"Barbara Bush Mask");break;
      case MASK_BUSH_GEORGE:      strcpy(str,"George HW Bush Mask");break;
      case MASK_BUSH_GEORGE2:      strcpy(str,"George W. Bush Mask");break;
      //LIMIT --------------------------------------------------:
      case MASK_CARTER:         strcpy(str,"Jimmy Carter Mask");break;
      case MASK_CHENEY:         strcpy(str,"Dick Cheney Mask");break;
      case MASK_CLINTON_BILL:      strcpy(str,"Bill Clinton Mask");break;
      case MASK_CLINTON_HILLARY:   strcpy(str,"Hillary Clinton Msk");break;
      case MASK_FALWELL:         strcpy(str,"Jerry Falwell Mask");break;
      case MASK_FORD:            strcpy(str,"Gerald Ford Mask");break;
      case MASK_GORE_AL:         strcpy(str,"Al Gore Mask");break;
      case MASK_GORE_TIPPER:      strcpy(str,"Tipper Gore Mask");break;
      case MASK_GUY_FAWKES:      strcpy(str,"Guy Fawkes Mask");break;
      case MASK_TED_STEVENS:      strcpy(str,"Ted Stevens Mask");break;
      case MASK_HELMS:         strcpy(str,"Jesse Helms Mask");break;
      case MASK_JASON:         strcpy(str,"Jason Mask");break;
      case MASK_LINCOLN:         strcpy(str,"Abraham Lincoln Msk");break;
      case MASK_KENNEDY_JOHN:      strcpy(str,"John F Kennedy Mask");break;
      case MASK_KENNEDY_ROBERT:   strcpy(str,"Robert Kennedy Mask");break;
      case MASK_KISSINGER:      strcpy(str,"Henry Kissinger Msk");break;
      case MASK_NIXON:         strcpy(str,"Richard Nixon Mask");break;
      case MASK_POWELL:         strcpy(str,"Colin Powell Mask");break;
      case MASK_REAGAN_NANCY:      strcpy(str,"Nancy Reagan Mask");break;
      case MASK_REAGAN_RONALD:   strcpy(str,"Ronald Reagan Mask");break;
      case MASK_RICE:            strcpy(str,"Condoleezza Rice Mk");break;
      case MASK_ROBERTSON:      strcpy(str,"Pat Robertson Mask");break;
      case MASK_RUMSFELD:         strcpy(str,"Donald Rumsfeld Msk");break;
      case MASK_SATAN:         strcpy(str,"Satan Mask");break;
      case MASK_STARR:         strcpy(str,"Ken Starr Mask");break;
      case MASK_THURMOND:         strcpy(str,"Strom Thurmond Mask");break;
      case MASK_WASHINGTON:      strcpy(str,"George Washngtn Msk");break;
      case MASK_WAXMAN:         strcpy(str,"Henry Waxman Mask");break;
      //LIMIT --------------------------------------------------:
      case MASK_JESUS:         strcpy(str,"Jesus Christ Mask");break;
      case MASK_COLEMAN_GARY:      strcpy(str,"Gary Coleman Mask");break;
      case MASK_MADONNA:         strcpy(str,"Madonna Mask");break;
      case MASK_SPEARS:         strcpy(str,"Britney Spears Mask");break;
      case MASK_EMINEM:         strcpy(str,"Eminem Mask");break;
      case MASK_AGUILERA:         strcpy(str,"Chrstna Aguilera Mk");break;
      case MASK_WAHLBERG:         strcpy(str,"Mark Wahlbrg Mask");break;
      case MASK_IGGYPOP:         strcpy(str,"Iggy Pop Mask");break;
      case MASK_CASH:            strcpy(str,"Johnny Cash Mask");break;
      case MASK_BINLADEN:         strcpy(str,"Osama bin Laden Msk");break;
      case MASK_LORDS:         strcpy(str,"Traci Lords Mask");break;
      case MASK_SHIELDS:         strcpy(str,"Brooke Shields Mask");break;
      case MASK_JACKSON_MICHAEL:   strcpy(str,"Michael Jackson Msk");break;
      case MASK_CRUTHERS:         strcpy(str,"Scatman Cruthers Mk");break;
      case MASK_KING_DON:         strcpy(str,"Don King Mask");break;
      //LIMIT --------------------------------------------------:
      default:
                           strcpy(str,"Mask");
         }
         break;
      case ARMOR_MILITARY:      strcpy(str,"Army Uniform");break;
      case ARMOR_PRISONGUARD:      strcpy(str,"Guard Uniform");break;
      case ARMOR_PRISONER:      strcpy(str,"Orange Jumpsuit");break;
      case ARMOR_TOGA:         strcpy(str,"Toga");break;
      case ARMOR_MITHRIL:         strcpy(str,"Mithril Mail");break;
      case ARMOR_SWATARMOR:      strcpy(str,"Tactical SWAT Armor");break;
      case ARMOR_POLICEARMOR:      strcpy(str,"Police Body Armor");break;
      case ARMOR_ARMYARMOR:    strcpy(str,"Army Body Armor");break;
      case ARMOR_CIVILLIANARMOR:                strcpy(str,"Body Armor");break;
      case ARMOR_HEAVYARMOR:   strcpy(str,"Heavy Body Armor");break;
      case ARMOR_BUNKERGEAR:           strcpy(str,"Bunker Gear");break;
      case ARMOR_DEATHSQUADUNIFORM:    strcpy(str,"Death Squad Uniform");break;
   }
}


void getmaskdesc(char *str,short mask)
{
   switch(mask)
   {
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_ASHCROFT:            strcpy(str,"U.S. Senator, Attorney General");break;
      case MASK_BARR:               strcpy(str,"U.S. Representative");break;
      case MASK_BLAIR:            strcpy(str,"British Prime Minister");break;
      case MASK_BUSH_BARBARA:         strcpy(str,"U.S. First Lady");break;
      case MASK_BUSH_GEORGE:         strcpy(str,"U.S. President");break;
      case MASK_BUSH_GEORGE2:         strcpy(str,"U.S. President");break;
      case MASK_CARTER:            strcpy(str,"U.S. President");break;
      case MASK_CHENEY:            strcpy(str,"U.S. Vice President, CEO");break;
      case MASK_CLINTON_BILL:         strcpy(str,"U.S. President");break;
      case MASK_CLINTON_HILLARY:      strcpy(str,"U.S. Senator, First Lady");break;
      case MASK_FALWELL:            strcpy(str,"Reverend");break;
      case MASK_FORD:               strcpy(str,"U.S. President");break;
      case MASK_GORE_AL:            strcpy(str,"U.S. Vice President");break;
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_GORE_TIPPER:         strcpy(str,"U.S. Second Lady");break;
      case MASK_GUY_FAWKES:      strcpy(str,"Remember the 5th of November");break;
      case MASK_TED_STEVENS:      strcpy(str,"U.S. Senator");break;
      case MASK_HELMS:            strcpy(str,"U.S. Senator");break;
      case MASK_JASON:            strcpy(str,"A Hockey Mask a la Friday the 13th");break;
      case MASK_LINCOLN:            strcpy(str,"U.S. President, Assassinated");break;
      case MASK_KENNEDY_JOHN:         strcpy(str,"U.S. President, Assassinated");break;
      case MASK_KENNEDY_ROBERT:      strcpy(str,"U.S. Senator, Assassinated");break;
      case MASK_KISSINGER:         strcpy(str,"U.S. Secretary of State");break;
      case MASK_NIXON:            strcpy(str,"U.S. President, Resigned");break;
      case MASK_POWELL:            strcpy(str,"U.S. Secretary of State, General");break;
      case MASK_REAGAN_NANCY:         strcpy(str,"U.S. First Lady");break;
      case MASK_REAGAN_RONALD:      strcpy(str,"U.S. President");break;
      case MASK_RICE:               strcpy(str,"U.S. National Security Advisor");break;
      case MASK_ROBERTSON:         strcpy(str,"Televangelist");break;
      case MASK_RUMSFELD:            strcpy(str,"U.S. Secretary of Defense");break;
      case MASK_SATAN:            strcpy(str,"Prince of Darkness, Red with Horns");break;
      case MASK_STARR:            strcpy(str,"Independent Counsel");break;
      case MASK_THURMOND:            strcpy(str,"U.S. Senator");break;
      case MASK_WASHINGTON:         strcpy(str,"U.S. President");break;
      case MASK_WAXMAN:            strcpy(str,"U.S. Representative");break;
      case MASK_JESUS:            strcpy(str,"The Son of God, The Lamb, He died 4 U");break;
      case MASK_COLEMAN_GARY:         strcpy(str,"Actor, Candidate for Governor");break;
      case MASK_MADONNA:            strcpy(str,"Singer, Actor");break;
      case MASK_SPEARS:            strcpy(str,"Singer, Actor");break;
      case MASK_EMINEM:            strcpy(str,"Rapper, Actor");break;
      case MASK_AGUILERA:            strcpy(str,"Singer");break;
      case MASK_WAHLBERG:            strcpy(str,"Actor, Rapper");break;
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_IGGYPOP:            strcpy(str,"Singer, Actor");break;
      case MASK_CASH:               strcpy(str,"Singer");break;
      case MASK_BINLADEN:            strcpy(str,"Terrorist");break;
      case MASK_LORDS:            strcpy(str,"Actor");break;
      case MASK_SHIELDS:            strcpy(str,"Actor");break;
      case MASK_JACKSON_MICHAEL:      strcpy(str,"Singer");break;
      case MASK_CRUTHERS:            strcpy(str,"Singer, Actor");break;
      case MASK_KING_DON:            strcpy(str,"Boxing Promoter");break;
   }
}


void getarmorfull(char *str,armorst &armor,char superfull)
{
   if(superfull)getarmorfull(str,armor.type,armor.subtype);
   else getarmor(str,armor.type,armor.subtype);

   if(armor.flag!=0||armor.quality!='1')
   {
      strcat(str,"[");
      if(armor.quality!='1')
      {
         char c[2];
         c[0]=armor.quality;
         c[1]='\x0';
         strcat(str,c);
      }
      if(armor.flag & ARMORFLAG_BLOODY)
      {
         strcat(str,"B");
      }
      if(armor.flag & ARMORFLAG_DAMAGED)
      {
         strcat(str,"D");
      }
      strcat(str,"]");
   }
}



void getskill(char *str,int type)
{
   strcpy(str,"");

   switch(type)
   {
      case SKILL_HANDTOHAND:strcpy(str,"Martial Arts");break;
      case SKILL_KNIFE:strcpy(str,"Knife");break;
      case SKILL_SWORD:strcpy(str,"Sword");break;
      case SKILL_THROWING:strcpy(str,"Throwing");break;
      case SKILL_CLUB:strcpy(str,"Club");break;
      case SKILL_AXE:strcpy(str,"Axe");break;
      case SKILL_PISTOL:strcpy(str,"Pistol");break;
      case SKILL_RIFLE:strcpy(str,"Rifle");break;
      case SKILL_FLAMETHROWER:strcpy(str,"Flamethrower");break;
      case SKILL_SHOTGUN:strcpy(str,"Shotgun");break;
      case SKILL_SMG:strcpy(str,"SMG");break;
      case SKILL_PERSUASION:strcpy(str,"Persuasion");break;
      case SKILL_PSYCHOLOGY:strcpy(str,"Psychology");break;
      case SKILL_SECURITY:strcpy(str,"Security");break;
      case SKILL_DISGUISE:strcpy(str,"Disguise");break;
      case SKILL_COMPUTERS:strcpy(str,"Computers");break;
      case SKILL_LAW:strcpy(str,"Law");break;
      case SKILL_TAILORING:strcpy(str,"Tailoring");break;
      case SKILL_DRIVING:strcpy(str,"Driving");break;
      case SKILL_WRITING:strcpy(str,"Writing");break;
      case SKILL_COOKING:strcpy(str,"Cooking");break;
      case SKILL_MUSIC:strcpy(str,"Music");break;
      case SKILL_ART:strcpy(str,"Art");break;
      case SKILL_RELIGION:strcpy(str,"Religion");break;
      case SKILL_SCIENCE:strcpy(str,"Science");break;
      case SKILL_BUSINESS:strcpy(str,"Business");break;
      //case SKILL_SURVIVAL:strcpy(str,"Survival");break;
      case SKILL_STEALTH:strcpy(str,"Stealth");break;
      case SKILL_THEFT:strcpy(str,"Theft");break;
      case SKILL_TEACHING:strcpy(str,"Teaching");break;
      case SKILL_STREETSENSE:strcpy(str,"Street Sense");break;
      case SKILL_SEDUCTION:strcpy(str,"Seduction");break;
      case SKILL_LEADERSHIP:strcpy(str,"Leadership");break;
      case SKILL_FIRSTAID:strcpy(str,"First Aid");break;
      case SKILL_TACTICS:strcpy(str,"Dodge");break;
   }
}



void getclip(char *str,int clip)
{
   strcpy(str,"");

   if(year<2100)
   {
      switch(clip)
      {
         case CLIP_9:strcat(str,"9mm Pistol Mag");break;
         case CLIP_45:strcat(str,".45 Pistol Mag");break;
         case CLIP_ASSAULT:strcat(str,"Aslt/Rifle Mag");break;
         case CLIP_SMG:strcat(str,"9mm SMG Mag");break;
         case CLIP_38:strcat(str,".38 Speedloader");break;
         case CLIP_44:strcat(str,".44 Speedloader");break;
         case CLIP_BUCKSHOT:strcat(str,"6 Shotgun Shells");break;
         case CLIP_MOLOTOV:strcat(str,"Molotov Bomb");break;
         case CLIP_GASOLINE:strcat(str,"Gas Tank");break;
      }
   }
   else
   {
      switch(clip)
      {
         case CLIP_9:strcat(str,"9mm Powerpack");break;
         case CLIP_45:strcat(str,".45 Powerpack");break;
         case CLIP_ASSAULT:strcat(str,"Rifle Powerpack");break;
         case CLIP_SMG:strcat(str,"SMG Powerpack");break;
         case CLIP_38:strcat(str,".38 Slug Mag");break;
         case CLIP_44:strcat(str,".44 Heavy Slug Mag");break;
         case CLIP_BUCKSHOT:strcat(str,"Shotgun Plasma Pack");break;
         case CLIP_MOLOTOV:strcat(str,"Molotov Bomb");break;
         case CLIP_GASOLINE:strcat(str,"Hydrogen Fuel");break;
      }
   }
}


void getloot(char *str,int loot)
{
   strcpy(str,"");

   switch(loot)
   {
      case LOOT_LABEQUIPMENT:strcat(str,"Lab Equipment");break;
      case LOOT_COMPUTER:strcat(str,"Computer");break;
      case LOOT_TV:strcat(str,"TV");break;
      case LOOT_SECRETDOCUMENTS:strcat(str,"Secret Documents");break;
      case LOOT_POLICERECORDS:strcat(str,"Police Records");break;
      case LOOT_VCR:strcat(str,"VCR");break;
      case LOOT_CELLPHONE:strcat(str,"Cellphone");break;
      case LOOT_TVCAMERA:strcat(str,"TV Camera");break;
      case LOOT_BROADCASTINGEQUIPMENT:strcat(str,"Broadcasting Equip.");break;
      case LOOT_SILVERWARE:strcat(str,"Silverware");break;
      case LOOT_SCANNER:strcat(str,"Scanner");break;
      case LOOT_PRINTER:strcat(str,"Printer");break;
      case LOOT_FINECLOTH:strcat(str,"Fine Cloth");break;
      case LOOT_CHEMICAL:strcat(str,"Weird Chemical");break;
      case LOOT_CEOPHOTOS:strcat(str,"CEO Photos");break;
      case LOOT_INTHQDISK:strcat(str,"Intel. HQ Data Disk");break;
      case LOOT_CORPFILES:strcat(str,"Secret Corporate Files");break;
      case LOOT_JUDGEFILES:strcat(str,"Judge Corrupt. Evidence");break;
      case LOOT_RESEARCHFILES:strcat(str,"Research Papers");break;
      case LOOT_PRISONFILES:strcat(str,"Prison Records");break;
      case LOOT_CABLENEWSFILES:strcat(str,"Cable News Memos");break;
      case LOOT_AMRADIOFILES:strcat(str,"AM Radio Plans");break;
   }
}



void getrecruitcreature(char *str,int type)
{
   strcpy(str,"");

   switch(type)
   {
      case CREATURE_BOUNCER:strcat(str,"Club Security");break;
      case CREATURE_SECURITYGUARD:strcat(str,"Security Guard");break;
      case CREATURE_SCIENTIST_LABTECH:strcat(str,"Lab Tech");break;
      case CREATURE_SCIENTIST_EMINENT:strcat(str,"Eminent Scientist");break;
      case CREATURE_CORPORATE_MANAGER:strcat(str,"Corporate Manager");break;
      case CREATURE_CORPORATE_CEO:strcat(str,"Corporate CEO");break;
      case CREATURE_WORKER_SERVANT:
         if(law[LAW_LABOR]==-2&&
             law[LAW_CORPORATE]==-2)strcpy(str,"Slave");
         else strcat(str,"Servant");
         break;
      case CREATURE_WORKER_JANITOR:strcat(str,"Janitor");break;
      case CREATURE_WORKER_SWEATSHOP:strcat(str,"Sweatshop Worker");break;
      case CREATURE_WORKER_FACTORY_NONUNION:strcat(str,"Factory Worker");break;
      case CREATURE_WORKER_FACTORY_CHILD:strcat(str,"Child Worker");break;
      case CREATURE_WORKER_SECRETARY:strcat(str,"Secretary");break;
      case CREATURE_WORKER_FACTORY_UNION:strcat(str,"Factory Worker");break;
      case CREATURE_LANDLORD:strcat(str,"Landlord");break;
      case CREATURE_COP:strcat(str,"Police Officer");break;
      case CREATURE_SWAT:strcat(str,"SWAT Officer");break;
      case CREATURE_DEATHSQUAD:strcat(str,"DethSquad Officer");break;
      case CREATURE_GANGUNIT:strcat(str,"Police Gang Unit");break;
      case CREATURE_FIREFIGHTER:strcat(str,"Fire Fighter");break;
      case CREATURE_PRISONGUARD:strcat(str,"Prison Guard");break;
      case CREATURE_EDUCATOR:strcat(str,"Educator");break;
      case CREATURE_MERC:strcat(str,"Mercenary");break;
      case CREATURE_HICK:strcat(str,"Redneck");break;
      case CREATURE_SOLDIER:strcat(str,"Soldier");break;
      case CREATURE_VETERAN:strcat(str,"Veteran");break;
      case CREATURE_HARDENED_VETERAN:strcat(str,"Hardened Veteran");break;
      case CREATURE_JUDGE_LIBERAL:strcat(str,"Judge");break;
      case CREATURE_JUDGE_CONSERVATIVE:strcat(str,"Judge");break;
      case CREATURE_AGENT:strcat(str,"Agent");break;
      case CREATURE_RADIOPERSONALITY:strcat(str,"Radio Personality");break;
      case CREATURE_NEWSANCHOR:strcat(str,"Cable News Anchor");break;
      case CREATURE_LAWYER:strcat(str,"Lawyer");break;
      case CREATURE_SEWERWORKER:strcat(str,"Sewer Worker");break;
      case CREATURE_COLLEGESTUDENT:strcat(str,"College Student");break;
      case CREATURE_MUSICIAN:strcat(str,"Musician");break;
      case CREATURE_MATHEMATICIAN:strcat(str,"Mathematician");break;
      case CREATURE_TEACHER:strcat(str,"Teacher");break;
      case CREATURE_HSDROPOUT:strcat(str,"Highschool Dropout");break;
      case CREATURE_BUM:strcat(str,"Transient");break;
      case CREATURE_MUTANT:strcat(str,"Mutant");break;
      case CREATURE_GANGMEMBER:strcat(str,"Gang Member");break;
      case CREATURE_CRACKHEAD:strcat(str,"Crack Head");break;
      case CREATURE_PRIEST:strcat(str,"Priest");break;
      case CREATURE_ENGINEER:strcat(str,"Engineer");break;
      case CREATURE_FASTFOODWORKER:strcat(str,"Fast Food Worker");break;
      case CREATURE_TELEMARKETER:strcat(str,"Telemarketer");break;
      case CREATURE_OFFICEWORKER:strcat(str,"Office Worker");break;
      case CREATURE_FOOTBALLCOACH:strcat(str,"Football Coach");break;
      case CREATURE_PROSTITUTE:strcat(str,"Prostitute");break;
      case CREATURE_MAILMAN:strcat(str,"Mail Carrier");break;
      case CREATURE_GARBAGEMAN:strcat(str,"Garbage Collector");break;
      case CREATURE_PLUMBER:strcat(str,"Plumber");break;
      case CREATURE_CHEF:strcat(str,"Chef");break;
      case CREATURE_CONSTRUCTIONWORKER:strcat(str,"Construction Worker");break;
      case CREATURE_AMATEURMAGICIAN:strcat(str,"Amateur Magician");break;
      case CREATURE_HIPPIE:strcat(str,"Hippie");break;
      case CREATURE_CRITIC_ART:strcat(str,"Art Critic");break;
      case CREATURE_CRITIC_MUSIC:strcat(str,"Music Critic");break;
      case CREATURE_AUTHOR:strcat(str,"Author");break;
      case CREATURE_JOURNALIST:strcat(str,"Journalist");break;
      case CREATURE_SOCIALITE:strcat(str,"Socialite");break;
      case CREATURE_BIKER:strcat(str,"Biker");break;
      case CREATURE_TRUCKER:strcat(str,"Trucker");break;
      case CREATURE_TAXIDRIVER:strcat(str,"Taxi Driver");break;
      case CREATURE_PROGRAMMER:strcat(str,"Programmer");break;
      case CREATURE_NUN:strcat(str,"Nun");break;
      case CREATURE_RETIREE:strcat(str,"Retiree");break;
      case CREATURE_PAINTER:strcat(str,"Painter");break;
      case CREATURE_SCULPTOR:strcat(str,"Sculptor");break;
      case CREATURE_DANCER:strcat(str,"Dancer");break;
      case CREATURE_PHOTOGRAPHER:strcat(str,"Photographer");break;
      case CREATURE_CAMERAMAN:strcat(str,"Cameraman");break;
      case CREATURE_HAIRSTYLIST:strcat(str,"Hairstylist");break;
      case CREATURE_FASHIONDESIGNER:strcat(str,"Fashion Designer");break;
      case CREATURE_CLERK:strcat(str,"Clerk");break;
      case CREATURE_THIEF:strcat(str,"Professional Thief");break;
      case CREATURE_ACTOR:strcat(str,"Actor");break;
      case CREATURE_YOGAINSTRUCTOR:strcat(str,"Yoga Instructor");break;
      case CREATURE_MARTIALARTIST:strcat(str,"Black Belt");break;
      case CREATURE_ATHLETE:strcat(str,"Athlete");break;
      case CREATURE_TEENAGER:strcat(str,"Teenager");break;
      case CREATURE_PRISONER:strcat(str,"Prisoner");break;
      case CREATURE_DOCTOR:strcat(str,"Doctor");break;
      case CREATURE_NURSE:strcat(str,"Nurse");break;
      case CREATURE_GUARDDOG:strcat(str,"Canine Hero");break;
      case CREATURE_GENETIC:strcat(str,"Genetic Monster");break;
      case CREATURE_TANK:strcat(str,"Armored Tank");break;
      case CREATURE_CCS_MOLOTOV:strcat(str,"Arsonist");break;
      case CREATURE_CCS_SNIPER:strcat(str,"Sniper");break;
      case CREATURE_CCS_VIGILANTE:strcat(str,"Vigilante");break;
      case CREATURE_CCS_ARCHCONSERVATIVE:strcat(str,"CCS Boss");break;
      case CREATURE_POLITICALACTIVIST:strcat(str,"Political Activist");break;
      default:
         strcat(str,"Liberal");
         break;
   }
}



void gettitle(char *str,Creature &cr)
{
   if(cr.align==-1)
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"[Darn] Worthless");
         else strcpy(str,"Damn Worthless");
      }
      else if(cr.juice<=-10)strcpy(str,"Conservative Dregs");
      else if(cr.juice<0)strcpy(str,"Conservative Punk");
      else if(cr.juice<10)strcpy(str,"Mindless Conservative");
      else if(cr.juice<50)strcpy(str,"Wrong-Thinker");
      else if(cr.juice<100)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"Stubborn as [Heck]");
         else strcpy(str,"Stubborn as Hell");
      }
      else if(cr.juice<200)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"Heartless [Jerk]");
         else strcpy(str,"Heartless Bastard");
      }
      else if(cr.juice<500)strcpy(str,"Insane Vigilante");
      else if(cr.juice<1000)strcpy(str,"Arch-Conservative");
      else strcpy(str,"Evil Incarnate");
   }
   else if(cr.align==0)
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"[Darn] Worthless");
         else strcpy(str,"Damn Worthless");
      }
      else if(cr.juice<=-10)strcpy(str,"Society's Dregs");
      else if(cr.juice<0)strcpy(str,"Non-Liberal Punk");
      else if(cr.juice<10)strcpy(str,"Non-Liberal");
      else if(cr.juice<50)strcpy(str,"Hard Working");
      else if(cr.juice<100)strcpy(str,"Respected");
      else if(cr.juice<200)strcpy(str,"Upstanding Citizen");
      else if(cr.juice<500)strcpy(str,"Great Person");
      else if(cr.juice<1000) strcpy(str,"Peacemaker");
      else strcpy(str,"Peace Prize Winner");
   }
   else
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2)strcpy(str,"[Darn] Worthless");
         else strcpy(str,"Damn Worthless");
      }
      else if(cr.juice<=-10)strcpy(str,"Society's Dregs");
      else if(cr.juice<0)strcpy(str,"Punk");
      else if(cr.juice<10)strcpy(str,"Civilian");
      else if(cr.juice<50)strcpy(str,"Activist");
      else if(cr.juice<100)strcpy(str,"Socialist Threat");
      else if(cr.juice<200)strcpy(str,"Revolutionary");
      else if(cr.juice<500)strcpy(str,"Urban Commando");
      else if(cr.juice<1000)strcpy(str,"Liberal Guardian");
      else strcpy(str,"Elite Liberal");
   }
}


void getview(char *str,short view)
{
   strcpy(str,"");

   switch(view)
   {
      case VIEW_GAY:strcat(str,"LGBT Rights");break;
      case VIEW_DEATHPENALTY:strcat(str,"The Death Penalty");break;
      case VIEW_TAXES:strcat(str,"Taxes");break;
      case VIEW_NUCLEARPOWER:strcat(str,"Nuclear Power");break;
      case VIEW_ANIMALRESEARCH:strcat(str,"Animal Cruelty");break;
      case VIEW_POLICEBEHAVIOR:strcat(str,"The Police");break;
      case VIEW_PRISONS:strcat(str,"Prisons");break;
      case VIEW_INTELLIGENCE:strcat(str,"Privacy");break;
      case VIEW_FREESPEECH:strcat(str,"Free Speech");break;
      case VIEW_GENETICS:strcat(str,"Genetics");break;
      case VIEW_JUSTICES:strcat(str,"The Judiciary");break;
      case VIEW_SWEATSHOPS:strcat(str,"Labor");break;
      case VIEW_POLLUTION:strcat(str,"Pollution");break;
      case VIEW_CORPORATECULTURE:strcat(str,"Corporate Culture");break;
      case VIEW_CEOSALARY:strcat(str,"CEO Compensation");break;
      case VIEW_WOMEN:strcat(str,"Gender Equality");break;
      case VIEW_CIVILRIGHTS:strcat(str,"Racial Equality");break;
      case VIEW_DRUGS:strcat(str,"Drugs");break;
      case VIEW_IMMIGRATION:strcat(str,"Immigration");break;
      case VIEW_MILITARY:strcat(str,"The Military");break;
      case VIEW_AMRADIO:strcat(str,"AM Radio");break;
      case VIEW_CABLENEWS:strcat(str,"Cable News");break;
      case VIEW_POLITICALVIOLENCE:strcat(str,"The Need For Action");break;
      case VIEW_LIBERALCRIMESQUAD:strcat(str,"Who We Are");break;
      case VIEW_LIBERALCRIMESQUADPOS:strcat(str,"Why We Rock");break;
      case VIEW_CONSERVATIVECRIMESQUAD:strcat(str,"The CCS Criminals");break;
   }
}



void getviewsmall(char *str,short view)
{
   strcpy(str,"");

   switch(view)
   {
      case VIEW_GAY:strcat(str,"LGBT rights");break;
      case VIEW_DEATHPENALTY:strcat(str,"the death penalty");break;
      case VIEW_TAXES:strcat(str,"taxes");break;
      case VIEW_NUCLEARPOWER:strcat(str,"nuclear power");break;
      case VIEW_ANIMALRESEARCH:strcat(str,"animal cruelty");break;
      case VIEW_POLICEBEHAVIOR:strcat(str,"the cops");break;
      case VIEW_PRISONS:strcat(str,"prison conditions");break;
      case VIEW_INTELLIGENCE:strcat(str,"privacy");break;
      case VIEW_FREESPEECH:strcat(str,"free speech");break;
      case VIEW_GENETICS:strcat(str,"genetic research");break;
      case VIEW_JUSTICES:strcat(str,"judges");break;
      case VIEW_SWEATSHOPS:strcat(str,"labor rights");break;
      case VIEW_POLLUTION:strcat(str,"pollution");break;
      case VIEW_CORPORATECULTURE:strcat(str,"corporations");break;
      case VIEW_CEOSALARY:strcat(str,"CEO compensation");break;
      case VIEW_WOMEN:strcat(str,"gender equality");break;
      case VIEW_CIVILRIGHTS:strcat(str,"racial equality");break;
      case VIEW_DRUGS:strcat(str,"drugs");break;
      case VIEW_IMMIGRATION:strcat(str,"immigration");break;
      case VIEW_MILITARY:strcat(str,"the military");break;
      case VIEW_AMRADIO:strcat(str,"AM radio");break;
      case VIEW_CABLENEWS:strcat(str,"cable news");break;
      case VIEW_POLITICALVIOLENCE:strcat(str,"the need for action");break;
      case VIEW_LIBERALCRIMESQUAD:strcat(str,"the LCS");break;
      case VIEW_LIBERALCRIMESQUADPOS:strcat(str,"the LCS");break;
      case VIEW_CONSERVATIVECRIMESQUAD:strcat(str,"the CCS");break;
   }
}



void getlaw(char *str,int l)
{
   switch(l)
   {
      case LAW_ABORTION:strcpy(str,"Abortion Rights");break;
      case LAW_ANIMALRESEARCH:strcpy(str,"Animal Research");break;
      case LAW_POLICEBEHAVIOR:strcpy(str,"Police Behavior");break;
      case LAW_PRIVACY:strcpy(str,"Privacy");break;
      case LAW_DEATHPENALTY:strcpy(str,"Capital Punishment");break;
      case LAW_NUCLEARPOWER:strcpy(str,"Nuclear Power");break;
      case LAW_POLLUTION:strcpy(str,"Pollution");break;
      case LAW_LABOR:strcpy(str,"Workers' Rights");break;
      case LAW_GAY:strcpy(str,"Homosexual Rights");break;
      case LAW_CORPORATE:strcpy(str,"Corporate Ethics");break;
      case LAW_FREESPEECH:strcpy(str,"Freedom of Speech");break;
		case LAW_TAX:strcpy(str,"Tax Law");break;
      case LAW_FLAGBURNING:strcpy(str,"Flag Burning");break;
      case LAW_GUNCONTROL:strcpy(str,"Gun Control");break;
      case LAW_WOMEN:addstr("Women's Rights");break;
      case LAW_CIVILRIGHTS:addstr("Civil Rights");break;
      case LAW_DRUGS:addstr("Drug Law");break;
      case LAW_IMMIGRATION:addstr("Immigration");break;
      case LAW_RELIGION:addstr("Religion");break;
      case LAW_ELECTIONS:addstr("Election Reform");break;
      case LAW_MILITARY:addstr("Military Spending");break;
      case LAW_TORTURE:addstr("Human Rights");break;
   }
}



void getcarfull(char *str,int type)
{
   strcpy(str,"");

   switch(type)
   {
      case VEHICLE_VAN:strcat(str,"Van");break;
      case VEHICLE_STATIONWAGON:strcat(str,"Station Wagon");break;
      case VEHICLE_SPORTSCAR:strcat(str,"Sportscar");break;
      case VEHICLE_BUG:strcat(str,"Bug");break;
      case VEHICLE_PICKUP:strcat(str,"Pickup Truck");break;
      case VEHICLE_POLICECAR:strcat(str,"Police Cruiser");break;
      case VEHICLE_TAXICAB:strcat(str,"Taxicab");break;
      case VEHICLE_SUV:strcat(str,"SUV");break;
      case VEHICLE_AGENTCAR:strcat(str,"Sportscar");break;
      case VEHICLE_JEEP:strcat(str,"Jeep");break;
   }
}


void getcarfull(char *str,vehiclest &car,char halffull)
{
   char str2[80],num[20];
   if(car.heat)strcpy(str,"Stolen ");
   else strcpy(str,"");
   if(car.color!=naturalcarcolor(car.type))
   {
      getcarcolor(str2,car.color);
      strcat(str,str2);
      strcat(str," ");
   }
   if(car.myear!=-1)
   {
      itoa(car.myear,num,10);
      strcat(str,num);
      strcat(str," ");
   }
   if(halffull)getcar(str2,car.type);
   else getcarfull(str2,car.type);
   strcat(str,str2);
}


void getcar(char *str,int type)
{
   strcpy(str,"");
   switch(type)
   {
      //LIMIT-------------------------------------:-----:
      case VEHICLE_VAN:            strcat(str,"Van");break;
      case VEHICLE_STATIONWAGON:      strcat(str,"Wagon");break;
      case VEHICLE_SPORTSCAR:         strcat(str,"Sport");break;
      case VEHICLE_BUG:            strcat(str,"Bug");break;
      case VEHICLE_PICKUP:         strcat(str,"Pickup");break;
      case VEHICLE_POLICECAR:         strcat(str,"Cruiser");break;
      case VEHICLE_TAXICAB:         strcat(str,"Taxicab");break;
      case VEHICLE_SUV:            strcat(str,"SUV");break;
      case VEHICLE_AGENTCAR:         strcat(str,"Sport");break;
      case VEHICLE_JEEP:            strcat(str,"Jeep");break;
   }
}



void getcarcolor(char *str,int type)
{
   strcpy(str,"");
   switch(type)
   {
      case VEHICLECOLOR_RED:strcat(str,"Red");break;
      case VEHICLECOLOR_WHITE:strcat(str,"White");break;
      case VEHICLECOLOR_BLUE:strcat(str,"Blue");break;
      case VEHICLECOLOR_BEIGE:strcat(str,"Beige");break;
      case VEHICLECOLOR_POLICE:strcat(str,"Police-Marked");break;
      case VEHICLECOLOR_TAXI:strcat(str,"Taxi-Striped");break;
      case VEHICLECOLOR_BLACK:strcat(str,"Black");break;
   }
}

short naturalcarcolor(int type)
{
   switch(type)
   {
      case VEHICLE_POLICECAR:return VEHICLECOLOR_POLICE;
      case VEHICLE_TAXICAB:return VEHICLECOLOR_TAXI;
   }

   return -1;
}


void cityname(char *story)
{
   switch(LCSrandom(20))
   {
      case 0:strcpy(story,"San Francisco, CA");break;
      case 1:strcpy(story,"Boston, MA");break;
      case 2:strcpy(story,"Los Angeles, CA");break;
      case 3:strcpy(story,"Detroit, MI");break;
      case 4:strcpy(story,"Cleveland, OH");break;
      case 5:strcpy(story,"Cincinnati, OH");break;
      case 6:strcpy(story,"New York, NY");break;
      case 7:strcpy(story,"Chicago, IL");break;
      case 8:strcpy(story,"Trenton, NJ");break;
      case 9:strcpy(story,"Denver, CO");break;
      case 10:strcpy(story,"Phoenix, AZ");break;
      case 11:strcpy(story,"Little Rock, AR");break;
      case 12:strcpy(story,"Houston, TX");break;
      case 13:strcpy(story,"Dallas, TX");break;
      case 14:strcpy(story,"Hartford, CT");break;
      case 15:strcpy(story,"Miami, FL");break;
      case 16:strcpy(story,"Baton Rouge, LA");break;
      case 17:strcpy(story,"Seattle, WA");break;
      case 18:strcpy(story,"Salt Lake City, UT");break;
      case 19:strcpy(story,"Philadelphia, PA");break;
   }
}

/* Allow the player to enter a name with an optional default */
void enter_name(char *name, int len, char* defname)
{
	keypad(stdscr,FALSE);
        raw_output(FALSE);
        echo();
        curs_set(1);
        getnstr(name,len);
        curs_set(0);
        noecho();
        raw_output(TRUE);
        keypad(stdscr,TRUE);
	
	if((defname!=NULL) && (strncmp(name,"",len)==0))
	{
		strncpy(name,defname,len);
	}
	name[len-1]='\0';
}


