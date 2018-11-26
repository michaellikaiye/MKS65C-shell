all: shell.o parseargs.o io.o execcom.o
	gcc -g shell.o parseargs.o execcom.o io.o -o main

debug: all
	gdb -tui main

test: test.c io.o
	gcc -g io.o test.c -o test

rtest: test
	./test

io.o: io.c io.h
	gcc -g -c io.c

shell.o: shell.c shell.h
	gcc -g -c shell.c

parseargs.o: parseargs.c parseargs.h
	gcc -g -c parseargs.c

execcom.o: execcom.c execcom.h
	gcc -g -c execcom.c

clean:
	rm *.o main test *.log

run: all
	./main
