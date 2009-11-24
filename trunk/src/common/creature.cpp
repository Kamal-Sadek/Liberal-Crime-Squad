/*

Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
                                                                                      //
This file is part of Liberal Crime Squad.                                             //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/

/*
        This file was created by Chris Johnson (grundee@users.sourceforge.net)
        by copying code from game.cpp.
        To see descriptions of files and functions, see the list at 
        the bottom of includes.h in the top src folder.
*/

//#include <includes.h>
#include <externs.h>

void Creature::train(int trainedskill, int experience, int upto)
{
   // Do we allow animals to gain skills? Right now, yes
   //if(animalgloss==ANIMALGLOSS_ANIMAL)return;

   // Don't give experience if already maxed out
   if(maxskill(trainedskill,*this)==skill[trainedskill] || upto==skill[trainedskill])
      return;
   // Don't give experience if requested to give none
   if(experience==0)
      return;
   // Skill gain scaled by ability in the area
   skill_ip[trainedskill]+=max(1,static_cast<int>(experience * maxskill(trainedskill,*this,false) / 6.0));

   int abovenextlevel;
   // only allow gaining experience on the new level if it doesn't put us over a level limit
   if (skill[trainedskill] >= (upto - 1) || 
       skill[trainedskill] >= (maxskill (trainedskill,*this) - 1))
     abovenextlevel = 0;
   else
     abovenextlevel = 50 + 5*(1+skill[trainedskill]); // enough skill points to get halfway through the next skill level

   skill_ip[trainedskill] = min(skill_ip[trainedskill], 100 + 10*skill[trainedskill] + abovenextlevel);

}

void Creature::train(int trainedskill, int experience)
{
   return this->train (trainedskill, experience, 20);
}

void Creature::skill_up()
{
   for(int s=0;s<SKILLNUM;s++)
   {
      while(skill_ip[s]>=100+10*skill[s]&&
            skill[s]<maxskill(s,*this))
      {
         skill_ip[s]-=100+10*skill[s];
         skill[s]++;
      }
      if(skill[s]==maxskill(s,*this))
         skill_ip[s]=0;
   }
}

int Creature::get_skill_ip(int skill)
{
   return skill_ip[skill];
}

bool Creature::enemy()
{
   if(align==-1)
   {
      return true;
   }
   else
   {
      if(type==CREATURE_COP)
      {
         for(int i=0;i<pool.size();i++)
         {
            if(pool[i]==this)
            {
               return false;
            }
         }
         return true;
      }
   }
   return false;
}


// Add an age estimate to a person's name
void add_age(Creature& person)
{
   // Who knows how old the purple gorilla/tank/flaming bunny/dog is?
   if(person.animalgloss!=ANIMALGLOSS_NONE)
   {
      addstr(" (?)");
      return;
   }

   // For humans, estimate their age and gender
   addstr(" (");

   // Almost precise estimates of child and teen ages
   if(person.age<20)
   {
      char num[5];
      // Inaccuracy in estimating age should be the same every
      // time a character is queried. I'm using the day of the
      // month the character was born on to determine this.
      itoa(person.age+person.birthday_day%3-1,num,10);
      addstr(num);
      addstr("?");
   }
   // More rough estimates of everyone else
   else
   {
      if(person.age<30)
         addstr("20s");
      else if(person.age<40)
         addstr("30s");
      else if(person.age<50)
         addstr("40s");
      else if(person.age<60)
         addstr("50s");
      else if(person.age<70)
         addstr("60s");
      else if(person.age<80)
         addstr("70s");
      else if(person.age<90)
         addstr("80s");
      else
         addstr("Very Old");
   }

   // Assess their gender Liberally
   if(person.gender_liberal == GENDER_MALE)
      addstr(", Male");
   else if(person.gender_liberal == GENDER_FEMALE)
      addstr(", Female");
   else
      addstr(", Unknown");

   // Note if there's some conflict with Conservative society's perceptions
   if(person.gender_liberal != person.gender_conservative)
      addstr("?");

   addstr(")");
}


/* rolls up a proper name for a creature */
void Creature::namecreature()
{
   generate_name(name, gender_liberal);
   strcpy(propername,name);
}



/* fills a string with a proper name */
void generate_name(char *str, char gender)
{
   strcpy(str,"");

   char first[80];
   firstname(first, gender);
   strcat(str,first);

   strcat(str," ");

   char last[80];
   lastname(last);
   strcat(str,last);
}



