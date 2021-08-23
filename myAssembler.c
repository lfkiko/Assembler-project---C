/*
This is the code for the main program wich reads the files and write new files if needed.
*/

#include "main_header.h"			/*The all inclusive header*/

char *FUNC_NAMES[NUM_OF_FUNC] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop\n"};

int main(int argc, char* argv[]) {

	FILE *READ_fp, *OBJ_fp, *ENT_fp, *EXT_fp;														/*Pointers to input files and out put files*/
	char *line, *FILE_NAME;
	status assembler;																	/*Pointers to readed line's buffer*/
	int i , j, first_op_val = -1 , second_op_val = -1, extrn_status, entry_status;
	int IC, DC, LC;	
	OBJ_LIST *OBJ_HEAD = NULL;
	SYMBOL_LIST *SYMBOL_HEAD = NULL;																		
	
	if (argc == 1) {																	/*checking for files*/
		printf("ERROR!, No avileble files \n");														/*No files were added*/
		return 0;
	}/*end of argc == 1 if*/
	
	line = (char*) malloc(LINE_LENGTH + 1);															/*buffer for the line*/
	if(line == NULL) {																	/*check if there's place in the memory*/
		exit(1);
	}

	while(--argc > 0) {																	/*main loop break when thre are no more files*/
		
		FILE_NAME = (char*) malloc(sizeof(*++argv) + 4);
		strcpy(FILE_NAME, *++argv);
		strcat(FILE_NAME, ".as");
		FILE_NAME[sizeof(*argv)+4] = '\0';
		READ_fp = fopen(FILE_NAME, "r");
		
		if(READ_fp == NULL)	{
			printf("ERROR!, the file %s cannot be open or has no content\n", FILE_NAME);			/*file is empty or can't be open*/
		}
		
		else {
		
			IC = minAdress;																/*Lines counter*/
			DC = 0;																	/*Data counter*/	
			LC = 0;																	/*In file line counter, for errors*/
			assembler = compiled;
			
			while (fgets(line, LINE_LENGTH, READ_fp) != NULL) {											/*beginning of the forst reading*/
				char *LABEL, *DIRECTIVE, *OPCODE_FUNCT, *FIRST_OP, *SECOND_OP, *TEMP, *end_of_str;	/*pointers to the relevant part in the line*/	
				LC += 1;															/*line counter*/
				TEMP = strtok(line, "	");													/*reads the next word from the line*/ 
				for( i  = 0 ; i <= sizeof(line) ; i++) {											/*finds the lines first char*/
					if((line[i] != '\t') && (line[i] != ' ')) {
						break;
					}
				}
				
				if((line[i] != ';') && strcmp(line,"\n")) {											/*if line is blank or start with ';'*/
					
					LABEL_CHECK(&TEMP, &LABEL, &assembler , LC);										/*checl for LABEL*/
					
					switch ( DIRECTIVE_CHECK(&TEMP, &DIRECTIVE, &LABEL) ) {
						
						case 0:														/*DIRECTIVE = .extern or DIRECTIVE = .entry*/
							TEMP = strtok(line," ");
							DIRECTIVE = TEMP;
							TEMP = strtok(NULL,"\n");
							FIRST_OP = TEMP;
							ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, FIRST_OP,0 ,LC, &assembler);
							break;
							
						case 1:														/*DIRECTIVE = .string and LABEL = NULL*/
							ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, LABEL,IC ,LC, &assembler);
							
						case 2:														/*DIRECTIVE = .string and LABEL != NULL*/
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
							break;
							
						case 3:														/*DIRECTIVE = .data and LABEL = NULL*/
							ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, LABEL,IC ,LC, &assembler);
							
						case 4:														/*DIRECTIVE = .data and LABEL != NULL*/
							TEMP = strtok(NULL, ",");
							while(TEMP != NULL) {
								FIRST_OP = TEMP;
								first_op_val = OPERAND_VALUE(FIRST_OP, DIRECTIVE, LC, &assembler);
								ADD_TO_OBJ(&OBJ_HEAD, LABEL, DIRECTIVE, FIRST_OP, &IC, &DC, first_op_val, 'A', LC, &assembler);
								TEMP = strtok(NULL, ",");	
							}
							break;
							
						case 5:														/*DIRECTIVE = NULL and LABEL = NULL*/
							ADD_TO_SYMBOL(&SYMBOL_HEAD, DIRECTIVE, LABEL,IC ,LC, &assembler);
							
						case 6:														/*DIRECTIVE = NULL and LABEL != NULL*/
							OPCODE_FUNCT = TEMP;
							for(j = 0 ; j < NUM_OF_FUNC ; ++j) {
								if(strcmp(OPCODE_FUNCT, FUNC_NAMES[j]) == 0) {
									break;
								}	
							}
							FUNCTION_CHECK(j, &TEMP, &LABEL, &DIRECTIVE, &FIRST_OP, &SECOND_OP, &OPCODE_FUNCT,
							&OBJ_HEAD, &assembler, &LC, &IC, &DC, first_op_val, second_op_val);	
							break;
							
						default:
							printf("ERROR in line %d, %s is not a directive.\n", LC, DIRECTIVE);
							assembler = error;
					}
					
			}																	/*End of if(line[0] != ';')*/
				line = (char*) malloc(LINE_LENGTH + 1);
		}																		/*end of the first reading*/
			
		free(line);	
			
		SECOND_READ(&SYMBOL_HEAD, &OBJ_HEAD);														/*Second reading*/
		
		if(assembler == 0) {																/*in case of an error ot want write new files*/
			
			int obj_file = WRITING_OB(&OBJ_fp, &OBJ_HEAD, *argv, IC, DC);										/*Writing OBJ*/
			if(obj_file == 1) {
				exit(1);
			}
			
			CHECK_EXT_ENT(&SYMBOL_HEAD, &extrn_status, &entry_status);										/*Cheking for external and entry cases in SYMBOL list*/
			
			if(extrn_status == 1) {															/*Checks if there are any externals*/
				int ext_file = WRITING_EXT(&EXT_fp, &OBJ_HEAD, &SYMBOL_HEAD, *argv);								/*writing external*/
				if(ext_file == 1) {
					exit(1);
				}
			}
			
			if(entry_status == 1) {															/*Checks if there are any entrys*/
				int ent_file = WRITING_ENT(&ENT_fp, &OBJ_HEAD, &SYMBOL_HEAD, *argv);								/*writing entrys*/
				if(ent_file == 1) {
					exit(1);
				}
			}
		}																		/*end of writing the new file*/
		
		else {
			printf("There was a problemme compiling %s, please check for errors\n", FILE_NAME);							/*error when no file was wirten*/
		}
		
		DELETE_LISTS(&OBJ_HEAD, &SYMBOL_HEAD);
		fclose(READ_fp);																/*freeing reading file*/
			exit(1);
		}																		/*end of else (line 41)*/
	}																			/*end of while(--argc > 0) (line 29)*/
	return 0;	
}	/*END OF MAIN*/



















