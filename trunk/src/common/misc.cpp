#include <externs.h>

/* pick a descriptor acronym */
void sexdesc(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(4))
   { // Descriptors
   case 0:strcat(str,"DTE");break;
   case 1:strcat(str,"ND");break;
   case 2:strcat(str,"NS");break;
   case 3:strcat(str,"VGL");break;

   default:strcat(str,"FOO");break;
   }
}

/* what kind of person? */
void sexwho(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(35))
   { // Who
   case 0:strcat(str,"BB");break;
   case 1:strcat(str,"BBC");break;
   case 2:strcat(str,"BF");break;
   case 3:strcat(str,"BHM");break;
   case 4:strcat(str,"BiF");break;
   case 5:strcat(str,"BiM");break;
   case 6:strcat(str,"BBW");break;
   case 7:strcat(str,"BMW");break; // lol big mexican woman
   case 8:strcat(str,"CD");break;
   case 9:strcat(str,"DWF");break;
   case 10:strcat(str,"DWM");break;
   case 11:strcat(str,"FTM");break;
   case 12:strcat(str,"GAM");break;
   case 13:strcat(str,"GBM");break;
   case 14:strcat(str,"GF");break;
   case 15:strcat(str,"GG");break;
   case 16:strcat(str,"GHM");break;
   case 17:strcat(str,"GWC");break;
   case 18:strcat(str,"GWF");break;
   case 19:strcat(str,"GWM");break;
   case 20:strcat(str,"MBC");break;
   case 21:strcat(str,"MBiC");break;
   case 22:strcat(str,"MHC");break;
   case 23:strcat(str,"MTF");break;
   case 24:strcat(str,"MWC");break;
   case 25:strcat(str,"SBF");break;
   case 26:strcat(str,"SBM");break;
   case 27:strcat(str,"SBiF");break;
   case 28:strcat(str,"SBiM");break;
   case 29:strcat(str,"SSBBW");break;
   case 30:strcat(str,"SWF");break;
   case 31:strcat(str,"SWM");break;
   case 32:strcat(str,"TG");break;
   case 33:strcat(str,"TS");break;
   case 34:strcat(str,"TV");break;

   default:strcat(str,"BAR");break;
   }
}

/* seeking acronym */
void sexseek(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(2))
   { // ISO
   case 0:strcat(str,"ISO");break;
   case 1:strcat(str,"LF");break;
   default:strcat(str,"BAZ");break;
   }
}

/* what type of sex? */
void sextype(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(14))
   { // TYPE
   case 0:strcat(str,"225");break;
   case 1:strcat(str,"ATM");break;
   case 2:strcat(str,"BDSM");break;
   case 3:strcat(str,"CBT");break;
   case 4:strcat(str,"BJ");break;
   case 5:strcat(str,"DP");break;
   case 6:strcat(str,"D/s");break;
   case 7:strcat(str,"GB");break;
   case 8:strcat(str,"HJ");break;
   case 9:strcat(str,"OTK");break;
   case 10:strcat(str,"PNP");break;
   case 11:strcat(str,"TT");break;
   case 12:strcat(str,"SWS");break;
   case 13:strcat(str,"W/S");break;
   default:strcat(str,"LOL");break;
   }
}

/* generate a vanity plate for the squad. */
/*void vanity(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(20))
   {
   case 0:strcat(str,"ABORTN");break;
   case 1:strcat(str,"LCS-"LCSrandom(50)+1);break;
   case 2:strcat(str,"LIB4LYFE");break;
   case 3:strcat(str,"");break;
   case 4:strcat(str,"");break;
   case 5:strcat(str,"");break;
   case 6:strcat(str,"");break;
   case 7:strcat(str,"");break;
   case 8:strcat(str,"");break;
   case 9:strcat(str,"");break;
   case 10:strcat(str,"");break;
   case 11:strcat(str,"");break;
   case 12:strcat(str,"");break;
   case 13:strcat(str,"");break;
   case 14:strcat(str,"");break;
   case 15:strcat(str,"");break;
   case 16:strcat(str,"");break;
   case 17:strcat(str,"");break;
   case 18:strcat(str,"");break;
   case 19:strcat(str,"");break;
   }
}
*/

/* return a letter, number, or one of either. */
void chooseLetterOrNumber(char *str, int type)
{
    strcpy(str,"");

    if(type == 1) // Choose a letter.
    {
        char randChar = 'a' + LCSrandom(26);
        addstr(&randChar);
    } else if(type == 2) // Choose a number.
    {
        char randNum = LCSrandom(10);
        addstr(&randNum);
    } else if(type == 3) {// Choose one of either.
        if(LCSrandom(36) > 25) {
            char randNum = LCSrandom(10);
            addstr(&randNum);
        } else {
        char randChar = 'a' + LCSrandom(26);
        addstr(&randChar);
        }
    } else {
        addstr("-ERR-");
    }
}

