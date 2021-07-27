
/*
This function are manging the OBG ans SYMBOL lists.
*/

#include "main_header.h"			/*The all inclusive header*/


/*
This function add a node to the OBJ list
*/
void ADD_TO_OBJ(OBJ_LIST** HEAD, char* LABEL, char* DIRECTIVE, char* OPCODE_OP, int* IC, int* DC, int MACHINE_CODE, char ARE_VAL, int LC, status* assembler) {
	OBJ_LIST* curr = NULL;
	OBJ_LIST* NEW = malloc(sizeof(OBJ_LIST)); 
	if(NEW == NULL)
		exit(1);

	if((LABEL == NULL) && (DIRECTIVE == NULL)) {
		strcpy(NEW->NAME, OPCODE_OP);
	}
	else if ((LABEL != NULL) && (DIRECTIVE == NULL)) {
		strcpy(NEW->NAME, LABEL);
	}
	else {
		(*DC)++;
		if(LABEL != NULL){
			strcpy(NEW->NAME, LABEL);
			
		}
		else {
			strcpy(NEW->NAME, DIRECTIVE);
		}
	} 

	NEW->IC = (*IC)++;
	NEW->MACHINE_CODE = MACHINE_CODE;
	NEW->ARE_LABEL = ARE_VAL;
	NEW->NEXT = NULL;

	if(*HEAD == NULL) {
		*HEAD = NEW;	
		return;
	}
	
	curr = *HEAD;
	while(curr->NEXT != NULL) {
		curr = curr->NEXT;
	}
	curr->NEXT = NEW;
}	/*End of ADD_TO_OBJ*/


/*
This function add node to the SYMBOL list or updating nodes int the list.
*/
void ADD_TO_SYMBOL(SYMBOL_LIST** HEAD, char* DIRECTIVE, char* NAME, int IC, int LC, status* assembler) {
	SYMBOL_LIST* curr = *HEAD;
	SYMBOL_LIST* NEW = NULL;
	if(*HEAD != NULL) {
		while(curr != NULL) {	
			if(strcmp(curr->NAME, NAME) == 0) {														/*check for a match*/
				if(strcmp(curr->ATTRIBUTES, "external") == 0) {										/*allready exit is external*/
					printf("ERROR in line %d, %s can be only external.\n", LC, NAME);
					*assembler = error;
					return;
				}
				else {
					if((strcmp(DIRECTIVE, ".entry") == 0) && (strcmp(curr->COD_DAT, "") != 0)) {
						strcpy(curr->ATTRIBUTES, "entry");
						return;
					}
					else if(strcmp(curr->ATTRIBUTES, "entry") == 0) {
						if((DIRECTIVE == NULL) && (strcmp(curr->COD_DAT, "") == 0)) {
							strcpy(curr->COD_DAT, "code");
							curr->IC = IC;
							return;
						}
						else if((DIRECTIVE != NULL) && (strcmp(curr->COD_DAT, "") == 0)) {
							strcpy(curr->COD_DAT, "data");
							curr->IC = IC;
							return;
						}
					}
					else {
						printf("ERROR in line %d, %s cant be data and code.\n", LC, NAME);
						*assembler = error;
						return;
					}
    	        }
			}	/*end of strcmp(curr->NAME,NAME) == 0*/
			curr = curr->NEXT;
		}	/*end of looking for a match in the list*/
	}
	curr = *HEAD;
	NEW =(SYMBOL_LIST*) malloc(sizeof(SYMBOL_LIST));
	if(NEW == NULL)
		exit(1);
		
	NEW->IC = IC;
	if (IC == 0) {
		strcpy(NEW->NAME, NAME);
		if(strcmp(DIRECTIVE, ".entry") == 0) {
			strcpy(NEW->ATTRIBUTES, "entry");
		}
		else if(strcmp(DIRECTIVE, ".extern") == 0) {
			strcpy(NEW->ATTRIBUTES, "external");
		}
	}
	else {
		strcpy(NEW->NAME, NAME);
		if(DIRECTIVE != NULL) {
			strcpy(NEW->COD_DAT, "data");
		}
		else {
			strcpy(NEW->COD_DAT, "code");
		}
	}
	NEW->NEXT = NULL;
	if(*HEAD == NULL) {						/*if the list ist empty sets the new node as head*/
		*HEAD = NEW;
		return;
	}
	while(curr->NEXT != NULL) {				/*looking for the last node on the list and sets the new as next*/
		curr = curr->NEXT;
	}
	curr->NEXT = NEW;	
}	/*End of ADD_TO_SYMBOL*/


/*
This function deletes the Node list from the memory
*/
void DELETE_LISTS(OBJ_LIST** ObHead, SYMBOL_LIST** SyHead) {
	OBJ_LIST* ObCurr = *ObHead;
	SYMBOL_LIST* SyCurr = *SyHead;
	while((ObCurr != NULL) || (SyCurr != NULL)) {
		if(ObCurr != NULL) {
			OBJ_LIST* ObTemp = ObCurr;
			ObCurr = ObCurr->NEXT;
			free(ObTemp);
		}
		if(SyCurr != NULL) {
			SYMBOL_LIST* SyTemp = SyCurr;
			SyCurr = SyCurr->NEXT;
			free(SyTemp);
		}
	}
}	/*End of DELETE_LISTS*/














