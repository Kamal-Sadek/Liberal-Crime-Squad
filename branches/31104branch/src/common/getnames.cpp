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

#include <includes.h>
#include <externs.h>

void getactivity(string& str,activityst &act)
{
   str="";

   switch(act.type)
   {
      case ACTIVITY_HOSTAGETENDING:
      {
         str+="Tending to ";
         int pl=getpoolcreature(act.arg);
         if(pl!=-1)
         {
            str+=pool[pl]->name;
         }
         break;
      }
      case ACTIVITY_REPAIR_ARMOR:
         str+="Repairing Clothing";
         break;
      case ACTIVITY_WHEELCHAIR:
         str+="Procuring a Wheelchair";
         break;
      case ACTIVITY_STEALCARS:
         str+="Stealing a Car";
         break;
      case ACTIVITY_POLLS:
         str+="Gathering Opinion Info";
         break;
      case ACTIVITY_MAKE_ARMOR:
      {
         str+="Making ";
         string str2;
         getarmor(str2,act.arg);
         str+=str2;
         break;
      }
      case ACTIVITY_TROUBLE:
         str+="Causing Trouble";
         break;
      case ACTIVITY_PROSTITUTION:
         str+="Prostituting";
         break;
      case ACTIVITY_COMMUNITYSERVICE:
         str+="Volunteering";
         break;
      case ACTIVITY_GRAFFITI:
         str+="Making Graffiti";
         break;
      case ACTIVITY_CCFRAUD:
         str+="Credit Card Fraud";
         break;
      case ACTIVITY_DOS_RACKET:
         str+="Extorting Websites";
         break;
      case ACTIVITY_DOS_ATTACKS:
         str+="Attacking Websites";
         break;
      case ACTIVITY_HACKING:
         str+="Hacking Networks";
         break;
      case ACTIVITY_SELL_TSHIRTS:
         str+="Selling T-Shirts";
         break;
      case ACTIVITY_SELL_ART:
         str+="Selling Art";
         break;
      case ACTIVITY_TEACH_GENERALED:
         str+="Teaching General ED";
         break;
      case ACTIVITY_TEACH_POLITICS:
         str+="Teaching Politics";
         break;
      case ACTIVITY_TEACH_SURVIVAL:
         str+="Teaching Survival";
         break;
      case ACTIVITY_TEACH_FIGHTING:
         str+="Teaching Fighting";
         break;
      case ACTIVITY_TEACH_COVERT:
         str+="Teaching Covert Ops";
         break;
      case ACTIVITY_SELL_MUSIC:
         str+="Selling Music";
         break;
      case ACTIVITY_BURY:
         str+="Disposing of Bodies";
         break;
      case ACTIVITY_DONATIONS:
         str+="Soliciting Donations";
         break;
      case ACTIVITY_SELL_DRUGS:
         str+="Selling Brownies";
         break;
      case ACTIVITY_VISIT:
      {
         str+="Going to ";
         str+=location[act.arg]->shortname;
         break;
      }
      case ACTIVITY_HEAL:
         str+="Tending to Injuries";
         break;
      case ACTIVITY_NONE:
         str+="Hanging Out";
         break;
      case ACTIVITY_WRITE_LETTERS:
      case ACTIVITY_WRITE_GUARDIAN:
         str+="Writing";
         break;
      case ACTIVITY_CLINIC:
         str+="Going to Free CLINIC";
   }
}



void getweapon(string& str,int type)
{
   str="";

   if(year<2100)
   {
      switch(type)
      {
         //LIMIT ----------------------------------------:----: (-5 for guns)
         case WEAPON_NONE:         str="None";break;
         case WEAPON_CROWBAR:      str="Crowbar";break;
         case WEAPON_BASEBALLBAT:   str="Baseball Bat";break;
         case WEAPON_KNIFE:         str="Knife";break;
         case WEAPON_SHANK:         str="Shank";break;
         case WEAPON_SYRINGE:      str="Syringe";break;
         case WEAPON_REVOLVER_22:   str=".22 Rev.";break;
         case WEAPON_REVOLVER_44:   str=".44 Mag.";break;
         case WEAPON_SEMIPISTOL_9MM:   str="9mm Semi.";break;
         case WEAPON_SEMIPISTOL_45:   str=".45 Semi.";break;
         case WEAPON_SEMIRIFLE_AR15:   str="AR15";break;
         case WEAPON_SMG_MP5:       str="MP5";break;
         case WEAPON_CARBINE_M4:    str="M4";break;
         case WEAPON_AUTORIFLE_M16:   str="M16";break;
         case WEAPON_AUTORIFLE_AK47:   str="AK47";break;
         case WEAPON_SHOTGUN_PUMP:      str="Shotgun";break;
         case WEAPON_SWORD:         str="Sword";break;
         case WEAPON_DAISHO:         str="Daisho";break;
         case WEAPON_HAMMER:         str="Hammer";break;
         case WEAPON_MAUL:         str="Maul";break;
         case WEAPON_CROSS:         str="Cross";break;
         case WEAPON_STAFF:         str="Staff";break;
         case WEAPON_CHAIN:         str="Chain";break;
         case WEAPON_NIGHTSTICK:    str="Nightstick";break;
         case WEAPON_GAVEL:         str="Gavel";break;
         case WEAPON_PITCHFORK:     str="Pitchfork";break;
         case WEAPON_TORCH:         str="Torch";break;
         case WEAPON_GUITAR:        str="Guitar";break;
         case WEAPON_SPRAYCAN:      str="Spraycan";break;
      }
   }
   else
   {
      switch(type)
      {
         //LIMIT ----------------------------------------:----: (-5 for guns)
         case WEAPON_NONE:         str="None";break;
         case WEAPON_CROWBAR:      str="Crowbar";break;
         case WEAPON_BASEBALLBAT:   str="Spaceball Bat";break;
         case WEAPON_KNIFE:         str="Vibro-Knife";break;
         case WEAPON_SHANK:         str="Shank";break;
         case WEAPON_SYRINGE:       str="Injector";break;
         case WEAPON_REVOLVER_22:   str=".22 Slug";break;
         case WEAPON_REVOLVER_44:   str=".44 Slug";break;
         case WEAPON_SEMIPISTOL_9MM:str="9mm Las.";break;
         case WEAPON_SEMIPISTOL_45: str=".45 Las.";break;
         case WEAPON_SEMIRIFLE_AR15:str="X15 Semi";break;
         case WEAPON_SMG_MP5:       str="Prince SMG";break;
         case WEAPON_CARBINE_M4:    str="J4 Laser";break;
         case WEAPON_AUTORIFLE_M16: str="X15 Full";break;
         case WEAPON_AUTORIFLE_AK47:str="P74 War";break;
         case WEAPON_SHOTGUN_PUMP:  str="Burstgun";break;
         case WEAPON_SWORD:         str="Light Sword";break;
         case WEAPON_DAISHO:        str="Twin Sword";break;
         case WEAPON_HAMMER:        str="Hammer";break;
         case WEAPON_MAUL:          str="Maul";break;
         case WEAPON_CROSS:         str="Cross";break;
         case WEAPON_STAFF:         str="Staff";break;
         case WEAPON_CHAIN:         str="CyberArm";break;
         case WEAPON_NIGHTSTICK:    str="Shock Stick";break;
         case WEAPON_GAVEL:         str="Gavel";break;
         case WEAPON_PITCHFORK:     str="Pitchfork";break;
         case WEAPON_TORCH:         str="Flare";break;
         case WEAPON_GUITAR:        str="Guitar";break;
         case WEAPON_SPRAYCAN:      str="Spraycan";break;
      }
   }
}

