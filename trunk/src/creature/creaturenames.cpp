// Note: this file is encoded in the PC-8 / Code Page 437 / OEM-US character set
// (The same character set used by Liberal Crime Squad when it is running)
// Certain special characters won't display correctly unless your text editor is
// set to use that character set, such as this e with an accent: ‚

// In Windows Notepad with the Terminal font, OEM/DOS encoding it should work fine.
// You can set this in Notepad by going to Format->Font and choosing the Terminal font,
// then choosing OEM/DOS in the Script dropdown box.

// In Notepad++ go to the Encoding menu, Character sets, Western European, OEM-US... easy!

// In Code::Blocks's editor go to Settings->Editor->the Other Settings tab and
// then pick WINDOWS-437 from the dropdown box and then choose the radio button
// to make this the default encoding and disable auto-detection of the encoding.
// Then close the file and reopen it (since Code::Blocks detects the encoding
// when it opens the file and it can't be changed after that; what we changed was
// how it detects encoding for files it opens in the future, not files already open).

// In Microsoft Visual C++, right-click the file in the Solution Explorer,
// select "Open With...", choose "C++ Source Code Editor (with encoding)",
// then choose "OEM United States - Codepage 437".

// In MS-DOS Editor (included with Windows as EDIT.COM in your system32 directory),
// the codepage will be correct already since it's running in a console window just
// like Liberal Crime Squad. Well OK, the encoding might be wrong, but then it's wrong
// in Liberal Crime Squad TOO, and to fix it, go to Control Panel, Regional and Language Settings,
// Advanced tab, and choose English (United States) from the dropdown box as the encoding
// for non-Unicode applications, then press OK.

// If you have a Linux or other UNIX-based system you are obviously smart enough
// to figure out for yourself how to open a file in OEM-US PC-8 codepage 437 in
// your favorite text editor. If you're on Mac OS X, well that's UNIX-based, figure
// it out for yourself.

//#include "includes.h"
#include "externs.h"

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

   // Assess their gender Liberally but allow ambiguity since you don't know them well enough yet
   if(person.gender_liberal == GENDER_MALE)
      addstr(", Male");
   else if(person.gender_liberal == GENDER_FEMALE)
      addstr(", Female");
   else
      addstr(", Ambiguous");

   // Note if there's some conflict with Conservative society's perceptions
   if(person.gender_liberal != person.gender_conservative && person.gender_liberal != GENDER_NEUTRAL)
      addstr("?");

   addstr(")");
}


/* rolls up a proper name for a creature */
void Creature::namecreature()
{
   if(!dontname)
   {
      generate_name(name, gender_liberal);
      strcpy(propername,name);
      dontname = true;
   }
}



/* fills a string with a proper name */
void generate_name(char *str, char gender)
{
   char first[80];
   char last[80];

   generate_name(first,last,gender);
   strcpy(str,first);
   strcat(str," ");
   strcat(str,last);
}



/* get a first and last name for the same person */
void generate_name(char *first, char *last, char gender)
{
   do {
      firstname(first, gender);
      lastname(last, gender == GENDER_WHITEMALEPATRIARCH);
   } while(strcmp(first,last) == 0);
}



/* get a first, middle, and last name for the same person */
void generate_long_name(char *first, char *middle, char *last, char gender)
{
   // pick either male or female so we can have 75% chance of first and middle names having matching genders
   if(gender == GENDER_NEUTRAL)
      gender = (LCSrandom(2)?GENDER_MALE:GENDER_FEMALE);
   do {
      firstname(first, gender);
      if(LCSrandom(2)) // middle name is a first name
         firstname(middle,(gender == GENDER_WHITEMALEPATRIARCH || LCSrandom(2)?gender:GENDER_NEUTRAL)); // 25% chance for middle name of other gender unless white male patriarch
      else // middle name is a last name
         lastname(middle, gender == GENDER_WHITEMALEPATRIARCH);
      lastname(last, gender == GENDER_WHITEMALEPATRIARCH);
   } while(strcmp(first,middle) == 0 && strcmp(first,last) == 0 && strcmp(middle,last) == 0);
}



