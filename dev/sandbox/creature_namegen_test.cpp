
#include "creature_test.h"
#include <string>

/* gets a random first name */
void firstname(std::string& str, enum gender name_gender/*=GENDER_NONE*/)
{
   str[0] = 0;

   if(name_gender == GENDER_MALE ||
      (name_gender == GENDER_NONE && LCSrandom(2)))
   {
      switch(LCSrandom(81))
      {
      case 0:str = "Ryan";break;
      case 1:str = "Sergio";break;
      case 2:str = "Bill";break;
      case 3:str = "James";break;
      case 4:str = "Marty";break;
      case 5:str = "Angel";break;
      case 6:str = "Pat";break;
      case 7:str = "Toshiro";break;
      case 8:str = "Yan-ping";break;
      case 9:str = "Tetsuo";break;
      case 10:str = "Akira";break;
      case 11:str = "Jimmy";break;
      case 12:str = "Carlos";break;
      case 13:str = "William";break;
      case 14:str = "Billy Bob";break;
      case 15:str = "Manuel";break;
      case 16:str = "Douglas";break;
      case 17:str = "Steven";break;
      case 18:str = "Howard";break;
      case 19:str = "Donald";break;
      case 20:str = "Barry";break;
      case 21:str = "Thomas";break;
      case 22:str = "Derek";break;
      case 23:str = "Gary";break;
      case 24:str = "Archie";break;
      case 25:str = "Ned";break;
      case 26:str = "Randy";break;
      case 27:str = "Taylor";break;
      case 28:str = "Kim";break;
      case 29:str = "Roger";break;
      case 30:str = "Raymond";break;
      case 31:str = "Harvey";break;
      case 32:str = "Robert";break;
      case 33:str = "Michael";break;
      case 34:str = "Aaron";break;
      case 35:str = "George";break;
      case 36:str = "Noel";break;
      case 37:str = "Lex";break;
      case 38:str = "Chuck";break;
      case 39:str = "Charlie";break;
      case 40:str = "Charles";break;
      case 41:str = "Malcolm";break;
      case 42:str = "Martin";break;
      case 43:str = "Sean";break;
      case 44:str = "Raven";break;
      case 45:str = "Wolf";break;
      case 46:str = "Miguel";break;
      case 47:str = "Pablo";break;
      case 48:str = "Paul";break;
      case 49:str = "Jesus";break;
      case 50:str = "Ali";break;
      case 51:str = "Bruce";break;
      case 52:str = "Dick";break;
      case 53:str = "Phillip";break;
      case 54:str = "Kirk";break;
      case 55:str = "Kurt";break;
      case 56:str = "John";break;
      case 57:str = "Alexander";break;
      case 58:str = "David";break;
      case 59:str = "Beau";break;
      case 60:str = "Mumtaz";break;
      case 61:str = "Diwakar";break;
      case 62:str = "Dale";break;
      case 63:str = "Woody";break;
      case 64:str = "Ariel";break;
      case 65:str = "Hans";break;
      case 66:str = "Jun";break;
      case 67:str = "Chin-Yuan";break;
      case 68:str = "Deepak";break;
		case 69:str = "Christopher";break;
		case 70:str = "Matthew";break;
		case 71:str = "Joseph";break;
		case 72:str = "James";break;
		case 73:str = "Daniel";break;
		case 74:str = "Robert";break;
		case 75:str = "John";break;
      case 76:str = "Kennedy";break;
		case 77:str = "Jonathan";break;
		case 78:str = "Adam";break;
		case 79:str = "Justin";break;
		case 80:str = "Robin";break;
      default:str = "MrDefault";break;
      }
   }
   else
   {
      switch(LCSrandom(54))
      {
      case 0:str = "Amanda";break;
      case 1:str = "Laura";break;
      case 2:str = "Anne";break;
      case 3:str = "Jessica";break;
      case 4:str = "Lisa";break;
      case 5:str = "Bonita";break;
      case 6:str = "Angel";break;
      case 7:str = "Pat";break;
      case 8:str = "Yan-ping";break;
      case 9:str = "Carol";break;
      case 10:str = "Jenny";break;
      case 11:str = "Jennifer";break;
      case 12:str = "Manuela";break;
      case 13:str = "Kristin";break;
      case 14:str = "Bonnie";break;
      case 15:str = "Joann";break;
      case 16:str = "Mayumi";break;
      case 17:str = "Felicia";break;
      case 18:str = "Sherry";break;
      case 19:str = "Judy";break;
      case 20:str = "Elinor";break;
      case 21:str = "Taylor";break;
      case 22:str = "Kim";break;
      case 23:str = "Ruthanne";break;
      case 24:str = "Adrienne";break;
      case 25:str = "Linda";break;
      case 26:str = "Raven";break;
      case 27:str = "Ingrid";break;
      case 28:str = "Kweisi";break;
      case 29:str = "Susanna";break;
      case 30:str = "Sharon";break;
      case 31:str = "Marion";break;
      case 32:str = "Kathy";break;
      case 33:str = "Bruce";break;
      case 34:str = "Elsie";break;
      case 35:str = "Satya";break;
      case 36:str = "Ariel";break;
      case 37:str = "Barbara";break;
      case 38:str = "Jun";break;
      case 39:str = "Rosemary";break;
      case 40:str = "Chin-Yuan";break;
      case 41:str = "Aiko";break;
      case 42:str = "Vithara";break;
		case 43:str = "Ashley";break;
		case 44:str = "Alene";break;
		case 45:str = "Janette";break;
		case 46:str = "Stephanie";break;
		case 47:str = "Kelly";break;
		case 48:str = "Robin";break;
		case 49:str = "Tiffany";break;
		case 50:str = "Monica";break;
		case 51:str = "Jaqueline";break;
		case 52:str = "Latoya";break;
		case 53:str = "Veronica";break;
      default:str = "MsDefault";break;
      }
   }
}



