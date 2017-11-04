#ifndef FIZZIFIER_H
#define FUZZIFIER_H

#include "rule_base.h"

double fuzifySingle(double value, int *tuple);

int fuzifyAll(var_sets **dptrVarTable, measurement *measurements);

#endif