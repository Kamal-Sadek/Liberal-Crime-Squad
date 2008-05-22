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



/* active squad visits the hospital */
void hospital(int loc)
{
   locatesquad(activesquad,loc);

   int partysize=0;
   int partydead=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      erase();

      locheader();
      printparty();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(10,1);
      addstr("F - Go in and fix up Conservative wounds");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(12,1);
      addstr("Enter - Leave");

      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(13,1);
      addstr("# - Check the status of a squad Liberal");
      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("0 - Show the squad's Liberal status");

      int c=getch();
      translategetch(c);

      if(c==10)break;

      if(c=='0')party_status=-1;

      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

      if(c=='f')
      {
         for(int p=5;p>=0;p--)
         {
            if(activesquad->squad[p]==NULL)continue;
            hospitalize(loc,*activesquad->squad[p]);
         }
         break;
      }
   }while(1);
}


/* active squad visits the arms dealer */
void armsdealer(int loc)
{
   short buyer=0;
   short in_gunshop=0;
   int l;

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      erase();

      locheader();
      printparty();

      move(8,60);
      addstr("Buyer: ");
      addstr(activesquad->squad[buyer]->name);
      
      if(in_gunshop==2)
      {
         if(year<2100)
         {
            if(funds>=25)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("P - Buy Shotgun Shells        ($25)");
            if(funds>=25)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,40);
            addstr("T - Buy a .22 Speedloader     ($25)");
            if(funds>=40)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("N - Buy a 9mm Pistol Magazine ($40)");
            if(funds>=40)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,40);
            addstr("F - Buy a .45 Pistol Magazine ($40)");
            if(funds>=40)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("M - Buy a .44 Speedloader     ($40)");
            if(funds>=50)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,40);
            addstr("S - Buy a 9mm SMG Magazine    ($50)");
            if(funds>=50)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(13,1);
            addstr("R - Buy an Assault Rifle Mag  ($50)");
         }
         else
         {
            if(funds>=25)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("P - Buy a Shotgun Plasma Pack ($25)");
            if(funds>=25)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,40);
            addstr("T - Buy a .22 Slug Magazine   ($25)");
            if(funds>=40)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("N - Buy a 9mm Powerpack       ($40)");
            if(funds>=40)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,40);
            addstr("F - Buy a .45 Powerpack       ($40)");
            if(funds>=40)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("M - Buy a .44 Heavy Slug Mag  ($40)");
            if(funds>=50)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,40);
            addstr("S - Buy a SMG Powerpack       ($50)");
            if(funds>=50)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(13,1);
            addstr("R - Buy a Rifle Powerpack     ($50)");
         }
         

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Done buying Liberal clips");
      }
      else if(in_gunshop==1)
      {
         if(year<2100)
         {
            if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("G - Buy a Shotgun              ($400)");
            if(funds>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,40);
            addstr("T - Buy a .22 Revolver         ($150)");
            if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("N - Buy a 9mm Semi-automatic   ($350)");
            if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,40);
            addstr("F - Buy a .45 Semi-automatic   ($350)");
            if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("M - Buy a .44 Magnum           ($350)");
            if(funds>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,40);
            addstr("S - Buy a 9mm MP5 SMG         ($2000)");
            if(funds>=2800)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(13,1);
            addstr("K - Buy an AK-47              ($2800)");
         }
         else
         {
            if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("G - Buy a Plasma Shotgun       ($400)");
            if(funds>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,40);
            addstr("T - Buy a .22 Slug Pistol      ($150)");
            if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("N - Buy a 9mm Laser Pistol     ($350)");
            if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,40);
            addstr("F - Buy a .45 Laser Pistol     ($350)");
            if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("M - Buy a .44 Slug Pistol      ($350)");
            if(funds>=2000)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,40);
            addstr("S - Buy a Beijing Prince SMG  ($2000)");
            if(funds>=2800)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(13,1);
            addstr("K - Buy a P74 War Laser       ($2800)");
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Done buying Liberal guns");
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,1);
         addstr("G - Buy a Liberal gun");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,40);
         addstr("C - Buy Liberal clips");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(11,1);
         addstr("E - Look over Equipment");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Leave");
      }

      if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(16,1);
      addstr("B - Choose a buyer");

      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,1);
      addstr("0 - Show the squad's Liberal status");
      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,40);
      addstr("# - Check the status of a squad Liberal");

      int c=getch();
      translategetch(c);
      if(in_gunshop==1)
      {
         int gunbought=-1;

         if(funds>=150&&c=='t')
         {
            gunbought=WEAPON_REVOLVER_22;
            funds-=150;
            stat_spent+=150;
            moneylost_goods+=150;
         }
         if(funds>=350&&c=='m')
         {
            gunbought=WEAPON_REVOLVER_44;
            funds-=350;
            stat_spent+=350;
            moneylost_goods+=350;
         }
         if(funds>=350&&c=='n')
         {
            gunbought=WEAPON_SEMIPISTOL_9MM;
            funds-=350;
            stat_spent+=350;
            moneylost_goods+=350;
         }
         if(funds>=350&&c=='f')
         {
            gunbought=WEAPON_SEMIPISTOL_45;
            funds-=350;
            stat_spent+=350;
            moneylost_goods+=350;
         }
         if(funds>=400&&c=='g')
         {
            gunbought=WEAPON_SHOTGUN_PUMP;
            funds-=400;
            stat_spent+=400;
            moneylost_goods+=400;
         }
         if(funds>=2000&&c=='s')
         {
            gunbought=WEAPON_SMG_MP5;
            funds-=2000;
            stat_spent+=2000;
            moneylost_goods+=2000;
         }
         if(funds>=2800&&c=='k')
         {
            gunbought=WEAPON_AUTORIFLE_AK47;
            funds-=2800;
            stat_spent+=2800;
            moneylost_goods+=2800;
         }


         if(gunbought!=-1)
         {
            weaponst swap=activesquad->squad[buyer]->weapon;
            activesquad->squad[buyer]->weapon.type=gunbought;
            activesquad->squad[buyer]->weapon.ammo=0;

            if(swap.type!=WEAPON_NONE)
            {
               itemst *newi=new itemst;
                  newi->type=ITEM_WEAPON;
                  newi->weapon=swap;
               activesquad->loot.push_back(newi);
            }

            //DROP ALL CLIPS THAT DON'T WORK
            for(int cl=0;cl<CLIPNUM;cl++)
            {
               if(cl==ammotype(activesquad->squad[buyer]->weapon.type))continue;

               for(int p2=0;p2<activesquad->squad[buyer]->clip[cl];p2++)
               {
                  itemst *newi=new itemst;
                     newi->type=ITEM_CLIP;
                     newi->cliptype=cl;
                  activesquad->loot.push_back(newi);
               }

               activesquad->squad[buyer]->clip[cl]=0;
            }
         }

         if(c==10)in_gunshop=0;
      }
      else if(in_gunshop==2)
      {
         int clipbought=-1;

         if(funds>=25&&c=='t')
         {
            clipbought=CLIP_22;
            funds-=25;
            stat_spent+=25;
            moneylost_goods+=25;
         }
         if(funds>=40&&c=='m')
         {
            clipbought=CLIP_44;
            funds-=20;
            stat_spent+=20;
            moneylost_goods+=20;
         }
         if(funds>=40&&c=='n')
         {
            clipbought=CLIP_9;
            funds-=30;
            stat_spent+=30;
            moneylost_goods+=30;
         }
         if(funds>=40&&c=='f')
         {
            clipbought=CLIP_45;
            funds-=30;
            stat_spent+=30;
            moneylost_goods+=30;
         }
         if(funds>=25&&c=='p')
         {
            clipbought=CLIP_BUCKSHOT;
            funds-=25;
            stat_spent+=25;
            moneylost_goods+=25;
         }
         if(funds>=50&&c=='r')
         {
            clipbought=CLIP_ASSAULT;
            funds-=50;
            stat_spent+=50;
            moneylost_goods+=50;
         }
         if(funds>=50&&c=='s')
         {
            clipbought=CLIP_SMG;
            funds-=50;
            stat_spent+=50;
            moneylost_goods+=50;
         }

         char conf=1;

         if(ammotype(activesquad->squad[buyer]->weapon.type)==clipbought&&clipbought!=-1)
         {
            if(activesquad->squad[buyer]->clip[clipbought]<9)
            {
               activesquad->squad[buyer]->clip[clipbought]++;
               conf=0;
            }
         }

         if(conf&&clipbought!=-1)
         {
            itemst *newi=new itemst;
               newi->type=ITEM_CLIP;
               newi->cliptype=clipbought;
            activesquad->loot.push_back(newi);
         }

         if(c==10)in_gunshop=0;
      }
      else
      {
         if(c==10)break;
         if(c=='c')in_gunshop=2;
         if(c=='g')in_gunshop=1;
         if(c=='e')equip(activesquad->loot,-1);
      }

      if(c=='b')choose_buyer(buyer);

      if(c=='0')party_status=-1;

      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);
}



