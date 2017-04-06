all: clean main datatrim serialtester

clean:
	rm -rf main serialtester datatrim

main: 
	mpicc -g -Wall -std=c99 src/main.c devkit/Lab4_IO.c -o main -lm

serialtester: 
	gcc -g -Wall -std=c99 devkit/serialtester.c devkit/Lab4_IO.c -o serialtester -lm

datatrim: 
	gcc -g -Wall devkit/datatrim.c  -o datatrim -lm
