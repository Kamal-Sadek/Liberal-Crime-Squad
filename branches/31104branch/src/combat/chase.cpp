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



char chasesequence(void)
{
   int p = 0;

   reloadparty();

   //NOTE: THIS FUNCTION RETURNS 1 IF ANYBODY ESCAPES
      //IT CAN DELETE CREATURES AND VEHICLES, BUT SHOULD NOT DELETE SQUADS
         //AS SQUADS MAY BE FICTITIOUS AND WILL BE DELETED LATER ANYWAY

   //BAIL IF NO CHASERS
   int chasenum=0;
   int v2=0;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].exists)chasenum++;
   }
   if(chasenum==0)return 1;

   chaseseq.friendcar.clear();
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;

      if(activesquad->squad[p]->carid!=-1)
      {
         long v=id_getcar(activesquad->squad[p]->carid);
         if(v!=-1)
         {
            for(v2=0;v2<chaseseq.friendcar.size();v2++)
            {
               if(chaseseq.friendcar[v2]->id==vehicle[v]->id)break;
            }
            if(v2==chaseseq.friendcar.size())chaseseq.friendcar.push_back(vehicle[v]);
         }
      }
   }

   mode=GAMEMODE_CHASECAR;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("As you pull away from the site, you notice that you are");
   move(1,0);
   addstr("being followed by Conservative swine!");
   refresh();
   getch();

   if(location[chaseseq.location]->parent!=-1)
   {
      chaseseq.location=location[chaseseq.location]->parent;
   }

   short obstacle=-1;

   do
   {
      int partysize=0;
      int partyalive=0;
      for(p=0;p<6;p++)
      {
         if(activesquad->squad[p]!=NULL)partysize++;
         else continue;

         if(activesquad->squad[p]->alive==1)partyalive++;
      }
      int encsize=0;
      for(int e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists)
         {
            encsize++;
         }
      }

      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addlocationname(location[chaseseq.location]);

      //PRINT PARTY
      if(partyalive==0)party_status=-1;
      printparty();

      if(partyalive>0)
      {
         //PRINT DRIVING SITUATION AND INSTRUCTIONS
         if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(9,40);
         addstr("O - Change the squad's Liberal order");
         if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("# - Check the status of a squad Liberal");
         if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("0 - Show the squad's Liberal status");

         int y=12;
         if(obstacle==-1)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            move(9,1);
            addstr("D - Try to lose them!");
            move(10,1);
            addstr("E - Equip");
            move(11,1);
            addstr("F - Fight!");
         }
         else
         {
            switch(obstacle)
            {
               case CARCHASE_OBSTACLE_FRUITSTAND:
                  set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                  move(9,1);
                  //LIMIT :------------------------------------:
                  addstr("You are speeding toward a fruit-stand!");
                  move(10,1);
                  addstr("D - Evasive driving!");
                  move(11,1);
                  addstr("F - Plow through it!");
                  break;
               case CARCHASE_OBSTACLE_TRUCKPULLSOUT:
                  set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                  move(9,1);
                  //LIMIT :------------------------------------:
                  addstr("A truck pulls out in your path!");
                  move(10,1);
                  addstr("D - Speed around it!");
                  move(11,1);
                  addstr("F - Slow down!");
                  break;
               case CARCHASE_OBSTACLE_CROSSTRAFFIC:
                  set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                  move(9,1);
                  //LIMIT :------------------------------------:
                  addstr("There's a red light with crosstraffic ahead!");
                  move(10,1);
                  addstr("D - Run the light anyway!");
                  move(11,1);
                  addstr("F - Slow down and turn!");
                  break;
            }
         }
         move(y,1);
         addstr("B - Bail out and run!");
         if(chaseseq.canpullover)
         {
            move(y+1,1);
            addstr("P - Pull over");
         }
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,1);
         addstr("C - Reflect on your lack of skill.");
      }

      //PRINT ENEMIES
      printchaseencounter();

      refresh();

      int c=getch();
      translategetch(c);

      if(partyalive==0&&c=='c')
      {
         //DESTROY ALL CARS BROUGHT ALONG WITH PARTY
         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;
            if(activesquad->squad[p]->carid!=-1)
            {
               long v=id_getcar(activesquad->squad[p]->carid);
               if(v!=-1)
               {
                  removecarprefs_pool(vehicle[v]->id);
                  delete vehicle[v];
                  vehicle.erase(vehicle.begin() + v);
               }
            }
         }

         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;

            for(int pl=pool.size()-1;pl>=0;pl--)
            {
               if(pool[pl]==activesquad->squad[p])
               {
                  pool[pl]->alive=0;
                  pool[pl]->location=-1;
                  //delete pool[pl];
                  //pool.erase(pool.begin() + pl);
                  break;
               }
            }

            activesquad->squad[p]=NULL;
         }

         if(!endcheck())
         {
            mode=GAMEMODE_BASE;
            return 0;
         }
      }

      if(partyalive>0)
      {
         if(c=='o'&&partysize>1)orderparty();

         if(c=='0')party_status=-1;

         if(c>='1'&&c<='6')
         {
            if(activesquad->squad[c-'1']!=NULL)
            {
               if(party_status==c-'1')fullstatus(party_status);
               else party_status=c-'1';
            }
         }

         if(c=='b')
         {
            for(int v=0;v<chaseseq.friendcar.size();v++)
            {
               for(int v2=(int)vehicle.size()-1;v2>=0;v2--)
               {
                  if(vehicle[v2]==chaseseq.friendcar[v])
                  {
                     removecarprefs_pool(vehicle[v2]->id);
                     delete vehicle[v2];
                     vehicle.erase(vehicle.begin() + v2);
                  }
               }
            }
            chaseseq.friendcar.clear();
            for(int p=0;p<6;p++)
            {
               if(activesquad->squad[p]==NULL)continue;
               activesquad->squad[p]->carid=-1;
            }

            return footchase();
         }
         else if(c=='p')
         {
            if(chaseseq.canpullover)
            {
               chase_giveup();
               return 0;
            }
         }
         else if(obstacle==-1)
         {
            if(c=='d')
            {
               if(encounter[0].exists&&
                  encounter[0].type==CREATURE_COP)
               {
                  if(chaseseq.location)sitestory->crime.push_back(CRIME_CARCHASE);
                  criminalizeparty(LAWFLAG_RESIST);
               }
               evasivedrive();
               enemyattack();
               creatureadvance();
               drivingupdate(obstacle);
            }

            if(c=='f')
            {
               if(encounter[0].exists&&
                  encounter[0].type==CREATURE_COP)
               {
                  if(chaseseq.location)sitestory->crime.push_back(CRIME_CARCHASE);
                  criminalizeparty(LAWFLAG_RESIST);
               }
               youattack();
               enemyattack();
               creatureadvance();
               drivingupdate(obstacle);
            }

            if(c=='e')
            {
               equip(activesquad->loot,-1);
            }
         }
         else
         {
            switch(obstacle)
            {
               case CARCHASE_OBSTACLE_CROSSTRAFFIC:
               case CARCHASE_OBSTACLE_TRUCKPULLSOUT:
               case CARCHASE_OBSTACLE_FRUITSTAND:
                  if(c=='d')
                  {
                     obstacledrive(obstacle,0);
                     creatureadvance();
                     drivingupdate(obstacle);
                  }
                  if(c=='f')
                  {
                     obstacledrive(obstacle,1);
                     creatureadvance();
                     drivingupdate(obstacle);
                  }
                  break;
            }
         }

         //HAVE YOU LOST ALL OF THEM?
            //THEN LEAVE
         partysize=0;
         partyalive=0;
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)partysize++;
            else continue;

            if(activesquad->squad[p]->alive==1)partyalive++;
         }
         int baddiecount=0;
         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].carid!=-1&&
               encounter[e].enemy()&&
               encounter[e].alive&&
               encounter[e].exists)baddiecount++;
         }
         if(partyalive>0&&baddiecount==0)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            clearmessagearea();
            move(16,1);
            addstr("It looks like you've lost them!");
            refresh();
            getch();
            for(int p=0;p<pool.size();p++)
            {
               for(int w=0;w<BODYPARTNUM;w++)
               {
                  pool[p]->wound[w]&=~WOUND_BLEEDING;
               }
            }
            mode=GAMEMODE_BASE;
            return 1;
         }
      }
      
   }while(1);

   mode=GAMEMODE_BASE;
   return 1;
}



