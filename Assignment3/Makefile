CC 	= gcc
CFLAGS 	= -g -Wall 
JC = javac
JFLAGS = -classpath .:java-getopt-1.0.14.jar

paperclient: paperclient.o rpcfunc_clnt.o rpcfunc_xdr.o 
	$(CC) -o paperclient paperclient.o rpcfunc_clnt.o rpcfunc_xdr.o -lnsl  

paperserver: paperserver.o rpcfunc_svc.o rpcfunc_xdr.o
	$(CC) -o paperserver paperserver.o rpcfunc_svc.o rpcfunc_xdr.o -lnsl 

rpcfunc_clnt.c rpcfunc.h rpcfunc_xdr.c rpcfunc_svc.c: rpcfunc.x
	rpcgen rpcfunc.x

paperclient.o: paperclient.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o paperclient.o paperclient.c

rpcfunc_clnt.o: rpcfunc_clnt.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o rpcfunc_clnt.o rpcfunc_clnt.c

rpcfunc_xdr.o: rpcfunc_xdr.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o rpcfunc_xdr.o rpcfunc_xdr.c

rpcfunc_svc.o: rpcfunc_svc.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o rpcfunc_svc.o rpcfunc_svc.c

paperserver.o: paperserver.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o paperserver.o paperserver.c

build: hotel-reservation

hotel-reservation: Hotel.class HotelClient.class HotelImpl.class HotelServer.class HotelRoomType.class HotelGateway.class

%.class: %.java
	$(JC) $(JFLAGS) $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.class core* *~