/* active squad visits the pawn shop */
void pawnshop(int loc)
{
   short buyer=0;
   short in_tools=0;
   short in_gunshop=0;
   short in_fence=0;
   int l;

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      erase();

      locheader();
      printparty();

      move(8,60);
      addstr("Buyer: ");
      addstr(activesquad->squad[buyer]->name);

      if(in_fence==1)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,1);
         addstr("E - Look over Equipment");

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("F - Pawn Selectively");

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("P - Pawn all Weapons");

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("R - Pawn all Armor");

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,1);
         addstr("W - Pawn all extra Weapons");

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,40);
         addstr("A - Pawn all extra Armor");

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("C - Pawn all extra Clips");

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,40);
         addstr("L - Pawn Loot");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Done pawning");
      }
      else if(in_gunshop==2)
      {
         if(year<2100)
         {
            if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            if(law[LAW_GUNCONTROL]<2)
            addstr("P - Buy Shotgun Shells        ($20)");
            if(funds>=15)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,40);
            if(law[LAW_GUNCONTROL]<2)
            addstr("T - Buy a .22 Speedloader     ($15)");
            if(funds>=30)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            if(law[LAW_GUNCONTROL]<1)
            addstr("N - Buy a 9mm Pistol Magazine ($30)");
            if(funds>=30)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,40);
            if(law[LAW_GUNCONTROL]<1)
            addstr("F - Buy a .45 Pistol Magazine ($30)");
            if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            if(law[LAW_GUNCONTROL]<1)
               addstr("M - Buy a .44 Speedloader     ($20)");
            if(funds>=35)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,40);
            if(law[LAW_GUNCONTROL]<0)
               addstr("R - Buy a Rifle/Aslt.Rifle Mag($35)");
            if(funds>=35)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(13,1);
            if(law[LAW_GUNCONTROL]<-1)
               addstr("S - Buy a 9mm SMG Magazine    ($35)");
         }
         else
         {
            if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            if(law[LAW_GUNCONTROL]<2)
            addstr("P - Buy a Shotgun Plasma Pack ($20)");
            if(funds>=15)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,40);
            if(law[LAW_GUNCONTROL]<2)
            addstr("T - Buy a .22 Slug Magazine   ($15)");
            if(funds>=30)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            if(law[LAW_GUNCONTROL]<1)
            addstr("N - Buy a 9mm Powerpack       ($30)");
            if(funds>=30)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,40);
            if(law[LAW_GUNCONTROL]<1)
            addstr("F - Buy a .45 Powerpack       ($30)");
            if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            if(law[LAW_GUNCONTROL]<1)
               addstr("M - Buy a .44 Heavy Slug Mag  ($20)");
            if(funds>=35)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,40);
            if(law[LAW_GUNCONTROL]<0)
               addstr("R - Buy a Rifle Powerpack     ($35)");
            if(funds>=35)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(13,1);
            if(law[LAW_GUNCONTROL]<-1)
               addstr("S - Buy a SMG Powerpack       ($35)");
         }
         

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Done buying Liberal clips");
      }
      else if(in_gunshop==1)
      {
         if(year<2100)
         {
            if(law[LAW_GUNCONTROL]<2)
            {
               if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(10,1);
               addstr("G - Buy a Shotgun             ($400)");
               if(funds>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(10,40);
               addstr("T - Buy a .22 Revolver        ($150)");
            }
            if(law[LAW_GUNCONTROL]<1)
            {
               if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(11,1);
               addstr("N - Buy a 9mm Semi-automatic  ($300)");
               if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(11,40);
               addstr("F - Buy a .45 Semi-automatic  ($300)");
               if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(12,1);
               addstr("M - Buy a .44 Magnum          ($300)");
            }
            if(law[LAW_GUNCONTROL]<0)
            {
               if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(12,40);
               addstr("R - Buy a Civilian AR-15      ($350)");
            }
            if(law[LAW_GUNCONTROL]==-2)
            {
               if(funds>=1200)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(13,1);
               addstr("S - Buy a 9mm MP5 SMG         ($1200)");
               if(funds>=1400)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(13,40);
               addstr("C - Buy an M4 Carbine         ($1400)");
               if(funds>=1500)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(14,1);
               addstr("A - Buy an M16                ($1500)");
               if(funds>=1500)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(14,40);
               addstr("K - Buy an AK-47              ($1500)");
            }
         }
         else
         {
            if(law[LAW_GUNCONTROL]<2)
            {
               if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(10,1);
               addstr("G - Buy a Plasma Shotgun      ($400)");
               if(funds>=150)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(10,40);
               addstr("T - Buy a .22 Slug Pistol     ($150)");
            }
            if(law[LAW_GUNCONTROL]<1)
            {
               if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(11,1);
               addstr("N - Buy a 9mm Laser Pistol    ($300)");
               if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(11,40);
               addstr("F - Buy a .45 Laser Pistol    ($300)");
               if(funds>=300)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(12,1);
               addstr("M - Buy a .44 Slug Pistol     ($300)");
            }
            if(law[LAW_GUNCONTROL]<0)
            {
               if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(12,40);
               addstr("R - Buy a Personal X-15 Rifle ($350)");
            }
            if(law[LAW_GUNCONTROL]==-2)
            {
               if(funds>=1200)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(13,1);
               addstr("S - Buy a Beijing Prince SMG  ($1200)");
               if(funds>=1400)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(13,40);
               addstr("C - Buy an X4 Laser Carbine   ($1400)");
               if(funds>=1500)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(14,1);
               addstr("A - Buy an Army X-15 Rifle    ($1500)");
               if(funds>=1500)set_color(COLOR_WHITE,COLOR_BLACK,0);
               else set_color(COLOR_BLACK,COLOR_BLACK,1);
               move(14,40);
               addstr("K - Buy a P74 War Laser       ($1500)");
            }
         }
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Done buying Liberal guns");
      }
      else if(in_tools==1)
      {
         if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,1);
         addstr("C - Buy a Crowbar              ($20)");

         if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("S - Buy Spray Paint            ($20)");

         if(funds>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("G - Buy a Guitar              ($200)");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Leave");
      }
      else
      {
         if(law[LAW_GUNCONTROL]<2)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,1);
         addstr("G - Buy a Liberal gun");

         if(law[LAW_GUNCONTROL]<2)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("C - Buy Liberal clips");

         move(11,1);
         addstr("T - Buy Liberal tools");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(12,1);
         addstr("E - Look over Equipment");

         set_color(COLOR_WHITE,COLOR_BLACK,0);

         if(activesquad->loot.size()>0)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("S - Sell something");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(16,40);
         addstr("Enter - Leave");
      }

      if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(16,1);
      addstr("B - Choose a buyer");

      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,1);
      addstr("0 - Show the squad's Liberal status");
      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,40);
      addstr("# - Check the status of a squad Liberal");

      int c=getch();
      translategetch(c);

      if(in_fence)
      {
         if(c==10)in_fence=0;

         if(c=='e')equip(activesquad->loot,-1);

         if((c=='p'||c=='r'||c=='w'||c=='a'||c=='l'||c=='f')&&
            activesquad->loot.size()>0)
         {
            unsigned long fenceamount=0;

            if(c=='f')fenceamount=fenceselect();
            else
            {
               int fenceweapon[WEAPONNUM];
               int fencearmor[ARMORNUM];
               int fenceclip[CLIPNUM];
               int fenceloot[LOOTNUM];

               memset(fenceweapon,0,WEAPONNUM*sizeof(int));
               memset(fencearmor,0,ARMORNUM*sizeof(int));
               memset(fenceclip,0,CLIPNUM*sizeof(int));
               memset(fenceloot,0,LOOTNUM*sizeof(int));

               for(l=activesquad->loot.size()-1;l>=0;l--)
               {
                  switch(activesquad->loot[l]->type)
                  {
                     case ITEM_WEAPON:fenceweapon[activesquad->loot[l]->weapon.type]+=activesquad->loot[l]->number;break;
                     case ITEM_ARMOR:
                        if(activesquad->loot[l]->armor.quality!='1')break;
                        if(activesquad->loot[l]->armor.flag!=0)break;
                        fencearmor[activesquad->loot[l]->armor.type]+=activesquad->loot[l]->number;
                        break;
                     case ITEM_CLIP:fenceclip[activesquad->loot[l]->cliptype]+=activesquad->loot[l]->number;break;
                     case ITEM_LOOT:fenceloot[activesquad->loot[l]->loottype]+=activesquad->loot[l]->number;break;
                  }
               }

               if(c=='l'||c=='a'||c=='r'||c=='c')memset(fenceweapon,0,WEAPONNUM*sizeof(int));
               else if(c=='w')
               {
                  for(int w=0;w<WEAPONNUM;w++)
                  {
                     if(fenceweapon[w]>6)fenceweapon[w]-=6;
                     else fenceweapon[w]=0;
                  }
               }

               if(c=='l'||c=='p'||c=='w'||c=='c')memset(fencearmor,0,ARMORNUM*sizeof(int));
               else if(c=='a')
               {
                  for(int a=0;a<ARMORNUM;a++)
                  {
                     if(fencearmor[a]>6)fencearmor[a]-=6;
                     else fencearmor[a]=0;
                  }
               }

               if(c!='c')memset(fenceclip,0,CLIPNUM*sizeof(int));
               else
               {
                  for(int c=0;c<CLIPNUM;c++)
                  {
                     if(fenceclip[c]>54)fenceclip[c]-=54;
                     else fenceclip[c]=0;
                  }
               }

               if(c!='l')memset(fenceloot,0,LOOTNUM*sizeof(int));
               else
               {
                  fenceloot[LOOT_CEOPHOTOS]=0;
                  fenceloot[LOOT_INTHQDISK]=0;
                  fenceloot[LOOT_CORPFILES]=0;
                  fenceloot[LOOT_SECRETDOCUMENTS]=0;
                  fenceloot[LOOT_POLICERECORDS]=0;
               }

               for(l=activesquad->loot.size()-1;l>=0;l--)
               {
                  switch(activesquad->loot[l]->type)
                  {
                     case ITEM_WEAPON:
                        if(fenceweapon[activesquad->loot[l]->weapon.type]>0)
                        {
                           long numbersold=fenceweapon[activesquad->loot[l]->weapon.type];
                           if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
                           fenceweapon[activesquad->loot[l]->weapon.type]-=numbersold;
                           activesquad->loot[l]->number-=numbersold;
                           fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

                           if(activesquad->loot[l]->number==0)
                           {
                              delete activesquad->loot[l];
                              activesquad->loot.erase(activesquad->loot.begin() + l);
                           }
                        }
                        break;
                     case ITEM_ARMOR:
                        if(activesquad->loot[l]->armor.quality!='1')break;
                        if(activesquad->loot[l]->armor.flag!=0)break;
                        if(fencearmor[activesquad->loot[l]->armor.type]>0)
                        {
                           long numbersold=fencearmor[activesquad->loot[l]->armor.type];
                           if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
                           fencearmor[activesquad->loot[l]->armor.type]-=numbersold;
                           activesquad->loot[l]->number-=numbersold;
                           fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

                           if(activesquad->loot[l]->number==0)
                           {
                              delete activesquad->loot[l];
                              activesquad->loot.erase(activesquad->loot.begin() + l);
                           }
                        }
                        break;
                     case ITEM_CLIP:
                        if(fenceclip[activesquad->loot[l]->cliptype]>0)
                        {
                           long numbersold=fenceclip[activesquad->loot[l]->cliptype];
                           if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
                           fenceclip[activesquad->loot[l]->cliptype]-=numbersold;
                           activesquad->loot[l]->number-=numbersold;
                           fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

                           if(activesquad->loot[l]->number==0)
                           {
                              delete activesquad->loot[l];
                              activesquad->loot.erase(activesquad->loot.begin() + l);
                           }
                        }
                        break;
                     case ITEM_LOOT:
                        if(fenceloot[activesquad->loot[l]->loottype]>0)
                        {
                           long numbersold=fenceloot[activesquad->loot[l]->loottype];
                           if(numbersold>activesquad->loot[l]->number)numbersold=activesquad->loot[l]->number;
                           fenceloot[activesquad->loot[l]->loottype]-=numbersold;
                           activesquad->loot[l]->number-=numbersold;
                           fenceamount+=fencevalue(*activesquad->loot[l])*numbersold;

                           if(activesquad->loot[l]->number==0)
                           {
                              delete activesquad->loot[l];
                              activesquad->loot.erase(activesquad->loot.begin() + l);
                           }
                        }
                        break;
                  }
               }
            }

            if(fenceamount>0)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);

               char num[20];

               move(8,1);
               addstr("You add $");
               itoa(fenceamount,num,10);
               addstr(num);addstr(" to Liberal Funds.");

               refresh();
               getch();

               funds+=fenceamount;
               stat_funds+=fenceamount;
               moneygained_goods+=fenceamount;
            }
         }
      }
      else if(in_gunshop==1)
      {
         int gunbought=-1;

         if(funds>=150&&c=='t'&&law[LAW_GUNCONTROL]<2)
         {
            gunbought=WEAPON_REVOLVER_22;
            funds-=150;
            stat_spent+=150;
            moneylost_goods+=150;
         }
         if(funds>=300&&c=='m'&&law[LAW_GUNCONTROL]<1)
         {
            gunbought=WEAPON_REVOLVER_44;
            funds-=300;
            stat_spent+=300;
            moneylost_goods+=300;
         }
         if(funds>=300&&c=='n'&&law[LAW_GUNCONTROL]<2)
         {
            gunbought=WEAPON_SEMIPISTOL_9MM;
            funds-=300;
            stat_spent+=300;
            moneylost_goods+=300;
         }
         if(funds>=300&&c=='f'&&law[LAW_GUNCONTROL]<2)
         {
            gunbought=WEAPON_SEMIPISTOL_45;
            funds-=300;
            stat_spent+=300;
            moneylost_goods+=300;
         }
         if(funds>=400&&c=='g' && law[LAW_GUNCONTROL]<2)
         {
            gunbought=WEAPON_SHOTGUN_PUMP;
            funds-=400;
            stat_spent+=400;
            moneylost_goods+=400;
         }
         if(funds>=350&&c=='r'&&law[LAW_GUNCONTROL]<0)
         {
            gunbought=WEAPON_SEMIRIFLE_AR15;
            funds-=350;
            stat_spent+=350;
            moneylost_goods+=350;
         }
         if(funds>=1200&&c=='s'&&law[LAW_GUNCONTROL]==-2)
         {
            gunbought=WEAPON_SMG_MP5;
            funds-=1200;
            stat_spent+=1200;
            moneylost_goods+=1200;
         }
         if(funds>=1400&&c=='c'&&law[LAW_GUNCONTROL]==-2)
         {
            gunbought=WEAPON_CARBINE_M4;
            funds-=1400;
            stat_spent+=1400;
            moneylost_goods+=1400;
         }
         if(funds>=1500&&c=='a'&&law[LAW_GUNCONTROL]==-2)
         {
            gunbought=WEAPON_AUTORIFLE_M16;
            funds-=1500;
            stat_spent+=1500;
            moneylost_goods+=1500;
         }
         if(funds>=1500&&c=='k'&&law[LAW_GUNCONTROL]==-2)
         {
            gunbought=WEAPON_AUTORIFLE_AK47;
            funds-=1500;
            stat_spent+=1500;
            moneylost_goods+=1500;
         }


         if(gunbought!=-1)
         {
            weaponst swap=activesquad->squad[buyer]->weapon;
            activesquad->squad[buyer]->weapon.type=gunbought;
            activesquad->squad[buyer]->weapon.ammo=0;

            if(swap.type!=WEAPON_NONE)
            {
               itemst *newi=new itemst;
                  newi->type=ITEM_WEAPON;
                  newi->weapon=swap;
               activesquad->loot.push_back(newi);
            }

            //DROP ALL CLIPS THAT DON'T WORK
            for(int cl=0;cl<CLIPNUM;cl++)
            {
               if(cl==ammotype(activesquad->squad[buyer]->weapon.type))continue;

               for(int p2=0;p2<activesquad->squad[buyer]->clip[cl];p2++)
               {
                  itemst *newi=new itemst;
                     newi->type=ITEM_CLIP;
                     newi->cliptype=cl;
                  activesquad->loot.push_back(newi);
               }

               activesquad->squad[buyer]->clip[cl]=0;
            }
         }

         if(c==10)in_gunshop=0;
      }
      else if(in_gunshop==2)
      {
         int clipbought=-1;

         if(funds>=15&&c=='t'&&law[LAW_GUNCONTROL]<2)
         {
            clipbought=CLIP_22;
            funds-=15;
            stat_spent+=15;
            moneylost_goods+=15;
         }
         if(funds>=20&&c=='m'&&law[LAW_GUNCONTROL]<1)
         {
            clipbought=CLIP_44;
            funds-=20;
            stat_spent+=20;
            moneylost_goods+=20;
         }
         if(funds>=30&&c=='n'&&law[LAW_GUNCONTROL]<2)
         {
            clipbought=CLIP_9;
            funds-=30;
            stat_spent+=30;
            moneylost_goods+=30;
         }
         if(funds>=30&&c=='f'&&law[LAW_GUNCONTROL]<2)
         {
            clipbought=CLIP_45;
            funds-=30;
            stat_spent+=30;
            moneylost_goods+=30;
         }
         if(funds>=20&&c=='p' && law[LAW_GUNCONTROL]<2)
         {
            clipbought=CLIP_BUCKSHOT;
            funds-=20;
            stat_spent+=20;
            moneylost_goods+=20;
         }
         if(funds>=35&&c=='r'&&law[LAW_GUNCONTROL]<0)
         {
            clipbought=CLIP_ASSAULT;
            funds-=35;
            stat_spent+=35;
            moneylost_goods+=35;
         }
         if(funds>=35&&c=='s'&&law[LAW_GUNCONTROL]==-2)
         {
            clipbought=CLIP_SMG;
            funds-=35;
            stat_spent+=35;
            moneylost_goods+=35;
         }

         char conf=1;

         if(ammotype(activesquad->squad[buyer]->weapon.type)==clipbought&&clipbought!=-1)
         {
            if(activesquad->squad[buyer]->clip[clipbought]<9)
            {
               activesquad->squad[buyer]->clip[clipbought]++;
               conf=0;
            }
         }

         if(conf&&clipbought!=-1)
         {
            itemst *newi=new itemst;
               newi->type=ITEM_CLIP;
               newi->cliptype=clipbought;
            activesquad->loot.push_back(newi);
         }

         if(c==10)in_gunshop=0;
      }
      else if(in_tools==1)
      {
         if(c=='c'&&funds>=20)
         {
            funds-=20;
            stat_spent+=20;
            moneylost_goods+=20;

            itemst *newi=new itemst;
               newi->type=ITEM_WEAPON;
               newi->weapon.type=WEAPON_CROWBAR;
            activesquad->loot.push_back(newi);
         }
         if(c=='s'&&funds>=20)
         {
            funds-=20;
            stat_spent+=20;
            moneylost_goods+=20;

            itemst *newi=new itemst;
               newi->type=ITEM_WEAPON;
               newi->weapon.type=WEAPON_SPRAYCAN;
            activesquad->loot.push_back(newi);
         }
         if(c=='g'&&funds>=200)
         {
            funds-=200;
            stat_spent+=200;
            moneylost_goods+=200;

            itemst *newi=new itemst;
               newi->type=ITEM_WEAPON;
               newi->weapon.type=WEAPON_GUITAR;
            activesquad->loot.push_back(newi);
         }
         if(c==10)in_tools=0;
      }
      else
      {
         if(c==10)break;
         if(c=='c' && law[LAW_GUNCONTROL]<2)in_gunshop=2;
         if(c=='g' && law[LAW_GUNCONTROL]<2)in_gunshop=1;
         if(c=='e')equip(activesquad->loot,-1);
         if(c=='s')in_fence=1;
         if(c=='t')in_tools=1;
      }

      if(c=='b')choose_buyer(buyer);

      if(c=='0')party_status=-1;

      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);
}



