
//#include <includeDefault.h>
//#include "configfile.h"
//#include "tinydir.h"

void displaymajoreventstory(newsstoryst& ns,char* story,short* storyx_s,short* storyx_e);
void squadstory_text_location(newsstoryst& ns,bool liberalguardian,bool ccs,char* story);
void squadstory_text_opening(newsstoryst& ns,bool liberalguardian,bool ccs,char* story);
void preparepage(newsstoryst& ns, bool liberalguardian);
void displayads(newsstoryst& ns, bool liberalguardian, short* storyx_s, short* storyx_e,int& it2);
void displaystoryheader(newsstoryst& ns, bool liberalguardian, int& y, int header);
void generate_random_event_news_stories();
void advance_ccs_defeat_storyline();
void clean_up_empty_news_stories();
void run_television_news_stories();
void assign_page_numbers_to_newspaper_stories();
void display_newspaper();
void handle_public_opinion_impact(const newsstoryst &ns);
int liberal_guardian_writing_power();
newsstoryst* ccs_exposure_story();
newsstoryst* ccs_fbi_raid_story();
newsstoryst* ccs_strikes_story();
newsstoryst* new_major_event();
