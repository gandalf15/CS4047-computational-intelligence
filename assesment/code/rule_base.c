#include "rule_base.h"

#define BUF_SIZE 1000	// buffer for one line of characters

// it will terminate a string with proper char '\0'
void copyString (char **dest, char *src) {
	int strlength = strlen(src);
	*dest = malloc(strlength+1*sizeof(char));
	memcpy(*dest, src, strlength);
	(*dest)[strlength] = '\0';
}

void insertVarValPair (var_val_pair **head, var_val_pair **tail, char *name, char *value, char *con) {
	var_val_pair *ptrNewNode = malloc(sizeof(var_val_pair));
	if (!ptrNewNode) {
		printf("\nNot enough memory.");
		return;
	}

	copyString(&ptrNewNode->name, name);
	copyString(&ptrNewNode->value, value);

	if (con != NULL) {
		copyString(&ptrNewNode->connective, con);
	}
	else {
		ptrNewNode->connective = malloc(sizeof(char));
		*(ptrNewNode->connective) = '\0';
	}
	if (*head == NULL) {
		*head = ptrNewNode;
		*tail = ptrNewNode;
	}
	else {
		(*tail)->next = ptrNewNode;
		*tail = ptrNewNode;
	}
}

void insertRuleNode (rule_node **head, int num) {
	rule_node *ptrNewNode = calloc(1,sizeof(rule_node));
	if (!ptrNewNode) {
		printf("\nNot enough memory.");
		return;
	}
	ptrNewNode->num = num;
	ptrNewNode->next = *head;
	*head = ptrNewNode;
}

void addFuzzySet (fuzzy_set **sets_table, char *val_name, int *tuple) {
	fuzzy_set *ptrNewFuzzySet;
	HASH_FIND_STR(*sets_table, val_name, ptrNewFuzzySet);
	if (!ptrNewFuzzySet) {
		ptrNewFuzzySet = calloc(1,sizeof(fuzzy_set));
		if (!ptrNewFuzzySet) {
			printf("\nNot enough memory.");
			return;
		}
		copyString(&ptrNewFuzzySet->val_name, val_name);
		memcpy(ptrNewFuzzySet->tuple, tuple, 4 * sizeof(int));
		HASH_ADD_KEYPTR(hh,*sets_table, ptrNewFuzzySet->val_name, 
						strlen(ptrNewFuzzySet->val_name), ptrNewFuzzySet);
		return;
	}
	printf("\nThe hash table %s already exist. The original remains!\n", val_name);
}

fuzzy_set* findFuzzySet (fuzzy_set **sets_table, char *val_name) {
	fuzzy_set *ptrFoundValSet;
	HASH_FIND_STR(*sets_table, val_name, ptrFoundValSet);
	return ptrFoundValSet;
}

var_sets* addVarSets (var_sets **var_table, char *var_name) {
	var_sets *ptrNewVarSets;
	HASH_FIND_STR(*var_table, var_name, ptrNewVarSets);
	if (!ptrNewVarSets) {
		ptrNewVarSets = calloc(1,sizeof(var_sets));
		if (!ptrNewVarSets) {
			printf("\nNot enough memory.");
			return NULL;
		}
		copyString(&ptrNewVarSets->var_name, var_name);
		ptrNewVarSets->sets_table = NULL;
		HASH_ADD_KEYPTR(hh,*var_table, ptrNewVarSets->var_name, 
						strlen(ptrNewVarSets->var_name), ptrNewVarSets);
		return ptrNewVarSets;
	}
	printf("\nThe hash table %s already exist. The original remains!\n", var_name);
	return NULL;
}

var_sets* findVarSets (var_sets **var_table, char *var_name) {
	var_sets *ptrFoundVarSets;
	HASH_FIND_STR(*var_table, var_name, ptrFoundVarSets);
	return ptrFoundVarSets;
}

void insertMeasurement (measurement **head, char *var_name, double value) {
	measurement *ptrNewNode = calloc(1,sizeof(measurement));
	if (!ptrNewNode) {
		printf("\nNot enough memory.");
		return;
	}
	copyString(&ptrNewNode->var_name, var_name);
	ptrNewNode->value = value;
	ptrNewNode->next = *head;
	*head = ptrNewNode;
}

char* get_next_rule_token(char *(*ptrStrtok)(char *, const char*), char *token) {
	token = ptrStrtok(NULL, " ");
	while ( token != NULL &&
		((strcmp(token, "the") == 0) ||
		(strcmp(token, "is") == 0) ||
		(strcmp(token, "will") == 0) ||
		(strcmp(token, "be") == 0)) ) {
	token = ptrStrtok(NULL, " ");
	}
	return token;
}

