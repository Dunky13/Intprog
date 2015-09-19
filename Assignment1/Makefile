CC 	= gcc
CFLAGS 	= -g -Wall 
JC = javac

build: mysh1 mysh2 mysh3 syn1 syn2 synthread1 synthread2 Syn1.class Syn2.class

synthread1: synthread1.o
	$(CC) -o synthread1 synthread1.o -lpthread

synthread2: synthread2.o
	$(CC) -o synthread2 synthread2.o -lpthread

Syn1.class: syn1.java
	$(JC) syn1.java

Syn2.class: syn2.java
	$(JC) syn2.java

%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *.o *.class mysh1 mysh2 mysh3 syn1 syn2 synthread1 synthread2 Syn1 Syn2 core* *~