char footchase(void)
{
   //NOTE: THIS FUNCTION RETURNS 1 IF ANYBODY ESCAPES
      //IT SHOULD NOT DELETE SQUADS OR CREATURES
         //SQUADS MAY BE FICTITIOUS AND BOTH WILL BE DELETED LATER ANYWAY
      int p;
   reloadparty();

   //NUKE ALL CARS
   for(int v=0;v<chaseseq.enemycar.size();v++)
   {
      delete chaseseq.enemycar[v];
   }
   chaseseq.enemycar.clear();

   //BAIL IF NO CHASERS
   int chasenum=0;
   for(int e=0;e<ENCMAX;e++)
   {
      if(encounter[e].exists)chasenum++;
      encounter[e].carid=-1;
   }
   if(chasenum==0)return 1;

   mode=GAMEMODE_CHASEFOOT;

   do
   {
      int partysize=0;
      int partyalive=0;
      for(p=0;p<6;p++)
      {
         if(activesquad->squad[p]!=NULL)partysize++;
         else continue;

         if(activesquad->squad[p]->alive==1)partyalive++;
      }
      int encsize=0;
      for(int e=0;e<ENCMAX;e++)
      {
         if(encounter[e].exists)
         {
            encsize++;
         }
      }

      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(0,0);
      addlocationname(location[chaseseq.location]);

      //PRINT PARTY
      if(partyalive==0)party_status=-1;
      printparty();

      if(partyalive>0)
      {
         //PRINT DRIVING SITUATION AND INSTRUCTIONS
         if(partysize>1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(9,40);
         addstr("O - Change the squad's Liberal order");
         if(partysize>0&&(party_status==-1||partysize>1))set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(10,40);
         addstr("# - Check the status of a squad Liberal");
         if(party_status!=-1)set_color(COLOR_WHITE,COLOR_BLACK,0);
         else set_color(COLOR_BLACK,COLOR_BLACK,1);
         move(11,40);
         addstr("0 - Show the squad's Liberal status");

         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,1);
         addstr("D - Try to lose them!");
         move(10,1);
         addstr("E - Equip");
         move(11,1);
         addstr("F - Fight!");
         if(chaseseq.canpullover)
         {
            move(12,1);
            addstr("G - Give Up");
         }
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         move(9,1);
         addstr("C - Reflect on your lack of skill.");
      }

      //PRINT ENEMIES
      printchaseencounter();

      refresh();

      int c=getch();
      translategetch(c);

      if(partyalive==0&&c=='c')
      {
         //DESTROY ALL CARS BROUGHT ALONG WITH PARTY
         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;
            if(activesquad->squad[p]->carid!=-1)
            {
               long v=id_getcar(activesquad->squad[p]->carid);
               if(v!=-1)
               {
                  removecarprefs_pool(vehicle[v]->id);
                  delete vehicle[v];
                  vehicle.erase(vehicle.begin() + v);
               }
            }
         }

         for(p=0;p<6;p++)
         {
            if(activesquad->squad[p]==NULL)continue;

            for(int pl=pool.size()-1;pl>=0;pl--)
            {
               if(pool[pl]==activesquad->squad[p])
               {
                  pool[pl]->alive=0;
                  pool[pl]->location=-1;
                  //delete pool[pl];
                  //pool.erase(pool.begin() + pl);
                  break;
               }
            }

            activesquad->squad[p]=NULL;
         }

         if(!endcheck())
         {
            mode=GAMEMODE_BASE;
            return 0;
         }
      }

      if(partyalive>0)
      {
         if(c=='o'&&partysize>1)orderparty();

         if(c=='0')party_status=-1;

         if(c>='1'&&c<='6')
         {
            if(activesquad->squad[c-'1']!=NULL)
            {
               if(party_status==c-'1')fullstatus(party_status);
               else party_status=c-'1';
            }
         }

         if(c=='g')
         {
            if(chaseseq.canpullover)
            {
               chase_giveup();
               return 0;
            }
         }
         if(c=='d')
         {
            if(encounter[0].exists&&
               encounter[0].type==CREATURE_COP)
            {
               sitestory->crime.push_back(CRIME_FOOTCHASE);
               criminalizeparty(LAWFLAG_RESIST);
            }
            evasiverun();
            enemyattack();
            creatureadvance();
         }

         if(c=='f')
         {
            if(encounter[0].exists&&
               encounter[0].type==CREATURE_COP)
            {
               sitestory->crime.push_back(CRIME_FOOTCHASE);
               criminalizeparty(LAWFLAG_RESIST);
            }
            youattack();
            enemyattack();
            creatureadvance();
         }

         if(c=='e')
         {
            equip(activesquad->loot,-1);
         }

         //HAVE YOU LOST ALL OF THEM?
            //THEN LEAVE
         partysize=0;
         partyalive=0;
         for(int p=0;p<6;p++)
         {
            if(activesquad->squad[p]!=NULL)partysize++;
            else continue;

            if(activesquad->squad[p]->alive==1)partyalive++;
         }
         int baddiecount=0;
         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].enemy()&&
               encounter[e].alive&&
               encounter[e].exists)baddiecount++;
         }
         if(partyalive>0&&baddiecount==0)
         {
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            clearmessagearea();
            move(16,1);
            addstr("It looks like you've lost them!");
            refresh();
            getch();
            for(int p=0;p<pool.size();p++)
            {
               for(int w=0;w<BODYPARTNUM;w++)
               {
                  pool[p]->wound[w]&=~WOUND_BLEEDING;
               }
            }
            mode=GAMEMODE_BASE;
            return 1;
         }
      }
      
   }while(1);

   mode=GAMEMODE_BASE;
   return 1;
}



