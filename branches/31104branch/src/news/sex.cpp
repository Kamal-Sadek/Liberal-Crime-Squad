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
