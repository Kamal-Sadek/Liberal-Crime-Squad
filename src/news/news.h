/**
 * Interface for the news module.
 */
/*
 * Copyright 2017 Stephen M. Webb
 *
 * This file is part of Liberal Crime Squad.
 *
 * Liberal Crime Squad is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
#ifndef LCS_NEWS_NEWS_H
#define LCS_NEWS_NEWS_H


/* news - graphics */
void loadgraphics();
void displaycenterednewsfont(std::string const& str, int y);
void displaycenteredsmallnews(std::string const& str, int y);
void displaynewspicture(int p,int y);
/* news - constructs non-LCS related event stories */
void constructeventstory(char *story, short view, char positive);
/* news - draws the specified block of text to the screen */
void displaynewsstory(char* story, short* storyx_s, short* storyx_e, int y);
/* news - shows animated news stories */
void run_television_news_stories();
/* news - make some filler junk */
void generatefiller(char* story, int amount);
/* news - major newspaper reporting on lcs and other topics */
void majornewspaper(char& clearformess, char canseethings);

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

#endif /* LCS_NEWS_NEWS_H */