void evasivedrive(void)
{
   int e;
   vector<long> yourrolls;
   long yourworst=10000;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(activesquad->squad[p]->alive&&
         activesquad->squad[p]->is_driver)
      {
         long v=id_getcar(activesquad->squad[p]->carid);
         yourrolls.push_back(driveskill(*activesquad->squad[p],vehicle[v])+LCSrandom(10));
         activesquad->squad[p]->train(SKILL_DRIVING,LCSrandom(20));
         if(yourworst>yourrolls.back())yourworst=yourrolls.back();
      }
   }
   if(yourrolls.size()==0)yourrolls.push_back(0);//error -- and for this you get a 0

   vector<long> theirrolls;
   vector<long> theirrolls_id;
   vector<long> theirrolls_drv;
   for(e=0;e<ENCMAX;e++)
   {
      if(encounter[e].carid!=-1&&
         encounter[e].enemy()&&
         encounter[e].alive&&
         encounter[e].exists&&
         encounter[e].is_driver)
      {
         for(int v=0;v<chaseseq.enemycar.size();v++)
         {
            if(chaseseq.enemycar[v]->id==encounter[e].carid)
            {
               theirrolls.push_back(driveskill(encounter[e],chaseseq.enemycar[v])+LCSrandom(10));
               theirrolls_id.push_back(encounter[e].carid);
               theirrolls_drv.push_back(encounter[e].id);
            }
         }
      }
   }

   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   switch(LCSrandom(yourworst/5))
   {
   default:
      addstr("You accelerate rapidly!");
      break;
   case 1:
      addstr("You swerve around the next corner!");
      break;
   case 2:
      addstr("You screech through an empty lot to the next street!");
      break;
   case 3:
      addstr("You boldly weave through oncoming traffic!");
      break;
   }
   refresh();
   getch();

   int cnt;
   for(int i=0;i<theirrolls.size();i++)
   {
      cnt=yourrolls[LCSrandom(yourrolls.size())];
      if(theirrolls[i]<cnt)
      {
         clearmessagearea();
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         move(16,1);
         for(e=0;e<ENCMAX;e++)
         {
            if(encounter[e].id==theirrolls_drv[i])
            {
               addstr(encounter[e].name);
               break;
            }
         }
         switch(LCSrandom(cnt/5))
         {
         default:
            addstr(" falls behind!");
            break;
         case 1:
            addstr(" skids out!");
            break;
         case 2:
            addstr(" backs off for safety.");
            break;
         case 3:
            addstr(" breaks hard and nearly crashes!");
            break;
         }

         for(e=0;e<ENCMAX;e++)
         {
            if(encounter[e].carid==theirrolls_id[i])
            {
               for(int e2=e;e2<ENCMAX-1;e2++)
               {
                  encounter[e2]=encounter[e2+1];
               }
               encounter[ENCMAX-1].exists=0;
               encounter[ENCMAX-1].carid=-1;
               e--;
            }
         }
         for(int v=0;v<chaseseq.enemycar.size();v++)
         {
            if(chaseseq.enemycar[v]->id==theirrolls_id[i])
            {
               delete chaseseq.enemycar[v];
               chaseseq.enemycar.erase(chaseseq.enemycar.begin() + v);
               break;
            }
         }

         printchaseencounter();
         refresh();
         getch();
      }
      else
      {
         clearmessagearea();
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(16,1);
         for(int e=0;e<ENCMAX;e++)
         {
            if(encounter[e].id==theirrolls_drv[i])
            {
               addstr(encounter[e].name);
               break;
            }
         }
         addstr(" is still on your tail!");
         refresh();
         getch();
      }
   }
}



