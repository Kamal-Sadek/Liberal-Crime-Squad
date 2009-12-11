#include <externs.h>

/**
   ACTIVITY_VISIT, TODO
   ACTIVITY_HOSTAGETENDING, TODO
   ACTIVITY_SELL_ART, TODO
   ACTIVITY_SELL_MUSIC, TODO
   ACTIVITY_SELL_TSHIRTS, TODO
   ACTIVITY_DONATIONS, TODO
   ACTIVITY_SELL_DRUGS, TODO
   ACTIVITY_PROSTITUTION, TODO
   ACTIVITY_CCFRAUD, TODO
   ACTIVITY_DOS_RACKET, TODO
   ACTIVITY_REPAIR_ARMOR, TODO
   ACTIVITY_MAKE_ARMOR, TODO
   ACTIVITY_STEALCARS, TODO
   ACTIVITY_WHEELCHAIR, TODO
   ACTIVITY_BURY, TODO
   ACTIVITY_WRITE_BLOG, TODO
   ACTIVITY_WRITE_LETTERS, TODO
   ACTIVITY_WRITE_GUARDIAN, TODO
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
        if(activityType == ACTIVITY_NONE)
        {
                return;
        }

        erase();
        set_color(COLOR_WHITE,COLOR_BLACK,0);
        move(0,0);
        addstr("Liberal help on: ");

switch(activityType)
   {
        case ACTIVITY_COMMUNITYSERVICE:
                addstr("===Community Service===");
                move(2,1);
                addstr("Has a minor effect on public opinion, increases Juice and has the chance");
                move(3,1);
                addstr("of reducing the activists Heat.");
        break;
        case ACTIVITY_TROUBLE:
                addstr("===Liberal Disobedience===");
                move(2,1);
                addstr("Liberal Disobedience is an illegal activity and a form of Liberal Activism"); 
                move(3,1);
                addstr("which has a positive effect on Public Opinion. It has very short prison");        
                move(4,1);
                addstr("sentences if you get convicted. It also increases juice.");        

                move(6,1);
                addstr("Persuasion, Charisma, Agility, Heart, Strength, Intelligence will increase the");        
                move(7,1);
                addstr("effect you have on Public Opinion.");

                move(9,1);
                addstr("There is a chance that someone will phone the police. Agility, intelligence,");
                move(10,1);
                addstr("disguise and street sense will shake them off the scent. There is also a");
                move(11,1);
                addstr("chance that an angry mob will beat the crap out of you.");
        break;
        case ACTIVITY_POLLS:
                addstr("===Search Opinion Polls===");
                move(2,1);
                addstr("Search Opinion Polls is a legal activity that lets you see what people think");
                move(3,1);
                addstr("about various Issues that are part of Public Opinion.");

                move(5,1);
                addstr("Skill in computers and intelligence will give better results.");
        break;
        case ACTIVITY_DOS_ATTACKS:
                addstr("===Harassing Websites===");
                move(2,1);
                addstr("Harassing Websites is an illegal activity and a form of Liberal Activism.");
                move(3,1);
                addstr("It is a weaker version of Hacking. Resulting in much shorter prison");
                move(4,1);
                addstr("sentences, and is much less likely to provoke a raid by the Police or");
                move(5,1);
                addstr("other law enforcement. Harassing websites only requires a");
                move(6,1);
                addstr("Computers skill of 1.");

                move(7,1);
                addstr("Utilizes only the Computers skill.");
        break;
        case ACTIVITY_HACKING:
                addstr("===Hacking===");
                move(2,1);
                addstr("Hacking is a highly illegal form of Liberal Activism, which has a good");
                move(3,1);
                addstr("effect on public opinion. With it, comes very long prison sentences.");

                move(5,1);
                addstr("Generally you need a lot of the Computer skill and Intelligence before"); 
                move(6,1);
                addstr("you can make a successful hacking attempt. If you are successful there");
                move(7,1);
                addstr("is a chance depending on the Computer skill and Intelligence of evading");
                move(8,1);
                addstr("being criminalized. If you are criminalized, heat on the activist");
                move(9,1);
                addstr("increases. Additionally if successful it will increase juice.");

                move(11,1);
                addstr("Occasionally instead of effecting public opinion your hacker");
                move(12,1);
                addstr("may find files that can be published in a special edition of");
                move(13,1);
                addstr("the Liberal Guardian.");
        break;
        case ACTIVITY_GRAFFITI:
                addstr("===Graffiti==");
                move(2,1);
                addstr("Spraying political graffiti is a misdemeanor, carrying with it");
                move(3,1);
                addstr("relatively short jail sentences. This has a positive effect on");
                move(4,1);
                addstr("public opinion.");

                move(5,1);
                addstr("If you do not have a can of spray paint, your character will spend");
                move(6,1);
                addstr("a day purchasing one.");

                move(8,1);
                addstr("A high art skill and heart will enhance the effect this has");
                move(9,1);
                addstr("on public opinion.");
        break;
        default:
                addstr("===THE GREAT UNKNOWN===");
		move(2,1);
		addstr("There is no help on this topic yet, as the help system is quite new.");
                move(3,1);
                addstr("However, if you want to help us, you could write one, and send it to us!");
        break;


        set_color(COLOR_WHITE,COLOR_BLACK,0);
        move(20,5);
        addstr("Press any key - Return to previous screen");
        getch();
}
