all: clean main datatrim serialtester

clean:
	rm -rf main serialtester datatrim

main: 
	mpicc -g -Wall -std=c99 -lm src/main.c devkit/Lab4_IO.c -o main

serialtester: 
	gcc -g -Wall -std=c99 -lm devkit/serialtester.c devkit/Lab4_IO.c -o serialtester

datatrim: 
	gcc -g -Wall -lm devkit/datatrim.c  -o datatrim
