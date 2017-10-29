#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rule_base.h"
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
	rule_node *ptrRules = ruleBase->rule_list;
	var_val_pair *ptrPair = ptrRules->head_of_rules;
	while (ptrRules != NULL) {
		printf("\nRule number %d:\n", ptrRules->num);
		ptrPair = ptrRules->head_of_rules;
		while (ptrPair != NULL) {
			printf("%s\n", ptrPair->name);
			printf("%s\n", ptrPair->value);
			if (ptrPair->connective != NULL) printf("%s\n", ptrPair->connective);
			ptrPair = ptrPair->next;
		}
		ptrRules = ptrRules->next;
	}

	fclose(ptr_rules);

	return 0;
}
