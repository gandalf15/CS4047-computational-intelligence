#if !defined TRUE && FALSE
#define TRUE 1
#define FALSE 0
#endif

typedef struct variable_value_pair {
	char *name;	// variable name
	char *value;	// value of the variable
	char *connective;	// connective to the next pair AND, OR, THEN
	struct variable_value_pair *next;	// address of the next variable_value_pair
} var_val_pair;

typedef struct rule_node {
	int num;	//rule number
	var_val_pair *head_of_rules;	// pointer to list of variable value pair and it's connectives
	var_val_pair *tail_of_rules;
	struct rule_node *next;	//pointer to the next rule
} rule_node;

typedef struct fuzzy_set {
	char *val_name;	// value name
	int tuple[4];	// tuple representing the set
	struct fuzzy_set *next;	// pointer to next fuzzy set
} fuzzy_set;

typedef struct variable_fuzzy_sets {
	char *var_name;	// nameof the variable which has many fuzzy sets
	fuzzy_set *set;	// values of fuzzy set
	//struct variable_fuzzy_sets *next;	// pointer to next variable fuzzy sets list
} var_sets;

typedef struct rule_base {
	char *name;	// name of rulebase
	rule_node *rule_list;	// list of rules in rule base
	var_sets *sets_list;	//	list of variables that each has a list of sets
} rule_base;

typedef struct measurement {
	char *var_name;	// name of the measured variable
	double value;	// value of the variable
	struct measurement *next;	// pointer to next measurement node
} measurement;

// function prototypes
// load rules loads rule base from a file and return pointer to it
rule_base* loadRuleBase (FILE *f_name);
/*
//	insert node to list
var_val_pair* insertVarValPair (var_val_pair **head, var_val_pair **tail, char *nema, char *value, char *con);
rule_node* insertRuleNode (rule_node **head, int num);
fuzzy_set* insertFuzzySet (fuzzy_set **head, char *val_name, int *tuple);
var_sets* insertVarSets (var_sets **head, char *var_name, fuzzy_set *set);
measurement* insertMeasurement (measurement **head, char *var_name, double value);
*/