void evasiverun(void)
{
   vector<int> yourspeed;
   yourspeed.resize(6);
   int yourworst=10000, yourbest=0;
   int wheelchair=0, notwheelchair=0;
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(activesquad->squad[p]->alive)
      {
         
         if(activesquad->squad[p]->flag & CREATUREFLAG_WHEELCHAIR)
         {
            yourspeed[p]=0;

            wheelchair++;
         }
         else 
         {
            yourspeed[p]=activesquad->squad[p]->attval(ATTRIBUTE_AGILITY)+
                         activesquad->squad[p]->attval(ATTRIBUTE_HEALTH)+LCSrandom(10);

            healthmodroll(yourspeed[p],*activesquad->squad[p]);

            notwheelchair++;
         }
         if(yourworst>yourspeed[p])yourworst=yourspeed[p];
         if(yourbest<yourspeed[p])yourbest=yourspeed[p];
      }
   }

   if(yourworst>5)
   {
      yourworst+=LCSrandom(5);
      
      clearmessagearea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);

      switch(LCSrandom(yourworst/5))
      {
      default:
         addstr("You suddenly dart into an alley!");
         break;
      case 1:
         addstr("You run as fast as you can!");
         break;
      case 2:
         addstr("You climb a fence in record time!");
         break;
      case 3:
         addstr("You scale a small building and leap between rooftops!");
      }
      
      refresh();
      getch();
   }

   int theirbest=0;
   int theirworst=10000;
   for(int e=0;e<ENCMAX;e++)
   {
      if(!encounter[e].exists)continue;

      int chaser=encounter[e].attval(ATTRIBUTE_AGILITY)+
                 encounter[e].attval(ATTRIBUTE_HEALTH) + LCSrandom(10);

      healthmodroll(chaser,encounter[e]);

      if(theirbest<chaser)theirbest=chaser;
      if(theirworst>chaser)theirworst=chaser;

      if(chaser<yourworst)
      {
         clearmessagearea();
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         move(16,1);
         addstr(encounter[e].name);
         addstr(" can't keep up!");

         delenc(e,0);
         e--;

         printchaseencounter();
         refresh();
         getch();
      }
      else
      {
         clearmessagearea();
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(16,1);
         addstr(encounter[e].name);
         addstr(" is still on your tail!");
         refresh();
         getch();
      }
   }

   
   //This last loop can be used to have fast people in
   //your squad escape one by one just as the enemy
   //falls behind one by one
   int othersleft=0;
   for(int p=5;p>=0;p--)
   {
      if(!encounter[0].exists)break;
      if(activesquad->squad[p]==NULL)continue;
      if(activesquad->squad[p]->alive)
      {
         if(yourspeed[p]>theirbest+5)
         {
            if(p==0 && othersleft==0)break;
            clearmessagearea();
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(16,1);
            addstr(activesquad->squad[p]->name);
            addstr(" breaks away!");
            refresh();
            getch();

            removesquadinfo(*activesquad->squad[p]);

            //Unload hauled hostage or body when they get back to the safehouse
            if(activesquad->squad[p]->prisoner!=NULL)
            {
               //If this is an LCS member or corpse being hauled (marked as in the squad)
               if(activesquad->squad[p]->prisoner->squadid!=-1)
               {
                  //Take them out of the squad
                  activesquad->squad[p]->prisoner->squadid=-1;
                  //Set base and current location to squad's safehouse
                  activesquad->squad[p]->prisoner->location=activesquad->squad[p]->base;
                  activesquad->squad[p]->prisoner->base=activesquad->squad[p]->base;
               }
               else //A kidnapped conservative
               {
                  //Convert them into a prisoner
                  kidnaptransfer(*activesquad->squad[p]->prisoner);
                  delete activesquad->squad[p]->prisoner;
               }
               activesquad->squad[p]->prisoner=NULL;
            }

            printparty();
         }
         else if(yourspeed[p]<theirbest-10)
         {
            clearmessagearea();
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(16,1);
            addstr(activesquad->squad[p]->name);
            addstr(" is seized, thrown to the ground, and ");
            switch(encounter[0].type)
            {
            case CREATURE_COP:addstr("handcuffed!");break;
            case CREATURE_GANGUNIT:addstr("tazered repeatedly!");activesquad->squad[p]->blood-=10;break;
            case CREATURE_DEATHSQUAD:addstr("shot in the head!");activesquad->squad[p]->blood=0;break;
            default:addstr("beaten senseless!");activesquad->squad[p]->blood-=60;break;
            }
            if(activesquad->squad[p]->blood<=0)
               activesquad->squad[p]->alive=0;

            capturecreature(*activesquad->squad[p]);
            for(int i=p+1, j=p;i<6;i++, j++)
            {
               activesquad->squad[j]=activesquad->squad[i];
            }
            activesquad->squad[5]=NULL;
            // Death squads don't mess around, and don't fall behind when executing your people
            if(encounter[0].type!=CREATURE_DEATHSQUAD)delenc(0,0);

            printparty();
            printchaseencounter();
            refresh();
            getch();
         }
         else othersleft++;
      }
   }
}



