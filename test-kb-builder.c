
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rule_base.h"
#include "fuzzifier.h"

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
	rule_node *ptrRules = ruleBase->rule_list;
	var_val_pair *ptrPair = ptrRules->head_of_rule;
	while (ptrRules != NULL) {
		printf("\nRule number %d:\n", ptrRules->num);
		ptrPair = ptrRules->head_of_rule;
		while (ptrPair != NULL) {
			printf("%s\n", ptrPair->name);
			printf("%s\n", ptrPair->value);
			if (ptrPair->connective != NULL) printf("%s\n", ptrPair->connective);
			ptrPair = ptrPair->next;
		}
		ptrRules = ptrRules->next;
	}

	var_sets *var_table;
	fuzzy_set *sets;

	/* iterate over hash elements  */
	for(var_table=ruleBase->var_table; var_table != NULL; var_table=var_table->hh.next) {
		printf("\n\nVariable name: %s\n", var_table->var_name);
		for (sets=var_table->sets_table; sets != NULL; sets=sets->hh.next) {
			printf("\nValue name: %s\nTuple: ", sets->val_name);
			for (char i=0; i<4; ++i) {
				printf(" %d ", sets->tuple[i]);
			}
		}

	}
	printf("\nMeasurements: \n");

	measurement *m;
	for (m=ruleBase->measurements; m!= NULL; m=m->next) {
		printf("\nVariable name: %s\nValue: %lf\n", m->var_name, m->value);
	}

	return 0;
}