default: prog

hashMap.o: hashMap.c hashMap.h
	gcc -Wall -ansi -pedantic-errors -c hashMap.c

prog: hashMap.o main.c
	gcc -Wall -ansi -pedantic-errors -o prog hashMap.o  main.c

clean:
	rm hashMap.o

cleanall: clean
	rm prog