void getweaponfull(string& str,int type,int subtype)
{
   str="";

   if(year<2100)
   {
      switch(type)
      {
         //LIMIT -----------------------------------------------:----: (-5 for guns)
         case WEAPON_NONE:          str="None";break;
         case WEAPON_CROWBAR:       str="Crowbar";break;
         case WEAPON_BASEBALLBAT:   str="Baseball Bat";break;
         case WEAPON_KNIFE:         str="Knife";break;
         case WEAPON_SHANK:         str="Shank";break;
         case WEAPON_SYRINGE:       str="Syringe";break;
         case WEAPON_REVOLVER_22:   str=".22 Revolver";break;
         case WEAPON_REVOLVER_44:   str=".44 Magnum";break;
         case WEAPON_SEMIPISTOL_9MM:str="9mm Semiauto";break;
         case WEAPON_SEMIPISTOL_45: str=".45 Semiauto";break;
         case WEAPON_SEMIRIFLE_AR15:str="AR15";break;
         case WEAPON_SMG_MP5:       str="MP5 SMG";break;
         case WEAPON_CARBINE_M4:    str="M4 Carbine";break;
         case WEAPON_AUTORIFLE_M16: str="M16";break;
         case WEAPON_AUTORIFLE_AK47:str="AK47";break;
         case WEAPON_SHOTGUN_PUMP:  str="Shotgun";break;
         case WEAPON_SWORD:         str="Sword of Morfiegor";break;
         case WEAPON_DAISHO:
            switch(subtype)
            {
            case 0: str="Daisho";break;
            case 1: str="Katana";break;
            case 2: str="Wakizashi";break;
            }
            break;
         case WEAPON_HAMMER:         str="Dwarven Hammer";break;
         case WEAPON_MAUL:          str="Maul of Anrin";break;
         case WEAPON_CROSS:         str="Silver Cross";break;
         case WEAPON_STAFF:         str="Wizard's Staff";break;
         case WEAPON_CHAIN:         str="Chain";break;
         case WEAPON_NIGHTSTICK:    str="Nightstick";break;
         case WEAPON_GAVEL:         str="Gavel";break;
         case WEAPON_PITCHFORK:     str="Pitchfork";break;
         case WEAPON_TORCH:         str="Torch";break;
         case WEAPON_GUITAR:        str="Guitar";break;
         case WEAPON_SPRAYCAN:      str="Spraycan";break;
      }
   }
   else
   {
      switch(type)
      {
      case WEAPON_BASEBALLBAT:   str="Spaceball Bat";break;
      case WEAPON_KNIFE:         str="Vibro-Knife";break;
      case WEAPON_SHANK:         str="Shank";break;
      case WEAPON_SYRINGE:      str="Medical Injector";break;
      case WEAPON_REVOLVER_22:   str=".22 Slug Pistol";break;
      case WEAPON_REVOLVER_44:   str=".44 Heavy Slug Pistol";break;
      case WEAPON_SEMIPISTOL_9MM:   str="9mm Laser Pistol";break;
      case WEAPON_SEMIPISTOL_45:   str=".45 Laser Pistol";break;
      case WEAPON_SEMIRIFLE_AR15:   str="X15 Personal Laser";break;
      case WEAPON_SMG_MP5:       str="Beijing Prince SMG";break;
      case WEAPON_CARBINE_M4:    str="J4 Laser Carbine";break;
      case WEAPON_AUTORIFLE_M16:   str="X15 Military Laser";break;
      case WEAPON_AUTORIFLE_AK47:   str="P74 War Laser";break;
      case WEAPON_SHOTGUN_PUMP:      str="Plasma Shotgun";break;
      case WEAPON_SWORD:         str="Light Sword";break;
      case WEAPON_DAISHO:
         switch(subtype)
         {
         case 0: str="Twin Swords of Liberalism";break;
         case 1: str="Retribution Sword";break;
         case 2: str="Justice Blade";break;
         }
         break;
      case WEAPON_HAMMER:         str="Dwarven Hammer";break;
      case WEAPON_MAUL:         str="Maul of Anrin";break;
      case WEAPON_CROSS:         str="Silver Cross";break;
      case WEAPON_STAFF:         str="Wizard's Staff";break;
      case WEAPON_CHAIN:         str="Industrial Cyber-Arm";break;
      case WEAPON_NIGHTSTICK:      str="Electro-Shock Stick";break;
      case WEAPON_GAVEL:         str="Gavel";break;
      case WEAPON_PITCHFORK:      str="Pitchfork";break;
      case WEAPON_TORCH:         str="Burning Flare";break;
      case WEAPON_GUITAR:      str="Ancient Guitar";break;
      case WEAPON_SPRAYCAN:         str="Retro Spraycan";break;
      }
   }
}


