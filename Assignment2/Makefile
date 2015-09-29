CC 	= gcc
CFLAGS 	= -g -Wall 
JC = javac

all: serv1 client

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.class client serv1 core* *~

