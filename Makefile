
all: cputemp

cputemp: main.o cputemp.o
	g++ $^ -o $@


clean:
	rm -f cputemp
	rm -f *.o

