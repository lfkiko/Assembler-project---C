/*
By: Lior Feigenbaum
Last Updated: 11/03/2021
This is the code for the main program wich reads the files and write new files if needed.
*/

#include "main_header.h"			/*The all inclusive header*/

char *FUNC_NAMES[NUM_OF_FUNC] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop\n"};
int FUNC_PRES[NUM_OF_FUNC] = {mov_b, cmp_b, add_b, sub_b, lea_b, clr_b, not_b, inc_b, dec_b, jmp_b, bne_b, jsr_b, red_b, prn_b, rts_b, stop_b};

int main(int argc, char* argv[]) {
	FILE *READ_fp, *OBJ_fp, *ENT_fp, *EXT_fp;														/*Pointers to input files and out put files*/
	char *line, *FILE_NAME;
	status assembler;																				/*Pointers to readed line's buffer*/
	int i , j, machine_code, opcode_funct, first_op, first_op_val, second_op, second_op_val;
	int IC, DC, LC;	
	OBJ_LIST *OBJ_curr = NULL, *OBJ_HEAD = NULL;
	SYMBOL_LIST *SYMBOL_curr = NULL, *SYMBOL_HEAD = NULL;																		
	
	if (argc == 1) {																				/*checking for files*/
		printf("ERROR!, No avileble files \n");														/*No files were added*/
		return 0;
	}	/*end of argc == 1 if*/
	
	line = (char*) malloc(LINE_LENGTH + 1);															/*buffer for the line*/
	if(line == NULL)																				/*check there's place in the memory*/
		exit(1);

	while(--argc > 0) {																				/*main loop break when thre are no more files*/
		FILE_NAME = (char*) malloc(sizeof(*++argv) + 4);
		strcpy(FILE_NAME, *++argv);
		strcat(FILE_NAME, ".as");
		FILE_NAME[sizeof(*argv)+4] = '\0';
		READ_fp = fopen(FILE_NAME, "r");
		if(READ_fp == NULL)	{
			printf("ERROR!, the file %s cannot be open or has no content\n", FILE_NAME);			/*file is empty or can't be open*/
		}
		else {
			IC = minAdress;																			/*Lines counter*/
			DC = 0;																					/*Data counter*/	
			LC = 0;																					/*In file line counter, for errors*/
			assembler = compiled;
			while (fgets(line, LINE_LENGTH, READ_fp) != NULL) {										/*beginning of the forst reading*/
				char *LABEL, *DIRECTIVE, *OPCODE_FUNCT, *FIRST_OP, *SECOND_OP, *TEMP, *end_of_str;	/*pointers to the relevant part in the line*/	
				LC += 1;
				TEMP = strtok(line, "	");															/*reads the next word from the line*/ 
				for( i  = 0 ; i <= sizeof(line) ; i++) {											/*finds the lines first char*/
					if((line[i] != '\t') && (line[i] != ' ')) {
						break;
					}
				}
				if((line[i] != ';') && strcmp(line,"\n")) {											/*if line is blank or start with ';'*/
					if(TEMP[strlen(TEMP) - 1] == ':') {												/*checks for LABEL*/
						if((TEMP[strlen(TEMP) - 2] != ' ') && (TEMP[strlen(TEMP) - 2] != '\t')) {
							LABEL = TEMP;
							TEMP = strtok(NULL, "	");
							LABEL[strlen(LABEL) - 1] = '\0';
						}
						else {
							printf("ERROR in line %d, %s is no avalide LABEL.\n", LC, TEMP);
							assembler = error;
						}
					}	
					else {
						LABEL = NULL;
				}
				if(TEMP[0] == '.') {																/*check for DIRECTIVE*/
					DIRECTIVE = TEMP;
					if((strncmp(DIRECTIVE, ".extern", 7) == 0) || (strncmp(DIRECTIVE, ".entry", 6) == 0)) {
						TEMP = strtok(line," ");
						DIRECTIVE = TEMP;
						TEMP = strtok(NULL,"\n");
						FIRST_OP = TEMP; 
						ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, FIRST_OP,0 ,LC, &assembler);
					}
					else if(strcmp(DIRECTIVE, ".string") == 0) {
						if(LABEL != NULL) {
							ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, LABEL,IC ,LC, &assembler);
						}
						TEMP = (TEMP[strlen(TEMP) + 1] == '\t') ? strtok(NULL, "	") : strtok(NULL, " ");
						FIRST_OP = TEMP;
						for(j = 0 ; j <= sizeof(FIRST_OP) ; ++j) {
							if(FIRST_OP[j] != '"') {
								if(LABEL != NULL) {
									ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE, &FIRST_OP[j], &IC, &DC, (int)FIRST_OP[j], 'A', LC, &assembler);
									LABEL = NULL;
								}
								else{
									ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE, &FIRST_OP[j], &IC, &DC, (int)FIRST_OP[j], 'A', LC, &assembler);
								}
							}
						}
						end_of_str = '\0';
						ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE, end_of_str, &IC, &DC, 0, 'A', LC, &assembler);						
					}
					else if(strcmp(DIRECTIVE, ".data") == 0) {
						if(LABEL != NULL) {
							ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, LABEL,IC ,LC, &assembler);
						}
						TEMP = strtok(NULL, ",");
						while(TEMP != NULL) {
							FIRST_OP = TEMP;
							first_op_val = OPERAND_VALUE(FIRST_OP, DIRECTIVE, LC, &assembler);
							ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE, FIRST_OP, &IC, &DC, first_op_val, 'A', LC, &assembler);
							TEMP = strtok(NULL, ",");
						}
					}		
				}	/*end of TEMP[0] == '.'*/
    	            
				else {
					DIRECTIVE = NULL;
					if(LABEL != NULL) {
						ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, LABEL,IC ,LC, &assembler);
					}
					OPCODE_FUNCT = TEMP;
					for(j = 0 ; j < NUM_OF_FUNC ; ++j) {
						if(strcmp(OPCODE_FUNCT, FUNC_NAMES[j]) == 0) {
							break;
						}	
					}
						
					switch (j) {
						case 0:
						case 1:
						case 2:
						case 3:
						case 4:
							TEMP = strtok(NULL, ",");
							FIRST_OP = TEMP;
							TEMP = strtok(NULL, "\n");
							if(TEMP == NULL) {
								printf("ERROR in line %d, not enough operands for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							if(TEMP[0] == ' ') {
								TEMP++;
							}
							SECOND_OP =TEMP;
							opcode_funct = FUNC_PRES[j];
							first_op = ADDRESSING_METHODE(FIRST_OP, 1);
							if(((j == 4) && (first_op != 4)) || (first_op == 8)) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							second_op = ADDRESSING_METHODE(SECOND_OP, 2);
							if(((j == 1) && (second_op == 2)) || ((j != 1) && ((second_op == 0) || (second_op == 2)))) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							machine_code = (opcode_funct | (first_op | second_op));
							ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE,OPCODE_FUNCT , &IC, &DC, machine_code, 'A', LC, &assembler);
							first_op_val = OPERAND_VALUE(FIRST_OP, DIRECTIVE, LC, &assembler);
							ADD_TO_OBJ(&OBJ_HEAD, NULL, DIRECTIVE, FIRST_OP, &IC, &DC, first_op_val, CHECK_ARE(FIRST_OP, LC, &assembler), LC, &assembler);
							second_op_val = OPERAND_VALUE(SECOND_OP, DIRECTIVE, LC, &assembler);
							ADD_TO_OBJ(&OBJ_HEAD, NULL, DIRECTIVE, SECOND_OP, &IC, &DC, second_op_val, CHECK_ARE(SECOND_OP,LC,&assembler), LC, &assembler);
							TEMP = strtok(NULL, " ");
							if(TEMP != NULL) {
								printf("ERROR in line %d, to meny operands for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							break;
	
						case 5:
						case 6:
						case 7:
						case 8:
						case 9:
						case 10:
						case 11:
						case 12:
						case 13:
							TEMP = strtok(NULL, "\n");
							if(TEMP == NULL) {
								printf("ERROR in line %d, not enough operands for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							FIRST_OP = TEMP;
							opcode_funct = FUNC_PRES[j];
							first_op = ADDRESSING_METHODE(FIRST_OP, 2);
							if((j != 13) && (first_op == 0)){
								printf("ERROR in line %d, wrong addresing method for for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							if(((j >= 9) && (j <= 11)) && (first_op == 3)) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							if(!((j >= 9) && (j <= 11)) && (first_op == 2)) {
								printf("ERROR in line %d, wrong addresing method for for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							machine_code = (opcode_funct | first_op);
							ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE,OPCODE_FUNCT , &IC, &DC, machine_code, 'A', LC, &assembler);
							first_op_val = OPERAND_VALUE(FIRST_OP, DIRECTIVE, LC, &assembler);
							ADD_TO_OBJ(&OBJ_HEAD, NULL, DIRECTIVE,FIRST_OP , &IC, &DC, first_op_val, CHECK_ARE(FIRST_OP, LC, &assembler), LC, &assembler);
							TEMP = strtok(NULL, " ");
							if(TEMP != NULL) {
								printf("ERROR in line %d, to meny operands for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							break;

						case 14:
						case 15:
							TEMP = strtok(NULL, " ");
							if(TEMP != NULL) {
								printf("ERROR in line %d, no need in operands for %s.\n", LC, OPCODE_FUNCT);
								assembler = error;
							}
							OPCODE_FUNCT[sizeof(OPCODE_FUNCT)] = '\0';
							opcode_funct = FUNC_PRES[j];
							ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE, OPCODE_FUNCT, &IC, &DC, opcode_funct, 'A', LC, &assembler);
							break;
	
						default:
							printf("ERROR in line %d, %s is no a function.\n", LC, OPCODE_FUNCT);
							assembler = error;
					}	/*end of switch*/
				}
			}	/*End of if(line[0] != ';')*/
				line = (char*) malloc(LINE_LENGTH + 1);
		}	/*end of the first reading*/
			
		free(line);	
		OBJ_curr = OBJ_HEAD;
		SYMBOL_curr = SYMBOL_HEAD;
		
		
		/*beginning of second reading*/	
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
			OBJ_curr = OBJ_HEAD;
			SYMBOL_curr = SYMBOL_curr->NEXT;
		}	/*End of srcond read*/
		SYMBOL_curr = SYMBOL_HEAD;
		
		
		/*beginning of writing OBJ*/
		if(assembler == 0) {																		/*in case of an error ot want write new files*/
			int entry_status = 0, extrn_status = 0;
			char* FILE_NAME_ob = malloc(sizeof(*argv)+4);											
			if(FILE_NAME_ob == NULL)
				exit(1);
			strcpy(FILE_NAME_ob, *argv);
			strcat(FILE_NAME_ob, ".ob");
			OBJ_fp = fopen(FILE_NAME_ob, "w");														/*creating OBJ file*/
			fprintf(OBJ_fp, "\t%d %d\n", (IC-DC-100), DC);
			while(OBJ_curr != NULL) {
				fprintf(OBJ_fp, "%04d %03X %c\n", OBJ_curr->IC, OBJ_curr->MACHINE_CODE, OBJ_curr->ARE_LABEL);
				OBJ_curr=OBJ_curr->NEXT;
			}
			fclose(OBJ_fp);
			free(FILE_NAME_ob);
			OBJ_curr = OBJ_HEAD;
			
			
			while(SYMBOL_curr != NULL) {				
				if(strcmp(SYMBOL_curr->ATTRIBUTES, "external") == 0) {								/*checking the SYMBOL list for external cases*/
					extrn_status = 1;
					break;
				}
				else {
					SYMBOL_curr = SYMBOL_curr->NEXT;
				}
			}
			SYMBOL_curr = SYMBOL_HEAD;
			
			while(SYMBOL_curr != NULL) {
				if(strcmp(SYMBOL_curr->ATTRIBUTES, "entry") == 0) {									/*checking the SYMBOL list for entry cases*/
					entry_status = 1;
					break;
				}
				else {
					SYMBOL_curr = SYMBOL_curr->NEXT;
				}
			}
			SYMBOL_curr = SYMBOL_HEAD;
			
			
			/*beginning of writing external*/
			if(extrn_status == 1) {
				char* FILE_NAME_ext = malloc(sizeof(*argv)+5);
				if(*argv == NULL)
					exit(1);
				strcpy(FILE_NAME_ext, *argv);
				strcat(FILE_NAME_ext, ".ext");
				EXT_fp = fopen(FILE_NAME_ext, "w");
				while(SYMBOL_curr != NULL) {
					if(strcmp(SYMBOL_curr->ATTRIBUTES, "external") == 0){
						while(OBJ_curr != NULL) {
							if((strcmp(OBJ_curr->NAME, SYMBOL_curr->NAME)== 0)&& (OBJ_curr->ARE_LABEL == 'E')){
								fprintf(EXT_fp, "%s	%04d\n", OBJ_curr->NAME, OBJ_curr->IC);
							}
							OBJ_curr = OBJ_curr->NEXT;
						}
						OBJ_curr = OBJ_HEAD;
					}
					SYMBOL_curr = SYMBOL_curr->NEXT;
				}
				SYMBOL_curr = SYMBOL_HEAD;
				fclose(EXT_fp);
				free(FILE_NAME_ext);
			}
			
			
			/*veginning of writing entry*/
			if(entry_status == 1) {
				char* FILE_NAME_ent = malloc(sizeof(*argv)+5);
					if(FILE_NAME_ent == NULL)
					exit(1);
				strcpy(FILE_NAME_ent, *argv);
				strcat(FILE_NAME_ent, ".ent");
				ENT_fp = fopen(FILE_NAME_ent, "w");
				while(SYMBOL_curr != NULL) {
					if(strcmp(SYMBOL_curr->ATTRIBUTES, "entry") == 0) {
						while(OBJ_curr != NULL) {
							if((strcmp(OBJ_curr->NAME, SYMBOL_curr->NAME) == 0) && (OBJ_curr->ARE_LABEL == 'A')) {
								fprintf(EXT_fp, "%s	%04d\n", OBJ_curr->NAME, OBJ_curr->IC);
							}
							OBJ_curr = OBJ_curr->NEXT;
						}
						OBJ_curr = OBJ_HEAD;
					}
					SYMBOL_curr = SYMBOL_curr->NEXT;
				}
				SYMBOL_curr = SYMBOL_HEAD;
				fclose(ENT_fp);
				free(FILE_NAME_ent);
			}
		}	/*end of writing the new file*/
		else {
			printf("There was a problemme compiling %s, please check for errors\n", FILE_NAME);		/*error when no file was wirten*/
		}
		DELETE_LISTS(&OBJ_HEAD, &SYMBOL_HEAD);
		fclose(READ_fp);																			/*freeing reading file*/
			exit(1);
		}	/*end of else*/
	}	/*end of while(--argc > 0)*/
	
	return 0;
}	/*END OF MAIN*/



















