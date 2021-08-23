CC = gcc
CFLAGS = -g -Wall -ansi -pedantic

assembler: myAssembler.o lists.o side_functions.o first_read_functions.o second_read_functions.o writing_functions.o
	$(CC) $(CFLAGS) myAssembler.o lists.o side_functions.o first_read_functions.o second_read_functions.o writing_functions.o -o assembler

myAssembler.o: myAssembler.c main_header.h
	$(CC) $(CFLAGS) -c myAssembler.c -o myAssembler.o

lists.o: lists.c main_header.h
	$(CC) $(CFLAGS) -c lists.c -o lists.o
	
side_functions.o: side_functions.c main_header.h
	$(CC) $(CFLAGS) -c side_functions.c -o side_functions.o
	
first_read_functions.o:  first_read_functions.c main_header.h
	$(CC) $(CFLAGS) -c first_read_functions.c -o first_read_functions.o
	
second_read_functions.o: second_read_functions.c main_header.h
	$(CC) $(CFLAGS) -c second_read_functions.c -o second_read_functions.o
	
writing_functions.o: writing_functions.c main_header.h
	$(CC) $(CFLAGS) -c writing_functions.c -o writing_functions.o
 
clean:
	rm -f *.o assembler
