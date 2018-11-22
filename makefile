all: shell.o parseargs.o
	gcc -g shell.o parseargs.o -o main

test:
	gcc -g test.c -o test

rtest:
	./test

shell.o: shell.c shell.h
	gcc -g -c shell.c

parseargs.o: parseargs.c parseargs.h
	gcc -g -c parseargs.c

clean:
	rm *.o main test

run: all
	./main
