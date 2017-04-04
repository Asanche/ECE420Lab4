all: main datatrim serialtester

main: 
	gcc -g -Wall -std=c99 src/main.c devkit/Lab4_IO.c -o main.exe

serialtester: 
	gcc -g -Wall -std=c99 devkit/serialtester.c devkit/Lab4_IO.c -o serialtester.exe

datatrim: 
	gcc -g -Wall -std=c99 devkit/datatrim.c  -o datatrim.exe