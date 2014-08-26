// Note: this file is encoded in the PC-8 / Code Page 437 / OEM-US character set
// (The same character set used by Liberal Crime Squad when it is running)
// Certain special characters won't display correctly unless your text editor is
// set to use that character set, such as this e with an accent: 

// In Windows Notepad with the Terminal font, OEM/DOS encoding it should work fine.
// You can set this in Notepad by going to Format->Font and choosing the Terminal font,
// then choosing OEM/DOS in the Script dropdown box.

// In Notepad++ go to the Encoding menu, Character sets, Western European, OEM-US... easy!

// In Code::Blocks's editor go to Settings->Editor->the Other Settings tab and
// then pick WINDOWS-437 from the dropdown box and then choose the radio button
// to make this the default encoding and disable auto-detection of the encoding.
// Then close the file and reopen it (since Code::Blocks detects the encoding
// when it opens the file and it can't be changed after that; what we changed was
// how it detects encoding for files it opens in the future, not files already open).

// In Microsoft Visual C++, right-click the file in the Solution Explorer,
// select "Open With...", choose "C++ Source Code Editor (with encoding)",
// then choose "OEM United States - Codepage 437".

// In MS-DOS Editor (included with Windows as EDIT.COM in your system32 directory),
// the codepage will be correct already since it's running in a console window just
// like Liberal Crime Squad. Well OK, the encoding might be wrong, but then it's wrong
// in Liberal Crime Squad TOO, and to fix it, go to Control Panel, Regional and Language Settings,
// Advanced tab, and choose English (United States) from the dropdown box as the encoding
// for non-Unicode applications, then press OK.

// If you have a Linux or other UNIX-based system you are obviously smart enough
// to figure out for yourself how to open a file in OEM-US PC-8 codepage 437 in
// your favorite text editor. If you're on Mac OS X, well that's UNIX-based, figure
// it out for yourself.