/* gets a random first name */
void firstname(char *str, char gender)
{
   strcpy(str,"");

   int roll, nametable;

   // If we don't care if the name is male or female, pick one randomly
   // This ensures gender balance in the names chosen
   if(gender == GENDER_NEUTRAL)
      gender = LCSrandom(2) + 1;

   // For white male Arch-Conservative politicians
   if(gender == GENDER_WHITEMALEPATRIARCH)
   {
      roll = LCSrandom(50);
      nametable = GENDER_WHITEMALEPATRIARCH;
   }

   // Assign a name from the available names for each gender
   if(gender == GENDER_MALE)
   {
      // Roll on the number of gender-specific names,
      // plus the number of gender-neutral names
      roll = LCSrandom(346 + 34);
      // Decide whether to use a gender-specific name
      // or a gender-neutral name
      if(roll >= 34) { roll -= 34; nametable = GENDER_MALE; }
      else nametable = GENDER_NEUTRAL;
   }
   else if(gender == GENDER_FEMALE)
   {
      // (Same here, just use the number of female names instead)
      roll = LCSrandom(346 + 34);
      if(roll >= 34) { roll -= 34; nametable = GENDER_FEMALE; }
      else nametable = GENDER_NEUTRAL;
   }

   if(nametable == GENDER_MALE)
//{{{ Male
   {
      switch(roll)
      {
         //{{{ Male name table
         case 0:strcat(str,"Aaron");break;
         case 1:strcat(str,"Abraham");break;
         case 2:strcat(str,"Adam");break;
         case 3:strcat(str,"Adolph");break;
         case 4:strcat(str,"Adrian");break;
         case 5:strcat(str,"Al");break;
         case 6:strcat(str,"Alan");break;
         case 7:strcat(str,"Albert");break;
         case 8:strcat(str,"Alec");break;
         case 9:strcat(str,"Alejandro");break;
         case 10:strcat(str,"Alexander");break;
         case 11:strcat(str,"Alf");break;
         case 12:strcat(str,"Alfie");break;
         case 13:strcat(str,"Alfred");break;
         case 14:strcat(str,"Ali");break;
         case 15:strcat(str,"Allan");break;
         case 16:strcat(str,"Allen");break;
         case 17:strcat(str,"Andrew");break;
         case 18:strcat(str,"Andy");break;
         case 19:strcat(str,"Anthony");break;
         case 20:strcat(str,"Antonio");break;
         case 21:strcat(str,"Archie");break;
         case 22:strcat(str,"Arnold");break;
         case 23:strcat(str,"Art");break;
         case 24:strcat(str,"Arthur");break;
         case 25:strcat(str,"Avi");break;
         case 26:strcat(str,"Axel");break;
         case 27:strcat(str,"Barack");break;
         case 28:strcat(str,"Barney");break;
         case 29:strcat(str,"Barry");break;
         case 30:strcat(str,"Bartholomew");break;
         case 31:strcat(str,"Beau");break;
         case 32:strcat(str,"Ben");break;
         case 33:strcat(str,"Benjamin");break;
         case 34:strcat(str,"Bernard");break;
         case 35:strcat(str,"Bert");break;
         case 36:strcat(str,"Bill");break;
         case 37:strcat(str,"Billy Bob");break;
         case 38:strcat(str,"Blaire");break;
         case 39:strcat(str,"Blake");break;
         case 40:strcat(str,"Bob");break;
         case 41:strcat(str,"Bobby");break;
         case 42:strcat(str,"Boris");break;
         case 43:strcat(str,"Brad");break;
         case 44:strcat(str,"Bradley");break;
         case 45:strcat(str,"Brandon");break;
         case 46:strcat(str,"Brendon");break;
         case 47:strcat(str,"Brett");break;
         case 48:strcat(str,"Brian");break;
         case 49:strcat(str,"Bruce");break;
         case 50:strcat(str,"Bryan");break;
         case 51:strcat(str,"Calvin");break;
         case 52:strcat(str,"Carl");break;
         case 53:strcat(str,"Carlos");break;
         case 54:strcat(str,"Cesar");break;
         case 55:strcat(str,"Chad");break;
         case 56:strcat(str,"Charles");break;
         case 57:strcat(str,"Charlie");break;
         case 58:strcat(str,"Chester");break;
         case 59:strcat(str,"Chuck");break;
         case 60:strcat(str,"Chris");break;
         case 61:strcat(str,"Christian");break;
         case 62:strcat(str,"Christopher");break;
         case 63:strcat(str,"Claude");break;
         case 64:strcat(str,"Cliff");break;
         case 65:strcat(str,"Clifford");break;
         case 66:strcat(str,"Clyde");break;
         case 67:strcat(str,"Cody");break;
         case 68:strcat(str,"Colin");break;
         case 69:strcat(str,"Colton");break;
         case 70:strcat(str,"Conrad");break;
         case 71:strcat(str,"Cory");break;
         case 72:strcat(str,"Craig");break;
         case 73:strcat(str,"Dale");break;
         case 74:strcat(str,"Damon");break;
         case 75:strcat(str,"Dan");break;
         case 76:strcat(str,"Danforth");break;
         case 77:strcat(str,"Daniel");break;
         case 78:strcat(str,"Danny");break;
         case 79:strcat(str,"Dag");break;
         case 80:strcat(str,"Dave");break;
         case 81:strcat(str,"David");break;
         case 82:strcat(str,"Deepak");break;
         case 83:strcat(str,"Dennis");break;
         case 84:strcat(str,"Derak");break;
         case 85:strcat(str,"Derek");break;
         case 86:strcat(str,"Derrick");break;
         case 87:strcat(str,"Dexter");break;
         case 88:strcat(str,"Dick");break;
         case 89:strcat(str,"Diego");break;
         case 90:strcat(str,"Diwakar");break;
         case 91:strcat(str,"Dominic");break;
         case 92:strcat(str,"Donald");break;
         case 93:strcat(str,"Donovan");break;
         case 94:strcat(str,"Doug");break;
         case 95:strcat(str,"Douglas");break;
         case 96:strcat(str,"Dustin");break;
         case 97:strcat(str,"Dwayne");break;
         case 98:strcat(str,"Dwight");break;
         case 99:strcat(str,"Eathan");break;
         case 100:strcat(str,"Ed");break;
         case 101:strcat(str,"Eddie");break;
         case 102:strcat(str,"Edgar");break;
         case 103:strcat(str,"Eduardo");break;
         case 104:strcat(str,"Edward");break;
         case 105:strcat(str,"Edwin");break;
         case 106:strcat(str,"Eli");break;
         case 107:strcat(str,"Eliah");break;
         case 108:strcat(str,"Elliot");break;
         case 109:strcat(str,"Enrique");break;
         case 110:strcat(str,"Enzo");break;
         case 111:strcat(str,"Eric");break;
         case 112:strcat(str,"Erik");break;
         case 113:strcat(str,"Ernest");break;
         case 114:strcat(str,"Ernie");break;
         case 115:strcat(str,"Eugene");break;
         case 116:strcat(str,"Evan");break;
         case 117:strcat(str,"Fabio");break;
         case 119:strcat(str,"Felix");break;
         case 120:strcat(str,"Fernando");break;
         case 121:strcat(str,"Floyd");break;
         case 122:strcat(str,"Francesco");break;
         case 123:strcat(str,"Francis");break;
         case 124:strcat(str,"Frank");break;
         case 125:strcat(str,"Franklin");break;
         case 126:strcat(str,"Fred");break;
         case 127:strcat(str,"Frederick");break;
         case 128:strcat(str,"Gabriel");break;
         case 129:strcat(str,"Garett");break;
         case 130:strcat(str,"Garreth");break;
         case 131:strcat(str,"Gary");break;
         case 132:strcat(str,"George");break;
         case 133:strcat(str,"Gerald");break;
         case 134:strcat(str,"Glen");break;
         case 135:strcat(str,"Graham");break;
         case 136:strcat(str,"Greg");break;
         case 137:strcat(str,"Gregory");break;
         case 138:strcat(str,"Gus");break;
         case 139:strcat(str,"Guy");break;
         case 140:strcat(str,"Hank");break;
         case 141:strcat(str,"Hans");break;
         case 142:strcat(str,"Harold");break;
         case 143:strcat(str,"Harry");break;
         case 144:strcat(str,"Harvey");break;
         case 145:strcat(str,"Henry");break;
         case 146:strcat(str,"Herbert");break;
         case 147:strcat(str,"Homer");break;
         case 148:strcat(str,"Howard");break;
         case 149:strcat(str,"Howie");break;
         case 150:strcat(str,"Hugh");break;
         case 151:strcat(str,"Hugo");break;
         case 152:strcat(str,"Ian");break;
         case 153:strcat(str,"Isaac");break;
         case 154:strcat(str,"Issac");break;
         case 155:strcat(str,"Ivan");break;
         case 156:strcat(str,"Jack");break;
         case 157:strcat(str,"Jacob");break;
         case 158:strcat(str,"Jake");break;
         case 159:strcat(str,"Jamel");break;
         case 160:strcat(str,"James");break;
         case 161:strcat(str,"Jared");break;
         case 162:strcat(str,"Jason");break;
         case 163:strcat(str,"Javier");break;
         case 164:strcat(str,"Jay");break;
         case 165:strcat(str,"Jean");break;
         case 166:strcat(str,"Jeff");break;
         case 167:strcat(str,"Jeffrey");break;
         case 168:strcat(str,"Jeremy");break;
         case 169:strcat(str,"Jeroen");break;
         case 170:strcat(str,"Jerome");break;
         case 171:strcat(str,"Jerry");break;
         case 172:strcat(str,"Jesse");break;
         case 173:strcat(str,"Jesus");break;
         case 174:strcat(str,"Jim");break;
         case 175:strcat(str,"Jimmy");break;
         case 176:strcat(str,"Jinyuan");break;
         case 177:strcat(str,"Joe");break;
         case 178:strcat(str,"Joel");break;
         case 179:strcat(str,"Joey");break;
         case 180:strcat(str,"John");break;
         case 181:strcat(str,"Johnny");break;
         case 182:strcat(str,"Jon");break;
         case 183:strcat(str,"Jonathan");break;
         case 184:strcat(str,"Jordan");break;
         case 185:strcat(str,"Jorge");break;
         case 186:strcat(str,"Jose");break;
         case 187:strcat(str,"Joseph");break;
         case 188:strcat(str,"Josh");break;
         case 189:strcat(str,"Joshua");break;
         case 190:strcat(str,"Jowar");break;
         case 191:strcat(str,"Juan");break;
         case 192:strcat(str,"Julian");break;
         case 193:strcat(str,"Justin");break;
         case 194:strcat(str,"Kaelen");break;
         case 195:strcat(str,"Karl");break;
         case 196:strcat(str,"Keith");break;
         case 197:strcat(str,"Ken");break;
         case 198:strcat(str,"Kenji");break;
         case 199:strcat(str,"Kennedy");break;
         case 200:strcat(str,"Kenneth");break;
         case 201:strcat(str,"Kevin");break;
         case 202:strcat(str,"Kirk");break;
         case 203:strcat(str,"Konstantine");break;
         case 204:strcat(str,"Kurt");break;
         case 205:strcat(str,"Kweisi");break;
         case 206:strcat(str,"Kyle");break;
         case 207:strcat(str,"Lance");break;
         case 208:strcat(str,"Larry");break;
         case 209:strcat(str,"Lawrence");break;
         case 210:strcat(str,"Lenny");break;
         case 211:strcat(str,"Leo");break;
         case 212:strcat(str,"Leonard");break;
         case 213:strcat(str,"Leroy");break;
         case 214:strcat(str,"Lex");break;
         case 215:strcat(str,"Liam");break;
         case 216:strcat(str,"Lionel");break;
         case 217:strcat(str,"Louis");break;
         case 218:strcat(str,"Lucas");break;
         case 219:strcat(str,"Luis");break;
         case 220:strcat(str,"Luke");break;
         case 221:strcat(str,"Malcolm");break;
         case 222:strcat(str,"Manny");break;
         case 223:strcat(str,"Manuel");break;
         case 224:strcat(str,"Marcus");break;
         case 225:strcat(str,"Marvin");break;
         case 226:strcat(str,"Mark");break;
         case 227:strcat(str,"Mario");break;
         case 228:strcat(str,"Martin");break;
         case 229:strcat(str,"Marty");break;
         case 230:strcat(str,"Mason");break;
         case 231:strcat(str,"Matt");break;
         case 232:strcat(str,"Matthew");break;
         case 233:strcat(str,"Maurice");break;
         case 234:strcat(str,"Max");break;
         case 235:strcat(str,"Mel");break;
         case 236:strcat(str,"Michael");break;
         case 237:strcat(str,"Mickey");break;
         case 238:strcat(str,"Miguel");break;
         case 239:strcat(str,"Mike");break;
         case 240:strcat(str,"Miles");break;
         case 241:strcat(str,"Mitchell");break;
         case 242:strcat(str,"Moe");break;
         case 243:strcat(str,"Mumtaz");break;
         case 244:strcat(str,"Murray");break;
         case 245:strcat(str,"Mustafa");break;
         case 246:strcat(str,"Nathan");break;
         case 247:strcat(str,"Ned");break;
         case 248:strcat(str,"Neal");break;
         case 249:strcat(str,"Neil");break;
         case 250:strcat(str,"Nelson");break;
         case 251:strcat(str,"Nicholas");break;
         case 252:strcat(str,"Nick");break;
         case 253:strcat(str,"Noah");break;
         case 254:strcat(str,"Norm");break;
         case 255:strcat(str,"Norman");break;
         case 256:strcat(str,"Oliver");break;
         case 257:strcat(str,"Otto");break;
         case 258:strcat(str,"Owen");break;
         case 259:strcat(str,"Pablo");break;
         case 260:strcat(str,"Patrick");break;
         case 261:strcat(str,"Paul");break;
         case 262:strcat(str,"Pedro");break;
         case 263:strcat(str,"Pete");break;
         case 264:strcat(str,"Peter");break;
         case 265:strcat(str,"Phil");break;
         case 266:strcat(str,"Phillip");break;
         case 267:strcat(str,"Preston");break;
         case 268:strcat(str,"Rafael");break;
         case 269:strcat(str,"Ralf");break;
         case 270:strcat(str,"Ralph");break;
         case 271:strcat(str,"Randy");break;
         case 272:strcat(str,"Rane");break;
         case 273:strcat(str,"Ransom");break;
         case 274:strcat(str,"Raphael");break;
         case 275:strcat(str,"Raul");break;
         case 276:strcat(str,"Ray");break;
         case 277:strcat(str,"Raymond");break;
         case 278:strcat(str,"Reginald");break;
         case 279:strcat(str,"Ricardo");break;
         case 280:strcat(str,"Richard");break;
         case 281:strcat(str,"Rick");break;
         case 282:strcat(str,"Robert");break;
         case 283:strcat(str,"Rod");break;
         case 284:strcat(str,"Rodney");break;
         case 285:strcat(str,"Roger");break;
         case 286:strcat(str,"Ron");break;
         case 287:strcat(str,"Ronald");break;
         case 288:strcat(str,"Rory");break;
         case 289:strcat(str,"Roy");break;
         case 290:strcat(str,"Rubin");break;
         case 291:strcat(str,"Rudy");break;
         case 292:strcat(str,"Rupert");break;
         case 293:strcat(str,"Russ");break;
         case 294:strcat(str,"Russell");break;
         case 295:strcat(str,"Ryan");break;
         case 296:strcat(str,"Samuel");break;
         case 297:strcat(str,"Scott");break;
         case 298:strcat(str,"Sean");break;
         case 299:strcat(str,"Sebastian");break;
         case 300:strcat(str,"Serge");break;
         case 301:strcat(str,"Sergio");break;
         case 302:strcat(str,"Seth");break;
         case 303:strcat(str,"Shane");break;
         case 304:strcat(str,"Shaun");break;
         case 305:strcat(str,"Shawn");break;
         case 306:strcat(str,"Sid");break;
         case 307:strcat(str,"Simon");break;
         case 308:strcat(str,"Sonny");break;
         case 309:strcat(str,"Stan");break;
         case 310:strcat(str,"Stanley");break;
         case 311:strcat(str,"Stephen");break;
         case 312:strcat(str,"Steve");break;
         case 313:strcat(str,"Steven");break;
         case 314:strcat(str,"Stewart");break;
         case 315:strcat(str,"Stu");break;
         case 316:strcat(str,"Stuart");break;
         case 317:strcat(str,"Ted");break;
         case 318:strcat(str,"Tetsuo");break;
         case 319:strcat(str,"Theodore");break;
         case 320:strcat(str,"Thomas");break;
         case 321:strcat(str,"Thor");break;
         case 322:strcat(str,"Tim");break;
         case 323:strcat(str,"Timothy");break;
         case 324:strcat(str,"Toby");break;
         case 325:strcat(str,"Tom");break;
         case 326:strcat(str,"Tony");break;
         case 327:strcat(str,"Toshiro");break;
         case 328:strcat(str,"Travis");break;
         case 329:strcat(str,"Trevor");break;
         case 330:strcat(str,"Tyler");break;
         case 331:strcat(str,"Tyrone");break;
         case 332:strcat(str,"Victor");break;
         case 333:strcat(str,"Vincent");break;
         case 334:strcat(str,"Wade");break;
         case 335:strcat(str,"Wally");break;
         case 336:strcat(str,"Walter");break;
         case 337:strcat(str,"Wayne");break;
         case 338:strcat(str,"William");break;
         case 339:strcat(str,"Willy");break;
         case 340:strcat(str,"Winston");break;
         case 341:strcat(str,"Wolf");break;
         case 342:strcat(str,"Woody");break;
         case 343:strcat(str,"Xavier");break;
         case 344:strcat(str,"Zachary");break;
         case 345:strcat(str,"Zeke");break;
//}}}


      default:strcat(str,"Defaulto");break;
      }
   }
//}}}
   else if(nametable == GENDER_FEMALE)
//{{{ Female
   {
      switch(roll)
      {
         // {{{Female name table
      case 0:strcat(str,"Abigail");break;
      case 1:strcat(str,"Ada");break;
      case 2:strcat(str,"Adriana");break;
      case 3:strcat(str,"Adrianna");break;
      case 4:strcat(str,"Adrienne");break;
      case 5:strcat(str,"Agatha");break;
      case 6:strcat(str,"Aiko");break;
      case 7:strcat(str,"Aileen");break;
      case 8:strcat(str,"Alene");break;
      case 9:strcat(str,"Alexandra");break;
      case 10:strcat(str,"Alice");break;
      case 11:strcat(str,"Alison");break;
      case 12:strcat(str,"Allison");break;
      case 13:strcat(str,"Alora");break;
      case 14:strcat(str,"Alyson");break;
      case 15:strcat(str,"Amanda");break;
      case 16:strcat(str,"Amber");break;
      case 17:strcat(str,"Amelia");break;
      case 18:strcat(str,"Amy");break;
      case 19:strcat(str,"Andrea");break;
      case 20:strcat(str,"Angela");break;
      case 21:strcat(str,"Angelica");break;
      case 22:strcat(str,"Anita");break;
      case 23:strcat(str,"Ann");break;
      case 24:strcat(str,"Anna");break;
      case 25:strcat(str,"Anne");break;
      case 26:strcat(str,"Annie");break;
      case 27:strcat(str,"April");break;
      case 28:strcat(str,"Autumn");break;
      case 29:strcat(str,"Barbara");break;
      case 30:strcat(str,"Bea");break;
      case 31:strcat(str,"Beatrice");break;
      case 32:strcat(str,"Bebe");break;
      case 33:strcat(str,"Becky");break;
      case 34:strcat(str,"Belle");break;
      case 35:strcat(str,"Beth");break;
      case 36:strcat(str,"Beth Ann");break;
      case 37:strcat(str,"Bethany");break;
      case 38:strcat(str,"Betty");break;
      case 39:strcat(str,"Bianca Maria");break;
      case 40:strcat(str,"Blanche");break;
      case 41:strcat(str,"Bobbie");break;
      case 42:strcat(str,"Bonita");break;
      case 43:strcat(str,"Bonnie");break;
      case 44:strcat(str,"Brenda");break;
      case 45:strcat(str,"Brittany");break;
      case 46:strcat(str,"Brooke");break;
      case 47:strcat(str,"Brooklyn");break;
      case 48:strcat(str,"Camilia");break;
      case 49:strcat(str,"Carla");break;
      case 50:strcat(str,"Carmen");break;
      case 51:strcat(str,"Carol");break;
      case 52:strcat(str,"Caroline");break;
      case 53:strcat(str,"Cassandra");break;
      case 54:strcat(str,"Catherine");break;
      case 55:strcat(str,"Cecilia");break;
      case 56:strcat(str,"Celine");break;
      case 57:strcat(str,"Charlotte");break;
      case 58:strcat(str,"Chelsea");break;
      case 59:strcat(str,"Cheryl");break;
      case 60:strcat(str,"Chloe");break;
      case 61:strcat(str,"Christine");break;
      case 62:strcat(str,"Cindi");break;
      case 63:strcat(str,"Cindy");break;
      case 64:strcat(str,"Clara");break;
      case 65:strcat(str,"Clarity");break;
      case 66:strcat(str,"Claudette");break;
      case 67:strcat(str,"Claudia");break;
      case 68:strcat(str,"Colette");break;
      case 69:strcat(str,"Colleen");break;
      case 70:strcat(str,"Consuela");break;
      case 71:strcat(str,"Courtney");break;
      case 72:strcat(str,"Crystal");break;
      case 73:strcat(str,"Cybill");break;
      case 74:strcat(str,"Dakota");break;
      case 75:strcat(str,"Dana");break;
      case 76:strcat(str,"Daniela");break;
      case 77:strcat(str,"Danielle");break;
      case 78:strcat(str,"Daphne");break;
      case 79:strcat(str,"Dawn");break;
      case 80:strcat(str,"Debbie");break;
      case 81:strcat(str,"Denise");break;
      case 82:strcat(str,"Desiree");break;
      case 83:strcat(str,"Diane");break;
      case 84:strcat(str,"Dimitra");break;
      case 85:strcat(str,"Donna");break;
      case 86:strcat(str,"Dora");break;
      case 87:strcat(str,"Doreen");break;
      case 88:strcat(str,"Edna");break;
      case 89:strcat(str,"Elaine");break;
      case 90:strcat(str,"Elinor");break;
      case 91:strcat(str,"Elizabeth");break;
      case 92:strcat(str,"Ellen");break;
      case 93:strcat(str,"Elsie");break;
      case 94:strcat(str,"Emily");break;
      case 95:strcat(str,"Emma");break;
      case 96:strcat(str,"Erica");break;
      case 97:strcat(str,"Erika");break;
      case 98:strcat(str,"Esmerelda");break;
      case 99:strcat(str,"Eve");break;
      case 100:strcat(str,"Faith");break;
      case 101:strcat(str,"Fatima");break;
      case 102:strcat(str,"Felicia");break;
      case 103:strcat(str,"Felicity");break;
      case 104:strcat(str,"Fiona");break;
      case 105:strcat(str,"Florence");break;
      case 106:strcat(str,"Florida");break;
      case 107:strcat(str,"Fran");break;
      case 108:strcat(str,"Francine");break;
      case 109:strcat(str,"Gabrielle");break;
      case 110:strcat(str,"Georgia");break;
      case 111:strcat(str,"Gina");break;
      case 112:strcat(str,"Giovanna");break;
      case 113:strcat(str,"Gloria");break;
      case 114:strcat(str,"Grace");break;
      case 115:strcat(str,"Gwen");break;
      case 116:strcat(str,"Gwendolyn");break;
      case 117:strcat(str,"Hailey");break;
      case 118:strcat(str,"Haley");break;
      case 119:strcat(str,"Hannah");break;
      case 120:strcat(str,"Harmony");break;
      case 121:strcat(str,"Hayley");break;
      case 122:strcat(str,"Hazel");break;
      case 123:strcat(str,"Heather");break;
      case 124:strcat(str,"Helen");break;
      case 125:strcat(str,"Helga");break;
      case 126:strcat(str,"Holly");break;
      case 127:strcat(str,"Honor");break;
      case 128:strcat(str,"Ida");break;
      case 129:strcat(str,"Ingrid");break;
      case 130:strcat(str,"Irene");break;
      case 131:strcat(str,"Iris");break;
      case 132:strcat(str,"Isabel");break;
      case 133:strcat(str,"Isabella");break;
      case 134:strcat(str,"Isabelle");break;
      case 135:strcat(str,"Ivy");break;
      case 136:strcat(str,"Jaclyn");break;
      case 137:strcat(str,"Jade");break;
      case 138:strcat(str,"Jane");break;
      case 139:strcat(str,"Janet");break;
      case 140:strcat(str,"Janette");break;
      case 141:strcat(str,"January");break;
      case 142:strcat(str,"Jaqueline");break;
      case 143:strcat(str,"Jasmin");break;
      case 144:strcat(str,"Jasmine");break;
      case 145:strcat(str,"Jennifer");break;
      case 146:strcat(str,"Jenny");break;
      case 147:strcat(str,"Jessica");break;
      case 148:strcat(str,"Jill");break;
      case 149:strcat(str,"Jillian");break;
      case 150:strcat(str,"Joan");break;
      case 151:strcat(str,"Joann");break;
      case 152:strcat(str,"Josephine");break;
      case 153:strcat(str,"Judy");break;
      case 154:strcat(str,"Juliana");break;
      case 155:strcat(str,"Julia");break;
      case 156:strcat(str,"Julie");break;
      case 157:strcat(str,"Juliette");break;
      case 158:strcat(str,"June");break;
      case 159:strcat(str,"Kaitlyn");break;
      case 160:strcat(str,"Kaley");break;
      case 161:strcat(str,"Karen");break;
      case 162:strcat(str,"Karla");break;
      case 163:strcat(str,"Kate");break;
      case 164:strcat(str,"Katelin");break;
      case 165:strcat(str,"Katherine");break;
      case 166:strcat(str,"Kathleen");break;
      case 167:strcat(str,"Kathy");break;
      case 168:strcat(str,"Katie");break;
      case 169:strcat(str,"Kay");break;
      case 170:strcat(str,"Kayla");break;
      case 171:strcat(str,"Kaylee");break;
      case 172:strcat(str,"Keely");break;
      case 173:strcat(str,"Kiki");break;
      case 174:strcat(str,"Kristal");break;
      case 175:strcat(str,"Kristie");break;
      case 176:strcat(str,"Kristin");break;
      case 177:strcat(str,"Krystal");break;
      case 178:strcat(str,"Lacey");break;
      case 179:strcat(str,"Lara");break;
      case 180:strcat(str,"Larissa");break;
      case 181:strcat(str,"Latoya");break;
      case 182:strcat(str,"Laura");break;
      case 183:strcat(str,"Lauren");break;
      case 184:strcat(str,"Lavender");break;
      case 185:strcat(str,"Leah");break;
      case 186:strcat(str,"Leila");break;
      case 187:strcat(str,"Leyna");break;
      case 188:strcat(str,"Liliana");break;
      case 189:strcat(str,"Lilly");break;
      case 190:strcat(str,"Lily");break;
      case 191:strcat(str,"Lina");break;
      case 192:strcat(str,"Linda");break;
      case 193:strcat(str,"Lindsay");break;
      case 194:strcat(str,"Lindsey");break;
      case 195:strcat(str,"Lisa");break;
      case 196:strcat(str,"Liz");break;
      case 197:strcat(str,"Lizabeth");break;
      case 198:strcat(str,"Lizzie");break;
      case 199:strcat(str,"Lois");break;
      case 200:strcat(str,"Loretta");break;
      case 201:strcat(str,"Louane");break;
      case 202:strcat(str,"Louanne");break;
      case 203:strcat(str,"Louise");break;
      case 204:strcat(str,"Lucia");break;
      case 205:strcat(str,"Lynn");break;
      case 206:strcat(str,"Lynnette");break;
      case 207:strcat(str,"Madison");break;
      case 208:strcat(str,"Madonna");break;
      case 209:strcat(str,"Manuela");break;
      case 210:strcat(str,"Margaret");break;
      case 211:strcat(str,"Marge");break;
      case 212:strcat(str,"Maria");break;
      case 213:strcat(str,"Mariana");break;
      case 214:strcat(str,"Marianne");break;
      case 215:strcat(str,"Marie");break;
      case 216:strcat(str,"Marion");break;
      case 217:strcat(str,"Marlene");break;
      case 218:strcat(str,"Martha");break;
      case 219:strcat(str,"Martina");break;
      case 220:strcat(str,"Mary");break;
      case 221:strcat(str,"Mary Ann");break;
      case 222:strcat(str,"Matilda");break;
      case 223:strcat(str,"Maureen");break;
      case 224:strcat(str,"Mayumi");break;
      case 225:strcat(str,"Megan");break;
      case 226:strcat(str,"Meghan");break;
      case 227:strcat(str,"Melanie");break;
      case 228:strcat(str,"Melissa");break;
      case 229:strcat(str,"Mercedes");break;
      case 230:strcat(str,"Merissa");break;
      case 231:strcat(str,"Mia");break;
      case 232:strcat(str,"Michelle");break;
      case 233:strcat(str,"Minal");break;
      case 234:strcat(str,"Mindy");break;
      case 235:strcat(str,"Miranda");break;
      case 236:strcat(str,"Molly");break;
      case 237:strcat(str,"Monica");break;
      case 238:strcat(str,"Nadia");break;
      case 239:strcat(str,"Nancy");break;
      case 240:strcat(str,"Natalie");break;
      case 241:strcat(str,"Natasha");break;
      case 242:strcat(str,"Nellie");break;
      case 243:strcat(str,"Nicole");break;
      case 244:strcat(str,"Nicolette");break;
      case 245:strcat(str,"Nina");break;
      case 246:strcat(str,"Norma");break;
      case 247:strcat(str,"October");break;
      case 248:strcat(str,"Olivia");break;
      case 249:strcat(str,"Pam");break;
      case 250:strcat(str,"Patricia");break;
      case 251:strcat(str,"Patty");break;
      case 252:strcat(str,"Paula");break;
      case 253:strcat(str,"Paulette");break;
      case 254:strcat(str,"Peggy");break;
      case 255:strcat(str,"Petunia");break;
      case 256:strcat(str,"Phoebe");break;
      case 257:strcat(str,"Priscilla");break;
      case 258:strcat(str,"Rachael");break;
      case 259:strcat(str,"Racheal");break;
      case 260:strcat(str,"Rachel");break;
      case 261:strcat(str,"Rachelle");break;
      case 262:strcat(str,"Rania");break;
      case 263:strcat(str,"Raphaela");break;
      case 264:strcat(str,"Raquel");break;
      case 265:strcat(str,"Rebecca");break;
      case 266:strcat(str,"Renee");break;
      case 267:strcat(str,"Rita");break;
      case 268:strcat(str,"Rosalie");break;
      case 269:strcat(str,"Rosalinda");break;
      case 270:strcat(str,"Rosanna");break;
      case 271:strcat(str,"Roberta");break;
      case 272:strcat(str,"Rose");break;
      case 273:strcat(str,"Rosemary");break;
      case 274:strcat(str,"Roxanne");break;
      case 275:strcat(str,"Roz");break;
      case 276:strcat(str,"Ruth");break;
      case 277:strcat(str,"Ruthanne");break;
      case 278:strcat(str,"Sabrina");break;
      case 279:strcat(str,"Sally");break;
      case 280:strcat(str,"Samantha");break;
      case 281:strcat(str,"Sandra");break;
      case 282:strcat(str,"Sara");break;
      case 283:strcat(str,"Sarah");break;
      case 284:strcat(str,"Satya");break;
      case 285:strcat(str,"Selma");break;
      case 286:strcat(str,"Shaniqua");break;
      case 287:strcat(str,"Shanna");break;
      case 288:strcat(str,"Shannon");break;
      case 289:strcat(str,"Sharon");break;
      case 290:strcat(str,"Sherry");break;
      case 291:strcat(str,"Shirley");break;
      case 292:strcat(str,"Shyann");break;
      case 293:strcat(str,"Silvia");break;
      case 294:strcat(str,"Sonia");break;
      case 295:strcat(str,"Sophia");break;
      case 296:strcat(str,"Sophie");break;
      case 297:strcat(str,"Stacie");break;
      case 298:strcat(str,"Stacy");break;
      case 299:strcat(str,"Stephanie");break;
      case 300:strcat(str,"Summer");break;
      case 301:strcat(str,"Sue");break;
      case 302:strcat(str,"Susan");break;
      case 303:strcat(str,"Susanna");break;
      case 304:strcat(str,"Suzanne");break;
      case 305:strcat(str,"Suzie");break;
      case 306:strcat(str,"Tabatha");break;
      case 307:strcat(str,"Tabitha");break;
      case 308:strcat(str,"Tamara");break;
      case 309:strcat(str,"Tammy");break;
      case 310:strcat(str,"Tara");break;
      case 311:strcat(str,"Tawanda");break;
      case 312:strcat(str,"Teresa");break;
      case 313:strcat(str,"Terri");break;
      case 314:strcat(str,"Tetiana");break;
      case 315:strcat(str,"Theresa");break;
      case 316:strcat(str,"Tiffanie");break;
      case 317:strcat(str,"Tiffany");break;
      case 318:strcat(str,"Tina");break;
      case 319:strcat(str,"Toni");break;
      case 320:strcat(str,"Tracy");break;
      case 321:strcat(str,"Tricia");break;
      case 322:strcat(str,"Trudy");break;
      case 323:strcat(str,"Tyra");break;
      case 324:strcat(str,"Valentina");break;
      case 325:strcat(str,"Valeria");break;
      case 326:strcat(str,"Valerie");break;
      case 327:strcat(str,"Vanessa");break;
      case 328:strcat(str,"Verena");break;
      case 329:strcat(str,"Veronica");break;
      case 330:strcat(str,"Virginia");break;
      case 331:strcat(str,"Vicki");break;
      case 332:strcat(str,"Vicky");break;
      case 333:strcat(str,"Victoria");break;
      case 334:strcat(str,"Violet");break;
      case 335:strcat(str,"Vithara");break;
      case 336:strcat(str,"Vonnita");break;
      case 337:strcat(str,"Wanda");break;
      case 338:strcat(str,"Wendy");break;
      case 339:strcat(str,"Willow");break;
      case 340:strcat(str,"Wilma");break;
      case 341:strcat(str,"Xenia");break;
      case 342:strcat(str,"Yoko");break;
      case 343:strcat(str,"Yuri");break;
      case 344:strcat(str,"Yvonne");break;
      case 345:strcat(str,"Zoe");break;
//}}}
      default:strcat(str,"Defaulta");break;
      }
   }
//}}}
//{{{ Neutral
   else if(nametable == GENDER_NEUTRAL)
   {
      switch(roll)
      {
         //{{{ Gender-neutral name table
      case 0:strcat(str,"Akira");break;
      case 1:strcat(str,"Alex");break;
      case 2:strcat(str,"Alexis");break;
      case 3:strcat(str,"Angel");break;
      case 4:strcat(str,"Ariel");break;
      case 5:strcat(str,"Ashley");break;
      case 6:strcat(str,"Billy");break;
      case 7:strcat(str,"Frankie");break;
      case 8:strcat(str,"Hillary");break;
      case 9:strcat(str,"Kelly");break;
      case 10:strcat(str,"Jackie");break;
      case 11:strcat(str,"Jamie");break;
      case 12:strcat(str,"Jo");break;
      case 13:strcat(str,"Jun");break;
      case 14:strcat(str,"Kelsey");break;
      case 15:strcat(str,"Kelsie");break;
      case 16:strcat(str,"Kim");break;
      case 17:strcat(str,"Lesley");break;
      case 18:strcat(str,"Leslie");break;
      case 19:strcat(str,"Morgan");break;
      case 20:strcat(str,"Nicky");break;
      case 21:strcat(str,"Noel");break;
      case 22:strcat(str,"Page");break;
      case 23:strcat(str,"Paige");break;
      case 24:strcat(str,"Pat");break;
      case 25:strcat(str,"Raven");break;
      case 26:strcat(str,"Riley");break;
      case 27:strcat(str,"Robin");break;
      case 28:strcat(str,"Sam");break;
      case 29:strcat(str,"Sandy");break;
      case 30:strcat(str,"Sidney");break;
      case 31:strcat(str,"Taylor");break;
      case 32:strcat(str,"Terry");break;
      case 33:strcat(str,"Yanping");break;
//}}}

      default:strcat(str,"Default");break;
      }
   }
//}}}
   else if(nametable == GENDER_WHITEMALEPATRIARCH)
//{{{ White Male Patriarchs
   {
      switch(roll)
      {
      //{{{ White Male Patriarch
      case 0:strcat(str,"Roger");break;
      case 1:strcat(str,"James");break;
      case 2:strcat(str,"John");break;
      case 3:strcat(str,"Bob");break;
      case 4:strcat(str,"Mike");break;
      case 5:strcat(str,"Bill");break;
      case 6:strcat(str,"Dave");break;
      case 7:strcat(str,"Rick");break;
      case 8:strcat(str,"Colin");break;
      case 9:strcat(str,"Joe");break;
      case 10:strcat(str,"Tom");break;
      case 11:strcat(str,"Chris");break;
      case 12:strcat(str,"Dan");break;
      case 13:strcat(str,"Paul");break;
      case 14:strcat(str,"Mark");break;
      case 15:strcat(str,"Donald");break;
      case 16:strcat(str,"Jim");break;
      case 17:strcat(str,"Ken");break;
      case 18:strcat(str,"Steve");break;
      case 19:strcat(str,"Edward");break;
      case 20:strcat(str,"Brian");break;
      case 21:strcat(str,"Ron");break;
      case 22:strcat(str,"Tony");break;
      case 23:strcat(str,"Kevin");break;
      case 24:strcat(str,"Jason");break;
      case 25:strcat(str,"Matt");break;
      case 26:strcat(str,"Gary");break;
      case 27:strcat(str,"Tim");break;
      case 28:strcat(str,"Alan");break;
      case 29:strcat(str,"Larry");break;
      case 30:strcat(str,"Jeff");break;
      case 31:strcat(str,"Frank");break;
      case 32:strcat(str,"Scott");break;
      case 33:strcat(str,"Eric");break;
      case 34:strcat(str,"Steve");break;
      case 35:strcat(str,"Andy");break;
      case 36:strcat(str,"Ray");break;
      case 37:strcat(str,"Greg");break;
      case 38:strcat(str,"Josh");break;
      case 39:strcat(str,"Jerry");break;
      case 40:strcat(str,"Dennis");break;
      case 41:strcat(str,"Walter");break;
      case 42:strcat(str,"Jesse");break;
      case 43:strcat(str,"Peter");break;
      case 44:strcat(str,"Harry");break;
      case 45:strcat(str,"Doug");break;
      case 46:strcat(str,"Henry");break;
      case 47:strcat(str,"Carl");break;
      case 48:strcat(str,"Arthur");break;
      case 49:strcat(str,"Ryan");break;//}}}
      }
   }
}