void getarmor(string& str,int type,int subtype)
{
   switch(type)
   {
      //LIMIT ---------------------------------------------:
      case ARMOR_NONE:         str="Naked";break;
      case ARMOR_CLOTHES:         str="Clothes";break;
      case ARMOR_OVERALLS:      str="Overalls";break;
      case ARMOR_WIFEBEATER:      str="Wife Beater";break;
      case ARMOR_TRENCHCOAT:      str="Trenchcoat";break;
      case ARMOR_WORKCLOTHES:      str="Work Clothes";break;
      case ARMOR_SECURITYUNIFORM:   str="Security Unif.";break;
      case ARMOR_POLICEUNIFORM:   str="Police Uniform";break;
      case ARMOR_CHEAPSUIT:      str="Cheap Suit";break;
      case ARMOR_EXPENSIVESUIT:   str="Expensive Suit";break;
      case ARMOR_BLACKSUIT:      str="Black Suit";break;
      case ARMOR_CHEAPDRESS:      str="Cheap Dress";break;
      case ARMOR_EXPENSIVEDRESS:   str="Expensve Dress";break;
      case ARMOR_BLACKDRESS:      str="Black Dress";break;
      case ARMOR_LABCOAT:         str="Lab Coat";break;
      case ARMOR_BLACKROBE:      str="Black Robe";break;
      case ARMOR_CLOWNSUIT:      str="Clown Suit";break;
      case ARMOR_BONDAGEGEAR:      str="Bondage Gear";break;
      case ARMOR_MASK:
         switch(subtype)
         {
      case MASK_ASHCROFT:         str="Ashcroft Mask";break;
      case MASK_BARR:            str="Bob Barr Mask";break;
      case MASK_BLAIR:         str="Blair Mask";break;
      case MASK_BUSH_BARBARA:      str="Brb. Bush Mask";break;
      case MASK_BUSH_GEORGE:      str="G HW Bush Mask";break;
      case MASK_BUSH_GEORGE2:      str="G W. Bush Mask";break;
      //LIMIT ---------------------------------------------:
      case MASK_CARTER:         str="Carter Mask";break;
      case MASK_CHENEY:         str="Cheney Mask";break;
      case MASK_CLINTON_BILL:      str="B Clinton Mask";break;
      case MASK_CLINTON_HILLARY:   str="H Clinton Mask";break;
      case MASK_FALWELL:         str="Falwell Mask";break;
      case MASK_FORD:            str="Gld. Ford Mask";break;
      case MASK_GORE_AL:         str="Al Gore Mask";break;
      case MASK_GORE_TIPPER:      str="Tpr. Gore Mask";break;
      case MASK_HELMS:         str="Helms Mask";break;
      case MASK_JASON:         str="Jason Mask";break;
      case MASK_LINCOLN:         str="Lincoln Mask";break;
      case MASK_KENNEDY_JOHN:      str="JFK Mask";break;
      case MASK_KENNEDY_ROBERT:   str="RFK Mask";break;
      case MASK_KISSINGER:      str="Kissinger Mask";break;
      case MASK_NIXON:         str="Nixon Mask";break;
      case MASK_POWELL:         str="Powell Mask";break;
      case MASK_REAGAN_NANCY:      str="N. Reagan Mask";break;
      case MASK_REAGAN_RONALD:   str="R. Reagan Mask";break;
      case MASK_RICE:            str="Cn. Rice Mask";break;
      case MASK_ROBERTSON:      str="Robertson Mask";break;
      case MASK_RUMSFELD:         str="Rumsfeld Mask";break;
      case MASK_SATAN:         str="Satan Mask";break;
      case MASK_STARR:         str="Ken Starr Mask";break;
      case MASK_THURMOND:         str="Thurmond Mask";break;
      case MASK_WASHINGTON:      str="Washngton Mask";break;
      case MASK_WAXMAN:         str="Waxman Mask";break;
      //LIMIT ---------------------------------------------:
      case MASK_JESUS:         str="Jesus Mask";break;
      case MASK_COLEMAN_GARY:      str="G Coleman Mask";break;
      case MASK_MADONNA:         str="Madonna Mask";break;
      case MASK_SPEARS:         str="B Spears Mask";break;
      case MASK_EMINEM:         str="Eminem Mask";break;
      case MASK_AGUILERA:         str="C Aguilra Mask";break;
      case MASK_WAHLBERG:         str="M Wahlbrg Mask";break;
      case MASK_IGGYPOP:         str="Iggy Pop Mask";break;
      case MASK_CASH:            str="Jny. Cash Mask";break;
      case MASK_BINLADEN:         str="bin Laden Mask";break;
      case MASK_LORDS:         str="Tr. Lords Mask";break;
      case MASK_SHIELDS:         str="Br. Shlds Mask";break;
      case MASK_JACKSON_MICHAEL:   str="M Jackson Mask";break;
      case MASK_CRUTHERS:         str="Cruthers Mask";break;
      case MASK_KING_DON:         str="Don King Mask";break;
      //LIMIT ---------------------------------------------:
      default:
                           str="Mask";
         }
         break;
      case ARMOR_MILITARY:      str="Army Uniform";break;
      case ARMOR_PRISONGUARD:      str="Guard Uniform";break;
      case ARMOR_PRISONER:      str="Orange Jmpsuit";break;
      case ARMOR_TOGA:         str="Toga";break;
      case ARMOR_MITHRIL:         str="Mithril Mail";break;
      case ARMOR_BALLISTICVEST:
         {
            switch(subtype)
            {
            case BVEST_POLICE:      str="Police BdyArmr";break;
            case BVEST_MILITARY:    str="Army BodyArmor";break;
            default:                str="Body Armor";break;
            }
            break;
         }
      case ARMOR_HEAVYBALLISTICVEST:   str="Hvy Body Armor";break;
   }
}


