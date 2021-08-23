/*
This header contain all the rellevant function for the writing process.
*/
#ifndef WRITING_H
#define WRITING_H

#include "main_header.h"

int WRITING_OB(FILE** OBJ_fp, OBJ_LIST** OBJ_HEAD, char* name, int IC, int DC);
void CHECK_EXT_ENT(SYMBOL_LIST** SYMBOL_HEAD, int* extrn_status, int* entry_status);
int WRITING_EXT(FILE** EXT_fp, OBJ_LIST** OBJ_HEAD, SYMBOL_LIST** SYMBOL_HEAD, char* name);
int WRITING_ENT(FILE** ENT_fp, OBJ_LIST** OBJ_HEAD, SYMBOL_LIST** SYMBOL_HEAD, char* name);

#endif	
