#ifndef RULE_BASE_H
#define RULE_BASE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "uthash.h"

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
	var_val_pair *head_of_rule;	// pointer to list of variable value pair and it's connectives
	var_val_pair *tail_of_rule;
	struct rule_node *next;	//pointer to the next rule
} rule_node;

typedef struct fuzzy_set {
	char *val_name;	// value name is also hash KEY
	int tuple[4];	// tuple representing the set
	double last_fuzzy_val;	// last value after fuzzification
	UT_hash_handle hh;	// makes this structure hashable
} fuzzy_set;

typedef struct variable_fuzzy_sets {
	// nameof the variable which has many fuzzy sets.
	char *var_name;	// Also it's hash KEY
	int number_of_sets;
	struct fuzzy_set *sets_table;	// values of fuzzy sets in hash table
	UT_hash_handle hh;	// makes this structure hashable
} var_sets;

typedef struct rule_base {
	char *name;	// name of rulebase
	rule_node *rule_list;	// list of rules in rule base
	var_sets *var_table;	//	hash table of variables that each has a hash table of sets
	struct measurement *measurements;
} rule_base;

typedef struct measurement {
	char *var_name;	// name of the measured variable
	double value;	// value of the variable
	struct measurement *next;	// pointer to next measurement node
} measurement;

// function prototypes
// load rules loads rule base from a file and return pointer to it
rule_base* loadRuleBase (FILE *f_name);
var_sets* findVarSets (var_sets **var_table, char *var_name);
fuzzy_set* findFuzzySet (fuzzy_set **sets_table, char *val_name);
void copyString(char **dest, char *src);

//	insert node to list
void insertMeasurement (measurement **head, char *var_name, double value);
#endif