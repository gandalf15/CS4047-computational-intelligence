#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "rule_base.h"

#define BUF_SIZE 1000	// buffer for one line of characters

void insertVarValPair (var_val_pair **head, var_val_pair **tail, char *name, char *value, char *con) {
	var_val_pair *ptrNewNode = calloc(1,sizeof(var_val_pair));
	if (ptrNewNode == NULL) {
		printf("\nNot enough memory.");
		return;
	}
	int strlength = strlen(name);
	ptrNewNode->name = calloc(strlength+1,sizeof(char));
	strncpy(ptrNewNode->name, name, strlength);
	ptrNewNode->name[strlength] = '\0';
	strlength = strlen(value);
	ptrNewNode->value = calloc(strlength+1,sizeof(char));
	strncpy(ptrNewNode->value, value, strlength);
	ptrNewNode->value[strlength] = '\0';
	if (con != NULL) {
		strlength = strlen(con);
		ptrNewNode->connective = calloc(strlength+1, sizeof(char));
		strncpy(ptrNewNode->connective, con, strlength);
		ptrNewNode->connective[strlength] = '\0';
	}
	else {
		ptrNewNode->connective = '\0';
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
	if (ptrNewNode == NULL) {
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
	if (ptrNewFuzzySet == NULL) {
		fuzzy_set *ptrNewFuzzySet = calloc(1,sizeof(fuzzy_set));
		if (ptrNewFuzzySet == NULL) {
			printf("\nNot enough memory.");
			return;
		}
		int strlength = strlen(val_name);
		ptrNewFuzzySet->val_name = calloc(strlength+1,sizeof(char));
		strncpy(ptrNewFuzzySet->val_name, val_name, strlength);
		ptrNewFuzzySet->val_name[strlength] = '\0';
		memcpy(ptrNewFuzzySet->tuple, tuple, 4 * sizeof(int));
		HASH_ADD_KEYPTR(hh,*sets_table, ptrNewFuzzySet->val_name, strlength, ptrNewFuzzySet);
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
	if (ptrNewVarSets == NULL) {
		ptrNewVarSets = calloc(1,sizeof(var_sets));
		if (ptrNewVarSets == NULL) {
			printf("\nNot enough memory.");
			return NULL;
		}
		int strlength = strlen(var_name);
		ptrNewVarSets->var_name = calloc(strlength+1,sizeof(char));
		strncpy(ptrNewVarSets->var_name, var_name, strlength);
		ptrNewVarSets->var_name[strlength] = '\0';
		ptrNewVarSets->sets_table = NULL;
		HASH_ADD_KEYPTR(hh,*var_table, ptrNewVarSets->var_name, strlength, ptrNewVarSets);
		return ptrNewVarSets;
	}
	printf("\nThe hash table %s already exist. The original remains!\n", var_name);
}

var_sets* findVarSets (var_sets **var_table, char *var_name) {
	var_sets *ptrFoundVarSets;
	HASH_FIND_STR(*var_table, var_name, ptrFoundVarSets);
	return ptrFoundVarSets;
}

void insertMeasurement (measurement **head, char *var_name, double value) {
	measurement *ptrNewNode = calloc(1,sizeof(measurement));
	if (ptrNewNode == NULL) {
		printf("\nNot enough memory.");
		return;
	}
	int strlength = strlen(var_name);
	ptrNewNode->var_name = calloc(strlength+1,sizeof(char));
	strncpy(ptrNewNode->var_name, var_name, strlength);
	ptrNewNode->var_name[strlength] = '\0';
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
	int strlength = 0;
	if (fgets (line, BUF_SIZE, f_rules) != NULL) {
		// get name of rulebase
		strlength = strlen(line);
		ptrRuleBase->name = calloc(strlength+1,sizeof(char));
		strncpy(ptrRuleBase->name, line, strlength);
		ptrRuleBase->name[strlength] = '\0';
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
									strlength = strlen(token);
									var_name = calloc(strlength+1, sizeof(char));
									strncpy(var_name, token, strlength);
									var_name[strlength] = '\0';
									// get new token which is value of the variable
 									token = get_next_rule_token(&strtok, token);
									strlength = strlen(token);
									value_name = calloc(strlength+1, sizeof(char));
									strncpy(value_name, token, strlength);
									value_name[strlength] = '\0';
									// get new token which is either [and | or | then | EOL]
 									token = get_next_rule_token(&strtok, token);
									if (token != NULL) {
										strlength = strlen(token);
										con = calloc(strlength+1, sizeof(char));
										strncpy(con, token, strlen(token));
										con[strlength] = '\0';
									}
									// create new variable value pair for the rule
									insertVarValPair( &(ptrRuleBase->rule_list->head_of_rules), 
														&(ptrRuleBase->rule_list->tail_of_rules), 
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
					strlength = strlen(token);
					prev_token = calloc(strlength+1, sizeof(char));
					strncpy(prev_token, token, strlength);
					prev_token[strlength] = '\0';

					token = strtok(NULL, " ");
					// there is only variable name, create new hash entry for it
					if (token == NULL) {
						var_table = addVarSets(&(ptrRuleBase->var_table), prev_token);
					}
					else {
						// there is also second token that is tuple or value reading
						// check if the second token is '='
						if (strcmp(token, "=") == 0){
							char *var_name;
							double value;
							strlength = strlen(prev_token);
							char *m_var_name = calloc(strlength+1, sizeof(char));
							strncpy(m_var_name, prev_token, strlength);
							m_var_name[strlength] = '\0';
							token = strtok(NULL, " ");
							if (sscanf(token, "%lf", &value) == 1) {
								insertMeasurement(&(ptrRuleBase->measurements), m_var_name, value);						
							}
							else {
								printf("\nValue reading for variable %s failed!\n", var_name);
							}
						}
						else{
							// copy the name of value
							strlength = strlen(prev_token);
							char *val_name = calloc(strlength+1, sizeof(char));
							strncpy(val_name, prev_token, strlength);
							val_name[strlength] = '\0';
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