int driveskill(Creature &cr,vehiclest *v)
{
   int vbonus=0;
   switch(v->type)
   {
      case VEHICLE_PICKUP:
      case VEHICLE_SUV:
      case VEHICLE_TAXICAB:
      case VEHICLE_JEEP:
         vbonus=1;
         break;
      case VEHICLE_POLICECAR:
         vbonus=2;
         break;
      case VEHICLE_SPORTSCAR:
      case VEHICLE_AGENTCAR:
         vbonus=3;
         break;
   }
   int driveskill=cr.skillval(SKILL_DRIVING)*(3+vbonus);
   healthmodroll(driveskill,cr);
   if(driveskill<0)driveskill=0;
   driveskill*=static_cast<int>(cr.blood/50.0);
   return driveskill;
}



void drivingupdate(short &obstacle)
{
   //CHECK TO SEE WHICH CARS ARE BEING DRIVEN
   vector<int> passenger;
   int driver;
   int v;
   int p;
   for(v=chaseseq.friendcar.size()-1;v>=0;v--)
   {
      passenger.clear();
      driver=-1;
      for(p=0;p<6;p++)
      {
         if(activesquad->squad[p]==NULL)continue;
         if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id)
         {
            if(activesquad->squad[p]->is_driver)
            {
               if(activesquad->squad[p]->canwalk())driver=p;
               else activesquad->squad[p]->is_driver=0;
            }
            passenger.push_back(p);
         }
      }

      if(passenger.size()>0&&driver==-1)
      {
         //MAKE BEST DRIVING PASSENGER INTO A DRIVER
         vector<int> goodp;
         int max=0;
         for(p=0;p<passenger.size();p++)
         {
            if(driveskill(*activesquad->squad[passenger[p]],chaseseq.friendcar[v])>max&&
               activesquad->squad[passenger[p]]->canwalk())
            {
               max=driveskill(*activesquad->squad[passenger[p]],chaseseq.friendcar[v]);
            }
         }
         for(p=0;p<passenger.size();p++)
         {
            if(driveskill(*activesquad->squad[passenger[p]],chaseseq.friendcar[v])==max&&
               activesquad->squad[passenger[p]]->canwalk())
            {
               goodp.push_back(passenger[p]);
            }
         }

         if(goodp.size()>0)
         {
            int p=goodp[LCSrandom(goodp.size())];
            activesquad->squad[p]->is_driver=1;
            driver=p;

            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr(activesquad->squad[p]->name);
            addstr(" takes over the wheel.");
            printparty();
            refresh();
            getch();
         }
      }
      if(driver==-1)
      {
         crashfriendlycar(v);
         sitestory->crime.push_back(CRIME_CARCHASE);
      }
   }

   for(v=chaseseq.enemycar.size()-1;v>=0;v--)
   {
      passenger.clear();
      driver=-1;
      for(p=0;p<ENCMAX;p++)
      {
         if(!encounter[p].exists)continue;
         if(encounter[p].carid==chaseseq.enemycar[v]->id)
         {
            if(encounter[p].is_driver)
            {
               if(encounter[p].canwalk())driver=p;
               else encounter[p].is_driver=0;
            }
            passenger.push_back(p);
         }
      }
      // Enemies don't take over the wheel when driver incapacitated
      if(driver==-1)
      {
         crashenemycar(v);
         sitestory->crime.push_back(CRIME_CARCHASE);
      }
   }

   //SET UP NEXT OBSTACLE
   if(!LCSrandom(3))
   {
      obstacle=LCSrandom(CARCHASE_OBSTACLENUM);
   }
   else obstacle=-1;
}



