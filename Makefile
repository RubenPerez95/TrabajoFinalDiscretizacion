all: discretizo 

discretizo: Discretizo.c
	gcc -o Discretizo -fopenmp Discretizo.c

clean:
	rm -f Discretizo *.o

