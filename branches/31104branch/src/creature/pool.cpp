
#include <map>
#include "includes.h"

class pool
{
private:
   std::map<int,creaturest*> map;
   void dispersal_ok(creaturest* cr)
   {
      for(int i=0; i<
      cr.subordinates
   }
public:
   creaturest* getFounder(void)
   {
      pool::iterator i = map.begin();
      while((*i).second->hireid!=-1)i++;
      return (*i).second;
   }
   creaturest* operator[](int i)
   {
      return map[i];
   }
   void dispersalcheck(char &clearformess)
   {
      if(size()!=0)
      {
         
      }
   }
};



/* squad members with no chain of command lose contact */
void dispersalcheck(char &clearformess)
{
   int p = 0;
   //NUKE DISPERSED SQUAD MEMBERS WHOSE MASTERS ARE NOT AVAILABLE
   if(pool.size()>0)
   {
      // *JDS* I'm documenting this algorithm carefully because it
      // took me awhile to figure out what exactly was going on here.
      //
      // nukeme tracks whether each person has a secure chain of command.
      // 
      // if nukeme == 1, no confirmation of contact has been made
      // if nukeme == 0, confirmation that THEY are safe is given,
      //    but it is still needed to check whether their subordinates
      //    can reach them.
      // if nukeme == -1, confirmation has been made that this squad
      //    member is safe, and their immediate subordinates have also
      //    checked.
      //
      // The way the algorithm works, everyone starts at nukeme = 1.
      // Then we start at the top of the chain of command and walk
      // down it slowly, marking people 0 and then -1 as we sweep
      // down the chain. If someone is dead or in an unreachable state,
      // they block progression down the chain to their subordinates,
      // preventing everyone who requires contact with that person
      // from being marked safe. After everyone reachable has been
      // reached and marked safe, all remaining squad members are nuked.
      vector<int> nukeme;
      nukeme.resize(pool.size());

      bool promotion;
      do
      {
         promotion=0;
         for(p=0;p<pool.size();p++)
         {
            // Default: members are marked nukeme = 1
            //(no contact verified)
            nukeme[p]=DISPERSAL_NOCONTACT;
            // If member has no boss (founder level), mark
            // them nukeme = 0, using them as a starting point
            // at the top of the chain.
            if(pool[p]->hireid==-1)nukeme[p]=DISPERSAL_BOSSSAFE;
            // If they're dead, mark them nukeme = -1, so they
            // don't ever have their subordinates checked
            // and aren't lost themselves (they're a corpse,
            // corpses don't lose contact)
            if(!pool[p]->alive)
            {
               nukeme[p]=DISPERSAL_SAFE;
               //Attempt to promote their subordinates
               if(promotesubordinates(*pool[p], clearformess))promotion=1;

               if(pool[p]->location==-1 || location[pool[p]->location]->renting == -1)
               {
                  delete pool[p];
                  pool.erase(pool.begin() + p);
               }
            }
         }
      }while(promotion);

      char changed;

      do // while(changed!=0)
      {
         changed=0;

         char inprison;

         // Go through the entire pool to locate people at nukeme = 0,
         // so we can verify that their subordinates can reach them.
         for(p=pool.size()-1;p>=0;p--)
         {
            if(pool[p]->location!=-1&&location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON)
               inprison=1;
            else inprison=0;

            // If your boss is in hiding
            if(nukeme[p]==DISPERSAL_BOSSINHIDING)
            {
               nukeme[p]=DISPERSAL_HIDING;
               for(int p2=pool.size()-1;p2>=0;p2--)
               {
                  if(pool[p2]->hireid==pool[p]->id && pool[p2]->alive)
                  {
                     nukeme[p2]=DISPERSAL_BOSSINHIDING; // Mark them as unreachable
                     changed=1; // Need another iteration
                  }
               }
            }

            // If in prison or unreachable due to a member of the command structure
            // above being in prison
            else if(nukeme[p]==DISPERSAL_BOSSSAFE&&inprison||nukeme[p]==DISPERSAL_BOSSINPRISON)
            {
               // If you're here because you're unreachable, mark as checked and unreachable
               if(!inprison)
               {
                  // Roll to see if you go into hiding or not
                  if(!pool[p]->hiding&&
                     pool[p]->attval(ATTRIBUTE_HEART)*5+
                     pool[p]->skill[SKILL_SURVIVAL]*10+
                     pool[p]->juice<LCSrandom(200))
                  {
                     nukeme[p]=DISPERSAL_NOCONTACT;//Vanish forever
                  }
                  else nukeme[p]=DISPERSAL_HIDING;//Go into hiding
               }
               else nukeme[p]=DISPERSAL_SAFE; // Else you're in prison; you're guaranteed contactable
               
               // Find all subordinates if you didn't lose contact completely
               if(nukeme[p]!=DISPERSAL_NOCONTACT)
               {
                  for(int p2=pool.size()-1;p2>=0;p2--)
                  {
                     if(pool[p2]->hireid==pool[p]->id && pool[p2]->alive)
                     {
                        nukeme[p2]=DISPERSAL_BOSSINHIDING; // Mark them as unreachable
                        changed=1; // Need another iteration
                     }
                  }
               }
            }
            // Otherwise, if they're reachable
            else if(nukeme[p]==0&&!inprison)
            {
               // Start looking through the pool again.
               for(int p2=pool.size()-1;p2>=0;p2--)
               {
                  // Locate each of this person's subordinates.
                  if(pool[p2]->hireid==pool[p]->id)
                  {
                     // Protect them from being dispersed -- their boss is
                     // safe. Their own subordinates will then be considered
                     // in the next loop iteration.
                     nukeme[p2]=DISPERSAL_BOSSSAFE;
                     // If they're hiding indefinately and their boss isn't
                     // hiding at all, then have them discreetly return in a
                     // couple of weeks
                     if(pool[p2]->hiding==-1&&!pool[p]->hiding)
                     {
                        pool[p2]->hiding=LCSrandom(10)+3;
                     }
                     changed=1; // Take note that another iteration is needed.
                  }
               }
               // Now that we've dealt with this person's subordinates, mark
               // them so that we don't look at them again in this loop.
               nukeme[p]=DISPERSAL_SAFE;
            }
         }
      }while(changed); // If another iteration is needed, continue the loop.

      // After checking through the entire command structure, proceed
      // to nuke all squad members who are unable to make contact with
      // the LCS.
      for(p=pool.size()-1;p>=0;p--)
      {
         if(nukeme[p]==1||nukeme[p]==3)
         {
            if(clearformess)
            {
               erase();
            }
            else
            {
               makedelimiter(8,0);
            }

            if(!pool[p]->hiding&&nukeme[p]==3)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(pool[p]->name);
               addstr(" has lost touch with the Liberal Crime Squad.");
               move(9,1);
               addstr("The Liberal has gone into hiding...");
               refresh();
               getch();
            }
            else if(nukeme[p]==1)
            {
               set_color(COLOR_WHITE,COLOR_BLACK,1);
               move(8,1);
               addstr(pool[p]->name);
               addstr(" has lost touch with the Liberal Crime Squad.");
               refresh();
               getch();
            }


            removesquadinfo(*pool[p]);
            if(nukeme[p]==1)
            {
               delete pool[p];
               pool.erase(pool.begin() + p);
            }
            else
            {
               int hs=0;
               for(int l=0;l<location.size();l++)
               {
                  if(location[l]->type==SITE_RESIDENTIAL_SHELTER)
                  {
                     hs=l;
                     break;
                  }
               }
               pool[p]->location=-1;
               pool[p]->base=hs;
               pool[p]->activity.type=ACTIVITY_NONE;
               pool[p]->hiding=-1; // Hide indefinately
            }
         }
      }
   }

   //MUST DO AN END OF GAME CHECK HERE BECAUSE OF DISPERSAL
   endcheck(END_DISPERSED);

   cleangonesquads();
}
