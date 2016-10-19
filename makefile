all: try
try: project.o
	gcc project.o -o try -lncurses
project.o: project.c
	gcc project.c 
clean:
	rm *.o
