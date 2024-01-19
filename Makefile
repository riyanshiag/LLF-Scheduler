all: sched.c
	gcc sched.c

run:
	./a.out

clean:
	rm -f *.o *.out