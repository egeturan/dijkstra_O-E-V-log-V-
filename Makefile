all: A B 

A: mainA.c
	gcc -o A mainA.c

B: mainB.c
	gcc -o B mainB.c
clean:
	rm A B
