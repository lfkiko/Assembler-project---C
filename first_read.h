
#ifndef FIRST_READ_H
#define FIRST_READ_H

#include "main_header.h"
#include "structures.h"

void LABEL_CHECK(char** TEMP, char** LABEL, status* assembler , int LC);
int DIRECTIVE_CHECK(char** TEMP, char** DIRECTIVE, char** LABEL);
void FUNCTION_CHECK(int j, char** TEMP, char** LABEL, char** DIRECTIVE, char** FIRST_OP, char** SECOND_OP, char** OPCODE_FUNCT, OBJ_LIST** OBJ_HEAD,
status* assembler, int* LC, int* IC, int* DC, int first_op_val, int );


#endif
