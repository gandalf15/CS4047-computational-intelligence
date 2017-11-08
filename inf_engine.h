#ifndef INF_ENGINE_H
#define INF_ENGINE_H

#include "rule_base.h"

typedef struct set_signal {
	char *var_name;	// variable name
    char *val_name;	// KEY - value of the variable
    double out_signal;   // output signal for the specific value name
    UT_hash_handle hh;	// makes this structure hashable
    
} set_signal;

// function prototype
set_signal *inferenceEngine(rule_node *ptrRule, var_sets **ptrVarTable);

#endif