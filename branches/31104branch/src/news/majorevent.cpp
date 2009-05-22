

//#include "includes.h"
#include "externs.h"


/* news - constructs non-LCS related event stories */
void constructeventstory(char *story,short view,char positive)
{
   strcpy(story,"");

   if(positive)
   {
      switch(view)
      {
         case VIEW_WOMEN:
         {
            char str[200],str2[200];
            char gen[50];
            cityname(story);
            strcat(story," - ");
            strcat(story,"A doctor that routinely performed abortions was ruthlessly ");
            strcat(story,"gunned down outside of the ");
            lastname(str);strcat(story,str);
            strcat(story," CLINIC yesterday.  ");
            strcat(story,"Dr. ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            char gn=LCSrandom(2);
            switch(gn)
            {
               case 0:strcpy(gen,"his");break;
               case 1:strcpy(gen,"her");break;
            }
            strcat(story," was walking to ");strcat(story,gen);
            strcat(story," car when, according to police reports, ");
            strcat(story,"shots were fired from a nearby vehicle.  ");
            strcat(story,dstr2);
            strcat(story," was hit ");
            char num[20];
            itoa(LCSrandom(15)+3,num,10);
            strcat(story,num);
            strcat(story," times and died immediately in the parking lot.  ");
            strcat(story,"The suspected shooter, ");
            firstname(str);strcat(story,str);
            strcat(story," ");
            lastname(str2);strcat(story,str2);
            strcat(story,", is in custody.&r");
            strcat(story,"  Witnesses report that ");
            strcat(story,str2);
            strcat(story," remained at the scene after the shooting, screaming ");
<<<<<<< .mine
			if(VIEW_STALIN<=50)
			{
			strcat(story, "anti-Malthusian quotes from Karl Marx at the stunned onlookers. Someone ");
            strcat(story,"called the police on a cellphone and they arrived shortly thereafter.  ");
            strcat(story,str2);
			}
			else
			{
=======
>>>>>>> .r282
            strcat(story,"verses of the Bible at the stunned onlookers.  Someone ");
            strcat(story,"called the police on a cellphone and they arrived shortly thereafter.  ");
            strcat(story,str2);
<<<<<<< .mine
			}
			if(law[LAW_WOMEN]=-2)
			{
				strcat(story," later admitted to being a rogue FBI vigliante, hunting down ");
				strcat(story," abortion doctors instead of just arresting them.&r");
			}
			if(VIEW_STALIN<=50)
			{
            strcat(story," surrendered without a struggle, reportedly saying that Karl Marx's work ");
            strcat(story,"had been completed.&r");	
			}
			else
			{
=======
>>>>>>> .r282
            strcat(story," surrendered without a struggle, reportedly saying that God's work ");
            strcat(story,"had been completed.&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," is survived by ");
            strcat(story,gen);
            strcat(story," ");
            char spouse=LCSrandom(2);
            if(law[LAW_GAY]<=1)
            {
               spouse=1-gn;
            }
            switch(spouse)
            {
               case 0:strcat(story,"husband");break;
               case 1:strcat(story,"wife");break;
            }
            strcat(story," and ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"two");break;
               case 1:strcat(story,"three");break;
               case 2:strcat(story,"five");break;
            }
            strcat(story," children.&r");
            break;
         }
         case VIEW_GAY:
         {
            cityname(story);
            strcat(story," - ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            if(law[LAW_GAY]<=-1)
			{
				strcat(story,", a known homosexual, was ");
			}
            else strcat(story,", a homosexual, was ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"dragged to death behind a pickup truck");break;
               case 1:strcat(story,"burned alive");break;
               case 2:strcat(story,"beaten to death");break;
            }
            strcat(story," here yesterday.  ");
            strcat(story,"A police spokesperson reported that four suspects ");
            strcat(story,"were apprehended after a high speed chase.  Their names ");
            strcat(story,"have not yet been released.");
            strcat(story,"&r");
            strcat(story,"  Witnesses of the freeway chase described the pickup of the alleged ");
            strcat(story,"murderers swerving wildly, ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"throwing beer bottles");break;
               case 1:strcat(story,"urinating out the window");break;
               case 2:strcat(story,"taking swipes");break;
            }
            strcat(story," at the pursuing police cruisers.  ");
            strcat(story,"The chase ended when ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"the suspects ran out of gas, ");break;
               case 1:strcat(story,"the suspects collided with a manure truck, ");break;
               case 2:strcat(story,"the suspects veered into a ditch, ");break;
            }
            strcat(story,"at which point they were taken into custody.  Nobody was seriously injured during the incident.");
            strcat(story,"&r");
            strcat(story,"  Authorities have stated that they will vigorously ");
            strcat(story,"prosecute this case as a hate crime, due to the ");
            strcat(story,"aggravated nature of the offense");
            if(law[LAW_GAY]==-2)strcat(story,", even though being gay is deviant, as we all know.");
            else strcat(story,".");
            strcat(story,"&r");
            break;
         }
         case VIEW_DEATHPENALTY:
         {
            strcat(story,"Texas - ");
            strcat(story,"An innocent citizen has been put to death in the electric chair.  ");
            char dstr[200],dstr2[200],dstr3[200];
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was pronounced dead at 7:06PM yesterday at the ");
            char jstr[200];
            lastname(jstr);
            strcat(story,jstr);
            strcat(story," Correctional Facility.&r");

            strcat(story,"  ");
            strcat(story,dstr3);
            strcat(story," was convicted in ");
            char num[20];
            itoa(year-LCSrandom(11)-10,num,10);
            strcat(story,num);
            strcat(story," of thirteen serial murders.  ");
            strcat(story,"Since then, numerous pieces of exculpatory evidence ");
            strcat(story,"have been produced, including ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"a confession from another convict.  ");break;
               case 1:strcat(story,"a battery of negative DNA tests.  ");break;
               case 2:
                  strcat(story,"an admission from a former prosecutor that ");
                  strcat(story,dstr3);
                  strcat(story," was framed.  ");
                  break;
            }
            strcat(story,"The state still went through with the execution, with a ");
            strcat(story,"spokesperson for the governor saying, ");
            strcat(story,"\"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Let's not forget the convict is colored.  You know how their kind are");break;
               case 1:
                  strcat(story,"The convict is always referred to by three names.  ");
                  strcat(story,"Assassin, serial killer, either way -- guilty.  ");
                  strcat(story,"End of story");
                  break;
               case 2:strcat(story,"The family wants closure.  We don't have time for another trial");break;
            }
            strcat(story,".\"");
            strcat(story,"&r");
            strcat(story,"  Candlelight vigils were held throughout the country last night during the execution, ");
            strcat(story,"and more events are expected this evening.  If there is a bright side to be found from this ");
            strcat(story,"tragedy, it will be that our nation is now evaluating the ease with which people ");
            strcat(story,"can be put to death in this country.");
            strcat(story,"&r");
            break;
         }
        /* case VIEW_GUNS:
         {
            cityname(story);
            strcat(" - ");
            strcat(story,"A student has gone on a shooting rampage at a local high school.  ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,", 15, used a variety of weapons to shoot more than a dozen classmates and two teachers at ");
            char jstr[200];
            lastname(jstr);
            strcat(story,jstr);
            strcat(story," High School, before committing suicide.&r");

            strcat(story,"  ");
            strcat(story,dstr);
            strcat(story," was considered an outcast ");
            char num[20];
            itoa(year-LCSrandom(11)-10,num,10);
            strcat(story,num);
            strcat(story," of thirteen serial murders.  ");
            strcat(story,"Since then, numerous pieces of exculpatory evidence ");
            strcat(story,"have been produced, including ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"a confession from another convict.  ");break;
               case 1:strcat(story,"a battery of negative DNA tests.  ");break;
               case 2:
                  strcat(story,"an admission from a former prosecutor that ");
                  strcat(story,dstr3);
                  strcat(story," was framed.  ");
                  break;
            }
            strcat(story,"The state still went through with the execution, with a ");
            strcat(story,"spokesperson for the governor saying, ");
            strcat(story,"\"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Let's not forget the convict is colored.  You know how their kind are");break;
               case 1:
                  strcat(story,"The convict is always referred to by three names.  ");
                  strcat(story,"Assassin, serial killer, either way -- guilty.  ");
                  strcat(story,"End of story");
                  break;
               case 2:strcat(story,"The family wants closure.  We don't have time for another trial");break;
            }
            strcat(story,".\"");
            strcat(story,"&r");
            strcat(story,"  Candlelight vigils were held throughout the country last night during the execution, ");
            strcat(story,"and more events are expected this evening.  If there is a bright side to be found from this ");
            strcat(story,"tragedy, it will be that our nation is now evaluating the ease with which people ");
            strcat(story,"can be put to death in this country.");
            strcat(story,"&r");
            break;
         } */
         case VIEW_PRISONS:
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
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Nightmare");break;
               case 1:strcat(story,"Primal");break;
               case 2:strcat(story,"Animal");break;
               case 3:strcat(story,"American");break;
               case 4:strcat(story,"Solitary");break;
            }
            strcat(story,"_");
            switch(LCSrandom(7))
            {
               case 0:strcat(story,"Packer");break;
               case 1:strcat(story,"Soap");break;
               case 2:strcat(story,"Punk");break;
               case 3:strcat(story,"Kid");break;
               case 4:strcat(story,"Cell");break;
               case 5:strcat(story,"Shank");break;
               case 6:strcat(story,"Lockdown");break;
               case 7:strcat(story,"Buttlord");break;
            }
            strcat(story,"_.&r");
            strcat(story,"   Take this excerpt, \"");

            strcat(story,"The steel bars grated forward in their rails, ");
            strcat(story,"coming to a halt with a deafening clang that said it all -- ");
            strcat(story,"I was trapped with them now.  There were three, looking me over ");
            strcat(story,"with dark glares of bare lust, as football players might stare at a stupified, drunken, helpless teenager.  ");
            strcat(story,"My shank's under the mattress.  Better to be a man and fight or a punk and let them take it?  ");
            strcat(story,"Maybe lose an eye the one way, maybe catch AIDS the other.  A helluva choice, ");
            strcat(story,"and I would only have a few seconds before they made it for me");
            strcat(story,".\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_INTELLIGENCE:
         {
            strcat(story,"Washington D.C. - The FBI might be keeping tabs on you.  ");
            strcat(story,"This newspaper yesterday received a collection of files from a source in the Federal Bureau of Investigations.  ");
            strcat(story,"The files contain information on which people have been attending demonstrations, organizing ");
            strcat(story,"unions, working for liberal organizations -- even ");
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"buying music with 'Explicit Lyrics' labels.");break;
               case 1:strcat(story,"helping homeless people");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"  More disturbingly, the files make reference to a plan to ");
            strcat(story,"\"deal with the undesirables\", although this phrase is not clarified.  ");
            strcat(story,"&r");
            strcat(story,"  The FBI refused to comment initially, but when confronted with the information, ");
            strcat(story,"a spokesperson stated, \"");
            strcat(story,"Well, you know, there's privacy, and there's privacy.  ");
            strcat(story,"It might be a bit presumptive to assume that ");
            strcat(story,"these files deal with the one and not the other.  ");
            strcat(story,"You think about that before you continue slanging accusations");
            strcat(story,".\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_POLITICALVIOLENCE:
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
            strcat(story," stock for 'unamerican content'. Attempts to take the matter to the legal arena "
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
         case VIEW_FREESPEECH:
         {
            cityname(story);
            strcat(story," - A children's story has been removed from libraries here after ");
            strcat(story,"the city bowed to pressure from religious groups.");
            strcat(story,"&r");
            strcat(story,"   The book, ");
            strcat(story,"_");
            char dstr[200],nstr[200];
            firstname(nstr);
            strcat(story,nstr);
            strcat(story,"_");
            lastname(dstr);
            strcat(story,dstr);
            strcat(story,"_and_the_");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Mysterious");break;
               case 1:strcat(story,"Magical");break;
               case 2:strcat(story,"Golden");break;
               case 3:strcat(story,"Invisible");break;
               case 4:strcat(story,"Wondrous");break;
            }
            strcat(story,"_");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Thing");break;
               case 1:strcat(story,"Stuff");break;
               case 2:strcat(story,"Object");break;
               case 3:strcat(story,"Whatever");break;
               case 4:strcat(story,"Something");break;
            }
            strcat(story,"_, is the third in an immensely popular series by ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"British");break;
               case 1:strcat(story,"Indian");break;
               case 2:strcat(story,"Chinese");break;
               case 3:strcat(story,"Rwandan");break;
               case 4:strcat(story,"Palestinian");break;
            }
            strcat(story," author ");
            char c[2];
            c[0]='A'+LCSrandom(26);
            c[1]='\x0';
            strcat(story,c);
            strcat(story,".");
            c[0]='A'+LCSrandom(26);
            c[1]='\x0';
            strcat(story,c);
            strcat(story,". ");
            lastname(dstr);
            strcat(story,dstr);
            strcat(story,".  ");
            strcat(story,"Although the series is adored by children worldwide, ");
            strcat(story,"some conservatives feel that the books ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"glorify Satan worship and are spawned by demons from the pit.  ");break;
               case 1:strcat(story,"teach children to kill their parents and hate life.  ");break;
               case 2:strcat(story,"cause violence in schools and are a gateway to cocaine use.  ");break;
               case 3:strcat(story,"breed demonic thoughts that manifest themselves as dreams of murder.  ");break;
               case 4:strcat(story,"contain step-by-step instructions to summon the Prince of Darkness.  ");break;
            }
            strcat(story,"In their complaint, the groups cited an incident involving ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"a child that swore in class");break;
               case 1:strcat(story,"a child that said a magic spell at her parents");break;
               case 2:strcat(story,"a child that pushed her older sister");break;
            }
            strcat(story," as key evidence of the dark nature of the book.");
            strcat(story,"&r");
            strcat(story,"   When the decision to ban the book was announced yesterday, ");
            strcat(story,"many area children spontoneously broke into tears.  One child was ");
            strcat(story,"heard saying, \"");
            switch(LCSrandom(2))
            {
               case 0:
                  strcat(story,"Mamma, is ");
                  strcat(story,nstr);
                  strcat(story," dead?");
                  break;
               case 1:
                  strcat(story,"Mamma, why did they kill ");
                  strcat(story,nstr);
                  strcat(story,"?");
                  break;
            }
            strcat(story,"\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_JUSTICES:
         {
            cityname(story);
            strcat(story," - Conservative federal judge ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," has resigned in disgrace after being caught with a prostitute.");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story,", who once ");
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"defied the federal government by putting a Ten Commandments monument in the local federal building");break;
               case 1:strcat(story,"stated that, \"Segregation wasn't the bad idea everybody makes it out to be these days\"");break;
            }
            strcat(story,", was found with ");
            char pstr[200],pstr2[200];
            firstname(pstr);lastname(pstr2);
            strcat(story,pstr);
            strcat(story," ");
            strcat(story,pstr2);
            strcat(story," last week in a hotel during a police sting operation.  ");
            strcat(story,"According to sources familiar with the particulars, ");
            strcat(story,"when police broke into the hotel room they saw ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"\"the most perverse and spine-tingling debauchery imaginable, at least with only two people.\"");break;
               case 1:strcat(story,"the judge relieving himself on the prostitute.");break;
               case 2:strcat(story,"the prostitute hollering like a cowboy astride the judge.");break;
            }
            strcat(story,"  ");
            strcat(story,pstr2);
            strcat(story," reportedly offered ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"the arresting officers money");break;
               case 1:strcat(story,"to let the officers join in");break;
               case 2:strcat(story,"the arresting officers \"favors\"");break;
            }
            strcat(story," in exchange for their silence.");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," could not be reached for comment, although an aid stated that ");
            strcat(story,"the judge would be going on a Bible retreat for a few weeks to ");
            strcat(story,"\"Make things right with the Almighty Father.\"  ");
            strcat(story,"&r");
            break;
         }
         case VIEW_AMRADIO:
         {
            cityname(story);
            strcat(story," - Well-known AM radio personality ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," went off for fifteen minutes in an inexplicable rant ");
            strcat(story,"two nights ago during the syndicated radio program \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Straight");break;
               case 1:strcat(story,"Real");break;
               case 2:strcat(story,"True");break;
            }
            strcat(story," ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Talk");break;
               case 1:strcat(story,"Chat");break;
               case 2:strcat(story,"Discussion");break;
            }
            strcat(story,"\".");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story,"'s monologue for the evening began the way that fans ");
            strcat(story,"had come to expect, with attacks on the \"liberal media establishment\" and ");
            strcat(story,"the \"elite liberal agenda\".  But when the radio icon said, \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"and the Grays are going to take over the planet in the End Times");break;
               case 1:strcat(story,"a liberal chupacabra will suck the blood from us like a goat, a goat!, a goat!");break;
               case 2:strcat(story,"I feel translucent rods passing through my body...  it's like making love to the future");break;
            }
            strcat(story,"\", a former fan of the show, ");
            char nstr[200],nstr2[200];
            firstname(nstr);lastname(nstr2);
            strcat(story,nstr);
            strcat(story," ");
            strcat(story,nstr2);
            strcat(story,", knew that \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"my old hero");break;
               case 1:strcat(story,"my old idol");break;
               case 2:strcat(story,"the legend");break;
            }
            strcat(story," had ");
            switch(LCSrandom(3))
            {
               case 0:
                  strcat(story,"lost ");
                  switch(LCSrandom(2))
                  {
                     case 0:strcat(story,"his");break;
                     case 1:strcat(story,"her");break;
                  }
                  strcat(story," g*dd*mn mind");
                  break;
               case 1:strcat(story,"maybe gone a little off the deep end");break;
               case 2:strcat(story,"probably been listening to Art Bell in the next studio a little too long");break;
            }
            strcat(story,".  After that, it just got worse and worse.\"");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," issued an apology later in the program, but ");
            strcat(story,"the damage might already be done.  ");
            strcat(story,"According to a poll completed yesterday, ");
            strcat(story,"fully half of the host's most loyal supporters ");
            strcat(story,"have decided to leave the program for saner ");
            strcat(story,"pastures.  Of these, many said that they would be switching over ");
            strcat(story,"to the FM band.");
            strcat(story,"&r");
            break;
         }
      }
   }
   else
   {
      switch(view)
      {
         case VIEW_DEATHPENALTY:
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
<<<<<<< .mine
            strcat(story,"seeking ");
				if(law[LAW_DEATHPENALTY]=2)
				{
					strcat(story,"life imprisonment in this case.");
				}
					else
				{
					strcat(story,"the death penalty in this case.");
				}
=======
            strcat(story,"seeking the death penalty in this case.");
>>>>>>> .r282
            strcat(story,"&r");
            break;
         }
         case VIEW_ANIMALRESEARCH:
         {
            cityname(story);
<<<<<<< .mine
            strcat(story," - Researchers ");
			if(law[LAW_ANIMALRESEARCH]=2)
			{
				strcat(story,"from ");
					switch(LCSrandom(5))
				{
					case 0:strcat(story,"Russia");break;
					case 1:strcat(story,"North Korea");break;
					case 2:strcat(story,"Taiwan");break;
					case 3:strcat(story,"Iran");break;
					case 4:strcat(story,"China");break;
				}
				strcat(story," report that they have discovered an amazing new wonder drug. ");
			}
			else
			{
				strcat(story,"here report that they have discovered an amazing new wonder drug.  ");
			}
=======
            strcat(story," - Researchers here report that they have discovered an amazing new wonder drug.  ");
>>>>>>> .r282
            strcat(story,"Called ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Anal");break;
               case 1:strcat(story,"Colo");break;
               case 2:strcat(story,"Lacta");break;
               case 3:strcat(story,"Pur");break;
               case 4:strcat(story,"Loba");break;
            }
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"nephrin");break;
               case 1:strcat(story,"tax");break;
               case 2:strcat(story,"zac");break;
               case 3:strcat(story,"thium");break;
               case 4:strcat(story,"drene");break;
            }
            strcat(story,", the drug apparently ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"boosts intelligence in chimpanzees");break;
               case 1:strcat(story,"corrects erectile dysfunction in chimpanzees");break;
               case 2:strcat(story,"allows chimpanzees to move blocks with their minds");break;
               case 3:strcat(story,"allows chimpanzees to fly short distances");break;
               case 4:strcat(story,"increases the attention span of young chimpanzees");break;
            }
            strcat(story,".  ");
            strcat(story,"&r");
            strcat(story,"   Along with bonobos, chimpanzees are our closest cousins");
            if(law[LAW_FREESPEECH]==-2)strcat(story,", at least according to the now-discredited theory of evolution");
            strcat(story,".  ");
            strcat(story,"Fielding questions about the ethics of their experiments from reporters during a press conference yesterday, ");
            strcat(story,"a spokesperson for the research team stated that, \"It really isn't so bad as all that.  Chimpanzees are very resilient creatures.  ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"The ones that survived are all doing very well");break;
               case 1:strcat(story,"They hardly notice when you drill their brains out, if you're fast");break;
               case 2:strcat(story,"When we started muffling the screams of our subjects, the other chimps all calmed down quite a bit");break;
            }
            strcat(story,".  We have a very experienced research team.  ");
            strcat(story,"While we understand your concerns, any worries are entirely unfounded.  ");
            strcat(story,"I think the media should be focusing on the enormous benefits of this drug.");
            strcat(story,"\"");
            strcat(story,"&r");
            strcat(story,"   The first phase of human trials is slated to begin in a few months.");
            strcat(story,"&r");
            break;
         }
         case VIEW_PRISONS:
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
            strcat(story,"   Two weeks ago, convicted rapist ");
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
            switch(LCSrandom(2))
            {
               case 0:strcat(story,"himself");break;
               case 1:strcat(story,"herself");break;
            }
            strcat(story," with the guard in a prison tower.  ");
            strcat(story,"Authorities locked down the prison and ");
            strcat(story,"attempted to negotiate by phone for 13 days, ");
            strcat(story,"but talks were cut short when ");
            strcat(story,dstr2);
            strcat(story," reportedly screamed into the receiver \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Ah, f*ck this sh*t.  This punk b*tch is f*ckin' dead!");break;
               case 1:strcat(story,"F*ck a m*th*f*ck*n' bull.  I'm killin' this pig sh*t.");break;
               case 2:strcat(story,"Why the f*ck am I talkin' to you?  I'd rather kill this pig.");break;
            }
            strcat(story,"\"");
            strcat(story,"  The tower was breached in an attempt to reach ");
            strcat(story,"the hostage, but ");
            strcat(story,dstr2);
            strcat(story," had already ");
            switch(LCSrandom(3))
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
         case VIEW_POLITICALVIOLENCE:
         {
            cityname(story);
            strcat(story," - Nine people were killed today as special forces brought the three-day "
                         "hostage crisis at the ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"City Hall");break;
               case 1:strcat(story,"Trinity Disco");break;
               case 2:strcat(story,"Henderson Cafeteria");break;
               case 3:strcat(story,"Unemployment Office");break;
               case 4:strcat(story,"Milburn Orphanage");break;
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
         case VIEW_INTELLIGENCE:
         {
            strcat(story,"Washington D.C. - The CIA announced yesterday that it has averted a terror attack that ");
            strcat(story,"would have occurred on American soil.");
            strcat(story,"&r");
            strcat(story,"   According to a spokesperson for the agency, ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"white supremacists");break;
               case 1:strcat(story,"Islamic fundamentalists");break;
               case 2:strcat(story,"outcast goths from a suburban highschool");break;
            }
            strcat(story," planned to ");
            switch(LCSrandom(9))
            {
               case 0:strcat(story,"fly planes into skyscrapers");break;
               case 1:strcat(story,"detonate a fertilizer bomb at a federal building");break;
               case 2:strcat(story,"ram a motorboat loaded with explosives into a warship");break;
               case 3:strcat(story,"detonate explosives on a school bus");break;
               case 4:strcat(story,"blow out a section of a major bridge");break;
               case 5:strcat(story,"kidnap the president");break;
               case 6:strcat(story,"assassinate the president");break;
               case 7:strcat(story,"destroy the Capitol Building");break;
               case 8:strcat(story,"detonate a nuclear bomb in New York");break;
            }
            strcat(story,".  However, intelligence garnered from deep within the mysterious ");
            strcat(story,"terrorist organization allowed the plot to be foiled just days before it ");
            strcat(story,"was to occur.");
            strcat(story,"&r");
            strcat(story,"   The spokesperson further stated, \"");
            strcat(story,"I won't compromise our sources and methods, but let me just say ");
            strcat(story,"that we are grateful to the Congress and this Administration for ");
            strcat(story,"providing us with the tools we need to neutralize these enemies of ");
            strcat(story,"civilization before they can destroy American families.  ");
            strcat(story,"However, let me also say that there's more that needs to be done.  ");
            strcat(story,"The Head of the Agency will be sending a request to Congress ");
            strcat(story,"for what we feel are the essential tools for combating terrorism in ");
            strcat(story,"this new age.");
            strcat(story,"\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_GENETICS:
         {
            cityname(story);
            strcat(story," - The genetic foods industry staged a major event here yesterday ");
            strcat(story,"to showcase its upcoming products.  Over thirty companies set up ");
            strcat(story,"booths and gave talks to wide-eyed onlookers.");
            strcat(story,"&r");
            strcat(story,"   One such corporation, ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Altered");break;
               case 1:strcat(story,"Gene-tech");break;
               case 2:strcat(story,"DNA");break;
               case 3:strcat(story,"Proteomic");break;
               case 4:strcat(story,"Genomic");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Foods");break;
               case 1:strcat(story,"Agriculture");break;
               case 2:strcat(story,"Meals");break;
               case 3:strcat(story,"Farming");break;
               case 4:strcat(story,"Living");break;
            }
            strcat(story,", presented their product, \"");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Mega");break;
               case 1:strcat(story,"Epic");break;
               case 2:strcat(story,"Overlord");break;
               case 3:strcat(story,"Franken");break;
               case 4:strcat(story,"Transcendent");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Rice");break;
               case 1:strcat(story,"Beans");break;
               case 2:strcat(story,"Corn");break;
               case 3:strcat(story,"Wheat");break;
               case 4:strcat(story,"Potatoes");break;
            }
            strcat(story,"\", during an afternoon Power Point presentation.  ");
            strcat(story,"According to the public relations representative speaking, ");
            strcat(story,"this amazing new product actually ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"extends human life by a few minutes every bite");break;
               case 1:strcat(story,"mends split-ends upon digestion.  Hair is also made glossier and thicker");break;
               case 2:strcat(story,"allows people to see in complete darkness");break;
               case 3:strcat(story,"causes a person to slowly attain their optimum weight with repeated use");break;
               case 4:strcat(story,"cures the common cold");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   Spokespeople for the GM corporations were universal ");
            strcat(story,"in their dismissal of the criticism which often follows the industry.  ");
            strcat(story,"One in particular said, \"");
            strcat(story,"Look, these products are safe.  That thing about the ");
            switch(LCSrandom(4))
            {
               case 0:strcat(story,"guy going on a killing spree");break;
               case 1:strcat(story,"gal turning blue and exploding");break;
               case 2:strcat(story,"guy speaking in tongues and worshipping Satan");break;
               case 3:strcat(story,"gal having a ruptured intestine");break;
            }
            strcat(story," is just a load of ");
            switch(LCSrandom(4))
            {
               case 0:strcat(story,"hooey");break;
               case 1:strcat(story,"poppycock");break;
               case 2:strcat(story,"horse radish");break;
               case 3:strcat(story,"skunk weed");break;
            }
            strcat(story,".  Would we stake the reputation of our company on unsafe products?  ");
            strcat(story,"No.  That's just ridiculous.  I mean, sure companies have put unsafe products out, ");
            strcat(story,"but the GM industry operates at a higher ethical standard.  That goes without saying.");
            strcat(story,"\"");
            strcat(story,"&r");
            break;
         }
         case VIEW_JUSTICES:
         {
            cityname(story);
            strcat(story," - The conviction of confessed serial killer ");
            char dstr[200],dstr2[200],dstr3[200];
            firstname(dstr);firstname(dstr2);lastname(dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was overturned by a federal judge yesterday.  ");
            strcat(story,"Justice ");
            char jstr[200],jstr2[200];
            firstname(jstr);lastname(jstr2);
            strcat(story,jstr);
            strcat(story," ");
            strcat(story,jstr2);
            strcat(story," of the notoriously liberal circuit of appeals here ");
            strcat(story,"made the decision based on ");
            char gen[20];
            switch(LCSrandom(2))
            {
               case 0:strcpy(gen,"his");break;
               case 1:strcpy(gen,"her");break;
            }
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"ten-year-old eye witness testimony");break;
               case 1:strcat(story,gen);strcat(story," general feeling about police corruption");break;
               case 2:strcat(story,gen);strcat(story," belief that the crimes were a right-wing conspiracy");break;
               case 3:
                  strcat(story,gen);
                  strcat(story," belief that ");
                  strcat(story,dstr3);
                  strcat(story," deserved another chance");
                  break;
               case 4:
                  strcat(story,gen);
                  strcat(story," personal philosophy of liberty");break;
            }
            strcat(story,", despite the confession of ");
            strcat(story,dstr3);
            strcat(story,", which even Justice ");
            strcat(story,jstr2);
            strcat(story," grants was not coerced in any way.&r");

            strcat(story,"   Ten years ago, ");
            strcat(story,dstr3);
            strcat(story," was convicted of the now-infamous ");
            char sstr[200];
            lastname(sstr);
            strcat(story,sstr);
            strcat(story," slayings.  ");
            strcat(story,"After an intensive manhunt, ");
            strcat(story,dstr3);
            strcat(story," was found with the murder weapon, ");
            strcat(story,"covered in the victims' blood.  ");
            strcat(story,dstr3);
            strcat(story," confessed and was sentenced to life, saying \"");
            strcat(story,"Thank you for saving me from myself.  ");
            strcat(story,"If I were to be released, I would surely kill again.\"&r");
            strcat(story,"   A spokesperson for the district attorney ");
            strcat(story,"has stated that the case will not be retried, due ");
            strcat(story,"to the current economic doldrums that have left the state ");
            strcat(story,"completely strapped for cash.&r");
            break;
         }
         case VIEW_POLLUTION:
            cityname(story);
            strcat(story," - Pollution might not be so bad after all.  The ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"American");break;
               case 1:strcat(story,"United");break;
               case 2:strcat(story,"Patriot");break;
               case 3:strcat(story,"Family");break;
               case 4:strcat(story,"Children's");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Heritage");break;
               case 1:strcat(story,"Enterprise");break;
               case 2:strcat(story,"Freedom");break;
               case 3:strcat(story,"Liberty");break;
               case 4:strcat(story,"Charity");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Partnership");break;
               case 1:strcat(story,"Institute");break;
               case 2:strcat(story,"Consortium");break;
               case 3:strcat(story,"Forum");break;
               case 4:strcat(story,"Center");break;
            }
            strcat(story," recently released a wide-ranging report detailing recent trends ");
            strcat(story,"and the latest science on the issue.  ");
            strcat(story,"Among the most startling of the think tank's findings is that ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"a modest intake of radiactive waste");break;
               case 1:strcat(story,"a healthy dose of radiation");break;
               case 2:strcat(story,"a bath in raw sewage");break;
               case 3:strcat(story,"watching animals die in oil slicks");break;
               case 4:strcat(story,"inhaling carbon monoxide");break;
            }
            strcat(story," might actually ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"purify the soul");break;
               case 1:strcat(story,"increase test scores");break;
               case 2:strcat(story,"increase a child's attention span");break;
               case 3:strcat(story,"make children behave better");break;
               case 4:strcat(story,"make shy children fit in");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   When questioned about the science behind these results, ");
            strcat(story,"a spokesperson stated that, \"");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"Research is complicated, and there are always two ways to think about things");break;
               case 1:strcat(story,"The jury is still out on pollution.  You really have to keep an open mind");break;
               case 2:strcat(story,"They've got their scientists, and we have ours.  The issue of pollution is wide open as it stands today");break;
            }
            strcat(story,".  You have to realize that ");
            switch(LCSrandom(3))
            {
               case 0:strcat(story,"the elitist liberal media often distorts");break;
               case 1:strcat(story,"the vast left-wing education machine often distorts");break;
               case 2:strcat(story,"the fruits, nuts, and flakes of the environmentalist left often distort");break;
            }
            strcat(story," these issues to their own advantage.  ");
            strcat(story,"All we've done is introduced a little clarity into the ongoing debate.  ");
            strcat(story,"Why is there contention on the pollution question?  It's because ");
            strcat(story,"there's work left to be done.  We should study much more ");
            strcat(story,"before we urge any action.  Society really just ");
            strcat(story,"needs to take a breather on this one.  We don't see why there's such a rush to judgement here.  ");
            strcat(story,"&r");
            break;
         case VIEW_CORPORATECULTURE:
            cityname(story);
            strcat(story," - Several major companies have announced ");
            strcat(story,"at a joint news conference here that they ");
            strcat(story,"will be expanding their work forces considerably ");
            strcat(story,"during the next quarter.  Over thirty thousand jobs ");
            strcat(story,"are expected in the first month, with ");
            strcat(story,"tech giant ");
            switch(LCSrandom(10))
            {
               case 0:strcat(story,"Ameri");break;
               case 1:strcat(story,"Gen");break;
               case 2:strcat(story,"Oro");break;
               case 3:strcat(story,"Amelia");break;
               case 4:strcat(story,"Vivo");break;
               case 5:strcat(story,"Benji");break;
               case 6:strcat(story,"Amal");break;
               case 7:strcat(story,"Ply");break;
               case 8:strcat(story,"Seli");break;
               case 9:strcat(story,"Rio");break;
            }
            switch(LCSrandom(10))
            {
               case 0:strcat(story,"tech");break;
               case 1:strcat(story,"com");break;
               case 2:strcat(story,"zap");break;
               case 3:strcat(story,"cor");break;
               case 4:strcat(story,"dyne");break;
               case 5:strcat(story,"bless");break;
               case 6:strcat(story,"chip");break;
               case 7:strcat(story,"co");break;
               case 8:strcat(story,"wire");break;
               case 9:strcat(story,"rex");break;
            }
            strcat(story," increasing its payrolls by over ten thousand workers alone.  ");
            strcat(story,"Given the state of the economy recently and in ");
            strcat(story,"light of the tendency ");
            strcat(story,"of large corporations to export jobs overseas these days, ");
            strcat(story,"this welcome news is bound to be a pleasant surprise to those in the unemployment lines.  ");
            strcat(story,"The markets reportedly responded to the announcement with mild interest, ");
            strcat(story,"although the dampened movement might be expected due to the uncertain ");
            strcat(story,"futures of some of the companies in the tech sector.  On the whole, however, ");
            strcat(story,"analysts suggest that not only does the expansion speak to the health ");
            strcat(story,"of the tech industry but is also indicative of a full economic recover.&r");
            break;
         case VIEW_AMRADIO:
         {
            //THIS ONE IS SHORTER BECAUSE OF DOUBLE HEADLINE
            cityname(story);
            strcat(story," - Infamous FM radio shock jock ");
            char dstr[200],dstr2[200];
            firstname(dstr);lastname(dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," has brought radio entertainment to a new low.  During yesterday's ");
            strcat(story,"broadcast of the program \"");
            strcat(story,dstr);
            strcat(story,"'s ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Morning");break;
               case 1:strcat(story,"Commuter");break;
               case 2:strcat(story,"Jam");break;
               case 3:strcat(story,"Talk");break;
               case 4:strcat(story,"Radio");break;
            }
            strcat(story," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"Swamp");break;
               case 1:strcat(story,"Jolt");break;
               case 2:strcat(story,"Club");break;
               case 3:strcat(story,"Show");break;
               case 4:strcat(story,"Fandango");break;
            }
            strcat(story,"\", ");
            strcat(story,dstr2);
            strcat(story," reportedly ");
            switch(LCSrandom(5))
            {
               case 0:strcat(story,"had intercourse");break;
               case 1:strcat(story,"encouraged listeners to call in and relieve themselves");break;
               case 2:strcat(story,"screamed \"f*ck the police those g*odd*mn m*th*f*ck*rs.  I got a f*cking ticket this morning and I'm f*cking p*ss*d as sh*t.\"");break;
               case 3:strcat(story,"breastfed from a lactating woman");break;
               case 4:strcat(story,"masturbated");break;
            }
            strcat(story," on the air.  Although ");
            strcat(story,dstr2);
            strcat(story," later apologized, ");
            strcat(story,"the FCC received thousands of complaints ");
            strcat(story,"from irate listeners.  A spokesperson for the FCC ");
            strcat(story,"stated that the incident is under investigation.");
            strcat(story,"&r");
         }
            break;
      }
   }
}