#include <externs.h>

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
            char str[200],str2[200],gen[50];
            strcpy(story,cityname());
            strcat(story," - ");
            if(law[LAW_ABORTION]==-2) strcat(story,"A doctor that routinely performed illegal abortion-murders was ruthlessly ");
            else if(law[LAW_ABORTION]==-1) strcat(story,"A doctor that routinely performed illegal abortions was ruthlessly ");
            else if(law[LAW_ABORTION]==0) strcat(story,"A doctor that routinely performed semi-legal abortions was ruthlessly ");
            else strcat(story,"A doctor that routinely performed abortions was ruthlessly ");
            strcat(story,"gunned down outside of the ");
            lastname(str,true); strcat(story,str);
            strcat(story," Clinic yesterday.  ");
            strcat(story,"Dr. ");
            char dstr[200],dstr2[200];
            char gn=(LCSrandom(2)?GENDER_MALE:GENDER_FEMALE);
            generate_name(dstr,dstr2,gn);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcpy(gen,(gn==GENDER_FEMALE?"her":"his"));
            strcat(story," was walking to "); strcat(story,gen);//TODO: Add more variety, not just in the parking lot.
            strcat(story," car when, according to police reports, ");
            strcat(story,"shots were fired from a nearby vehicle.  ");
            strcat(story,dstr2);
            strcat(story," was hit ");
            strcat(story,LCSrandom(15)+3);
            strcat(story," times and died immediately in the parking lot.  ");
            strcat(story,"The suspected shooter, ");
            generate_name(str,str2);
            strcat(story,str);
            strcat(story," ");
            strcat(story,str2);
            strcat(story,", is in custody.&r");
            strcat(story,"  Witnesses report that ");
            strcat(story,str2);
            strcat(story," remained at the scene after the shooting, screaming ");
            strcat(story,"verses of the Bible at the stunned onlookers.  Someone ");
            strcat(story,"called the police on a cellphone and they arrived shortly thereafter.  ");
            strcat(story,str2);
            if(law[LAW_WOMEN]==-2)
            {
               strcat(story," later admitted to being a rogue FBI vigilante, hunting down ");
               strcat(story," abortion doctors as opposed to arresting them.&r");
            }
            else
            {
               strcat(story," surrendered without a struggle, reportedly saying that God's work ");
               strcat(story,"had been completed.&r");
            }
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," is survived by ");
            strcat(story,gen);
            strcat(story," ");
            char spouse=(LCSrandom(2)?GENDER_MALE:GENDER_FEMALE);
            if(law[LAW_GAY]<=1)
               spouse=(gn==GENDER_FEMALE?GENDER_MALE:GENDER_FEMALE);
            strcat(story,(spouse==GENDER_FEMALE?"wife":"husband"));
            strcat(story," and ");
            switch(LCSrandom(4))
            {
            case 0: strcat(story,"two");break;
            case 1: strcat(story,"three");break;
            case 2: strcat(story,"four");break;
            case 3: strcat(story,"five");break;
            }
            strcat(story," children.&r");
            break;
         }
         case VIEW_GAY:
         {
            strcpy(story,cityname());
            strcat(story," - ");
            char dstr[200],dstr2[200];
            generate_name(dstr,dstr2);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            if(law[LAW_GAY]==-2) strcat(story,", a known sexual deviant, was ");
            else if(law[LAW_GAY]==-1) strcat(story,", a known homosexual, was ");
            else strcat(story,", a homosexual, was ");
            switch(LCSrandom(3))
            {
            case 0: strcat(story,"dragged to death behind a pickup truck");break;
            case 1: strcat(story,"burned alive");break;
            case 2: strcat(story,"beaten to death");break;
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
            case 0:
               if(law[LAW_FREESPEECH]==-2) strcat(story,"throwing [juice boxes]");
               else strcat(story,"throwing beer bottles");
               break;
            case 1:
               if(law[LAW_FREESPEECH]==-2) strcat(story,"[relieving themselves] out the window");
               else if(law[LAW_FREESPEECH]==2) strcat(story,"pissing out the window");
               else strcat(story,"urinating out the window");
               break;
            case 2: strcat(story,"taking swipes"); break;
            }
            strcat(story," at the pursuing police cruisers.  ");
            strcat(story,"The chase ended when ");
            switch(LCSrandom(5))
            {
            case 0: strcat(story,"the suspects ran out of gas, "); break;
            case 1: strcat(story,"the suspects collided with a manure truck, "); break;
            case 2: strcat(story,"the suspects veered into a ditch, "); break;
            case 3: strcat(story,"the suspects were surrounded by alert citizens, "); break;
            case 4: strcat(story,"the suspects were caught in traffic, "); break;
            }
            strcat(story,"at which point they were taken into custody.  Nobody was seriously injured during the incident.");
            strcat(story,"&r");
            strcat(story,"  Authorities have stated that they will vigorously ");
            strcat(story,"prosecute this case as a hate crime, due to the ");
            strcat(story,"aggravated nature of the offense");
            if(law[LAW_GAY]==-2&&law[LAW_FREESPEECH]!=-2)
            {
               strcat(story,", despite the fact that ");
               strcat(story,dstr);
               strcat(story," ");
               strcat(story,dstr2);
               strcat(story," is a known faggot");
            }
            else if(law[LAW_GAY]==-2)strcat(story,", even though being gay is deviant, as we all know.");
            else strcat(story,".");
            strcat(story,"&r");
            break;
         }
         case VIEW_DEATHPENALTY:
         {
            strcat(story,statename());
            strcat(story," - An innocent citizen has been put to death in the electric chair.  ");
            char dstr[200],dstr2[200],dstr3[200];
            generate_long_name(dstr,dstr2,dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was pronounced dead at ");
            strcat(story,LCSrandom(12)+1);
            strcat(story,":");
            strcat(story,LCSrandom(60));
            switch(LCSrandom(2))
            {
            case 0: strcat(story,"AM"); break;
            case 1: strcat(story,"PM"); break;
            }
            strcat(story," yesterday at the ");
            char jstr[200];
            lastname(jstr,true);
            strcat(story,jstr);
            strcat(story," Correctional Facility.&r");

            strcat(story,"  ");
            strcat(story,dstr3);
            strcat(story," was convicted in ");
            strcat(story,year-LCSrandom(11)-10);
            strcat(story," of 13 serial murders.  ");
            strcat(story,"Since then, numerous pieces of exculpatory evidence ");
            strcat(story,"have been produced, including ");
            switch(LCSrandom(3))
            {
            case 0: strcat(story,"a confession from another convict.  "); break;
            case 1: strcat(story,"a battery of negative DNA tests.  "); break;
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
            case 0: strcat(story,"Let's not forget the convict is colored.  You know how their kind are"); break;
            case 1:
               strcat(story,"The convict is always referred to by three names.  ");
               strcat(story,"Assassin, serial killer, either way ฤฤ guilty.  ");
               strcat(story,"End of story");
               break;
            case 2: strcat(story,"The family wants closure.  We don't have time for another trial"); break;
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
         case VIEW_INTELLIGENCE:
         {
            strcat(story,"Washington, DC - The FBI might be keeping tabs on you.  ");
            strcat(story,"This newspaper yesterday received a collection of files from a source in the Federal Bureau of Investigations.  ");
            strcat(story,"The files contain information on which people have been attending demonstrations, organizing ");
            strcat(story,"unions, working for liberal organizations ฤฤ even ");
            switch(LCSrandom(2))
            {
            case 0:strcat(story,"buying music with 'Explicit Lyrics' labels."); break;
            case 1:strcat(story,"helping homeless people"); break;
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
         case VIEW_FREESPEECH:
         {
            strcpy(story,cityname());
            strcat(story," - A children's story has been removed from libraries here after ");
            strcat(story,"the city bowed to pressure from religious groups.");
            strcat(story,"&r");
            strcat(story,"   The book, ");
            strcat(story,"_");
            char dstr[200],nstr[200];
            generate_name(nstr,dstr);
            strcat(story,nstr);
            strcat(story,"_");
            strcat(story,dstr);
            strcat(story,"_and_the_");
            switch(LCSrandom(7))
            {
            case 0: strcat(story,"Mysterious"); break;
            case 1: strcat(story,"Magical"); break;
            case 2: strcat(story,"Golden"); break;
            case 3: strcat(story,"Invisible"); break;
            case 4: strcat(story,"Wondrous"); break;
            case 5: strcat(story,"Amazing"); break;
            case 6: strcat(story,"Secret"); break;
            }
            strcat(story,"_");
            switch(LCSrandom(5))
            {
            case 0: strcat(story,"Thing"); break;
            case 1: strcat(story,"Stuff"); break;
            case 2: strcat(story,"Object"); break;
            case 3: strcat(story,"Whatever"); break;
            case 4: strcat(story,"Something"); break;
            }
            strcat(story,"_, is the third in an immensely popular series by ");
            switch(LCSrandom(11))
            {
            case 0: strcat(story,"British"); break;
            case 1: strcat(story,"Indian"); break;
            case 2: strcat(story,"Chinese"); break;
            case 3: strcat(story,"Rwandan"); break;
            case 4: strcat(story,"Palestinian"); break;
            case 5: strcat(story,"Egyptian"); break;
            case 6: strcat(story,"French"); break;
            case 7: strcat(story,"German"); break;
            case 8: strcat(story,"Iraqi"); break;
            case 9: strcat(story,"Bolivian"); break;
            case 10: strcat(story,"Columbian"); break;
            default: strcat(story,"Elbonian"); break; // Dilbert
            }
            strcat(story," author ");
            char c[2]={0,0};
            c[0]='A'+LCSrandom(26);
            strcat(story,c);
            strcat(story,".");
            c[0]='A'+LCSrandom(26);
            strcat(story,c);
            strcat(story,". ");
            lastname(dstr);
            strcat(story,dstr);
            strcat(story,".  ");
            strcat(story,"Although the series is adored by children worldwide, ");
            strcat(story,"some conservatives feel that the books ");
            switch(LCSrandom(5))
            {
            case 0: strcat(story,"glorify Satan worship and are spawned by demons from the pit.  "); break;
            case 1: strcat(story,"teach children to kill their parents and hate life.  "); break;
            case 2: strcat(story,"cause violence in schools and are a gateway to cocaine use.  "); break;
            case 3: strcat(story,"breed demonic thoughts that manifest themselves as dreams of murder.  "); break;
            case 4: strcat(story,"contain step-by-step instructions to summon the Prince of Darkness.  "); break;
            }
            strcat(story,"In their complaint, the groups cited an incident involving ");
            switch(LCSrandom(3))
            {
            case 0: strcat(story,"a child that swore in class"); break;
            case 1: strcat(story,"a child that said a magic spell at her parents"); break;
            case 2: strcat(story,"a child that ");
               switch(LCSrandom(5))
               {
               case 0: strcat(story,"pushed "); break;
               case 1: strcat(story,"hit "); break;
               case 2: strcat(story,"slapped "); break;
               case 3: strcat(story,"insulted "); break;
               case 4: strcat(story,"tripped "); break;
               }
               switch(LCSrandom(2))
               {
               case 0: strcat(story,"his "); break;
               case 1: strcat(story,"her "); break;
               }
               switch(LCSrandom(3))
               {
               case 0: strcat(story,"older "); break;
               case 1: strcat(story,"younger "); break;
               case 2: strcat(story,"twin "); break;
               }
               switch(LCSrandom(2))
               {
               case 0: strcat(story,"brother"); break;
               case 1: strcat(story,"sister"); break;
               }
               break;
            }
            strcat(story," as key evidence of the dark nature of the book.");
            strcat(story,"&r");
            strcat(story,"   When the decision to ban the book was announced yesterday, ");
            strcat(story,"many area children spontaneously broke into tears.  One child was ");
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
            strcpy(story,cityname());
            strcat(story," - Conservative federal judge ");
            char dstr[200],dstr2[200];
            generate_name(dstr,dstr2,GENDER_WHITEMALEPATRIARCH);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            if(law[LAW_FREESPEECH]==-2) strcat(story," has resigned in disgrace after being caught with a [civil servant].");
            else strcat(story," has resigned in disgrace after being caught with a prostitute.");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story,", who once ");
            switch(LCSrandom(2))
            {
            case 0: strcat(story,"defied the federal government by putting a Ten Commandments monument in the local federal building"); break;
            case 1: strcat(story,"stated that, \"Segregation wasn't the bad idea everybody makes it out to be these days\""); break;
            }
            strcat(story,", was found with ");
            char pstr[200],pstr2[200];
            generate_name(pstr,pstr2);
            strcat(story,pstr);
            strcat(story," ");
            strcat(story,pstr2);
            strcat(story," last week in a hotel during a police sting operation.  ");
            strcat(story,"According to sources familiar with the particulars, ");
            strcat(story,"when police broke into the hotel room they saw ");
            switch(LCSrandom(3))
            {
            case 0:strcat(story,"\"the most perverse and spine-tingling debauchery imaginable, at least with only two people.\""); break;
            case 1:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"the judge [going to the bathroom in the vicinity of] the [civil servant].");
               else if(law[LAW_FREESPEECH]==2)strcat(story,"the judge pissing on the prostitute.");
               else strcat(story,"the judge relieving himself on the prostitute."); break; // Himself? Maybe themselves would work better? -- LK
            case 2:                                                                        // No, the judge is a white male Arch-Conservative patriarch. -- yetisyny
               if(law[LAW_FREESPEECH]==-2)strcat(story,"the [civil servant] hollering like a cowboy [at a respectable distance from] the judge.");
               else strcat(story,"the prostitute hollering like a cowboy astride the judge."); break;
            }
            strcat(story,"  ");
            strcat(story,pstr2);
            strcat(story," reportedly offered ");
            switch(LCSrandom(3))
            {
            case 0: strcat(story,"the arresting officers money"); break;
            case 1: strcat(story,"to let the officers join in"); break;
            case 2: strcat(story,"the arresting officers \"favors\""); break;
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
            strcpy(story,cityname());
            strcat(story," - Well-known AM radio personality ");
            char dstr[200],dstr2[200];
            generate_name(dstr,dstr2,GENDER_WHITEMALEPATRIARCH);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," went off for fifteen minutes in an inexplicable rant ");
            strcat(story,"two nights ago during the syndicated radio program \"");
            switch(LCSrandom(3))
            {
            case 0: strcat(story,"Straight"); break;
            case 1: strcat(story,"Real"); break;
            case 2: strcat(story,"True"); break;
            }
            strcat(story," ");
            switch(LCSrandom(3))
            {
            case 0: strcat(story,"Talk"); break;
            case 1: strcat(story,"Chat"); break;
            case 2: strcat(story,"Discussion"); break;
            }
            strcat(story,"\".");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story,"'s monologue for the evening began the way that fans ");
            strcat(story,"had come to expect, with attacks on the \"liberal media establishment\" and ");
            strcat(story,"the \"elite liberal agenda\".  But when the radio icon said, \"");
            switch(LCSrandom(4))
            {
            case 0: strcat(story,"and the Grays are going to take over the planet in the End Times"); break;
            case 1: strcat(story,"a liberal chupacabra will suck the blood from us like a goat, a goat!, a goat!"); break;
            case 2: strcat(story,"I feel translucent rods passing through my body...  it's like making love to the future"); break;
            case 3:
               strcat(story,"and the greatest living example of a reverse racist is the ");
               if(presparty!=CONSERVATIVE_PARTY) strcat(story,"current president!"); // Limbaugh
               else strcat(story,"liberal media establishment!");
               break;
            }
            strcat(story,"\", a former fan of the show, ");
            char nstr[200],nstr2[200];
            generate_name(nstr,nstr2);
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
               strcat(story,"his"); // the AM personality's a white male patriarch

               if(law[LAW_FREESPEECH]==2)strcat(story," goddamn mind");
               else if(law[LAW_FREESPEECH]==-2)strcat(story," [gosh darn] mind");
               else strcat(story," g*dd*mn mind");
               break;
            case 1: strcat(story,"maybe gone a little off the deep end"); break;
            case 2: strcat(story,"probably been listening to Art Bell in the next studio a little too long"); break;
            }
            strcat(story,".  After that, it just got worse and worse.\"");
            strcat(story,"&r");
            strcat(story,"  ");
            strcat(story,dstr2);
            strcat(story," issued an apology later in the program, but ");
            strcat(story,"the damage might already be done.  ");
            strcat(story,"According to a poll completed yesterday, ");
            strcat(story,"fully half of the host's most loyal supporters ");                    // XXX How many of them switch should
            strcat(story,"have decided to leave the program for saner ");                        //     depend on [LAW_FREESPEECH]
            strcat(story,"pastures.  Of these, many said that they would be switching over ");
            strcat(story,"to the FM band.");
            strcat(story,"&r");
            break;
         }
         case VIEW_GUNCONTROL:
         {
            int schtype = LCSrandom(4);
            strcpy(story,cityname());
            strcat(story," - ");
            strcat(story,"A student has gone on a ");
            if(law[LAW_FREESPEECH]==-2) strcat(story,"[hurting spree]");
            else strcat(story,"shooting rampage");
            strcat(story, " at a local ");
            switch(schtype)
            {
            case 0: strcat(story,"elementary school"); break;
            case 1: strcat(story,"middle school"); break;
            case 2: strcat(story,"high school"); break;
            case 3: strcat(story,"university"); break;
            }
            strcat(story,".  ");
            char dstr[200],dstr2[200],dg;
            dg = (LCSrandom(2)==1?GENDER_MALE:GENDER_FEMALE);
            generate_name(dstr,dstr2,dg);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,", ");
            //6->11,10->15,14->19,18->23
            strcat(story,6+(schtype*4)+LCSrandom(6)); //generate an age that would roughly correspond to the schtype
            strcat(story, ", used a variety of guns to ");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"[scare]");
            else strcat(story,"mow down");
            strcat(story," more than a dozen classmates and two teachers at ");
            char jstr[200];
            lastname(jstr,true);
            strcat(story,jstr);
            switch(schtype)
            {
            case 0: strcat(story," Elementary School"); break;
            case 1: strcat(story," Middle School"); break;
            case 2: strcat(story," High School"); break;
            case 3: strcat(story," University"); break;
            }
            strcat(story, ".  ");
            strcat(story,dstr2);
            strcat(story, " entered the ");
            if(schtype!=3) strcat(story,"school ");
            else strcat(story,"university ");
            strcat(story, " while classes were in session, then systematically started breaking into ");
            strcat(story, "classrooms, ");
            if(law[LAW_FREESPEECH]==-2) strcat(story,"[scaring]");
            else strcat(story,"spraying bullets at");
            strcat(story," students and teachers inside.  ");
            strcat(story,"When other students tried to wrestle the weapons away from ");
            strcat(story,(dg==GENDER_FEMALE?"her":"him"));
            strcat(story,", they were ");
            if(law[LAW_FREESPEECH]==-2) strcat(story,"[unfortunately harmed]");
            else strcat(story,"shot");
            strcat(story," as well.&r");
            strcat(story, "  When the police arrived, the student had already ");
            if(law[LAW_FREESPEECH]==-2) strcat(story, "[hurt some people].  ");
            else
            {
               strcat(story, "killed ");
               strcat(story, 2+LCSrandom(30));
               strcat(story, " and wounded dozens more.  ");
            }
            strcat(story,dstr);
            if(law[LAW_FREESPEECH]==-2)strcat(story, " [feel deeply asleep]");
            else strcat(story, " committed suicide");
            strcat(story," shortly afterwards.&r");
            strcat(story,"  Investigators are currently searching the student's belongings, and initial ");
            strcat(story,"reports indicate that the student kept a journal that showed ");
            strcat(story,(dg==GENDER_FEMALE?"she":"he"));
            strcat(story," was disturbingly obsessed with guns and death.&r");
            break;
         }
         case VIEW_PRISONS:
         {
            strcpy(story,cityname());
            strcat(story," - A former prisoner has written a book describing in horrifying ");
            strcat(story,"detail what goes on behind bars.  ");
            strcat(story,"Although popular culture has used, or perhaps overused, the ");
            strcat(story,"prison theme lately in its offerings for mass consumption, rarely ");
            strcat(story,"have these works been as poignant as ");
            char dstr[200],dstr2[200];
            generate_name(dstr,dstr2); // allow either gender (look up "Orange is the New Black" online to see why)
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,"'s new tour-de-force, _");
            switch(LCSrandom(6))
            {
            case 0: strcat(story,"Nightmare"); break;
            case 1: strcat(story,"Primal"); break;
            case 2: strcat(story,"Animal"); break;
            case 3: strcat(story,"American"); break;
            case 4: strcat(story,"Solitary"); break;
            case 5: strcat(story,"Painful"); break;//Painful Soap, anyone?
            }
            strcat(story,"_");
            switch(LCSrandom(8))
            {
            case 0: strcat(story,"Packer"); break;
            case 1: strcat(story,"Soap"); break;//Nightmare Soap, anyone?
            case 2: strcat(story,"Punk"); break;
            case 3: strcat(story,"Kid"); break;
            case 4: strcat(story,"Cell"); break;
            case 5: strcat(story,"Shank"); break;
            case 6: strcat(story,"Lockdown"); break;
            case 7:
               if(law[LAW_FREESPEECH]==-2) strcat(story,"[Bum]lord");
               else strcat(story,"Buttlord"); break;
            case 8:strcat(story,"Shower"); break;
            }
            strcat(story,"_.&r");
            strcat(story,"   Take this excerpt, \"");//TODO: Add more excerpts, more variety.
            strcat(story,"The steel bars grated forward in their rails, ");
            strcat(story,"coming to a halt with a deafening clang that said it all ฤฤ ");
            strcat(story,"I was trapped with them now.  There were three, looking me over ");
            strcat(story,"with dark glares of bare lust, as football players might stare at a stupefied, drunken, helpless teenager.  ");
            strcat(story,"My shank's under the mattress.  Better to be brave and fight or chicken out and let them take it?  ");
            strcat(story,"Maybe lose an eye the one way, maybe catch ");
            if(law[LAW_GAY]==-2)strcat(story,"GRIDS");// Gay Related Immunodeficiency Syndrome, an obsoleted/politically incorrect name for "AIDS".
            else strcat(story,"AIDS");
            strcat(story," the other.  A ");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"[difficult]");
            else strcat(story,"helluva");
            strcat(story," choice, and I would only have a few seconds before they made it for me");
            strcat(story,".\"");
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
            strcpy(story,cityname());
            strcat(story," - Perhaps parents can rest easier tonight.  ");
            strcat(story,"The authorities have apprehended their primary suspect in the ");
            strcat(story,"string of brutal child killings that has kept everyone in the area on edge, ");
            strcat(story,"according to a spokesperson for the police department here.  ");
            char dstr[200],dstr2[200],dstr3[200];
            generate_long_name(dstr,dstr2,dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was detained yesterday afternoon, reportedly in possession of ");
            switch(LCSrandom(5))
            {
            case 0: strcat(story,"pieces of another victim"); break;
            case 1: strcat(story,"bloody toys"); break;
            case 2: strcat(story,"a child's clothing stained with DNA evidence"); break;
            case 3: strcat(story,"seven junior high school yearbooks"); break;
            case 4: strcat(story,"two small backpacks"); break;
            }
            strcat(story,".  Over twenty children in the past two years have gone missing, ");
            strcat(story,"only to turn up later");
            if(law[LAW_FREESPEECH]==-2)
               strcat(story," [in a better place]");
            else
            {
               strcat(story," dead and ");
               switch(LCSrandom(5))
               {
               case 0: strcat(story,"carved with satanic symbols"); break;
               case 1: strcat(story,"sexually mutilated"); break;
               case 2: strcat(story,"missing all of their teeth"); break;
               case 3: strcat(story,"missing all of their fingers"); break;
               case 4: strcat(story,"without eyes"); break;
               }
            }
            strcat(story,".  Sources say that the police got a break in the case when ");
            switch(LCSrandom(5))
            {
            case 0: strcat(story,"a victim called 911 just prior to being slain while still on the phone"); break;
            case 1: strcat(story,"the suspect allegedly carved an address into one of the bodies"); break;
            case 2: strcat(story,"an eye witness allegedly spotted the suspect luring a victim into a car"); break;
            case 3: strcat(story,"a blood trail was found on a road that led them to the suspect's car trunk"); break;
            case 4: strcat(story,"they found a victim in a ditch, still clinging to life"); break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   The district attorney's office has already repeatedly said it will be ");
            strcat(story,"seeking ");
            if(law[LAW_DEATHPENALTY]==2)
               strcat(story,"life imprisonment in this case.");
            else
               strcat(story,"the death penalty in this case.");
            strcat(story,"&r");
            break;
         }
         case VIEW_ANIMALRESEARCH:
         {
            strcpy(story,cityname());
            strcat(story," - Researchers ");
            if(law[LAW_ANIMALRESEARCH]==2)
            {
               strcat(story,"from ");
               switch(LCSrandom(5))
               {
               case 0:strcat(story,"Russia");break;
               case 1:strcat(story,"North Korea");break;
               case 2:strcat(story,"Cuba");break;
               case 3:strcat(story,"Iran");break;
               case 4:strcat(story,"China");break;
               }
               strcat(story," report that they have discovered an amazing new wonder drug. ");
            }
            else
               strcat(story,"here report that they have discovered an amazing new wonder drug.  ");
            strcat(story,"Called ");
            switch(LCSrandom(5))
            {
            case 0:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"Bum-Bum");
               else strcat(story,"Anal");break;
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
            case 1:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[helps chimpanzees reproduce]");
               else strcat(story,"corrects erectile dysfunction in chimpanzees");break;
            case 2:strcat(story,"allows chimpanzees to move blocks with their minds");break;
            case 3:strcat(story,"allows chimpanzees to fly short distances");break;
            case 4:strcat(story,"increases the attention span of young chimpanzees");break;
            }
            strcat(story,".  ");
            strcat(story,"&r");
            strcat(story,"   Along with bonobos, chimpanzees are our closest cousins");
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
         case VIEW_INTELLIGENCE:
         {
            strcat(story,"Washington, DC - The CIA announced yesterday that it has averted a terror attack that ");
            strcat(story,"would have occurred on American soil.");
            strcat(story,"&r");
            strcat(story,"   According to a spokesperson for the agency, ");
            switch(LCSrandom(3))
            {
            case 0:strcat(story,"white supremacists");break;
            case 1:strcat(story,"Islamic fundamentalists");break;
            case 2:strcat(story,"outcast goths from a suburban high school");break;
            }
            strcat(story," planned to ");
            switch(LCSrandom(9))
            {
            case 0:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[land] planes [on apartment buildings]");
               else strcat(story,"fly planes into skyscrapers");break;
            case 1:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[put] fertilizer [on plants] at a federal building");
               else strcat(story,"detonate a fertilizer bomb at a federal building");break;
            case 2:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[show up uninvited on] a warship");
               else strcat(story,"ram a motorboat loaded with explosives into a warship");break;
            case 3:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[give children owies and boo-boos]");
               else strcat(story,"detonate explosives on a school bus");break;
            case 4:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[cause a traffic jam on] a major bridge");
               else strcat(story,"blow out a section of a major bridge");break;
            case 5:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[take] the president [on vacation]");
               else strcat(story,"kidnap the president");break;
            case 6:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[hurt] the president");
               else strcat(story,"assassinate the president");break;
            case 7:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[vandalize] the Capitol Building");
               else strcat(story,"destroy the Capitol Building");break;
            case 8:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"detonate [fireworks] in New York");
               else strcat(story,"detonate a nuclear bomb in New York");break;
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
            strcpy(story,cityname());
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
            strcat(story,"\", during an afternoon PowerPoint presentation.  ");
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
            case 2:strcat(story,"guy speaking in tongues and worshiping Satan");break;
            case 3:strcat(story,"gal having a ruptured intestine");break;
            }
            strcat(story," is just a load of ");
            if(law[LAW_FREESPEECH]==-2) switch(LCSrandom(5))
            {
            case 0:strcat(story,"hooey");break;
            case 1:strcat(story,"poppycock");break;
            case 2:strcat(story,"horse radish");break;
            case 3:strcat(story,"skunk weed");break;
            case 4:strcat(story,"garbage");break;
            }
            else switch(LCSrandom(3))
            {
            case 0:strcat(story,"horseshit");break; // McCain
            case 1:strcat(story,"bullshit");break;
            case 2:strcat(story,"shit");break;
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
            strcpy(story,cityname());
            strcat(story," - The conviction of confessed serial killer ");
            char dstr[200],dstr2[200],dstr3[200];
            generate_long_name(dstr,dstr2,dstr3);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story," ");
            strcat(story,dstr3);
            strcat(story," was overturned by a federal judge yesterday.  ");
            strcat(story,"Justice ");
            char jstr[200],jstr2[200];
            char gn=(LCSrandom(2)==1?GENDER_MALE:GENDER_FEMALE);
            generate_name(jstr,jstr2,gn);
            strcat(story,jstr);
            strcat(story," ");
            strcat(story,jstr2);
            strcat(story," of the notoriously liberal circuit of appeals here ");
            strcat(story,"made the decision based on ");
            char gen[20];
            strcpy(gen,(gn==GENDER_FEMALE?"her":"his"));
            switch(LCSrandom(7))
            {
            case 0:strcat(story,"ten-year-old eyewitness testimony");break;
            case 1:strcat(story,gen);strcat(story," general feeling about police corruption");break;
            case 2:strcat(story,gen);strcat(story," belief that the crimes were a vast right-wing conspiracy");break; // Clinton
            case 3:
               strcat(story,gen);
               strcat(story," belief that ");
               strcat(story,dstr3);
               strcat(story," deserved another chance");
               break;
            case 4:
               strcat(story,gen);
               strcat(story," personal philosophy of liberty");break;
            case 5:
               strcat(story,gen);
               strcat(story," close personal friendship with the ");
               strcat(story,dstr3);      // I know Charles Manson.
               strcat(story," family");  // Charles Manson was a friend of mine.
               break;                    // And you, sir, are no Charles Manson!
            case 6:strcat(story,gen);strcat(story," consultations with a Magic 8-Ball");break;
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
            strcpy(story,cityname());
            strcat(story," - Pollution might not be so bad after all.  The ");
            switch(LCSrandom(6))
            {
            case 0:strcat(story,"American");break;
            case 1:strcat(story,"United");break;
            case 2:strcat(story,"Patriot");break;
            case 3:strcat(story,"Family");break;
            case 4:strcat(story,"Children's");break;
            case 5:strcat(story,"National");break;
            }
            strcat(story," ");
            switch(LCSrandom(6))
            {
            case 0:strcat(story,"Heritage");break;
            case 1:strcat(story,"Enterprise");break;
            case 2:strcat(story,"Freedom");break;
            case 3:strcat(story,"Liberty");break;
            case 4:strcat(story,"Charity");break;
            case 5:strcat(story,"Equality");break;
            }
            strcat(story," ");
            switch(LCSrandom(6))
            {
            case 0:strcat(story,"Partnership");break;
            case 1:strcat(story,"Institute");break;
            case 2:strcat(story,"Consortium");break;
            case 3:strcat(story,"Forum");break;
            case 4:strcat(story,"Center");break;
            case 5:strcat(story,"Association");break;
            }
            strcat(story," recently released a wide-ranging report detailing recent trends ");
            strcat(story,"and the latest science on the issue.  ");
            strcat(story,"Among the most startling of the think tank's findings is that ");
            switch(LCSrandom(6))
            {
            case 0:strcat(story,"a modest intake of radioactive waste");break;
            case 1:strcat(story,"a healthy dose of radiation");break;
            case 2:strcat(story,"a bath in raw sewage");break;
            case 3:strcat(story,"watching animals die in oil slicks");break;
            case 4:strcat(story,"inhaling carbon monoxide");break;
            case 5:strcat(story,"drinking a cup of fracking fluid a day");break;
            }
            strcat(story," might actually ");
            switch(LCSrandom(6))
            {
            case 0:strcat(story,"purify the soul");break;
            case 1:strcat(story,"increase test scores");break;
            case 2:strcat(story,"increase a child's attention span");break;
            case 3:strcat(story,"make children behave better");break;
            case 4:strcat(story,"make shy children fit in");break;
            case 5:strcat(story,"cure everything from abdominal ailments to zygomycosis");break;
            }
            strcat(story,".");
            strcat(story,"&r");
            strcat(story,"   When questioned about the science behind these results, ");
            strcat(story,"a spokesperson stated that, \"");
            switch(LCSrandom(4))
            {
            case 0:strcat(story,"Research is complicated, and there are always two ways to think about things");break;
            case 1:strcat(story,"The jury is still out on pollution.  You really have to keep an open mind");break;
            case 2:strcat(story,"They've got their scientists, and we have ours.  The issue of pollution is wide open as it stands today");break;
            case 3:strcat(story,"I just tried it myself and I feel like a million bucks!  *Coughs up blood*  I'm OK, that's just ketchup");break;
            }
            strcat(story,".  You have to realize that ");
            switch(LCSrandom(4))
            {
            case 0:strcat(story,"the elitist liberal media often distorts");break;
            case 1:strcat(story,"the vast left-wing education machine often distorts");break;
            case 2:strcat(story,"the fruits, nuts, and flakes of the environmentalist left often distort");break;
            case 3:strcat(story,"leftists suffering from the mental disorder chemophobia often distort");break;
            }
            strcat(story," these issues to their own advantage.  ");
            strcat(story,"All we've done is introduced a little clarity into the ongoing debate.  ");
            strcat(story,"Why is there contention on the pollution question?  It's because ");
            strcat(story,"there's work left to be done.  We should study much more ");
            strcat(story,"before we urge any action.  Society really just ");
            strcat(story,"needs to take a breather on this one.  We don't see why there's such a rush to judgment here.  ");
            strcat(story,"&r");
            break;
         case VIEW_CORPORATECULTURE:
            strcpy(story,cityname());
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
         {  //THIS ONE IS SHORTER BECAUSE OF DOUBLE HEADLINE
            strcpy(story,cityname());
            strcat(story," - Infamous FM radio shock jock ");
            char dstr[200],dstr2[200];
            generate_name(dstr,dstr2,GENDER_WHITEMALEPATRIARCH);
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
            case 0:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[had consensual intercourse in the missionary position]");
               else if(law[LAW_FREESPEECH]==2)strcat(story,"fucked");
               else strcat(story,"had intercourse");break;
            case 1:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"encouraged listeners to call in and [urinate]");
               else if(law[LAW_FREESPEECH]==2)strcat(story,"encouraged listeners to call in and take a piss");
               else strcat(story,"encouraged listeners to call in and relieve themselves");break;
            case 2:
               if(law[LAW_FREESPEECH]==2)strcat(story,"screamed \"fuck the police those goddamn motherfuckers.  I got a fucking ticket this morning and I'm fucking pissed as shit.\"");
               else if(law[LAW_FREESPEECH]==-2)strcat(story,"screamed \"[darn] the police those [big dumb jerks]. I got a [stupid] ticket this morning and I'm [so angry].\"");
               else strcat(story,"screamed \"f*ck the police those g*dd*mn m*th*f*ck*rs.  I got a f*cking ticket this morning and I'm f*cking p*ss*d as sh*t.\"");break;
            case 3:
               if(law[LAW_FREESPEECH]==-2 && law[LAW_WOMEN]==-2)strcat(story,"[fed] from [an indecent] woman");
               else if(law[LAW_FREESPEECH]!=-2 && law[LAW_WOMEN]==-2)strcat(story,"breastfed from an exposed woman");
               else if(law[LAW_FREESPEECH]==-2 && law[LAW_WOMEN]!=-2)strcat(story,"[fed] from a [woman]");
               else strcat(story,"breastfed from a lactating woman");break;
            case 4:
               if(law[LAW_FREESPEECH]==-2)strcat(story,"[had fun]");
               else strcat(story,"masturbated");break;
            }
            strcat(story," on the air.  Although ");
            strcat(story,dstr2);
            strcat(story," later apologized, ");
            strcat(story,"the FCC received ");
            if(law[LAW_FREESPEECH]==-2) strcat(story,"thousands of");
            else if(law[LAW_FREESPEECH]==-1) strcat(story,"several hundred");
            else if(law[LAW_FREESPEECH]==0) strcat(story,"hundreds of");
            else if(law[LAW_FREESPEECH]==1) strcat(story,"dozens of");
            else strcat(story,"some");
            strcat(story," complaints ");
            strcat(story,"from irate listeners ");
            if(law[LAW_FREESPEECH]==-2) strcat(story,"across the nation. ");
            else if(law[LAW_FREESPEECH]==-1) strcat(story,"from all over the state. ");
            else if(law[LAW_FREESPEECH]==0) strcat(story,"within the county. ");
            else if(law[LAW_FREESPEECH]==1) strcat(story,"in neighboring towns. ");
            else strcat(story,"within the town. ");
            strcat(story," A spokesperson for the FCC ");
            strcat(story,"stated that the incident is under investigation.");
            strcat(story,"&r");
            break;
         }
         case VIEW_GUNCONTROL:
         {
            strcpy(story,cityname());
            char jstr[200],jstr2[200],jstr3[200],jstr4[200],jstr5[200],jg1,jg2,tg2[10];
            jg1=(LCSrandom(2)?GENDER_MALE:GENDER_FEMALE);
            jg2=(LCSrandom(2)?GENDER_MALE:GENDER_FEMALE);
            generate_long_name(jstr,jstr2,jstr3,jg1);
            generate_name(jstr4,jstr5,jg2);
            strcat(story," - In a surprising turn, a ");
            if (law[LAW_FREESPEECH]==-2) strcat(story, "[hurting spree]"); else strcat(story,"mass shooting");
            strcat(story," was prevented by a bystander with a gun.");
            strcat(story," After ");
            strcat(story,jstr);
            strcat(story," ");
            strcat(story,jstr2);
            strcat(story," opened fire at the ");
            strcat(story,jstr3);
            switch(LCSrandom(4))
            {
            case 0:strcat(story," Mall");break;
            case 1:strcat(story," Theater");break;
            case 2:strcat(story," High School");break;
            case 3:strcat(story," University");break;
            }
            strcat(story,", ");
            strcat(story,jstr4);
            strcat(story," ");
            strcat(story,jstr5);
            strcat(story," sprung into action. ");
            strcat(story,"The citizen pulled a concealed handgun and fired once at the shooter, ");
            strcat(story,"forcing ");
            strcat(story,jstr2);
            strcat(story," to take cover while others called the police.&r");
            strcat(story,"  Initially, ");
            if(jg2==GENDER_FEMALE)
            {
               if(LCSrandom(4)<law[LAW_WOMEN]+2) // 0% chance at law[LAW_WOMEN]==-2, 100% chance at law[LAW_WOMEN]==2
                  strcpy(tg2,"Ms. ");
               else
                  strcpy(tg2,(LCSrandom(2)?"Mrs. ":"Miss "));
            }
            else strcpy(tg2,"Mr. ");
            strcat(story,tg2);
            strcat(story,jstr5);
            strcat(story," attempted to talk down the shooter, but as ");
            strcat(story,jstr2);
            strcat(story," became more agitated, the heroic citizen was forced to engage the shooter in a ");
            if(law[LAW_FREESPEECH]==-2) strcat(story, "firefight, [putting the attacker to sleep] ");
            else strcat(story,"firefight, killing the attacker ");
            strcat(story,"before ");
            strcat(story,(jg1==GENDER_FEMALE?"she ":"he "));
            strcat(story,"could hurt anyone else.&r");
            strcat(story,"  The spokesperson for the police department said, \"We'd have a yet another ");
            if(law[LAW_FREESPEECH]==-2) strcat(story,"[hurting spree]");
            else strcat(story,"mass shooting");
            strcat(story," if not for ");
            strcat(story,tg2);
            strcat(story,jstr5);
            strcat(story,"'s heroic actions.\"");
            break;
         }
         case VIEW_PRISONS:
         {
            strcpy(story,cityname());
            strcat(story," - The hostage crisis at the ");
            char jstr[200];
            lastname(jstr,true);
            strcat(story,jstr);
            strcat(story," Correctional Facility ended tragically yesterday with the ");
            strcat(story,"death of both the prison guard being held hostage and ");
            char ggn=(LCSrandom(2)==1?GENDER_MALE:GENDER_FEMALE);
            strcat(story,(ggn==GENDER_FEMALE?"her":"his"));
            strcat(story," captor.");
            strcat(story,"&r");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"   Two weeks ago, convicted [reproduction fiend] ");
            else strcat(story,"   Two weeks ago, convicted rapist ");
            char dstr[200],dstr2[200];
            char dgn=(LCSrandom(2)==1?GENDER_MALE:GENDER_FEMALE);
            generate_name(dstr,dstr2,dgn);
            strcat(story,dstr);
            strcat(story," ");
            strcat(story,dstr2);
            strcat(story,", an inmate at ");
            strcat(story,jstr);
            strcat(story,", overpowered ");
            char gstr[200],gstr2[200];
            generate_name(gstr,gstr2,ggn);
            strcat(story,gstr);
            strcat(story," ");
            strcat(story,gstr2);
            strcat(story," and barricaded ");
            strcat(story,(dgn==GENDER_FEMALE?"herself":"himself"));
            strcat(story," with the guard in a prison tower.  ");
            strcat(story,"Authorities locked down the prison and ");
            strcat(story,"attempted to negotiate by phone for ");
            strcat(story,LCSrandom(18)+5);
            strcat(story," days, but talks were cut short when ");
            strcat(story,dstr2);
            strcat(story," reportedly screamed into the receiver \"");
            switch(LCSrandom(4))
            {
            case 0:
               if(law[LAW_FREESPEECH]==2)strcat(story,"Ah, fuck this shit.  This punk bitch is fuckin' dead!");
               else if(law[LAW_FREESPEECH]==-2)strcat(story,"Ah, [no way.]  This [police officer will be harmed!]");
               else strcat(story,"Ah, f*ck this sh*t.  This punk b*tch is f*ckin' dead!");
               break;
            case 1:
               if(law[LAW_FREESPEECH]==2)strcat(story,"Fuck a muthafuckin' bull.  I'm killin' this pig shit.");
               else if(law[LAW_FREESPEECH]==-2)strcat(story,"[Too late.]  [I am going to harm this police officer.]");
               else strcat(story,"F*ck a m*th*f*ck*n' bull.  I'm killin' this pig sh*t.");
               break;
            case 2:
               if(law[LAW_FREESPEECH]==2)strcat(story,"Why the fuck am I talkin' to you?  I'd rather kill this pig.");
               else if(law[LAW_FREESPEECH]==-2)strcat(story,"Why [am I] talkin' to you?  I'd rather [harm this police officer.]");
               else strcat(story,"Why the f*ck am I talkin' to you?  I'd rather kill this pig.");
               break;
             case 3:
               if(law[LAW_FREESPEECH]==2)strcat(story,"Imma kill all you bitches, startin' with this mothafucker here.");
               else if(law[LAW_FREESPEECH]==-2)strcat(story,"[I will harm all police officers], startin' with this [one] here.");
               else strcat(story,"Imma kill all you b*tches, startin' with this m*th*f*ck*r here.");
               break;
           }
            strcat(story,"\"");
            strcat(story,"  The tower was breached in an attempt to reach ");
            strcat(story,"the hostage, but ");
            strcat(story,dstr2);
            strcat(story," had already ");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"[harmed] the guard");
            else if(law[LAW_FREESPEECH]==-1)strcat(story,"killed the guard");
            else switch(LCSrandom(15))
            {
            case 0:strcat(story,"slit the guard's throat with a shank");break;
            case 1:strcat(story,"strangled the guard to death with a knotted bed sheet");break;
            case 2:strcat(story,"chewed out the guard's throat");break;
            case 3:strcat(story,"smashed the guard's skull with the toilet seat from ");
                   strcat(story,(dgn==GENDER_FEMALE?"her":"his"));
                   strcat(story," cell");break;
            case 4:strcat(story,"shot the guard with ");
                   strcat(story,(ggn==GENDER_FEMALE?"her":"his"));
                   strcat(story," own gun");break;
            case 5:strcat(story,"poisoned the guard with drugs smuggled into the prison by the ");
                   strcat(story,(LCSrandom(2)?"Crips":"Bloods"));break;
            case 6:strcat(story,"hit all 36 pressure points of death on the guard");break;
            case 7:strcat(story,"electrocuted the guard with high-voltage wires");break;
            case 8:strcat(story,"thrown the guard out the top-storey window");break;
            case 9:strcat(story,"taken the guard to the execution chamber and finished ");
                   strcat(story,(ggn==GENDER_FEMALE?"her":"him"));
                   strcat(story," off");break;
            case 10:strcat(story,"tricked another guard into shooting the guard dead");break;
            case 11:strcat(story,"burnt the guard to a crisp using a lighter and some gasoline");break;
            case 12:strcat(story,"eaten the guard's liver with some fava beans and a nice chianti");break;
            case 13:strcat(story,"performed deadly experiments on the guard unheard of since Dr. Mengele");break;
            case 14:strcat(story,"sacrificed the guard on a makeshift ");
                    strcat(story,(LCSrandom(2)?"Satanic":"neo-pagan"));
                    strcat(story," altar");break;
            }
            strcat(story,".  The prisoner was ");
            if(law[LAW_FREESPEECH]==-2)strcat(story,"[also harmed]");
            else strcat(story,"beaten to death");
            strcat(story," while \"resisting capture\", according to a prison spokesperson.");
            strcat(story,"&r");
            break;
         }
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
            /*
         case VIEW_POLITICALVIOLENCE:
            displaycenterednewsfont("NIGHTMARE",5);
            constructeventstory(story,ns.view,ns.positive);
            generatefiller(story,200);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            */
         case VIEW_GUNCONTROL:
            displaycenterednewsfont("MASS SHOOTING",5);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
         case VIEW_TAXES:
         {
            displaycenterednewsfont("REAGAN FLAWED",5);//XXX: "Reagan was wrong" or something?
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
            displaycenteredsmallnews("An enormous company files for bankruptcy, shattering the previous record.",12);// random company name
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
                  if(law[LAW_FREESPEECH]==-2&&law[LAW_WOMEN]!=-2)strcat(str,"regularly visits [working women].");
                  else if(law[LAW_FREESPEECH]==-2&&law[LAW_WOMEN]==-2)strcat(str,"regularly [donates to sperm banks].");
                  else strcat(str,"regularly visits prostitutes.");
                  break;
               case 1:
                  strcat(str,"seeks the aid of psychics.");
                  break;
               case 2:
                  strcat(str,"donated millions to the KKK.");
                  break;
               case 3:
                  strcat(str,"hasn't paid taxes in over 20 years.");
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
            break;
                }
        }
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
            /*
         case VIEW_POLITICALVIOLENCE:
            displaycenterednewsfont("END IN TEARS",5);
            constructeventstory(story,ns.view,ns.positive);
            generatefiller(story,200);
            displaynewsstory(story,storyx_s,storyx_e,13);
            break;
            */
         case VIEW_GUNCONTROL:
            displaycenterednewsfont("ARMED CITIZEN",5);
            displaycenterednewsfont("SAVES LIVES",13);
            constructeventstory(story,ns.view,ns.positive);
            displaynewsstory(story,storyx_s,storyx_e,21);
            break;
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
            if(law[LAW_FREESPEECH]==-2)displaycenterednewsfont("[JERKS]",5);
            else displaycenterednewsfont("BASTARDS",5);
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

