#include <externs.h>

/**
ACTIVITY_VISIT, TODO
ACTIVITY_HOSTAGETENDING, TODO
ACTIVITY_DOS_RACKET, TODO
ACTIVITY_MAKE_ARMOR, TODO
ACTIVITY_STEALCARS, TODO
ACTIVITY_WHEELCHAIR, TODO
ACTIVITY_BURY, TODO
ACTIVITY_WRITE_BLOG, TODO
ACTIVITY_TEACH_GENERALED, TODO
ACTIVITY_TEACH_POLITICS, TODO
ACTIVITY_TEACH_SURVIVAL, TODO
ACTIVITY_TEACH_FIGHTING, TODO
ACTIVITY_TEACH_COVERT, TODO
ACTIVITY_CLINIC, TODO
ACTIVITY_HEAL, TODO
ACTIVITY_SLEEPER_LIBERAL, TODO
ACTIVITY_SLEEPER_CONSERVATIVE, TODO
ACTIVITY_SLEEPER_SPY, TODO
ACTIVITY_SLEEPER_RECRUIT, TODO
ACTIVITY_SLEEPER_SCANDAL, TODO
ACTIVITY_SLEEPER_EMBEZZLE, TODO
ACTIVITY_SLEEPER_STEAL, TODO
ACTIVITY_SLEEPER_JOINLCS, TODO
*/

// Help pages are drawn here.

// NOTE: This will probably need updating if there are any fundamental code
// changes that will change gameplay.
void help(const char* string)
{
   erase();
}