/* active squad visits the department store */
void deptstore(int loc)
{
   short buyer=0;

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      int weaponbought=-1;
      int armorbought=-1;

      erase();

      locheader();
      printparty();

      move(8,60);
      addstr("Buyer: ");
      addstr(activesquad->squad[buyer]->name);

      if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(10,1);
      addstr("C - Buy a Cheap Suit         ($400)");

      if(funds>=5000)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,1);
      addstr("V - Buy an Expensive Suit   ($5000)");

      if(funds>=500)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,1);
      addstr("S - Buy a Black Suit         ($500)");

      if(funds>=400)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(10,40);
      addstr("H - Buy a Cheap Dress        ($400)");

      if(funds>=5000)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(11,40);
      addstr("D - Buy an Expensive Dress  ($5000)");

      if(funds>=500)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(12,40);
      addstr("R - Buy a Black Dress        ($500)");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(15,1);
      addstr("E - Look over Equipment");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(15,30);
      addstr("Enter - Leave");
      if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,50);
      addstr("B - Choose a buyer");
      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("0 - Show the squad's Liberal status");
      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,40);
      addstr("# - Check the status of a squad Liberal");

      int c=getch();
      translategetch(c);

      if(c==10)break;

      if(funds>=400&&c=='c')
      {
         armorbought=ARMOR_CHEAPSUIT;
         funds-=400;
         stat_spent+=400;
         moneylost_goods+=400;
      }
      if(funds>=5000&&c=='v')
      {
         armorbought=ARMOR_EXPENSIVESUIT;
         funds-=5000;
         stat_spent+=5000;
         moneylost_goods+=5000;
      }
      if(funds>=500&&c=='s')
      {
         armorbought=ARMOR_BLACKSUIT;
         funds-=500;
         stat_spent+=500;
         moneylost_goods+=500;
      }
      if(funds>=400&&c=='h')
      {
         armorbought=ARMOR_CHEAPDRESS;
         funds-=400;
         stat_spent+=400;
         moneylost_goods+=400;
      }
      if(funds>=5000&&c=='d')
      {
         armorbought=ARMOR_EXPENSIVEDRESS;
         funds-=5000;
         stat_spent+=5000;
         moneylost_goods+=5000;
      }
      if(funds>=500&&c=='r')
      {
         armorbought=ARMOR_BLACKDRESS;
         funds-=500;
         stat_spent+=500;
         moneylost_goods+=500;
      }

      if(armorbought!=-1)
      {
         armorst swap=activesquad->squad[buyer]->armor;
         activesquad->squad[buyer]->armor.type=armorbought;
         activesquad->squad[buyer]->armor.flag=0;
         activesquad->squad[buyer]->armor.quality='1';

         if(swap.type!=ARMOR_NONE)
         {
            itemst *newi=new itemst;
               newi->type=ITEM_ARMOR;
               newi->armor=swap;
            activesquad->loot.push_back(newi);
         }
      }
      if(weaponbought!=-1)
      {
         weaponst swap=activesquad->squad[buyer]->weapon;
         activesquad->squad[buyer]->weapon.type=weaponbought;
         activesquad->squad[buyer]->weapon.ammo=0;

         if(swap.type!=WEAPON_NONE)
         {
            itemst *newi=new itemst;
               newi->type=ITEM_WEAPON;
               newi->weapon=swap;
            activesquad->loot.push_back(newi);
         }

         //DROP ALL CLIPS THAT DON'T WORK
         for(int cl=0;cl<CLIPNUM;cl++)
         {
            if(cl==ammotype(activesquad->squad[buyer]->weapon.type))continue;

            for(int p2=0;p2<activesquad->squad[buyer]->clip[cl];p2++)
            {
               itemst *newi=new itemst;
                  newi->type=ITEM_CLIP;
                  newi->cliptype=cl;
               activesquad->loot.push_back(newi);
            }

            activesquad->squad[buyer]->clip[cl]=0;
         }
      }

      if(c=='e')equip(activesquad->loot,-1);
      if(c=='b')choose_buyer(buyer);
      if(c=='0')party_status=-1;
      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);
}



