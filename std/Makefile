all:
	as fopen.S -o fopen.o
	as malloc.S -o malloc.o
	as exit.S -o exit.o
	ar rcs ./libstd.a *.o
	rm -v *.o
clean:
	rm libstd.a
