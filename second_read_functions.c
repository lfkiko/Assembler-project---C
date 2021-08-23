/*
This functions are for the second read
*/

#include "main_header.h"			/*The all inclusive header*/

void SECOND_READ(SYMBOL_LIST** SYMBOL_HEAD, OBJ_LIST** OBJ_HEAD) {
	OBJ_LIST *OBJ_curr = *OBJ_HEAD;
	SYMBOL_LIST *SYMBOL_curr = *SYMBOL_HEAD;
	
	while(SYMBOL_curr != NULL) {															
		while(OBJ_curr != NULL) {
			if((strcmp(SYMBOL_curr->NAME, OBJ_curr->NAME) == 0) && (OBJ_curr->ARE_LABEL == 'N')) {
			OBJ_curr->MACHINE_CODE = SYMBOL_curr->IC;
				if(OBJ_curr->MACHINE_CODE == 0) {												/*checks wich "A,R,E" label it needs*/
					OBJ_curr->ARE_LABEL = 'E';													/*Relabeling A.R.E*/
				}
				else {
					OBJ_curr->ARE_LABEL = 'R';													/*Relabeling A.R.E*/
				}
			}
			OBJ_curr = OBJ_curr->NEXT;
		}
		SYMBOL_curr = SYMBOL_curr->NEXT;
	}	/*End of srcond read*/
}
