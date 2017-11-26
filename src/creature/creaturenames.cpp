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

#include <externs.h>

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
      // Inaccuracy in estimating age should be the same every
      // time a character is queried. I'm using the day of the
      // month the character was born on to determine this.
      addstr(person.age+person.birthday_day%3-1);
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


std::string
generate_name(char gender)
{
  char name[POLITICIAN_NAMELEN];
  generate_name(name, gender);
  return name;
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
   "Aaron",   "Abdel",   "Abner",   "Abraham",
   "Adam",   "Adnan",   "Adolf",   "Adolph",
   "Ahmed",   "Aidan",   "Al",   "Alan",
   "Albert",   "Alec",   "Alejandro",   "Alexander",
   "Alexei",   "Alf",   "Alfie",   "Alfonse",
   "Alfred",   "Allan",   "Allen",   "Alphonse",
   "Alvaro",   "Alvin",   "Andr s",   "Andr‚",
   "Andrei",   "Andrew",   "Andy",   "Angelo",
   "Angus",   "Anthony",   "Antonio",   "Antony",
   "Archibald",   "Archie",   "Ari",   "Arnold",
   "Arp d",   "Art",   "Arthur",   "Attila",
   "Augusto",   "Austin",   "Avi",   "Axel",
   "Baal",   "Barack",   "Bashar",   "Basil",
   "Beltran",   "Barney",   "Barry",   "Bart",
   "Bartholomew",   "Beau",   "B‚la",   "Ben",
   "Benedict",   "Benito",   "Benjamin",   "Benny",
   "Benoit",   "Bernard",   "Bernie",   "Bert",
   "Bertrand",   "Bilal",   "Bill",   "Billy Bob",
   "Blake",   "Bob",   "Bohumil",   "Boleslaw",
   "Boris",   "Brad",   "Bradley",   "Brandon",
   "Brendon",   "Brent",   "Brian",   "Bronco",
   "Bruce",   "Brutus",   "Bryan",   "Bubba",
   "Buster",   "Byron",   "Cain",   "Cal",
   "Calvin",   "Carl",   "Carlos",   "Casimir",
   "Cassius",   "Cecil",   "C‚sar",   "Chad",
   "Chaim",   "Charles",   "Charlie",   "Chauncey",
   "Chester",   "Chuck",   "Chuckie",   "Christian",
   "Christopher",   "Ciar n",   "Cid",   "Claude",
   "Claudius",   "Cliff",   "Clifford",   "Clive",
   "Cliven",   "Clovis",   "Clyde",   "Colin",
   "Colton",   "Conrad",   "Cooper",   "Craig",
   "Csaba",   "Cyrus",   "Dale",   "Dallas",
   "Damien",   "Damon",   "Dan",   "Danforth",
   "Daniel",   "Danny",   "Dag",   "Dagwood",
   "Dave",   "David",   "Davy",   "Dean",
   "Deepak",   "Dennis",   "Denzel",   "Derak",
   "Derek",   "Derrick",   "Dexter",   "Dick",
   "Diego",   "Dilbert",   "Dinesh",   "Dink",
   "Dino",   "Diwakar",   "Dobromir",   "Doc",
   "Dom",   "Dominic",   "Don",   "Donald",
   "Donovan",   "Doug",   "Douglas",   "Dustin",
   "Dusty",   "Dwayne",   "Dwight",   "Dylan",
   "Earl",   "Ed",   "Eddie",   "Edgar",
   "Eduardo",   "Edward",   "Edwin",   "Emmanuel",
   "Emmett",   "Eli",   "Eliah",   "Elian",
   "Elias",   "Eliot",   "Elliot",   "Elmer",
   "Elvis",   "Emil",   "Emiliano",   "Enrico",
   "Enrique",   "Enzo",   "Eric",   "Erik",
   "Ernest",   "Ernie",   "Ernst",   "Ethan",
   "Eugene",   "Evan",   "Evo",   "Ezekiel",
   "Ezra",   "Fabio",   "Felix",   "Ferdinand",
   "Ferenc",   "Fernando",   "Fidel",   "Fischer",
   "Floyd",   "Forrest",   "Francesco",   "Francis",
   "Francisco",   "Frank",   "Franklin",   "Franz",
   "Fred",   "Freddy",   "Frederick",   "Fritz",
   "G bor",   "Gabriel",   "Garett",   "Garreth",
   "Garth",   "Gary",   "George",   "Gerald",
   "Geraldo",   "G‚za",   "Gilbert",   "Giovanni",
   "Glen",   "Glenn",   "Graham",   "Greg",
   "Gregory",   "Gus",   "Gustav",   "Guy",
   "Gy”rgy",   "Gyula",   "Hafez",   "Hagar",
   "Hakeem",   "Hamid",   "Hank",   "Hannibal",
   "Hans",   "Harold",   "Harry",   "Harvey",
   "Heinrich",   "Henry",   "Herb",   "Herbert",
   "Herman",   "Hiro",   "Hiroshi",   "Homer",
   "Howard",   "Howie",   "Hugh",   "Hugo",
   "Hunter",   "Ian",   "Igor",   "Ike",
   "Imre",   "Ingmar",   "Ira",   "Irving",
   "Isaac",   "Israel",   "Istv n",   "Ivan",
   "Jack",   "Jacob",   "Jacobo",   "Jake",
   "Jamel",   "James",   "J nos",   "Jared",
   "Jason",   "Javier",   "Jay",   "Jeb",
   "Jebediah",   "Jeff",   "Jefferson",   "Jeffrey",
   "Jeremiah",   "Jeremy",   "Jeroen",   "Jerome",
   "Jerry",   "Jesse",   "Jes£s",   "Jim",
   "Jimbo",   "Jimi",   "Jimmy",   "Jinyuan",
   "Joe",   "Joel",   "Joey",   "Johann",
   "John",   "Johnny",   "Jon",   "Jonathan",
   "Jorge",   "Jos‚",   "Josef",   "Joseph",
   "Josh",   "Joshua",   "Jowar",   "J¢zsef",
   "Juan",   "Judas",   "Jude",   "Julian",
   "Julius",   "Justin",   "Kaelen",   "Kamel",
   "Kane",   "Karl",   "K roly",   "Keith",
   "Ken",   "Kenny",   "Kenji",   "Kenneth",
   "Kevin",   "Khalil",   "Kirk",   "Kofi",
   "Konstantine",   "Kurt",   "Kweisi",   "Kyle",
   "Lachlan",   "Lajos",   "Lamar",   "Lance",
   "Lancelot",   "Larry",   "L szl¢",   "Lawrence",
   "Lenny",   "Leo",   "Leon",   "Leonard",
   "Leroy",   "Lev",   "Levi",   "Lex",
   "Liam",   "Linus",   "Lionel",   "Lloyd",
   "Lothar",   "Louis",   "Lowell",   "Lucas",
   "Lucifer",   "Lucius",   "Ludwig",   "Luigi",
   "Lu¡s",   "Luke",   "Luther",   "Lyle",
   "Malcolm",   "Manfred",   "Manny",   "Manuel",
   "Marc",   "Marco",   "Marcus",   "Marduk",
   "Marvin",   "Mark",   "Markos",   "Mario",
   "Martin",   "M rton",   "Marty",   "Mason",
   "Matt",   "Matthew",   "Matteo",   "M ty s",
   "Maurice",   "Maury",   "Maximilian",   "Mehmet",
   "Melvin",   "Michael",   "Mick",   "Mickey",
   "Miguel",   "Mike",   "Mikey",   "Mikhail",
   "Miles",   "Milton",   "Miroslav",   "Mitch",
   "Mitchell",   "Mitt",   "Moe",   "Mohammed",
   "Morris",   "Moses",   "Moshe",   "Mumtaz",
   "Murray",   "Mustafa",   "Myron",   "Napoleon",
   "Nate",   "Nathan",   "Nathaniel",   "Ned",
   "Neal",   "Neil",   "Nelson",   "Newt",
   "Newton",   "Nicholas",   "Nick",   "Noah",
   "Noam",   "Norm",   "Norman",   "Olaf",
   "Oliver",   "Oral",   "Osama",   "Oscar",
   "Otis",   "Otto",   "Owen",   "Pablo",
   "Pascal",   "Patrick",   "Patton",   "Paul",
   "Pedro",   "Percival",   "Percy",   "Pervez",
   "Pete",   "Peter",   "Petro",   "Phil",
   "Philip",   "Phillip",   "Phillipe",   "Pierre",
   "Piers",   "Prescott",   "Preston",   "Quentin",
   "Quincy",   "Qusay",   "Radoslav",   "Rafael",
   "Ralf",   "Ralph",   "Randall",   "Randy",
   "Rane",   "Ransom",   "Raphael",   "Ra£l",
   "Ray",   "Raymond",   "Reginald",   "Regis",
   "Rex",   "Ricardo",   "Rich",   "Richard",
   "Richie",   "Rick",   "Ricky",   "Ringo",
   "Rob",   "Robert",   "Robbie",   "Rod",
   "Rodney",   "Roger",   "Romeo",   "Ron",
   "Ronald",   "Rory",   "Roy",   "Rubin",
   "Rudolf",   "Rudolph",   "Rudy",   "Rufus",
   "Rupert",   "Rush",   "Russ",   "Russell",
   "Ryan",   "Saddam",   "Sambor",   "Samson",
   "Samuel",   "Sander",   "S ndor",   "Sanford",
   "Scott",   "Se n",   "Sebastian",   "Serge",
   "Sergei",   "Sergio",   "Seth",   "Shane",
   "Shaun",   "Shawn",   "Sheldon",   "Sherlock",
   "Sherman",   "Sherrod",   "Sid",   "Simon",
   "Sonny",   "Spike",   "Spiro",   "Stan",
   "Stanislaus",   "Stanislav",   "Stanley",   "Stephen",
   "Steve",   "Steven",   "Stewart",   "Stu",
   "Stuart",   "Tam s",   "Ted",   "Teddy",
   "Terence",   "Tetsuo",   "Theo",   "Theodore",
   "Thom",   "Thomas",   "Thor",   "Tiberius",
   "Tim",   "Timothy",   "Titus",   "Toby",
   "Todd",   "Tom",   "Tommy",   "Tony",
   "Toshiro",   "Travis",   "Trayvon",   "Trevor",
   "Tucker",   "Tyler",   "Tyrone",   "Tyson",
   "Uday",   "Uther",   "Vasily",   "Victor",
   "Viktor",   "Vince",   "Vincent",   "Vlad",
   "Vladimir",   "Vladislav",   "Vlastimil",   "Vyacheslav",
   "Wade",   "Wally",   "Walt",   "Walter",
   "Wayne",   "Wendell",   "Wilhelm",   "Will",
   "William",   "Willie",   "Willy",   "Winston",
   "Wojciech",   "Wolf",   "Woodrow",   "Woody",
   "Wyatt",   "Xander",   "Xavier",   "Yakov",
   "Yann",   "Yasser",   "Yoshi",   "Yusef",
   "Zachary",   "Zack",   "Zaid",   "Zbigniew",
   "Zebulon",   "Zeke",   "Zolt n",   "Zsigmond"
   };

   static const char *female_first_names[] =
   {
   "Abby",   "Abigail",   "Ada",   "Adriana",
   "Adrianna",   "Adrienne",   "Agatha",   "Agnes",
   "Aiko",   "Aileen",   "Alberta",   "Alene",
   "Aleksandra",   "Alexa",   "Alexandra",   "Alice",
   "Alicia",   "Alison",   "Allison",   "Alonya",
   "Alora",   "Alyson",   "Alyssa",   "Amanda",
   "Amber",   "Amelia",   "Amelie",   "Am‚rica",
   "Amy",   "Ana",   "Anastasia",   "Andrea",
   "Angela",   "Angelica",   "Angelina",   "Angelique",
   "Angie",   "Anisa",   "Anita",   "Ann",
   "Anna",   "Anne",   "Annemarie",   "Annie",
   "Annis",   "Anya",   "April",   "Ariadne",
   "Ariana",   "Arianna",   "Asma",   "Astrid",
   "Athena",   "Audra",   "Audrey",   "Aura",
   "Autumn",   "Ava",   "Ayla",   "Ayn",
   "Barb",   "Barbara",   "Barbie",   "Bea",
   "Beatrice",   "Beatrix",   "Bebe",   "Becca",
   "Becky",   "Bella",   "Belle",   "Bernadette",
   "Bernadine",   "Bernice",   "Bertha",   "Beth",
   "Beth Ann",   "Bethany",   "Betty",   "Bianca",
   "Blanche",   "Bonita",   "Bonnie",   "Bree",
   "Brenda",   "Briana",   "Brianne",   "Bridget",
   "Brigid",   "Brigitte",   "Britney",   "Brittany",
   "Brittney",   "Brooke",   "Brooklyn",   "Camilia",
   "Camilla",   "Candace",   "Candy",   "Carena",
   "Carina",   "Carla",   "Carmen",   "Carol",
   "Carolina",   "Caroline",   "Cass",   "Cassandra",
   "Cate",   "Catherine",   "Cathleen",   "Cathy",
   "Cecilia",   "Celine",   "Charity",   "Charlene",
   "Charlize",   "Charlotte",   "Chastity",   "Chelsea",
   "Cherry",   "Cheryl",   "Chlo‰",   "Christina",
   "Christine",   "Christy",   "Cindi",   "Cindy",
   "Claire",   "Clara",   "Clarissa",   "Clarity",
   "Claudette",   "Claudia",   "Cleofas",   "Cleopatra",
   "Colette",   "Colleen",   "Consuela",   "Contessa",
   "Corinne",   "Cornelia",   "Crystal",   "Cybill",
   "Cynthia",   "Daisy",   "Dakota",   "Daniela",
   "Danielle",   "Daphne",   "Dawn",   "Debbie",
   "Debra",   "Delilah",   "Denielle",   "Denise",
   "Desir‚e",   "Diana",   "Diane",   "Dimitra",
   "Dina",   "Donna",   "Dora",   "Doreen",
   "Doris",   "Dorothea",   "Dorothy",   "Dorottya",
   "Ebony",   "Edith",   "Edna",   "Eileen",
   "Ekaterina",   "Elaine",   "Eleanor",   "Elena",
   "Elinor",   "Eliza",   "Elizabeth",   "Ellen",
   "Eloise",   "Elsie",   "Elvira",   "Emily",
   "Emma",   "Entissar",   "Erica",   "Erika",
   "Erin",   "Eris",   "Erma",   "Erzs‚bet",
   "Esmerelda",   "Esther",   "Eudora",   "Euphemia",
   "Eva",   "Eve",   "Faith",   "Fanny",
   "Fatima",   "Faye",   "Felicia",   "Felicity",
   "Fiona",   "Flo",   "Flora",   "Florence",
   "Florida",   "Fran",   "Francine",   "Frida",
   "Gabby",   "Gabriela",   "Gabrielle",   "Galaxina",
   "Gana",   "Gavia",   "Georgia",   "Gertrude",
   "Gina",   "Giovanna",   "Gloria",   "Golda",
   "Grace",   "Greta",   "Gretchen",   "Gwen",
   "Gwendolyn",   "Gwyneth",   "Hailey",   "Hala",
   "Haley",   "Hannah",   "Harmony",   "Harriet",
   "Hayley",   "Hazel",   "Heather",   "Helen",
   "Helena",   "Helga",   "Hilda",   "Hollie",
   "Holly",   "Honor",   "Ida",   "Ilona",
   "Ilse",   "Ingrid",   "Irene",   "Iris",
   "Isabel",   "Isabella",   "Isabelle",   "Isla",
   "Ivana",   "Ivy",   "Izzy",   "Jaclyn",
   "Jada",   "Jade",   "Jane",   "Janet",
   "Janette",   "Janice",   "January",   "Jaqueline",
   "Jasmin",   "Jasmine",   "Jen",   "Jennifer",
   "Jenny",   "Jessica",   "Jill",   "Jillian",
   "Joan",   "Joann",   "Jodi",   "Jodie",
   "Josephine",   "Joy",   "Judith",   "Judy",
   "Julia",   "Juliana",   "Julie",   "Juliet",
   "Juliette",   "June",   "Justina",   "Justine",
   "Kaitlyn",   "Kaley",   "Karen",   "Karena",
   "Karla",   "Karmen",   "Katalin",   "Kate",
   "Katelin",   "Katerina",   "Katherine",   "Kathleen",
   "Kathy",   "Katie",   "Katrina",   "Kay",
   "Kayla",   "Kaylee",   "Keely",   "Keisha",
   "Kiki",   "Kirsten",   "Klara",   "Kristal",
   "Kristen",   "Kristie",   "Kristin",   "Krystal",
   "Lacey",   "Lainie",   "Lana",   "Lara",
   "Larissa",   "Latoya",   "Laura",   "Lauren",
   "Laurie",   "Lavender",   "Lea",   "Leah",
   "Leela",   "Leia",   "Leila",   "Lena",
   "Lexi",   "Leyna",   "Liliana",   "Lilith",
   "Lilly",   "Lily",   "Lina",   "Linda",
   "Lisa",   "Liv",   "Liz",   "Liza",
   "Lizabeth",   "Lizbeth",   "Lizzie",   "Lois",
   "Lorena",   "Loretta",   "Lorraine",   "Lory",
   "Louane",   "Louanne",   "Louisa",   "Louise",
   "Luca",   "Lucia",   "Lucy",   "Lukki",
   "Luisa",   "Luna",   "Lydia",   "Lyn",
   "Lynn",   "Lynnette",   "Lyra",   "Madonna",
   "Mags",   "Malia",   "Mandy",   "Mango",
   "Manuela",   "Marcia",   "Marcie",   "Margaret",
   "Margarete",   "Margarita",   "Marge",   "Margit",
   "Maria",   "Marian",   "Mariana",   "Marianne",
   "Maribel",   "Marie",   "Marielle",   "Marilyn",
   "Marina",   "Marion",   "Marle",   "Marlena",
   "Marlene",   "Marsha",   "Marta",   "Martha",
   "Martina",   "Mary",   "Mary Ann",   "Matilda",
   "Maura",   "Maureen",   "Maxine",   "Maya",
   "Mayumi",   "Mazie",   "Medea",   "Meg",
   "Megan",   "Meghan",   "Megumi",   "Melanie",
   "Melinda",   "Melissa",   "Melody",   "Melvanie",
   "Mercedes",   "Merissa",   "Mia",   "Michele",
   "Michelle",   "Mika",   "Milena",   "Milly",
   "Mimi",   "Minal",   "Mindy",   "Minnie",
   "Miranda",   "Molly",   "Monica",   "Monique",
   "M•rag",   "Nadezhda",   "Nadia",   "Nadine",
   "Nadya",   "Nan",   "Nancy",   "Naomi",
   "Natalia",   "Natalie",   "Natasha",   "Nel",
   "Nellie",   "Nelly",   "Nicole",   "Nicolette",
   "Nina",   "Noemi",   "Nora",   "Norma",
   "October",   "Olga",   "Olivia",   "Oprah",
   "Orly",   "Pam",   "Pamela",   "Pandora",
   "Patricia",   "Patty",   "Paula",   "Paulette",
   "Pauline",   "Peach",   "Pearl",   "Peggy",
   "Penelope",   "Penny",   "Petunia",   "Phoebe",
   "Phyllis",   "Piper",   "Piroska",   "Polina",
   "Polly",   "Prachi",   "Priscilla",   "Prudence",
   "Rachael",   "Racheal",   "Rachel",   "Rachelle",
   "Raghad",   "Ramona",   "Rana",   "Rania",
   "Raphaela",   "Raquel",   "Raylene",   "Rebecca",
   "Regina",   "Ren‚e",   "Rita",   "Romi",
   "Rosa",   "Rosalie",   "Rosalind",   "Rosalinda",
   "Rosanna",   "Rosanne",   "Roberta",   "Rose",
   "Rosemary",   "Rosie",   "Roxane",   "Roxanne",
   "Roz",   "Ruth",   "Ruthanne",   "Sabrina",
   "Sabrise",   "Sajida",   "Sally",   "Samantha",
   "Samia",   "Samira",   "Sandra",   "Sara",
   "Sarah",   "Satya",   "Sehba",   "Selma",
   "Serena",   "Shaniqua",   "Shanna",   "Sharon",
   "Shenna",   "Sherry",   "Shirley",   "Shyann",
   "Siƒn",   "Silvia",   "Sofia",   "Sofy",
   "Sonali",   "Sonia",   "Sophia",   "Sophie",
   "Soraya",   "Stacie",   "Stacy",   "Steph",
   "Stephanie",   "Suha",   "Summer",   "Sue",
   "Susan",   "Susanna",   "Suzanne",   "Suzie",
   "Svetlana",   "Sylvia",   "Tabatha",   "Tabitha",
   "Tamara",   "Tamaya",   "Tammy",   "Tara",
   "Tawanda",   "Teresa",   "Terri",   "Tessa",
   "Tetiana",   "Theresa",   "Thyra",   "Tia",
   "Tiff",   "Tiffanie",   "Tiffany",   "Tina",
   "Toni",   "Tori",   "Tricia",   "Trish",
   "Trudy",   "Tyra",   "Valentina",   "Valeria",
   "Valerie",   "Vampira",   "Vanessa",   "Venus",
   "Verena",   "Veronica",   "Vesna",   "Virginia",
   "Vicki",   "Vicky",   "Victoria",   "Violet",
   "Vithara",   "Vivian",   "Vonnita",   "Wanda",
   "Wendy",   "Whoopi",   "Wilhemina",   "Willow",
   "Wilma",   "Winifred",   "Xenia",   "Yekaterina",
   "Yoko",   "Yolanda",   "Yulia",   "Yvonne",
   "Zahwa",   "Zarina",   "Zeenat",   "Zelda",
   "Zo‰",   "Zofia",   "Zs¢fia",   "Zuzana"
   };

   // these names should not appear in the male or female
   // or great white male patriarch first name lists, only here
   static const char *gender_neutral_first_names[] =
   {
   "Adrian",   "Akira",   "Alex",   "Alexis",
   "Ali",   "Angel",   "Ariel",   "Ashley",
   "Beverly",   "Billy",   "Blair",   "Bobbie",
   "Bobby",   "Brett",   "Brit",   "Cameron",
   "Camille",   "Casey",   "Chas",   "Chaz",
   "Chris",   "Cody",   "Cory",   "Courtney",
   "Dana",   "Frankie",   "Hayden",   "Hillary",
   "Kelly",   "Jackie",   "Jamie",   "Jayden",
   "Jean",   "Jess",   "Jo",   "Jordan",
   "Joyce",   "Jun",   "Kelsey",   "Kelsie",
   "Kennedy",   "Kim",   "Kimberley",   "Kimberly",
   "Kris",   "Lee",   "Les",   "Lesley",
   "Leslie",   "Lindsay",   "Lindsey",   "Logan",
   "Lou",   "Madison",   "Mao",   "Max",
   "Mel",   "Morgan",   "Nat",   "Nickie",
   "Nicky",   "No‰l",   "Ollie",   "Page",
   "Paige",   "Paris",   "Pat",   "Ranma",
   "Raven",   "Riley",   "Robin",   "Rowan",
   "Sam",   "Sandy",   "Sasha",   "Shannon",
   "Sidney",   "Stevie",   "Taylor",   "Terry",
   "Tracy",   "Val",   "Yanping",   "Yuri"
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
      nametable = GENDER_WHITEMALEPATRIARCH;

   // Assign a name from the available names for each gender
   if(gender == GENDER_MALE)
   {
      // Roll on the number of gender-specific names,
      // plus the number of gender-neutral names
      roll = LCSrandom(len(male_first_names) +
                       len(gender_neutral_first_names));
      // Decide whether to use a gender-specific name
      // or a gender-neutral name
      if(roll >= len(gender_neutral_first_names))
         nametable = GENDER_MALE;
      else
         nametable = GENDER_NEUTRAL;
   }
   else if(gender == GENDER_FEMALE)
   {
      // (Same here, just use the number of female names instead)
      roll = LCSrandom(len(female_first_names) +
                       len(gender_neutral_first_names));
      if(roll >= len(gender_neutral_first_names))
         nametable = GENDER_FEMALE;
      else
         nametable = GENDER_NEUTRAL;
   }

   if(nametable == GENDER_MALE)
      strcat(str, pickrandom(male_first_names));
   else if(nametable == GENDER_FEMALE)
      strcat(str, pickrandom(female_first_names));
   else if(nametable == GENDER_NEUTRAL)
      strcat(str, pickrandom(gender_neutral_first_names));
   else if(nametable == GENDER_WHITEMALEPATRIARCH)
      strcat(str, pickrandom(great_white_male_patriarch_first_names));
   else
      strcat(str, "Errol");
}
/* gets a random last name */
void lastname(char *str, bool archconservative)
//{{{ Last Name
{
   // note: every last name should only occur ONCE in either this list
   // or the Arch-Conservative list
   static const char *regular_last_names[] =
   {
   "Abbey", "Abbott", "Abbzug", "Abramowicz",
   "Adamson", "Adkins", "Africa", "Aguilera",
   "Ahmed", "Ahn", "Alanson", "Alcott",
   "Alda", "Ali", "Allanshaw", "Alley",
   "Alvarez", "Andress", "Aniston", "Annan",
   "Antle", "Appleton", "Arafat", "Armstead",
   "Arnaz", "Artfield", "Ashmore", "al-Assad",
   "Assange", "Ayers", "Aykroyd", "Azoulay",
   "Bacall", "Bacon", "Baird", "Baldwin",
   "Balgos", "Balogh", "B r ny", "Barrow",
   "Barwick", "Beaumont", "Becker", "Bendix",
   "Biden", "Birkin", "Bitton", "Blanchard",
   "Blitzer", "Bogart", "Boggs", "Bolger",
   "Booker", "Bosanquet", "Bradbury", "Bradley",
   "Bradshaw", "Brandis", "Brewster", "Breyer",
   "Brightman", "Briscoe", "Brownlee", "Bryant",
   "Buckman", "Buckner", "Bugbee", "Bump",
   "Bunton", "Burghoff", "Butcher", "Butler",
   "Butt", "Butterman", "Buttman", "Carberry",
   "Carr", "Carson", "Carter", "Cartwright",
   "Castro", "Cavallero", "Cavill", "Chamberlain",
   "Chan", "Chandler", "Chang", "Channing",
   "Chao", "Chau", "Ch vez", "Chen",
   "Cheng", "Childress", "Childs", "Chilton",
   "Chiu", "Cho", "Choi", "Chomsky",
   "Chow", "Chu", "Chubb", "Chung",
   "Clavin", "Clinton", "Clooney", "Coffey",
   "Cohen", "Colcord", "Colombe", "Conner",
   "Coopersmith", "Copeland", "Coppola", "Cordero",
   "Cosgrove", "Cottrell", "Court", "Craft",
   "Critchley", "Croft", "Cromwell", "Cross",
   "Crowe", "Crowson", "Cruise", "Cuomo",
   "Curnew", "Cutlass", "Cutler", "Dahan",
   "Damon", "Danes", "Dangerfield", "Dante",
   "Darnand", "Dash", "Dawkins", "Daveson",
   "Day", "Dean", "Delgado", "DeLorean",
   "Dench", "De Niro", "Dent", "Dentz",
   "D¡az", "DiMeo", "Dom¡nguez", "Dorgan",
   "Dorsey", "Doucette", "Douglas", "Downey",
   "Downing", "Drummond", "Duell", "Dukakis",
   "Duval", "Earle", "Ebert", "Edwards",
   "Einstein", "English", "Escobar", "Ericson",
   "Eubanks", "al-Fadil", "Fairbanks", "Fairchild",
   "Falco", "Falstaff", "Farkas", "Farnsworth",
   "Farr", "Faulkner", "Feingold", "Fekete",
   "Fenoglio", "Fiddlebottom", "Filler", "Fimple",
   "Fingleton", "Finn", "Fl¢rez", "Fortmuller",
   "France", "Frank", "Franken", "Frost",
   "Friend", "Fudge", "Fulsom", "Furley",
   "Fushimi", "Galindo", "Gao", "Garc¡a",
   "Garnier", "Gatien", "Gelbart", "Gephardt",
   "Gilbert", "Gillmouth", "Gilpin", "Ginsberg",
   "Ginsburg", "Godwin", "Gleaves", "Goldberg",
   "Goldstein", "G¢mez", "Gonz lez", "Goodman",
   "Gore", "Gorbachev", "Govorov", "Grant",
   "Grayson", "Green", "Greenberg", "Grijalva",
   "Grover", "Gu", "Guo", "Gupta",
   "Guti‚rrez", "Guy", "Hal sz", "Hall",
   "Hammond", "Han", "Hann", "Hannemann",
   "Hatcher", "Hawking", "Hayashi", "Hayes",
   "Haymer", "He", "Headley", "Heath",
   "Hellmann", "Helton", "Hemingway", "Hendren",
   "Hendrix", "Hepburn", "Hepditch", "Hewson",
   "Heyduke", "Hiaasen", "Hilliard", "Himelstein",
   "Ho", "Hoddinott", "Holder", "Honda",
   "Hong", "Hooker", "Hope", "Hopper",
   "Hornberger", "Horv th", "Houghton", "Houseman",
   "Howard", "Howe", "Hsu", "Hu",
   "Huang", "Huffington", "Hunnicutt", "Hussein",
   "Hutchison", "Hwang", "Ibarra", "Incledon",
   "Inking", "Inoue", "Irvin", "Irving",
   "Iscariot", "Issacson", "It“", "Ivanson",
   "Ives", "Jang", "Jefferson", "Jeong",
   "Jimison", "Jolie", "Jordan", "Joyce",
   "Juh sz", "Justice", "Kagan", "Kaltenbrunner",
   "Kang", "Kao", "Kardashian", "Karzai",
   "Kat“", "Keaton", "Keats", "Keene",
   "Keitel", "Kennedy", "Kershaw", "Kesten",
   "Kilbourne", "Kilmer", "Kim", "Kimura",
   "Kindale", "Kinslow", "Kiss", "Kitchen",
   "Kobayashi", "Kokernot", "Koprowski", "Kov cs",
   "Krasow", "Krieger", "Krishna", "Krugman",
   "Kucinich", "Kunstler", "Kurosawa", "Kwok",
   "Kwon", "bin Laden", "Lakatos", "Lam",
   "LaSaga", "Lauder", "Law", "Lee",
   "Leeves", "Lengyel", "Lenin", "Lennon",
   "Levy", "Lewinsky", "Leyden", "Li",
   "Lim", "Lin", "Lincoln", "Linville",
   "Lipman", "Liu", "Lo", "Loeb",
   "Logsdon", "Lombard", "London", "L¢pez",
   "Lovecraft", "Lowcourt", "Lowerly", "Lucas",
   "Luna", "Luo", "Lushington", "Luzinski",
   "Lyles", "Lynam", "Van Lynden", "Lyons",
   "Ma", "MacKinnon", "MacLeod", "MacNeil",
   "Macy", "Maddow", "Malach", "Malkovich",
   "Malone", "Mandela", "Mann", "Manning",
   "Mansfield", "March", "Marsden", "Marsh",
   "Marshall", "Marx", "Mathers", "Matheson",
   "Mathewson", "Matthews", "Matsumoto", "Maxwell",
   "McAdams", "McAllister", "McBroom", "McCorkell",
   "McFadden", "McFatridge", "McFaul", "McFly",
   "McGee", "McGovern", "McGrath", "McGraw",
   "McGuinness", "McIntosh", "McIntyre", "McKenzie",
   "McKinney", "McLeod", "McMurray", "McNabb",
   "McPhail", "McPherson", "McQueen", "Meek",
   "Meffert", "Meir", "Mel‚ndez", "Men‚ndez",
   "M‚sz ros", "Metzger", "Mifune", "Milburn",
   "Miyamoto", "Mizrahi", "Mohammed", "Moln r",
   "Mondale", "Montag", "Montalvo", "Molotov",
   "Moon", "Moore", "Morales", "Morrison",
   "Morrow", "Morton", "Mosley", "Mller",
   "Murrell", "Muscat", "Musharraf", "Muskhelishvili",
   "Mussa", "Myatt", "Nader", "Nagel",
   "Nagelberg", "Nagy", "Nakagawa", "Nakahara",
   "Nakamura", "Nallet", "Nance", "Nash",
   "Nathanson", "Necker", "N‚meth", "Nettles",
   "Neuwirth", "Nevinson", "Newberry", "Newhook",
   "Ng", "Nicolle", "Nickson", "O",
   "Obama", "Obeid", "Oberlander", "Ochs",
   "Ogden", "Oh", "Ol h", "Olbermann",
   "Olde", "Olds", "Oldsmar", "Ollerenshaw",
   "Orlando", "Ortiz", "Osborne", "Pacino",
   "Paltrow", "Paoletti", "Palm", "Papp",
   "Park", "Parking", "Parr", "Patel",
   "Patterson", "Pearson", "Peck", "Pegg",
   "Pelosi", "Pelton", "Pendergrass", "Penn",
   "Peretz", "P‚rez", "Pert", "Petermann",
   "Pfeiffer", "Pickett", "Picketty", "Piercey",
   "Pitt", "Pommerencke", "Pomper", "Porcaro",
   "Porter", "Postman", "Pound", "Powers",
   "Probst", "Proudfoot", "Puente", "Purcell",
   "Putin", "Quaid", "Queen", "Quennell",
   "Quick", "Quilico", "Quilliam", "Quilty",
   "Quincey", "Quinlivan", "Quinn", "Quirk",
   "Rabban", "Rackard", "R cz", "Radman",
   "Rainey", "Ram¡rez", "Ramos", "Randell",
   "Rangel", "Ransom", "Rappaport", "Raskin",
   "Rastrelli", "Rather", "Ratzenberger", "Ravenhill",
   "Rawls", "Read", "Reading", "Reasoner",
   "Van Rechteren", "Redford", "Redman", "Redwine",
   "Reed", "Reekers", "Reid", "Rendell",
   "Rexford", "Ricci", "Richter", "Rideout",
   "Riffle", "Rivera", "Rivers", "Rodham",
   "Rodr¡guez", "Rojo", "Roland", "Roosevelt",
   "Rose", "Rosenberg", "Ross", "Rota",
   "Rothstein", "Roy", "Roylance", "Rudkin",
   "Rush", "Rushworth", "LaRussa", "Russell",
   "Ryman", "Sabella", "Saddler", "Sadler",
   "Sait“", "S nchez", "Sanders", "Sansome",
   "Santiago", "Santos", "Sasaki", "Sat“",
   "Savedra", "Saxton", "Scales", "Scardino",
   "Schmitt", "Schneider", "Schr”der", "Schmidt",
   "Schumacher", "Schumer", "Schuster", "Scully",
   "Scurfield", "Sealock", "Seay", "Seinfeld",
   "Sellers", "Semin", "Seo", "Serling",
   "Sewall", "Sexton", "Shackleton", "Shagwell",
   "al-Shamie", "Sharpe", "Sharpton", "Shedd",
   "Sheen", "Shelton", "Shengde", "Shepard",
   "Shepardson", "Sheppard", "Shew", "Shields",
   "Shiller", "Shimazu", "Shimizu", "Shin",
   "Shipman", "Shipp", "Sho", "Shrapnal",
   "Shrum", "Schulz", "Shumway", "Shurin",
   "Sidebottom", "Siegel", "Siever", "da Silva",
   "Silverman", "Silverstein", "Simmons", "Simms",
   "Simon", "Singh", "al-Sisi", "Skidmore",
   "Skirrow", "Skutnik", "Skye", "Skyler",
   "Slaten", "Smalley", "Smallwood", "Smart",
   "Smyth", "Snead", "Snook", "Snowden",
   "Sol¡s", "Sommer", "Somner", "Song",
   "Soren", "Sotomayor", "Souter", "Spacey",
   "Spielberg", "Spasov", "Spitzer", "Stalin",
   "Stang", "Stark", "Starkweather", "Stassen",
   "Stelling", "Stephenson", "Sternin", "Stevens",
   "Stevenson", "Stewart", "Stockton", "Storm",
   "Storms", "Stiver", "Strain", "Straley",
   "Straub", "Stripe", "Strossen", "Suave",
   "Suiter", "Sulaiman", "Sun", "Surnow",
   "Susman", "Suzuki", "Swain", "Swaine",
   "Swift", "Swit", "Symington", "Symonds",
   "Szab¢", "Tabb", "Tailboys", "Tak cs",
   "Takahashi", "Takayoshi", "Talbert", "Talbot",
   "Talbott", "Talty", "Tanaka", "Tanner",
   "Tarantino", "Tatters", "Taubman", "Tayo",
   "Teige", "Tell", "Tennent", "Tenney",
   "Tenpenny", "Terrill", "Testore", "Tetley",
   "Thalberg", "Thomaz", "Thomson", "Thorne",
   "Threepwood", "Thurgood", "Thurlow", "Tickell",
   "Tillett", "Tillinghast", "Tillson", "Tillsley",
   "Timmonds", "Tippett", "Tisdall", "Tishman",
   "Titcomb", "Tobin", "Todd", "Todhunter",
   "Toft", "Tolbert", "Tolkien", "Tolson",
   "Tonelli", "Toogood", "Tooke", "Toole",
   "T¢th", "Towne", "Townsend", "Trafford",
   "Travolta", "Trebek", "Trotsky", "Truman",
   "Truong", "Tsui", "Tubbs", "Tubman",
   "Twain", "Tymchyshyn", "Ulmer", "Underhill",
   "Underdown", "Underwood", "Updike", "Usher",
   "Ussher", "Valdez", "Vallance", "Vance",
   "Varga", "Vasilevsky", "Venus", "Villa",
   "Villanueva", "Viscuso", "Voland", "Vulcar",
   "Wade", "Wadsworth", "Walden", "Walwyn",
   "Wang", "Ward", "Warman", "Warr",
   "Warren", "Washington", "Watanabe", "Waxman",
   "Weathers", "Webber", "Weber", "Weeks",
   "Weller", "Wellstone", "Wendt", "Westlake",
   "Weston", "Weygand", "Whitehead", "Whitley",
   "Wickes", "Willett", "Williamson", "Willman",
   "Winchester", "Winfrey", "Winkelman", "Winslow",
   "Winter", "Wiseman", "Wolfe", "Wong",
   "Woodrow", "Woods", "Wu", "Wuornos",
   "Xu", "Xue", "Yamada", "Yamaguchi",
   "Yamamoto", "Yang", "Yankovic", "Yee",
   "Yeltsin", "Yerkovich", "Yi", "Yim",
   "Yoo", "Yoon", "Yoshida", "Youngblood",
   "Yu", "Yue", "Zachary", "Zadora",
   "Zakharov", "Zale", "Zapata", "Zappa",
   "Zedner", "Zedong", "Zhang", "Zhao",
   "Zhou", "Zhu", "Zicree", "Zito",
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
   "Adams", "Adelson", "Agnew", "Akin",
   "Alexander", "Alito", "Allen", "Ames",
   "Anderson", "Armey", "Armstrong", "Arnold",
   "Ashcroft", "Atwater", "Bachmann", "Bailey",
   "Baker", "Bakker", "Banks", "Barr",
   "Bates", "Baxter", "Beck", "Bennett",
   "Berkowitz", "Bernadotte", "Bezos", "Bianchi",
   "Bilderberg", "Black", "Blankenship", "Blum",
   "Boehner", "Bolton", "Bonaparte", "Bourbon",
   "Boyd", "Bozell", "Braganza", "Brandt",
   "Braun", "Breitbart", "Bremer", "Brooks",
   "Brown", "Buchanan", "Buckley", "Bundy",
   "Buono", "Burns", "Burr", "Burton",
   "Bush", "Byrd", "Callahan", "Cameron",
   "Campbell", "Cantor", "Carey", "Carlson",
   "Carlyle", "Carnegie", "Carpenter", "Chaffee",
   "Chambers", "Cheney", "Churchill", "Clark",
   "Clay", "Cobb", "Colbert", "Cole",
   "Coleman", "Collins", "Colt", "Cook",
   "Cooper", "Corll", "Coughlin", "Coulter",
   "Cox", "Crane", "Crockett", "Cruz",
   "Cunningham", "Dahmer", "Danson", "Darby",
   "Davidson", "Davis", "Decker", "Deere",
   "Delaney", "DeLay", "DeMint", "Diehl",
   "Discordia", "Disney", "Dixon", "Dobbs",
   "Dobson", "Dole", "Doyle", "Drake",
   "D'Souza", "Duke", "Duffield", "Dunham",
   "Dunlap", "Earp", "Ellison", "Evans",
   "Falwell", "Fawkes", "Fenderson", "Fields",
   "Fischer", "Fisher", "Fitzgerald", "Fitzpatrick",
   "Flake", "Flanders", "Fletcher", "Forbes",
   "Ford", "Foster", "Fox", "Franco",
   "Franklin", "Friedman", "Gacy", "Gallagher",
   "Galt", "Gates", "Gein", "Getty",
   "Gibson", "Gingrich", "Giuliani", "Glcksburg",
   "Gohmert", "Goldwater", "Goode", "Goodyear",
   "Graham", "Grammer", "Gray", "Gregg",
   "Griffin", "Grimaldi", "Hagee", "Hamilton",
   "Hannity", "Hanover", "Hansen", "Hapsburg",
   "Harper", "Harris", "Hastert", "Heidnik",
   "Heller", "Helms", "Henderson", "Henley",
   "Hess", "Hill", "Hinn", "Hitler",
   "Hoffmann", "Hogan", "Hohenzollern", "Holliday",
   "Holmes", "Hoover", "Hopkins", "Huckabee",
   "Hudson", "Hughes", "Jackson", "James",
   "Jameson", "Jenkins", "Johnson", "Jones",
   "Judge", "Kemper", "Kent", "Keyes",
   "King", "Kissinger", "Koch", "Koresh",
   "Kraft", "Krauthammer", "Kristol", "Kuklinski",
   "Lake", "Langley", "Lewis", "Limbaugh",
   "Little", "Logan", "Lott", "Lowe",
   "Luxembourg", "Lynch", "MacArthur", "MacGyver",
   "Mahoney", "McCain", "McCarthy", "McConnell",
   "McDonald", "McKinley", "McLaughlin", "McVeigh",
   "Medici", "Meyer", "Miller", "Mitchell",
   "Monroe", "Montana", "Morgan", "Morris",
   "Mullin", "Murphy", "Murray", "Mussolini",
   "Murdoch", "Myers", "Nelson", "Ness",
   "Newman", "Nielson", "Nixon", "North",
   "Novak", "Oakley", "O'Connor", "O'Keefe",
   "Orange", "O'Reilly", "Osman", "Osteen",
   "Palin", "Palmer", "Parker", "Paul",
   "Pendleton", "Perry", "Perot", "Peterson",
   "Phelps", "Pierce", "Pinkerton", "Potter",
   "Powell", "Presley", "Pryor", "Quayle",
   "Rader", "Rand", "Reagan", "Rehnquist",
   "Reynolds", "Rhodes", "Rice", "Richardson",
   "Roberts", "Robertson", "Robinson", "Rockefeller",
   "Romanov", "Romney", "Rothschild", "Rove",
   "Rubio", "Rumsfeld", "Rutherford", "Sanford",
   "Santorum", "Savage", "Savoy", "Sawyer",
   "Saxe-Coburg-Gotha", "Scalia", "Schlafly", "Scott",
   "Shanahan", "Shaw", "Shawcross", "Shelby",
   "Sherman", "Short", "Shuttleworth", "Simpson",
   "Skinner", "Smith", "Smithers", "Snow",
   "Snyder", "Sowell", "Spears", "Spencer",
   "Starr", "Sterling", "Stone", "Strauss",
   "Stuart", "Sullivan", "Sutcliffe", "Swaggart",
   "Swanson", "Sweeney", "Taft", "Taitz",
   "Taylor", "Templeton", "Thatcher", "Thomas",
   "Thompson", "Thurmond", "Trudeau", "Trump",
   "Tucker", "Tudor", "Turner", "Tyler",
   "Unruh", "Upton", "Voight", "Wagner",
   "Walker", "Wallace", "Walsh", "Walton",
   "Warner", "Watson", "Wayne", "Webster",
   "West", "Wettin", "White", "Whitman",
   "Wilcox", "Williams", "Wilson", "Windsor",
   "Wittelsbach", "Wood", "Wright", "Yates",
   "Yoder", "Young", "Zimmerman", "Zuckerberg"
   };

   strcpy(str,"");

   // For non-Arch-Conservatives, pick from ALL last names
   if(!archconservative)
   {
      // Roll on the number of non-Arch-Conservative last names,
      // plus the number of regular last names
      int roll = LCSrandom(len(regular_last_names) +
                           len(archconservative_last_names));
      // Decide whether to use an Arch-Conservative last name
      // or a regular last name
      archconservative = (roll >= len(regular_last_names));
   }
   // Now the archconservative variable might be true even if the function wasn't called that way,
   // but if it WAS called that way it's definitely true... this way non-Arch-Conservatives get
   // random last names out of all the last names, while Arch-Conservatives are limited to their own
   // last names

   if(archconservative)
      strcat(str, pickrandom(archconservative_last_names));
   else
      strcat(str, pickrandom(regular_last_names));
}
