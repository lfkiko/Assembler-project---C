/*
This functions are for the writing process, creating files and writing into them when needed.
*/

#include "main_header.h"			/*The all inclusive header*/

/*
This function wrute the OBJ file
*/
int WRITING_OB(FILE** OBJ_fp, OBJ_LIST** OBJ_HEAD, char* name, int IC, int DC) {
	
	OBJ_LIST *OBJ_curr = *OBJ_HEAD;
	char* FILE_NAME_ob = malloc(sizeof(name)+4);											
	
	if(FILE_NAME_ob == NULL)
		return 1;
	strcpy(FILE_NAME_ob, name);
	strcat(FILE_NAME_ob, ".ob");
	*OBJ_fp = fopen(FILE_NAME_ob, "w");																/*creating OBJ file*/
	fprintf(*OBJ_fp, "\t%d %d\n", (IC-DC-100), DC);
	while(OBJ_curr != NULL) {
		fprintf(*OBJ_fp, "%04d %03X %c\n", OBJ_curr->IC, OBJ_curr->MACHINE_CODE, OBJ_curr->ARE_LABEL);
	}
	fclose(*OBJ_fp);
	free(FILE_NAME_ob);
	return 0;
} /*End of WRITE_OBJ*/


/*
This function checks the SYMBOL list for external and entry cases
*/
void CHECK_EXT_ENT(SYMBOL_LIST** SYMBOL_HEAD, int* extrn_status, int* entry_status) {
	
	SYMBOL_LIST *SYMBOL_curr = *SYMBOL_HEAD;
	
	*extrn_status = 0;
	while(SYMBOL_curr != NULL) {
		if(strcmp(SYMBOL_curr->ATTRIBUTES, "external") == 0) {										/*checking the SYMBOL list for external cases*/
			*extrn_status = 1;
			break;
		}
		else {
			SYMBOL_curr = SYMBOL_curr->NEXT;
		}
	}
	SYMBOL_curr = *SYMBOL_HEAD;
	
	*entry_status = 0;
	while(SYMBOL_curr != NULL) {
		if(strcmp(SYMBOL_curr->ATTRIBUTES, "entry") == 0) {											/*checking the SYMBOL list for entry cases*/
			*entry_status = 1;
			break;
		}
		else {
			SYMBOL_curr = SYMBOL_curr->NEXT;
		}
	}
	return;
} /*END of CHECK_EXT_ENT*/


/*
This function wrute the EXT file
*/
int WRITING_EXT(FILE** EXT_fp, OBJ_LIST** OBJ_HEAD, SYMBOL_LIST** SYMBOL_HEAD, char* name) {
	
	OBJ_LIST *OBJ_curr = *OBJ_HEAD;
	SYMBOL_LIST *SYMBOL_curr = *SYMBOL_HEAD;
	char* FILE_NAME_ext = malloc(sizeof(name)+5);
	
	if(name == NULL) {
		return 1;
	}
	strcpy(FILE_NAME_ext, name);
	strcat(FILE_NAME_ext, ".ext");
	*EXT_fp = fopen(FILE_NAME_ext, "w");
	while(SYMBOL_curr != NULL) {
		if(strcmp(SYMBOL_curr->ATTRIBUTES, "external") == 0){
			while(OBJ_curr != NULL) {
				if((strcmp(OBJ_curr->NAME, SYMBOL_curr->NAME)== 0)&& (OBJ_curr->ARE_LABEL == 'E')){
					fprintf(*EXT_fp, "%s	%04d\n", OBJ_curr->NAME, OBJ_curr->IC);
				}
				OBJ_curr = OBJ_curr->NEXT;
			}
			OBJ_curr = *OBJ_HEAD;
		}
		SYMBOL_curr = SYMBOL_curr->NEXT;
	}
	
	fclose(*EXT_fp);
	free(FILE_NAME_ext);
	return 0;
	
} /*End of WRITING_EXT*/


/*This function wrute the ENT file*/
int WRITING_ENT(FILE** ENT_fp, OBJ_LIST** OBJ_HEAD, SYMBOL_LIST** SYMBOL_HEAD, char* name) {
	
	OBJ_LIST *OBJ_curr = *OBJ_HEAD;
	SYMBOL_LIST *SYMBOL_curr = *SYMBOL_HEAD;
	char* FILE_NAME_ent = malloc(sizeof(name)+5);
	
	if(FILE_NAME_ent == NULL) {
		return 1;
	}
	strcpy(FILE_NAME_ent, name);
	strcat(FILE_NAME_ent, ".ent");
	*ENT_fp = fopen(FILE_NAME_ent, "w");
	while(SYMBOL_curr != NULL) {
		if(strcmp(SYMBOL_curr->ATTRIBUTES, "entry") == 0) {
			while(OBJ_curr != NULL) {
				if((strcmp(OBJ_curr->NAME, SYMBOL_curr->NAME) == 0) && (OBJ_curr->ARE_LABEL == 'A')) {
					fprintf(*ENT_fp, "%s	%04d\n", OBJ_curr->NAME, OBJ_curr->IC);
				}
				OBJ_curr = OBJ_curr->NEXT;
			}
			OBJ_curr = *OBJ_HEAD;
		}
		SYMBOL_curr = SYMBOL_curr->NEXT;
	}
	
	fclose(*ENT_fp);
	free(FILE_NAME_ent);
	return 0;
	
} /*End of WRITING_ENT*/