/* active squad visits the oubliette */
void halloweenstore(int loc)
{
   short buyer=0;
   short in_halloween=0;

   locatesquad(activesquad,loc);

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)
      {
         partysize++;
      }
   }

   do
   {
      int weaponbought=-1;
      int armorbought=-1,armorbought2=-1;

      erase();

      locheader();
      printparty();

      move(8,60);
      addstr("Buyer: ");
      addstr(activesquad->squad[buyer]->name);

      if(in_halloween==1)
      {
         if(funds>=70)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,1);
         addstr("T - Buy a Trench Coat         ($70)");

         if(funds>=50)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,1);
         addstr("W - Buy Work Clothes          ($50)");

         if(funds>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,1);
         addstr("L - Buy a Lab Coat           ($200)");

         if(funds>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("R - Buy a Black Judge's Robe ($200)");

         if(funds>=200)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("C - Buy a Clown Suit         ($200)");

         if(funds>=350)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("G - Buy Bondage Gear         ($350)");

         if(funds>=15)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,40);
         addstr("M - Buy a Mask                ($15)");

         if(funds>=90)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,40);
         addstr("O - Buy a Toga                ($90)");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,30);
         addstr("Enter - Done");
      }
      else if(in_halloween==2)
      {
         if(year<2100)
         {
            if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("K - Buy a Knife                 ($20)");

            if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("S - Buy the Sword of Morfiegor ($250)");

            if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("A - Buy a Katana and Wakizashi ($250)");
         }
         else
         {
            if(funds>=20)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,1);
            addstr("K - Buy a Vibro-Knife          ($20)");

            if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(11,1);
            addstr("S - Buy a Light Sword          ($250)");

            if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
            else set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(12,1);
            addstr("A - Buy the Liberal Twin Swords($250)");
         }

         if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,1);
         addstr("H - Buy a Dwarven Hammer       ($250)");

         if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("M - Buy the Maul of Anrin      ($250)");

         if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("C - Buy a Silver Cross         ($250)");

         if(funds>=250)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(12,40);
         addstr("W - Buy a Wizard's Staff       ($250)");

         if(funds>=1000)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(13,40);
         addstr("! - Buy Mithril Mail          ($1000)");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,30);
         addstr("Enter - Done");
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(10,1);
         addstr("C - Purchase Halloween Costumes");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(11,1);
         addstr("M - Purchase Medieval Gear");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(15,30);
         addstr("Enter - Leave");
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(15,1);
      addstr("E - Look over Equipment");
      if(partysize>=2)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(15,50);
      addstr("B - Choose a buyer");
      if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,1);
      addstr("0 - Show the squad's Liberal status");
      if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
      else set_color(COLOR_BLACK,COLOR_BLACK,1);
      move(14,40);
      addstr("# - Check the status of a squad Liberal");

      int c=getch();
      translategetch(c);

      if(in_halloween==1)
      {
         if(c==10)in_halloween=0;

         if(c=='t'&&funds>=70)
         {
            armorbought=ARMOR_TRENCHCOAT;
            funds-=70;
            stat_spent+=70;
            moneylost_goods+=70;
         }

         if(c=='w'&&funds>=50)
         {
            armorbought=ARMOR_WORKCLOTHES;
            funds-=50;
            stat_spent+=50;
            moneylost_goods+=50;
         }

         if(c=='l'&&funds>=200)
         {
            armorbought=ARMOR_LABCOAT;
            funds-=200;
            stat_spent+=200;
            moneylost_goods+=200;
         }

         if(c=='r'&&funds>=200)
         {
            armorbought=ARMOR_BLACKROBE;
            funds-=200;
            stat_spent+=200;
            moneylost_goods+=200;
         }

         if(c=='c'&&funds>=200)
         {
            armorbought=ARMOR_CLOWNSUIT;
            funds-=200;
            stat_spent+=200;
            moneylost_goods+=200;
         }

         if(c=='g'&&funds>=350)
         {
            armorbought=ARMOR_BONDAGEGEAR;
            funds-=350;
            stat_spent+=350;
            moneylost_goods+=350;
         }

         if(c=='m'&&funds>=15)
         {
            short mask;
            if(maskselect(activesquad->squad[buyer],mask))
            {
               armorbought=ARMOR_MASK;
               armorbought2=mask;
               funds-=15;
               stat_spent+=15;
               moneylost_goods+=15;
            }
         }

         if(c=='o'&&funds>=90)
         {
            armorbought=ARMOR_TOGA;
            funds-=90;
            stat_spent+=90;
            moneylost_goods+=90;
         }
      }
      else if(in_halloween==2)
      {
         if(c==10)in_halloween=0;

         if(c=='k'&&funds>=20)
         {
            weaponbought=WEAPON_KNIFE;
            funds-=20;
            stat_spent+=20;
            moneylost_goods+=20;
         }

         if(c=='s'&&funds>=250)
         {
            weaponbought=WEAPON_SWORD;
            funds-=250;
            stat_spent+=250;
            moneylost_goods+=250;
         }

         if(c=='a'&&funds>=250)
         {
            weaponbought=WEAPON_DAISHO;
            funds-=250;
            stat_spent+=250;
            moneylost_goods+=250;
         }

         if(c=='h'&&funds>=250)
         {
            weaponbought=WEAPON_HAMMER;
            funds-=250;
            stat_spent+=250;
            moneylost_goods+=250;
         }

         if(c=='m'&&funds>=250)
         {
            weaponbought=WEAPON_MAUL;
            funds-=250;
            stat_spent+=250;
            moneylost_goods+=250;
         }

         if(c=='c'&&funds>=250)
         {
            weaponbought=WEAPON_CROSS;
            funds-=250;
            stat_spent+=250;
            moneylost_goods+=250;
         }

         if(c=='w'&&funds>=250)
         {
            weaponbought=WEAPON_STAFF;
            funds-=250;
            stat_spent+=250;
            moneylost_goods+=250;
         }

         if(c=='!'&&funds>=1000)
         {
            armorbought=ARMOR_MITHRIL;
            funds-=1000;
            stat_spent+=1000;
            moneylost_goods+=1000;
         }
      }
      else
      {
         if(c==10)break;
         if(c=='c')in_halloween=1;
         if(c=='m')in_halloween=2;
      }

      if(armorbought!=-1)
      {
         armorst swap=activesquad->squad[buyer]->armor;
         activesquad->squad[buyer]->armor.type=armorbought;
         activesquad->squad[buyer]->armor.subtype=armorbought2;
         activesquad->squad[buyer]->armor.flag=0;
         activesquad->squad[buyer]->armor.quality='1';

         if(swap.type!=ARMOR_NONE)
         {
            itemst *newi=new itemst;
               newi->type=ITEM_ARMOR;
               newi->armor=swap;
            activesquad->loot.push_back(newi);
         }
      }
      if(weaponbought!=-1)
      {
         weaponst swap=activesquad->squad[buyer]->weapon;
         activesquad->squad[buyer]->weapon.type=weaponbought;
         activesquad->squad[buyer]->weapon.ammo=0;

         if(swap.type!=WEAPON_NONE)
         {
            itemst *newi=new itemst;
               newi->type=ITEM_WEAPON;
               newi->weapon=swap;
            activesquad->loot.push_back(newi);
         }

         //DROP ALL CLIPS THAT DON'T WORK
         for(int cl=0;cl<CLIPNUM;cl++)
         {
            if(cl==ammotype(activesquad->squad[buyer]->weapon.type))continue;

            for(int p2=0;p2<activesquad->squad[buyer]->clip[cl];p2++)
            {
               itemst *newi=new itemst;
                  newi->type=ITEM_CLIP;
                  newi->cliptype=cl;
               activesquad->loot.push_back(newi);
            }

            activesquad->squad[buyer]->clip[cl]=0;
         }
      }

      if(c=='e')equip(activesquad->loot,-1);
      if(c=='b')choose_buyer(buyer);
      if(c=='0')party_status=-1;
      if(c>='1'&&c<='6'&&activesquad!=NULL)
      {
         if(activesquad->squad[c-'1']!=NULL)
         {
            if(party_status==c-'1')fullstatus(party_status);
            else party_status=c-'1';
         }
      }

   }while(1);
}



