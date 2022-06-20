main: main.o 
	gcc main.c -pthread -o main
main.o: main.c
	gcc -c main.c -pthread
clean:
	rm*.o main
	 
