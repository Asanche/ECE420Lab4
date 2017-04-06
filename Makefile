all: clean main datatrim serialtester

clean:
	rm -rf main serialtester datatrim

main: 
	mpicc -g -Wall -std=c99 src/main.c Lab4_IO.c -o main -lm

serialtester: 
	gcc -g -Wall -std=c99 serialtester.c Lab4_IO.c -o serialtester -lm

datatrim: 
	gcc -g -Wall datatrim.c  -o datatrim -lm