/* gets a random last name */
void lastname(std::string& str)
{
   str[0] = 0;

   switch(LCSrandom(123))
   {
      case 0:str = "King";break;
      case 1:str = "Lewis";break;
      case 2:str = "Black";break;
      case 3:str = "White";break;
      case 4:str = "Ames";break;
      case 5:str = "Warner";break;
      case 6:str = "Simpson";break;
      case 7:str = "Parker";break;
      case 8:str = "Suave";break;
      case 9:str = "Mifune";break;
      case 10:str = "Gu";break;
      case 11:str = "Bolger";break;
      case 12:str = "Ross";break;
      case 13:str = "Ramirez";break;
      case 14:str = "Kurosawa";break;
      case 15:str = "Johnson";break;
      case 16:str = "Buchanan";break;
      case 17:str = "Adamson";break;
      case 18:str = "Hendrix";break;
      case 19:str = "Rojo";break;
      case 20:str = "Villa";break;
      case 21:str = "Fields";break;
      case 22:str = "Templeton";break;
      case 23:str = "Ivanson";break;
      case 24:str = "Blitzer";break;
      case 25:str = "Muhammed";break;
      case 26:str = "Stone";break;
      case 27:str = "Cho";break;
      case 28:str = "Childress";break;
      case 29:str = "Africa";break;
      case 30:str = "Balgos";break;
      case 31:str = "Baird";break;
      case 32:str = "Bailey";break;
      case 33:str = "Diaz";break;
      case 34:str = "Decker";break;
      case 35:str = "Ericson";break;
      case 36:str = "Loeb";break;
      case 37:str = "Meffert";break;
      case 38:str = "McLeod";break;
      case 39:str = "Tucker";break;
      case 40:str = "Takayoshi";break;
      case 41:str = "Tanner";break;
      case 42:str = "Lipman";break;
      case 43:str = "Little";break;
      case 44:str = "Logsdon";break;
      case 45:str = "Krasow";break;
      case 46:str = "Krieger";break;
      case 47:str = "Dahmer";break;
      case 48:str = "Gacy";break;
      case 49:str = "Krishna";break;
      case 50:str = "la Russa";break;
      case 51:str = "Savedra";break;
      case 52:str = "Scardino";break;
      case 53:str = "Keitel";break;
      case 54:str = "Wallace";break;
      case 55:str = "Buckman";break;
      case 56:str = "Fulsom";break;
      case 57:str = "Smith";break;
      case 58:str = "Venus";break;
      case 59:str = "Straley";break;
      case 60:str = "Purcell";break;
      case 61:str = "al Fadil";break;
      case 62:str = "Storm";break;
      case 63:str = "Patterson";break;
      case 64:str = "Pelton";break;
      case 65:str = "Ng";break;
      case 66:str = "Filler";break;
      case 67:str = "Buttman";break;
      case 68:str = "Fingleton";break;
      case 69:str = "Fenoglio";break;
      case 70:str = "de la Cruz";break;
      case 71:str = "Delgado";break;
      case 72:str = "Hatcher";break;
      case 73:str = "Jameson";break;
      case 74:str = "Franklin";break;
      case 75:str = "Washington";break;
      case 76:str = "Jefferson";break;
      case 77:str = "Strossen";break;
      case 78:str = "Hannemann";break;
      case 79:str = "Hammond";break;
      case 80:str = "Logan";break;
      case 81:str = "Hutchison";break;
      case 82:str = "Jimison";break;
      case 83:str = "Sawyer";break;
      case 84:str = "Santiago";break;
      case 85:str = "Rudkin";break;
      case 86:str = "Bump";break;
      case 87:str = "Simon";break;
      case 88:str = "Davis";break;
      case 89:str = "Reagan";break;
      case 90:str = "Bush";break;
      case 91:str = "Bradshaw";break;
      case 92:str = "Yamaguchi";break;
      case 93:str = "Roy";break;
      case 94:str = "Colt";break;
      case 95:str = "Rothstein";break;
      case 96:str = "Spears";break;
      case 97:str = "Lopez";break;
      case 98:str = "Aguilera";break;
      case 99:str = "Carey";break;
		case 100:str = "Griffith";break;
		case 101:str = "Valdez";break;
		case 102:str = "McGee";break;
		case 103:str = "Copeland";break;
		case 104:str = "Blackenship";break;
		case 105:str = "Faulkner";break;
		case 106:str = "Melendez";break;
		case 107:str = "Dunlap";break;
		case 108:str = "Guy";break;
		case 109:str = "Duke";break;
		case 110:str = "Villanueva";break;
		case 111:str = "Boggs";break;
		case 112:str = "Godwin";break;
		case 113:str = "Brewster";break;
		case 114:str = "Irvin";break;
		case 115:str = "Galindo";break;
		case 116:str = "Cordero";break;
		case 117:str = "Eubanks";break;
		case 118:str = "Youngblood";break;
		case 119:str = "Seay";break;
		case 120:str = "Hope";break;
		case 121:str = "Winslow";break;
		case 122:str = "Fox";break;
      default:str = "Defaultson";break;
   }
}

/* fills a string with a proper name */
void GenerateName(std::string& name, enum gender name_gender)
{
   std::string first;
   std::string last;
   firstname(first,name_gender);
   lastname(last);
   name = first + ' ' + last;
}