void getarmorfull(string& str,int type,int subtype)
{
   switch(type)
   {
      //LIMIT --------------------------------------------------:
      case ARMOR_NONE:         str="Naked";break;
      case ARMOR_CLOTHES:         str="Clothes";break;
      case ARMOR_OVERALLS:      str="Overalls";break;
      case ARMOR_WIFEBEATER:      str="Wife Beater";break;
      case ARMOR_TRENCHCOAT:      str="Trenchcoat";break;
      case ARMOR_WORKCLOTHES:      str="Work Clothes";break;
      case ARMOR_SECURITYUNIFORM:   str="Security Uniform";break;
      case ARMOR_POLICEUNIFORM:   str="Police Uniform";break;
      case ARMOR_CHEAPSUIT:      str="Cheap Suit";break;
      case ARMOR_EXPENSIVESUIT:   str="Expensive Suit";break;
      case ARMOR_BLACKSUIT:      str="Black Suit";break;
      case ARMOR_CHEAPDRESS:      str="Cheap Dress";break;
      case ARMOR_EXPENSIVEDRESS:   str="Expensve Dress";break;
      case ARMOR_BLACKDRESS:      str="Black Dress";break;
      case ARMOR_LABCOAT:         str="Lab Coat";break;
      case ARMOR_BLACKROBE:      str="Black Robe";break;
      case ARMOR_CLOWNSUIT:      str="Clown Suit";break;
      case ARMOR_BONDAGEGEAR:      str="Bondage Gear";break;
      case ARMOR_MASK:
         switch(subtype)
         {
      case MASK_ASHCROFT:         str="John Ashcroft Mask";break;
      case MASK_BARR:            str="Bob Barr Mask";break;
      case MASK_BLAIR:         str="Tony Blair Mask";break;
      case MASK_BUSH_BARBARA:      str="Barbara Bush Mask";break;
      case MASK_BUSH_GEORGE:      str="George HW Bush Mask";break;
      case MASK_BUSH_GEORGE2:      str="George W. Bush Mask";break;
      //LIMIT --------------------------------------------------:
      case MASK_CARTER:         str="Jimmy Carter Mask";break;
      case MASK_CHENEY:         str="Dick Cheney Mask";break;
      case MASK_CLINTON_BILL:      str="Bill Clinton Mask";break;
      case MASK_CLINTON_HILLARY:   str="Hillary Clinton Msk";break;
      case MASK_FALWELL:         str="Jerry Falwell Mask";break;
      case MASK_FORD:            str="Gerald Ford Mask";break;
      case MASK_GORE_AL:         str="Al Gore Mask";break;
      case MASK_GORE_TIPPER:      str="Tipper Gore Mask";break;
      case MASK_HELMS:         str="Jesse Helms Mask";break;
      case MASK_JASON:         str="Jason Mask";break;
      case MASK_LINCOLN:         str="Abraham Lincoln Msk";break;
      case MASK_KENNEDY_JOHN:      str="John F Kennedy Mask";break;
      case MASK_KENNEDY_ROBERT:   str="Robert Kennedy Mask";break;
      case MASK_KISSINGER:      str="Henry Kissinger Msk";break;
      case MASK_NIXON:         str="Richard Nixon Mask";break;
      case MASK_POWELL:         str="Colin Powell Mask";break;
      case MASK_REAGAN_NANCY:      str="Nancy Reagan Mask";break;
      case MASK_REAGAN_RONALD:   str="Ronald Reagan Mask";break;
      case MASK_RICE:            str="Condoleezza Rice Mk";break;
      case MASK_ROBERTSON:      str="Pat Robertson Mask";break;
      case MASK_RUMSFELD:         str="Donald Rumsfeld Msk";break;
      case MASK_SATAN:         str="Satan Mask";break;
      case MASK_STARR:         str="Ken Starr Mask";break;
      case MASK_THURMOND:         str="Strom Thurmond Mask";break;
      case MASK_WASHINGTON:      str="George Washngtn Msk";break;
      case MASK_WAXMAN:         str="Henry Waxman Mask";break;
      //LIMIT --------------------------------------------------:
      case MASK_JESUS:         str="Jesus Christ Mask";break;
      case MASK_COLEMAN_GARY:      str="Gary Coleman Mask";break;
      case MASK_MADONNA:         str="Madonna Mask";break;
      case MASK_SPEARS:         str="Britney Spears Mask";break;
      case MASK_EMINEM:         str="Eminem Mask";break;
      case MASK_AGUILERA:         str="Chrstna Aguilera Mk";break;
      case MASK_WAHLBERG:         str="Mark Wahlbrg Mask";break;
      case MASK_IGGYPOP:         str="Iggy Pop Mask";break;
      case MASK_CASH:            str="Johnny Cash Mask";break;
      case MASK_BINLADEN:         str="Osama bin Laden Msk";break;
      case MASK_LORDS:         str="Traci Lords Mask";break;
      case MASK_SHIELDS:         str="Brooke Shields Mask";break;
      case MASK_JACKSON_MICHAEL:   str="Michael Jackson Msk";break;
      case MASK_CRUTHERS:         str="Scatman Cruthers Mk";break;
      case MASK_KING_DON:         str="Don King Mask";break;
      //LIMIT --------------------------------------------------:
      default:
                           str="Mask";
         }
         break;
      case ARMOR_MILITARY:      str="Army Uniform";break;
      case ARMOR_PRISONGUARD:      str="Guard Uniform";break;
      case ARMOR_PRISONER:      str="Orange Jumpsuit";break;
      case ARMOR_TOGA:         str="Toga";break;
      case ARMOR_MITHRIL:         str="Mithril Mail";break;
      case ARMOR_BALLISTICVEST:
         {
            switch(subtype)
            {
            case BVEST_POLICE:      str="Police Body Armor";break;
            case BVEST_MILITARY:    str="Army Body Armor";break;
            default:                str="Body Armor";break;
            }
            break;
         }
      case ARMOR_HEAVYBALLISTICVEST:   str="Heavy Body Armor";break;
   }
}


