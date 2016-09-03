// Add an age estimate to a person's name
void add_age(Creature& person);
/* rolls up a creature's stats and equipment */
void makecreature(Creature &cr, short type);
/* fills a string with a proper name */
void generate_name(char *str, char gender = GENDER_NEUTRAL);
/* get a first and last name for the same person */
void generate_name(char *first, char *last, char gender = GENDER_NEUTRAL);
/* get a first, middle, and last name for the same person */
void generate_long_name(char *first, char *middle, char *last, char gender = GENDER_NEUTRAL);
/* gets a random first name */
void firstname(char *str, char gender = GENDER_NEUTRAL);
/* gets a random last name */
void lastname(char *str, bool archconservative = false);
/* ensures that the creature's work location is appropriate to its type */
bool verifyworklocation(Creature &cr, char test_location = -1, char test_type = -1);
/* turns a creature into a conservative */
void conservatise(Creature &cr);
/* turns a creature into a liberal */
void liberalize(Creature &cr, bool rename = true);
/* gives a cover name to CCS members */
void nameCCSMember(Creature &cr);