#include "inf_engine.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void addSetSignal (set_signal **set_signals_table, char *var_name, char *val_name, double out_signal) {
	set_signal *ptrValSignal;
	HASH_FIND_STR(*set_signals_table, val_name, ptrValSignal);
	if (!ptrValSignal) {
		ptrValSignal = malloc(sizeof(set_signal));
		if (!ptrValSignal) {
			printf("\nNot enough memory.");
			return;
        }
        else {
            copyString(&ptrValSignal->var_name, var_name);
            copyString(&ptrValSignal->val_name, val_name);
            ptrValSignal->out_signal = out_signal;
            HASH_ADD_KEYPTR(hh,*set_signals_table, ptrValSignal->val_name, 
                            strlen(ptrValSignal->val_name), ptrValSignal);
        }
    }
    else {
        // there already exist a signal for this value. Do MAX on them and update the signal
        ptrValSignal->out_signal = MAX(ptrValSignal->out_signal, out_signal);
    }
    //printf("\n\nInf engine: Variable: %s\nValue: %s\nSignal: %g", ptrValSignal->var_name, ptrValSignal->val_name, ptrValSignal->out_signal);
}


set_signal *inferenceEngine(rule_node *ptrRules, var_sets **dptrVarTable) {
    // create pointer of hash table for results
    set_signal *ptrConseqTable = NULL;

    var_val_pair *ptrRulePair;
    var_sets *ptrVariableSets;
    fuzzy_set *val_set;
    double OutSignal = 0;
    
    // while there is a rule in the list
	while (ptrRules != NULL) {
        // get the next rule head
        ptrRulePair = ptrRules->head_of_rule;

        // check what is the connective
        if ( strcmp(ptrRulePair->connective, "and") == 0 ) {
            // there is more variable value pairs in the rule, all the connectives need to be 'and'
            OutSignal = 1.0;

            while ( strcmp(ptrRulePair->connective, "and") == 0 || (strcmp(ptrRulePair->connective, "then") == 0) ) {
                // get table of sets for variable
                ptrVariableSets = findVarSets (dptrVarTable, ptrRulePair->name);
                // get the current value for the set based on it's name
                val_set = findFuzzySet( &(ptrVariableSets->sets_table), ptrRulePair->value );
                OutSignal = MIN(OutSignal, val_set->last_fuzzy_val);
                ptrRulePair = ptrRulePair->next;
            }
            if ( *(ptrRulePair->connective) == '\0' ) {
                // add the resulting signal to the consequence hash table
                addSetSignal( &(ptrConseqTable), ptrRulePair->name, ptrRulePair->value, OutSignal);
            }
            else {
                printf("Connective mismatch. \nExpected either 'and' or 'then'\nGot: %s", ptrRulePair->connective);
                return NULL;
            }
        }
        else if (strcmp(ptrRulePair->connective, "or") == 0 ) {
            // there is more variable value pairs in the rule, all the connectives need to be 'or'
            OutSignal = 0;
            while ( strcmp(ptrRulePair->connective, "or") == 0 || (strcmp(ptrRulePair->connective, "then") == 0) ) {
                // get table of sets for variable
                ptrVariableSets = findVarSets (dptrVarTable, ptrRulePair->name);
                // get the current value for the set based on it's name
                val_set = findFuzzySet( &(ptrVariableSets->sets_table), ptrRulePair->value );
                OutSignal = MAX(OutSignal, val_set->last_fuzzy_val);
                ptrRulePair = ptrRulePair->next;
            }
            if ( *(ptrRulePair->connective) == '\0' ) {
                // add the resulting signal to the consequence hash table
                addSetSignal( &(ptrConseqTable), ptrRulePair->name, ptrRulePair->value, OutSignal);
            }
            else {
                printf("Connective mismatch. Expected either 'or' or 'then'\nGot: %s", ptrRulePair->connective);
                return NULL;
            }
        }
        else if (strcmp(ptrRulePair->connective, "then") == 0 ) {
            // the next value is consequence
            // get table of sets for variable
            ptrVariableSets = findVarSets (dptrVarTable, ptrRulePair->name);
            // get the current value for the set based on it's name
            val_set = findFuzzySet( &(ptrVariableSets->sets_table), ptrRulePair->value );
            // progress to the next
            ptrRulePair = ptrRulePair->next;
            if ( *(ptrRulePair->connective) == '\0' ) {
                // add the resulting signal to the consequence hash table
                addSetSignal( &(ptrConseqTable), ptrRulePair->name, ptrRulePair->value, val_set->last_fuzzy_val);
            }
            else {
                printf("Connective mismatch. Expected 'then'\nGot: %s", ptrRulePair->connective);
                return NULL;
            }
        }
        else {
            printf("\nGot unexpected connective: %s\n", ptrRulePair->connective);
            return NULL;
        }
		ptrRules = ptrRules->next;
	}
    return ptrConseqTable;
}