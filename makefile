snake: snake.o main.o
	cc snake.o main.o -lncurses -o project
snake.o: snake.c snake.h
	cc -c snake.c -lncurses
main.o: main.c snake.h
	cc -c main.c -lncurses
clean:
	rm *.o