/* generate a non-vanity plate for the squad. */
void plate(char *str)
{
   strcpy(str,"");

   switch(LCSrandom(8)) // Actual license plate formats.
   {
   case 0: // 123 ABC
         // CT,WA
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      strcat(str," ");
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 1);
      break;
   case 1: // 12A B34
         // CT,MA
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 1);
      strcat(str," ");
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      break;
   case 2: // 123 4567
         // NH
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      strcat(str," ");
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      break;
   case 3: // ABC 1234
         // PA, NY, MI, MD
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 1);
      strcat(str," ");
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      break;
   case 4: // 12A34
         // School Buses.
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      break;
   case 5: // A 12345
         // Trucks, etc.
      chooseLetterOrNumber(str, 1);
      strcat(str," ");
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      break;
   case 6: // 1AB 234
         // CT, MA
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 1);
      strcat(str," ");
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      break;
   case 7: // ABC 123
         // VT, MD
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 1);
      chooseLetterOrNumber(str, 1);
      strcat(str," ");
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      chooseLetterOrNumber(str, 2);
      break;
   }
}

/* pick a random state, or give the name of the specified state */
const char* statename(int i)
{
   if(i < 0 || i >= 50) i = LCSrandom(50);

   switch(i)
   {
   case 0:return "Alabama";
   case 1:return "Alaska";
   case 2:return "Arkansas";
   case 3:return "Arizona";
   case 4:return "California";
   case 5:return "Colorado";
   case 6:return "Connecticut";
   case 7:return "Delaware";
   case 8:return "Florida";
   case 9:return "Georgia";
   case 10:return "Hawaii";
   case 11:return "Idaho";
   case 12:return "Illinois";
   case 13:return "Indiana";
   case 14:return "Iowa";
   case 15:return "Kansas";
   case 16:return "Kentucky";
   case 17:return "Louisiana";
   case 18:return "Maine";
   case 19:return "Maryland";
   case 20:return "Massachusetts";
   case 21:return "Michigan";
   case 22:return "Minnesota";
   case 23:return "Mississippi";
   case 24:return "Missouri";
   case 25:return "Montana";
   case 26:return "Nebraska";
   case 27:return "Nevada";
   case 28:return "New Hampshire";
   case 29:return "New Jersey";
   case 30:return "New Mexico";
   case 31:return "New York";
   case 32:return "North Carolina";
   case 33:return "North Dakota";
   case 34:return "Ohio";
   case 35:return "Oklahoma";
   case 36:return "Oregon";
   case 37:return "Pennsylvania";
   case 38:return "Rhode Island";
   case 39:return "South Carolina";
   case 40:return "South Dakota";
   case 41:return "Tennessee";
   case 42:return "Texas";
   case 43:return "Utah";
   case 44:return "Vermont";
   case 45:return "Virginia";
   case 46:return "Washington";
   case 47:return "West Virginia";
   case 48:return "Wisconsin";
   case 49:return "Wyoming";

   default:return "Hohoq"; // The Areas of My Expertise -- John Hodgman
   }
}

/* endgame - converts an integer into a roman numeral for amendments */
void romannumeral(int amendnum)
{
   while(amendnum>=1000)
   {
      amendnum-=1000;
      addchar('M');
   }
   if(amendnum>=900)
   {
      amendnum-=900;
      addchar('C');
      addchar('M');
   }
   if(amendnum>=500)
   {
      amendnum-=500;
      addchar('D');
   }
   if(amendnum>=400)
   {
      amendnum-=400;
      addchar('C');
      addchar('D');
   }
   while(amendnum>=100)
   {
      amendnum-=100;
      addchar('C');
   }
   if(amendnum>=90)
   {
      amendnum-=90;
      addchar('X');
      addchar('C');
   }
   if(amendnum>=50)
   {
      amendnum-=50;
      addchar('L');
   }
   if(amendnum>=40)
   {
      amendnum-=40;
      addchar('X');
      addchar('L');
   }
   while(amendnum>=10)
   {
      amendnum-=10;
      addchar('X');
   }
   if(amendnum>=9)
   {
      amendnum-=9;
      addchar('I');
      addchar('X');
   }
   if(amendnum>=5)
   {
      amendnum-=5;
      addchar('V');
   }
   if(amendnum>=4)
   {
      amendnum-=4;
      addchar('I');
      addchar('V');
   }
   while(amendnum>=1)
   {
      amendnum-=1;
      addchar('I');
   }
}

