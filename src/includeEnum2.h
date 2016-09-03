
enum UnlockTypes
{
	UNLOCK_DOOR,
	UNLOCK_CAGE,
	UNLOCK_CAGE_HARD,
	UNLOCK_CELL,
	UNLOCK_SAFE,
	UNLOCK_ARMORY,
	UNLOCK_VAULT,
	UNLOCKNUM
};

enum BashTypes
{
	BASH_DOOR,
	BASHNUM
};

enum HackTypes
{
	HACK_SUPERCOMPUTER,
	HACK_VAULT,
	HACKNUM
};

enum SpecialAttacks
{
	ATTACK_CANNON,
	ATTACK_FLAME,
	ATTACK_SUCK,
	ATTACKNUM
};

enum endgame
{
	ENDGAME_NONE,
	ENDGAME_CCS_APPEARANCE,
	ENDGAME_CCS_ATTACKS,
	ENDGAME_CCS_SIEGES,
	ENDGAME_CCS_DEFEATED,
	ENDGAME_MARTIALLAW,
	ENDGAMENUM
};

enum ccsexposure
{
	CCSEXPOSURE_NONE,
	CCSEXPOSURE_LCSGOTDATA,
	CCSEXPOSURE_EXPOSED,
	CCSEXPOSURE_NOBACKERS,
	CCSEXPOSURENUM
};


/* *JDS* I'm making laws an array instead of a bunch
* of bits which are either on or off. Each charge can be
* duplicated, for multiple counts, and intense focus
* for wanted persons.
*/
enum Lawflags
{
	LAWFLAG_TREASON,      // "Treason"
	LAWFLAG_TERRORISM,    // "Terrorism"
	LAWFLAG_MURDER,       // "Murder"
	LAWFLAG_KIDNAPPING,   // "Kidnapping"
	LAWFLAG_BANKROBBERY,  // "Bank robbery"
	LAWFLAG_ARSON,        // "Arson"
	LAWFLAG_BURNFLAG,     // "Flag burning" / "Flag Murder"
	LAWFLAG_SPEECH,       // "Harmful speech"
	LAWFLAG_BROWNIES,     // "Drug dealing"
	LAWFLAG_ESCAPED,      // "Escaping prison"
	LAWFLAG_HELPESCAPE,   // "Releasing prisoners"
	LAWFLAG_JURY,         // "Jury tampering"
	LAWFLAG_RACKETEERING, // "Racketeering"
	LAWFLAG_EXTORTION,    // "Extortion"
	LAWFLAG_ARMEDASSAULT, // "Armed assault"
	LAWFLAG_ASSAULT,      // "Assault"
	LAWFLAG_CARTHEFT,     // "Grand theft auto"
	LAWFLAG_CCFRAUD,      // "Credit card fraud"
	LAWFLAG_THEFT,        // "Theft"
	LAWFLAG_PROSTITUTION, // "Prostitution"
	LAWFLAG_HIREILLEGAL,  // "Hiring illegal aliens" / "Hiring undocumented workers"
						  //LAWFLAG_GUNUSE,     // "Firing illegal weapons"
						  //LAWFLAG_GUNCARRY,   // "Carrying illegal weapons"
	LAWFLAG_COMMERCE,     // "Electronic sabotage"
	LAWFLAG_INFORMATION,  // "Hacking"
	LAWFLAG_BURIAL,       // "Unlawful burial"
	LAWFLAG_BREAKING,     // "Breaking and entering"
	LAWFLAG_VANDALISM,    // "Vandalism"
	LAWFLAG_RESIST,       // "Resisting arrest"
	LAWFLAG_DISTURBANCE,  // "Disturbing the peace"
	LAWFLAG_PUBLICNUDITY, // "Public nudity"
	LAWFLAG_LOITERING,    // "Loitering"
	LAWFLAGNUM
};


/* *JDS* In addition to laws being an array,
* each law will be associated with a "heat" value,
* indicating how much that particular crime draws
* police attention to the wanted person, and to the
* LCS as a whole. This is a global array, available
* at runtime and indexed to the lawflag enum, so that
* code can dynamically look up these heat values as
* needed at runtime.
*/


enum BusinessFronts
{
	BUSINESSFRONT_INSURANCE,
	BUSINESSFRONT_TEMPAGENCY,
	BUSINESSFRONT_RESTAURANT,
	BUSINESSFRONT_MISCELLANEOUS,
	BUSINESSFRONTNUM
};

enum AnimalGlosses
{
	ANIMALGLOSS_NONE,//IMPORTANT THAT THIS BE HERE AT ZERO
	ANIMALGLOSS_TANK,
	ANIMALGLOSS_ANIMAL,
	ANIMALGLOSSNUM
};