/* oubliette - buy a mask */
char maskselect(creaturest *cr,short &mask)
{
   mask=-1;

   vector<int> masktype;
   for(int a=0;a<MASKNUM;a++)
   {
      switch(a)
      {
         case MASK_JESUS:
         case MASK_COLEMAN_GARY:
         case MASK_MADONNA:
         case MASK_SPEARS:
         case MASK_EMINEM:
         case MASK_AGUILERA:
         case MASK_WAHLBERG:
         case MASK_IGGYPOP:
         case MASK_CASH:
         case MASK_BINLADEN:
         case MASK_LORDS:
         case MASK_SHIELDS:
         case MASK_JACKSON_MICHAEL:
         case MASK_CRUTHERS:
         case MASK_KING_DON:
            break;
         default:
            masktype.push_back(a);
      }
   }

   short page=0;

   char str[200];

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Which mask will ");
      addstr(cr->name);
      addstr(" buy?");
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(1,0);
      addstr("----PRODUCT NAME-----------------------DESCRIPTION------------------------------");

      int y=2;
      for(int p=page*19;p<masktype.size()&&p<page*19+19;p++)
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,0);
         addch(y+'A'-2);addstr(" - ");
         getarmorfull(str,ARMOR_MASK,masktype[p]);
         addstr(str);

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(y,39);
         getmaskdesc(str,masktype[p]);
         addstr(str);

         y++;
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(22,0);
      addstr("Press a Letter to select a Mask");
      move(23,0);
      addpagestr();
      move(24,0);
      addstr("Z - Surprise ");
      addstr(cr->name);
      addstr(" With a Random Mask");

      refresh();

      int c=getch();
      translategetch(c);

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*19<masktype.size())page++;

      if(c>='a'&&c<='s')
      {
         int p=page*19+(int)(c-'a');
         if(p<masktype.size())
         {
            mask=masktype[p];
            return 1;
         }
      }
      if(c=='z')
      {
         mask=LCSrandom(MASKNUM);
         return 1;
      }

      if(c==10)break;
   }while(1);

   return 0;
}