// Sets the interval according to a string that is either a number or two
// number separated by a dash. Returns false and does not change the
// interval if the given string is not a valid interval.
bool Interval::set_interval(const string& interval)
{
   if(!len(interval) ||
      interval.find_first_not_of("1234567890-")!=string::npos)
      return false;

   size_t dashpos=interval.find('-',1);
   if(dashpos==string::npos) // Just a constant.
   {
      if(!valid(interval)) return false;
      max=min=atoi(interval);
   }
   else
   {
      string smin=interval.substr(0,dashpos),smax=interval.substr(dashpos+1);
      if(!valid(smin)||!valid(smax)) return false;
      int tmin=atoi(smin),tmax=atoi(smax);
      if(tmin>tmax) return false;
      min=tmin,max=tmax;
   }
   return true;
}

#ifndef DONT_INCLUDE_SDL
/* helper function for initsongs() */
void MusicClass::loadmidi(int i,const char* filename)
{  // the reason it prints progress on the screen is because it might be a little slow sometimes so this reassures the user progress is being made
   erase();
   mvaddstr(12,10,"Loading MIDI music ("+tostring(i+1)+"/"+tostring(MUSIC_OFF)+"): "+filename);
   refresh();
   songs[i]=Mix_LoadMUS((string(artdir)+filename).c_str());
   if(!songs[i]) // there was an error with Mix_LoadMUS()
      gamelog.log(string("SDL_mixer function Mix_LoadMUS() failed to load ")+filename+":  "+Mix_GetError()); // Music failed to load
}
#endif // DONT_INCLUDE_SDL

/* initialize songs */
void MusicClass::init()
{
#ifndef DONT_INCLUDE_SDL
   if(songsinitialized) return; // only initialize once
   // titlemode.mid - The Liberty Bell March by John Philip Sousa
   loadmidi(MUSIC_TITLEMODE,"titlemode.mid"); // load title mode music
   // newgame.mid - Also sprach Zarathustra, introduction by Richard Strauss
   loadmidi(MUSIC_NEWGAME,"newgame.mid"); // load new game music
   // basemode.mid - The Stars and Stripes Forever by John Philip Sousa
   loadmidi(MUSIC_BASEMODE,"basemode.mid"); // load regular base mode music
   // siege.mid - The Planets, 1st Movement "Mars" by Gustav Holst
   loadmidi(MUSIC_SIEGE,"siege.mid"); // load base mode while under siege music
   // activate.mid - Piano Sonata #11, 3rd Movement "Rondo Alla Turca" by Wolfgang Amadeus Mozart
   loadmidi(MUSIC_ACTIVATE,"activate.mid"); // load activate Liberals music
   // sleepers.mid - Toccata and Fugue in D Minor by Johann Sebastian Bach
   loadmidi(MUSIC_SLEEPERS,"sleepers.mid"); // load activate Sleepers music
   // stopevil.mid - Hungarian Dance #5 by Johannes Brahms (based on the csardas "Bartfai emlek" by Bela Keler)
   loadmidi(MUSIC_STOPEVIL,"stopevil.mid"); // load go forth to stop evil music
   // reviewmode.mid - Symphony #94, 2nd Movement "Surprise Symphony" by Joseph Haydn
   loadmidi(MUSIC_REVIEWMODE,"reviewmode.mid"); // load review mode music
   // liberalagenda.mid - Beautiful Dreamer by Stephen Foster
   loadmidi(MUSIC_LIBERALAGENDA,"liberalagenda.mid"); // load status of the Liberal agenda music
   // disbanded.mid - La Cucaracha, a traditional Mexican folk song originally from Spain
   loadmidi(MUSIC_DISBANDED,"disbanded.mid"); // load disbanded music
   // finances.mid - Minuet in G Major by Christian Petzold
   loadmidi(MUSIC_FINANCES,"finances.mid"); // load finance report music
   // cartheft.mid - The Ride of the Valkyries by Richard Wanger
   loadmidi(MUSIC_CARTHEFT,"cartheft.mid"); // load car theft music
   // elections.mid - Habanera from Carmen by Georges Bizet
   loadmidi(MUSIC_ELECTIONS,"elections.mid"); // load elections music
   // shopping.mid - The Entertainer by Scott Joplin
   loadmidi(MUSIC_SHOPPING,"shopping.mid"); // load shopping music
   // sitemode.mid - Dance of the Sugar Plum Fairy by Pyotr Ilyich Tchaikovsky
   loadmidi(MUSIC_SITEMODE,"sitemode.mid"); // load site mode music
   // suspicious.mid - Hall of the Mountain King by Edvard Grieg
   loadmidi(MUSIC_SUSPICIOUS,"suspicious.mid"); // load suspicious music
   // alarmed.mid - 5th Symphony, 1st Movement by Ludwig van Beethoven
   loadmidi(MUSIC_ALARMED,"alarmed.mid"); // load alarmed music
   // heavycombat.mid - 6th Symphony "Pastorale", 4th Movement by Ludwig van Beethoven
   loadmidi(MUSIC_HEAVYCOMBAT,"heavycombat.mid"); // load massive Conservative response or escaping/engaging a siege music
   // carchase.mid - The William Tell Overture by Gioacchino Antonio Rossini
   loadmidi(MUSIC_CARCHASE,"carchase.mid"); // load car chase music
   // footchase.mid - The Maple Leaf Rag by Scott Joplin
   loadmidi(MUSIC_FOOTCHASE,"footchase.mid"); // load foot chase music
   // interrogation.mid - Night on Bald Mountain by Modest Mussorgsky
   loadmidi(MUSIC_INTERROGATION,"interrogation.mid"); // load interrogation music
   // trial.mid - Hungarian Rhapsody #2 by Franz Liszt
   loadmidi(MUSIC_TRIAL,"trial.mid"); // load trial music
   // recruiting.mid - Dance of the Hours by Amilcare Ponchielli
   loadmidi(MUSIC_RECRUITING,"recruiting.mid"); // load recruiting music
   // dating.mid - The Blue Danube Waltz by Johann Strauss Jr.
   loadmidi(MUSIC_DATING,"dating.mid"); // load dating music
   // newspaper.mid - Eine Kleine Nachtmusik, 1st Movement by Wolfgang Amadeus Mozart
   loadmidi(MUSIC_NEWSPAPER,"newspaper.mid"); // load newspaper music
   // lacops.mid - The Flight of the Bumblebee by Nikolai Rimsky-Korsakov
   loadmidi(MUSIC_LACOPS,"lacops.mid"); // load LA cops beating black man and getting caught on video music
   // newscast.mid - La Marseillaise, The French National Anthem by Claude Joseph Rouget de Lisle
   loadmidi(MUSIC_NEWSCAST,"newscast.mid"); // load newscast where smart liberal guest gets some words in edgewise music
   // glamshow.mid - Das Deutschlandlied, The German National Anthem by Joseph Haydn
   loadmidi(MUSIC_GLAMSHOW,"glamshow.mid"); // load glamorous TV show about lifestyles of the rich and famous music
   // anchor.mid - I Am the Very Model of a Modern Major-General by Sir Arthur Seymour Sullivan
   loadmidi(MUSIC_ANCHOR,"anchor.mid"); // load handsome charismatic new Conservative cable news anchor music
   // abort.mid - Tarantella Napoletana, a traditional Italian folk song from Naples
   loadmidi(MUSIC_ABORT,"abort.mid"); // load failed partial birth abortion on trashy daytime talk show music
   // victory.mid - The Star-Spangled Banner, The U.S. National Anthem by John Stafford Smith
   loadmidi(MUSIC_VICTORY,"victory.mid"); // load victory music
   // defeat.mid - Piano Sonata #2, 3rd Movement "Funeral March" by Frederic Francois Chopin
   loadmidi(MUSIC_DEFEAT,"defeat.mid"); // load defeat music
   // reagainified.mid - Dixie, The Confederate National Anthem by Daniel Decatur Emmett
   loadmidi(MUSIC_REAGANIFIED,"reaganified.mid"); // load Reaganified music
   // stalinized.mid - The Soviet (and now Russian) National Anthem by Alexander Vasilyevich Alexandrov
   loadmidi(MUSIC_STALINIZED,"stalinized.mid"); // load Stalinized music
   erase();
   refresh();
   songsinitialized=true;
#endif // DONT_INCLUDE_SDL
}

