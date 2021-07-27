
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "main_header.h"
#include "structures.h"

void ADD_TO_OBJ(OBJ_LIST** HEAD, char* LABLE, char* DIRECTIVE, char* OPCODE_OP, int* IC, int* DC, int MACHINE_CODE, char ARE_VAL, int LC, status* assembler);
void ADD_TO_SYMBOL(SYMBOL_LIST** HEAD, char* DIRECTIVE, char* NAME, int IC, int LC, status* assembler);
void DELETE_LISTS(OBJ_LIST** ObHead, SYMBOL_LIST** SyHead);
int ADDRESSING_METHODE(char *operand, int wich);
int OPERAND_VALUE(char *operand, char *DIRECTIVE, int LC, status* assemble);
char CHECK_ARE(char *operand, int LC, status* assemble);

#endif
