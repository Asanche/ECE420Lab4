all: clean main datatrim serialtester

clean:
	rm -rf main serialtester datatrim

main: 
	mpicc -g -Wall -std=c99 main.c Lab4_IO.c -o main -lm

serialtester: 
	gcc -g -Wall -std=c99 serialtester.c Lab4_IO.c -o serialtester -lm

datatrim: 
	gcc -g -Wall datatrim.c  -o datatrim -lm

make zip:
	zip 1395330.zip datatrim.c hostlist Lab4_IO.c Lab4_IO.g main.c Makefile members README.md serialtester.c timer.h web-stanford.txt
