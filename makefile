all: shell.o
	gcc shell.o

shell.o: shell.c shell.h
	gcc -c shell.c

clean:
	rm *.o
	rm a.out

run:
	./a.out
