CC 	= gcc
CCGI = -I ../ccgi
CFLAGS 	= -g -Wall $(CCGI)
build: paperserver paperclient

papers: papers.o rpcfunc_clnt.o rpcfunc_xdr.o 
	$(CC) -o papers.cgi papers.o rpcfunc_clnt.o rpcfunc_xdr.o -lnsl -L ../ccgi -lccgi

paperclient: paperclient.o rpcfunc_clnt.o rpcfunc_xdr.o 
	$(CC) -o paperclient paperclient.o rpcfunc_clnt.o rpcfunc_xdr.o -lnsl  

paperserver: paperserver.o rpcfunc_svc.o rpcfunc_xdr.o
	$(CC) -o paperserver paperserver.o rpcfunc_svc.o rpcfunc_xdr.o -lnsl 

rpcfunc_clnt.c rpcfunc.h rpcfunc_xdr.c rpcfunc_svc.c: rpcfunc.x
	rpcgen rpcfunc.x

papers.o: papers.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o papers.o papers.c

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

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.cgi core* *~ paperserver paperclient
