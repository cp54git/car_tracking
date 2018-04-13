find.e:	findcar.o tools.o
	gcc findcar.o tools.o -o find.e
findcar.o:	findcar.c tr2field.h
	gcc findcar.c -c -Wall
tools.o:	tools.c tr2field.h
	gcc tools.c -c -Wall
cls:
	rm findcar.o tools.o find.e