void getmaskdesc(string& str,short mask)
{
   switch(mask)
   {
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_ASHCROFT:            str="U.S. Senator, Attorney General";break;
      case MASK_BARR:               str="U.S. Representative";break;
      case MASK_BLAIR:            str="British Prime Minister";break;
      case MASK_BUSH_BARBARA:         str="U.S. First Lady";break;
      case MASK_BUSH_GEORGE:         str="U.S. President";break;
      case MASK_BUSH_GEORGE2:         str="U.S. President";break;
      case MASK_CARTER:            str="U.S. President";break;
      case MASK_CHENEY:            str="U.S. Vice President, CEO";break;
      case MASK_CLINTON_BILL:         str="U.S. President";break;
      case MASK_CLINTON_HILLARY:      str="U.S. Senator, First Lady";break;
      case MASK_FALWELL:            str="Reverend";break;
      case MASK_FORD:               str="U.S. President";break;
      case MASK_GORE_AL:            str="U.S. Vice President";break;
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_GORE_TIPPER:         str="U.S. Second Lady";break;
      case MASK_HELMS:            str="U.S. Senator";break;
      case MASK_JASON:            str="A Hockey Mask a la Friday the 13th";break;
      case MASK_LINCOLN:            str="U.S. President, Assassinated";break;
      case MASK_KENNEDY_JOHN:         str="U.S. President, Assassinated";break;
      case MASK_KENNEDY_ROBERT:      str="U.S. Senator, Assassinated";break;
      case MASK_KISSINGER:         str="U.S. Secretary of State";break;
      case MASK_NIXON:            str="U.S. President, Resigned";break;
      case MASK_POWELL:            str="U.S. Secretary of State, General";break;
      case MASK_REAGAN_NANCY:         str="U.S. First Lady";break;
      case MASK_REAGAN_RONALD:      str="U.S. President";break;
      case MASK_RICE:               str="U.S. National Security Advisor";break;
      case MASK_ROBERTSON:         str="Televangelist";break;
      case MASK_RUMSFELD:            str="U.S. Secretary of Defense";break;
      case MASK_SATAN:            str="Prince of Darkness, Red with Horns";break;
      case MASK_STARR:            str="Independent Counsel";break;
      case MASK_THURMOND:            str="U.S. Senator";break;
      case MASK_WASHINGTON:         str="U.S. President";break;
      case MASK_WAXMAN:            str="U.S. Representative";break;
      case MASK_JESUS:            str="The Son of God, The Lamb, He died 4 U";break;
      case MASK_COLEMAN_GARY:         str="Actor, Candidate for Governor";break;
      case MASK_MADONNA:            str="Singer, Actor";break;
      case MASK_SPEARS:            str="Singer, Actor";break;
      case MASK_EMINEM:            str="Rapper, Actor";break;
      case MASK_AGUILERA:            str="Singer";break;
      case MASK_WAHLBERG:            str="Actor, Rapper";break;
      //LIMIT-------------------------------------:--------------------------------------:
      case MASK_IGGYPOP:            str="Singer, Actor";break;
      case MASK_CASH:               str="Singer";break;
      case MASK_BINLADEN:            str="Terrorist";break;
      case MASK_LORDS:            str="Actor";break;
      case MASK_SHIELDS:            str="Actor";break;
      case MASK_JACKSON_MICHAEL:      str="Singer";break;
      case MASK_CRUTHERS:            str="Singer, Actor";break;
      case MASK_KING_DON:            str="Boxing Promoter";break;
   }
}


void getarmorfull(string& str,armorst &armor,char superfull)
{
   if(superfull)getarmorfull(str,armor.type,armor.subtype);
   else getarmor(str,armor.type,armor.subtype);

   if(armor.flag!=0||armor.quality!='1')
   {
      str+="[";
      if(armor.quality!='1')
      {
         char c[2];
         c[0]=armor.quality;
         c[1]='\x0';
         str+=c;
      }
      if(armor.flag & ARMORFLAG_BLOODY)
      {
         str+="B";
      }
      if(armor.flag & ARMORFLAG_DAMAGED)
      {
         str+="D";
      }
      str+="]";
   }
}