// Generates help page on activities.
// Just supply Activity type.
void HelpActivities(int activityType)
{
   // I have actually created a screen editor to help me create the code
   // for the help. It can probably be used all over the place for formatting
   // large pieces of text.

   if(activityType == ACTIVITY_NONE)
   {
      return;
   }

   erase();
   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(0,0);
   addstr("Liberal help on:");

   switch(activityType)
   {
      // Liberal activism
   case ACTIVITY_COMMUNITYSERVICE:
      move(2,0);
      addstr("===Community Service===");
      move(4,0);
      addstr(" Has a minor effect on public opinion, increases Juice and has the chance");
      move(5,0);
      addstr(" of reducing the activists Heat.");
      break;
   case ACTIVITY_TROUBLE:
      move(2,0);
      addstr("===Liberal Disobedience===");
      move(4,0);
      addstr(" Liberal Disobedience is an illegal activity and a form of Liberal Activism");
      move(5,0);
      addstr(" which has a positive effect on Public Opinion. It has very short prison");
      move(6,0);
      addstr(" sentences if you get convicted. It also increases juice.");
      move(8,0);
      addstr(" Persuasion, Charisma, Agility, Heart, Strength, Intelligence will increase ");
      move(9,0);
      addstr(" the effect you have on Public Opinion.");
      move(11,0);
      addstr(" There is a chance that someone will phone the police. Agility, intelligence,");
      move(12,0);
      addstr(" disguise and street sense will shake them off the scent. There is also a");
      move(13,0);
      addstr(" chance that an angry mob will beat the crap out of you.");
      break;
   case ACTIVITY_GRAFFITI:
      move(2,0);
      addstr("===Graffiti==");
      move(4,0);
      addstr(" Spraying political graffiti is a misdemeanor, carrying with it relatively ");
      move(5,0);
      addstr(" short jail sentences. This has a positive effect on public opinion. If you ");
      move(6,0);
      addstr(" do not have a can of spray paint, your character will spend a day purchasing  ");
      move(7,0);
      addstr(" one.");
      move(9,0);
      addstr(" A high art skill and heart will enhance the effect this has on public opinion.");
      break;
   case ACTIVITY_POLLS:
      move(2,0);
      addstr("===Search Opinion Polls===");
      move(4,0);
      addstr(" Search Opinion Polls is a legal activity that lets you see what people think");
      move(5,0);
      addstr(" about various Issues. Unlike other activities listed as Liberal Activism,  ");
      move(6,0);
      addstr(" Search Opinion Polls does not have any effect on Public Opinion; it just ");
      move(7,0);
      addstr(" shows you what Public Opinion is without changing it.");
      move(9,0);
      addstr(" Skill in computers and intelligence will give better results.");
      break;
   case ACTIVITY_DOS_ATTACKS:
      move(2,0);
      addstr("===Harassing Websites===");
      move(4,0);
      addstr(" Harassing Websites is an illegal activity and a form of Liberal Activism. It ");
      move(5,0);
      addstr(" is a weaker version of Hacking. Resulting in much shorter prison sentences, ");
      move(6,0);
      addstr(" and is much less likely to provoke a raid by the Police or other law  ");
      move(7,0);
      addstr(" enforcement. Harassing websites only requires a Computers skill of 1.");
      move(9,0);
      addstr(" Utilizes only the Computers skill.");
      break;
   case ACTIVITY_HACKING:
      move(2,0);
      addstr("===Hacking===");
      move(4,0);
      addstr(" Hacking is a highly illegal form of Liberal Activism, which has a good effect");
      move(5,0);
      addstr(" on public opinion. With it, comes very long prison sentences.");
      move(7,0);
      addstr(" Generally you need a lot of the Computer skill and Intelligence before you");
      move(8,0);
      addstr(" can make a successful hacking attempt. You will need a minimum computer skill ");
      move(9,0);
      addstr(" of 6 to have an effect. The effect of having more hackers will compound the ");
      move(10,0);
      addstr(" chance of successfully penetrating a network.");
      move(12,0);
      addstr(" If you are successful there is a chance depending on Computer skill and ");
      move(13,0);
      addstr(" Intelligence of evading being criminalized. If you are criminalized, heat on ");
      move(14,0);
      addstr(" the activist increases. Additionally if you have made a successful hack it ");
      move(15,0);
      addstr(" will increase juice.");
      move(17,0);
      addstr(" Occasionally instead of effecting public opinion your hacker may find files");
      move(18,0);
      addstr(" that can be published in a special edition of the Liberal Guardian.");
      break;

   case ACTIVITY_WRITE_LETTERS:
      move(2,0);
      addstr("===Write to Newspapers===");
      move(4,0);
      addstr(" Write to Newspapers is a legal activity and form of Liberal Activism that");
      move(5,0);
      addstr(" boosts the Writing skill and is more effective at influencing Public Opinion");
      move(6,0);
      addstr(" the higher the Writing skill is. A main reason for writing to newspapers is");
      move(7,0);
      addstr(" developing enough writing skill to effectively Write for the Liberal Guardian,");
      move(8,0);
      addstr(" which has a much bigger influence than Write to Newspapers and is still");
      move(9,0);
      addstr(" completely legal.");
      move(11,0);
      addstr(" Unlike writing for the Liberal Guardian, writing to other newspapers does not");
      move(12,0);
      addstr(" require you to own your own newspaper that you set up by deciding to Invest in");
      move(13,0);
      addstr(" a location and spending lots of money on the one-time fee for starting up your");
      move(14,0);
      addstr(" own newspaper, plus you can do it from anywhere, and it boosts your Writing");
      move(15,0);
      addstr(" skill much faster than writing for the Liberal Guardian.");
      move(17,0);
      addstr(" To have the option to write for the Liberal Guardian, you need to own your own ");
      move(18,0);
      addstr(" newspaper that you set up by deciding to Invest in a location, by pressing 'Z' ");
      move(19,0);
      addstr(" on the main screen untill you have an option to invest and spending $3,000 on ");
      move(20,0);
      addstr(" a Printing Press. Additionally a character must be physically located at that ");
      move(21,0);
      addstr(" safehouse.");
      break;
   case ACTIVITY_WRITE_GUARDIAN:
      move(2,0);
      addstr("===Write for the Liberal Guardian ===");
      move(4,0);
      addstr(" Writing for the Liberal Guardian is a legal (or illegal depending on the ");
      move(5,0);
      addstr(" free speech laws) activity and a form of Liberal Activism that boosts the");
      move(6,0);
      addstr(" Writing skill very slowly and is more effective at influencing Public ");
      move(7,0);
      addstr(" Opinion the higher the Writing skill is. Writing for the Liberal Guardian ");
      move(8,0);
      addstr(" has three times the impact on Public Opinion that Write to Newspapers has.");
      move(10,0);
      addstr("Items that prompt a special edition");
      move(11,0);
      addstr(">     Police Records - Found in the Police Station.");
      move(12,0);
      addstr(">     Secret Corporate Files - Found in the Corporate Headquarters, or");
      move(13,0);
      addstr("      obtained through Hacking.");
      move(14,0);
      addstr(">     CEO Photos - Found in the CEO Residence.");
      move(15,0);
      addstr(">     Intel. HQ Data Disk - Found in the Intelligence HQ.");
      move(16,0);
      addstr(">     Cable News Memos - obtained through Hacking.");
      move(17,0);
      addstr(">     Research Papers - obtained through Hacking.");
      move(18,0);
      addstr(">     Judge Corruption Evidence - obtained through Hacking.");
      move(19,0);
      addstr(">     AM Radio Memos - obtained through Hacking.");
      move(20,0);
      addstr(" All of the above items can be delivered to the LCS from sleepers in the ");
      move(21,0);
      addstr(" appropriate location or from hackers.");
      break;
      // Legal fund raising
   case ACTIVITY_DONATIONS:
      move(2,0);
      addstr("===Soliciting Donations===");
      move(4,0);
      addstr(" Income depends on persuasion and how sharply the activist is dressed. ");
      move(5,0);
      addstr(" This activity only generates significant income if the country is ");
      move(6,0);
      addstr(" relatively liberal. ");
      move(8,0);
      addstr(" This activity trains persuasion.");
      break;
   case ACTIVITY_SELL_TSHIRTS:
      move(2,0);
      addstr("===Sell Shirts===");
      move(4,0);
      addstr(" The cost of shirt supplies is $4/day. Income depends on tailoring and ");
      move(5,0);
      addstr(" business skills. Income is lower if the country is very conservative.");
      move(7,0);
      addstr(" This activity trains tailoring and business.");
      break;
   case ACTIVITY_SELL_ART:
      move(2,0);
      addstr("===Sell Portrait Sketches===");
      move(4,0);
      addstr(" The cost of art supplies is $1/day. Income depends on art skill. ");
      move(6,0);
      addstr(" This activity trains art. Income is lower if the country is very conservative.");
   case ACTIVITY_SELL_MUSIC:
      move(2,0);
      addstr("===Play Street Music===");
      move(4,0);
      addstr(" There is no overhead for this activity. Income depends on music skill. ");
      move(6,0);
      addstr(" An equipped guitar will increase income substantially and speed skill gains.");
      move(7,0);
      addstr(" Income is lower if the country is very conservative.");
      move(9,0);
      addstr(" This activity trains music.");
      break;

      // Illegal fund raising
   case ACTIVITY_SELL_DRUGS:
      move(2,0);
      addstr("===Selling Brownies===");
      move(4,0);
      addstr(" Selling brownies on the street is an illegal way to make money. Money earned is ");
      move(5,0);
      addstr(" based upon the activists persuasion, cooking, street sense and charisma.");
      move(7,0);
      addstr(" It raises persuasion, street sense, business and cooking. Street sense is used ");
      move(8,0);
      addstr(" to avoid being caught. If caught and tried, the severity of the crime can vary ");
      move(9,0);
      addstr(" greatly based upon circumstances. ");
      move(11,0);
      addstr(" The more illegal drugs are in the country, the more money you will make. However, ");
      move(12,0);
      addstr(" the consequences of being convicted will increase as well. ");
      break;
   case ACTIVITY_PROSTITUTION:
      move(2,0);
      addstr("===Prostitution===");
      move(4,0);
      addstr(" Prostitution is an activity which raises money illegally. The amount of money");
      move(5,0);
      addstr(" made is based on seduction. The Seduction skill is trained and it also");
      move(6,0);
      addstr(" decreases your Juice every time you do it. Prostitution is quite lucrative ");
      move(7,0);
      addstr(" with seduction that is over 10.");
      move(9,0);
      addstr(" Street sense will help you evade being criminalized and caught.");
      break;
   case ACTIVITY_CCFRAUD:
      move(2,0);
      addstr("===Credit Card Fraud===");
      move(4,0);
      addstr(" To steal credit card numbers you need to have someone who has a computer ");
      move(5,0);
      addstr(" skill of at least 2. The more computer skill your hacker has the more money ");
      move(6,0);
      addstr(" you will make. This activity trains computer skill.");
      break;
   case ACTIVITY_REPAIR_ARMOR:
      move(2,0);
      addstr("===Repair Clothing===");
      move(4,0);
      addstr(" Repairing clothing will repair damaged armour and remove blood stains if there ");
      move(5,0);
      addstr(" are any. ");
      move(7,0);
      addstr(" The chance of repairing the clothing is dependant on the quality of ");
      move(8,0);
      addstr(" the clothing and the tailoring skill of the activist.");
      break;
   default:
      addstr("===THE GREAT UNKNOWN===");
      move(2,1);
      addstr("There is no help on this topic yet, as the help system is quite new.");
      move(3,1);
      addstr("However, if you want to help us, you could write one, and send it to us!");
      break;
   }

   set_color(COLOR_WHITE,COLOR_BLACK,0);
   move(23,0);
   addstr("   Press any key - Return to previous screen");
   getch();
}
