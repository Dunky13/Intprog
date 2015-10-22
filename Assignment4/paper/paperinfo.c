#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rpcfunc.h"
#include "ccgi.h"

#define HOST "localhost"

void printReason(char* reason){
	printf("{\"reason\": \"%s\"}", reason);
}
int parseInt(const char* argv)
{
	int id;
	char* end;
	id = strtol(argv, &end, 10);
	if(!*end)
	{
		return id;
	}
	else
	{
		return -1;
	}
}

CLIENT* createClient(){
	CLIENT *cl;
	cl = clnt_create(HOST, RPC_FUNCTIONS, RPC_FUNC_VERS, "tcp");
	if(cl == NULL)
	{
		printReason("Error creating RPC Client");
		exit(1);
	}
	return cl;
}
int getArticleInformation(CLIENT *cl, int article_id)
{
	int_in in;
	paper_information *out;

	in = (int_in) article_id;

	out = info_paper_1(&in, cl);

	if (out==NULL)
	{
		printReason("Get Article Information Error");
		return 1;
	}
	if(!(strlen(out->author) == 0 && strlen(out->title) == 0))
	{
		printf("{");
		printf("\t\"id:\" %d,\n", article_id);
		printf("\t\"title:\" %s,\n", out->title);
		printf("\t\"author:\" %s\n", out->author);
		printf("}");
	}
	else
	{
		printReason("No Article Found");
	}
	return 0;
}

int main(int argc, char **argv) {
	CLIENT *cl;
	const char* input;
    CGI_varlist *varlist;
    int value;
	
	fputs("Content-type: application/json\r\n\r\n", stdout);
/*
    if ((varlist = CGI_get_all(0)) == NULL || varlist == 0) {
        printReason("No CGI data received");
        return 0;
    }

	input = CGI_lookup(varlist, "id");
	value = parseInt(input);
    	
	if(value < 0){
		printReason("Not correct number received");
		return 0;
	}
	*/
	value = 0;
	cl = createClient();
	value = getArticleInformation(cl, value);
	clnt_destroy(cl);
	CGI_free_varlist(varlist);
	return value;
	
}
