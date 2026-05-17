all:
	cpp syscalls.S > syscalls.S.pp
	as fileio.S -o fileio.o
	as mman.S -o mman.o
	as exit.S -o exit.o
	ar rcs ./libstd.a *.o
	rm *.o *.pp
clean:
	rm libstd.a
