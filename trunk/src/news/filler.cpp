#include "externs.h"

void constructfillerstory(void)
{
   strcpy(story,"");

      switch(LCSrandom(7)
      {

        case 0://VIEW_GUNS:
         {
            cityname(story);
            strcat(" - ");
            strcat(story,"A student has gone on a ");
            if(law[LAW_FREESPEECH]==-2)
                strcat(story,"[violent]");
            else
                strcat(story,"shooting");
            strcat(story," rampage at a local high school.  ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,", ");
            int killerage = LCSrandom(14)+5;
            strcat(story,killerage);
            strcat(story,", used a variety of ");
            if(law[LAW_GUNCONTROL]==1)strcat(story,"illegal ");
            else if(law[LAW_GUNCONTROL]==2)strcat(story,"highly illegal ");
            strcat(story,"weapons to shoot more than a dozen classmates and two teachers at ");
            char jstr[200];
            lastname(jstr);
            strcat(story,jstr);
            strcat(story," High School, before committing suicide.&r");

            strcat(story,"  ");
            strcat(story,dstr);
            strcat(story," was considered an outcast ");
            break;
         }

         case 1://VIEW_PRISONS:
         {
            cityname(story);
            strcat(story," - A former prisoner has written a book describing in horrifying ");
            strcat(story,"detail what goes on behind bars.  ");
            strcat(story,"Although popular culture has used, or perhaps overused, the ");
            strcat(story,"prison theme lately in its offerings for mass consumption, rarely ");
            strcat(story,"have these works been as poignant as ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,"'s new tour-de-force, _");
            switch(LCSrandom(6))
            {
               case 0:strcat(story,"Nightmare");break;
               case 1:strcat(story,"Primal");break;
               case 2:strcat(story,"Animal");break;
               case 3:strcat(story,"American");break;
               case 4:strcat(story,"Solitary");break;
               case 5:strcat(story,"Painful");break;//Painful Soap, anyone?
            }
            strcat(story,"_");
            switch(LCSrandom(7))
            {
               case 0:strcat(story,"Packer");break;
               case 1:strcat(story,"Soap");break;//Nightmare Soap, anyone?
               case 2:strcat(story,"Punk");break;
               case 3:strcat(story,"Kid");break;
               case 4:strcat(story,"Cell");break;
               case 5:strcat(story,"Shank");break;
               case 6:strcat(story,"Lockdown");break;
               case 7:
                      if(law[LAW_FREESPEECH]==-2)strcat(story,"[Bum]lord");
                      else strcat(story,"Buttlord");break;
            }
            strcat(story,"_.&r");
            strcat(story,"   Take this excerpt, \"");//TODO: Add more excerpts.

            strcat(story,"The steel bars grated forward in their rails, ");
            strcat(story,"coming to a halt with a deafening clang that said it all -- ");
            strcat(story,"I was trapped with them now.  There were three, looking me over ");
            strcat(story,"with dark glares of bare lust, as football players might stare at a stupefied, drunken, helpless teenager.  ");
            strcat(story,"My shank's under the mattress.  Better to be a man and fight or a punk and let them take it?  ");
            strcat(story,"Maybe lose an eye the one way, maybe catch ");
            if(law[LAW_GAY]==-2)strcat(story,"GRIDS");// Gay Related Immunodeficiency Syndrome, an obsoleted/politically incorrect name for "AIDS".
            else strcat(story,"AIDS");
            strcat(story," the other.  A ");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"[heck]uva");
            else strcat(story,"helluva");
            strcat(story," choice, and I would only have a few seconds before they made it for me");
            strcat(story,".\"");
            strcat(story,"&r");
            break;
         }
         case 2://VIEW_POLITICALVIOLENCE:
         {
            cityname(story);
            strcat(story," - The dismissal of ");
            char gender;
            if(LCSrandom(2))
            {
               gender=GENDER_MALE;
               strcat(story,"his");
            }
            else
            {
               gender=GENDER_FEMALE;
               strcat(story,"her");
            }
            strcat(story," final appeal ended the tragic tale of ");
            char dstr[200],nstr[200];
            firstname(nstr,gender);
            strcat(story,nstr);
            strcat(story," ");
            lastname(dstr);
            strcat(story,dstr);
            strcat(story," today. ");
            strcat(story,dstr);
            strcat(story,", ");
            itoa(30+LCSrandom(40),nstr,10);
            strcat(story,nstr);
            strcat(story,", is the central figure of what many consider to be the greatest political "
                         "scandal in years. A successful owner of a French language-themed bookshop "
                         "until two years ago, ");
            strcat(story,dstr);
            strcat(story,"'s downfall began when local police unlawfully confiscated much of ");
            if(gender==GENDER_MALE)
               strcat(story,"his");
            else
               strcat(story,"her");
            strcat(story," stock for 'Un-American content'. Attempts to take the matter to the legal arena "
                         "failed after the judge - who happened to be a brother-in-law of one of the "
                         "accused officers - threw out the case and the state supreme court declined to "
                         "get involved despite widespread media publication of the case.");
            strcat(story,"&r "); // comment this line to create a wall of text
            strcat(story,"  Four months ago, a disillusioned and bankrupt ");
            strcat(story,dstr);
            strcat(story," was caught in the act of vandalizing the courthouse windows with a baseball bat. ");
            if(gender==GENDER_MALE)
               strcat(story,"He");
            else
               strcat(story,"She");
            strcat(story," was subsequently convicted of breaking the peace, vandalism, attempted murder "
                         "and terrorism and sentenced to life in prison. The presiding judge was a childhood "
                         "friend of (continued: Nightmare, A6)");
            strcat(story,"&r");
            break;
         }
         case 3://VIEW_PRISONS:
         {
            cityname(story);
            strcat(story," - The hostage crisis at the ");
            char jstr[200];
            lastname(jstr);
            strcat(story,jstr);
            strcat(story," Correctional Facility ended tragically yesterday with the ");
            strcat(story,"death of both the prison guard being held hostage and ");
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"his");break;
               case 1:strcat(story,"her");break;
            }
            strcat(story," captor.");
            strcat(story,"&r");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"   Two weeks ago, convicted [reproduction fiend] ");
            else strcat(story,"   Two weeks ago, convicted rapist ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,", an inmate at ");
            strcat(story,jstr);
            strcat(story,", overpowered ");
            char gstr[200],gstr2[200];
            firstname(gstr);lastname(gstr2);
            strcat(story,gstr);
            strcat(story," ");
            strcat(story,gstr2);
            strcat(story," and barricaded ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"himself");break;
               case 1:strcat(story,"herself");break;
               case 2:strcat(story,"themselves");break;
            }
            strcat(story," with the guard in a prison tower.  ");
            strcat(story,"Authorities locked down the prison and ");
            strcat(story,"attempted to negotiate by phone for 13 days, ");
            strcat(story,"but talks were cut short when ");
            strcat(story,dstr2);
            strcat(story," reportedly screamed into the receiver \"");
            switch(LCSrandom(3))
            {
               case 0:
                      if(law[LAW_FREESPEECH]==2)strcat(story,"Ah, fuck this shit.  This punk bitch is fuckin' dead!");
                      else if(law[LAW_FREESPEECH]==-2)strcat(story,"Ah, [no way.]  This [police officer will be harmed!]");
                      else strcat(story,"Ah, f*ck this sh*t.  This punk b*tch is f*ckin' dead!");break;
               case 1:
                      if(law[LAW_FREESPEECH]==2)strcat(story,"Fuck a muthafuckin' bull.  I'm killin' this pig shit.");
                      else if(law[LAW_FREESPEECH]==-2)strcat(story,"[Too late.]  [I am going to harm this police officer.]");
                      else strcat(story,"F*ck a m*th*f*ck*n' bull.  I'm killin' this pig sh*t.");break;
               case 2:
                      if(law[LAW_FREESPEECH]==2)strcat(story,"Why the fuck am I talkin' to you?  I'd rather kill this pig.");
                      else if(law[LAW_FREESPEECH]==-2)strcat(story,"Why am I talkin' to you?  I'd rather [harm this police officer.]");
                      else strcat(story,"Why the f*ck am I talkin' to you?  I'd rather kill this pig.");break;
            }
            strcat(story,"\"");
            strcat(story,"  The tower was breached in an attempt to reach ");
            strcat(story,"the hostage, but ");
            strcat(story,dstr2);
            strcat(story," had already ");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"[harmed] the guard");
            else if(law[LAW_FREESPEECH]==-1)strcat(story,"killed the guard");
            else switch(LCSrandom(3))
            {
               case 0:strcat(story,"slit the guard's throat with a shank");break;
               case 1:strcat(story,"strangled the guard to death with a knotted bed sheet");break;
               case 2:strcat(story,"chewed out the guard's throat");break;
            }
            strcat(story,".  The prisoner was beaten to death while ");
            strcat(story,"\"resisting capture\", according to a prison spokesperson.");
            strcat(story,"&r");
            break;
         }
         case 4://VIEW_POLITICALVIOLENCE:
         {
            cityname(story);
            strcat(story," - Nine people were killed today as special forces brought the three-day "
                         "hostage crisis at the ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"City Hall");break;
               case 1:
                      char disco[200];
                      firstname(disco);
                      strcat(story,disco);
                      strcat(story," Disco");
                      break;
               case 2:
                      char cafname[200];
                      lastname(cafname);
                      strcat(story,cafname);
                      strcat(story," Cafeteria");
                      break;
               case 3:strcat(story,"Unemployment Office");break;
               case 4:
                      char orphname[200];
                      lastname(orphname);
                      strcat(story,orphname);
                      strcat(story," Orphanage");break;
            }
            strcat(story,", instigated by members of the ");
            switch(LCSrandom(4))
            {
               case 0:strcat(story,"Stalinist Comrade Squad");break;
               case 1:strcat(story,"Radical Feminist Brigades");break;
               case 2:strcat(story,"2nd Amendment Society");break;
               case 3:strcat(story,"Anarcho-Fascist Collective");break;
            }
            strcat(story,", to a bloody end. Prior to the raid, the group had taken over 80 people captive and demanded, "
                         "among other things: safe passage to North Korea, 10 billion dollars in cash, "
                         "2 fully-fueled airliners and the dissolution of the United States of America. "
                         "According to police sources, the raid became necessary after the kidnappers "
                         "revealed they had been executing twenty people at the end of each day of "
                         "non-compliance with their demands.");
            strcat(story,"&r");
            break;
         }
         case 6://VIEW_DEATHPENALTY:
         {
            cityname(story);
            strcat(story," - Perhaps parents can rest easier tonight.  ");
            strcat(story,"The authorities have apprehended their primary suspect in the ");
            strcat(story,"string of brutal child killings that has kept everyone in the area on edge, ");
            strcat(story,"according to a spokesperson for the police department here.  ");
            char dstr[200],dstr2[200],dstr3[200];
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was detained yesterday afternoon, reportedly in possession of ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"pieces of another victim");break;
               case 1:strcat(story,"bloody toys");break;
               case 2:strcat(story,"a child's clothing stained with DNA evidence");break;
               case 3:strcat(story,"seven junior high school yearbooks");break;
               case 4:strcat(story,"two small backpacks");break;
            }
            strcat(story,".  Over twenty children in the past two years have gone missing, ");
            strcat(story,"only to turn up later dead and ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"carved with satanic symbols");break;
               case 1:strcat(story,"sexually mutilated");break;
               case 2:strcat(story,"missing all of their teeth");break;
               case 3:strcat(story,"missing all of their fingers");break;
               case 4:strcat(story,"without eyes");break;
            }
            strcat(story,".  Sources say that the police got a break in the case when ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"a victim called 911 just prior to being slain while still on the phone");break;
               case 1:strcat(story,"the suspect allegedly carved an address into one of the bodies");break;
               case 2:strcat(story,"an eye witness allegedly spotted the suspect luring a victim into a car");break;
               case 3:strcat(story,"a blood trail was found on a road that led them to the suspect's car trunk");break;
               case 4:strcat(story,"they found a victim in a ditch, still clinging to life");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   The district attorney's office has already repeatedly said it will be ");
            strcat(story,"seeking the death penalty in this case.");
            strcat(story,"&r");
            break;
         }
     }
}