rule_base* loadRuleBase (FILE *f_rules) {
	rule_base *ptrRuleBase = calloc(1,sizeof(rule_base));

	char line[BUF_SIZE];
	if (fgets (line, BUF_SIZE, f_rules) != NULL) {
		// get name of rulebase
		copyString(&ptrRuleBase->name, line);
		// count empty lines
		int empty_line_counter = 0;
		// pointer for token from line
		char *token;
		var_sets *var_table;
		// while there is a line read it to the buffer 'line'
		while (fgets (line, BUF_SIZE, f_rules) != NULL) {
			if (strcmp(line, "\n") != 0) {
				// get rid of '\n' char at the end of the line
				line[strlen(line)-1] = ' ';
				// if empty line counter is 1 then Rule section starts here
				if (empty_line_counter == 1) {
					// working in rule section
					token = strtok(line, " ");
					// check if the keyword Rule is there
					if (strcmp(token, "Rule") == 0) {
						// get rule number
						token = strtok(NULL, " ");
						int rule_num;
						if (sscanf(token, "%d", &rule_num) == 1) {
							// create new rule node in rule base							
							insertRuleNode( &(ptrRuleBase->rule_list), rule_num);
							// get next token which should be If
							token = strtok(NULL, " ");
							// check 'If' token
							if (strcmp(token, "If") == 0) {
								// get next token which should be 'the' or name of variable
								token = strtok(NULL, " ");
								// if there is 'the' just ignore it and get new token
								if (strcmp(token, "the") == 0) {
									token = strtok(NULL, " ");
								}
								// while there is a variable value pair then add them to linked
								// list
								// declare new variable used to copy names, etc...
								char *var_name;
								char *value_name;
								char *con;
								while (token != NULL) {
									// copy variable name
									copyString(&var_name, token);
									// get new token which is value of the variable
 									token = get_next_rule_token(&strtok, token);
									copyString(&value_name, token);
									// get new token which is either [and | or | then | EOL]
 									token = get_next_rule_token(&strtok, token);
									if (token != NULL) {
										copyString(&con, token);
									}
									// create new variable value pair for the rule
									insertVarValPair( &(ptrRuleBase->rule_list->head_of_rule), 
														&(ptrRuleBase->rule_list->tail_of_rule), 
														var_name, value_name, con );
									free(var_name);
									var_name = NULL;
									free(value_name);
									value_name = NULL;

									if (con != NULL) {
										free(con);
										con = NULL;
 										token = get_next_rule_token(&strtok, token);
									}
								}
							}
							else printf("\nCannot find If keyword\n");
						}
						else printf("\nCannot read rule number\n");
					}
					else printf("\nError no rule found\n");
				}
				else {
					// working with variable names and their sets
					token = strtok(line, " ");
					char *prev_token;
					copyString(&prev_token, token);

					token = strtok(NULL, " ");
					// there is only variable name, create new hash entry for it
					if (!token) {
						var_table = addVarSets(&(ptrRuleBase->var_table), prev_token);
					}
					else {
						// there is also second token that is tuple or value reading
						// check if the second token is '='
						if (strcmp(token, "=") == 0){
							double value;
							char *m_var_name;
							copyString(&m_var_name, prev_token);
							token = strtok(NULL, " ");
							if (sscanf(token, "%lf", &value) == 1) {
								insertMeasurement(&(ptrRuleBase->measurements), m_var_name, value);						
							}
							else {
								printf("\nValue reading for variable %s failed!\n", m_var_name);
							}
						}
						else{
							// copy the name of value
							char *val_name;
							copyString(&val_name, prev_token);
							// copy the tuple values
							int tuple[4];
							for (int i=0; i<4; ++i) {
								if (sscanf(token, "%d", &tuple[i]) == 1) {
									token = strtok(NULL, " ");								
								}
								else {
									printf("\nReading of tuple for value %s failed!\n", val_name);
								}
							}
							// now add new fuzzy set for the variable
							addFuzzySet(&(var_table->sets_table), val_name, tuple);
							var_table->number_of_sets += 1;	// keep track of how many sets certain variable has
						}
					}
				}
			}
			else {
				++empty_line_counter;
			}
		}
	}
	else {
		printf("\nProblem has occured while loading rule base file.\n");
		return NULL;
	}
	return ptrRuleBase;
}