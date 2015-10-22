JC = javac
JFLAGS = -classpath .:java-getopt-1.0.14.jar

build: HotelInterface.class HotelClient.class HotelImpl.class HotelServer.class HotelRoomType.class HotelGateway.class HotelGatewayThread.class

%.class: %.java
	$(JC) $(JFLAGS) $<

clean:
	rm -f *.class *~