#define PICTURE_MUTANT_BEAST  0
#define PICTURE_CEO           1
#define PICTURE_BOOK          2
#define PICTURE_MELTDOWN      3
#define PICTURE_GENETICS      4
#define PICTURE_RIVERFIRE     5
#define PICTURE_DOLLARS       6
#define PICTURE_TINKYWINKY    7
#define PICTURE_OIL           8
#define PICTURE_TERRORISTS    9
#define PICTURE_KKK           10
#define PICTURE_TSHIRT        11

void displaymajoreventstory(newsstoryst& ns,char* story,short* storyx_s,short* storyx_e)
{
   if(ns.positive)
   {
      switch(ns.view)
      {
         case VIEW_WOMEN:
            displaycenterednewsfont("CLINIC MURDER",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_GAY:
            displaycenterednewsfont("CRIME OF HATE",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_DEATHPENALTY:
            displaycenterednewsfont("JUSTICE DEAD",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            /*
         case VIEW_MILITARY:
            displaycenterednewsfont("CASUALTIES MOUNT",5);
            displaycenteredsmallnews("Is the latest military invasion yet another quagmire?",12);
            strcpy(story,"");
            generatefiller(story,200);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            */
         case VIEW_POLITICALVIOLENCE:
            displaycenterednewsfont("NIGHTMARE",5);
            constructeventstory(story,ns.view,ns.positive);
            generatefiller(story,200);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            /*
         case VIEW_GUNS:
            displaycenterednewsfont("SCHOOL SHOOTING",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            */
         case VIEW_TAXES:
         {
            displaycenterednewsfont("REAGAN FLAWED",5);
            char str[80];
            strcpy(str,"");
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"Shadow");break;
               case 1:strcat(str,"Dark");break;
               case 2:strcat(str,"Abyssal");break;
               case 3:strcat(str,"Orwellian");break;
               case 4:strcat(str,"Craggy");break;
            }
            strcat(str," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"Actor");break;
               case 1:strcat(str,"Lord");break;
               case 2:strcat(str,"Emperor");break;
               case 3:strcat(str,"Puppet");break;
               case 4:strcat(str,"Dementia");break;
            }
            strcat(str,": A new book further documenting the other side of Reagan.");
            displaycenteredsmallnews(str,12);
            displaynewspicture(PICTURE_BOOK,13);
            break;
         }
         case VIEW_NUCLEARPOWER:
            displaycenterednewsfont("MELTDOWN",5);
            displaycenteredsmallnews("A nuclear power plant suffers a catastrophic meltdown.",12);
            displaynewspicture(PICTURE_MELTDOWN,13);
            break;
         case VIEW_ANIMALRESEARCH:
            displaycenterednewsfont("HELL ON EARTH",5);
            displaycenteredsmallnews("A mutant animal has escaped from a lab and killed thirty people.",12);
            displaynewspicture(PICTURE_MUTANT_BEAST,13);
            break;
         case VIEW_PRISONS:
            displaycenterednewsfont("ON THE INSIDE",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_INTELLIGENCE:
            displaycenterednewsfont("THE FBI FILES",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_FREESPEECH:
            displaycenterednewsfont("BOOK BANNED",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_GENETICS:
            displaycenterednewsfont("KILLER FOOD",5);
            displaycenteredsmallnews("Over a hundred people become sick from genetically modified food.",12);
            displaynewspicture(PICTURE_GENETICS,13);
            break;
         case VIEW_JUSTICES:
            displaycenterednewsfont("IN CONTEMPT",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_SWEATSHOPS:
            displaycenterednewsfont("CHILD'S PLEA",5);
            displaycenteredsmallnews("A T-shirt in a store is found scrawled with a message from a sweatshop worker.",12);
            displaynewspicture(PICTURE_TSHIRT,13);
            break;
         case VIEW_POLLUTION:
            displaycenterednewsfont("RING OF FIRE",5);
            displaycenteredsmallnews("The Ohio River caught on fire again.",12);
            displaynewspicture(PICTURE_RIVERFIRE,13);
            break;
         case VIEW_CORPORATECULTURE:
            displaycenterednewsfont("BELLY UP",5);
            displaycenteredsmallnews("An enormous company files for bankruptcy, shattering the previous record.",12);
            displaynewspicture(PICTURE_DOLLARS,13);
            break;
         case VIEW_CEOSALARY:
         {
            displaycenterednewsfont("AMERICAN CEO",5);
            char str[80];
            strcpy(str,"This major CEO ");
            switch(LCSrandom(10))
            {
               case 0:
                  strcat(str,"regularly visits prostitutes.");
                  break;
               case 1:
                  strcat(str,"seeks the aid of psychics.");
                  break;
               case 2:
                  strcat(str,"donated millions to the KKK.");
                  break;
               case 3:
                  strcat(str,"evaded paying taxes.");
                  break;
               case 4:
                  strcat(str,"took out a contract on his wife.");
                  break;
               case 5:
                  strcat(str,"doesn't know what his company does.");
                  break;
               case 6:
                  strcat(str,"has a zoo of imported exotic worms.");
                  break;
               case 7:
                  strcat(str,"paid millions for high-tech bondage gear.");
                  break;
               case 8:
                  strcat(str,"installed a camera in an office bathroom.");
                  break;
               case 9:
                  strcat(str,"owns slaves in another country.");
                  break;
            }
            displaycenteredsmallnews(str,12);
            displaynewspicture(PICTURE_CEO,13);
            break;
         }
         case VIEW_AMRADIO:
            displaycenterednewsfont("AM IMPLOSION",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
<<<<<<< .mine
            break;
      case VIEW_STALIN:
		   displaycenterednewsfont("STALIN'S CRIMES",5);
	       char str[80];
           strcpy(str,"");
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"Genocide in ");break;
               case 1:strcat(str,"Warfare in");break;
               case 2:strcat(str,"Famine in");break;
               case 3:strcat(str,"Death in");break;
               case 4:strcat(str,"Terror in");break;
            }
            strcat(str," ");
            switch(LCSrandom(6))
            {
               case 0:strcat(str,"Ukraine");break;
               case 1:strcat(str,"Sibera");break;
               case 2:strcat(str,"Poland");break;
               case 3:strcat(str,"Moscow");break;
               case 4:strcat(str,"Czechslovkia");break;
               case 5:strcat(str,"the Baltic States");break;
               case 6:strcat(str,"Eastern Europe");break;
               case 7:strcat(str,"Russia");break;
            }            
            strcat(str,": A new book proves once and for all that Stalin and the USSR has committed vast atrocities in the region.");
            displaycenteredsmallnews(str,12);
            displaynewspicture(PICTURE_BOOK,13);
            break;
		}
	}
=======
            break;
      }
   }
>>>>>>> .r282
   else
   {
      switch(ns.view)
      {
         case VIEW_GAY:
            displaycenterednewsfont("KINKY WINKY",5);
            displaycenteredsmallnews("Jerry Falwell explains the truth about Tinky Winky.  Again.",12);
            displaynewspicture(PICTURE_TINKYWINKY,13);
            break;
         case VIEW_DEATHPENALTY:
            displaycenterednewsfont("LET'S FRY 'EM",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            /*
         case VIEW_MILITARY:
            displaycenterednewsfont("BIG VICTORY",5);
            displaycenteredsmallnews("Our boys defend freedom once again, defeating an evil dictator.",13);
            strcpy(story,"");
            generatefiller(story,200);
            displaynewsstory(story,storyx_s,storyx_e,15);
            break;
            */
         case VIEW_POLITICALVIOLENCE:
            displaycenterednewsfont("END IN TEARS",5);
            constructeventstory(story,ns.view,ns.positive);
            generatefiller(story,200);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            /*
         case VIEW_GUNS:
            displaycenterednewsfont("KILLER STOPPED",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            */
         case VIEW_TAXES:
         {
            displaycenterednewsfont("REAGAN THE MAN",5);
            char str[80];
            strcpy(str,"");
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"Great");break;
               case 1:strcat(str,"Noble");break;
               case 2:strcat(str,"True");break;
               case 3:strcat(str,"Pure");break;
               case 4:strcat(str,"Golden");break;
            }
            strcat(str," ");
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"Leadership");break;
               case 1:strcat(str,"Courage");break;
               case 2:strcat(str,"Pioneer");break;
               case 3:strcat(str,"Communicator");break;
               case 4:strcat(str,"Faith");break;
            }
            strcat(str,": A new book lauding Reagan and the greatest generation.");
            displaycenteredsmallnews(str,12);
            displaynewspicture(PICTURE_BOOK,13);
            break;
<<<<<<< .mine
         }
            case VIEW_STALIN:
				{
			displaycenterednewsfont("STALIN FRAMED",5);
			char str[80];
            strcpy(str,"");
            switch(LCSrandom(5))
            {
               case 0:strcat(str,"Fall of the");break;
               case 1:strcat(str,"Legacy of the");break;
               case 2:strcat(str,"Sacrifices of The");break;
               case 3:strcat(str,"Lies Against The");break;
               case 4:strcat(str,"Truth Behind The");break;
            }
            strcat(str," ");
            switch(LCSrandom(6))
            {
               case 0:strcat(str,"Dear Leader");break;
               case 1:strcat(str,"Couragous Fighter");break;
               case 2:strcat(str,"Economic Miracle-Worker");break;
               case 3:strcat(str,"Great Communicator");break;
               case 4:strcat(str,"Socialist Democrat");break;
               case 5:strcat(str,"Revolutionary Secretary");break;
            }            
            strcat(str,": A new book details new evidence suggesting Stalin and the USSR was framed by the vast right-wing machine.");
            displaycenteredsmallnews(str,12);
            displaynewspicture(PICTURE_BOOK,13);
            break;
=======
>>>>>>> .r282
         }
         case VIEW_NUCLEARPOWER:
            displaycenterednewsfont("OIL CRUNCH",5);
            displaycenteredsmallnews("OPEC cuts oil production sharply in response to a US foreign policy decision.",12);
            displaynewspicture(PICTURE_OIL,13);
            break;
         case VIEW_ANIMALRESEARCH:
            displaycenterednewsfont("APE EXPLORERS",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_POLICEBEHAVIOR:
            displaycenterednewsfont("BASTARDS",5);
            displaynewspicture(PICTURE_TERRORISTS,13);
            break;
         case VIEW_PRISONS:
            displaycenterednewsfont("HOSTAGE SLAIN",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_INTELLIGENCE:
            displaycenterednewsfont("DODGED BULLET",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_FREESPEECH:
            displaycenterednewsfont("HATE RALLY",5);
            displaycenteredsmallnews("Free speech advocates fight hard to let a white supremacist rally take place.",12);
            displaynewspicture(PICTURE_KKK,13);
            break;
         case VIEW_GENETICS:
            displaycenterednewsfont("GM FOOD FAIRE",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_JUSTICES:
            displaycenterednewsfont("JUSTICE AMOK",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_SWEATSHOPS:
            displaycenterednewsfont("THEY ARE HERE",5);
            if(month>=8&&month<=11)displaycenteredsmallnews("Fall fashions hit the stores across the country.",12);
            else displaycenteredsmallnews("Fall fashions are previewed in stores across the country.",12);
            displaynewspicture(PICTURE_TSHIRT,13);
            break;
         case VIEW_POLLUTION:
            displaycenterednewsfont("LOOKING UP",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_CORPORATECULTURE:
            displaycenterednewsfont("NEW JOBS",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_AMRADIO:
            displaycenterednewsfont("THE DEATH",5);
            displaycenterednewsfont("OF CULTURE",13);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,21);
            break;
      }
   }
}
