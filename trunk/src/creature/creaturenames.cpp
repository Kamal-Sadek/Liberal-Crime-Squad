#include "includes.h"

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
      addstr(", Ambiguous");

   // Note if there's some conflict with Conservative society's perceptions
   if(person.gender_liberal != person.gender_conservative)
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
   }
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
   static const char *male_first_names[] =
   {
   "Aaron", "Abraham",   "Adam",   "Adolph",
   "Adrian",   "Al",   "Alan",   "Albert",
   "Alec",   "Alejandro",   "Alexander",   "Alf",
   "Alfie",   "Alfred", "Ali",   "Allan",
   "Allen",   "Andrew",   "Andy",   "Anthony",
   "Antonio",   "Archie",   "Arnold",   "Art",
   "Arthur",   "Avi",   "Axel",   "Barack",
   "Barney",   "Barry",   "Bartholomew",   "Beau",
   "Ben",   "Benjamin",   "Bernard",   "Bert",
   "Bill",   "BillyBob",   "Blaire",   "Blake",
   "Bob",   "Bobby",   "Boris",   "Brad",
   "Bradley",   "Brandon",   "Brendon",   "Brett",
   "Brian",   "Bruce",   "Bryan",   "Calvin",
   "Carl",   "Carlos",   "Cesar",   "Chad",
   "Charles",   "Charlie",   "Chester",   "Chuck",
   "Chris",   "Christian",   "Christopher",   "Claude",
   "Cliff",   "Clifford",   "Clyde",   "Cody",
   "Colin",   "Colton",   "Conrad",   "Cory",
   "Craig",   "Dale",   "Damon",   "Dan",
   "Danforth",   "Daniel",   "Danny",   "Dag",
   "Dave",   "David",   "Deepak",   "Dennis",
   "Derak",   "Derek",   "Derrick",   "Dexter",
   "Dick",   "Diego",   "Diwakar",   "Dominic",
   "Donald",   "Donovan",   "Doug",   "Douglas",
   "Dustin",   "Dwayne",   "Dwight",   "Eathan",
   "Ed",   "Eddie",   "Edgar",   "Eduardo",
   "Edward",   "Edwin",   "Eli",   "Eliah",
   "Elliot",   "Enrique",   "Enzo",   "Eric",
   "Erik",   "Ernest",   "Ernie",   "Eugene",
   "Evan",   "Fabio",   "Lyle",   "Felix",
   "Fernando",   "Floyd",   "Francesco",   "Francis",
   "Frank",   "Franklin",   "Fred",   "Frederick",
   "Gabriel",   "Garett",   "Garreth",   "Gary",
   "George",   "Gerald",   "Glen",   "Graham",
   "Greg",   "Gregory",   "Gus",   "Guy",
   "Hank",   "Hans",   "Harold",   "Harry",
   "Harvey",   "Henry",   "Herbert",   "Homer",
   "Howard",   "Howie",   "Hugh",   "Hugo",
   "Ian",   "Isaac",   "Issac",   "Ivan",
   "Jack",   "Jacob",   "Jake",   "Jamel",
   "James",   "Jared",   "Jason",   "Javier",
   "Jay",   "Jean",   "Jeff",   "Jeffrey",
   "Jeremy",   "Jeroen",   "Jerome",   "Jerry",
   "Jesse",   "Jesus",   "Jim",   "Jimmy",
   "Jinyuan",   "Joe",   "Joel",   "Joey",
   "John",   "Johnny",   "Jon",   "Jonathan",
   "Jordan",   "Jorge",   "Jose",   "Joseph",
   "Josh",   "Joshua",   "Jowar",   "Juan",
   "Julian",   "Justin",   "Kaelen",   "Karl",
   "Keith",   "Ken",   "Kenji",   "Kennedy",
   "Kenneth",   "Kevin",   "Kirk",   "Konstantine",
   "Kurt",   "Kweisi",   "Kyle",   "Lance",
   "Larry",   "Lawrence",   "Lenny",   "Leo",
   "Leonard",   "Leroy",   "Lex",   "Liam",
   "Lionel",   "Louis",   "Lucas",   "Luis",
   "Luke",   "Malcolm",   "Manny",   "Manuel",
   "Marcus",   "Marvin",   "Mark",   "Mario",
   "Martin",   "Marty",   "Mason",   "Matt",
   "Matthew",   "Maurice",   "Max",   "Mel",
   "Michael",   "Mickey",   "Miguel",   "Mike",
   "Miles",   "Mitchell",   "Moe",   "Mumtaz",
   "Murray",   "Mustafa",   "Nathan",   "Ned",
   "Neal",   "Neil",   "Nelson",   "Nicholas",
   "Nick",   "Noah",   "Norm",   "Norman",
   "Oliver",   "Otto",   "Owen",   "Pablo",
   "Patrick",   "Paul",   "Pedro",   "Pete",
   "Peter",   "Phil",   "Phillip",   "Preston",
   "Rafael",   "Ralf",   "Ralph",   "Randy",
   "Rane",   "Ransom",   "Raphael",   "Raul",
   "Ray",   "Raymond",   "Reginald",   "Ricardo",
   "Richard",   "Rick",   "Robert",   "Rod",
   "Rodney",   "Roger",   "Ron",   "Ronald",
   "Rory",   "Roy",   "Rubin",   "Rudy",
   "Rupert",   "Russ",   "Russell",   "Ryan",
   "Samuel",   "Scott",   "Sean",   "Sebastian",
   "Serge",   "Sergio",   "Seth",   "Shane",
   "Shaun",   "Shawn",   "Sid",   "Simon",
   "Sonny",   "Stan",   "Stanley",   "Stephen",
   "Steve",   "Steven",   "Stewart",   "Stu",
   "Stuart",   "Ted",   "Tetsuo",   "Theodore",
   "Thomas",   "Thor",   "Tim",   "Timothy",
   "Toby",   "Tom",   "Tony",   "Toshiro",
   "Travis",   "Trevor",   "Tyler",   "Tyrone",
   "Victor",   "Vincent",   "Wade",   "Wally",
   "Walter",   "Wayne",   "William",   "Willy",
   "Winston",   "Wolf",   "Woody",   "Xavier",
   "Zachary",   "Zeke",   "Quentin"
   };

   static const char *female_first_names[] =
   {
   "Abigail",   "Ada",   "Adriana",   "Adrianna",
   "Adrienne",   "Agatha",   "Aiko",   "Aileen",
   "Alene",   "Alexandra",   "Alice",   "Alison",
   "Allison",   "Alora",   "Alyson",   "Amanda",
   "Amber",   "Amelia",   "Amy",   "Andrea",
   "Angela",   "Angelica",   "Anita",   "Ann",
   "Anna",   "Anne",   "Annie",   "April",
   "Autumn",   "Barbara",   "Bea",   "Beatrice",
   "Bebe",   "Becky",   "Belle",   "Beth",
   "Beth Ann",   "Bethany",   "Betty",   "Bianca Maria",
   "Blanche",   "Bobbie",   "Bonita",   "Bonnie",
   "Brenda",   "Brittany",   "Brooke",   "Brooklyn",
   "Camilia",   "Carla",   "Carmen",   "Carol",
   "Caroline",   "Cassandra",   "Catherine",   "Cecilia",
   "Celine",   "Charlotte",   "Chelsea",   "Cheryl",
   "Chloe",   "Christine",   "Cindi",   "Cindy",
   "Clara",   "Clarity",   "Claudette",   "Claudia",
   "Colette",   "Colleen",   "Consuela",   "Courtney",
   "Crystal",   "Cybill",   "Dakota",   "Dana",
   "Daniela",   "Danielle",   "Daphne",   "Dawn",
   "Debbie",   "Denise",   "Desiree",   "Diane",
   "Dimitra",   "Donna",   "Dora",   "Doreen",
   "Edna",   "Elaine",   "Elinor",   "Elizabeth",
   "Ellen",   "Elsie",   "Emily",   "Emma",
   "Erica",   "Erika",   "Esmerelda",   "Eve",
   "Faith",   "Fatima",   "Felicia",   "Felicity",
   "Fiona",   "Florence",   "Florida",   "Fran",
   "Francine",   "Gabrielle",   "Georgia",   "Gina",
   "Giovanna",   "Gloria",   "Grace",   "Gwen",
   "Gwendolyn",   "Hailey",   "Haley",   "Hannah",
   "Harmony",   "Hayley",   "Hazel",   "Heather",
   "Helen",   "Helga",   "Holly",   "Honor",
   "Ida",   "Ingrid",   "Irene",   "Iris",
   "Isabel",   "Isabella",   "Isabelle",   "Ivy",
   "Jaclyn",   "Jade",   "Jane",   "Janet",
   "Janette",   "January",   "Jaqueline",   "Jasmin",
   "Jasmine",   "Jennifer",   "Jenny",   "Jessica",
   "Jill",   "Jillian",   "Joan",   "Joann",
   "Josephine",   "Judy",   "Juliana",   "Julia",
   "Julie",   "Juliette",   "June",   "Kaitlyn",
   "Kaley",   "Karen",   "Karla",   "Kate",
   "Katelin",   "Katherine",   "Kathleen",   "Kathy",
   "Katie",   "Kay",   "Kayla",   "Kaylee",
   "Keely",   "Kiki",   "Kristal",   "Kristie",
   "Kristin",   "Krystal",   "Lacey",   "Lara",
   "Larissa",   "Latoya",   "Laura",   "Lauren",
   "Lavender",   "Leah",   "Leila",   "Leyna",
   "Liliana",   "Lilly",   "Lily",   "Lina",
   "Linda",   "Lindsay",   "Lindsey",   "Lisa",
   "Liz",   "Lizabeth",   "Lizzie",   "Lois",
   "Loretta",   "Louane",   "Louanne",   "Louise",
   "Lucia",   "Lynn",   "Lynnette",   "Madison",
   "Madonna",   "Manuela",   "Margaret",   "Marge",
   "Maria",   "Mariana",   "Marianne",   "Marie",
   "Marion",   "Marlene",   "Martha",   "Martina",
   "Mary",   "Mary Ann",   "Matilda",   "Maureen",
   "Mayumi",   "Megan",   "Meghan",   "Melanie",
   "Melissa",   "Mercedes",   "Merissa",   "Mia",
   "Michelle",   "Minal",   "Mindy",   "Miranda",
   "Molly",   "Monica",   "Nadia",   "Nancy",
   "Natalie",   "Natasha",   "Nellie",   "Nicole",
   "Nicolette",   "Nina",   "Norma",   "October",
   "Olivia",   "Pam",   "Patricia",   "Patty",
   "Paula",   "Paulette",   "Peggy",   "Petunia",
   "Phoebe",   "Priscilla",   "Rachael",   "Racheal",
   "Rachel",   "Rachelle",   "Rania",   "Raphaela",
   "Raquel",   "Rebecca",   "Renee",   "Rita",
   "Rosalie",   "Rosalinda",   "Rosanna",   "Roberta",
   "Rose",   "Rosemary",   "Roxanne",   "Roz",
   "Ruth",   "Ruthanne",   "Sabrina",   "Sally",
   "Samantha",   "Sandra",   "Sara",   "Sarah",
   "Satya",   "Selma",   "Shaniqua",   "Shanna",
   "Shannon",   "Sharon",   "Sherry",   "Shirley",
   "Shyann",   "Silvia",   "Sonia",   "Sophia",
   "Sophie",   "Stacie",   "Stacy",   "Stephanie",
   "Summer",   "Sue",   "Susan",   "Susanna",
   "Suzanne",   "Suzie",   "Tabatha",   "Tabitha",
   "Tamara",   "Tammy",   "Tara",   "Tawanda",
   "Teresa",   "Terri",   "Tetiana",   "Theresa",
   "Tiffanie",   "Tiffany",   "Tina",   "Toni",
   "Tracy",   "Tricia",   "Trudy",   "Tyra",
   "Valentina",   "Valeria",   "Valerie",   "Vanessa",
   "Verena",   "Veronica",   "Virginia",   "Vicki",
   "Vicky",   "Victoria",   "Violet",   "Vithara",
   "Vonnita",   "Wanda",   "Wendy",   "Willow",
   "Wilma",   "Xenia",   "Yoko",   "Yuri",
   "Yvonne",   "Zoe"
   };

   static const char *gender_neutral_first_names[] =
   {
   "Akira",   "Alex",   "Alexis",   "Angel",
   "Ariel",   "Ashley",   "Billy",   "Frankie",
   "Hillary",   "Kelly",   "Jackie",   "Jamie",
   "Jo",   "Jun",   "Kelsey",   "Kelsie",
   "Kim",   "Lesley",   "Leslie",   "Morgan",
   "Nicky",   "Noel",   "Page",   "Paige",
   "Pat",   "Raven",   "Riley",   "Robin",
   "Sam",   "Sandy",   "Sidney",   "Terry",
   "Yanping"
   };

   static const char *great_white_male_patriarch_first_names[] =
   {
   "Roger",   "James",   "John",   "Bob",
   "Mike",   "Bill",   "Dave",   "Rick",
   "Colin",   "Joe",   "Tom",   "Chris",
   "Dan",   "Paul",   "Mark",   "Donald",
   "Jim",   "Ken",   "Steve",   "Edward",
   "Brian",   "Ron",   "Tony",   "Kevin",
   "Jason",   "Matt",   "Gary",   "Tim",
   "Alan",   "Larry",   "Jeff",   "Frank",
   "Scott",   "Eric",   "Steve",   "Andy",
   "Ray",   "Greg",   "Josh",   "Jerry",
   "Dennis","Walter","Jesse","Peter",
   "Harry",   "Doug",   "Henry",   "Carl",
   "Arthur",   "Ryan"
   };

   strcpy(str,"");

   int roll, nametable;

   // If we don't care if the name is male or female, pick one randomly
   // This ensures gender balance in the names chosen
   if(gender == GENDER_NEUTRAL)
      gender = LCSrandom(2) + 1;

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
      strcat(str, selectRandomString(  male_first_names,
                                       ARRAY_ELEMENTS(male_first_names)));
   }
