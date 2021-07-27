/*
This header collects all the strctures we use
*/

#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum status {					/*The assembler status*/
	compiled,
	error
} status;

typedef struct OBJ_LIST {
	char NAME[12];					/*saves the string*/
	int IC;							/*The number of the line acording to the IC*/
	int MACHINE_CODE;				/*A 12 bit binary representation*/
	char ARE_LABEL;					/*The "A,R,E" signeture*/
	struct OBJ_LIST* NEXT;			/*Pointer to the next link on the list*/
} OBJ_LIST;							/*Structure with al the rellevant data for a line*/

typedef struct SYMBOL_LIST {
	char NAME[12];
	int IC;
	char ATTRIBUTES[9];				/*12 chars for case of external*/
	char COD_DAT[5];				/*4 chars for case of code or data*/
	struct SYMBOL_LIST* NEXT;
} SYMBOL_LIST;

#endif