/* play music specified by a MusicMode */
void MusicClass::play(int _musicmode)
{
#ifndef DONT_INCLUDE_SDL
   if(!songsinitialized) init(); // if it hasn't been initialized already, do it now

   if(_musicmode==MUSIC_CURRENT) return; // keep playing current music if that's what's requested
   if(_musicmode==MUSIC_RANDOM) _musicmode=LCSrandom(MUSIC_OFF); // play a random song if that's what's requested
   if(_musicmode==MUSIC_PREVIOUS) _musicmode=previous; // restore previous setting if that's what's requested
   if(musicmode==_musicmode) return; // already playing the right music
   previous=musicmode; // store previous setting
   musicmode=_musicmode; // set musicmode to input

   Mix_HaltMusic(); // stop any music that we're playing

   if(musicmode<0||musicmode>=MUSIC_OFF)
   {
      musicmode=MUSIC_OFF; // just in case we had odd input, make sure we keep track that music is off
      return; // return without playing music
   }

   if(!songs[musicmode]) // there was an error with Mix_LoadMUS() back when it was called on this song
      return; // we can't play music if it isn't loaded, might as well return

   if(Mix_PlayMusic(songs[musicmode],-1)!=0) // start playing the music, and have it loop indefinitely
      gamelog.log(string("SDL_mixer function Mix_PlayMusic() failed:  ")+Mix_GetError()); // Music failed to play

   enableIf(isEnabled());
#endif // DONT_INCLUDE_SDL
}
