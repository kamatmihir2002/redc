.PHONY: std bootstrap
all : bootstrap std
	echo "Made all"

std :
	make -C std
	
bootstrap : 
	make -C bootstrap

clean : 
	make -C std clean
	make -C bootstrap clean