//}}}
/* gets a random last name */
void lastname(char *str)
//{{{ Last Name
{
   strcpy(str,"");

   switch(LCSrandom(830))
   {
      //{{{ Last name table
      case 0:strcat(str,"Abbey");break;
      case 1:strcat(str,"Abbott");break;
      case 2:strcat(str,"Abbzug");break;
      case 3:strcat(str,"Adamson");break;
      case 4:strcat(str,"Adkins");break;
      case 5:strcat(str,"Africa");break;
      case 6:strcat(str,"Aguilera");break;
      case 7:strcat(str,"Alanson");break;
      case 8:strcat(str,"Alcott");break;
      case 9:strcat(str,"Alda");break;
      case 10:strcat(str,"Alexander");break;
      case 11:strcat(str,"Allanshaw");break;
      case 12:strcat(str,"Allen");break;
      case 13:strcat(str,"Alley");break;
      case 14:strcat(str,"Ames");break;
      case 15:strcat(str,"Anderson");break;
      case 16:strcat(str,"Andress");break;
      case 17:strcat(str,"Antle");break;
      case 18:strcat(str,"Appleton");break;
      case 19:strcat(str,"Armstead");break;
      case 20:strcat(str,"Armstrong");break;
      case 21:strcat(str,"Arnaz");break;
      case 22:strcat(str,"Artfield");break;
      case 23:strcat(str,"Ashmore");break;
      case 24:strcat(str,"Ayers");break;
      case 25:strcat(str,"Aykroyd");break;
      case 26:strcat(str,"Bacall");break;
      case 27:strcat(str,"Bailey");break;
      case 28:strcat(str,"Baird");break;
      case 29:strcat(str,"Baker");break;
      case 30:strcat(str,"Baldwin");break;
      case 31:strcat(str,"Balgos");break;
      case 32:strcat(str,"Barr");break;
      case 33:strcat(str,"Baranyi");break;
      case 34:strcat(str,"Barrow");break;
      case 35:strcat(str,"Barwick");break;
      case 36:strcat(str,"Bates");break;
      case 37:strcat(str,"Baxter");break;
      case 38:strcat(str,"Beaumont");break;
      case 39:strcat(str,"Bendix");break;
      case 40:strcat(str,"Bennett");break;
      case 41:strcat(str,"Bezos");break;
      case 42:strcat(str,"Birkin");break;
      case 43:strcat(str,"Black");break;
      case 44:strcat(str,"Blackenship");break;
      case 45:strcat(str,"Blanchard");break;
      case 46:strcat(str,"Blitzer");break;
      case 47:strcat(str,"Blum");break;
      case 48:strcat(str,"Bogart");break;
      case 49:strcat(str,"Boggs");break;
      case 50:strcat(str,"Bolger");break;
      case 51:strcat(str,"Booker");break;
      case 52:strcat(str,"Bosanquet");break;
      case 53:strcat(str,"Boyd");break;
      case 54:strcat(str,"Bradbury");break;
      case 55:strcat(str,"Bradley");break;
      case 56:strcat(str,"Bradshaw");break;
      case 57:strcat(str,"Brandis");break;
      case 58:strcat(str,"Bremer");break;
      case 59:strcat(str,"Brewster");break;
      case 60:strcat(str,"Brightman");break;
      case 61:strcat(str,"Briscoe");break;
      case 62:strcat(str,"Brooks");break;
      case 63:strcat(str,"Brown");break;
      case 64:strcat(str,"Brownlee");break;
      case 65:strcat(str,"Bryant");break;
      case 66:strcat(str,"Buchanan");break;
      case 67:strcat(str,"Buckley");break;
      case 68:strcat(str,"Buckman");break;
      case 69:strcat(str,"Buckner");break;
      case 70:strcat(str,"Bugbee");break;
      case 71:strcat(str,"Bump");break;
      case 72:strcat(str,"Bundy");break;
      case 73:strcat(str,"Bunton");break;
      case 74:strcat(str,"Burghoff");break;
      case 75:strcat(str,"Burton");break;
      case 76:strcat(str,"Bush");break;
      case 77:strcat(str,"Butcher");break;
      case 78:strcat(str,"Butler");break;
      case 79:strcat(str,"Butt");break;
      case 80:strcat(str,"Butterman");break;
      case 81:strcat(str,"Buttman");break;
      case 82:strcat(str,"Byrd");break;
      case 83:strcat(str,"Callahan");break;
      case 84:strcat(str,"Campbell");break;
      case 85:strcat(str,"Carberry");break;
      case 86:strcat(str,"Carey");break;
      case 87:strcat(str,"Carpenter");break;
      case 88:strcat(str,"Carr");break;
      case 89:strcat(str,"Carson");break;
      case 90:strcat(str,"Cartwright");break;
      case 91:strcat(str,"Cavallero");break;
      case 92:strcat(str,"Cavill");break;
      case 93:strcat(str,"Chaffee");break;
      case 94:strcat(str,"Chamberlain");break;
      case 95:strcat(str,"Chambers");break;
      case 96:strcat(str,"Chandler");break;
      case 97:strcat(str,"Channing");break;
      case 98:strcat(str,"Cheney");break;
      case 99:strcat(str,"Cheng");break;
      case 100:strcat(str,"Childress");break;
      case 101:strcat(str,"Childs");break;
      case 102:strcat(str,"Chilton");break;
      case 103:strcat(str,"Cho");break;
      case 104:strcat(str,"Chubb");break;
      case 105:strcat(str,"Churchill");break;
      case 106:strcat(str,"Clavin");break;
      case 107:strcat(str,"Clay");break;
      case 108:strcat(str,"Clinton");break;
      case 109:strcat(str,"Cobb");break;
      case 110:strcat(str,"Coffey");break;
      case 111:strcat(str,"Cohen");break;
      case 112:strcat(str,"Colcord");break;
      case 113:strcat(str,"Cole");break;
      case 114:strcat(str,"Coleman");break;
      case 115:strcat(str,"Collins");break;
      case 116:strcat(str,"Colombe");break;
      case 117:strcat(str,"Colt");break;
      case 118:strcat(str,"Conner");break;
      case 119:strcat(str,"Cook");break;
      case 120:strcat(str,"Cooper");break;
      case 121:strcat(str,"Coopersmith");break;
      case 122:strcat(str,"Copeland");break;
      case 123:strcat(str,"Coppola");break;
      case 124:strcat(str,"Cordero");break;
      case 125:strcat(str,"Cosgrove");break;
      case 126:strcat(str,"Cottrell");break;
      case 127:strcat(str,"Court");break;
      case 128:strcat(str,"Cox");break;
      case 129:strcat(str,"Craft");break;
      case 130:strcat(str,"Crane");break;
      case 131:strcat(str,"Critchley");break;
      case 132:strcat(str,"Crockett");break;
      case 133:strcat(str,"Croft");break;
      case 134:strcat(str,"Cromwell");break;
      case 135:strcat(str,"Cross");break;
      case 136:strcat(str,"Crowe");break;
      case 137:strcat(str,"Crowson");break;
      case 138:strcat(str,"de la Cruz");break;
      case 139:strcat(str,"Cunningham");break;
      case 140:strcat(str,"Curnew");break;
      case 141:strcat(str,"Cutlass");break;
      case 142:strcat(str,"Cutler");break;
      case 143:strcat(str,"Dahmer");break;
      case 144:strcat(str,"Danes");break;
      case 145:strcat(str,"Danson");break;
      case 146:strcat(str,"Dante");break;
      case 147:strcat(str,"Darby");break;
      case 148:strcat(str,"Darnand");break;
      case 149:strcat(str,"Dash");break;
      case 150:strcat(str,"Daveson");break;
      case 151:strcat(str,"Davidson");break;
      case 152:strcat(str,"Davis");break;
      case 153:strcat(str,"Day");break;
      case 154:strcat(str,"Decker");break;
      case 155:strcat(str,"Deere");break;
      case 156:strcat(str,"Delaney");break;
      case 157:strcat(str,"Delgado");break;
      case 158:strcat(str,"DeLorean");break;
      case 159:strcat(str,"Dench");break;
      case 160:strcat(str,"Dent");break;
      case 161:strcat(str,"Dentz");break;
      case 162:strcat(str,"Diaz");break;
      case 163:strcat(str,"Diehl");break;
      case 164:strcat(str,"DiMeo");break;
      case 165:strcat(str,"Dole");break;
      case 166:strcat(str,"Dorgan");break;
      case 167:strcat(str,"Dorsey");break;
      case 168:strcat(str,"Doucette");break;
      case 169:strcat(str,"Downey");break;
      case 170:strcat(str,"Downing");break;
      case 171:strcat(str,"Doyle");break;
      case 172:strcat(str,"Drake");break;
      case 173:strcat(str,"Duell");break;
      case 174:strcat(str,"Duffield");break;
      case 175:strcat(str,"Duke");break;
      case 176:strcat(str,"Dunham");break;
      case 177:strcat(str,"Dunlap");break;
      case 178:strcat(str,"Duval");break;
      case 179:strcat(str,"Earle");break;
      case 180:strcat(str,"English");break;
      case 181:strcat(str,"Escobar");break;
      case 182:strcat(str,"Ericson");break;
      case 183:strcat(str,"Eubanks");break;
      case 184:strcat(str,"Evans");break;
      case 185:strcat(str,"al Fadil");break;
      case 186:strcat(str,"Fairbanks");break;
      case 187:strcat(str,"Fairchild");break;
      case 188:strcat(str,"Falco");break;
      case 189:strcat(str,"Farnsworth");break;
      case 190:strcat(str,"Farr");break;
      case 191:strcat(str,"Faulkner");break;
      case 192:strcat(str,"Fenoglio");break;
      case 193:strcat(str,"Fiddlebottom");break;
      case 194:strcat(str,"Fields");break;
      case 195:strcat(str,"Filler");break;
      case 196:strcat(str,"Fimple");break;
      case 197:strcat(str,"Fingleton");break;
      case 198:strcat(str,"Finn");break;
      case 199:strcat(str,"Fisher");break;
      case 200:strcat(str,"Fitzgerald");break;
      case 201:strcat(str,"Fitzpatrick");break;
      case 202:strcat(str,"Flanders");break;
      case 203:strcat(str,"Fletcher");break;
      case 204:strcat(str,"Florez");break;
      case 205:strcat(str,"Fortmuller");break;
      case 206:strcat(str,"Ford");break;
      case 207:strcat(str,"Foster");break;
      case 208:strcat(str,"Fox");break;
      case 209:strcat(str,"France");break;
      case 210:strcat(str,"Franklin");break;
      case 211:strcat(str,"Frost");break;
      case 212:strcat(str,"Friend");break;
      case 213:strcat(str,"Fudge");break;
      case 214:strcat(str,"Fulsom");break;
      case 215:strcat(str,"Furley");break;
      case 216:strcat(str,"Fushimi");break;
      case 217:strcat(str,"Gacy");break;
      case 218:strcat(str,"Galindo");break;
      case 219:strcat(str,"Garcia");break;
      case 220:strcat(str,"Garnier");break;
      case 221:strcat(str,"Gates");break;
      case 222:strcat(str,"Gatien");break;
      case 223:strcat(str,"Gelbart");break;
      case 224:strcat(str,"Getty");break;
      case 225:strcat(str,"Gilbert");break;
      case 226:strcat(str,"Gillmouth");break;
      case 227:strcat(str,"Gilpin");break;
      case 228:strcat(str,"Godwin");break;
      case 229:strcat(str,"Gleaves");break;
      case 230:strcat(str,"Goldberg");break;
      case 231:strcat(str,"Goldstein");break;
      case 232:strcat(str,"Goode");break;
      case 233:strcat(str,"Goodyear");break;
      case 234:strcat(str,"Gore");break;
      case 235:strcat(str,"Govorov");break;
      case 236:strcat(str,"Grammer");break;
      case 237:strcat(str,"Grant");break;
      case 238:strcat(str,"Gray");break;
      case 239:strcat(str,"Green");break;
      case 240:strcat(str,"Greenberg");break;
      case 241:strcat(str,"Gregg");break;
      case 242:strcat(str,"Grover");break;
      case 243:strcat(str,"Gu");break;
      case 244:strcat(str,"Guy");break;
      case 245:strcat(str,"Hall");break;
      case 246:strcat(str,"Hamilton");break;
      case 247:strcat(str,"Hammond");break;
      case 248:strcat(str,"Hann");break;
      case 249:strcat(str,"Hannemann");break;
      case 250:strcat(str,"Hapsburg");break;
      case 251:strcat(str,"Harper");break;
      case 252:strcat(str,"Harris");break;
      case 253:strcat(str,"Hatcher");break;
      case 254:strcat(str,"Hawking");break;
      case 255:strcat(str,"Haymer");break;
      case 256:strcat(str,"Headley");break;
      case 257:strcat(str,"Heath");break;
      case 258:strcat(str,"Heller");break;
      case 259:strcat(str,"Hellmann");break;
      case 260:strcat(str,"Helton");break;
      case 261:strcat(str,"Hemingway");break;
      case 262:strcat(str,"Henderson");break;
      case 263:strcat(str,"Hendren");break;
      case 264:strcat(str,"Hendrix");break;
      case 265:strcat(str,"Hepburn");break;
      case 266:strcat(str,"Hepditch");break;
      case 267:strcat(str,"Hewson");break;
      case 268:strcat(str,"Heyduke");break;
      case 269:strcat(str,"Hiaasen");break;
      case 270:strcat(str,"Hill");break;
      case 271:strcat(str,"Hilliard");break;
      case 272:strcat(str,"Himelstein");break;
      case 273:strcat(str,"Hoddinott");break;
      case 274:strcat(str,"Hoffmann");break;
      case 275:strcat(str,"Hogan");break;
      case 276:strcat(str,"Holder");break;
      case 277:strcat(str,"Hooker");break;
      case 278:strcat(str,"Hoover");break;
      case 279:strcat(str,"Hope");break;
      case 280:strcat(str,"Hopkins");break;
      case 281:strcat(str,"Hopper");break;
      case 282:strcat(str,"Hornberger");break;
      case 283:strcat(str,"Houghton");break;
      case 284:strcat(str,"Houseman");break;
      case 285:strcat(str,"Howard");break;
      case 286:strcat(str,"Howe");break;
      case 287:strcat(str,"Hudson");break;
      case 288:strcat(str,"Hughes");break;
      case 289:strcat(str,"Hunnicutt");break;
      case 290:strcat(str,"Hutchison");break;
      case 291:strcat(str,"Ibarra");break;
      case 292:strcat(str,"Incledon");break;
      case 293:strcat(str,"Inking");break;
      case 294:strcat(str,"Irvin");break;
      case 295:strcat(str,"Irving");break;
      case 296:strcat(str,"Issacson");break;
      case 297:strcat(str,"Ivanson");break;
      case 298:strcat(str,"Ives");break;
      case 299:strcat(str,"Jackson");break;
      case 300:strcat(str,"Jameson");break;
      case 301:strcat(str,"Jefferson");break;
      case 302:strcat(str,"Jenkins");break;
      case 303:strcat(str,"Jimison");break;
      case 304:strcat(str,"Johnson");break;
      case 305:strcat(str,"Jones");break;
      case 306:strcat(str,"Jordan");break;
      case 307:strcat(str,"Judge");break;
      case 308:strcat(str,"Justice");break;
      case 309:strcat(str,"Kaltenbrunner");break;
      case 310:strcat(str,"Keaton");break;
      case 311:strcat(str,"Keats");break;
      case 312:strcat(str,"Keene");break;
      case 313:strcat(str,"Keitel");break;
      case 314:strcat(str,"Kent");break;
      case 315:strcat(str,"Kershaw");break;
      case 316:strcat(str,"Kesten");break;
      case 317:strcat(str,"Kilbourne");break;
      case 318:strcat(str,"Kindale");break;
      case 319:strcat(str,"King");break;
      case 320:strcat(str,"Kinslow");break;
      case 321:strcat(str,"Kitchen");break;
      case 322:strcat(str,"Kokernot");break;
      case 323:strcat(str,"Koprowski");break;
      case 324:strcat(str,"Krasow");break;
      case 325:strcat(str,"Krieger");break;
      case 326:strcat(str,"Krishna");break;
      case 327:strcat(str,"Kunstler");break;
      case 328:strcat(str,"Kurosawa");break;
      case 329:strcat(str,"Langley");break;
      case 330:strcat(str,"LaSaga");break;
      case 331:strcat(str,"Lauder");break;
      case 332:strcat(str,"Lee");break;
      case 333:strcat(str,"Leeves");break;
      case 334:strcat(str,"Lewis");break;
      case 335:strcat(str,"Leyden");break;
      case 336:strcat(str,"Li");break;
      case 337:strcat(str,"Lincoln");break;
      case 338:strcat(str,"Linville");break;
      case 339:strcat(str,"Lipman");break;
      case 340:strcat(str,"Little");break;
      case 341:strcat(str,"Loeb");break;
      case 342:strcat(str,"Logan");break;
      case 343:strcat(str,"Logsdon");break;
      case 344:strcat(str,"Lombard");break;
      case 345:strcat(str,"London");break;
      case 346:strcat(str,"Lopez");break;
      case 347:strcat(str,"Lovecraft");break;
      case 348:strcat(str,"Lowcourt");break;
      case 349:strcat(str,"Lowe");break;
      case 350:strcat(str,"Lowerly");break;
      case 351:strcat(str,"Lucas");break;
      case 352:strcat(str,"Luna");break;
      case 353:strcat(str,"Lushington");break;
      case 354:strcat(str,"Luzinski");break;
      case 355:strcat(str,"Lyles");break;
      case 356:strcat(str,"Lynam");break;
      case 357:strcat(str,"Lynch");break;
      case 358:strcat(str,"Van Lynden");break;
      case 359:strcat(str,"Lyons");break;
      case 360:strcat(str,"MacGyver");break;
      case 361:strcat(str,"MacKinley");break;
      case 362:strcat(str,"MacKinnon");break;
      case 363:strcat(str,"MacLeod");break;
      case 364:strcat(str,"MacNeil");break;
      case 365:strcat(str,"Macy");break;
      case 366:strcat(str,"Mahoney");break;
      case 367:strcat(str,"Malone");break;
      case 368:strcat(str,"Mann");break;
      case 369:strcat(str,"Manning");break;
      case 370:strcat(str,"Mansfield");break;
      case 371:strcat(str,"March");break;
      case 372:strcat(str,"Marsden");break;
      case 373:strcat(str,"Marsh");break;
      case 374:strcat(str,"Marshall");break;
      case 375:strcat(str,"Matheson");break;
      case 376:strcat(str,"Mathewson");break;
      case 377:strcat(str,"Maxwell");break;
      case 378:strcat(str,"McAdams");break;
      case 379:strcat(str,"McAllister");break;
      case 380:strcat(str,"McArthur");break;
      case 381:strcat(str,"McBroom");break;
      case 382:strcat(str,"McCain");break;
      case 383:strcat(str,"McCarthy");break;
      case 384:strcat(str,"McCorkell");break;
      case 385:strcat(str,"McDonald");break;
      case 386:strcat(str,"McFadden");break;
      case 387:strcat(str,"McFatridge");break;
      case 388:strcat(str,"McFaul");break;
      case 389:strcat(str,"McGee");break;
      case 390:strcat(str,"McGovern");break;
      case 391:strcat(str,"McGrath");break;
      case 392:strcat(str,"McGraw");break;
      case 393:strcat(str,"McGuinness");break;
      case 394:strcat(str,"McIntosh");break;
      case 395:strcat(str,"McIntyre");break;
      case 396:strcat(str,"McKenzie");break;
      case 397:strcat(str,"McLaughlin");break;
      case 398:strcat(str,"McLeod");break;
      case 399:strcat(str,"McMurray");break;
      case 400:strcat(str,"McNabb");break;
      case 401:strcat(str,"McPhail");break;
      case 402:strcat(str,"McPherson");break;
      case 403:strcat(str,"McQueen");break;
      case 404:strcat(str,"McVeigh");break;
      case 405:strcat(str,"Meek");break;
      case 406:strcat(str,"Meffert");break;
      case 407:strcat(str,"Melendez");break;
      case 408:strcat(str,"Metzger");break;
      case 409:strcat(str,"Mifune");break;
      case 410:strcat(str,"Milburn");break;
      case 411:strcat(str,"Miller");break;
      case 412:strcat(str,"Mitchell");break;
      case 413:strcat(str,"Miyamoto");break;
      case 414:strcat(str,"Monroe");break;
      case 415:strcat(str,"Montag");break;
      case 416:strcat(str,"Montalvo");break;
      case 417:strcat(str,"Montana");break;
      case 418:strcat(str,"Moon");break;
      case 419:strcat(str,"Morris");break;
      case 420:strcat(str,"Morrow");break;
      case 421:strcat(str,"Morton");break;
      case 422:strcat(str,"Mosley");break;
      case 423:strcat(str,"Muhammed");break;
      case 424:strcat(str,"Murphy");break;
      case 425:strcat(str,"Murray");break;
      case 426:strcat(str,"Murrell");break;
      case 427:strcat(str,"Muscat");break;
      case 428:strcat(str,"Muskhelishvili");break;
      case 429:strcat(str,"Mussa");break;
      case 430:strcat(str,"Myatt");break;
      case 431:strcat(str,"Myers");break;
      case 432:strcat(str,"Nagel");break;
      case 433:strcat(str,"Nagelberg");break;
      case 434:strcat(str,"Nakagawa");break;
      case 435:strcat(str,"Nakahara");break;
      case 436:strcat(str,"Nallet");break;
      case 437:strcat(str,"Nance");break;
      case 438:strcat(str,"Nash");break;
      case 439:strcat(str,"Nathanson");break;
      case 440:strcat(str,"Necker");break;
      case 441:strcat(str,"Nelson");break;
      case 442:strcat(str,"Ness");break;
      case 443:strcat(str,"Nettles");break;
      case 444:strcat(str,"Neuwirth");break;
      case 445:strcat(str,"Nevinson");break;
      case 446:strcat(str,"Newberry");break;
      case 447:strcat(str,"Newhook");break;
      case 448:strcat(str,"Newman");break;
      case 449:strcat(str,"Ng");break;
      case 450:strcat(str,"Nicolle");break;
      case 451:strcat(str,"Nickson");break;
      case 452:strcat(str,"Nielson");break;
      case 453:strcat(str,"Nixon");break;
      case 454:strcat(str,"North");break;
      case 455:strcat(str,"Oakley");break;
      case 456:strcat(str,"Obama");break;
      case 457:strcat(str,"Obeid");break;
      case 458:strcat(str,"Oberlander");break;
      case 459:strcat(str,"Ochs");break;
      case 460:strcat(str,"Ogden");break;
      case 461:strcat(str,"Olde");break;
      case 462:strcat(str,"Olds");break;
      case 463:strcat(str,"Oldsmar");break;
      case 464:strcat(str,"Ollerenshaw");break;
      case 465:strcat(str,"Orlando");break;
      case 466:strcat(str,"Ortiz");break;
      case 467:strcat(str,"Paoletti");break;
      case 468:strcat(str,"Palm");break;
      case 469:strcat(str,"Palmer");break;
      case 470:strcat(str,"Parker");break;
      case 471:strcat(str,"Parking");break;
      case 472:strcat(str,"Parr");break;
      case 473:strcat(str,"Patterson");break;
      case 474:strcat(str,"Pearson");break;
      case 475:strcat(str,"Peck");break;
      case 476:strcat(str,"Pegg");break;
      case 477:strcat(str,"Pelton");break;
      case 478:strcat(str,"Pendergrass");break;
      case 479:strcat(str,"Pendleton");break;
      case 480:strcat(str,"Perez");break;
      case 481:strcat(str,"Pert");break;
      case 482:strcat(str,"Peterson");break;
      case 483:strcat(str,"Petermann");break;
      case 484:strcat(str,"Pickett");break;
      case 485:strcat(str,"Pierce");break;
      case 486:strcat(str,"Piercey");break;
      case 487:strcat(str,"Pommerencke");break;
      case 488:strcat(str,"Pomper");break;
      case 489:strcat(str,"Porcaro");break;
      case 490:strcat(str,"Porter");break;
      case 491:strcat(str,"Postman");break;
      case 492:strcat(str,"Potter");break;
      case 493:strcat(str,"Pound");break;
      case 494:strcat(str,"Powers");break;
      case 495:strcat(str,"Presley");break;
      case 496:strcat(str,"Probst");break;
      case 497:strcat(str,"Proudfoot");break;
      case 498:strcat(str,"Pryor");break;
      case 499:strcat(str,"Puente");break;
      case 500:strcat(str,"Purcell");break;
      case 501:strcat(str,"Queen");break;
      case 502:strcat(str,"Quennell");break;
      case 503:strcat(str,"Quick");break;
      case 504:strcat(str,"Quilico");break;
      case 505:strcat(str,"Quilliam");break;
      case 506:strcat(str,"Quilty");break;
      case 507:strcat(str,"Quincey");break;
      case 508:strcat(str,"Quinlivan");break;
      case 509:strcat(str,"Quinn");break;
      case 510:strcat(str,"Quirk");break;
      case 511:strcat(str,"Rabban");break;
      case 512:strcat(str,"Rackard");break;
      case 513:strcat(str,"Radman");break;
      case 514:strcat(str,"Rainey");break;
      case 515:strcat(str,"Ramirez");break;
      case 516:strcat(str,"Rand");break;
      case 517:strcat(str,"Randell");break;
      case 518:strcat(str,"Ransom");break;
      case 519:strcat(str,"Rappaport");break;
      case 520:strcat(str,"Raskin");break;
      case 521:strcat(str,"Rastrelli");break;
      case 522:strcat(str,"Rather");break;
      case 523:strcat(str,"Ratzenberger");break;
      case 524:strcat(str,"Ravenhill");break;
      case 525:strcat(str,"Rawls");break;
      case 526:strcat(str,"Read");break;
      case 527:strcat(str,"Reading");break;
      case 528:strcat(str,"Reagan");break;
      case 529:strcat(str,"Reasoner");break;
      case 530:strcat(str,"Van Rechteren");break;
      case 531:strcat(str,"Redford");break;
      case 532:strcat(str,"Redman");break;
      case 533:strcat(str,"Redwine");break;
      case 534:strcat(str,"Reed");break;
      case 535:strcat(str,"Reekers");break;
      case 536:strcat(str,"Rendell");break;
      case 537:strcat(str,"Rexford");break;
      case 538:strcat(str,"Reynolds");break;
      case 539:strcat(str,"Ricci");break;
      case 540:strcat(str,"Richardson");break;
      case 541:strcat(str,"Richter");break;
      case 542:strcat(str,"Rideout");break;
      case 543:strcat(str,"Riffle");break;
      case 544:strcat(str,"Robinson");break;
      case 545:strcat(str,"Rodham");break;
      case 546:strcat(str,"Rojo");break;
      case 547:strcat(str,"Roland");break;
      case 548:strcat(str,"Roosevelt");break;
      case 549:strcat(str,"Rose");break;
      case 550:strcat(str,"Rosenberg");break;
      case 551:strcat(str,"Ross");break;
      case 552:strcat(str,"Rota");break;
      case 553:strcat(str,"Rothstein");break;
      case 554:strcat(str,"Roy");break;
      case 555:strcat(str,"Roylance");break;
      case 556:strcat(str,"Rudkin");break;
      case 557:strcat(str,"Rush");break;
      case 558:strcat(str,"Rushworth");break;
      case 559:strcat(str,"la Russa");break;
      case 560:strcat(str,"Russell");break;
      case 561:strcat(str,"Rutherford");break;
      case 562:strcat(str,"Ryman");break;
      case 563:strcat(str,"Sabella");break;
      case 564:strcat(str,"Saddler");break;
      case 565:strcat(str,"Sadler");break;
      case 566:strcat(str,"Sansome");break;
      case 567:strcat(str,"Santiago");break;
      case 568:strcat(str,"Santos");break;
      case 569:strcat(str,"Savedra");break;
      case 570:strcat(str,"Sawyer");break;
      case 571:strcat(str,"Saxton");break;
      case 572:strcat(str,"Scales");break;
      case 573:strcat(str,"Scardino");break;
      case 574:strcat(str,"Schmitt");break;
      case 575:strcat(str,"Schumacher");break;
      case 576:strcat(str,"Schuster");break;
      case 577:strcat(str,"Scott");break;
      case 578:strcat(str,"Scully");break;
      case 579:strcat(str,"Scurfield");break;
      case 580:strcat(str,"Sealock");break;
      case 581:strcat(str,"Seay");break;
      case 582:strcat(str,"Sellers");break;
      case 583:strcat(str,"Semin");break;
      case 584:strcat(str,"Serling");break;
      case 585:strcat(str,"Sewall");break;
      case 586:strcat(str,"Sexton");break;
      case 587:strcat(str,"Shackleton");break;
      case 588:strcat(str,"Shagwell");break;
      case 589:strcat(str,"al Shamie");break;
      case 590:strcat(str,"Shanahan");break;
      case 591:strcat(str,"Sharpe");break;
      case 592:strcat(str,"Shaw");break;
      case 593:strcat(str,"Shedd");break;
      case 594:strcat(str,"Shelby");break;
      case 595:strcat(str,"Shelton");break;
      case 596:strcat(str,"Shengde");break;
      case 597:strcat(str,"Shepard");break;
      case 598:strcat(str,"Shepardson");break;
      case 599:strcat(str,"Sheppard");break;
      case 600:strcat(str,"Sherman");break;
      case 601:strcat(str,"Shew");break;
      case 602:strcat(str,"Shields");break;
      case 603:strcat(str,"Shiller");break;
      case 604:strcat(str,"Shimazu");break;
      case 605:strcat(str,"Shipman");break;
      case 606:strcat(str,"Shipp");break;
      case 607:strcat(str,"Sho");break;
      case 608:strcat(str,"Short");break;
      case 609:strcat(str,"Shrapnal");break;
      case 610:strcat(str,"Shrum");break;
      case 611:strcat(str,"Shumway");break;
      case 612:strcat(str,"Shurin");break;
      case 613:strcat(str,"Shuttleworth");break;
      case 614:strcat(str,"Sidebottom");break;
      case 615:strcat(str,"Siegel");break;
      case 616:strcat(str,"Siever");break;
      case 617:strcat(str,"da Silva");break;
      case 618:strcat(str,"Silverman");break;
      case 619:strcat(str,"Silverstein");break;
      case 620:strcat(str,"Simmons");break;
      case 621:strcat(str,"Simms");break;
      case 622:strcat(str,"Simon");break;
      case 623:strcat(str,"Simpson");break;
      case 624:strcat(str,"Skidmore");break;
      case 625:strcat(str,"Skinner");break;
      case 626:strcat(str,"Skirrow");break;
      case 627:strcat(str,"Skutnik");break;
      case 628:strcat(str,"Skye");break;
      case 629:strcat(str,"Skyler");break;
      case 630:strcat(str,"Slaten");break;
      case 631:strcat(str,"Smalley");break;
      case 632:strcat(str,"Smallwood");break;
      case 633:strcat(str,"Smart");break;
      case 634:strcat(str,"Smith");break;
      case 635:strcat(str,"Smithers");break;
      case 636:strcat(str,"Smyth");break;
      case 637:strcat(str,"Snead");break;
      case 638:strcat(str,"Snook");break;
      case 639:strcat(str,"Snow");break;
      case 640:strcat(str,"Snyder");break;
      case 641:strcat(str,"Sommer");break;
      case 642:strcat(str,"Somner");break;
      case 643:strcat(str,"Soren");break;
      case 644:strcat(str,"Spasov");break;
      case 645:strcat(str,"Spears");break;
      case 646:strcat(str,"Spencer");break;
      case 647:strcat(str,"Stark");break;
      case 648:strcat(str,"Starkweather");break;
      case 649:strcat(str,"Stelling");break;
      case 650:strcat(str,"Stephenson");break;
      case 651:strcat(str,"Sterling");break;
      case 652:strcat(str,"Sternin");break;
      case 653:strcat(str,"Stevenson");break;
      case 654:strcat(str,"Stockton");break;
      case 655:strcat(str,"Stone");break;
      case 656:strcat(str,"Storm");break;
      case 657:strcat(str,"Storms");break;
      case 658:strcat(str,"Stiver");break;
      case 659:strcat(str,"Strain");break;
      case 660:strcat(str,"Straley");break;
      case 661:strcat(str,"Straub");break;
      case 662:strcat(str,"Strauss");break;
      case 663:strcat(str,"Stripe");break;
      case 664:strcat(str,"Strossen");break;
      case 665:strcat(str,"Suave");break;
      case 666:strcat(str,"Suiter");break;
      case 667:strcat(str,"Sulaiman");break;
      case 668:strcat(str,"Sullivan");break;
      case 669:strcat(str,"Surnow");break;
      case 670:strcat(str,"Susman");break;
      case 671:strcat(str,"Swain");break;
      case 672:strcat(str,"Swaine");break;
      case 673:strcat(str,"Swanson");break;
      case 674:strcat(str,"Sweeney");break;
      case 675:strcat(str,"Swift");break;
      case 676:strcat(str,"Swit");break;
      case 677:strcat(str,"Symington");break;
      case 678:strcat(str,"Symonds");break;
      case 679:strcat(str,"Tabb");break;
      case 680:strcat(str,"Tailboys");break;
      case 681:strcat(str,"Takayoshi");break;
      case 682:strcat(str,"Talbert");break;
      case 683:strcat(str,"Talbot");break;
      case 684:strcat(str,"Talbott");break;
      case 685:strcat(str,"Talty");break;
      case 686:strcat(str,"Tanner");break;
      case 687:strcat(str,"Tatters");break;
      case 688:strcat(str,"Taubman");break;
      case 689:strcat(str,"Taylor");break;
      case 690:strcat(str,"Tayo");break;
      case 691:strcat(str,"Teige");break;
      case 692:strcat(str,"Tell");break;
      case 693:strcat(str,"Templeton");break;
      case 694:strcat(str,"Tennent");break;
      case 695:strcat(str,"Tenney");break;
      case 696:strcat(str,"Tenpenny");break;
      case 697:strcat(str,"Terrill");break;
      case 698:strcat(str,"Testore");break;
      case 699:strcat(str,"Tetley");break;
      case 700:strcat(str,"Thalberg");break;
      case 701:strcat(str,"Thatcher");break;
      case 702:strcat(str,"Thomaz");break;
      case 703:strcat(str,"Thompson");break;
      case 704:strcat(str,"Thomson");break;
      case 705:strcat(str,"Thorne");break;
      case 706:strcat(str,"Threepwood");break;
      case 707:strcat(str,"Thurgood");break;
      case 708:strcat(str,"Thurlow");break;
      case 709:strcat(str,"Tickell");break;
      case 710:strcat(str,"Tillett");break;
      case 711:strcat(str,"Tillinghast");break;
      case 712:strcat(str,"Tillson");break;
      case 713:strcat(str,"Tillsley");break;
      case 714:strcat(str,"Timmonds");break;
      case 715:strcat(str,"Tippett");break;
      case 716:strcat(str,"Tisdall");break;
      case 717:strcat(str,"Tishman");break;
      case 718:strcat(str,"Titcomb");break;
      case 719:strcat(str,"Tobin");break;
      case 720:strcat(str,"Todd");break;
      case 721:strcat(str,"Todhunter");break;
      case 722:strcat(str,"Toft");break;
      case 723:strcat(str,"Tolbert");break;
      case 724:strcat(str,"Tolkien");break;
      case 725:strcat(str,"Tolson");break;
      case 726:strcat(str,"Tonelli");break;
      case 727:strcat(str,"Toogood");break;
      case 728:strcat(str,"Tooke");break;
      case 729:strcat(str,"Toole");break;
      case 730:strcat(str,"Toth");break;
      case 731:strcat(str,"Towne");break;
      case 732:strcat(str,"Townsend");break;
      case 733:strcat(str,"Trafford");break;
      case 734:strcat(str,"Trebek");break;
      case 735:strcat(str,"Truman");break;
      case 736:strcat(str,"Tubbs");break;
      case 737:strcat(str,"Tubman");break;
      case 738:strcat(str,"Tucker");break;
      case 739:strcat(str,"Turner");break;
      case 740:strcat(str,"Tyler");break;
      case 741:strcat(str,"Tymchyshyn");break;
      case 742:strcat(str,"Ulmer");break;
      case 743:strcat(str,"Underhill");break;
      case 744:strcat(str,"Underdown");break;
      case 745:strcat(str,"Underwood");break;
      case 746:strcat(str,"Updike");break;
      case 747:strcat(str,"Upton");break;
      case 748:strcat(str,"Usher");break;
      case 749:strcat(str,"Ussher");break;
      case 750:strcat(str,"Valdez");break;
      case 751:strcat(str,"Vallance");break;
      case 752:strcat(str,"Vance");break;
      case 753:strcat(str,"Vasilevsky");break;
      case 754:strcat(str,"Venus");break;
      case 755:strcat(str,"Villa");break;
      case 756:strcat(str,"Villanueva");break;
      case 757:strcat(str,"Viscuso");break;
      case 758:strcat(str,"Voland");break;
      case 759:strcat(str,"Vulcar");break;
      case 760:strcat(str,"Wade");break;
      case 761:strcat(str,"Wadsworth");break;
      case 762:strcat(str,"Walden");break;
      case 763:strcat(str,"Walker");break;
      case 764:strcat(str,"Wallace");break;
      case 765:strcat(str,"Walsh");break;
      case 766:strcat(str,"Walton");break;
      case 767:strcat(str,"Walwyn");break;
      case 768:strcat(str,"Wang");break;
      case 769:strcat(str,"Ward");break;
      case 770:strcat(str,"Warman");break;
      case 771:strcat(str,"Warner");break;
      case 772:strcat(str,"Warr");break;
      case 773:strcat(str,"Washington");break;
      case 774:strcat(str,"Weathers");break;
      case 775:strcat(str,"Webber");break;
      case 776:strcat(str,"Webster");break;
      case 777:strcat(str,"Weeks");break;
      case 778:strcat(str,"Weller");break;
      case 779:strcat(str,"Wendt");break;
      case 780:strcat(str,"West");break;
      case 781:strcat(str,"Westlake");break;
      case 782:strcat(str,"Weston");break;
      case 783:strcat(str,"Weygand");break;
      case 784:strcat(str,"White");break;
      case 785:strcat(str,"Whitehead");break;
      case 786:strcat(str,"Whitley");break;
      case 787:strcat(str,"Wickes");break;
      case 788:strcat(str,"Wilcox");break;
      case 789:strcat(str,"Willett");break;
      case 790:strcat(str,"Williams");break;
      case 791:strcat(str,"Williamson");break;
      case 792:strcat(str,"Willman");break;
      case 793:strcat(str,"Wilson");break;
      case 794:strcat(str,"Winchester");break;
      case 795:strcat(str,"Winkelman");break;
      case 796:strcat(str,"Winslow");break;
      case 797:strcat(str,"Winter");break;
      case 798:strcat(str,"Wiseman");break;
      case 799:strcat(str,"Wolfe");break;
      case 800:strcat(str,"Wong");break;
      case 801:strcat(str,"Wood");break;
      case 802:strcat(str,"Woodrow");break;
      case 803:strcat(str,"Wright");break;
      case 804:strcat(str,"Wuornos");break;
      case 805:strcat(str,"Stassen");break;
      case 806:strcat(str,"Xue");break;
      case 807:strcat(str,"Yamaguchi");break;
      case 808:strcat(str,"Yates");break;
      case 809:strcat(str,"Yee");break;
      case 810:strcat(str,"Yerkovich");break;
      case 811:strcat(str,"Yim");break;
      case 812:strcat(str,"Young");break;
      case 813:strcat(str,"Youngblood");break;
      case 814:strcat(str,"Yue");break;
      case 815:strcat(str,"Zachary");break;
      case 816:strcat(str,"Zadora");break;
      case 817:strcat(str,"Zakharov");break;
      case 818:strcat(str,"Zale");break;
      case 819:strcat(str,"Zedner");break;
      case 820:strcat(str,"Zhang");break;
      case 821:strcat(str,"Zicree");break;
      case 822:strcat(str,"Zimmermann");break;
      case 823:strcat(str,"Zito");break;
      case 824:strcat(str,"Zlotoff");break;
      case 825:strcat(str,"Zondervan");break;
      case 826:strcat(str,"Zucker");break;
      case 827:strcat(str,"Zumbach");break;
      case 828:strcat(str,"Starr");break;
//}}}

      default:strcat(str,"Errorman");break;
   }
}
//}}}



