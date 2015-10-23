CC 	= gcc
CFLAGS 	= -g -Wall 

build: paperserver paperclient

paperclient: paperclient.o paperserver_clnt.o paperserver_xdr.o 
	$(CC) -o paperclient paperclient.o paperserver_clnt.o paperserver_xdr.o -lnsl  

paperserver: paperserver.o paperserver_svc.o paperserver_xdr.o
	$(CC) -o paperserver paperserver.o paperserver_svc.o paperserver_xdr.o -lnsl 

paperserver_clnt.c paperserver.h paperserver_xdr.c paperserver_svc.c: paperserver.x
	rpcgen paperserver.x

paperclient.o: paperclient.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperclient.o paperclient.c

paperserver_clnt.o: paperserver_clnt.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_clnt.o paperserver_clnt.c

paperserver_xdr.o: paperserver_xdr.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_xdr.o paperserver_xdr.c

paperserver_svc.o: paperserver_svc.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_svc.o paperserver_svc.c

paperserver.o: paperserver.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver.o paperserver.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o core* *~ paperserver paperclient