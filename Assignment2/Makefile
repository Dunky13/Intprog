CC 	= gcc
CFLAGS 	= -g -Wall 
JC = javac

build: serv1 serv2 serv3 client talk

talk: talk.o
	$(CC) -o talk talk.o -lpthread -lncurses

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.class client serv1 serv2 serv3 talk core* *~
