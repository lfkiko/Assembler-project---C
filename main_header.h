/**
This is the main header, it contain all the relevant headers for the hall programe end all the defines.
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#ifndef MAIN_HWADER_H
#define MAIN_HWADER_H

/*typdefs and strctures*/

#include "structures.h"
#include "read.h"
#include "writing.h"

/*Defines fot the progaram*/
#define fileName 24
#define minAdress 100			/*The "adress" of the first line*/
#define LINE_LENGTH 81			/*The buffer size for reading a line*/
#define mov_b 0					/*a numer presentation to 0000 0000 0000*/
#define cmp_b 256				/*a numer presentation to 0001 0000 0000*/
#define add_b 672				/*a numer presentation to 0010 1010 0000*/
#define sub_b 688				/*a numer presentation to 0010 1011 0000*/
#define lea_b 1024				/*a numer presentation to 0100 0000 0000*/
#define clr_b 1440				/*a numer presentation to 0101 1010 0000*/
#define not_b 1456				/*a numer presentation to 0101 1011 0000*/
#define inc_b 1472				/*a numer presentation to 0101 1100 0000*/
#define dec_b 1488				/*a numer presentation to 0101 1101 0000*/
#define jmp_b 2464				/*a numer presentation to 1001 1010 0000*/
#define bne_b 2480				/*a numer presentation to 1001 1011 0000*/
#define jsr_b 2496				/*a numer presentation to 1001 1100 0000*/
#define red_b 3072				/*a numer presentation to 1100 0000 0000*/
#define prn_b 3328				/*a numer presentation to 1101 0000 0000*/
#define rts_b 3584				/*a numer presentation to 1110 0000 0000*/
#define stop_b 3840				/*a numer presentation to 1111 0000 0000*/
#define NUM_OF_FUNC 16			/*The number of function the assembler no to do*/
#define COMP 4095				/*Tow's complenetary for 12 bits*/

/*functions calls*/
#include "functions.h"

#endif
