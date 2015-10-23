CC 	= gcc
CCGI = -I ../ccgi
CFLAGS 	= -g -Wall $(CCGI)
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

all: build papers paperinfo paperview

build: paperserver paperclient

papers: papers.o rpcfunc_clnt.o rpcfunc_xdr.o 
	$(CC) -o papers.cgi papers.o rpcfunc_clnt.o rpcfunc_xdr.o -lnsl -L ../ccgi -lccgi
	ln -s $(ROOT_DIR)/papers.cgi ../web/cgi-bin/papers.cgi

paperinfo: paperinfo.o rpcfunc_clnt.o rpcfunc_xdr.o 
	$(CC) -o paperinfo.cgi paperinfo.o rpcfunc_clnt.o rpcfunc_xdr.o -lnsl -L ../ccgi -lccgi
	ln -s $(ROOT_DIR)/paperinfo.cgi ../web/cgi-bin/paperinfo.cgi	

paperview: paperview.o rpcfunc_clnt.o rpcfunc_xdr.o 
	$(CC) -o paperview.cgi paperview.o rpcfunc_clnt.o rpcfunc_xdr.o -lnsl -L ../ccgi -lccgi
	ln -s $(ROOT_DIR)/paperview.cgi ../web/cgi-bin/paperview.cgi	
	
paperclient: paperclient.o rpcfunc_clnt.o rpcfunc_xdr.o 
	$(CC) -o paperclient paperclient.o rpcfunc_clnt.o rpcfunc_xdr.o -lnsl  

paperserver: paperserver.o rpcfunc_svc.o rpcfunc_xdr.o
	$(CC) -o paperserver paperserver.o rpcfunc_svc.o rpcfunc_xdr.o -lnsl 

rpcfunc_clnt.c rpcfunc.h rpcfunc_xdr.c rpcfunc_svc.c: rpcfunc.x
	rpcgen rpcfunc.x

papers.o: papers.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o papers.o papers.c
	
paperinfo.o: paperinfo.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o paperinfo.o paperinfo.c

paperview.o: paperview.c rpcfunc.h
	$(CC) $(CFLAGS) -c -o paperview.o paperview.c

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
	rm -f *.o *.cgi core* *~ paperserver paperclient ../web/cgi-bin/*.cgi