/* pick stuff to fence */
unsigned long fenceselect(void)
{
   unsigned long ret=0;

   consolidateloot(activesquad->loot);

   int page=0;

   vector<char> selected;
   selected.resize(activesquad->loot.size());
   for(int s=0;s<selected.size();s++)selected[s]=0;

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addstr("What will you sell?");

      if(ret>0)
      {
         char num[20];
         move(0,30);
         addstr("Estimated Liberal Amount: $");
         itoa(ret,num,10);
         addstr(num);
      }

      printparty();

      int x=1,y=10;
      char str[200],str2[200];

      for(int l=page*18;l<activesquad->loot.size()&&l<page*18+18;l++)
      {
         if(selected[l])set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         if(activesquad->loot[l]->type==ITEM_WEAPON)
         {
            getweaponfull(str2,activesquad->loot[l]->weapon.type);
            if(activesquad->loot[l]->weapon.ammo>0)
            {
               char num[20];
               itoa(activesquad->loot[l]->weapon.ammo,num,10);
               strcat(str2," (");
               strcat(str2,num);
               strcat(str2,")");
            }
         }
         if(activesquad->loot[l]->type==ITEM_ARMOR)
         {
            getarmorfull(str2,activesquad->loot[l]->armor,0);
         }
         if(activesquad->loot[l]->type==ITEM_CLIP)
         {
            getclip(str2,activesquad->loot[l]->cliptype);
         }
         if(activesquad->loot[l]->type==ITEM_LOOT)
         {
            getloot(str2,activesquad->loot[l]->loottype);
         }
         if(activesquad->loot[l]->number>1)
         {
            char num[20];
            strcat(str2," ");
            if(selected[l]>0)
            {
               itoa(selected[l],num,10);
               strcat(str2,num);
               strcat(str2,"/");
            }
            else strcat(str2,"x");
            itoa(activesquad->loot[l]->number,num,10);
            strcat(str2,num);
         }

         str[0]=l-page*18+'A';
         str[1]='\x0';
         strcat(str," - ");
         strcat(str,str2);

         move(y,x);
         addstr(str);

         x+=26;
         if(x>53)
         {
            x=1;
            y++;
         }
      }

      //PAGE UP
      set_color(COLOR_WHITE,COLOR_BLACK,0);
      if(page>0)
      {
         move(17,1);
         addprevpagestr();
      }
      //PAGE DOWN
      if((page+1)*18<activesquad->loot.size())
      {
         move(17,53);
         addnextpagestr();
      }

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(23,1);
      addstr("Press a letter to select an item to sell.");
      move(24,1);
      addstr("X - Done");

      refresh();

      int c=getch();
      translategetch(c);

      if(c>='a'&&c<='r')
      {
         int slot=c-'a'+page*18;

         if(slot>=0&&slot<activesquad->loot.size())
         {
            if(selected[slot])
            {
               ret-=fencevalue(*activesquad->loot[slot])*selected[slot];
               selected[slot]=0;
            }
            else
            {
               char bad=0;
               if(activesquad->loot[slot]->type==ITEM_ARMOR)
               {
                  if(activesquad->loot[slot]->armor.quality!='1')bad=1;
                  if(activesquad->loot[slot]->armor.flag!=0)bad=1;
               }

               if(bad)
               {
                  printparty();

                  move(8,15);
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  addstr(" You can't sell damaged or second-rate goods.");

                  refresh();
                  getch();
               }
               else
               {
                  if(activesquad->loot[slot]->number>1)
                  {
                     selected[slot]=1;

                     printparty();

                     move(8,15);
                     set_color(COLOR_WHITE,COLOR_BLACK,1);
                     addstr("       How many?          ");

                     refresh();

                     char str[100];

                     keypad(stdscr,FALSE);
                     raw_output(FALSE);
                     echo();
                     curs_set(1);
                     mvgetstr(8,30,str);
                     curs_set(0);
                     noecho();
                     raw_output(TRUE);
                     keypad(stdscr,TRUE);

                     selected[slot]=atoi(str);
                     if(selected[slot]<0)selected[slot]=0;
                     else if(selected[slot]>activesquad->loot[slot]->number)selected[slot]=activesquad->loot[slot]->number;
                  }
                  else selected[slot]=1;
                  ret+=fencevalue(*activesquad->loot[slot])*selected[slot];
               }
            }
         }
      }

      if(c=='x')break;

      //PAGE UP
      if((c==interface_pgup||c==KEY_UP||c==KEY_LEFT)&&page>0)page--;
      //PAGE DOWN
      if((c==interface_pgdn||c==KEY_DOWN||c==KEY_RIGHT)&&(page+1)*18<activesquad->loot.size())page++;

   }while(1);

   for(int l=activesquad->loot.size()-1;l>=0;l--)
   {
      if(selected[l]>0)
      {
         activesquad->loot[l]->number-=selected[l];
         if(activesquad->loot[l]->number<=0)
         {
            delete activesquad->loot[l];
            activesquad->loot.erase(activesquad->loot.begin() + l);
         }
      }
   }

      return ret;
}



