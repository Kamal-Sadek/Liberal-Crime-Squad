//#include "includes.h"
#include "externs.h"

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

/* pick a random state */
const char* statename(int i)
{
   if(i < 0) i = LCSrandom(50);

   switch(LCSrandom(50))
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
      addch('M');
   }
   if(amendnum>=900)
   {
      amendnum-=900;
      addch('C');
      addch('M');
   }
   if(amendnum>=500)
   {
      amendnum-=500;
      addch('D');
   }
   if(amendnum>=400)
   {
      amendnum-=400;
      addch('C');
      addch('D');
   }
   while(amendnum>=100)
   {
      amendnum-=100;
      addch('C');
   }
   if(amendnum>=90)
   {
      amendnum-=90;
      addch('X');
      addch('C');
   }
   if(amendnum>=50)
   {
      amendnum-=50;
      addch('L');
   }
   if(amendnum>=40)
   {
      amendnum-=40;
      addch('X');
      addch('L');
   }
   while(amendnum>=10)
   {
      amendnum-=10;
      addch('X');
   }
   if(amendnum>=9)
   {
      amendnum-=9;
      addch('I');
      addch('X');
   }
   if(amendnum>=5)
   {
      amendnum-=5;
      addch('V');
   }
   if(amendnum>=4)
   {
      amendnum-=4;
      addch('I');
      addch('V');
   }
   while(amendnum>=1)
   {
      amendnum-=1;
      addch('I');
   }
}

/*
   Select one string randomly from a table of strings, e.g.
   const char *strtbl[] = {"abc", "d", "efgh"};
   SelectRandomString(strtbl, 3);
*/
const char *selectRandomString(const char **string_table, int table_size)
{
   int roll;

   roll = LCSrandom(table_size);

   return(string_table[roll]);
}

