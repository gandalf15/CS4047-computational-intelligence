#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "rule_base.h"

#define BUF_SIZE 1000	// buffer for one line of characters

var_val_pair* insertVarValPair (var_val_pair **head, var_val_pair **tail, char *name, char *value, char *con) {
	var_val_pair *ptrNewNode = calloc(1,sizeof(var_val_pair));
	if (ptrNewNode == NULL) {
		printf("\nNot enough memory.");
		return NULL;
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
	return *head;
}

rule_node* insertRuleNode (rule_node **head, int num) {
	rule_node *ptrNewNode = calloc(1,sizeof(rule_node));
	if (ptrNewNode == NULL) {
		printf("\nNot enough memory.");
		return NULL;
	}
	ptrNewNode->num = num;
	ptrNewNode->next = *head;
	*head = ptrNewNode;
	return *head;
}

fuzzy_set* insertFuzzySet (fuzzy_set **head, char *val_name, int *tuple) {
	fuzzy_set *ptrNewNode = calloc(1,sizeof(fuzzy_set));
	if (ptrNewNode == NULL) {
		printf("\nNot enough memory.");
		return NULL;
	}
	int strlength = strlen(val_name);
	ptrNewNode->val_name = calloc(strlength+1,sizeof(char));
	strncpy(ptrNewNode->val_name, val_name, strlength);
	ptrNewNode->val_name[strlength] = '\0';
	memcpy(ptrNewNode->tuple, tuple, 4 * sizeof(int));
	ptrNewNode->next = *head;
	*head = ptrNewNode;
	return *head;
}

var_sets* insertVarSets (var_sets **head, char *var_name, fuzzy_set *set) {
	var_sets *ptrNewNode = calloc(1,sizeof(var_sets));
	if (ptrNewNode == NULL) {
		printf("\nNot enough memory.");
		return NULL;
	}
	int strlength = strlen(var_name);
	ptrNewNode->var_name = calloc(strlength+1,sizeof(char));
	strncpy(ptrNewNode->var_name, var_name, strlength);
	ptrNewNode->var_name[strlength] = '\0';
	ptrNewNode->set = set;
	//ptrNewNode->next = *head;
	*head = ptrNewNode;
	return *head;
}

measurement* insertMeasurement (measurement **head, char *var_name, double value) {
	measurement *ptrNewNode = calloc(1,sizeof(measurement));
	if (ptrNewNode == NULL) {
		printf("\nNot enough memory.");
		return NULL;
	}
	int strlength = strlen(var_name);
	ptrNewNode->var_name = calloc(strlength+1,sizeof(char));
	strncpy(ptrNewNode->var_name, var_name, strlength);
	ptrNewNode->var_name[strlength] = '\0';
	ptrNewNode->value = value;
	ptrNewNode->next = *head;
	*head = ptrNewNode;
	return *head;
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
		// while there is a line read it to the buffer 'line'
		while (fgets (line, BUF_SIZE, f_rules) != NULL) {
			printf("\nRead new line\n");
			printf("%s", line);
			if (strcmp(line, "\n") != 0) {
				// if empty line counter is 1 then Rule section starts here
				if (empty_line_counter == 1) {
					printf("\nNumber of empty lines: %d\n", empty_line_counter);
					// working in rule section
					// get rid of '\n' char at the end of the line
					line[strlen(line)-1] = ' ';
					token = strtok(line, " ");
					// check if the keyword Rule is there
					if (strcmp(token, "Rule") == 0) {
						// get rule number
						token = strtok(NULL, " ");
						int rule_num;
						if (sscanf(token, "%d", &rule_num) == 1) {
							// create new rule node in rule base							
							insertRuleNode( &(ptrRuleBase->rule_list), rule_num);
							printf("Got rule number: %d", rule_num);
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
								
									printf("\nBeginning of while. Current token: %s\n", token);
									// copy variable name
									printf("\n var_name. Current token: %s\n", token);
									strlength = strlen(token);
									var_name = calloc(strlength+1, sizeof(char));
									strncpy(var_name, token, strlength);
									var_name[strlength] = '\0';
									// get new token which is value of the variable
 									token = get_next_rule_token(&strtok, token);
									printf("\n value_name. Current token: %s\n", token);
									strlength = strlen(token);
									value_name = calloc(strlength+1, sizeof(char));
									strncpy(value_name, token, strlength);
									value_name[strlength] = '\0';
									// get new token which is either [and | or | then | EOL]
 									token = get_next_rule_token(&strtok, token);
									if (token != NULL) {
										printf("\n Connective -> Current token: %s\n", token);
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
					printf("\nReached end of Rules\n");
					token = strtok(line, " ");
					char *prev_token;
					strlength = strlen(token);
					prev_token = calloc(strlength+1, sizeof(char));
					strncpy(prev_token, token, strlength);
					prev_token[strlength] = '\0';
					token = strtok(NULL, " ");
					if (token == NULL) {
						insertVarSets(&(ptrRuleBase->sets_list), )
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