void makechasers(long sitetype,long sitecrime)
{
   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

   if(sitecrime==0)return;

   short encslot=0;
   int n;

   long cartype=-1;
   long pnum;

   chaseseq.canpullover=0;
   // 50% of CCS harassing your teams once they reach the
   // "attacks" stage (but not for activities, which are
   // law enforcement response specific)
   if(endgamestate<ENDGAME_CCS_DEFEATED&&
      endgamestate>=ENDGAME_CCS_ATTACKS&&
      LCSrandom(2)&&sitetype!=-1)
   {
      cartype=VEHICLE_SUV;
      pnum=LCSrandom(sitecrime/5 + 1)+1;
      if(pnum>12)pnum=12;
      for(n=0;n<pnum;n++)
      {
         makecreature(encounter[encslot],CREATURE_CCS_VIGILANTE);
         encslot++;
      }
   }
   else
   {
      switch(sitetype)
      {
         case SITE_GOVERNMENT_INTELLIGENCEHQ:
            cartype=VEHICLE_AGENTCAR;
            pnum=LCSrandom(sitecrime/5 + 1)+1;
            if(pnum>6)pnum=6;
            for(n=0;n<pnum;n++)
            {
               makecreature(encounter[encslot],CREATURE_AGENT);
               encslot++;
            }
            break;
         case SITE_CORPORATE_HEADQUARTERS:
         case SITE_CORPORATE_HOUSE:
            if(LCSrandom(2))cartype=VEHICLE_SUV;
            else cartype=VEHICLE_JEEP;
            pnum=LCSrandom(sitecrime/5 + 1)+1;
            if(pnum>6)pnum=6;
            for(n=0;n<pnum;n++)
            {
               makecreature(encounter[encslot],CREATURE_MERC);
               encslot++;
            }
            break;
         case SITE_MEDIA_AMRADIO:
         case SITE_MEDIA_CABLENEWS:
            cartype=VEHICLE_PICKUP;
            pnum=LCSrandom(sitecrime/5 + 1)+1;
            if(pnum>18)pnum=18;
            for(n=0;n<pnum;n++)
            {
               makecreature(encounter[encslot],CREATURE_HICK);
               encslot++;
            }
            break;
         default:
            chaseseq.canpullover=1;
            cartype=VEHICLE_POLICECAR;
            pnum=LCSrandom(sitecrime/5 + 1)+1;
            if(pnum>6)pnum=6;
            for(n=0;n<pnum;n++)
            {
               if(law[LAW_DEATHPENALTY]==-2&&
                  law[LAW_POLICEBEHAVIOR]==-2){makecreature(encounter[encslot],CREATURE_DEATHSQUAD);chaseseq.canpullover=0;}
               else if(law[LAW_POLICEBEHAVIOR]<=-1)makecreature(encounter[encslot],CREATURE_GANGUNIT);
               else makecreature(encounter[encslot],CREATURE_COP);
               encslot++;
            }
            break;
      }
   }

   for(n=0;n<pnum;n++)conservatise(encounter[n]);

   //ASSIGN CARS TO CREATURES
   int carnum;
   if(pnum<=2)carnum=1;
   else if(pnum<=3)carnum=LCSrandom(2)+1;
   else if(pnum<=5)carnum=LCSrandom(2)+2;
   else if(pnum<=7)carnum=LCSrandom(2)+3;
   else carnum=4;

   for(int c=0;c<carnum;c++)
   {
      vehiclest *v=new vehiclest;
         v->init(cartype);
      chaseseq.enemycar.push_back(v);

      for(n=0;n<pnum;n++)
      {
         if(encounter[n].carid==-1)
         {
            encounter[n].carid=v->id;
            encounter[n].is_driver=1;
            break;
         }
      }
   }

   long load[4]={0,0,0,0};

   for(n=0;n<pnum;n++)
   {
      if(encounter[n].carid==-1)
      {
         int v;
         do
         {
            v=LCSrandom(chaseseq.enemycar.size());
            encounter[n].carid=chaseseq.enemycar[v]->id;
            encounter[n].is_driver=0;
         }while(load[v]>=4);
         load[v]++;
      }
   }
}



void obstacledrive(short obstacle,char choice)
{
   switch(obstacle)
   {
      case CARCHASE_OBSTACLE_CROSSTRAFFIC:
         if(choice==0)
         {
            dodgedrive();
         }
         else if(choice==1)
         {
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("You slow down, and turn the corner.");
            refresh();
            getch();

            if(!LCSrandom(3))
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(17,1);
               addstr("Here they come!");
               refresh();
               getch();
               enemyattack();
               youattack();
            }
         }
         break;
      case CARCHASE_OBSTACLE_TRUCKPULLSOUT:
         if(choice==0)
         {
            dodgedrive();
         }
         else if(choice==1)
         {
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("You slow down, and carefully evade the truck.");
            refresh();
            getch();

            if(!LCSrandom(3))
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(17,1);
               addstr("Here they come!");
               refresh();
               getch();
               enemyattack();
               youattack();
            }
         }
         break;
      case CARCHASE_OBSTACLE_FRUITSTAND:
         if(choice==0)
         {
            dodgedrive();
         }
         else if(choice==1)
         {
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("You plow through the fruit stand!");
            refresh();
            getch();

            if(!LCSrandom(5))
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               move(17,1);
               addstr("The fruit-seller has been squashed!");
               refresh();
               getch();

               criminalizeparty(LAWFLAG_MURDER);
            }
         }
         break;
   }
}



