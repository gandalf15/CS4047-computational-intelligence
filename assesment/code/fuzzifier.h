#ifndef FIZZIFIER_H
#define FUZZIFIER_H

#include "rule_base.h"

double fuzzifySingle(double value, int *tuple);

int fuzzifyAll(var_sets **dptrVarTable, measurement *measurements);

#endif