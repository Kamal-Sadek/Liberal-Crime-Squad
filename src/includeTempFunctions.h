/*
TempFunctions
Ideally, all of these should be made obsolete
*/
static Alignment getAlignFromInt(int i);

static PoliticalParties getPartyFromInt(int i);

static CreatureSkill getSkillFromInt(int i);

static CreatureAttribute getAttributeFromInt(int i);

static Alignment getAlignFromInt(int i) {
	return (Alignment)i;
}

static PoliticalParties getPartyFromInt(int i) {
	return (PoliticalParties)i;
}

static CreatureSkill getSkillFromInt(int i) {
	return (CreatureSkill)i;
}

static CreatureAttribute getAttributeFromInt(int i) {
	return (CreatureAttribute)i;
}
static CreatureTypes getCreatureTypeFromInt(int i) {
	return (CreatureTypes)i;
}