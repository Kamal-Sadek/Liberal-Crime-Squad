#include <includes.h>
#include <externs.h>

enum Pages
{
   PAGE_LEADERS,
   PAGE_ISSUES_A,
   PAGE_ISSUES_B,
   PAGE_POLLS_A,
   PAGE_POLLS_B
};

/* base - liberal agenda */
char liberalagenda(char won)
{
   int page=0, y;

   while(1)
   {
      erase();
      if(won==1)
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(0,0);
         addstr("The Triumph of the Liberal Agenda");
      }
      else if(won==-1)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         move(0,0);
         addstr("The Abject Failure of the Liberal Agenda");
      }
      else
      {
         set_color(COLOR_WHITE,COLOR_BLACK,1);
         move(0,0);
         addstr("The Status of the Liberal Agenda");
      }

      if(page<0)page+=3;
      if(page>=3)page-=3;

      switch(page)
      {
      case PAGE_LEADERS:
      {

         move(1,0);
         addstr(" _________________ __________ __________ ");
         move(2,0);
         addstr("/ GENERAL SUMMARY \\ ISSUES A \\ ISSUES B \\");
         move(3,0);
         addstr("                   -------------------------------------------------------------");

         if(exec[EXEC_PRESIDENT]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(exec[EXEC_PRESIDENT]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(exec[EXEC_PRESIDENT]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(exec[EXEC_PRESIDENT]==1)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(5,0);
         if(won!=-1&&won!=-2)
         {
            addstr("President ");
            if(execterm==1)addstr("(1st Term):");
            else addstr("(2nd Term):");
         }
         else if(won==-1)
		 {
			 addstr("King: ");
		 }
		 else if(won==-2)
		 {
			 addstr("President For Life ");
		 }
         move(5,25);
         addstr(execname[EXEC_PRESIDENT]);

         if(exec[EXEC_VP]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(exec[EXEC_VP]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(exec[EXEC_VP]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(exec[EXEC_VP]==1)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(6,0);
         addstr("Vice President: ");
         move(6,25);
         addstr(execname[EXEC_VP]);

         if(exec[EXEC_STATE]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(exec[EXEC_STATE]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(exec[EXEC_STATE]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(exec[EXEC_STATE]==1)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(7,0);
         if(won!=-2)
         {
            addstr("Secretary of State: ");
         }
         else
         {
            addstr("General Secretary: ");
         }
         move(7,25);
         addstr(execname[EXEC_STATE]);

         if(exec[EXEC_ATTORNEY]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(exec[EXEC_ATTORNEY]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(exec[EXEC_ATTORNEY]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(exec[EXEC_ATTORNEY]==1)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(8,0);
         addstr("Attorney General: ");
         move(8,25);
         addstr(execname[EXEC_ATTORNEY]);

         int housemake[5]={0,0,0,0,0};
         for(int h=0;h<435;h++)
         {
            housemake[house[h]+2]++;
         }
         int lsum=housemake[3]+housemake[4]
            -housemake[0]-housemake[1];
         if(housemake[0]+housemake[1]/2>=290)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(lsum<-90)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(lsum<90)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(housemake[4]+housemake[3]/2<290)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         char num[20];
         if(won!=-1&&won!=2)
         {
            move(10,0);
            addstr("House: ");
            itoa(housemake[4],num,10);
            addstr(num);addstr("Lib+, ");
            itoa(housemake[3],num,10);
            addstr(num);addstr("Lib, ");
            itoa(housemake[2],num,10);
            addstr(num);addstr("Mod, ");
            itoa(housemake[1],num,10);
            addstr(num);addstr("Cons, ");
            itoa(housemake[0],num,10);
            addstr(num);addstr("Cons+");
         }

         int senatemake[5]={0,0,0,0,0};
         for(int s=0;s<100;s++)
         {
            senatemake[senate[s]+2]++;
         }
         lsum=senatemake[3]+senatemake[4]
            -senatemake[0]-senatemake[1];
         if(senatemake[0]+senatemake[1]/2>=67)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(lsum<-33)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         else if(lsum<33)set_color(COLOR_YELLOW,COLOR_BLACK,1);
         else if(senatemake[4]+senatemake[3]/2<66)set_color(COLOR_CYAN,COLOR_BLACK,1);
         else set_color(COLOR_GREEN,COLOR_BLACK,1);
         if(won!=-1&&won!=-2)
         {
            move(11,0);
            addstr("Senate: ");
            itoa(senatemake[4],num,10);
            addstr(num);addstr("Lib+, ");
            itoa(senatemake[3],num,10);
            addstr(num);addstr("Lib, ");
            itoa(senatemake[2],num,10);
            addstr(num);addstr("Mod, ");
            itoa(senatemake[1],num,10);
            addstr(num);addstr("Cons, ");
            itoa(senatemake[0],num,10);
            addstr(num);addstr("Cons+");
         }
         else if(won==-1)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(10,0);
            addstr("The Congress consists of CEOs and televangelists.");
         }
		 else if(won==-2)
		 {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(10,0);
            addstr("The People's Congress is composed of Stalinist Party lackeys.");
		 }
         int elibjudge=0;
         for(int c=0;c<9;c++)
         {
            if(court[c]==2)elibjudge++;
         }

         if(won==-1)set_color(COLOR_RED,COLOR_BLACK,1);
		   else if(won==-2)set_color(COLOR_RED,COLOR_BLACK,1);
         else if(won==1||elibjudge>=5)set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,1);

         move(4,56);addch('S');
         move(5,56);addch('U');
         move(6,56);addch('P');
         move(7,56);addch('R');
         move(8,56);addch('E');
         move(9,56);addch('M');
         move(10,56);addch('E');

         move(4,58);addch('C');
         move(5,58);addch('O');
         move(6,58);addch('U');
         move(7,58);addch('R');
         move(8,58);addch('T');

         if(won!=-1&&won!=-2)
         {
            y=4;

            for(int c=0;c<9;c++)
            {
               if(court[c]==-2)set_color(COLOR_RED,COLOR_BLACK,1);
               else if(court[c]==-1)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               else if(court[c]==0)set_color(COLOR_YELLOW,COLOR_BLACK,1);
               else if(court[c]==1)set_color(COLOR_CYAN,COLOR_BLACK,1);
               else set_color(COLOR_GREEN,COLOR_BLACK,1);

               move(y,60);
               addstr(courtname[c]);

               y++;
            }
         }
         else if(won==-1)
         {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(5,60);
            addstr("   Replaced");
            move(6,60);
            addstr(" By Corporate");
            move(7,60);
            addstr("Ethics Officers");
         }
		 else if(won==-2)
		 {
            set_color(COLOR_RED,COLOR_BLACK,1);
            move(5,60);
            addstr("   Controlled by");
            move(6,60);
            addstr(" the People's");
            move(7,60);
            addstr("Congress");
		 }
         for(int l=0;l<LAWNUM;l++)
         {
            move(14+l/3,l%3*26);

            set_color(COLOR_BLACK,COLOR_BLACK,1);

            addstr("<-----> ");

            if(law[l]==ALIGN_ARCHCONSERVATIVE)set_color(COLOR_RED,COLOR_BLACK,1);
            else if(law[l]==ALIGN_CONSERVATIVE)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            else if(law[l]==ALIGN_MODERATE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
            else if(law[l]==ALIGN_LIBERAL)set_color(COLOR_CYAN,COLOR_BLACK,1);
            else set_color(COLOR_GREEN,COLOR_BLACK,1);

            switch(l)
            {
               case LAW_WOMEN:addstr("Women's Rights");break;
               case LAW_CIVILRIGHTS:addstr("Civil Rights");break;
               case LAW_DRUGS:addstr("Drug Law");break;
               case LAW_IMMIGRATION:addstr("Immigration");break;
               case LAW_RELIGION:addstr("Religion");break;
               case LAW_ELECTIONS:addstr("Election Reform");break;
               case LAW_MILITARY:addstr("Military Spending");break;
               case LAW_TORTURE:addstr("Human Rights");break;
               case LAW_TAX:
                  addstr("Tax Structure");
		            break;
               case LAW_ABORTION:
                  addstr("Abortion Rights");
                  break;
               case LAW_ANIMALRESEARCH:
                  addstr("Animal Rights");
                  break;
               case LAW_POLICEBEHAVIOR:
                  addstr("Police Regulation");
                  break;
               case LAW_PRIVACY:
                  addstr("Privacy Rights");
                  break;
               case LAW_DEATHPENALTY:
                  addstr("Death Penalty");
                  break;
               case LAW_NUCLEARPOWER:
                  addstr("Nuclear Power");
                  break;
               case LAW_POLLUTION:
                  addstr("Pollution");
                  break;
               case LAW_LABOR:
                  addstr("Labor Laws");
                  break;
               case LAW_GAY:
                  addstr("Gay Rights");
                  break;
               case LAW_CORPORATE:
                  addstr("Corporate Law");
                  break;
               case LAW_FREESPEECH:
                  addstr("Free Speech");
                  break;
               case LAW_FLAGBURNING:
                  addstr("Flag Burning");
                  break;
               case LAW_GUNCONTROL:
                  addstr("Gun Control");
                  break;
            }

            move(14+l/3,l%3*26 + 3 + law[l]);

            addch('O');
         }
         break;
      }

      case PAGE_ISSUES_A:
      case PAGE_ISSUES_B:
      {
         move(1,0);
         addstr(" _________________ __________ __________ ");
         move(2,0);
         if(page==PAGE_ISSUES_A)
         {
            addstr("/ GENERAL SUMMARY / ISSUES A \\ ISSUES B \\");
            move(3,0);
            addstr("------------------            --------------------------------------------------");
         }
         else
         {
            addstr("/ GENERAL SUMMARY / ISSUES A / ISSUES B \\");
            move(3,0);
            addstr("-----------------------------            ---------------------------------------");
         }

         int y=4;
         int startinglaw=0;
         if(page==PAGE_ISSUES_B)startinglaw=18;
         for(int l=startinglaw;l<startinglaw+18 && l<LAWNUM;l++)
         {
            if(won==-1)set_color(COLOR_RED,COLOR_BLACK,1);
			   else if(won==-2)set_color(COLOR_RED,COLOR_BLACK,1);
            else
            {
               if(law[l]==ALIGN_ARCHCONSERVATIVE)set_color(COLOR_RED,COLOR_BLACK,1);
               else if(law[l]==ALIGN_CONSERVATIVE)set_color(COLOR_MAGENTA,COLOR_BLACK,1);
               else if(law[l]==ALIGN_MODERATE)set_color(COLOR_YELLOW,COLOR_BLACK,1);
               else if(law[l]==ALIGN_LIBERAL)set_color(COLOR_CYAN,COLOR_BLACK,1);
               else set_color(COLOR_GREEN,COLOR_BLACK,1);
            }

            move(y,0);

            switch(l)
            {
               case LAW_WOMEN:
                  if(won==-1)addstr("Women have been stripped of the right to vote.");
				      else if(won==-2)addstr("Women are usually drafted into the armed forces to fight in place of men.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Women are regularly treated as second-class citizens.");
				      else if(law[l]==-1)addstr("Non-discrimination laws do not apply to gender.");
				      else if(law[l]==0)addstr("Women are nominally equal under law.");
				      else if(law[l]==1)addstr("Women have substantial recourse against discrimination.");
				      else addstr("Gender equality is universally respected.");
				      break;
               case LAW_CIVILRIGHTS:
                  if(won==-1)addstr("Segregation has been reintroduced.");
				      else if(won==-2)addstr("Entire ethnic groups are branded 'enemies of the state'.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Racial discrimination is prohibited in name only.");
				      else if(law[l]==-1)addstr("Civil rights laws are inconsistently enforced.");
				      else if(law[l]==0)addstr("Pervasive racial inequality exists.");
				      else if(law[l]==1)addstr("Affirmative action is in place to counteract discrimination.");
				      else addstr("Racial equality is guaranteed and vigorously enforced.");
                  break;
			   case LAW_HUMANRIGHTS:
                  if(won==-1)addstr("The CSA is the founding member of the Alliance of Conservative Dictatorships.");
				      else if(won==-2)addstr("The USA is the founding member of the Alliance of Socialist Dictatorships.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("The United States actively assist human right violators in order to further American Interests.");
				      else if(law[l]==-1)addstr("The government inconsistently promote human rights.");
				      else if(law[l]==0)addstr("The government's human rights record is fairly weak.");
				      else if(law[l]==1)addstr("The government has a strong reputation on human rights issues.");
				      else addstr("The country is respected as an international leader on human rights.");
                  break;
               case LAW_DRUGS:
                  if(won==-1)addstr("Talking about recreational drugs is punishable by death.");
				      else if(won==-2)addstr("Vodka is the only legal recreational drug in the USA.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Violent criminals are released to make room for drug offenders.");
				      else if(law[l]==-1)addstr("Prisons are filled with the targets of a war on drugs.");
				      else if(law[l]==0)addstr("Recreational drugs are prohibited unless medically prescribed.");
				      else if(law[l]==1)addstr("Marijuana is regulated and taxed.");
				      else addstr("Recreational drugs are regulated and taxed.");
                  break;
               case LAW_IMMIGRATION:
                  if(won==-1)addstr("Border guards shoot suspected foreigners on sight.");
				      else if(won==-2)addstr("All Americans must carry around an internal passport, or be shot on sight.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Immigration is illegal, and noncitizens are shipped to Mexico at gunpoint.");
				      else if(law[l]==-1)addstr("The military has been deployed to the borders to slow immigration.");
				      else if(law[l]==0)addstr("Great expense is taken to slow immigration, without success.");
				      else if(law[l]==1)addstr("The government works to accommodate potential immigrants.");
				      else addstr("Immigration is unregulated.");
                  break;
               case LAW_RELIGION:
                  if(won==-1)addstr("The world derisively refers to the country as \"Jesusland\".");
				      else if(won==-2)addstr("Religion is illegal.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Encouraging religious faith is a major goal of public policy.");
				      else if(law[l]==-1)addstr("Public schools include organized prayer and study of creation beliefs.");
				      else if(law[l]==0)addstr("Religious charities and private schools may compete for public funding.");
				      else if(law[l]==1)addstr("Government is prohibited from supporting religious organizations.");
				      else addstr("Purely faith-based viewpoints are irrelevant to public policy.");
                  break;
               case LAW_ELECTIONS:
                  if(won==-1)addstr("Only independently wealthy candidates have a chance of defeating incumbents.");
				      else if(won==-2)addstr("Only Stalinist Party members may run in elections.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Virtually no ethics restrictions exist on political officeholders.");
				      else if(law[l]==-1)addstr("Elections are mostly unregulated, but basic ethics restrictions are in place.");
				      else if(law[l]==0)addstr("Moderate campaign finance reform is implemented.");
				      else if(law[l]==1)addstr("Election financing is transparent and well-regulated.");
				      else addstr("Election expenses are publicly funded, and voting is by ranked list.");
				      break;
               case LAW_MILITARY:
                  if(won==-1)addstr("The massive military is a tool of political oppression.");
				      else if(won==-2)addstr("The military promote Stalinism throughout the world by using force.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Ballooning military boondoggles are utterly out of control.");
				      else if(law[l]==-1)addstr("Massive investment is put into the military.");
				      else if(law[l]==0)addstr("Military spending is growing each year.");
				      else if(law[l]==1)addstr("Military strength is not a major priority.");
				      else addstr("The military has been weakened significantly.");
				      break;
			   case LAW_WELFARE:
				   if(won==-1)addstr("Corporate workhouses help the degenerate poor pull themselves up by their bootstraps.");
				   else if(won==-2)addstr("People who receive welfare must work for the Stalinist Party.");
				   else if(law[l]==ALIGN_ARCHCONSERVATIVE)
					   addstr("Welfare is provided purely by private charities.")
				   else if(law[l]==-1)addstr("A minimal welfare system provides temporary benefits and rigorously searches for fraud.");
				   else if(law[l]==0)addstr("The welfare system provides benefits in some cases.");
				   else if(law[l]==1)addstr("There is a effective societal safety net for those who can't work.");
				   else addstr("The welfare system shows that there is such a thing as a free lunch.)";
				   break;
               case LAW_TORTURE:
                  if(won==-1)addstr("Torture is a prescribed practice in police interrogations.");
				      else if(won==-2)addstr("The Cheka constanly invents new methods of torture.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Military and intelligence interrogators regularly engage in torture.");
				      else if(law[l]==-1)addstr("The line between standard interrogation and torture is severely blurred.");
				      else if(law[l]==0)addstr("Torture allegations still occasionally crop up.");
				      else if(law[l]==1)addstr("The government strongly enforces a ban on torture.");
				      else addstr("The country is respected as an international leader on Moral Interrogation Practices.");
				      break;
               case LAW_TAX:
				      if(won==-1)addstr("There are no taxes, yet most people have no money.");
					  else if(won==-2)addstr("Having any money whatsoever is considered a criminal offense.");
				      else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("The tax code is a nightmare designed to maintain class structure.");
				      else if(law[l]==-1)addstr("A flat tax is in effect.");
				      else if(law[l]==0)addstr("Taxes are moderate, and the code has loop-holes.");
				      else if(law[l]==1)addstr("Taxes are very high and steeply graded.");
				      else addstr("Rich people are virtually unheard of, due to taxation.");
				      break;
               case LAW_ABORTION:
                  if(won==-1)addstr("Use of contraception and abortion are capital offenses.");
				      else if(won==-2)addstr("Use of contraception and abortion are capital offenses.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Abortion is a felony equal to murder.");
                  else if(law[l]==-1)addstr("Abortion is prohibited except in extreme circumstances.");
                  else if(law[l]==0)addstr("Abortion is limited to early pregnancy.");
                  else if(law[l]==1)addstr("Abortion is mostly legal, but discouraged by law.");
                  else addstr("Abortion is legal.");
                  break;
               case LAW_ANIMALRESEARCH:
                  if(won==-1)addstr("All forms of human experimentation on the poor are encouraged.");
				      else if(won==-2)addstr("All forms of human experimentation on 'class enemies' are encouraged.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Animals are things of inferior existence.");
                  else if(law[l]==-1)addstr("Animal research is lightly regulated.");
                  else if(law[l]==0)addstr("Animal research is moderately regulated.");
                  else if(law[l]==1)addstr("Animal research is stiffly regulated.");
                  else addstr("Animals are people too.");
                  break;
               case LAW_POLICEBEHAVIOR:
                  if(won==-1)addstr("Policing is administered by corporations and has a draft.");
				      else if(won==-2)addstr("The police is administered by the Stalinist Party and has a draft.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Law enforcement is given free reign.");
                  else if(law[l]==-1)addstr("Law enforcement is lightly regulated.");
                  else if(law[l]==0)addstr("Law enforcement is moderately regulated.");
                  else if(law[l]==1)addstr("Law enforcement is strictly controlled.");
                  else addstr("All law enforcement positions are subject to election and recall.");
                  break;
               case LAW_PRIVACY:
                  if(won==-1)addstr("Files on each citizen are easily accessible to corporations.");
				      else if(won==-2)addstr("Files on each citizen are easily accessible to the Stalinist Party.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Any corporation requesting private information is granted access.");
                  else if(law[l]==-1)addstr("Privacy laws are weak.");
                  else if(law[l]==0)addstr("Privacy laws are moderate.");
                  else if(law[l]==1)addstr("Privacy laws are strong.");
                  else addstr("Individual privacy is sacred.");
                  break;
               case LAW_DEATHPENALTY:
                  if(won==-1)addstr("Poor criminals receive mandatory death sentences.");
				      else if(won==-2)addstr("Class enemies receive mandatory death sentences.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("People can be put to death for minor offenses.");
                  else if(law[l]==-1)addstr("The death penalty is actively enforced in many states.");
                  else if(law[l]==0)addstr("The death penalty is in effect but under scrutiny.");
                  else if(law[l]==1)addstr("The death penalty is not permitted in many circumstances.");
                  else addstr("The death penalty is considered barbaric and never practiced.");
                  break;
               case LAW_NUCLEARPOWER:
                  if(won==-1)addstr("Nuclear power plants are ubiquitous.");
				      else if(won==-2)addstr("Nuclear power plants are ubiquitous.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Nuclear power is proliferating with no controls.");
                  else if(law[l]==-1)addstr("Nuclear power is a preferred energy source.");
                  else if(law[l]==0)addstr("Nuclear power is often an energy source.");
                  else if(law[l]==1)addstr("Nuclear power is intensely regulated and seldom used.");
                  else addstr("Nuclear power is illegal.");
                  break;
               case LAW_POLLUTION:
                  if(won==-1)addstr("Deformed children are the norm in industrial zones.");
				      else if(won==-2)addstr("Industry is subject to zero-tolerance pollution regulations.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Industry may pollute as much as they like.");
                  else if(law[l]==-1)addstr("Industry voluntarily regulates pollution.");
                  else if(law[l]==0)addstr("Industry is subject to moderate pollution regulations.");
                  else if(law[l]==1)addstr("Industry is subject to strict pollution regulations.");
                  else addstr("Industry is subject to zero-tolerance pollution regulations.");
                  break;
               case LAW_LABOR:
                  if(won==-1)addstr("People are bred in pens to be farmed out to corporations like beasts.");
				      else if(won==-2)addstr("The state has nationalized all industries.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("There is no weekend and children are forced to work.");
                  else if(law[l]==-1)addstr("Working conditions are miserable and the minimum wage is low.");
                  else if(law[l]==0)addstr("Workers still require some benefits.");
                  else if(law[l]==1)addstr("Workers are fairly compensated and have benefits.");
                  else addstr("There are universal workers' rights and high wages.");
                  break;
               case LAW_GAY:
                  if(won==-1)addstr("Homosexuals are executed regularly.");
				      else if(won==-2)addstr("Homosexuals are executed regularly.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Homosexuals are routinely persecuted with no recourse.");
                  else if(law[l]==-1)addstr("Homosexuals are not tolerated.");
                  else if(law[l]==0)addstr("Homosexuals are grudgingly tolerated but have few equal rights.");
                  else if(law[l]==1)addstr("Homosexuals have many rights shared by heterosexuals.");
                  else addstr("Homosexuals have equal rights.");
                  break;
               case LAW_CORPORATE:
                  if(won==-1)addstr("Corporations under the King run the country in a feudal system.");
				      else if(won==-2)addstr("Belonging to a corporation is a criminal offense.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Corporations essentially run the country in a feudal system.");
                  else if(law[l]==-1)addstr("Corporate culture is corrupt and there is a great disparity in wages.");
                  else if(law[l]==0)addstr("Corporations are moderately regulated, although wages are still unfair.");
                  else if(law[l]==1)addstr("Corporations are stiffly regulated, and executives are fairly compensated.");
                  else addstr("Corporations are subject to intense regulation, and there is a maximum wage law.");
                  break;
               case LAW_FREESPEECH:
                  if(won==-1)addstr("Unacceptable speech is a capital crime.");
				      else if(won==-2)addstr("Unacceptable speech is a capital crime.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Armored squads are tasked with suppressing unacceptable speech.");
                  else if(law[l]==-1)addstr("Some individuals are harassed because of their speech.");
                  else if(law[l]==0)addstr("Free speech is tolerated.");
                  else if(law[l]==1)addstr("Free speech is encouraged.");
                  else addstr("Free speech is universally supported.");
                  break;
               case LAW_FLAGBURNING:
                  if(won==-1)addstr("Images or words describing flag burning are punished by death.");
				      else if(won==-2)addstr("Flags of the old American regime is burnt primarily as as fuel.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Burning the flag is a crime on par with murder.");
                  else if(law[l]==-1)addstr("Burning the flag is a felony.");
                  else if(law[l]==0)addstr("Flag-burning is a misdemeanor.");
                  else if(law[l]==1)addstr("Flag-burning is legal but stigmatized.");
                  else addstr("Flag-burning is legal.");
                  break;
               case LAW_GUNCONTROL:
                  if(won==-1)addstr("Gangs of young children carrying AK-47s roam the streets.");
				      else if(won==-2)addstr("Anyone owning a gun is executed by firing squad.");
                  else if(law[l]==ALIGN_ARCHCONSERVATIVE)
                     addstr("Machine guns can be bought and sold freely.");
                  else if(law[l]==-1)addstr("Military weapons are banned, but similar-looking guns are available.");
                  else if(law[l]==0)addstr("A comprehensive ban on military-style weapons is in effect.");
                  else if(law[l]==1)addstr("Most guns cannot be sold to anyone outside of law enforcement.");
                  else addstr("It is illegal to buy, sell, or carry a gun in public.");
                  break;
            }

            y++;
         }
         break;
      }
      }

      if(won==1)
      {
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         move(23,0);
         addstr("The country has achieved Elite Liberal status.");
         move(24,0);
         addstr("Press 'L' to view the high score list.");

         do
         {
            refresh();
            int c=getch();
            translategetch(c);

            if(c=='l')break;
         }while(1);
      }
      else if(won==-1)
      {
         set_color(COLOR_RED,COLOR_BLACK,1);
         move(23,0);
         addstr("The country has been Reaganified.");
         move(24,0);
         addstr("Press 'L' to view the high score list.");

         do
         {
            refresh();
            int c=getch();
            translategetch(c);

            if(c=='l')break;
         }while(1);
      }
	  else if(won==-2)
	  {

         set_color(COLOR_RED,COLOR_BLACK,1);
         move(23,0);
         addstr("The country has been Stalinified.");
         move(24,0);
         addstr("Press 'L' to view the high score list.");

         do
         {
            refresh();
            int c=getch();
            translategetch(c);

            if(c=='l')break;
         }while(1);
	  }
      else
      {
         move(23,0);
         set_color(COLOR_RED,COLOR_BLACK,1);
         addstr("Arch-Conservative  ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("-  ");
         set_color(COLOR_MAGENTA,COLOR_BLACK,1);
         addstr("Conservative  ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("-  ");
         set_color(COLOR_YELLOW,COLOR_BLACK,1);
         addstr("moderate  ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("-  ");
         set_color(COLOR_CYAN,COLOR_BLACK,1);
         addstr("Liberal  ");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         addstr("-  ");
         set_color(COLOR_GREEN,COLOR_BLACK,1);
         addstr("Elite Liberal");
         set_color(COLOR_WHITE,COLOR_BLACK,0);
         //move(23,0);
         //addstr("Once these are Green, the country will have achieved Elite Liberal status.");
         move(24,0);
         addstr("Press D to disband and wait.  Press any other key to consider the situation.");

         refresh();
         int c=getch();
         translategetch(c);

         if(c==KEY_RIGHT || c==KEY_DOWN)
         {
            page++;
            continue;
         }

         if(c==KEY_LEFT || c==KEY_UP)
         {
            page--;
            continue;
         }

         if(c=='d')
         {
            return confirmdisband();
         }

         break;
      }
   }

   return 0;
}



/* base - liberal agenda - disband */
char confirmdisband(void) // XXX The (current) issue that the masses are most
{			  // XXX	concerned should be more likely to be
   char word[80];	  // XXX	the phrase. (Issue, not the CCS, etc.)
   int pos=0;		  // XXX			-- LK

   switch(LCSrandom(22)) // or more... (preferably 44)
   {    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Liberal Phrase                                       // Conservative Equivalent      // Stalinist Equivalent  //
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      case 0:strcpy(word,"Corporate Accountability");break;     // Corporate Trust              //                       //
      case 1:strcpy(word,"Free Speech");break;                  // Protect The Children         // Thoughtcrime          //
      case 2:strcpy(word,"Gay Marriage");break;                 // Queer Fear                   //                       //
      case 3:strcpy(word,"Abortion Rights");break;              // Sacred Life                  //                       //
      case 4:strcpy(word,"Separation Clause");break;            // Under God                    //                       //
      case 5:strcpy(word,"Racial Equality");break;              // Racial Profiling             //                       //
      case 6:strcpy(word,"Gun Control");break;                  // Second Amendment             // Firing Squad          //
      case 7:strcpy(word,"Campaign Finance Reform");break;      // Lobbyists                    //                       //
      case 8:strcpy(word,"Animal Rights");break;                // Animal Testing FIXME         //                       //
      case 9:strcpy(word,"Worker's Rights");break;              // Small Business FIXME         //                       //
      case 10:strcpy(word,"Police Responsibility");break;       // Police Brutality             //                       //
      case 11:strcpy(word,"Global Warming");break;              // Environmental Wasteland      //                       //
      case 12:strcpy(word,"Immigration Reform");break;          // Border Control FIXME         //                       //
      case 13:strcpy(word,"Human Rights");break;                // Enemy Combatant FIXME        //                       //
      case 14:strcpy(word,"Woman's Suffrage");break;            // Traditional Gender Roles     //                       //
      case 15:strcpy(word,"Right To Privacy");break;            // Wiretapping                  // Big Brother           //
      case 16:strcpy(word,"Medical Marijuana");break;           // Drug Trafficking             //                       //
      case 17:strcpy(word,"Flag Burning");break;                // Stars And Stripes            //                       //
      case 18:strcpy(word,"Life Imprisonment");break;           // Lethal Injection             //                       //
      case 19:strcpy(word,"Conflict Resolution");break;         // Preemptive Strike            //                       //
      case 20:strcpy(word,"Radiation Poisoning");break;         // Nuclear Power                //                       //
      case 21:strcpy(word,"Tax Bracket");break;                 // Poverty                      // Proletariat           //
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   }

   do
   {
      erase();

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(0,0);
      addstr("Are you sure you want to disband?");

      set_color(COLOR_WHITE,COLOR_BLACK,0);
      move(2,0);  addstr("Disbanding scatters the Liberal Crime Squad, sending all of its members");
      move(3,0);  addstr("into hiding, free to pursue their own lives.  You will be able to observe");
      move(4,0);  addstr("the political situation in brief, and wait until a resolution is reached.");

      move(6,0);  addstr("If at any time you determine that the Liberal Crime Squad will be needed");
      move(7,0);  addstr("again, you may return to the homeless shelter to restart the campaign.");

      move(9,0);  addstr("Do not make this decision lightly.  If you do need to return to action,");
      move(10,0); addstr("only the most devoted of your former members will return.");

      set_color(COLOR_WHITE,COLOR_BLACK,1);
      move(13,0); addstr("Type this Liberal phrase to confirm (press a wrong letter to rethink it):");

      for(int x=0;x<strlen(word);x++)
      {
         move(15,x);
         if(x==pos)set_color(COLOR_GREEN,COLOR_BLACK,0);
         else if(x<pos)set_color(COLOR_GREEN,COLOR_BLACK,1);
         else set_color(COLOR_WHITE,COLOR_BLACK,0);
         addch(word[x]);
      }

      int c=getch();
      translategetch(c);

      if((c==word[pos])||((c+'A'-'a')==word[pos]))
      {
         pos++;
         if(word[pos]==' ')pos++;
         if(int(pos)>=strlen(word))
         {
            //SET UP THE DISBAND
            for(int p=pool.size()-1;p>=0;p--)
            {
               if(!pool[p]->alive)
               {
                  delete pool[p];
                  pool.erase(pool.begin() + p);
               }
               if(!(pool[p]->flag & CREATUREFLAG_SLEEPER))
               {
                  removesquadinfo(*pool[p]);
                  pool[p]->hiding=-1;
               }
            }
            cleangonesquads();

            disbandtime=year;

            return 1;
         }
      }
      else break;

   }while(1);

   return 0;
}