void getskill(string& str,int type)
{
   str="";

   switch(type)
   {
      case SKILL_HANDTOHAND:str="Hand-to-Hand";break;
      case SKILL_KNIFE:str="Knife";break;
      case SKILL_SWORD:str="Sword";break;
      case SKILL_IMPROVISED:str="Improv Weapons";break;
      case SKILL_CLUB:str="Club";break;
      case SKILL_PISTOL:str="Pistol";break;
      case SKILL_RIFLE:str="Rifle";break;
      case SKILL_SHOTGUN:str="Shotgun";break;
      case SKILL_SMG:str="Submachine Gun";break;
      case SKILL_PERSUASION:str="Persuasion";break;
      case SKILL_INTERROGATION:str="Interrogation";break;
      case SKILL_SECURITY:str="Security";break;
      case SKILL_DISGUISE:str="Disguise";break;
      case SKILL_COMPUTERS:str="Computers";break;
      case SKILL_LAW:str="Law";break;
      case SKILL_GARMENTMAKING:str="Garment Making";break;
      case SKILL_DRIVING:str="Driving";break;
      case SKILL_WRITING:str="Writing";break;
      case SKILL_COOKING:str="Cooking";break;
      case SKILL_MUSIC:str="Music";break;
      case SKILL_ART:str="Art";break;
      case SKILL_RELIGION:str="Religion";break;
      case SKILL_SCIENCE:str="Science";break;
      case SKILL_BUSINESS:str="Business";break;
      case SKILL_GANGSTERISM:str="Gangsterism";break;
      case SKILL_STEALTH:str="Stealth";break;
      case SKILL_SLEIGHTOFHAND:str="Sleight of Hand";break;
      case SKILL_TEACHING:str="Teaching";break;
      case SKILL_STREETSENSE:str="Street Sense";break;
      case SKILL_SEDUCTION:str="Seduction";break;
      case SKILL_LEADERSHIP:str="Leadership";break;
      case SKILL_MEDICAL:str="First Aid";break;
      case SKILL_TACTICS:str="Tactics";break;
   }
}



void getclip(string& str,int clip)
{
   str="";

   if(year<2100)
   {
      switch(clip)
      {
         case CLIP_9:str+="9mm Pistol Mag";break;
         case CLIP_45:str+=".45 Pistol Mag";break;
         case CLIP_ASSAULT:str+="Aslt/Rifle Mag";break;
         case CLIP_SMG:str+="9mm SMG Mag";break;
         case CLIP_22:str+=".22 Speedloader";break;
         case CLIP_44:str+=".44 Speedloader";break;
         case CLIP_BUCKSHOT:str+="6 Shotgun Shells";break;
      }
   }
   else
   {
      switch(clip)
      {
         case CLIP_9:str+="9mm Powerpack";break;
         case CLIP_45:str+=".45 Powerpack";break;
         case CLIP_ASSAULT:str+="Rifle Powerpack";break;
         case CLIP_SMG:str+="SMG Powerpack";break;
         case CLIP_22:str+=".22 Slug Mag";break;
         case CLIP_44:str+=".44 Heavy Slug Mag";break;
         case CLIP_BUCKSHOT:str+="Shotgun Plasma Pack";break;
      }
   }
}


void getloot(string& str,int loot)
{
   str="";

   switch(loot)
   {
      case LOOT_LABEQUIPMENT:str+="Lab Equipment";break;
      case LOOT_COMPUTER:str+="Computer";break;
      case LOOT_TV:str+="TV";break;
      case LOOT_SECRETDOCUMENTS:str+="Secret Documents";break;
      case LOOT_POLICERECORDS:str+="Police Records";break;
      case LOOT_VCR:str+="VCR";break;
      case LOOT_CELLPHONE:str+="Cellphone";break;
      case LOOT_TVCAMERA:str+="TV Camera";break;
      case LOOT_BROADCASTINGEQUIPMENT:str+="Broadcasting Equip.";break;
      case LOOT_SILVERWARE:str+="Silverware";break;
      case LOOT_SCANNER:str+="Scanner";break;
      case LOOT_PRINTER:str+="Printer";break;
      case LOOT_FINECLOTH:str+="Fine Cloth";break;
      case LOOT_CHEMICAL:str+="Weird Chemical";break;
      case LOOT_CEOPHOTOS:str+="CEO Photos";break;
      case LOOT_INTHQDISK:str+="Intel. HQ Data Disk";break;
      case LOOT_CORPFILES:str+="Secret Corporate Files";break;
   }
}



