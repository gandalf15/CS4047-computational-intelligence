#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rule_base.h"
#include "fuzzifier.h"
#include "inf_engine.h"
#include "defuzzifier.h"

int main(int argc, char *argv[]) {
	FILE *ptr_rules;
	rule_base *ruleBase;
	if (argc != 2) {
		printf("\nIt takes one argument that is the rulebase file\n");
		return 1;
	}

	ptr_rules = fopen(argv[1], "r");
	if (!ptr_rules) {
		printf("\nRule file cannot be opened. Check if it's in same directory\n");
		return 1;
	}

	ruleBase = loadRuleBase(ptr_rules);
	fclose(ptr_rules);

	fuzifyAll(&ruleBase->var_table, ruleBase->measurements);
	set_signal *signalsFromEngine = NULL;
	signalsFromEngine = inferenceEngine(ruleBase->rule_list, &ruleBase->var_table);
	double output_crisp_val = defuzzify(signalsFromEngine, &ruleBase->var_table);
	printf("\n\n%lf\n\n",output_crisp_val);
	printf("\nPress Return key to exit\n");
	getc(stdin);
	return 0;
}