/* ensures that the creature's work location is appropriate to its type */
bool verifyworklocation(Creature &cr, char test_location, char test_type)
{
   int okaysite[SITENUM];
   memset(okaysite,0,SITENUM*sizeof(int));

   short type;

   // If the caller sets test_type, they're just
   // asking if the chosen creature type is appropriate
   // to the location they provided, not actually setting
   // the creature work location -- this is useful
   // for things like stealth
   if(test_type!=-1)
      type=test_type;
   else
      type=cr.type;

   switch(type)
   {
      case CREATURE_BOUNCER:
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         break;
      case CREATURE_CORPORATE_CEO:
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         break;
      case CREATURE_SECURITYGUARD:
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         //okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_SCIENTIST_LABTECH:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         break;
      case CREATURE_SCIENTIST_EMINENT:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         break;
      case CREATURE_CORPORATE_MANAGER:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_WORKER_SERVANT:
         okaysite[SITE_CORPORATE_HOUSE]=1;
         break;
      case CREATURE_WORKER_JANITOR:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_HOSPITAL_CLINIC]=1;
         okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_GOVERNMENT_PRISON]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_BUSINESS_PAWNSHOP]=1;
         okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
         okaysite[SITE_BUSINESS_JUICEBAR]=1;
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         okaysite[SITE_BUSINESS_LATTESTAND]=1;
         okaysite[SITE_BUSINESS_VEGANCOOP]=1;
         okaysite[SITE_BUSINESS_INTERNETCAFE]=1;
         okaysite[SITE_BUSINESS_DEPTSTORE]=1;
         okaysite[SITE_BUSINESS_HALLOWEEN]=1;
         okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_WORKER_SWEATSHOP:
         okaysite[SITE_INDUSTRY_SWEATSHOP]=1;
         break;
      case CREATURE_WORKER_FACTORY_NONUNION:
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         break;
      case CREATURE_WORKER_FACTORY_CHILD:
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         break;
      case CREATURE_WORKER_SECRETARY:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_HOSPITAL_CLINIC]=1;
         okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_WORKER_FACTORY_UNION:
         okaysite[SITE_INDUSTRY_POLLUTER]=1;
         break;
      case CREATURE_LANDLORD:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         break;
      case CREATURE_TEENAGER:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_APARTMENT_UPSCALE]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         break;
      case CREATURE_COP:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_DEATHSQUAD:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_FIREFIGHTER:
         okaysite[SITE_GOVERNMENT_FIRESTATION]=1;
         break;
      case CREATURE_GANGUNIT:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_SWAT:
         okaysite[SITE_GOVERNMENT_POLICESTATION]=1;
         break;
      case CREATURE_JUDGE_LIBERAL:
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         break;
      case CREATURE_JUDGE_CONSERVATIVE:
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         break;
      case CREATURE_AGENT:
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         break;
      case CREATURE_RADIOPERSONALITY:
         okaysite[SITE_MEDIA_AMRADIO]=1;
         break;
      case CREATURE_NEWSANCHOR:
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_GENETIC:
         okaysite[SITE_LABORATORY_GENETIC]=1;
         break;
      case CREATURE_GUARDDOG:
         okaysite[SITE_GOVERNMENT_PRISON]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_CORPORATE_HOUSE]=1;
         break;
      case CREATURE_PRISONER:
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
      case CREATURE_JUROR:
         okaysite[SITE_RESIDENTIAL_APARTMENT]=1;
         okaysite[SITE_RESIDENTIAL_TENEMENT]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
      case CREATURE_LAWYER:
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         break;
      case CREATURE_DOCTOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_PSYCHOLOGIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_NURSE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_CCS_ARCHCONSERVATIVE:
      case CREATURE_CCS_MOLOTOV:
      case CREATURE_CCS_SNIPER:
      case CREATURE_CCS_VIGILANTE:
         if(ccs_kills==2)okaysite[SITE_OUTDOOR_BUNKER]=1;
         if(ccs_kills==1)okaysite[SITE_RESIDENTIAL_BOMBSHELTER]=1;
         if(ccs_kills==0)okaysite[SITE_BUSINESS_BARANDGRILL]=1;
         break;
      case CREATURE_SEWERWORKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_COLLEGESTUDENT:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_MUSICIAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_MATHEMATICIAN:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_TEACHER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_HSDROPOUT:
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_BUM:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
         break;
      case CREATURE_GANGMEMBER:
         okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
         break;
      case CREATURE_CRACKHEAD:
         okaysite[SITE_BUSINESS_CRACKHOUSE]=1;
         break;
      case CREATURE_PRIEST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_ENGINEER:
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_INDUSTRY_NUCLEAR]=1;
         break;
      case CREATURE_FASTFOODWORKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_TELEMARKETER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_OFFICEWORKER:
         okaysite[SITE_LABORATORY_COSMETICS]=1;
         okaysite[SITE_LABORATORY_GENETIC]=1;
         okaysite[SITE_HOSPITAL_CLINIC]=1;
         okaysite[SITE_HOSPITAL_UNIVERSITY]=1;
         okaysite[SITE_GOVERNMENT_COURTHOUSE]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_BUSINESS_DEPTSTORE]=1;
         break;
      case CREATURE_FOOTBALLCOACH:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_PROSTITUTE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_MAILMAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_GARBAGEMAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PLUMBER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_CHEF:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_BUSINESS_CIGARBAR]=1;
         break;
      case CREATURE_CONSTRUCTIONWORKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_AMATEURMAGICIAN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_TANK:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_MERC:
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         break;
      case CREATURE_HICK:
         okaysite[SITE_MEDIA_AMRADIO]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         okaysite[SITE_OUTOFTOWN]=1;
         break;
      case CREATURE_VETERAN:
      case CREATURE_HARDENED_VETERAN:
      case CREATURE_SOLDIER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_EDUCATOR:
      case CREATURE_PRISONGUARD:
         okaysite[SITE_GOVERNMENT_PRISON]=1;
         break;
      case CREATURE_HIPPIE:
         okaysite[SITE_BUSINESS_VEGANCOOP]=1;
         break;
      case CREATURE_CRITIC_ART:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_CRITIC_MUSIC:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_MEDIA_AMRADIO]=1;
         break;
      case CREATURE_SOCIALITE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PROGRAMMER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         okaysite[SITE_GOVERNMENT_INTELLIGENCEHQ]=1;
         okaysite[SITE_CORPORATE_HEADQUARTERS]=1;
         break;
      case CREATURE_RETIREE:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PAINTER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_SCULPTOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_AUTHOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_JOURNALIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_DANCER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_PHOTOGRAPHER:
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_CAMERAMAN:
         okaysite[SITE_MEDIA_CABLENEWS]=1;
         break;
      case CREATURE_HAIRSTYLIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_FASHIONDESIGNER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_CLERK:
         okaysite[SITE_BUSINESS_JUICEBAR]=1;
         okaysite[SITE_BUSINESS_LATTESTAND]=1;
         okaysite[SITE_BUSINESS_INTERNETCAFE]=1;
         okaysite[SITE_BUSINESS_DEPTSTORE]=1;
         okaysite[SITE_BUSINESS_HALLOWEEN]=1;
         break;
      case CREATURE_THIEF:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_ACTOR:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_YOGAINSTRUCTOR:
         okaysite[SITE_BUSINESS_VEGANCOOP]=1;
         break;
      case CREATURE_MARTIALARTIST:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
      case CREATURE_ATHLETE:
         okaysite[SITE_UDISTRICT]=1;
         break;
      case CREATURE_BIKER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_TRUCKER:
         okaysite[SITE_OUTOFTOWN]=1;
         break;
      case CREATURE_TAXIDRIVER:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      case CREATURE_NUN:
         okaysite[SITE_DOWNTOWN]=1;
         okaysite[SITE_UDISTRICT]=1;
         okaysite[SITE_INDUSTRIAL]=1;
         break;
      default:
         okaysite[SITE_RESIDENTIAL_SHELTER]=1;
   }

   // Quick exit if only checking if a certain type works
   if(test_type!=-1)
   {
      return okaysite[test_location];
   }

   char swap=0;
   if(cr.worklocation==-1)swap=1;
   else
   {
      if(!okaysite[location[cr.worklocation]->type])swap=1;
   }

   if(swap)
   {
      //PICK A TYPE OF WORK LOCATION
      cr.worklocation=choose_one(okaysite,SITENUM,0);

      //FIND ONE OF THESE
      vector<int> goodlist;

      for(int l=0;l<location.size();l++)
      {
         if(location[l]->type==cr.worklocation)
         {
            goodlist.push_back(l);
         }
      }
// Sadler - This line sometimes causes a memory fault
//               Only thing I can think of is if loop above didn'
//               find any locations of type == to cr.worklocation
//               My hunch is that some locations, such as the 1st four
//               are special and cannot be used here..
//      
//   TODO There was a bug in the makecharacter() code where th
//   SITE_OUTOFTOWN was not set properly. This was fixed but the bug here
//   is still occuring, normally at the Latte Bar Downtown ;
      if (goodlist.size()==0)
      {
          cr.worklocation=0;
      }
      else
      {
         cr.worklocation=goodlist[LCSrandom(goodlist.size())];
      }
   }
   return false;
}



