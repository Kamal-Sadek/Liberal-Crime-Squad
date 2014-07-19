
//#include "includes.h"
#include "externs.h"
#include "news/news.h"

std::string getLastNameForHeadline(char* fullName)
{
   int i=0;
   int j=-1;
   char lastName[20];
   // Parse through full name to get the last name
   for(i=0; fullName[i]!=0; i++)
   {
      // Start recording last name at the space between first and last
      if(fullName[i]==' ')
      {
         j=0;
      }
      // When recording last name, transcribe in uppercase
      else if(j >= 0)
      {
         lastName[j++] = toupper(fullName[i]);
      }
   }
   lastName[j] = 0; // To finish, NULL terminate the transcribed string

   // Write to std::string
   std::string ret = lastName;
   return ret;
}

void displaystoryheader(newsstoryst& ns, bool liberalguardian, int& y, int header)
{
   switch(ns.type)
   {
   case NEWSSTORY_PRESIDENT_IMPEACHED:
      displaycenterednewsfont(getLastNameForHeadline(oldPresidentName), 5);
      displaycenterednewsfont("IMPEACHED",13);
      break;
   case NEWSSTORY_PRESIDENT_BELIEVED_DEAD:
      displaycenterednewsfont(getLastNameForHeadline(oldPresidentName), 5);
      displaycenterednewsfont("BELIEVED DEAD",13);
      break;
   case NEWSSTORY_PRESIDENT_FOUND_DEAD:
      displaycenterednewsfont(getLastNameForHeadline(oldPresidentName), 5);
      displaycenterednewsfont("FOUND DEAD",13);
      break;
   case NEWSSTORY_PRESIDENT_FOUND:
      displaycenterednewsfont(getLastNameForHeadline(oldPresidentName), 5);
      displaycenterednewsfont("RESCUED",13);
      break;
   case NEWSSTORY_PRESIDENT_KIDNAPPED:
      displaycenterednewsfont(getLastNameForHeadline(oldPresidentName), 5);
      displaycenterednewsfont("KIDNAPPED",13);
      break;
   case NEWSSTORY_PRESIDENT_MISSING:
      displaycenterednewsfont(getLastNameForHeadline(oldPresidentName), 5);
      displaycenterednewsfont("MISSING",13);
      break;
   case NEWSSTORY_PRESIDENT_ASSASSINATED:
      displaycenterednewsfont(getLastNameForHeadline(oldPresidentName), 5);
      displaycenterednewsfont("ASSASSINATED",13);
      break;
   case NEWSSTORY_CCS_NOBACKERS:
      displaycenterednewsfont("FBI HUNTS CCS",5);
      y=13;
      break;
   case NEWSSTORY_CCS_DEFEATED:
      displaycenterednewsfont("RAIDS END CCS",5);
      y=13;
      break;
   case NEWSSTORY_NUDITYARREST:
   case NEWSSTORY_CARTHEFT:
   case NEWSSTORY_WANTEDARREST:
   case NEWSSTORY_DRUGARREST:
   case NEWSSTORY_GRAFFITIARREST:
   case NEWSSTORY_BURIALARREST:
      displaycenterednewsfont("POLICE KILLED",5);
      y=13;
      break;
   case NEWSSTORY_SQUAD_ESCAPED:
   case NEWSSTORY_SQUAD_FLEDATTACK:
      displaycenterednewsfont("LCS ESCAPES",5);
      displaycenterednewsfont("POLICE SIEGE",13);
      break;
   case NEWSSTORY_SQUAD_DEFENDED:
   case NEWSSTORY_SQUAD_BROKESIEGE:
      displaycenterednewsfont("LCS FIGHTS",5);
      displaycenterednewsfont("OFF COPS",13);
      break;
   case NEWSSTORY_SQUAD_KILLED_SIEGEATTACK:
   case NEWSSTORY_SQUAD_KILLED_SIEGEESCAPE:
      if(!liberalguardian)
      {
         displaycenterednewsfont("LCS SIEGE",5);
         displaycenterednewsfont("TRAGIC END",13);
      }
      else
      {
         displaycenterednewsfont("POLICE KILL",5);
         displaycenterednewsfont("LCS MARTYRS",13);
      }
      break;
   case NEWSSTORY_CCS_SITE:
   case NEWSSTORY_CCS_KILLED_SITE:
      if(newscherrybusted<2)
      {
         displaycenterednewsfont("CONSERVATIVE",5);
         displaycenterednewsfont("CRIME SQUAD",13);
      }
      else
      {
         if(ns.positive)
            displaycenterednewsfont("CCS STRIKES",5);//AGAIN?
         else
            displaycenterednewsfont("CCS RAMPAGE",5);
         y=13;
      }
      break;
   default:
      if(ns.positive)
      {
         if(newscherrybusted||liberalguardian)
         {

            if(!liberalguardian)
            {
               if(ns.priority>250)
               {
                  y=13;
                  displaycenterednewsfont("UNSTOPPABLE",5);
               }
               else
               {
                  y=13;
                  displaycenterednewsfont("LCS STRIKES",5);
               }
            }
            else
            {
               y=13;
               if(ns.priority>150)
               {
                  change_public_opinion(header,5,1); // Bonus for big story
                  switch(header)
                  {
                  case VIEW_TAXES:
                  case VIEW_SWEATSHOPS:
                  case VIEW_CEOSALARY:
                     displaycenterednewsfont("CLASS WAR",5);
                     break;
                  case VIEW_NUCLEARPOWER:
                     displaycenterednewsfont("MELTDOWN RISK",5);
                     break;
                  case VIEW_POLICEBEHAVIOR:
                     displaycenterednewsfont("LCS VS COPS",5);
                     break;
                  case VIEW_DEATHPENALTY:
                     displaycenterednewsfont("PRISON WAR",5);
                     break;
                  case VIEW_INTELLIGENCE:
                     displaycenterednewsfont("LCS VS CIA",5);
                     break;
                  case VIEW_ANIMALRESEARCH:
                  case VIEW_GENETICS:
                     displaycenterednewsfont("EVIL RESEARCH",5);
                     break;
                  case VIEW_FREESPEECH:
                  case VIEW_GAY:
                  case VIEW_JUSTICES:
                     displaycenterednewsfont("NO JUSTICE",5);
                     break;
                  case VIEW_POLLUTION:
                     displaycenterednewsfont("POLLUTER HIT",5);
                     break;
                  case VIEW_CORPORATECULTURE:
                     displaycenterednewsfont("LCS HITS CORP",5);
                     break;
                  case VIEW_AMRADIO:
                     displaycenterednewsfont("LCS HITS AM",5);
                     break;
                  case VIEW_CABLENEWS:
                     displaycenterednewsfont("LCS HITS TV",5);
                     break;
                  default:
                     displaycenterednewsfont("HEROIC STRIKE",5);
                  }
               }
               else
               {
                  displaycenterednewsfont("LCS STRIKES",5);
               }
            }
         }
         else
         {
            displaycenterednewsfont("LIBERAL CRIME",5);
            displaycenterednewsfont("SQUAD STRIKES",13);
         }
      }
      else
      {
         if(newscherrybusted||liberalguardian)
         {
            if(!liberalguardian)
               displaycenterednewsfont("LCS RAMPAGE",5);
            else
               displaycenterednewsfont("LCS SORRY",5);
            y=13;
         }
         else
         {
            displaycenterednewsfont("LIBERAL CRIME",5);
            displaycenterednewsfont("SQUAD RAMPAGE",13);
         }
      }
      break;
   }
}