void dodgedrive(void)
{
   int v;
   clearmessagearea();
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   move(16,1);
   addstr("You swerve to avoid the obstacle!");
   refresh();
   getch();

   int driver;
   for(v=chaseseq.friendcar.size()-1;v>=0;v--)
   {
      driver=-1;
      for(int p=0;p<6;p++)
      {
         if(activesquad->squad[p]==NULL)continue;
         if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id)
         {
            if(activesquad->squad[p]->is_driver)
            {
               driver=p;
               break;
            }
         }
      }

      if(driver!=-1)
      {
         if(LCSrandom(11)>driveskill(*activesquad->squad[driver],vehicle[id_getcar(activesquad->squad[driver]->carid)]))
         {
            crashfriendlycar(v);
            sitestory->crime.push_back(CRIME_CARCHASE);
         }
      }
   }

   for(v=chaseseq.enemycar.size()-1;v>=0;v--)
   {
      driver=-1;
      for(int p=0;p<ENCMAX;p++)
      {
         if(!encounter[p].exists)continue;
         if(encounter[p].carid==chaseseq.enemycar[v]->id)
         {
            if(encounter[p].is_driver)
            {
               driver=p;
               break;
            }
         }
      }

      if(driver!=-1)
      {
         if(LCSrandom(11)>driveskill(encounter[driver],chaseseq.enemycar[v]))
         {
            crashenemycar(v);
            sitestory->crime.push_back(CRIME_CARCHASE);
         }
      }
   }
}



void crashfriendlycar(int v)
{
   char str[80];
   getcarfull(str,*chaseseq.friendcar[v]);

   int victimsum=0;

   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id)
      {
         victimsum++;
         if(activesquad->squad[p]->alive&&
            activesquad->squad[p]->align==1)stat_dead++;
         activesquad->squad[p]->alive=0;
         if(activesquad->squad[p]->prisoner!=NULL)
         {
            victimsum++;
            activesquad->squad[p]->prisoner->alive=0;
            if(activesquad->squad[p]->prisoner->squadid!=-1)
            {
               if(activesquad->squad[p]->prisoner->alive&&
                  activesquad->squad[p]->prisoner->align==1)stat_dead++;
               for(int pl=pool.size()-1;pl>=0;pl--)
               {
                  if(pool[pl]==activesquad->squad[p]->prisoner)
                  {
                     pool[pl]->alive=0;
                     pool[pl]->location=-1;
                     //delete pool[pl];
                     //pool.erase(pool.begin() + pl);
                     break;
                  }
               }
            }
            else delete activesquad->squad[p]->prisoner;
            activesquad->squad[p]->prisoner=NULL;
         }
         for(int pl=pool.size()-1;pl>=0;pl--)
         {
            if(pool[pl]==activesquad->squad[p])
            {
               pool[pl]->alive=false;
               //delete pool[pl];
               //pool.erase(pool.begin() + pl);
               int boss=getpoolcreature(pool[pl]->hireid);
               if(boss!=-1&&pool[boss]->juice>50)
               {
                  int juice=pool[boss]->juice-50;
                  if(juice>10)juice=10;
                  addjuice(*pool[boss],-juice);
               }
               break;
            }
         }
         activesquad->squad[p]=NULL;
      }
   }

   //CONSOLIDATE SQUAD
   for(int i=0;i<5;i++)
   {
      char flipstart=0;
      for(int pt=0;pt<6;pt++)
      {
         if(activesquad->squad[pt]==NULL&&!flipstart)flipstart=1;
         if(flipstart&&pt<5)activesquad->squad[pt]=activesquad->squad[pt+1];
      }
      if(flipstart)activesquad->squad[5]=NULL;
   }

   //GET RID OF CAR
   for(int v2=(int)vehicle.size()-1;v2>=0;v2--)
   {
      if(vehicle[v2]==chaseseq.friendcar[v])
      {
         removecarprefs_pool(vehicle[v2]->id);
         delete vehicle[v2];
         vehicle.erase(vehicle.begin() + v2);
         break;
      }
   }

   chaseseq.friendcar.erase(chaseseq.friendcar.begin() + v);

   //CRASH CAR
   clearmessagearea();
   set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   move(16,1);
   addstr("Your ");
   addstr(str);
   switch(LCSrandom(3))
   {
      case 0:addstr(" slams into a building.");break;
      case 1:
         addstr(" spins out and crashes.");
         move(17,1);
         if(victimsum>1)addstr("Everyone inside is peeled off against the pavement.");
         else if(victimsum==1)addstr("The person inside is squashed into a cube.");
         break;
      case 2:addstr(" hits a parked car and flips over.");break;
   }
   printparty();
   refresh();
   getch();
}



