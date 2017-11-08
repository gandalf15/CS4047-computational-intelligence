#include "fuzzifier.h"

double fuzzifySingle(double value, int *tuple) {
    double result = 0.0;
    // chack if the value is between 'a' and 'a+alpha'
    if ( (value >= ((double)tuple[0] - (double)tuple[2])) && (value < (double)tuple[0]) ) {
        result = (value - (double)tuple[0] + (double)tuple[2])/(double)tuple[2];
    }
    // else if the value is between 'b' and 'b+beta'
    else if ( (value <= ((double)tuple[1] + (double)tuple[3])) && (value > (double)tuple[1]) ) {
        result = ((double)tuple[1] + (double)tuple[3] - value)/(double)tuple[3];
    }
    // else if the value is between 'a' and 'b'
    else if (value >= (double)tuple[0] && value <= (double)tuple[1]) {
        result = 1.0;
    }
    return result;
}

int fuzzifyAll(var_sets **dptrVarTable, measurement *measurements) {
    // iterate through every measurement
    if (dptrVarTable != NULL && measurements != NULL) {
        while (measurements != NULL) {
            var_sets *currentVarSets;
            // find a hash table for the variable
            HASH_FIND_STR(*dptrVarTable, measurements->var_name, currentVarSets);
            fuzzy_set *currentSet;
            if (currentVarSets != NULL) {
                // iterate through every set and calculate fuzzy value for it
                for (currentSet = currentVarSets->sets_table; currentSet != NULL; currentSet = currentSet->hh.next) {
                    currentSet->last_fuzzy_val = fuzzifySingle(measurements->value, currentSet->tuple);
                    //printf("\nFuzzy value of %s -> %s: %g", measurements->var_name, currentSet->val_name, currentSet->last_fuzzy_val);
                }
            }
            else {
                return 1;
            }
            // progress to the next measurement
            measurements = measurements->next;
        }
        return 0;
    }
    else return 1;
}