enum Activity
{
	ACTIVITY_NONE,
	ACTIVITY_VISIT,
	ACTIVITY_HOSTAGETENDING,
	ACTIVITY_TROUBLE,
	ACTIVITY_GRAFFITI,
	ACTIVITY_COMMUNITYSERVICE,
	ACTIVITY_SELL_ART,
	ACTIVITY_SELL_MUSIC,
	ACTIVITY_SELL_TSHIRTS,
	ACTIVITY_DONATIONS,
	ACTIVITY_SELL_DRUGS,
	ACTIVITY_PROSTITUTION,
	ACTIVITY_POLLS,
	ACTIVITY_CCFRAUD,
	ACTIVITY_DOS_RACKET,
	ACTIVITY_DOS_ATTACKS,
	ACTIVITY_HACKING,
	ACTIVITY_REPAIR_ARMOR,
	ACTIVITY_MAKE_ARMOR,
	ACTIVITY_STEALCARS,
	ACTIVITY_WHEELCHAIR,
	ACTIVITY_BURY,
	ACTIVITY_WRITE_BLOG,
	ACTIVITY_WRITE_LETTERS,
	ACTIVITY_WRITE_GUARDIAN,
	ACTIVITY_TEACH_POLITICS,
	ACTIVITY_TEACH_FIGHTING,
	ACTIVITY_TEACH_COVERT,
	ACTIVITY_STUDY_DEBATING,
	ACTIVITY_STUDY_MARTIAL_ARTS,
	ACTIVITY_STUDY_DRIVING,
	ACTIVITY_STUDY_PSYCHOLOGY,
	ACTIVITY_STUDY_FIRST_AID,
	ACTIVITY_STUDY_LAW,
	ACTIVITY_STUDY_DISGUISE,
	ACTIVITY_STUDY_SCIENCE,
	ACTIVITY_STUDY_BUSINESS,
	//ACTIVITY_STUDY_COOKING,
	ACTIVITY_STUDY_GYMNASTICS,
	ACTIVITY_STUDY_MUSIC,
	ACTIVITY_STUDY_ART,
	ACTIVITY_STUDY_TEACHING,
	ACTIVITY_STUDY_WRITING,
	ACTIVITY_STUDY_LOCKSMITHING,
	ACTIVITY_STUDY_COMPUTERS,
	ACTIVITY_CLINIC,
	ACTIVITY_HEAL,
	ACTIVITY_SLEEPER_LIBERAL,
	ACTIVITY_SLEEPER_CONSERVATIVE,
	ACTIVITY_SLEEPER_SPY,
	ACTIVITY_SLEEPER_RECRUIT,
	ACTIVITY_SLEEPER_SCANDAL,
	ACTIVITY_SLEEPER_EMBEZZLE,
	ACTIVITY_SLEEPER_STEAL,
	ACTIVITY_SLEEPER_JOINLCS,
	ACTIVITY_RECRUITING,
	ACTIVITY_AUGMENT,
	ACTIVITYNUM
};

enum IncomeType         // the below names are used in fundreport() in lcsmonthly.cpp
{                       // new types added without updating fundreport() will show as "Other Income"
	INCOME_BROWNIES,     // "Brownies"
	INCOME_CARS,         // "Car Sales"
	INCOME_CCFRAUD,      // "Credit Card Fraud"
	INCOME_DONATIONS,    // "Donations"
	INCOME_SKETCHES,     // "Drawing Sales"
	INCOME_EMBEZZLEMENT, // "Embezzlement"
	INCOME_EXTORTION,    // "Extortion"
	INCOME_HUSTLING,     // "Hustling"
	INCOME_PAWN,         // "Pawning Goods"
	INCOME_PROSTITUTION, // "Prostitution"
	INCOME_BUSKING,      // "Street Music"
	INCOME_THIEVERY,     // "Thievery"
	INCOME_TSHIRTS,      // "T-Shirt Sales"
	INCOMETYPENUM        // # of types of income
};

enum ExpenseType          // the below names are used in fundreport() in lcsmonthly.cpp
{                         // new types added without updating fundreport() will show as "Other Expenses"
	EXPENSE_TROUBLEMAKING, // "Activism"
	EXPENSE_CONFISCATED,   // "Confiscated"
	EXPENSE_DATING,        // "Dating"
	EXPENSE_SKETCHES,      // "Drawing Materials"
	EXPENSE_FOOD,          // "Groceries"
	EXPENSE_HOSTAGE,       // "Hostage Tending"
	EXPENSE_LEGAL,         // "Legal Fees"
	EXPENSE_MANUFACTURE,   // "Manufacturing"
	EXPENSE_CARS,          // "New Cars"
	EXPENSE_SHOPPING,      // "Purchasing Goods"
	EXPENSE_RECRUITMENT,   // "Recruitment"
	EXPENSE_RENT,          // "Rent"
	EXPENSE_COMPOUND,      // "Safehouse Investments"
	EXPENSE_TRAINING,      // "Training"
	EXPENSE_TRAVEL,        // "Travel"
	EXPENSE_TSHIRTS,       // "T-Shirt Materials"
	EXPENSETYPENUM         // # of types of expenses
};