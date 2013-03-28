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

#define CARCHASE_CONTINUE        0
#define CARCHASE_GOTOFOOTCHASE   1
#define CARCHASE_SURRENDER       2
#define CARCHASE_EVERYONEDEAD    3
#define CARCHASE_ESCAPED         4

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
   if(chasenum==0)
   {
      gamelog.newline();
      return 1;
   }

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
               if(chaseseq.friendcar[v2]->id()==vehicle[v]->id())break;
            }
            if(v2==chaseseq.friendcar.size())chaseseq.friendcar.push_back(vehicle[v]);
         }
      }
   }

   mode=GAMEMODE_CHASECAR;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("As you pull away from the site, you notice that you are ", gamelog);
   move(1,0);
   addstr("being followed by Conservative swine!", gamelog);
   gamelog.newline(); //New line.
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
                  addstr("You are speeding toward a flimsy fruit stand!", gamelog);
                  gamelog.newline(); //Newline!
                  move(10,1);
                  addstr("D - Swerve to avoid hitting anyone!");
                  move(11,1);
                  addstr("F - Play it safe and plow through it!");
                  break;
               case CARCHASE_OBSTACLE_TRUCKPULLSOUT:
                  set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                  move(9,1);
                  //LIMIT :------------------------------------:
                  addstr("A truck pulls out in your path!", gamelog);
                  gamelog.newline(); //Newline.
                  move(10,1);
                  addstr("D - Speed around it!");
                  move(11,1);
                  addstr("F - Slow down!");
                  break;
               case CARCHASE_OBSTACLE_CROSSTRAFFIC:
                  set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                  move(9,1);
                  //LIMIT :------------------------------------:
                  addstr("There's a red light with cross traffic ahead!", gamelog);
                  gamelog.newline(); //Newline...
                  move(10,1);
                  addstr("D - Run the light anyway!");
                  move(11,1);
                  addstr("F - Slow down and turn!");
                  break;
               case CARCHASE_OBSTACLE_CHILD:
                  set_color(COLOR_MAGENTA,COLOR_BLACK,1);
                  move(9,1);
                  //LIMIT :------------------------------------:
                  addstr("A kid runs into the street for his ball!", gamelog);
                  gamelog.newline(); //Newline.
                  move(10,1);
                  addstr("D - Swerve around him!");
                  move(11,1);
                  addstr("F - Slam the brakes!");
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
                  pool[pl]->die();
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
            gamelog.nextMessage(); //Next message.
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
               gamelog.nextMessage(); //Next message.
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
               youattack();
               creatureadvance();
               if(drivingupdate(obstacle))
                  return footchase();
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
               if(drivingupdate(obstacle))
                  return footchase();
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
            case CARCHASE_OBSTACLE_CHILD:
            case CARCHASE_OBSTACLE_CROSSTRAFFIC:
            case CARCHASE_OBSTACLE_TRUCKPULLSOUT:
            case CARCHASE_OBSTACLE_FRUITSTAND:
               if(c=='d')
               {
                  if(obstacledrive(obstacle,0))
                     return footchase();
                  creatureadvance();
                  drivingupdate(obstacle);
               }
               if(c=='f')
               {
                  if(obstacledrive(obstacle,1))
                     return footchase();
                  creatureadvance();
                  if(drivingupdate(obstacle))
                     return footchase();
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
            addstr("It looks like you've lost them!", gamelog);
            gamelog.newline(); //New line.
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
            //Make sure all possible exist of the chase have the nextMessage() call
            //to ensure that the gamelog is split properly into blocks.
            gamelog.nextMessage();
            return 1;
         }
      }
      
   }while(1);

   mode=GAMEMODE_BASE;
   //Make sure all possible exits of the chase have the nextMessage() call
   //to ensure that the gamelog is split properly into blocks.
   gamelog.nextMessage();
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
   if(chasenum==0)
   {
      return 1;
   }

   mode=GAMEMODE_CHASEFOOT;

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(0,0);
   addstr("As you exit the site, you notice that you are ", gamelog);
   move(1,0);
   gamelog.newline(); //New line. I'd rather it be continuous but whatever.
   addstr("being followed by Conservative swine!", gamelog);
   gamelog.newline(); //New line.
   refresh();
   getch();

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

      // check if we fought the previous loop; if so, add a blank gamelog line
      if(foughtthisround)gamelog.newline();
      foughtthisround=0;

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
                  pool[pl]->die();
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
            gamelog.nextMessage(); //Output this message, preparing for next.
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
               gamelog.nextMessage(); //Get ready for the next message.
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
            addstr("It looks like you've lost them!", gamelog);
            gamelog.newline(); //New line.
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
            gamelog.newline();
            return 1;
         }
      }
      
   }while(1);

   mode=GAMEMODE_BASE;
   //All possible exits of the chase should be covered by a nextMessage() call.
   gamelog.nextMessage();
   return 1;
}


