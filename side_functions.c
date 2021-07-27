/*
This functions return relevant values for the OBJ and SYMBOL lists. 
*/

#include "main_header.h"			/*The all inclusive header*/

/*
This function gets an operand and send back its ADDRESSING METHODE.
*/
int ADDRESSING_METHODE(char *operand, int wich) {
	int method;
	if(operand[0] == '#') {
		method = 0;
	}
	else if(operand[0] == '%') {
		method = 2;
	}
	else if(operand[0] == 'r') {
		method = 3;
	}
	else {
		method = 1;
	}
	if( wich == 1) {
		method = method << 2;
	}
	return method;
} /*End of ADDRESSING_METHODE*/


/*
This function gets an operand and send its value.
*/
int OPERAND_VALUE(char *operand, char *DIRECTIVE, int LC, status* assembler) {
	char *start = operand + 1;
	char *end_of_num;
	long int value;
	long int rValue = 1;
	int ArrayLength =  (sizeof(operand) - 1);
	if(DIRECTIVE != NULL) {
		if(strcmp(DIRECTIVE, ".string") == 0) {
			value = (long int)*operand;
			return (int)value;
		}
		else if(strcmp(DIRECTIVE, ".data") == 0) {
			value = strtol(operand, &end_of_num, 10);
			if((start != end_of_num) || (end_of_num == operand + ArrayLength) || ((value>=0) && (value<10)) ) {
				if(value < 0) {
					value = (((value * (-1))^COMP) + 1);
				}
				return value;
			}
			else {
				printf("ERROR in line %d, %s is not a number.\n", LC, operand);
				*assembler = error;
				return 0;
			}
		}
	}
	else if(operand[0] == 'r') {
		value = strtol(start, &end_of_num, 10);
		if((start != end_of_num) || (end_of_num == operand + ArrayLength)) {
			if(value <= 7) {
				while(value-- > 0) {
					rValue *= 2;
				}
			return (int)rValue;
		}
	}
		else {
			printf("ERROR in line %d, %s can't be found.\n", LC, operand);
			*assembler = error;
			return 0;	
		}	
	}
	else if(operand[0] == '#') {
		value = strtol(start, &end_of_num, 10);
		if((start != end_of_num) && (end_of_num == operand + ArrayLength)) {
			if(value < 0) {
				value = (((value * (-1))^COMP) + 1);
			}
			return (int)value;
		}
		else {
			printf("ERROR in line %d, %s is not a number.\n", LC, operand);
			*assembler = error;
			return 0;
		}
	}
	else {
		value = 0;
	}
	return (int) value;
}	/*End of OPERAND_VALUE*/


/*
This function checks for the relevant A.R.E value
*/
char CHECK_ARE(char *operand, int LC, status* assembler) {
	char *start = operand + 1;
	char *end_of_num;
	long int value;
	int ArrayLength =  (sizeof(operand) - 1);
	if(operand[0] == 'r') {
		value = strtol(start, &end_of_num, 10);
		if((start != end_of_num) || (end_of_num == operand + ArrayLength)) {
			if(value <= 7) {
				return 'A';
		}
	}
		else {
			printf("ERROR in line %d, %s can't be found.\n", LC, operand);
			*assembler = error;
			return 'F';
		}	
	}
	else if(operand[0] == '#') {
		value = strtol(start, &end_of_num, 10);
		if((start != end_of_num) && (end_of_num == operand + ArrayLength)) {
			return 'A';
		}
		else {
			printf("ERROR in line %d, %s is not a number.\n", LC, operand);
			*assembler = error;
			return 'F';
		}
	}
	else {
		value = strtol(operand, &end_of_num, 10);
		if((operand == end_of_num) && (end_of_num != operand + ArrayLength)) {
			return 'N';
		}
		else {
			return 'F';
		}
	}
	return 'F';
}	/*End of CHECK_ARE*/










