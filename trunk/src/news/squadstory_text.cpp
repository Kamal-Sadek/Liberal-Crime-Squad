

//#include "includes.h"
#include "externs.h"

void squadstory_text_location(newsstoryst& ns,bool liberalguardian,bool ccs,char* story)
{
   strcat(story,"  The events took place at the ");
   if(liberalguardian&&!ccs)strcat(story,"notorious ");
   if(ccs)
   {
      switch(location[ns.loc]->type)
      {
      case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
         strcat(story,"University Dormitory.  ");break;
      case SITE_BUSINESS_CIGARBAR:
         strcat(story,"Lady Luck Strip Club.  ");break;
      case SITE_LABORATORY_COSMETICS:
         strcat(story,"Animal Shelter.  ");break;
      case SITE_LABORATORY_GENETIC:
         strcat(story,"Research Ethics Commission HQ.  ");break;
      case SITE_GOVERNMENT_POLICESTATION:
         strcat(story,"Seedy Back Alley(tm).  ");break;
      case SITE_GOVERNMENT_COURTHOUSE:
         strcat(story,"Abortion Clinic.  ");break;
      case SITE_GOVERNMENT_PRISON:
         strcat(story,"Rehabilitation Center.  ");break;
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
         strcat(story,"ACLU Branch Office.  ");break;
      case SITE_INDUSTRY_SWEATSHOP:
         strcat(story,"Labor Union HQ.  ");break;
      case SITE_INDUSTRY_POLLUTER:
         strcat(story,"Greenpeace Offices.  ");break;
      case SITE_INDUSTRY_NUCLEAR:
         strcat(story,"Whirled Peas Museum.  ");break;
      case SITE_CORPORATE_HEADQUARTERS:
         strcat(story,"Welfare Assistance Agency.  ");break;
      case SITE_CORPORATE_HOUSE:
         strcat(story,"Richard Dawkins Food Bank.  ");break;
      case SITE_MEDIA_AMRADIO:
         strcat(story,"Public Radio Station.  ");break;
      case SITE_MEDIA_CABLENEWS:
         strcat(story,"Network News Station.  ");break;
      case SITE_GOVERNMENT_ARMYBASE:
         strcat(story,"Greenpeace Offices.  ");break;
      case SITE_GOVERNMENT_FIRESTATION:
         strcat(story,"ACLU Branch Office.  ");break;
      case SITE_BUSINESS_BANK:
         strcat(story,"Richard Dawkins Food Bank.  ");break;
      default:
         strcat(story,location[ns.loc]->name);
         strcat(story,".  ");break;
      }
   }
   else strcat(story,location[ns.loc]->name);
   if(liberalguardian&&!ccs)
   {
      switch(location[ns.loc]->type)
      {
      case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
         strcat(story,", known for its rich and snooty residents.  ");break;
      case SITE_BUSINESS_CIGARBAR:
         strcat(story,", a spawning ground of Wrong Conservative Ideas.  ");break;
      case SITE_LABORATORY_COSMETICS:
         strcat(story,", a Conservative animal rights abuser.  ");break;
      case SITE_LABORATORY_GENETIC:
         strcat(story,", a dangerous Conservative genetic research lab.  ");break;
      case SITE_GOVERNMENT_POLICESTATION:
         strcat(story,", headquarters of one of the most oppressive and Conservative police forces in the country.  ");break;
      case SITE_GOVERNMENT_COURTHOUSE:
         strcat(story,", site of numerous Conservative Injustices.  ");break;
      case SITE_GOVERNMENT_PRISON:
         strcat(story,", where innocent people are regularly beaten by Conservative guards.  ");break;
      case SITE_GOVERNMENT_INTELLIGENCEHQ:
         strcat(story,", the Conservative headquarters of one of the biggest privacy violators in the world.  ");break;
      case SITE_GOVERNMENT_ARMYBASE:
         strcat(story,", pride of Conservative torturers and warmongers everywhere.  ");break;
      case SITE_INDUSTRY_SWEATSHOP:
         strcat(story,", a Conservative sweatshop and human rights abuser.  ");break;
      case SITE_INDUSTRY_POLLUTER:
         strcat(story,", a factory whose Conservative smokestacks choke the city with deadly pollutants.  ");break;
      case SITE_INDUSTRY_NUCLEAR:
         strcat(story,", also known to be a Conservative storage facility for radioactive waste.  ");break;
      case SITE_CORPORATE_HEADQUARTERS:
         strcat(story,", where evil and Conservatism coagulate in the hallways.  ");break;
      case SITE_CORPORATE_HOUSE:
         strcat(story,", a building with enough square footage enough to house a hundred people if it weren't in Conservative Hands.  ");break;
      case SITE_MEDIA_AMRADIO:
      case SITE_MEDIA_CABLENEWS:
         strcat(story,", known for its Extreme Conservative Bias.  ");break;
      case SITE_BUSINESS_BANK:
         strcat(story,", the very symbol of economic inequality.  ");break;
      default:
         strcat(story,".  ");break;
      }
   }
   else if(!ccs)
      strcat(story,".  ");
}