void run_television_news_stories()
{
   char del;
   for(int n=len(newsstory)-1;n>=0;n--)
   {
      del=0;
      if(newsstory[n]->type==NEWSSTORY_MAJOREVENT)
      {
         if(newsstory[n]->positive)
         {
            switch(newsstory[n]->view)
            {
               case VIEW_POLICEBEHAVIOR:
                  music.play(MUSIC_LACOPS);
                  movie.loadmovie("lacops.cmv");
                  movie.playmovie(0,0);

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(19,13);
                  addstr("ษออออออออออออออออออออออออออออออออออออออออออออออออออออป");
                  move(20,13);
                  addstr("บ     The  police  have  beaten  a  black  man  in   บ");
                  move(21,13);
                  addstr("บ   Los Angeles again.  This time, the incident is   บ");
                  move(22,13);
                  addstr("บ   taped by  a passerby  and saturates  the news.   บ");
                  move(23,13);
                  addstr("ศออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

                  getkey();

                  del=1;
                  break;
               case VIEW_CABLENEWS:
               {
                  music.play(MUSIC_NEWSCAST);
                  char str[80];
                  strcpy(str,"Tonight on a Cable News channel: ");
                  switch(LCSrandom(5))
                  {
                  case 0:strcat(str,"Cross");break;
                  case 1:strcat(str,"Hard");break;
                  case 2:strcat(str,"Lightning");break;
                  case 3:strcat(str,"Washington");break;
                  case 4:strcat(str,"Capital");break;
                  }
                  switch(LCSrandom(5))
                  {
                  case 0:strcat(str," Fire");break;
                  case 1:strcat(str," Ball");break;
                  case 2:strcat(str," Talk");break;
                  case 3:strcat(str," Insider");break;
                  case 4:strcat(str," Gang");break;
                  }
                  strcat(str," with ");
                  char bname[80];
                  generate_name(bname,GENDER_WHITEMALEPATRIARCH);
                  strcat(str,bname);

                  erase();
                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(0,39-((len(str)-1)>>1));
                  addstr(str);

                  move(16,20);
                  addstr(bname);
                  move(17,20);
                  switch(LCSrandom(3))
                  {
                  case 0:addstr("Washington, DC");break;
                  case 1:addstr("New York, NY");break;
                  case 2:addstr("Atlanta, GA");break;
                  }

                  move(16,41);
                  generate_name(bname);
                  addstr(bname);
                  move(17,41);
                  switch(LCSrandom(4))
                  {
                  case 0:addstr("Eugene, OR");break;
                  case 1:addstr("San Francisco, CA");break;
                  case 2:addstr("Cambridge, MA");break;
                  case 3:addstr("Ithaca, NY");break;
                  }

                  movie.loadmovie("newscast.cmv");
                  movie.playmovie(1,1);

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(19,13);
                  addstr("ษออออออออออออออออออออออออออออออออออออออออออออออออออออป");
                  move(20,13);
                  addstr("บ     A  Cable  News  anchor  accidentally  let  a   บ");
                  move(21,13);
                  addstr("บ   bright Liberal guest  finish a sentence.  Many   บ");
                  move(22,13);
                  addstr("บ   viewers  across  the  nation  were  listening.   บ");
                  move(23,13);
                  addstr("ศออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

                  getkey();

                  del=1;
                  break;
               }
            }
         }
         else
         {
            switch(newsstory[n]->view)
            {
               case VIEW_CEOSALARY:
                  music.play(MUSIC_GLAMSHOW);
                  movie.loadmovie("glamshow.cmv");
                  movie.playmovie(0,0);

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(19,13);
                  addstr("ษออออออออออออออออออออออออออออออออออออออออออออออออออออป");
                  move(20,13);
                  addstr("บ     A new show glamorizing the lives of the rich   บ");
                  move(21,13);
                  addstr("บ   begins airing  this week.  With the nationwide   บ");
                  move(22,13);
                  addstr("บ   advertising  blitz, it's bound  to be popular.   บ");
                  move(23,13);
                  addstr("ศออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

                  getkey();

                  del=1;
                  break;
               case VIEW_CABLENEWS:
                  music.play(MUSIC_ANCHOR);
                  movie.loadmovie("anchor.cmv");
                  movie.playmovie(0,0);

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(19,13);
                  addstr("ษออออออออออออออออออออออออออออออออออออออออออออออออออออป");
                  move(20,13);
                  addstr("บ     A major Cable News channel has hired a slick   บ");
                  move(21,13);
                  addstr("บ   new anchor for  one of its news shows.  Guided   บ");
                  move(22,13);
                  addstr("บ   by impressive  advertising, America  tunes in.   บ");
                  move(23,13);
                  addstr("ศออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

                  getkey();

                  del=1;
                  break;
               case VIEW_WOMEN:
                  music.play(MUSIC_ABORT);
                  erase();

                  movie.loadmovie("abort.cmv");
                  movie.playmovie(0,0);

                  set_color(COLOR_WHITE,COLOR_BLACK,1);
                  move(19,13);
                  addstr("ษออออออออออออออออออออออออออออออออออออออออออออออออออออป");
                  move(20,13);
                  addstr("บ     A  failed partial  birth abortion  goes on a   บ");
                  move(21,13);
                  addstr("บ   popular  afternoon  talk  show.    The  studio   บ");
                  move(22,13);
                  addstr("บ   audience and viewers nationwide feel its pain.   บ");
                  move(23,13);
                  addstr("ศออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

                  getkey();

                  del=1;
                  break;
            }
         }
      }
      if(del) delete_and_remove(newsstory,n);
   }
}