/* turns a creature into a conservative */
void conservatise(Creature &cr)
{
   if(cr.align==ALIGN_LIBERAL && cr.juice>0)cr.juice=0;
   
   cr.align=ALIGN_CONSERVATIVE;

   switch(cr.type)
   {
      case CREATURE_WORKER_FACTORY_UNION:
         strcpy(cr.name,"Ex-Union Worker");
         break;
      case CREATURE_JUDGE_LIBERAL:
         strcpy(cr.name,"Jaded Liberal Judge");
         break;
   }
}



/* turns a creature into a liberal */
void liberalize(Creature &cr,bool rename)
{
   if(cr.align==ALIGN_CONSERVATIVE && cr.juice>0)cr.juice=0;

   cr.align=ALIGN_LIBERAL;

   if(rename)
   {
      switch(cr.type)
      {
         case CREATURE_WORKER_FACTORY_NONUNION:
            strcpy(cr.name,"New Union Worker");
            break;
//       case CREATURE_JUDGE_CONSERVATIVE:
//          strcpy(cr.name,"Enlightened Judge");
//          break;
      }
   }
}

/* gives a CCS member a cover name */
void nameCCSMember(Creature &cr)
{
   if(cr.armor.type==ARMOR_CIVILLIANARMOR)
   {
      strcpy(cr.name,"Mercenary");
   }
   else if(cr.armor.type==ARMOR_ARMYARMOR)
   {
      strcpy(cr.name,"Soldier");
   }
   else if(cr.armor.type==ARMOR_HEAVYARMOR)
   {
      strcpy(cr.name,"Hardened Veteran");
   }
   else if(cr.weapon.type==WEAPON_SHOTGUN_PUMP||LCSrandom(2))
   {
      switch(LCSrandom(5))
      {
         case 0:strcpy(cr.name,"Country Boy");break;
         case 1:strcpy(cr.name,"Hick");break;
         case 2:strcpy(cr.name,"Redneck");break;
         case 3:strcpy(cr.name,"Rube");break;
         case 4:strcpy(cr.name,"Yokel");break;
      }
   }
   else
   {
      switch(LCSrandom(9))
      {
      case 0: strcpy(cr.name,"Biker");break;
      case 1: strcpy(cr.name,"Transient");break;
      case 2: strcpy(cr.name,"Crackhead");break;
      case 3: strcpy(cr.name,"Fast Food Worker");break;
      case 4: strcpy(cr.name,"Telemarketer");break;
      case 5: strcpy(cr.name,"Office Worker");break;
      case 6: strcpy(cr.name,"Mailman");break;
      case 7: strcpy(cr.name,"Musician");break;
      case 8: strcpy(cr.name,"Hairstylist");break;
      }
   }
}