void squadstory_text_opening(newsstoryst& ns,bool liberalguardian,bool ccs,char* story)
{
   if(ns.type==NEWSSTORY_SQUAD_SITE)
   {
      if(!newscherrybusted&&!liberalguardian)
      {
         if(ns.positive)
         {
            strcat(story,"A group calling itself the Liberal Crime Squad ");
            strcat(story,"burst onto the scene of political activism yesterday, according ");
            strcat(story,"to a spokesperson from the police department.");
            strcat(story,"&r");
         }
         else
         {
            strcat(story,"A group of thugs calling itself the Liberal Crime Squad ");
            strcat(story,"went on a rampage yesterday, according ");
            strcat(story,"to a spokesperson from the police department.");
         }
      }
      else
      {
         if(ns.positive)
         {
            strcat(story,"The Liberal Crime Squad has struck again.  ");
            strcat(story,"&r");
         }
         else
         {
            if(!liberalguardian)
               strcat(story,"The Liberal Crime Squad has gone on a rampage.  ");
            else
               strcat(story,"A Liberal Crime Squad operation went horribly wrong.  ");
            strcat(story,"&r");
         }
      }
   }
   else if(ns.type==NEWSSTORY_CCS_SITE)
   {
      if(newscherrybusted<2)
      {
         if(ns.positive&&!liberalguardian)
         {
            strcat(story,"A group of M16-wielding vigilantes calling itself the Conservative Crime Squad ");
            strcat(story,"burst onto the scene of political activism yesterday, according ");
            strcat(story,"to a spokesperson from the police department.");
            strcat(story,"&r");
         }
         else
         {
            strcat(story,"A group of worthless M16-toting hicks calling itself the Conservative Crime Squad ");
            strcat(story,"went on a rampage yesterday, according ");
            strcat(story,"to a spokesperson from the police department.");
         }
      }
      else
      {
         if(ns.positive&&!liberalguardian)
         {
            strcat(story,"The Conservative Crime Squad has struck again.  ");
            strcat(story,"&r");
         }
         else
         {
            strcat(story,"The Conservative Crime Squad has gone on another rampage.  ");
            strcat(story,"&r");
         }
      }
   }
   else if(ns.type==NEWSSTORY_CCS_KILLED_SITE)
   {
      if(newscherrybusted<2)
      {
         if(ns.positive&&!liberalguardian)
         {
            strcat(story,"A group of M16-wielding vigilantes calling themselves the Conservative Crime Squad ");
            strcat(story,"burst briefly onto the scene of political activism yesterday, according ");
            strcat(story,"to a spokesperson from the police department.  ");
            strcat(story,"&r");
         }
         else
         {
            strcat(story,"A group of ");
            switch(LCSrandom(4))
            {
            case 0:strcat(story,"pathetic, ");break;
            case 1:strcat(story,"worthless, ");break;
            case 2:strcat(story,"disheveled, ");break;
            case 3:strcat(story,"inbred, ");break;
            }
            switch(LCSrandom(4))
            {
            case 0:strcat(story,"violent, ");break;
            case 1:strcat(story,"bloodthirsty, ");break;
            case 2:strcat(story,"savage, ");break;
            case 3:strcat(story,"");break;
            }
            strcat(story,"M16-toting ");
            switch(LCSrandom(3))
            {
            case 0:strcat(story,"hicks ");break;
            case 1:strcat(story,"rednecks ");break;
            case 2:strcat(story,"losers ");break;
            }
            strcat(story,"calling themselves the Conservative Crime Squad went on a ");
            switch(LCSrandom(3))
            {
            case 0:strcat(story,"suicidal ");break;
            case 1:strcat(story,"homicidal ");break;
            case 2:strcat(story,"bloodthirsty ");break;
            }
            strcat(story,"rampage yesterday, according to a spokesperson from the police department.  ");
            strcat(story,"&r");
         }
      }
      else
      {
         if(ns.positive&&!liberalguardian)
         {
            strcat(story,"The Conservative Crime Squad has struck again, albeit with a tragic end.  ");
            strcat(story,"&r");
         }
         else
         {
            strcat(story,"The Conservative Crime Squad has gone on another rampage, and they got what they deserved.  ");
            strcat(story,"&r");
         }
      }
   }
   else
   {
      if(!newscherrybusted&&!liberalguardian)
      {
         if(ns.positive)
         {
            strcat(story,"A group calling itself the Liberal Crime Squad ");
            strcat(story,"burst briefly onto the scene of political activism yesterday, according ");
            strcat(story,"to a spokesperson from the police department.  ");
            strcat(story,"&r");
         }
         else
         {
            strcat(story,"A group of thugs calling itself the Liberal Crime Squad ");
            strcat(story,"went on a suicidal rampage yesterday, according ");
            strcat(story,"to a spokesperson from the police department.  ");
            strcat(story,"&r");
         }
      }
      else
      {
         if(ns.positive)
         {
            strcat(story,"The Liberal Crime Squad has struck again, albeit with a tragic end.  ");
            strcat(story,"&r");
         }
         else
         {
            if(!liberalguardian)
               strcat(story,"The Liberal Crime Squad has gone on a rampage, and they got what they deserved.  ");
            else
               strcat(story,"A Liberal Crime Squad operation went horribly wrong, and came to a tragic end.  ");//XXX: Rewrite me -- LK
            strcat(story,"&r");
         }
      }
   }

   squadstory_text_location(ns,liberalguardian,ccs,story);

   if(ns.type==NEWSSTORY_SQUAD_KILLED_SITE)
   {
      if(liberalguardian)strcat(story,"Unfortunately, the LCS group was defeated by the forces of evil.  ");
      else if(ns.positive)strcat(story,"Everyone in the LCS group was arrested or killed.  ");
      else strcat(story,"Fortunately, the LCS thugs were stopped by brave citizens.  ");
   }
   if(ns.type==NEWSSTORY_CCS_KILLED_SITE)
   {
      if(ns.positive&&!liberalguardian)strcat(story,"Everyone in the CCS group was arrested or killed.  ");
      else strcat(story,"Fortunately, the CCS thugs were stopped by brave citizens.  ");
   }
   strcat(story,"&r");
}
