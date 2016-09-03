
//just a float that is initialized to 0
struct float_zero
{
	float_zero() : n(0.0f) { }
	operator float&() { return n; }
	float n;
};
//Interrogation information for the interrogation system, to be
//dynamically created on capture and deleted when interrogation ends,
//referenced using a pointer typecast into one of the arguments
//of the target's current action.
struct interrogation
{
	interrogation() : druguse(0) { techniques[0] = 1, techniques[1] = 1, techniques[2] = 0, techniques[3] = 0, techniques[4] = 0, techniques[5] = 0; }
	bool techniques[6]; //yesterday's interrogation plan
	int druguse; //total days of drug use
				 //Maps individual characters to the rapport built with them
	map<long, struct float_zero> rapport;
};


struct activityst
{
	activityst() : type(0), arg(0), arg2(0) { }
	int type;
	long arg, arg2;
	// return a reference to arg, with arg typecast as a pointer to an object of type interrogation,
	// allowing us to easily access and modify the interrogation data without typecasting outside this function
	interrogation* &intr() { interrogation** i = reinterpret_cast<interrogation**>(&arg); return *i; }
};

class Ledger
{
private:
	int funds;
public:
	int income[INCOMETYPENUM], expense[EXPENSETYPENUM], total_income, total_expense, dailyIncome[INCOMETYPENUM], dailyExpense[EXPENSETYPENUM];
	Ledger() : funds(7), total_income(0), total_expense(0)
	{
		for (int i = 0; i<INCOMETYPENUM; i++) income[i] = 0, dailyIncome[i] = 0;
		for (int e = 0; e<EXPENSETYPENUM; e++) expense[e] = 0, dailyExpense[e] = 0;
	}
	int get_funds() { return funds; }
	void force_funds(int amount) { funds = amount; }
	void add_funds(int amount, int incometype)
	{
		funds += amount,
			income[incometype] += amount,
			dailyIncome[incometype] += amount,
			total_income += amount;
	}
	void subtract_funds(int amount, int expensetype)
	{
		funds -= amount,
			expense[expensetype] += amount,
			dailyExpense[expensetype] += amount,
			total_expense += amount;
	}
	void resetMonthlyAmounts()
	{
		for (int i = 0; i<INCOMETYPENUM; i++) income[i] = 0;
		for (int e = 0; e<EXPENSETYPENUM; e++) expense[e] = 0;
	}
	void resetDailyAmounts()
	{
		for (int i = 0; i<INCOMETYPENUM; i++) dailyIncome[i] = 0;
		for (int e = 0; e<EXPENSETYPENUM; e++) dailyExpense[e] = 0;
	}
};

class Interval
{
public:
	int min, max;
	Interval() : min(0), max(0) { }
	Interval(int value) : min(value), max(value) { }
	Interval(int low, int high) : min(low), max(high) { }
	void set_interval(int low, int high) { min = low, max = high; }
	// Sets the interval according to a string that is either a number or two
	// number separated by a dash. Returns false and does not change the
	// interval if the given string is not a valid interval.
	bool set_interval(const string& interval); // implemented in misc.cpp
	int roll() const { return LCSrandom(max - min + 1) + min; }
private:
	// Checks if a string is a number. Assumes non-numeric characters other
	// than dashes have already been checked for.
	bool valid(const string& v)
	{
		return len(v) &&                       // Blank string is invalid.
			(len(v) != 1 || v[0] != '-') &&        // Just a dash is invalid.
			v.find('-', 1) == string::npos;
	} // A dash after the first char is invalid.
};

/*
Declarations for every function grouped by folder and file.
Created by jonathansfox.
*/

/* This is declared again lower down, just needed here for this header. */
std::string tostring(long i);

/* end the game and clean up */
void end_game(int err = EXIT_SUCCESS);