/* gets a random first name */
void firstname(char *str, char gender)
{
   static const char *male_first_names[] =
   {
   "Aaron",   "Abraham",   "Adam",   "Adolf",
   "Adolph",   "Adrian",   "Al",   "Alan",
   "Albert",   "Alec",   "Alejandro",   "Alexander",
   "Alf",   "Alfie",   "Alfonse",   "Alfred",
   "Ali",   "Allan",   "Allen",   "Andrew",
   "Andy",   "Angus",   "Anthony",   "Antonio",
   "Archibald",   "Archie",   "Arnold",   "Art",
   "Arthur",   "Attila",   "Avi",   "Axel",
   "Barack",   "Beltran",   "Barney",   "Barry",
   "Bart",   "Bartholomew",   "Beau",   "Ben",
   "Benedict",   "Benito",   "Benjamin",   "Benny",
   "Bernard",   "Bernie",   "Bert",   "Bill",
   "Billy Bob",   "Blaire",   "Blake",   "Bob",
   "Bobby",   "Boris",   "Brad",   "Bradley",
   "Brandon",   "Brendon",   "Brent",   "Brett",
   "Brian",   "Bruce",   "Bryan",   "Bubba",
   "Cain",   "Calvin",   "Carl",   "Carlos",
   "Cesar",   "Chad",   "Charles",   "Charlie",
   "Chester",   "Chuck",   "Christian",   "Christopher",
   "Cid",   "Claude",   "Cliff",   "Clifford",
   "Clive",   "Cliven",   "Clyde",   "Cody",
   "Colin",   "Colton",   "Conrad",   "Cooper",
   "Cory",   "Craig",   "Dale",   "Damien",
   "Damon",   "Dan",   "Danforth",   "Daniel",
   "Danny",   "Dag",   "Dave",   "David",
   "Deepak",   "Dennis",   "Derak",   "Derek",
   "Derrick",   "Dexter",   "Dick",   "Diego",
   "Dinesh",   "Dink",   "Diwakar",   "Dominic",
   "Don",   "Donald",   "Donovan",   "Doug",
   "Douglas",   "Dustin",   "Dwayne",   "Dwight",
   "Earl",   "Ed",   "Eddie",   "Edgar",
   "Eduardo",   "Edward",   "Emmanuel",   "Emmit",
   "Edwin",   "Eli",   "Eliah",   "Eliot",
   "Elliot",   "Elvis",   "Enrico",   "Enrique",
   "Enzo",   "Eric",   "Erik",   "Ernest",
   "Ernie",   "Ethan",   "Eugene",   "Evan",
   "Ezekiel",   "Ezra",   "Fabio",   "Felix",
   "Fernando",   "Floyd",   "Forrest",   "Francesco",
   "Francis",   "Francisco",   "Frank",   "Franklin",
   "Franz",   "Fred",   "Frederick",   "Fritz",
   "Gabriel",   "Garett",   "Garreth",   "Garth",
   "Gary",   "George",   "Gerald",   "Glen",
   "Glenn",   "Graham",   "Greg",   "Gregory",
   "Gus",   "Gustav",   "Guy",   "Hagar",
   "Hank",   "Hans",   "Harold",   "Harry",
   "Harvey",   "Henry",   "Herb",   "Herbert",
   "Herman",   "Hiro",   "Homer",   "Howard",
   "Howie",   "Hugh",   "Hugo",   "Ian",
   "Igor",   "Ike",   "Ira",   "Irving",
   "Isaac",   "Ivan",   "Jack",   "Jacob",
   "Jacobo",   "Jake",   "Jamel",   "James",
   "Jared",   "Jason",   "Javier",   "Jay",
   "Jeff",   "Jeffrey",   "Jeremiah",   "Jeremy",
   "Jeroen",   "Jerome",   "Jerry",   "Jesse",
   "Jes£s",   "Jim",   "Jimbo",   "Jimi",
   "Jimmy",   "Jinyuan",   "Joe",   "Joel",
   "Joey",   "John",   "Johnny",   "Jon",
   "Jonathan",   "Jorge",   "Jos‚",   "Josef",
   "Joseph",   "Josh",   "Joshua",   "Jowar",
   "Juan",   "Judas",   "Jude",   "Julian",
   "Julius",   "Justin",   "Kaelen",   "Kane",
   "Karl",   "Keith",   "Ken",   "Kenny",
   "Kenji",   "Kennedy",   "Kenneth",   "Kevin",
   "Kirk",   "Konstantine",   "Kurt",   "Kweisi",
   "Kyle",   "Lachlan",   "Lamar",   "Lance",
   "Larry",   "Lawrence",   "Lenny",   "Leo",
   "Leon",   "Leonard",   "Leroy",   "Levi",
   "Lex",   "Liam",   "Lionel",   "Lloyd",
   "Lothar",   "Louis",   "Lucas",   "Luigi",
   "Luis",   "Luke",   "Lyle",   "Malcolm",
   "Manny",   "Manuel",   "Marcus",   "Marvin",
   "Mark",   "Mario",   "Martin",   "Marty",
   "Mason",   "Matt",   "Matthew",   "Matteo",
   "Maurice",   "Maximilian",   "Melvin",   "Michael",
   "Mick",   "Mickey",   "Miguel",   "Mike",
   "Mikhail",   "Miles",   "Milton",   "Mitch",
   "Mitchell",   "Mitt",   "Moe",   "Mumtaz",
   "Murray",   "Mustafa",   "Napoleon",   "Nathan",
   "Nathaniel",   "Ned",   "Neal",   "Neil",
   "Nelson",   "Newt",   "Nicholas",   "Nick",
   "Noah",   "Norm",   "Norman",   "Oliver",
   "Osama",   "Otto",   "Owen",   "Pablo",
   "Pascal",   "Patrick",   "Paul",   "Pedro",
   "Pete",   "Peter",   "Phil",   "Phillip",
   "Pierre",   "Preston",   "Quentin",   "Quincy",
   "Rafael",   "Ralf",   "Ralph",   "Randall",
   "Randy",   "Rane",   "Ransom",   "Raphael",
   "Raul",   "Ray",   "Raymond",   "Reginald",
   "Regis",   "Rex",   "Ricardo",   "Rich",
   "Richard",   "Rick",   "Robert",   "Rod",
   "Rodney",   "Roger",   "Romeo",   "Ron",
   "Ronald",   "Rory",   "Roy",   "Rubin",
   "Rudolf",   "Rudolph",   "Rudy",   "Rufus",
   "Rupert",   "Russ",   "Russell",   "Ryan",
   "Samuel",   "Scott",   "Sean",   "Sebastian",
   "Serge",   "Sergio",   "Seth",   "Shane",
   "Shaun",   "Shawn",   "Sid",   "Simon",
   "Sonny",   "Stan",   "Stanley",   "Stephen",
   "Steve",   "Steven",   "Stewart",   "Stu",
   "Stuart",   "Ted",   "Tetsuo",   "Theo",
   "Theodore",   "Thomas",   "Thor",   "Tim",
   "Timothy",   "Toby",   "Todd",   "Tom",
   "Tony",   "Toshiro",   "Travis",   "Trayvon",
   "Trevor",   "Tyler",   "Tyrone",   "Tyson",
   "Victor",   "Vincent",   "Vlad",   "Vladimir",
   "Wade",   "Wally",   "Walt",   "Walter",
   "Wayne",   "Wilhelm",   "Will",   "William",
   "Willy",   "Winston",   "Wolf",   "Woody",
   "Xavier",   "Yann",   "Yuri",   "Zachary",
   "Zack",   "Zebulon",   "Zeke",   "Zoltan"
   };

   static const char *female_first_names[] =
   {
   "Abigail",   "Ada",   "Adriana",   "Adrianna",
   "Adrienne",   "Agatha",   "Aiko",   "Aileen",
   "Alene",   "Alexa",   "Alexandra",   "Alice",
   "Alicia",   "Alison",   "Allison",   "Alora",
   "Alyson",   "Amanda",   "Amber",   "Amelia",
   "America",   "Amy",   "Ana",   "Anastasia",
   "Andrea",   "Angela",   "Angelica",   "Angelina",
   "Angelique",   "Anita",   "Ann",   "Anna",
   "Anne",   "Annie",   "April",   "Autumn",
   "Ava",   "Ayn",   "Barb",   "Barbara",
   "Barbie",   "Bea",   "Beatrice",   "Bebe",
   "Becky",   "Bella",   "Belle",   "Bernice",
   "Bertha",   "Beth",   "Beth Ann",   "Bethany",
   "Betty",   "Bianca",   "Blanche",   "Bobbie",
   "Bonita",   "Bonnie",   "Brenda",   "Brittany",
   "Brooke",   "Brooklyn",   "Camilia",   "Carla",
   "Carmen",   "Carol",   "Caroline",   "Cassandra",
   "Catherine",   "Cathy",   "Cecilia",   "Celine",
   "Charlotte",   "Chastity",   "Chelsea",   "Cheryl",
   "Chloe",   "Christina",   "Christine",   "Christy",
   "Cindi",   "Cindy",   "Clara",   "Clarissa",
   "Clarity",   "Claudette",   "Claudia",   "Cleopatra",
   "Colette",   "Colleen",   "Consuela",   "Courtney",
   "Crystal",   "Cybill",   "Dakota",   "Dana",
   "Daniela",   "Danielle",   "Daphne",   "Dawn",
   "Debbie",   "Debra",   "Denielle",   "Denise",
   "Desiree",   "Diane",   "Dimitra",   "Donna",
   "Dora",   "Doreen",   "Doris",   "Dorothy",
   "Edna",   "Eileen",   "Elaine",   "Elena",
   "Elinor",   "Eliza",   "Elizabeth",   "Ellen",
   "Elsie",   "Elvira",   "Emily",   "Emma",
   "Erica",   "Erika",   "Erin",   "Erma",
   "Esmerelda",   "Eva",   "Eve",   "Faith",
   "Fatima",   "Felicia",   "Felicity",   "Fiona",
   "Florence",   "Florida",   "Fran",   "Francine",
   "Gabrielle",   "Galaxina",   "Georgia",   "Gertrude",
   "Gina",   "Giovanna",   "Gloria",   "Grace",
   "Gwen",   "Gwendolyn",   "Hailey",   "Haley",
   "Hannah",   "Harmony",   "Harriet",   "Hayley",
   "Hazel",   "Heather",   "Helen",   "Helga",
   "Hilda",   "Holly",   "Honor",   "Ida",
   "Ingrid",   "Irene",   "Iris",   "Isabel",
   "Isabella",   "Isabelle",   "Isla",   "Ivana",
   "Ivy",   "Jaclyn",   "Jade",   "Jane",
   "Janet",   "Janette",   "Janice",   "January",
   "Jaqueline",   "Jasmin",   "Jasmine",   "Jennifer",
   "Jenny",   "Jessica",   "Jill",   "Jillian",
   "Joan",   "Joann",   "Josephine",   "Joyce",
   "Judy",   "Juliana",   "Julia",   "Julie",
   "Juliet",   "Juliette",   "June",   "Kaitlyn",
   "Kaley",   "Karen",   "Karla",   "Kate",
   "Katelin",   "Katerina",   "Katherine",   "Kathleen",
   "Kathy",   "Katie",   "Kay",   "Kayla",
   "Kaylee",   "Keely",   "Kiki",   "Klara",
   "Kristal",   "Kristen",   "Kristie",   "Kristin",
   "Krystal",   "Lacey",   "Lara",   "Larissa",
   "Latoya",   "Laura",   "Lauren",   "Lavender",
   "Leah",   "Leela",   "Leila",   "Leyna",
   "Liliana",   "Lilly",   "Lily",   "Lina",
   "Linda",   "Lisa",   "Liz",   "Lizabeth",
   "Lizzie",   "Lois",   "Loretta",   "Louane",
   "Louanne",   "Louise",   "Lucia",   "Lucy",
   "Lydia",   "Lynn",   "Lynnette",   "Madison",
   "Madonna",   "Mags",   "Manuela",   "Margaret",
   "Marge",   "Margit",   "Maria",   "Mariana",
   "Marianne",   "Maribel",   "Marie",   "Marielle",
   "Marion",   "Marlene",   "Marta",   "Martha",
   "Martina",   "Mary",   "Mary Ann",   "Matilda",
   "Maureen",   "Maxine",   "Mayumi",   "Meg",
   "Megan",   "Meghan",   "Megumi",   "Melanie",
   "Melissa",   "Melody",   "Mercedes",   "Merissa",
   "Mia",   "Michele",   "Michelle",   "Mimi",
   "Minal",   "Mindy",   "Miranda",   "Molly",
   "Monica",   "Nadia",   "Nancy",   "Natalia",
   "Natalie",   "Natasha",   "Nellie",   "Nicole",
   "Nicolette",   "Nina",   "Nora",   "Norma",
   "October",   "Olivia",   "Oprah",   "Orly",
   "Pam",   "Pamela",   "Patricia",   "Patty",
   "Paula",   "Paulette",   "Peggy",   "Petunia",
   "Phoebe",   "Phyllis",   "Priscilla",   "Prudence",
   "Rachael",   "Racheal",   "Rachel",   "Rachelle",
   "Ramona",   "Rania",   "Raphaela",   "Raquel",
   "Rebecca",   "Regina",   "Ren‚e",   "Rita",
   "Rosa",   "Rosalie",   "Rosalinda",   "Rosanna",
   "Roberta",   "Rose",   "Rosemary",   "Rosie",
   "Roxanne",   "Roz",   "Ruth",   "Ruthanne",
   "Sabrina",   "Sally",   "Samantha",   "Sandra",
   "Sara",   "Sarah",   "Satya",   "Selma",
   "Shaniqua",   "Shanna",   "Sharon",   "Sherry",
   "Shirley",   "Shyann",   "Sian",   "Silvia",
   "Sofia",   "Sonia",   "Sophia",   "Sophie",
   "Stacie",   "Stacy",   "Stephanie",   "Summer",
   "Sue",   "Susan",   "Susanna",   "Suzanne",
   "Suzie",   "Sylvia",   "Tabatha",   "Tabitha",
   "Tamara",   "Tammy",   "Tara",   "Tawanda",
   "Teresa",   "Terri",   "Tessa",   "Tetiana",
   "Theresa",   "Thyra",   "Tiffanie",   "Tiffany",
   "Tina",   "Toni",   "Tricia",   "Trish",
   "Trudy",   "Tyra",   "Valentina",   "Valeria",
   "Valerie",   "Vampira",   "Vanessa",   "Verena",
   "Veronica",   "Virginia",   "Vicki",   "Vicky",
   "Victoria",   "Violet",   "Vithara",   "Vonnita",
   "Wanda",   "Wendy",   "Whoopi",   "Wilhemina",
   "Willow",   "Wilma",   "Xenia",   "Yoko",
   "Yolanda",   "Yvonne",   "Zelda",   "Zo‰"
   };

   // these names should not appear in the male or female
   // or great white male patriarch first name lists, only here
   static const char *gender_neutral_first_names[] =
   {
   "Akira",   "Alex",   "Alexis",   "Angel",
   "Ariel",   "Ashley",   "Billy",   "Chris",
   "Frankie",   "Hillary",   "Kelly",   "Jackie",
   "Jamie",   "Jayden",   "Jean",   "Jess",
   "Jo",   "Jordan",   "Jun",   "Kelsey",
   "Kelsie",   "Kim",   "Lesley",   "Leslie",
   "Lindsay",   "Lindsey",   "Lou",   "Mao",
   "Max",   "Mel",   "Morgan",   "Nickie",
   "Nicky",   "No‰l",   "Page",   "Paige",
   "Pat",   "Raven",   "Riley",   "Robin",
   "Sam",   "Sandy",   "Sasha",   "Shannon",
   "Sidney",   "Terry",   "Tracy",   "Yanping"
   };

   // these Arch-Conservative names also all appear under the male names list too
   // and NONE of them are female or gender-neutral
   // (no "Chris", that can be short for Christine or Christina)
   static const char *great_white_male_patriarch_first_names[] =
   {
   "Alan",   "Andy",   "Arthur",   "Ben",
   "Bill",   "Bob",   "Brad",   "Brian",
   "Carl",   "Colin",   "Dan",   "Dave",
   "Dennis",   "Dick",   "Donald",   "Doug",
   "Ed",   "Edward",   "Eric",   "Frank",
   "Gary",   "Greg",   "Harry",   "Henry",
   "James",   "Jason",   "Jeff",   "Jerry",
   "Jesse",   "Jim",   "Joe",   "John",
   "Josh",   "Ken",   "Kevin",   "Kyle",
   "Larry",   "Luke",   "Mark",   "Matt",
   "Mike",   "Nick",   "Paul",   "Peter",
   "Ray",   "Rick",   "Roger",   "Ron",
   "Ryan",   "Scott",   "Steve",   "Ted",
   "Tim",   "Tom",   "Tony",   "Walter"
   };

   strcpy(str,"");

   int roll, nametable;

   // If we don't care if the name is male or female, pick one randomly
   // This ensures gender balance in the names chosen
   if(gender == GENDER_NEUTRAL)
      gender = (LCSrandom(2)?GENDER_MALE:GENDER_FEMALE);

   // For white male Arch-Conservative politicians
   if(gender == GENDER_WHITEMALEPATRIARCH)
   {
      nametable = GENDER_WHITEMALEPATRIARCH;
   }

   // Assign a name from the available names for each gender
   if(gender == GENDER_MALE)
   {
      // Roll on the number of gender-specific names,
      // plus the number of gender-neutral names
      roll = LCSrandom( ARRAY_ELEMENTS(male_first_names) +
                        ARRAY_ELEMENTS(gender_neutral_first_names));
      // Decide whether to use a gender-specific name
      // or a gender-neutral name
      if(roll >= ARRAY_ELEMENTS(gender_neutral_first_names))
      {  nametable = GENDER_MALE;   }
      else
      {  nametable = GENDER_NEUTRAL;   }
   }
   else if(gender == GENDER_FEMALE)
   {
      // (Same here, just use the number of female names instead)
      roll = LCSrandom( ARRAY_ELEMENTS(female_first_names) +
                        ARRAY_ELEMENTS(gender_neutral_first_names));
      if(roll >= ARRAY_ELEMENTS(gender_neutral_first_names))
      {  nametable = GENDER_FEMALE; }
      else
      {  nametable = GENDER_NEUTRAL;   }
   }

   if(nametable == GENDER_MALE)
//{{{ Male
   {
      strcat(str, selectRandomString(male_first_names,
                      ARRAY_ELEMENTS(male_first_names)));
   }
//}}}
   else if(nametable == GENDER_FEMALE)
//{{{ Female
   {
      strcat(str, selectRandomString(female_first_names,
                      ARRAY_ELEMENTS(female_first_names)));
//}}}
   }
//}}}
//{{{ Neutral
   else if(nametable == GENDER_NEUTRAL)
   {
      strcat(str, selectRandomString(gender_neutral_first_names,
                      ARRAY_ELEMENTS(gender_neutral_first_names)));
   }
//}}}
   else if(nametable == GENDER_WHITEMALEPATRIARCH)
//{{{ White Male Patriarchs
   {
      strcat(str, selectRandomString(great_white_male_patriarch_first_names,
                      ARRAY_ELEMENTS(great_white_male_patriarch_first_names)));
   }
//}}}
   else
//{{{ We have an error, so let's name the person "Errol"
   {
      strcat(str, "Errol");
   }
//}}}
}
/* gets a random last name */
void lastname(char *str, bool archconservative)
//{{{ Last Name
{
   // note: every last name should only occur ONCE in either this list
   // or the Arch-Conservative list
   static const char *regular_last_names[] =
   {
   "Abbey", "Abbott", "Abbzug", "Adamson",
   "Adkins", "Africa", "Aguilera", "Alanson",
   "Alcott", "Alda", "Allanshaw", "Alley",
   "Alvarez", "Andress", "Antle", "Appleton",
   "Armstead", "Arnaz", "Artfield", "Ashmore",
   "Assange", "Ayers", "Aykroyd", "Bacall",
   "Baird", "Baldwin", "Balgos", "Baranyi",
   "Barrow", "Barwick", "Beaumont", "Bendix",
   "Biden", "Birkin", "Blackenship", "Blanchard",
   "Blitzer", "Bogart", "Boggs", "Bolger",
   "Booker", "Bosanquet", "Bradbury", "Bradley",
   "Bradshaw", "Brandis", "Brewster", "Breyer",
   "Brightman", "Briscoe", "Brownlee", "Bryant",
   "Buckman", "Buckner", "Bugbee", "Bump",
   "Bunton", "Burghoff", "Butcher", "Butler",
   "Butt", "Butterman", "Buttman", "Carberry",
   "Carr", "Carson", "Cartwright", "Cavallero",
   "Cavill", "Chamberlain", "Chandler", "Channing",
   "Cheng", "Childress", "Childs", "Chilton",
   "Cho", "Chubb", "Clavin", "Clinton",
   "Coffey", "Cohen", "Colcord", "Colombe",
   "Conner", "Coopersmith", "Copeland", "Coppola",
   "Cordero", "Cosgrove", "Cottrell", "Court",
   "Craft", "Critchley", "Croft", "Cromwell",
   "Cross", "Crowe", "Crowson", "Cuomo",
   "Curnew", "Cutlass", "Cutler", "Danes",
   "Dangerfield", "Dante", "Darnand", "Dash",
   "Dawkins", "Daveson", "Day", "Dean",
   "Delgado", "DeLorean", "Dench", "Dent",
   "Dentz", "Diaz", "DiMeo", "Dominguez",
   "Dorgan", "Dorsey", "Doucette", "Downey",
   "Downing", "Drummond", "Duell", "Duval",
   "Earle", "Edwards", "English", "Escobar",
   "Ericson", "Eubanks", "al-Fadil", "Fairbanks",
   "Fairchild", "Falco", "Falstaff", "Farnsworth",
   "Farr", "Faulkner", "Fawkes", "Feingold",
   "Fenoglio", "Fiddlebottom", "Filler", "Fimple",
   "Fingleton", "Finn", "Florez", "Fortmuller",
   "France", "Frank", "Franken", "Frost",
   "Friend", "Fudge", "Fulsom", "Furley",
   "Fushimi", "Galindo", "Garcia", "Garnier",
   "Gatien", "Gelbart", "Gephardt", "Gilbert",
   "Gillmouth", "Gilpin", "Ginsberg", "Ginsburg",
   "Godwin", "Gleaves", "Goldberg", "Goldstein",
   "Gore", "Gorbachev", "Govorov", "Grant",
   "Green", "Greenberg", "Grover", "Gu",
   "Gutierrez", "Guy", "Hall", "Hammond",
   "Hann", "Hannemann", "Hatcher", "Hawking",
   "Hayes", "Haymer", "Headley", "Heath",
   "Hellmann", "Helton", "Hemingway", "Hendren",
   "Hendrix", "Hepburn", "Hepditch", "Hewson",
   "Heyduke", "Hiaasen", "Hilliard", "Himelstein",
   "Hoddinott", "Holder", "Hooker", "Hope",
   "Hopper", "Hornberger", "Houghton", "Houseman",
   "Howard", "Howe", "Hunnicutt", "Hutchison",
   "Ibarra", "Incledon", "Inking", "Inoue",
   "Irvin", "Irving", "Iscariot", "Issacson",
   "Ivanson", "Ives", "Jefferson", "Jimison",
   "Jolie", "Jordan", "Judge", "Justice",
   "Kagan", "Kaltenbrunner", "Keaton", "Keats",
   "Keene", "Keitel", "Kennedy", "Kershaw",
   "Kesten", "Kilbourne", "Kindale", "Kinslow",
   "Kitchen", "Kokernot", "Koprowski", "Krasow",
   "Krieger", "Krishna", "Krugman", "Kucinich",
   "Kunstler", "Kurosawa", "bin Laden", "LaSaga",
   "Lauder", "Lee", "Leeves", "Lenin",
   "Lennon", "Lewinsky", "Leyden", "Li",
   "Lincoln", "Linville", "Lipman", "Loeb",
   "Logsdon", "Lombard", "London", "Lopez",
   "Lovecraft", "Lowcourt", "Lowerly", "Lucas",
   "Luna", "Lushington", "Luzinski", "Lyles",
   "Lynam", "Van Lynden", "Lyons", "MacKinnon",
   "MacLeod", "MacNeil", "Macy", "Maddow",
   "Malone", "Mann", "Manning", "Mansfield",
   "March", "Marsden", "Marsh", "Marshall",
   "Marx", "Mathers", "Matheson", "Mathewson",
   "Matthews", "Maxwell", "McAdams", "McAllister",
   "McBroom", "McCorkell", "McFadden", "McFatridge",
   "McFaul", "McFly", "McGee", "McGovern",
   "McGrath", "McGraw", "McGuinness", "McIntosh",
   "McIntyre", "McKenzie", "McLeod", "McMurray",
   "McNabb", "McPhail", "McPherson", "McQueen",
   "Meek", "Meffert", "Melendez", "Menendez",
   "Metzger", "Mifune", "Milburn", "Miyamoto",
   "Montag", "Montalvo", "Moon", "Morrow",
   "Morton", "Mosley", "Muhammed", "Murrell",
   "Muscat", "Muskhelishvili", "Mussa", "Myatt",
   "Nagel", "Nagelberg", "Nakagawa", "Nakahara",
   "Nallet", "Nance", "Nash", "Nathanson",
   "Necker", "Nettles", "Neuwirth", "Nevinson",
   "Newberry", "Newhook", "Ng", "Nicolle",
   "Nickson", "Obama", "Obeid", "Oberlander",
   "Ochs", "Ogden", "Olbermann", "Olde",
   "Olds", "Oldsmar", "Ollerenshaw", "Orlando",
   "Ortiz", "Paoletti", "Palm", "Parking",
   "Parr", "Patterson", "Pearson", "Peck",
   "Pegg", "Pelosi", "Pelton", "Pendergrass",
   "Penn", "Perez", "Pert", "Petermann",
   "Pickett", "Piercey", "Pommerencke", "Pomper",
   "Porcaro", "Porter", "Postman", "Pound",
   "Powers", "Probst", "Proudfoot", "Puente",
   "Purcell", "Putin", "Queen", "Quennell",
   "Quick", "Quilico", "Quilliam", "Quilty",
   "Quincey", "Quinlivan", "Quinn", "Quirk",
   "Rabban", "Rackard", "Radman", "Rainey",
   "Ramirez", "Ramos", "Randell", "Ransom",
   "Rappaport", "Raskin", "Rastrelli", "Rather",
   "Ratzenberger", "Ravenhill", "Rawls", "Read",
   "Reading", "Reasoner", "Van Rechteren", "Redford",
   "Redman", "Redwine", "Reed", "Reekers",
   "Reid", "Rendell", "Rexford", "Ricci",
   "Richter", "Rideout", "Riffle", "Rodham",
   "Rodriguez", "Rojo", "Roland", "Roosevelt",
   "Rose", "Rosenberg", "Ross", "Rota",
   "Rothstein", "Roy", "Roylance", "Rudkin",
   "Rush", "Rushworth", "LaRussa", "Russell",
   "Ryman", "Sabella", "Saddler", "Sadler",
   "Sanders", "Sansome", "Santiago", "Santos",
   "Savedra", "Saxton", "Scales", "Scardino",
   "Schmitt", "Schumacher", "Schumer", "Schuster",
   "Scully", "Scurfield", "Sealock", "Seay",
   "Seinfeld", "Sellers", "Semin", "Serling",
   "Sewall", "Sexton", "Shackleton", "Shagwell",
   "al-Shamie", "Sharpe", "Sharpton", "Shedd",
   "Sheen", "Shelton", "Shengde", "Shepard",
   "Shepardson", "Sheppard", "Shew", "Shields",
   "Shiller", "Shimazu", "Shipman", "Shipp",
   "Sho", "Shrapnal", "Shrum", "Shumway",
   "Shurin", "Sidebottom", "Siegel", "Siever",
   "da Silva", "Silverman", "Silverstein", "Simmons",
   "Simms", "Simon", "Skidmore", "Skirrow",
   "Skutnik", "Skye", "Skyler", "Slaten",
   "Smalley", "Smallwood", "Smart", "Smyth",
   "Snead", "Snook", "Snowden", "Sommer",
   "Somner", "Soren", "Sotomayor", "Souter",
   "Spasov", "Stalin", "Stang", "Stark",
   "Starkweather", "Stelling", "Stephenson", "Sternin",
   "Stevens", "Stevenson", "Stewart", "Stockton",
   "Storm", "Storms", "Stiver", "Strain",
   "Straley", "Straub", "Stripe", "Strossen",
   "Suave", "Suiter", "Sulaiman", "Surnow",
   "Susman", "Swain", "Swaine", "Swift",
   "Swit", "Symington", "Symonds", "Tabb",
   "Tailboys", "Takayoshi", "Talbert", "Talbot",
   "Talbott", "Talty", "Tanner", "Tatters",
   "Taubman", "Tayo", "Teige", "Tell",
   "Tennent", "Tenney", "Tenpenny", "Terrill",
   "Testore", "Tetley", "Thalberg", "Thomaz",
   "Thomson", "Thorne", "Threepwood", "Thurgood",
   "Thurlow", "Tickell", "Tillett", "Tillinghast",
   "Tillson", "Tillsley", "Timmonds", "Tippett",
   "Tisdall", "Tishman", "Titcomb", "Tobin",
   "Todd", "Todhunter", "Toft", "Tolbert",
   "Tolkien", "Tolson", "Tonelli", "Toogood",
   "Tooke", "Toole", "Toth", "Towne",
   "Townsend", "Trafford", "Trebek", "Trotsky",
   "Truman", "Tubbs", "Tubman", "Tymchyshyn",
   "Ulmer", "Underhill", "Underdown", "Underwood",
   "Updike", "Usher", "Ussher", "Valdez",
   "Vallance", "Vance", "Vasilevsky", "Venus",
   "Villa", "Villanueva", "Viscuso", "Voland",
   "Vulcar", "Wade", "Wadsworth", "Walden",
   "Walwyn", "Wang", "Ward", "Warman",
   "Warr", "Warren", "Washington", "Waxman",
   "Weathers", "Webber", "Weeks", "Weller",
   "Wendt", "Westlake", "Weston", "Weygand",
   "Whitehead", "Whitley", "Wickes", "Willett",
   "Williamson", "Willman", "Winchester", "Winfrey",
   "Winkelman", "Winslow", "Winter", "Wiseman",
   "Wolfe", "Wong", "Woodrow", "Woods",
   "Wuornos", "Stassen", "Xue", "Yamaguchi",
   "Yee", "Yeltsin", "Yerkovich", "Yim",
   "Youngblood", "Yue", "Zachary", "Zadora",
   "Zakharov", "Zale", "Zedner", "Zedong",
   "Zhang", "Zicree", "Zimmermann", "Zito",
   "Zlotoff", "Zondervan", "Zucker", "Zumbach"
   };

   // these names are approved for use by Arch-Conservatives,
   // but will also sometimes appear as names for regular people too
   // last names for famous conservatives, fascists, European royalty, billionaires,
   // serial killers, and common, generic WASPy Anglo-Saxon names go here,
   // but be sure to exclude last names of famous liberals and communists,
   // instead those go in the previous list (otherwise we end up with
   // Arch-Conservatives with names like Bill Clinton!)
   static const char *archconservative_last_names[] =
   {
   "Adams", "Adelson", "Akin", "Alexander",
   "Alito", "Allen", "Ames", "Anderson",
   "Armey", "Armstrong", "Arnold", "Ashcroft",
   "Bachmann", "Bailey", "Baker", "Barr",
   "Bates", "Baxter", "Bennett", "Bernadotte",
   "Bezos", "Black", "Blum", "Boehner",
   "Bonaparte", "Bourbon", "Boyd", "Bozell",
   "Braganza", "Braun", "Breitbart", "Bremer",
   "Brooks", "Brown", "Buchanan", "Buckley",
   "Bundy", "Burr", "Burton", "Bush",
   "Byrd", "Callahan", "Cameron", "Campbell",
   "Carey", "Carnegie", "Carpenter", "Chaffee",
   "Chambers", "Cheney", "Churchill", "Clay",
   "Cobb", "Colbert", "Cole", "Coleman",
   "Collins", "Colt", "Cook", "Cooper",
   "Coulter", "Cox", "Crane", "Crockett",
   "Cruz", "Cunningham", "Dahmer", "Danson",
   "Darby", "Davidson", "Davis", "Decker",
   "Deere", "Delaney", "DeLay", "DeMint",
   "Diehl", "Disney", "Dixon", "Dobbs",
   "Dobson", "Dole", "Doyle", "Drake",
   "D'Souza", "Duke", "Duffield", "Dunham",
   "Dunlap", "Evans", "Falwell", "Fenderson",
   "Fields", "Fisher", "Fitzgerald", "Fitzpatrick",
   "Flake", "Flanders", "Fletcher", "Ford",
   "Foster", "Fox", "Franco", "Franklin",
   "Friedman", "Gacy", "Galt", "Gates",
   "Getty", "Gibson", "Gingrich", "Giuliani",
   "Glcksburg", "Gohmert", "Goldwater", "Goode",
   "Goodyear", "Graham", "Grammer", "Gray",
   "Gregg", "Grimaldi", "Hamilton", "Hannity",
   "Hanover", "Hapsburg", "Harper", "Harris",
   "Hastert", "Heller", "Helms", "Henderson",
   "Hess", "Hill", "Hitler", "Hoffmann",
   "Hogan", "Hohenzollern", "Holmes", "Hoover",
   "Hopkins", "Huckabee", "Hudson", "Hughes",
   "Jackson", "Jameson", "Jenkins", "Johnson",
   "Jones", "Kent", "King", "Koch",
   "Krauthammer", "Kristol", "Langley", "Lewis",
   "Limbaugh", "Little", "Logan", "Lott",
   "Lowe", "Luxembourg", "Lynch", "MacArthur",
   "MacGyver", "Mahoney", "McCain", "McCarthy",
   "McConnell", "McDonald", "McKinley", "McLaughlin",
   "McVeigh", "Medici", "Meyer", "Miller",
   "Mitchell", "Monroe", "Montana", "Morgan",
   "Morris", "Murphy", "Murray", "Mussolini",
   "Murdoch", "Myers", "Nelson", "Ness",
   "Newman", "Nielson", "Nixon", "North",
   "Oakley", "O'Connor", "Orange", "O'Reilly",
   "Osman", "Palin", "Palmer", "Parker",
   "Paul", "Pendleton", "Perot", "Peterson",
   "Phelps", "Pierce", "Pinkerton", "Potter",
   "Presley", "Pryor", "Quayle", "Rand",
   "Reagan", "Rehnquist", "Reynolds", "Richardson",
   "Roberts", "Robertson", "Robinson", "Rockefeller",
   "Romanov", "Romney", "Rove", "Rumsfeld",
   "Rutherford", "Santorum", "Savage", "Savoy",
   "Sawyer", "Saxe-Coburg-Gotha", "Scalia", "Schlafly",
   "Scott", "Shanahan", "Shaw", "Shelby",
   "Sherman", "Short", "Shuttleworth", "Simpson",
   "Skinner", "Smith", "Smithers", "Snow",
   "Snyder", "Spears", "Spencer", "Starr",
   "Sterling", "Stone", "Strauss", "Sullivan",
   "Swanson", "Sweeney", "Taft", "Taitz",
   "Taylor", "Templeton", "Thatcher", "Thomas",
   "Thompson", "Thurmond", "Trump", "Tucker",
   "Tudor", "Turner", "Tyler", "Upton",
   "Voight", "Walker", "Wallace", "Walsh",
   "Walton", "Warner", "Webster", "West",
   "Wettin", "White", "Wilcox", "Williams",
   "Wilson", "Windsor", "Wittelsbach", "Wood",
   "Wright", "Yates", "Young", "Zuckerberg"
   };

   strcpy(str,"");

   // For non-Arch-Conservatives, pick from ALL last names
   if(!archconservative)
   {
      // Roll on the number of non-Arch-Conservative last names,
      // plus the number of regular last names
      int roll = LCSrandom( ARRAY_ELEMENTS(regular_last_names) +
                        ARRAY_ELEMENTS(archconservative_last_names));
      // Decide whether to use an Arch-Conservative last name
      // or a regular last name
      archconservative = (roll >= ARRAY_ELEMENTS(regular_last_names));
   }
   // Now the archconservative variable might be true even if the function wasn't called that way,
   // but if it WAS called that way it's definitely true... this way non-Arch-Conservatives get
   // random last names out of all the last names, while Arch-Conservatives are limited to their own
   // last names

   if(archconservative)
      strcat(str, selectRandomString(archconservative_last_names, ARRAY_ELEMENTS(archconservative_last_names)));
   else
      strcat(str, selectRandomString(regular_last_names, ARRAY_ELEMENTS(regular_last_names)));
}
