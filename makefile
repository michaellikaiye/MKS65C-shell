all: shell.o pipe.o redirect.o parseargs.o io.o execcom.o
	gcc -O3 -g shell.o pipe.o redirect.o parseargs.o execcom.o io.o -o main

debug: all
	gdb -tui main

pipe.o: pipe.c pipe.h
	gcc -O3 -g -c pipe.c

redirect.o: redirect.c redirect.h
	gcc -O3 -g -c redirect.c

io.o: io.c io.h
	gcc -O3 -g -c io.c

shell.o: shell.c 
	gcc -O3 -g -c shell.c

parseargs.o: parseargs.c parseargs.h
	gcc -O3 -g -c parseargs.c

execcom.o: execcom.c execcom.h
	gcc -O3 -g -c execcom.c

clean:
	rm *.o main test *.log *.h.gch *.lines

run: all
	./main
