
/* monthly - hold trial on a liberal */
void trial(Creature &g);
/* monthly - sentence a liberal */
void penalize(Creature &g, char lenient);
/* monthly - move a liberal to jail */
void imprison(Creature &g);
/* monthly - advances a liberal's prison time or executes them */
char prison(Creature &g);
/* monthly - subjects a liberal to liberal therapy */
void reeducation(Creature &g);
/* monthly - subjects a liberal to hard labor */
void laborcamp(Creature &g);
/* monthly - subjects a liberal to normal prison */
void prisonscene(Creature &g);