void getrecruitcreature(string& str,int type)
{
   str="";

   switch(type)
   {
      case CREATURE_BOUNCER:if(sitetype==SITE_BUSINESS_CIGARBAR)str+="Club Security";
                            else str+="Bouncer";
                            break;
      case CREATURE_SECURITYGUARD:str+="Security Guard";break;
      case CREATURE_SCIENTIST_LABTECH:str+="Lab Tech";break;
      case CREATURE_SCIENTIST_EMINENT:str+="Eminent Scientist";break;
      case CREATURE_CORPORATE_MANAGER:str+="Corporate Manager";break;
      case CREATURE_CORPORATE_CEO:str+="CEO of a Corporation";break;
      case CREATURE_WORKER_SERVANT:
         if(law[LAW_LABOR]==-2&&
             law[LAW_CORPORATE]==-2)str="Slave";
         else str+="Servant";
         break;
      case CREATURE_WORKER_JANITOR:str+="Janitor";break;
      case CREATURE_WORKER_SWEATSHOP:str+="Sweatshop Worker";break;
      case CREATURE_WORKER_FACTORY_NONUNION:str+="Factory Worker";break;
      case CREATURE_WORKER_FACTORY_CHILD:str+="Child Worker";break;
      case CREATURE_WORKER_SECRETARY:str+="Secretary";break;
      case CREATURE_WORKER_FACTORY_UNION:str+="Factory Worker";break;
      case CREATURE_LANDLORD:str+="Landlord";break;
      case CREATURE_COP:str+="Police Officer";break;
      case CREATURE_SWAT:str+="SWAT Officer";break;
      case CREATURE_DEATHSQUAD:str+="Death Squad Officer";break;
      case CREATURE_GANGUNIT:str+="Police Gang Unit";break;
      case CREATURE_PRISONGUARD:str+="Prison Guard";break;
      case CREATURE_EDUCATOR:str+="Educator";break;
      case CREATURE_MERC:str+="Mercenary";break;
      case CREATURE_HICK:str+="Redneck";break;
      case CREATURE_SOLDIER:str+="Soldier";break;
      case CREATURE_JUDGE_LIBERAL:str+="Judge";break;
      case CREATURE_JUDGE_CONSERVATIVE:str+="Judge";break;
      case CREATURE_AGENT:str+="Agent";break;
      case CREATURE_RADIOPERSONALITY:str+="Radio Personality";break;
      case CREATURE_NEWSANCHOR:str+="Cable News Anchor";break;
      case CREATURE_LAWYER:str+="Lawyer";break;
      case CREATURE_SEWERWORKER:str+="Sewer Worker";break;
      case CREATURE_COLLEGESTUDENT:str+="College Student";break;
      case CREATURE_MUSICIAN:str+="Musician";break;
      case CREATURE_MATHEMATICIAN:str+="Mathematician";break;
      case CREATURE_TEACHER:str+="Teacher";break;
      case CREATURE_HSDROPOUT:str+="Highschool Dropout";break;
      case CREATURE_BUM:str+="Transient";break;
      case CREATURE_MUTANT:str+="Mutant";break;
      case CREATURE_GANGMEMBER:str+="Gang Member";break;
      case CREATURE_CRACKHEAD:str+="Crack Head";break;
      case CREATURE_PRIEST:str+="Priest";break;
      case CREATURE_ENGINEER:str+="Engineer";break;
      case CREATURE_FASTFOODWORKER:str+="Fast Food Worker";break;
      case CREATURE_TELEMARKETER:str+="Telemarketer";break;
      case CREATURE_OFFICEWORKER:str+="Office Worker";break;
      case CREATURE_FOOTBALLCOACH:str+="Football Coach";break;
      case CREATURE_PROSTITUTE:str+="Prostitute";break;
      case CREATURE_MAILMAN:str+="Mail Carrier";break;
      case CREATURE_GARBAGEMAN:str+="Garbage Collector";break;
      case CREATURE_PLUMBER:str+="Plumber";break;
      case CREATURE_CHEF:str+="Chef";break;
      case CREATURE_CONSTRUCTIONWORKER:str+="Construction Worker";break;
      case CREATURE_AMATEURMAGICIAN:str+="Amateur Magician";break;
      case CREATURE_HIPPIE:str+="Hippie";break;
      case CREATURE_CRITIC_ART:str+="Art Critic";break;
      case CREATURE_CRITIC_MUSIC:str+="Music Critic";break;
      case CREATURE_AUTHOR:str+="Author";break;
      case CREATURE_JOURNALIST:str+="Journalist";break;
      case CREATURE_SOCIALITE:str+="Socialite";break;
      case CREATURE_BIKER:str+="Biker";break;
      case CREATURE_TRUCKER:str+="Trucker";break;
      case CREATURE_TAXIDRIVER:str+="Taxi Driver";break;
      case CREATURE_PROGRAMMER:str+="Programmer";break;
      case CREATURE_NUN:str+="Nun";break;
      case CREATURE_RETIREE:str+="Retiree";break;
      case CREATURE_PAINTER:str+="Painter";break;
      case CREATURE_SCULPTOR:str+="Sculptor";break;
      case CREATURE_DANCER:str+="Dancer";break;
      case CREATURE_PHOTOGRAPHER:str+="Photographer";break;
      case CREATURE_CAMERAMAN:str+="Cameraman";break;
      case CREATURE_HAIRSTYLIST:str+="Hairstylist";break;
      case CREATURE_FASHIONDESIGNER:str+="Fashion Designer";break;
      case CREATURE_CLERK:str+="Clerk";break;
      case CREATURE_THIEF:str+="Professional Thief";break;
      case CREATURE_ACTOR:str+="Actor";break;
      case CREATURE_YOGAINSTRUCTOR:str+="Yoga Instructor";break;
      case CREATURE_ATHLETE:str+="Athlete";break;
      case CREATURE_TEENAGER:str+="Teenager";break;
      case CREATURE_PRISONER:str+="Prisoner";break;
      case CREATURE_DOCTOR:str+="Doctor";break;
      case CREATURE_NURSE:str+="Nurse";break;
      case CREATURE_CCS_VIGILANTE:str+="CCS Vigilante";break;
      case CREATURE_CCS_ARCHCONSERVATIVE:str+="CCS Vigilante Boss";break;
      default:
         str+="Liberal";
         break;
   }
}



void gettitle(string& str,creaturest &cr)
{
   if(cr.align==-1)
   {
      str="Mindless Conservative";
   }
   else if(cr.align==0)
   {
      str="Non-Liberal";
   }
   else
   {
      if(cr.juice<=-50)
      {
         if(law[LAW_FREESPEECH]==-2)str="[Darn] Worthless";
         else str="Damn Worthless";
      }
      else if(cr.juice<=-10)str="Society's Dregs";
      else if(cr.juice<0)str="Punk";
      else if(cr.juice<10)str="Civilian";
      else if(cr.juice<50)str="Activist";
      else if(cr.juice<100)str="Socialist Threat";
      else if(cr.juice<200)str="Revolutionary";
      else if(cr.juice<500)str="Urban Commando";
      else str="Elite Liberal";
   }
}


void getview(string& str,short view)
{
   str="";

   switch(view)
   {
      case VIEW_ABORTION:str+="Abortion";break;
      case VIEW_GAY:str+="Homosexual Rights";break;
      case VIEW_DEATHPENALTY:str+="Death Penalty";break;
		case VIEW_TAXES:str+="Taxes";break;
      case VIEW_NUCLEARPOWER:str+="Nuclear Power";break;
      case VIEW_ANIMALRESEARCH:str+="Animal Cruelty";break;
      case VIEW_POLICEBEHAVIOR:str+="The Police";break;
      case VIEW_PRISONS:str+="Prisons";break;
      case VIEW_INTELLIGENCE:str+="Privacy";break;
      case VIEW_FREESPEECH:str+="Free Speech";break;
      case VIEW_GENETICS:str+="Genetics";break;
      case VIEW_JUSTICES:str+="The Judiciary";break;
      case VIEW_SWEATSHOPS:str+="Labor";break;
      case VIEW_POLLUTION:str+="Pollution";break;
      case VIEW_CORPORATECULTURE:str+="Corporate Culture";break;
      case VIEW_CEOSALARY:str+="CEO Compensation";break;
      case VIEW_AMRADIO:str+="AM Radio";break;
      case VIEW_CABLENEWS:str+="Cable News";break;
      case VIEW_LIBERALCRIMESQUAD:str+="Who We Are";break;
      case VIEW_LIBERALCRIMESQUADPOS:str+="Why We Rock";break;
   }
}