#define DRIVING_RANDOMNESS 13
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
         yourrolls.push_back(driveskill(*activesquad->squad[p],*(vehicle[v]))+LCSrandom(DRIVING_RANDOMNESS));
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
            if(chaseseq.enemycar[v]->id()==encounter[e].carid)
            {
               theirrolls.push_back(driveskill(encounter[e],*chaseseq.enemycar[v])+LCSrandom(DRIVING_RANDOMNESS));
               theirrolls_id.push_back(encounter[e].carid);
               theirrolls_drv.push_back(encounter[e].id);
            }
         }
      }
      else if(encounter[e].carid==-1)
         encounter[e].exists=false;
   }

   clearmessagearea();
   set_color(COLOR_WHITE,COLOR_BLACK,1);
   move(16,1);
   switch(LCSrandom(4))
   {
   case 0:
      addstr("You keep the gas floored!", gamelog);
      gamelog.newline(); //Newline.
      break;
   case 1:
      addstr("You swerve around the next corner!", gamelog);
      gamelog.newline(); //New line.
      break;
   case 2:
      addstr("You screech through an empty lot to the next street!", gamelog);
      gamelog.newline(); //New line.
      break;
   case 3:
      if(yourworst>15)
         addstr("You boldly weave through oncoming traffic!", gamelog);
      else
         addstr("You make obscene gestures at the pursuers!", gamelog);
      gamelog.newline(); //new line.
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
               addstr(encounter[e].name, gamelog);
               break;
            }
         }
         switch(LCSrandom(cnt/5))
         {
         default:
            addstr(" falls behind!", gamelog);
            gamelog.newline(); //New line.
            break;
         case 1:
            addstr(" skids out!", gamelog);
            gamelog.newline(); //New line.
            break;
         case 2:
            addstr(" backs off for safety.", gamelog);
            gamelog.newline(); //New line.
            break;
         case 3:
            addstr(" breaks hard and nearly crashes!", gamelog);
            gamelog.newline(); //New line.
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
            if(chaseseq.enemycar[v]->id()==theirrolls_id[i])
            {
               delete chaseseq.enemycar[v];
               chaseseq.enemycar.erase(chaseseq.enemycar.begin() + v);
               break;
            }
         }

         clearmaparea();
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
               addstr(encounter[e].name, gamelog);
               break;
            }
         }
         addstr(" is still on your tail!", gamelog);
         gamelog.newline(); //Blarg. Newline.
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
            yourspeed[p]=activesquad->squad[p]->attribute_roll(ATTRIBUTE_AGILITY)+
                         activesquad->squad[p]->attribute_roll(ATTRIBUTE_HEALTH);

            //healthmodroll(yourspeed[p],*activesquad->squad[p]);

            notwheelchair++;
         }
         if(yourworst>yourspeed[p])yourworst=yourspeed[p];
         if(yourbest<yourspeed[p])yourbest=yourspeed[p];
      }
   }

   if(yourworst>14)
   {
      yourworst+=LCSrandom(5);
      
      clearmessagearea();
      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(16,1);

      switch(LCSrandom(yourworst/5))
      {
      default:
         addstr("You suddenly dart into an alley!", gamelog);
         gamelog.newline(); //New line.
         break;
      case 1:
         addstr("You run as fast as you can!", gamelog);
         gamelog.newline(); //New line.
         break;
      case 2:
         addstr("You climb a fence in record time!", gamelog);
         gamelog.newline(); //New line.
         break;
      case 3:
         addstr("You scale a small building and leap between rooftops!", gamelog);
         gamelog.newline(); //New line.
      }
      
      refresh();
      getch();
   }

   int theirbest=0;
   int theirworst=10000;
   for(int e=0;e<ENCMAX;e++)
   {
      if(!encounter[e].exists)continue;

      int chaser=encounter[e].attribute_roll(ATTRIBUTE_AGILITY)+
                 encounter[e].attribute_roll(ATTRIBUTE_HEALTH);

      //healthmodroll(chaser,encounter[e]);

      if(theirbest<chaser)theirbest=chaser;
      if(theirworst>chaser)theirworst=chaser;

      if(encounter[e].type == CREATURE_TANK && LCSrandom(10))
      {
         clearmessagearea();
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         move(16,1);
         addstr(encounter[e].name, gamelog);
         switch(LCSrandom(4))
         {
         case 0:
            addstr(" plows through a brick wall like it was nothing!", gamelog);
            gamelog.newline(); //New line.
            break;
         case 1:
            addstr(" charges down an alley, smashing both side walls out!", gamelog);
            gamelog.newline(); //Blarg.
            break;
         case 2:
            addstr(" smashes straight through traffic, demolishing cars!", gamelog);
            gamelog.newline(); //Newline.
            break;
         case 3:
            addstr(" destroys everything in its path, closing the distance!", gamelog);
            gamelog.newline(); //I wish these were all contained in addstr.
            break;
         }

         refresh();
         getch();
      }
      else if(chaser<yourworst)
      {
         clearmessagearea();
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         move(16,1);
         addstr(encounter[e].name, gamelog);
         if(encounter[e].type == CREATURE_TANK)
         {
            addstr(" tips into a pool. The tank is trapped!", gamelog);
            gamelog.newline(); //New line.
         }
         else
         {
            addstr(" can't keep up!", gamelog);
            gamelog.newline(); //Another new line.
         }

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
         addstr(encounter[e].name, gamelog);
         addstr(" is still on your tail!", gamelog);
         gamelog.newline(); //New line.
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
         if(yourspeed[p]>theirbest)
         {
            if(p==0 && othersleft==0)break;
            clearmessagearea();
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(16,1);
            addstr(activesquad->squad[p]->name, gamelog);
            addstr(" breaks away!", gamelog);
            gamelog.newline(); //New line.
            refresh();
            getch();

            //Unload hauled hostage or body when they get back to the safehouse
            if(activesquad->squad[p]->prisoner!=NULL)
            {
               //If this is an LCS member or corpse being hauled (marked as in the squad)
               if(activesquad->squad[p]->prisoner->squadid!=-1)
               {
                  //Take them out of the squad
                  removesquadinfo(*activesquad->squad[p]->prisoner);
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

            removesquadinfo(*activesquad->squad[p]);

            printparty();
         }
         else if(yourspeed[p]<theirbest-10)
         {
            clearmessagearea();
            set_color(COLOR_CYAN,COLOR_BLACK,1);
            move(16,1);
            addstr(activesquad->squad[p]->name, gamelog);
            switch(encounter[0].type)
            {
            case CREATURE_COP:
               addstr(" is seized, ", gamelog);
               if(law[LAW_POLICEBEHAVIOR]>=ALIGN_LIBERAL)
               {
                  addstr("pushed to the ground, and handcuffed!", gamelog);
                  gamelog.newline(); //...Newline.
               }
               else
               {
                  if(activesquad->squad[p]->blood<=10)
                  {
                     addstr("thrown to the ground, and tazed to death!", gamelog);
                     gamelog.newline(); //Not another newline!
                  }
                  else
                  {
                     addstr("thrown to the ground, and tazed repeatedly!", gamelog);
                     gamelog.newline(); //Yet another newline...
                  }
                  activesquad->squad[p]->blood-=10;
               }
               break;
            case CREATURE_DEATHSQUAD:
               addstr(" is seized, ", gamelog);
               addstr("thrown to the ground, and shot in the head!", gamelog);
               gamelog.newline(); //New line.
               activesquad->squad[p]->blood=0;
               break;
            case CREATURE_TANK:
               addstr(" crushed beneath the tank's treads!", gamelog);
               gamelog.newline(); //New line.
               activesquad->squad[p]->blood=0;
               break;
            default:
               addstr(" is seized, ", gamelog);
               if(activesquad->squad[p]->blood<=60)
               {
                  addstr("thrown to the ground, and beaten to death!", gamelog);
                  gamelog.newline(); //New line.
               }
               else
               {
                  addstr("thrown to the ground, and beaten senseless!", gamelog);
                  gamelog.newline(); //New line.
               }
               activesquad->squad[p]->blood-=60;
               break;
            }
            if(activesquad->squad[p]->blood<=0)
               activesquad->squad[p]->die();

            capturecreature(*activesquad->squad[p]);
            for(int i=p+1, j=p;i<6;i++, j++)
            {
               activesquad->squad[j]=activesquad->squad[i];
            }
            activesquad->squad[5]=NULL;
            // Death squads don't mess around, and don't fall behind when executing your people
            // Tanks don't stop either.
            if(encounter[0].type!=CREATURE_DEATHSQUAD&&
               encounter[0].type!=CREATURE_TANK)delenc(0,0);

            printparty();
            printchaseencounter();
            refresh();
            getch();
         }
         else othersleft++;
      }
   }
}



int driveskill(Creature &cr,Vehicle &v)
{
   int driveskill=cr.get_skill(SKILL_DRIVING)*(3+v.drivebonus());
   healthmodroll(driveskill,cr);
   if(driveskill<0)driveskill=0;
   driveskill*=static_cast<int>(cr.blood/50.0);
   return driveskill;
}



char drivingupdate(short &obstacle)
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
         if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id())
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
            if(driveskill(*activesquad->squad[passenger[p]],*chaseseq.friendcar[v])>max&&
               activesquad->squad[passenger[p]]->canwalk())
            {
               max=driveskill(*activesquad->squad[passenger[p]],*chaseseq.friendcar[v]);
            }
         }
         for(p=0;p<passenger.size();p++)
         {
            if(driveskill(*activesquad->squad[passenger[p]],*chaseseq.friendcar[v])==max&&
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
            addstr(activesquad->squad[p]->name, gamelog);
            addstr(" takes over the wheel.", gamelog);
            gamelog.newline(); //New line.
            printparty();
            refresh();
            getch();
         }
      }
      if(driver==-1)
      {
         crashfriendlycar(v);
         sitestory->crime.push_back(CRIME_CARCHASE);
         return 1;
      }
   }

   for(v=chaseseq.enemycar.size()-1;v>=0;v--)
   {
      passenger.clear();
      driver=-1;
      for(p=0;p<ENCMAX;p++)
      {
         if(!encounter[p].exists)continue;
         if(encounter[p].carid==chaseseq.enemycar[v]->id())
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

   return 0;
}



void makechasers(long sitetype,long sitecrime)
{
   for(int e=0;e<ENCMAX;e++)encounter[e].exists=0;

   if(sitecrime==0)return;

   short encslot=0;
   int n;

   string cartype; //Temporary (transitionally) solution. -XML
   long pnum;

   chaseseq.canpullover=0;
   // 50% of CCS harassing your teams once they reach the
   // "attacks" stage (but not for activities, which are
   // law enforcement response specific)
   if(endgamestate<ENDGAME_CCS_DEFEATED&&
      endgamestate>=ENDGAME_CCS_ATTACKS&&
      LCSrandom(2)&&sitetype!=-1)
   {
      cartype="SUV"; //A CCS property, not a vehicle property. Temporary solution -XML
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
         case SITE_GOVERNMENT_ARMYBASE:
            cartype="HMMWV"; //Site property? Temporary solution. -XML
            pnum=LCSrandom(sitecrime/5 + 1)+3;
            if(pnum>6)pnum=6;
            for(n=0;n<pnum;n++)
            {
               makecreature(encounter[encslot],CREATURE_SOLDIER);
               encslot++;
            }
            break;
         case SITE_GOVERNMENT_INTELLIGENCEHQ:
            cartype="AGENTCAR"; //Site property? Temporary solution. -XML
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
            if(LCSrandom(2))cartype="SUV"; //Site property? Temporary solution. -XML
            else cartype="JEEP"; //Site property? Temporary solution. -XML
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
            cartype="PICKUP"; //Site property? Temporary solution. -XML
            pnum=LCSrandom(sitecrime/3 + 1)+1;
            if(pnum>18)pnum=18;
            for(n=0;n<pnum;n++)
            {
               makecreature(encounter[encslot],CREATURE_HICK);
               encslot++;
            }
            break;
         case SITE_BUSINESS_CRACKHOUSE:
            cartype="STATIONWAGON"; //Site property? Temporary solution. -XML
            pnum=LCSrandom(sitecrime/3 + 1)+1;
            if(pnum>18)pnum=18;
            for(n=0;n<pnum;n++)
            {
               makecreature(encounter[encslot],CREATURE_GANGMEMBER);
               encslot++;
            }
            break;
         default:
            chaseseq.canpullover=1;
            cartype="POLICECAR"; //Police property? Temporary solution. -XML
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
      Vehicle *v=new Vehicle(*vehicletype[getvehicletype(cartype)]); //If car type is unknown, due to change in xml file, the game will crash here. -XML
      chaseseq.enemycar.push_back(v);

      for(n=0;n<pnum;n++)
      {
         if(encounter[n].carid==-1)
         {
            encounter[n].carid=v->id();
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
            encounter[n].carid=chaseseq.enemycar[v]->id();
            encounter[n].is_driver=0;
         }while(load[v]>=4);
         load[v]++;
      }
   }
}



char obstacledrive(short obstacle,char choice)
{
   switch(obstacle)
   {
      case CARCHASE_OBSTACLE_CROSSTRAFFIC:
         if(choice==0)
         {
            if(dodgedrive())return 1;
         }
         else if(choice==1)
         {
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("You slow down, and turn the corner.", gamelog);
            gamelog.newline(); //New line.
            refresh();
            getch();

            if(!LCSrandom(3))
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(17,1);
               addstr("Here they come!", gamelog);
               gamelog.newline(); //New line.
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
            if(dodgedrive())return 1;
         }
         else if(choice==1)
         {
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("You slow down, and carefully evade the truck.", gamelog);
            gamelog.newline(); //New line.
            refresh();
            getch();

            if(!LCSrandom(3))
            {
               set_color(COLOR_YELLOW,COLOR_BLACK,1);
               move(17,1);
               addstr("Here they come!", gamelog);
               gamelog.newline(); //New line.
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
            if(dodgedrive())return 1;
         }
         else if(choice==1)
         {
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("Fruit smashes all over the windshield!", gamelog);
            gamelog.newline(); //New line.
            refresh();
            getch();

            if(!LCSrandom(5))
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               move(17,1);
               addstr("The fruit seller is squashed!", gamelog);
               gamelog.newline(); //All this logging and lining...
               refresh();
               getch();

               criminalizeparty(LAWFLAG_MURDER);
            }
         }
         break;
      case CARCHASE_OBSTACLE_CHILD:
         if(choice==0)
         {
            if(dodgedrive())return 1;
         }
         else if(choice==1)
         {
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr("You slow down and carefully avoid the kid.", gamelog);
            gamelog.newline(); //New line.
            refresh();
            getch();

            if(!LCSrandom(3))
            {
               set_color(COLOR_RED,COLOR_BLACK,1);
               move(17,1);
               addstr("The kid screams as a hail of gunfire breaks out!", gamelog);
               gamelog.newline(); //New line.
               refresh();
               getch();
               enemyattack();
               youattack();
            }
            else
            {
               set_color(COLOR_GREEN,COLOR_BLACK,1);
               move(17,1);
               addstr("Both sides refrain from exchanging fire...", gamelog);
               gamelog.newline(); //New line.
               refresh();
               getch();
            }
         }
         break;
   }
   return 0;
}



char dodgedrive(void)
{
   int v;
   clearmessagearea();
   set_color(COLOR_YELLOW,COLOR_BLACK,1);
   move(16,1);
   addstr("You swerve to avoid the obstacle!", gamelog);
   gamelog.newline(); //New line.
   refresh();
   getch();

   int driver;
   for(v=chaseseq.friendcar.size()-1;v>=0;v--)
   {
      driver=-1;
      for(int p=0;p<6;p++)
      {
         if(activesquad->squad[p]==NULL)continue;
         if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id())
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
         if(!activesquad->squad[driver]->skill_check(SKILL_DRIVING,DIFFICULTY_EASY))
         {
            crashfriendlycar(v);
            sitestory->crime.push_back(CRIME_CARCHASE);
            return 1;
         }
      }
   }

   for(v=chaseseq.enemycar.size()-1;v>=0;v--)
   {
      driver=-1;
      for(int p=0;p<ENCMAX;p++)
      {
         if(!encounter[p].exists)continue;
         if(encounter[p].carid==chaseseq.enemycar[v]->id())
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
         if(!encounter[driver].skill_check(SKILL_DRIVING,DIFFICULTY_EASY))
         {
            crashenemycar(v);
            sitestory->crime.push_back(CRIME_CARCHASE);
         }
      }
   }
   return 0;
}



