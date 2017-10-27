#if !defined TRUE && FALSE
#define TRUE 1
#define FALSE 0
#endif
#define NAME_DATA_TYPE char
#define VALUE_DATA_TYPE char
#define RULE_AND 10
#define RULE_OR 20
#define RULE_THEN 30
#define MEASUREMENT_DATA_TYPE double

typedef struct variable_value_pair {
	NAME_DATA_TYPE *name;	// variable name
	VALUE_DATA_TYPE *value;	// value of the variable
	char connective;	// connective to the next pair AND, OR, THEN
	var_val_pair *next;	// address of the next variable_value_pair
} var_val_pair;

typedef struct rule_node {
	int num;	//rule number
	var_val_pair *rule;	// pointer to list of variable value pair and it's connectives
	rule_node *next;	//pointer to the next rule
} rule_node;

typedef struct fuzzy_set {
	char *val_name;	// value name
	tuple[4];	// tuple representing the set
	fuzzy_set *next;	// pointer to next fuzzy set
} fuzzy_set;

typedef struct variable_fuzzy_sets {
	char *var_name;	// nameof the variable which has many fuzzy sets
	fuzzy_set set;	// values of fuzzy set
	var_sets *next;	// pointer to next variable fuzzy sets list
} var_sets;

typedef struct rule_base {
	int num;	// rule base number
	rule_node *rule_list;	// list of rules in rule base
	var_sets *sets_list;	//	list of variables that each has a list of sets
	rule_base *next;	// pointer to next rule base
} rule_base;

typedef struct measurement {
	NAME_DATA_TYPE *var_name;	// name of the measured variable
	MEASUREMENT_DATA_TYPE value;	// value of the variable
	measurement *next;	// pointer to next measurement node
} measurement;

typedef struct measurement_list {
	int number;	// measurement number
	measurement data;	// first measurement data
	measurement_list *next;	// pointer to next measurement list
} measurement_list;

// function prototypes
// load rules loads rule base from a file and return pointer to it
rule_base *load_rules (FILE *f_name);

// insert singly-linked list node
void* insertNode (void **dptr_head, void *value);
// delete singly-linked list node
void deleteNode (void **dptr_head, void *value);
// search singly-linked list node
void* searchNode (void *ptr_head, void value);
