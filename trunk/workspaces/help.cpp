#include <externs.h>

// Help pages are drawn here.

// NOTE: This will probably need updating if there are any fundamental code changes
// that will change gameplay.
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
		addstr("===Community service===");
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
		addstr("effect.");

		move(9,1);
		addstr("There is a chance that someone will phone to police. Agility, intelligence,");
		move(10,1);
		addstr("disguise and street sense will shake them off the scent.");
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
		addstr("===Harass Websites===");
		move(2,1);
		addstr("Harass Websites is an illegal activity and a form of Liberal Activism. It");
		move(3,1);
		addstr("is a weaker version of Hacking. Much shorter prison sentences, is much");
		move(4,1);
		addstr("less likely to provoke a Raid by the Police or other law enforcement, and");
		move(5,1);
		addstr("only requires a Computers skill of 1.");

		move(7,1);
		addstr("Utilises only the Computers skill.");
      break;
   case ACTIVITY_HACKING:
		addstr("===Hacking===");
		move(2,1);
		addstr("Hacking is an activity, the most highly illegal form of Liberal Activism,");
		move(3,1);
		addstr("which has a good effect on public opinion, carrying very long prison");
		move(4,1);
		addstr("sentences.");

		move(6,1);
		addstr("Generally you need a lot of the Computer skill and Intelligence before"); 
		move(7,1);
		addstr("you can make a successful Hacking attempt. If you are successful there");
		move(8,1);
		addstr("is a chance depending on the Computer skill and Intelligence of evading");
		move(9,1);
		addstr("being criminalized. If you are criminalised heat on the activist");
		move(10,1);
		addstr("increases. Additionally if successful it will increase juice.");

		move(12,1);
		addstr("In addition to affecting public opinion, your hackers may find files");
		move(13,1);
		addstr("that can be used with the Liberal Guardian.");
      break;
   default:
      addstr("===THE GREAT UNKNOWN===");
		move(2,1);
		addstr("There is no help on this topic yet. Sorry, the help system is very new!");
	}

	set_color(COLOR_WHITE,COLOR_BLACK,0);
	move(20,5);
	addstr("Press any key - Return to previous screen");
	getch();
}
