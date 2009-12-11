         // *JDS* LEGAL SELF-REPRESENTATION: Your intelligence, heart, and twice your charisma
         // form the baseline the jury will evaluate your arguments from, and after that it's
         // up to your roll. To succeed here, you really need to have two skills be
         // high: persuasion and law, with law being 1.5 times as influential. You can't have
         // just one or just the other, and that's a change I've made which will make this
         // much harder. Even if you're a very persuasive person, the court will eat you alive
         // if you can't sound intelligent when talking about the relevant charges, and you
         // won't be able to fool the jury into letting you go if you aren't persuasive, as no
         // matter how encyclopedic your legal knowledge is, it's all in the pitch. After these
         // skills are multiplied together, you make a roll on this "defense skill" value; this curves your
         // results toward a central value, and away from extreme good and bad performances.
         // All in all, it is significantly *harder* to defend yourself after these changes,
         // and a very powerful case will require someone have lawyerly skills to begin with.
         // Major note: Defense skill is capped at 100. Once you're rolling d200, adding more
         // would be really unrealistic. You can only be so persuasive.
         //
         // EXAMPLE: Suppose your Liberal has 4 persuasion and 6 law -- the average starting
         // skills for a seduced lawyer. This results in a defenseskill of 51. If their intelligence,
         // heart, and charisma are each four, that makes their roll 16+LCSrandom(102).
         // This is significantly weaker than a paid or Sleeper attorney, but much stronger
         // than the public defender, and about on par with the prosecutor so long as it isn't
         // a high-profile case.

         // A character build spefically to be strong in this area *will* still start out
         // slightly stronger than the public defender (and will be notably better once they
         // hit activist level).


/**********************************************************************
** *JDS*
** ----- The sleeper system has been completely reworked.
** - Sleepers no longer directly inflence the issues. They now affect
** the broad "liberal power" stats across many issues, which are used
** as a kind of monthly liberal roll akin to AM Radio and Cable News.
** - Each sleeper can affect one or more issue, throwing their power
** into the "abstracted debate" on that issue.
** - After all of the sleepers have contributed to the liberal power
** stats, a roll is made on each issue to see whether the liberals
** make background progress on those issues. 
** - Several sleepers have special abilities. Lawyers and Judges, as
** always, can aid your people in the legal system. Police officers,
** corporate managers, CEOs, and agents can all now leak secret
** documents of the appropriate types, and they will make a check
** each month. This will only happen if the homeless shelter is not
** under siege, and "canseethings" is enabled (eg, you're not in prison
** or disbanded or some other situation where your sleeper can't get
** in touch with anyone in your squad).
** - News Anchors and Radio Personalities remain the two most powerful
** sleepers.
**********************************************************************/

// *JDS* This should be expanded to cover
// any situation in which you want a bullet
// proof vest to act as a proper uniform.
// eg, security, etc...
//
// Yes, it would be in some ways cooler to
// do it the other way around -- have
// body armor be a subtype, and use the
// uniform as your main type, but that
// would necessitate modifying the names
// of the armor pieces across many cases.
//
// Perhaps I will change this system,
// so that there is a flag after the
// armor name (like the bloody and damaged
// flags) which signals a vest is being
// worn underneath, but eh... this should
// work for now.


/* *JDS* I'm making laws an array instead of a bunch
 * of bits which are either on or off. Each charge can be
 * duplicated, for multiple counts, and intense focus
 * for wanted persons.
 */

/* *JDS* In addition to laws being an array,
 * each law will be associated with a "heat" value,
 * indicating how much that particular crime draws
 * police attention to the wanted person, and to the
 * LCS as a whole. This is a global array, available
 * at runtime and indexed to the lawflag enum, so that
 * code can dynamically look up these heat values as
 * needed at runtime.
 */


   //*JDS* I'm using VIEWNUM-4 in a random generator that rolls a
   //random issue, not including the media ones, and this will
   //break if these stop being the last 4 issues; do a search
   //for VIEWNUM-4 to change it if it needs to be changed.



      // *JDS* compensate for "to hit" bonus for shotguns and SMG,
      // and "to hit" penalties for assault rifles, by
      // returning the attack roll to normal for the purpose
      // of calculating damage

      // *JDS* Scarefactor is the severity of the case against you; if you're a really
      // nasty person with a wide variety of major charges against you, then scarefactor
      // can get up there



               // *JDS* You get between $1 and $500, plus an extra $1-250 every
               // time you pass a check against your hacking skill, where chance of
               // failure is one over the adjusted hackers' skill divided by four. Once
               // a check fails, no more money is gained. This check will be made
               // up to 20 times, at which point the loop breaks. The skill check
               // here doesn't take into account funding.


               // *JDS* If your squad member is wanted by the police, a conservative who breaks up with
               // them has a 1 in 50 chance of ratting them out, unless the person being dated is law
               // enforcement, prison guard, or agent, in which case there is a 1 in 4 chance.


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

      for(p=pool.size()-1;p>=0;p--)
      {
         // If member is dead or has no boss (founder level), mark
         // them nukeme = 0, using them as a starting point at the top
         // of the chain.
         if(!pool[p]->alive||pool[p]->hireid==-1)nukeme[p]=0;
         // All remaining members are marked nukeme = 1.
         else nukeme[p]=1;
      }

      char changed;

      do // while(changed!=0)
      {
         changed=0;

         char inprison, alive;

         // Go through the entire pool to locate people at nukeme = 0,
         // so we can verify that their subordinates can reach them.
         for(p=pool.size()-1;p>=0;p--)
         {
            if(pool[p]->location!=-1&&location[pool[p]->location]->type==SITE_GOVERNMENT_PRISON)
               inprison=1;
            else inprison=0;

            if(pool[p]->alive)alive=1;
            else alive=0;

            // If in prison or unreachable due to a member of the command structure
            // above being in prison
            if(nukeme[p]==0&&alive&&inprison||nukeme[p]==2)
            {
               // If you're here because you're unreachable, mark as checked and unreachable
               if(!inprison)
               {
                  // Roll to see if you go into hiding or not
                  if(!pool[p]->hiding&&
                     pool[p]->attval(ATTRIBUTE_HEART)*5+pool[p]->juice<LCSrandom(200))
                  {
                     nukeme[p]=1;
                  }
                  else nukeme[p]=3;
               }
               else nukeme[p]=-1; // Else you're in prison; you're guaranteed contactable
               
               // Find all subordinates if you didn't lose contact completely
               if(nukeme[p]!=1)
               {
                  for(int p2=pool.size()-1;p2>=0;p2--)
                  {
                     if(pool[p2]->hireid==pool[p]->id)
                     {
                        nukeme[p2]=2; // Mark them as unreachable
                        changed=1; // Need another iteration
                     }
                  }
               }
            }
            // Otherwise, if they're both alive and reachable
            else if(nukeme[p]==0&&alive&&!inprison)
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
                     nukeme[p2]=0;
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
               nukeme[p]=-1;
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
               pool[p]->hiding=-1; // Hide indefinately
            }
         }
      }
   }

   //MUST DO AN END OF GAME CHECK HERE BECAUSE OF DISPERSAL
   endcheck(END_DISPERSED);
}

