#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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
	// measure time to calculate it
	clock_t begin = clock();
	ruleBase = loadRuleBase(ptr_rules);
	fclose(ptr_rules);
	clock_t mid_time = clock();
	fuzifyAll(&ruleBase->var_table, ruleBase->measurements);
	set_signal *signalsFromEngine = NULL;
	signalsFromEngine = inferenceEngine(ruleBase->rule_list, &ruleBase->var_table);
	double output_crisp_val = defuzzify(signalsFromEngine, &ruleBase->var_table);
	// end of measurement
	clock_t end = clock();
	double time_spent_all = ((double)(end - begin) / CLOCKS_PER_SEC)*1000000;
	double time_spent_calc = ((double)(end - mid_time) / CLOCKS_PER_SEC)*1000000;
	printf("\nExecution time including loading the rule base: %.2lf \xC2\xB5s", time_spent_all);
	printf("\nExecution time calculation: %.2lf \xC2\xB5s", time_spent_calc);
	printf("\n\nOutput signal: %lf\n\n",output_crisp_val);
	//printf("\nPress Return key to exit\n");
	//getc(stdin);
	return 0;
}