void getlaw(string& str,int l)
{
   switch(l)
   {
      case LAW_ABORTION:str="Abortion Rights";break;
      case LAW_ANIMALRESEARCH:str="Animal Research";break;
      case LAW_POLICEBEHAVIOR:str="Police Behavior";break;
      case LAW_PRIVACY:str="Privacy";break;
      case LAW_DEATHPENALTY:str="Capital Punishment";break;
      case LAW_NUCLEARPOWER:str="Nuclear Power";break;
      case LAW_POLLUTION:str="Pollution";break;
      case LAW_LABOR:str="Workers' Rights";break;
      case LAW_GAY:str="Homosexual Rights";break;
      case LAW_CORPORATE:str="Corporate Ethics";break;
      case LAW_FREESPEECH:str="Freedom of Speech";break;
		case LAW_TAX:str="Tax Law";break;
      case LAW_FLAGBURNING:str="Flag Burning";break;
      case LAW_GUNCONTROL:str="Gun Control";break;
   }
}



void getcarfull(string& str,int type)
{
   str="";

   switch(type)
   {
      case VEHICLE_VAN:str+="Van";break;
      case VEHICLE_STATIONWAGON:str+="Station Wagon";break;
      case VEHICLE_SPORTSCAR:str+="Sportscar";break;
      case VEHICLE_BUG:str+="Bug";break;
      case VEHICLE_PICKUP:str+="Pickup Truck";break;
      case VEHICLE_POLICECAR:str+="Police Cruiser";break;
      case VEHICLE_TAXICAB:str+="Taxicab";break;
      case VEHICLE_SUV:str+="SUV";break;
      case VEHICLE_AGENTCAR:str+="Car";break;
      case VEHICLE_JEEP:str+="Jeep";break;
   }
}


void getcarfull(string& str,vehiclest &car,char halffull)
{
   string str2;
   char num[20];
   str="";
   if(car.color!=naturalcarcolor(car.type))
   {
      getcarcolor(str2,car.color);
      str+=str2;
      str+=" ";
   }
   if(car.myear!=-1)
   {
      itoa(car.myear,num,10);
      str+=num;
      str+=" ";
   }
   if(halffull)getcar(str2,car.type);
   else getcarfull(str2,car.type);
   str+=str2;
}


void getcar(string& str,int type)
{
   str="";
   switch(type)
   {
      //LIMIT-------------------------------------:-----:
      case VEHICLE_VAN:            str+="Van";break;
      case VEHICLE_STATIONWAGON:      str+="Wagon";break;
      case VEHICLE_SPORTSCAR:         str+="Sport";break;
      case VEHICLE_BUG:            str+="Bug";break;
      case VEHICLE_PICKUP:         str+="Pickup";break;
      case VEHICLE_POLICECAR:         str+="Cruiser";break;
      case VEHICLE_TAXICAB:         str+="Taxicab";break;
      case VEHICLE_SUV:            str+="SUV";break;
      case VEHICLE_AGENTCAR:         str+="Car";break;
      case VEHICLE_JEEP:            str+="Jeep";break;
   }
}



void getcarcolor(string& str,int type)
{
   str="";
   switch(type)
   {
      case VEHICLECOLOR_RED:str+="Red";break;
      case VEHICLECOLOR_WHITE:str+="White";break;
      case VEHICLECOLOR_BLUE:str+="Blue";break;
      case VEHICLECOLOR_BEIGE:str+="Beige";break;
      case VEHICLECOLOR_POLICE:str+="Police-Marked";break;
      case VEHICLECOLOR_TAXI:str+="Taxi-Striped";break;
      case VEHICLECOLOR_BLACK:str+="Black";break;
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


void cityname(string& story)
{
   switch(LCSrandom(20))
   {
      case 0:story="San Francisco, CA";break;
      case 1:story="Boston, MA";break;
      case 2:story="Los Angeles, CA";break;
      case 3:story="Detroit, MC";break;
      case 4:story="Cleveland, OH";break;
      case 5:story="Cincinnati, OH";break;
      case 6:story="New York, NY";break;
      case 7:story="Chicago, IL";break;
      case 8:story="Trenton, NJ";break;
      case 9:story="Denver, CO";break;
      case 10:story="Phoenix, AZ";break;
      case 11:story="Little Rock, AR";break;
      case 12:story="Houston, TX";break;
      case 13:story="Dallas, TX";break;
      case 14:story="Hartford, CT";break;
      case 15:story="Miami, FL";break;
      case 16:story="Baton Rouge, LA";break;
      case 17:story="Seattle, WA";break;
      case 18:story="Salt Lake City, UT";break;
      case 19:story="Philadelphia, PA";break;
   }
}

/* Allow the player to enter a name with an optional default */
void enter_name(string& name, int len, string& defname)
{
   char* str = new char[len+1];
	keypad(stdscr,FALSE);
        raw_output(FALSE);
        echo();
        curs_set(1);
        getnstr(str,len);
        curs_set(0);
        noecho();
        raw_output(TRUE);
        keypad(stdscr,TRUE);

   str[len]='\0';
   name=str;
   delete str;
	
	if((defname!="") && (name==""))
	{
		name=defname;
	}
}