//}}}
   else if(nametable == GENDER_FEMALE)
//{{{ Female
   {
      strcat(str, selectRandomString(  female_first_names,
                                       ARRAY_ELEMENTS(female_first_names)));
//}}}
   }
//}}}
//{{{ Neutral
   else if(nametable == GENDER_NEUTRAL)
   {
      strcat(str, selectRandomString(  gender_neutral_first_names,
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
/*   else if(nametable == GENDER_MALE_IMMIGRANT)
// {{{ Illegal Immigrants
   {
      switch(roll)
      {
         case 0:strcat(str,"Alejandro");break;
         case 1:strcat(str,"Antonio");break;
         case 2:strcat(str,"Beltran");break;
         case 3:strcat(str,"Carlos");break;
         case 4:strcat(str,"Diego");break;
         case 5:strcat(str,"Eduardo");break;
         case 6:strcat(str,"Enrique");break;
         case 7:strcat(str,"Fernando");break;
         case 8:strcat(str,"Jacobo");break;
         case 9:strcat(str,"Javier");break;
         case 10:strcat(str,"Jorge");break;
         case 11:strcat(str,"Juan");break;
         case 12:strcat(str,"Luis");break;
         case 13:strcat(str,"Matteo");break;
         case 14:strcat(str,"Pablo");break;
         case 15:strcat(str,"Pedro");break;
         case 16:strcat(str,"Raul");break;
         case 17:strcat(str,"Ricardo");break;
      }
   }
// }}}
   else if(nametable == GENDER_FEMALE_IMMIGRANT)
// {{{ Illegal Immigrants, Female
   {
      switch(roll)
      {
         case 0:strcat(str,"Daniela");break;
         case 1:strcat(str,"Denielle");break;
         case 2:strcat(str,"Esmerelda");break;
         case 3:strcat(str,"Gabrielle");break;
         case 4:strcat(str,"Isabel");break;
         case 5:strcat(str,"Isabella");break;
         case 6:strcat(str,"Isabelle");break;
         case 7:strcat(str,"Maria");break;
         case 8:strcat(str,"Natalia");break;
         case 9:strcat(str,"Roberta");break;
      }
   }
// }}}
*/
}
/* gets a random last name */
void lastname(char *str)
//{{{ Last Name
{
   static const char *last_names[] =
   {
   "Abbey",         "Abbott",      "Abbzug",      "Adamson",
   "Adkins",      "Africa",      "Aguilera",      "Alanson",
   "Alcott",      "Alda",         "Alexander",   "Allanshaw",
   "Allen",       "Alley",         "Ames",         "Anderson",
   "Andress",      "Antle",         "Appleton",      "Armstead",
   "Armstrong",   "Arnaz",         "Artfield",      "Ashmore",
   "Ayers",         "Aykroyd",      "Bacall",      "Bailey",
   "Baird",         "Baker",         "Baldwin",      "Balgos",
   "Barr",         "Baranyi",      "Barrow",      "Barwick",
   "Bates", "Baxter", "Beaumont", "Bendix",
   "Bennett", "Bezos", "Birkin", "Black",
   "Blackenship", "Blanchard", "Blitzer", "Blum",
   "Bogart", "Boggs", "Bolger", "Booker",
   "Bosanquet", "Boyd", "Bradbury", "Bradley",
   "Bradshaw", "Brandis", "Bremer", "Brewster",
   "Brightman", "Briscoe", "Brooks", "Brown",
   "Brownlee", "Bryant", "Buchanan", "Buckley",
   "Buckman", "Buckner", "Bugbee", "Bump",
   "Bundy", "Bunton", "Burghoff", "Burton",
   "Bush", "Butcher", "Butler", "Butt",
   "Butterman", "Buttman", "Byrd", "Callahan",
   "Campbell", "Carberry", "Carey", "Carpenter",
   "Carr", "Carson", "Cartwright", "Cavallero",
   "Cavill", "Chaffee", "Chamberlain", "Chambers",
   "Chandler", "Channing", "Cheney", "Cheng",
   "Childress", "Childs", "Chilton", "Cho",
   "Chubb", "Churchill", "Clavin", "Clay",
   "Clinton", "Cobb", "Coffey", "Cohen",
   "Colcord", "Cole", "Coleman", "Collins",
   "Colombe", "Colt", "Conner", "Cook",
   "Cooper", "Coopersmith", "Copeland", "Coppola",
   "Cordero", "Cosgrove", "Cottrell", "Court",
   "Cox", "Craft", "Crane", "Critchley",
   "Crockett", "Croft", "Cromwell", "Cross",
   "Crowe", "Crowson", "delaCruz", "Cunningham",
   "Curnew", "Cutlass", "Cutler", "Dahmer",
   "Danes", "Danson", "Dante", "Darby",
   "Darnand", "Dash", "Daveson", "Davidson",
   "Davis", "Day", "Decker", "Deere",
   "Delaney", "Delgado", "DeLorean", "Dench",
   "Dent", "Dentz", "Diaz", "Diehl",
   "DiMeo", "Dole", "Dorgan", "Dorsey",
   "Doucette", "Downey", "Downing", "Doyle",
   "Drake", "Duell", "Duffield", "Duke",
   "Dunham", "Dunlap", "Duval", "Earle",
   "English", "Escobar", "Ericson", "Eubanks",
   "Evans", "alFadil", "Fairbanks", "Fairchild",
   "Falco", "Farnsworth", "Farr", "Faulkner",
   "Fenoglio", "Fiddlebottom", "Fields", "Filler",
   "Fimple", "Fingleton", "Finn", "Fisher",
   "Fitzgerald", "Fitzpatrick", "Flanders", "Fletcher",
   "Florez", "Fortmuller", "Ford", "Foster",
   "Fox", "France", "Franklin", "Frost",
   "Friend", "Fudge", "Fulsom", "Furley",
   "Fushimi", "Gacy", "Galindo", "Garcia",
   "Garnier", "Gates", "Gatien", "Gelbart",
   "Getty", "Gilbert", "Gillmouth", "Gilpin",
   "Godwin", "Gleaves", "Goldberg", "Goldstein",
   "Goode", "Goodyear", "Gore", "Govorov",
   "Grammer", "Grant", "Gray", "Green",
   "Greenberg", "Gregg", "Grover", "Gu",
   "Guy", "Hall", "Hamilton", "Hammond",
   "Hann", "Hannemann", "Hapsburg", "Harper",
   "Harris", "Hatcher", "Hawking", "Haymer",
   "Headley", "Heath", "Heller", "Hellmann",
   "Helton", "Hemingway", "Henderson", "Hendren",
   "Hendrix", "Hepburn", "Hepditch", "Hewson",
   "Heyduke", "Hiaasen", "Hill", "Hilliard",
   "Himelstein", "Hoddinott", "Hoffmann", "Hogan",
   "Holder", "Hooker", "Hoover", "Hope",
   "Hopkins", "Hopper", "Hornberger", "Houghton",
   "Houseman", "Howard", "Howe", "Hudson",
   "Hughes", "Hunnicutt", "Hutchison", "Ibarra",
   "Incledon", "Inking", "Irvin", "Irving",
   "Issacson", "Ivanson", "Ives", "Jackson",
   "Jameson", "Jefferson", "Jenkins", "Jimison",
   "Johnson", "Jones", "Jordan", "Judge",
   "Justice", "Kaltenbrunner", "Keaton", "Keats",
   "Keene", "Keitel", "Kent", "Kershaw",
   "Kesten", "Kilbourne", "Kindale", "King",
   "Kinslow", "Kitchen", "Kokernot", "Koprowski",
   "Krasow", "Krieger", "Krishna", "Kunstler",
   "Kurosawa", "Langley", "LaSaga", "Lauder",
   "Lee", "Leeves", "Lewis", "Leyden",
   "Li", "Lincoln", "Linville", "Lipman",
   "Little", "Loeb", "Logan", "Logsdon",
   "Lombard", "London", "Lopez", "Lovecraft",
   "Lowcourt", "Lowe", "Lowerly", "Lucas",
   "Luna", "Lushington", "Luzinski", "Lyles",
   "Lynam", "Lynch", "VanLynden", "Lyons",
   "MacGyver", "MacKinley", "MacKinnon", "MacLeod",
   "MacNeil", "Macy", "Mahoney", "Malone",
   "Mann", "Manning", "Mansfield", "March",
   "Marsden", "Marsh", "Marshall", "Matheson",
   "Mathewson", "Maxwell", "McAdams", "McAllister",
   "McArthur", "McBroom", "McCain", "McCarthy",
   "McCorkell", "McDonald", "McFadden", "McFatridge",
   "McFaul", "McGee", "McGovern", "McGrath",
   "McGraw", "McGuinness", "McIntosh", "McIntyre",
   "McKenzie", "McLaughlin", "McLeod", "McMurray",
   "McNabb", "McPhail", "McPherson", "McQueen",
   "McVeigh", "Meek", "Meffert", "Melendez",
   "Metzger", "Mifune", "Milburn", "Miller",
   "Mitchell", "Miyamoto", "Monroe", "Montag",
   "Montalvo", "Montana", "Moon", "Morris",
   "Morrow", "Morton", "Mosley", "Muhammed",
   "Murphy", "Murray", "Murrell", "Muscat",
   "Muskhelishvili", "Mussa", "Myatt", "Myers",
   "Nagel", "Nagelberg", "Nakagawa", "Nakahara",
   "Nallet", "Nance", "Nash", "Nathanson",
   "Necker", "Nelson", "Ness", "Nettles",
   "Neuwirth", "Nevinson", "Newberry", "Newhook",
   "Newman", "Ng", "Nicolle", "Nickson",
   "Nielson", "Nixon", "North", "Oakley",
   "Obama", "Obeid", "Oberlander", "Ochs",
   "Ogden", "Olde", "Olds", "Oldsmar",
   "Ollerenshaw", "Orlando", "Ortiz", "Paoletti",
   "Palm", "Palmer", "Parker", "Parking",
   "Parr", "Patterson", "Pearson", "Peck",
   "Pegg", "Pelton", "Pendergrass", "Pendleton",
   "Perez", "Pert", "Peterson", "Petermann",
   "Pickett", "Pierce", "Piercey", "Pommerencke",
   "Pomper", "Porcaro", "Porter", "Postman",
   "Potter", "Pound", "Powers", "Presley",
   "Probst", "Proudfoot", "Pryor", "Puente",
   "Purcell", "Queen", "Quennell", "Quick",
   "Quilico", "Quilliam", "Quilty", "Quincey",
   "Quinlivan", "Quinn", "Quirk", "Rabban",
   "Rackard", "Radman", "Rainey", "Ramirez",
   "Rand", "Randell", "Ransom", "Rappaport",
   "Raskin", "Rastrelli", "Rather", "Ratzenberger",
   "Ravenhill", "Rawls", "Read", "Reading",
   "Reagan", "Reasoner", "VanRechteren", "Redford",
   "Redman", "Redwine", "Reed", "Reekers",
   "Rendell", "Rexford", "Reynolds", "Ricci",
   "Richardson", "Richter", "Rideout", "Riffle",
   "Robinson", "Rodham", "Rojo", "Roland",
   "Roosevelt", "Rose", "Rosenberg", "Ross",
   "Rota", "Rothstein", "Roy", "Roylance",
   "Rudkin", "Rush", "Rushworth", "laRussa",
   "Russell", "Rutherford", "Ryman", "Sabella",
   "Saddler", "Sadler", "Sansome", "Santiago",
   "Santos", "Savedra", "Sawyer", "Saxton",
   "Scales", "Scardino", "Schmitt", "Schumacher",
   "Schuster", "Scott", "Scully", "Scurfield",
   "Sealock", "Seay", "Sellers", "Semin",
   "Serling", "Sewall", "Sexton", "Shackleton",
   "Shagwell", "alShamie", "Shanahan", "Sharpe",
   "Shaw", "Shedd", "Shelby", "Shelton",
   "Shengde", "Shepard", "Shepardson", "Sheppard",
   "Sherman", "Shew", "Shields", "Shiller",
   "Shimazu", "Shipman", "Shipp", "Sho",
   "Short", "Shrapnal", "Shrum", "Shumway",
   "Shurin", "Shuttleworth", "Sidebottom", "Siegel",
   "Siever", "daSilva", "Silverman", "Silverstein",
   "Simmons", "Simms", "Simon", "Simpson",
   "Skidmore", "Skinner", "Skirrow", "Skutnik",
   "Skye", "Skyler", "Slaten", "Smalley",
   "Smallwood", "Smart", "Smith", "Smithers",
   "Smyth", "Snead", "Snook", "Snow",
   "Snyder", "Sommer", "Somner", "Soren",
   "Spasov", "Spears", "Spencer", "Stark",
   "Starkweather", "Stelling", "Stephenson", "Sterling",
   "Sternin", "Stevenson", "Stockton", "Stone",
   "Storm", "Storms", "Stiver", "Strain",
   "Straley", "Straub", "Strauss", "Stripe",
   "Strossen", "Suave", "Suiter", "Sulaiman",
   "Sullivan", "Surnow", "Susman", "Swain",
   "Swaine", "Swanson", "Sweeney", "Swift",
   "Swit", "Symington", "Symonds", "Tabb",
   "Tailboys", "Takayoshi", "Talbert", "Talbot",
   "Talbott", "Talty", "Tanner", "Tatters",
   "Taubman", "Taylor", "Tayo", "Teige",
   "Tell", "Templeton", "Tennent", "Tenney",
   "Tenpenny", "Terrill", "Testore", "Tetley",
   "Thalberg", "Thatcher", "Thomaz", "Thompson",
   "Thomson", "Thorne", "Threepwood", "Thurgood",
   "Thurlow", "Tickell", "Tillett", "Tillinghast",
   "Tillson", "Tillsley", "Timmonds", "Tippett",
   "Tisdall", "Tishman", "Titcomb", "Tobin",
   "Todd", "Todhunter", "Toft", "Tolbert",
   "Tolkien", "Tolson", "Tonelli", "Toogood",
   "Tooke", "Toole", "Toth", "Towne",
   "Townsend", "Trafford", "Trebek", "Truman",
   "Tubbs", "Tubman", "Tucker", "Turner",
   "Tyler", "Tymchyshyn", "Ulmer", "Underhill",
   "Underdown", "Underwood", "Updike", "Upton",
   "Usher", "Ussher", "Valdez", "Vallance",
   "Vance", "Vasilevsky", "Venus", "Villa",
   "Villanueva", "Viscuso", "Voland", "Vulcar",
   "Wade", "Wadsworth", "Walden", "Walker",
   "Wallace", "Walsh", "Walton", "Walwyn",
   "Wang", "Ward", "Warman", "Warner",
   "Warr", "Washington", "Weathers", "Webber",
   "Webster", "Weeks", "Weller", "Wendt",
   "West", "Westlake", "Weston", "Weygand",
   "White", "Whitehead", "Whitley", "Wickes",
   "Wilcox", "Willett", "Williams", "Williamson",
   "Willman", "Wilson", "Winchester", "Winkelman",
   "Winslow", "Winter", "Wiseman", "Wolfe",
   "Wong", "Wood", "Woodrow", "Wright",
   "Wuornos", "Stassen", "Xue", "Yamaguchi",
   "Yates", "Yee", "Yerkovich", "Yim",
   "Young", "Youngblood", "Yue", "Zachary",
   "Zadora", "Zakharov", "Zale", "Zedner",
   "Zhang", "Zicree", "Zimmermann", "Zito",
   "Zlotoff", "Zondervan", "Zucker", "Zumbach",
   "Starr", "Falstaff"
   };

   strcpy(str,"");

   strcat(str, selectRandomString(last_names, ARRAY_ELEMENTS(last_names)));
}
