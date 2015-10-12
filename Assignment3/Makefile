CC 	= gcc
CFLAGS 	= -g -Wall 
JC = javac
JFLAGS = -classpath .:java-getopt-1.0.14.jar

build: hotel-reservation

hotel-reservation: Hotel.class HotelClient.class HotelImpl.class HotelServer.class HotelRoomType.class HotelGateway.class

%.class: %.java
	$(JC) $(JFLAGS) $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.class core* *~

