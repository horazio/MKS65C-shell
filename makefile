all : inputProc.o shell.o 
	gcc -o shell inputProc.o shell.o 

inputProc.o: inputProc.c inputProc.h swag.h
	gcc -c inputProc.c

shell.o : shell.c inputProc.h swag.h
	gcc -c shell.c

run:
	./shell