/* are they interested in talking about the issues? */
bool Creature::talkreceptive()
{
   if(enemy())return false;

   switch(type)
   {
      case CREATURE_WORKER_SERVANT:
      case CREATURE_WORKER_JANITOR:
      case CREATURE_WORKER_SWEATSHOP:
      case CREATURE_WORKER_FACTORY_CHILD:
      case CREATURE_TEENAGER:
      case CREATURE_SEWERWORKER:
      case CREATURE_COLLEGESTUDENT:
      case CREATURE_MUSICIAN:
      case CREATURE_MATHEMATICIAN:
      case CREATURE_TEACHER:
      case CREATURE_HSDROPOUT:
      case CREATURE_BUM:
      case CREATURE_POLITICALACTIVIST:
      case CREATURE_GANGMEMBER:
      case CREATURE_CRACKHEAD:
      case CREATURE_FASTFOODWORKER:
      case CREATURE_TELEMARKETER:
      case CREATURE_PROSTITUTE:
      case CREATURE_GARBAGEMAN:
      case CREATURE_PLUMBER:
      case CREATURE_AMATEURMAGICIAN:
      case CREATURE_HIPPIE:
      case CREATURE_RETIREE:
      case CREATURE_HAIRSTYLIST:
      case CREATURE_CLERK:
      case CREATURE_MUTANT: // What? -- LK
         return true;
   }

   return false;
}

#define ABS(x) ((x)<0)?(-x):(x)

/* are the characters close enough in age to date? */
bool Creature::can_date(Creature &a)
{
   if(age<11 || a.age<11) return false;
//   if(a.age<11) return false;
   if(age<16 || a.age<16)
   {
      if(ABS(age-a.age)<5)
         return true;
      else
         return false;
   }
   else return true;
}
