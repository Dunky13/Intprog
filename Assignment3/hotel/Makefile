JC = javac
JFLAGS = -classpath .:java-getopt-1.0.14.jar

build: Hotel.class HotelClient.class HotelImpl.class HotelServer.class HotelRoomType.class HotelGateway.class

%.class: %.java
	$(JC) $(JFLAGS) $<

clean:
	rm -f *.class *~