/* value of stuff to fence */
unsigned long fencevalue(itemst &it)
{
   unsigned long fenceamount=0;

   switch(it.type)
   {
      case ITEM_WEAPON:
         switch(it.weapon.type)
         {
            case WEAPON_CROWBAR:fenceamount=10;break;
            case WEAPON_BASEBALLBAT:fenceamount=20;break;
            case WEAPON_KNIFE:fenceamount=10;break;
            case WEAPON_SHANK:fenceamount=5;break;
            case WEAPON_SYRINGE:fenceamount=10;break;
            case WEAPON_REVOLVER_22:fenceamount=50;break;
            case WEAPON_REVOLVER_44:fenceamount=100;break;
            case WEAPON_SEMIPISTOL_9MM:fenceamount=100;break;
            case WEAPON_SEMIPISTOL_45:fenceamount=100;break;
            case WEAPON_SMG_MP5:fenceamount=450;break;
            case WEAPON_SEMIRIFLE_AR15:fenceamount=250;break;
            case WEAPON_AUTORIFLE_M16:fenceamount=500;break;
            case WEAPON_AUTORIFLE_AK47:fenceamount=500;break;
            case WEAPON_CARBINE_M4:fenceamount=400;break;
            case WEAPON_SHOTGUN_PUMP:fenceamount=150;break;
            case WEAPON_DAISHO:fenceamount=100;break;
            case WEAPON_HAMMER:fenceamount=100;break;
            case WEAPON_MAUL:fenceamount=100;break;
            case WEAPON_CROSS:fenceamount=100;break;
            case WEAPON_STAFF:fenceamount=100;break;
            case WEAPON_SWORD:fenceamount=100;break;
            case WEAPON_GUITAR:fenceamount=100;break;
            case WEAPON_CHAIN:fenceamount=10;break;
            case WEAPON_NIGHTSTICK:fenceamount=20;break;
            case WEAPON_GAVEL:fenceamount=20;break;
            case WEAPON_PITCHFORK:fenceamount=20;break;
            case WEAPON_TORCH:fenceamount=2;break;
            case WEAPON_SPRAYCAN:fenceamount=2;break;
         }
         break;
      case ITEM_ARMOR:
         switch(it.armor.type) // *JDS* all armor fence values changed to 2x manufacture cost (without cloth)
         {
            case ARMOR_CLOTHES:fenceamount=20;break;
            case ARMOR_OVERALLS:fenceamount=10;break;
            case ARMOR_WIFEBEATER:fenceamount=4;break;
            case ARMOR_TRENCHCOAT:fenceamount=40;break;
            case ARMOR_WORKCLOTHES:fenceamount=20;break;
            case ARMOR_SECURITYUNIFORM:fenceamount=80;break;
            case ARMOR_POLICEUNIFORM:fenceamount=80;break;
            case ARMOR_CHEAPSUIT:fenceamount=100;break;
            case ARMOR_EXPENSIVESUIT:fenceamount=600;break;
            case ARMOR_BLACKSUIT:fenceamount=120;break;
            case ARMOR_CHEAPDRESS:fenceamount=40;break;
            case ARMOR_EXPENSIVEDRESS:fenceamount=600;break;
            case ARMOR_BLACKDRESS:fenceamount=120;break;
            case ARMOR_LABCOAT:fenceamount=40;break;
            case ARMOR_BLACKROBE:fenceamount=40;break;
            case ARMOR_CLOWNSUIT:fenceamount=40;break;
            case ARMOR_BONDAGEGEAR:fenceamount=60;break;
            case ARMOR_MASK:fenceamount=10;break;
            case ARMOR_MILITARY:fenceamount=80;break;
            case ARMOR_PRISONGUARD:fenceamount=80;break;
            case ARMOR_PRISONER:fenceamount=40;break;
            case ARMOR_TOGA:fenceamount=10;break;
            case ARMOR_MITHRIL:fenceamount=50;break;
            case ARMOR_CIVILLIANARMOR:fenceamount=75;break;
            case ARMOR_POLICEARMOR:fenceamount=150;break;
            case ARMOR_ARMYARMOR:fenceamount=250;break;
            case ARMOR_HEAVYARMOR:fenceamount=400;break;
         }
         break;
      case ITEM_CLIP:
         switch(it.cliptype)
         {
            case CLIP_22:fenceamount=2;break;
            case CLIP_44:fenceamount=3;break;
            case CLIP_45:fenceamount=4;break;
            case CLIP_9:fenceamount=4;break;
            case CLIP_ASSAULT:fenceamount=8;break;
            case CLIP_SMG:fenceamount=5;break;
            case CLIP_BUCKSHOT:fenceamount=3;break;
         }
         break;
      case ITEM_LOOT:
         switch(it.loottype)
         {
            case LOOT_LABEQUIPMENT:fenceamount=50;break;
            case LOOT_COMPUTER:fenceamount=200;break;
            case LOOT_TV:fenceamount=50;break;
            case LOOT_VCR:fenceamount=50;break;
            case LOOT_CELLPHONE:fenceamount=20;break;
            case LOOT_TVCAMERA:fenceamount=50;break;
            case LOOT_BROADCASTINGEQUIPMENT:fenceamount=50;break;
            case LOOT_SILVERWARE:fenceamount=20;break;
            case LOOT_SCANNER:fenceamount=50;break;
            case LOOT_PRINTER:fenceamount=50;break;
            case LOOT_FINECLOTH:fenceamount=20;break;
            case LOOT_CHEMICAL:fenceamount=20;break;
            case LOOT_CEOPHOTOS:fenceamount=1000;break;
            case LOOT_SECRETDOCUMENTS:fenceamount=1500;break;
            case LOOT_INTHQDISK:fenceamount=1500;break;
            case LOOT_CORPFILES:fenceamount=1000;break;
            case LOOT_POLICERECORDS:fenceamount=750;break;
         }
         break;
   }

   return fenceamount;
}



/* choose buyer */
void choose_buyer(short &buyer)
{
   if(activesquad==NULL)return;

   party_status=-1;

   int partysize=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]!=NULL)partysize++;
   }

   if(partysize<=1)return;

   do
   {
      printparty();

      move(8,20);
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      addstr("Choose a Liberal squad member to SPEND.");

      refresh();

      int c=getch();
      translategetch(c);

      if(c==10)return;

      if(c>='1'&&c<=partysize+'1'-1)
      {
         buyer=c-'1';
         return;
      }
   }while(1);
}