void crashenemycar(int v)
{
   char str[80];
   getcarfull(str,*chaseseq.enemycar[v]);

   int victimsum=0;

   for(int p=ENCMAX-1;p>=0;p--)
   {
      if(!encounter[p].exists)continue;
      if(encounter[p].carid==chaseseq.enemycar[v]->id)
      {
         victimsum++;
         delenc(p,0);
      }
   }

   delete chaseseq.enemycar[v];
   chaseseq.enemycar.erase(chaseseq.enemycar.begin() + v);

   //CRASH CAR
   clearmessagearea();
   set_color(COLOR_CYAN,COLOR_BLACK,1);
   move(16,1);
   addstr("The ");
   addstr(str);
   switch(LCSrandom(3))
   {
      case 0:addstr(" slams into a building.");break;
      case 1:
         addstr(" spins out and crashes.");
         move(17,1);
         if(victimsum>1)addstr("Everyone inside is peeled off against the pavement.");
         else if(victimsum==1)addstr("The person inside is squashed into a cube.");
         break;
      case 2:addstr(" hits a parked car and flips over.");break;
   }
   printchaseencounter();
   refresh();
   getch();
}


void chase_giveup(void)
{
   int p;
   int ps=-1;
   for(int l=0;l<location.size();l++)
   {
      if(location[l]->type==SITE_GOVERNMENT_POLICESTATION)
      {
         ps=l;
      }
   }

   for(int v=0;v<chaseseq.friendcar.size();v++)
   {
      for(int v2=(int)vehicle.size()-1;v2>=0;v2--)
      {
         if(vehicle[v2]==chaseseq.friendcar[v])
         {
            removecarprefs_pool(vehicle[v2]->id);
            delete vehicle[v2];
            vehicle.erase(vehicle.begin() + v2);
         }
      }
   }
   chaseseq.friendcar.clear();
   int hostagefreed=0;
   for(p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      activesquad->squad[p]->squadid=-1;
      activesquad->squad[p]->carid=-1;
      activesquad->squad[p]->location=ps;
      activesquad->squad[p]->weapon.type=WEAPON_NONE;
      activesquad->squad[p]->weapon.ammo=0;
      activesquad->squad[p]->activity.type=ACTIVITY_NONE;
      if(activesquad->squad[p]->prisoner!=NULL)
      {
	      if(activesquad->squad[p]->prisoner->squadid==-1)
         {
            hostagefreed++;
         }
         freehostage(*activesquad->squad[p],2);
      }
      activesquad->squad[p]=NULL;
   }
   for(p=0;p<pool.size();p++)
   {
      for(int w=0;w<BODYPARTNUM;w++)
      {
         pool[p]->wound[w]&=~WOUND_BLEEDING;
      }
   }

   clearmessagearea();
   clearcommandarea();
   set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   move(16,1);
   if(mode!=GAMEMODE_CHASECAR)addstr("You stop and are arrested.");
   else addstr("You pull over and are arrested.");
   if(hostagefreed>0)
   {
	   move(17,1);
	   addstr("Your hostage");
	   if(hostagefreed>1)
		   addstr("s are free.");
	   else
		   addstr(" is free.");
   }
   refresh();
   getch();
}



/* the next two functions force a chase sequence with a specific liberal */
char footchase(Creature &cr)
{
   long oldsqid=cr.squadid;
   Creature *crp=&cr;
   squadst *sq=new squadst;
      sq->squad[0]=&cr;
      sq->squad[0]->squadid=cursquadid;cursquadid++;
      cr.squadid=sq->squad[0]->squadid;
      cr.carid=-1;

   squadst *oact=activesquad;
   short ops=party_status;
   activesquad=sq;
   party_status=0;
   char ret=footchase();

   party_status=ops;

   delete sq;

   if(ret)
   {
      cr.squadid=oldsqid;
   }
   else if(oldsqid!=-1)
   {
      activesquad=squad[getsquad(oldsqid)];
      for(int i=0;i<6;i++)
      {
         if(activesquad->squad[i]!=NULL && activesquad->squad[i]==crp)
         {
            for(int j=i+1;j<6;j++,i++)
            {
               activesquad->squad[i]=activesquad->squad[j];
            }
            activesquad->squad[5]=NULL;
            break;
         }
      }
   }
   activesquad=oact;
   return ret;
}



char chasesequence(Creature &cr,vehiclest &v)
{
   long oldsqid=cr.squadid;
   squadst *sq=new squadst;
      sq->squad[0]=&cr;
      sq->squad[0]->squadid=cursquadid;cursquadid++;
      sq->id=cursquadid;cursquadid++;
      cr.squadid=sq->squad[0]->squadid;
      cr.carid=v.id;
      cr.is_driver=1;

   squadst *oact=activesquad;
   short ops=party_status;
   activesquad=sq;
   party_status=0;
   char ret=chasesequence();
   party_status=ops;

   delete sq;

   if(ret)
   {
      cr.squadid=oldsqid;
      cr.carid=-1;
   }
   else if(oldsqid!=-1)
   {
      activesquad=squad[getsquad(oldsqid)];
      for(int i=0;i<6;i++)
      {
         if(activesquad->squad[i]!=NULL && activesquad->squad[i]==&cr)
         {
            for(int j=i+1;j<6;j++,i++)
            {
               activesquad->squad[i]=activesquad->squad[j];
            }
            activesquad->squad[5]=NULL;
            break;
         }
      }
   }
   activesquad=oact;
   return ret;
}
