all: clean main datatrim serialtester

clean:
	rm -rf main.exe serialtester.exe datatrim.exe

main: 
	mpicc -g -Wall -std=c99 -lm src/main.c devkit/Lab4_IO.c -o main.exe

serialtester: 
	gcc -g -Wall -std=c99 -lm devkit/serialtester.c devkit/Lab4_IO.c -o serialtester.exe

datatrim: 
	gcc -g -Wall -lm devkit/datatrim.c  -o datatrim.exe
