/*
This functions are for the first read
*/

#include "main_header.h"			/*The all inclusive header*/

int FUNC_PRES[NUM_OF_FUNC] = {mov_b, cmp_b, add_b, sub_b, lea_b, clr_b, not_b, inc_b, dec_b, jmp_b, bne_b, jsr_b, red_b, prn_b, rts_b, stop_b};

/*
This function checks for LABELs in the beginning of the line.
*/
void LABEL_CHECK(char** TEMP, char** LABEL, status* assembler, int LC) {

	if(*TEMP[strlen(*TEMP) - 1] == ':') {															/*checks for LABEL*/
		if((*TEMP[strlen(*TEMP) - 2] != ' ') && (*TEMP[strlen(*TEMP) - 2] != '\t')) {
			*LABEL = *TEMP;
			*TEMP = strtok(NULL, "	");
			*LABEL[strlen(*LABEL) - 1] = '\0';
		}
		else {
			printf("ERROR in line %d, %s is not an avalide LABEL.\n", LC, *TEMP);
			*assembler = error;
		}
	}
	else {
		*LABEL = NULL;
	}
} /*End of LABEL_CHECK*/


/*
This function checks for DIRECTIVE in a line.
*/
int DIRECTIVE_CHECK(char** TEMP, char** DIRECTIVE, char** LABEL) {

	if(*TEMP[0] == '.') {																			/*check for a DIRECTIVE prefix*/
		*DIRECTIVE = *TEMP;
		if((strncmp(*DIRECTIVE, ".extern", 7) == 0) || (strncmp(*DIRECTIVE, ".entry", 6) == 0)) {
			return 0;			
		}
		else if(strcmp(*DIRECTIVE, ".string") == 0) {
			if(*LABEL != NULL) {
				return 1;
			}
			return 2;						
		}
		else if(strcmp(*DIRECTIVE, ".data") == 0) {
			if(*LABEL != NULL) {
				return 3;
			}
			return 4;
		}		
	}
    else {
    	*DIRECTIVE = NULL;
    	if(*LABEL != NULL) {
    		return 5;
    	}
    	return 6;
    }
    return 7;
} /*End of DIRECTIVE_CHECK*/


/*
This function checks for FUNCTION in the "library" and verifies the operands if needed.
*/
void FUNCTION_CHECK(int j, char** TEMP, char** LABEL, char** DIRECTIVE, char** FIRST_OP, char** SECOND_OP, char** OPCODE_FUNCT,
					OBJ_LIST** OBJ_HEAD, status* assembler, int* LC, int* IC, int* DC, int first_op_val, int second_op_val) {
					
	int first_op, second_op, opcode_funct, machine_code;
	switch (j) { 
/*mov*/					case 0: 	
/*cmp*/					case 1:
/*add*/					case 2:
/*sub*/					case 3:
/*lea*/					case 4:
							*TEMP = strtok(NULL, ",");
							*FIRST_OP = *TEMP;
							*TEMP = strtok(NULL, "\n");
							if(*TEMP == NULL) {
								printf("ERROR in line %d, not enough operands for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							if(*TEMP[0] == ' ') {
								TEMP++;
							}
							*SECOND_OP =*TEMP;
							opcode_funct = FUNC_PRES[j];
							first_op = ADDRESSING_METHODE(*FIRST_OP, 1);
							if(((j == 4) && (first_op != 4)) || (first_op == 8)) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							second_op = ADDRESSING_METHODE(*SECOND_OP, 2);
							if(((j == 1) && (second_op == 2)) || ((j != 1) && ((second_op == 0) || (second_op == 2)))) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							machine_code = (opcode_funct | (first_op | second_op));
							ADD_TO_OBJ(OBJ_HEAD, *LABEL, *DIRECTIVE,*OPCODE_FUNCT , IC, DC, machine_code, 'A', *LC, assembler);
							first_op_val = OPERAND_VALUE(*FIRST_OP, *DIRECTIVE, *LC, assembler);
							ADD_TO_OBJ(OBJ_HEAD, NULL, *DIRECTIVE, *FIRST_OP, IC, DC, first_op_val, CHECK_ARE(*FIRST_OP, *LC, assembler), *LC,
							assembler);
							second_op_val = OPERAND_VALUE(*SECOND_OP, *DIRECTIVE, *LC, assembler);
							ADD_TO_OBJ(OBJ_HEAD, NULL, *DIRECTIVE, *SECOND_OP, IC, DC, second_op_val, CHECK_ARE(*SECOND_OP,*LC,assembler), *LC,
							assembler);
							*TEMP = strtok(NULL, " ");
							if(*TEMP != NULL) {
								printf("ERROR in line %d, to meny operands for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							break;
	 
/*clr*/					case 5:
/*not*/					case 6:
/*inc*/					case 7:
/*dec*/					case 8:
/*jmp*/					case 9:
/*bne*/					case 10:
/*jsr*/					case 11:
/*red*/					case 12:
/*prn*/					case 13:
							*TEMP = strtok(NULL, "\n");
							if(*TEMP == NULL) {
								printf("ERROR in line %d, not enough operands for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							*FIRST_OP = *TEMP;
							opcode_funct = FUNC_PRES[j];
							first_op = ADDRESSING_METHODE(*FIRST_OP, 2);
							if((j != 13) && (first_op == 0)){
								printf("ERROR in line %d, wrong addresing method for for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							if(((j >= 9) && (j <= 11)) && (first_op == 3)) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							if(!((j >= 9) && (j <= 11)) && (first_op == 2)) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							machine_code = (opcode_funct | first_op);
							ADD_TO_OBJ(OBJ_HEAD, *LABEL, *DIRECTIVE, *OPCODE_FUNCT , IC, DC, machine_code, 'A', *LC, assembler);
							first_op_val = OPERAND_VALUE(*FIRST_OP, *DIRECTIVE, *LC, assembler);
							ADD_TO_OBJ(OBJ_HEAD, NULL, *DIRECTIVE,*FIRST_OP , IC, DC, first_op_val, CHECK_ARE(*FIRST_OP, *LC, assembler), *LC,
							assembler);
							*TEMP = strtok(NULL, " ");
							if(*TEMP != NULL) {
								printf("ERROR in line %d, to meny operands for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							break;

/*rts*/					case 14:
/*stop*/					case 15:
							*TEMP = strtok(NULL, " ");
							if(*TEMP != NULL) {
								printf("ERROR in line %d, no need in operands for %s.\n", *LC, *OPCODE_FUNCT);
								*assembler = error;
							}
							*OPCODE_FUNCT[sizeof(*OPCODE_FUNCT)] = '\0';
							opcode_funct = FUNC_PRES[j];
							ADD_TO_OBJ(OBJ_HEAD, *LABEL, *DIRECTIVE, *OPCODE_FUNCT, IC, DC, opcode_funct, 'A', *LC, assembler);
							break;
	
						default:
							printf("ERROR in line %d, %s is no a function.\n", *LC, *OPCODE_FUNCT);
							*assembler = error;
					}	/*end of switch*/
}