void crashfriendlycar(int v)
{
   //CRASH CAR
   clearmessagearea();
   set_color(COLOR_MAGENTA,COLOR_BLACK,1);
   move(16,1);
   addstr("Your ", gamelog);
   addstr(chaseseq.friendcar[v]->fullname().c_str(), gamelog);
   switch(LCSrandom(3))
   {
      case 0:addstr(" slams into a building!", gamelog);break;
      case 1:addstr(" skids out and crashes!", gamelog);break;
      case 2:addstr(" hits a parked car and flips over!", gamelog);break;
   }
   gamelog.newline(); //New line it.
   printparty();
   refresh();
   getch();

   int victimsum=0;

   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      if(activesquad->squad[p]->carid==chaseseq.friendcar[v]->id())
      {
         // Inflict injuries on Liberals
         for(int w=0;w<BODYPARTNUM;w++)
         {
            // If limb is intact
            if(!(activesquad->squad[p]->wound[w] & (WOUND_CLEANOFF | WOUND_NASTYOFF)))
            {
               // Inflict injuries
               if(LCSrandom(2))
               {
                  activesquad->squad[p]->wound[w] |= WOUND_TORN;
                  activesquad->squad[p]->wound[w] |= WOUND_BLEEDING;
                  activesquad->squad[p]->blood -= 1 + LCSrandom(25);
               }
               if(!LCSrandom(3))
               {
                  activesquad->squad[p]->wound[w] |= WOUND_CUT;
                  activesquad->squad[p]->wound[w] |= WOUND_BLEEDING;
                  activesquad->squad[p]->blood -= 1 + LCSrandom(25);
               }
               if(LCSrandom(2) || activesquad->squad[p]->wound[w] == 0)
               {
                  activesquad->squad[p]->wound[w] |= WOUND_BRUISED;
                  activesquad->squad[p]->blood -= 1 + LCSrandom(10);
               }
            }
         }

         // Kill off hostages
         if(activesquad->squad[p]->prisoner!=NULL)
         {
            // Instant death
            if(activesquad->squad[p]->prisoner->alive)
            {
               clearmessagearea();
               set_color(COLOR_RED,COLOR_BLACK,1);
               move(16,1);
               addstr(activesquad->squad[p]->prisoner->name, gamelog);
               switch(LCSrandom(3))
               {
                  case 0:addstr(" is crushed inside the car.", gamelog);break;
                  case 1:addstr("'s lifeless body smashes through the windshield.", gamelog);break;
                  case 2:addstr(" is thrown from the car and killed instantly.", gamelog);break;
               }
               gamelog.newline(); //New line.
               printparty();
               refresh();
               getch();
            }
            activesquad->squad[p]->prisoner->die();
            victimsum++;
            
            // Record death if living Liberal is hauled
            if(activesquad->squad[p]->prisoner->squadid!=-1)
            {
               if(activesquad->squad[p]->prisoner->alive&&
                  activesquad->squad[p]->prisoner->align==1)stat_dead++;

               activesquad->squad[p]->prisoner->die();
               activesquad->squad[p]->prisoner->location=-1;
            }
            // Otherwise just kill them off and be done with it
            else delete activesquad->squad[p]->prisoner;
            activesquad->squad[p]->prisoner=NULL;
         }

         // Handle squad member death
         if(activesquad->squad[p]->blood <= 0)
         {
            // Inform the player
            clearmessagearea();
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(16,1);
            addstr(activesquad->squad[p]->name, gamelog);
            switch(LCSrandom(3))
            {
               case 0:addstr(" slumps in ", gamelog);
                  if(activesquad->squad[p]->gender_liberal==GENDER_MALE)
                     addstr("his", gamelog);
                  else if(activesquad->squad[p]->gender_liberal==GENDER_FEMALE)
                     addstr("her", gamelog);
                  else
                     addstr("their", gamelog);
                  addstr(" seat, out cold, and dies.", gamelog);break;
               case 1:addstr(" is crushed by the impact.", gamelog);break;
               case 2:addstr(" struggles free of the car, then collapses lifelessly.", gamelog);break;
            }
            gamelog.newline(); //New line.
            printparty();
            refresh();
            getch();

            // Mark as dead
            activesquad->squad[p]->die();
            victimsum++;

            // Account for deaths for high score
            if(activesquad->squad[p]->align == ALIGN_LIBERAL)stat_dead++;

            // Remove dead Liberal from squad
            activesquad->squad[p]=NULL;
         }
         else
         {
            // Inform the player of character survival
            clearmessagearea();
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            move(16,1);
            addstr(activesquad->squad[p]->name, gamelog);
            switch(LCSrandom(3))
            {
               case 0:
                  addstr(" grips the ", gamelog);
                  if(activesquad->squad[p]->is_armed())
                  {
                     addstr(activesquad->squad[p]->get_weapon().get_shortname().c_str(), gamelog);
                  }
                  else
                     addstr("car frame", gamelog);
                  addstr(" and struggles to ", gamelog);
                  if(activesquad->squad[p]->gender_liberal==GENDER_MALE)
                     addstr("his", gamelog);
                  else if(activesquad->squad[p]->gender_liberal==GENDER_FEMALE)
                     addstr("her", gamelog);
                  else
                     addstr("their", gamelog);
                  if(activesquad->squad[p]->flag & CREATUREFLAG_WHEELCHAIR)
                     addstr(" wheelchair.", gamelog);
                  else
                     addstr(" feet.", gamelog);
                  break;
               case 1:
                  addstr(" gasps in pain, but lives, for now.", gamelog);
                  break;
               case 2:addstr(" crawls free of the car, shivering with pain.", gamelog);
                  activesquad->squad[p]->drop_weapon(NULL);
                  break;
            }
            gamelog.newline(); //New line.
            printparty();
            refresh();
            getch();
         }
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

   //GET RID OF CARS
   for(int v2=0;v2<chaseseq.friendcar.size();v2++)
   {
      for(int v3=(int)vehicle.size()-1;v3>=0;v3--)
      {
         if(vehicle[v3]==chaseseq.friendcar[v2])
         {
            delete vehicle[v3];
            vehicle.erase(vehicle.begin() + v3);
         }
      }
   }
   chaseseq.friendcar.clear();
   for(int p=0;p<6;p++)
   {
      if(activesquad->squad[p]==NULL)continue;
      activesquad->squad[p]->carid=-1;
   }
}



void crashenemycar(int v)
{
   string str = chaseseq.enemycar[v]->fullname();

   int victimsum=0;

   for(int p=ENCMAX-1;p>=0;p--)
   {
      if(!encounter[p].exists)continue;
      if(encounter[p].carid==chaseseq.enemycar[v]->id())
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
   addstr("The ", gamelog);
   addstr(str.c_str(), gamelog);
   switch(LCSrandom(3))
   {
      case 0:addstr(" slams into a building.", gamelog);break;
      case 1:
         addstr(" spins out and crashes.", gamelog);
         move(17,1);
         if(victimsum>1)addstr("Everyone inside is peeled off against the pavement.", gamelog);
         else if(victimsum==1)addstr("The person inside is squashed into a cube.", gamelog);
         break;
      case 2:addstr(" hits a parked car and flips over.", gamelog);break;
   }
   gamelog.newline(); //New line.
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
      activesquad->squad[p]->drop_weapons_and_clips(NULL);
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
   if(mode!=GAMEMODE_CHASECAR)addstr("You stop and are arrested.", gamelog);
   else addstr("You pull over and are arrested.", gamelog);
   gamelog.newline(); //New line.
   if(hostagefreed>0)
   {
      move(17,1);
      addstr("Your hostage", gamelog);
      if(hostagefreed>1)
         addstr("s are free.", gamelog);
      else
         addstr(" is free.", gamelog);
      gamelog.newline(); //New line.
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



char chasesequence(Creature &cr,Vehicle &v)
{
   long oldsqid=cr.squadid;
   squadst *sq=new squadst;
      sq->squad[0]=&cr;
      sq->squad[0]->squadid=cursquadid;
      sq->id=cursquadid;cursquadid++;
      cr.carid=v.id();
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
   gamelog.nextMessage(); //Next message.
   return ret;
}
