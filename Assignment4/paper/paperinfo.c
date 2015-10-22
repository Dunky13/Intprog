#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rpcfunc.h"
#include "ccgi.h"

#define HOST "localhost"

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
		printf("{\"reason\": \"Error creating RPC Client\"}");
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
		printf("Error: %s\n",clnt_sperror(cl,"Get Article Information Error"));
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
	return 0;
}

int main(int argc, char **argv) {
	CLIENT *cl;
	const char* input;
    fputs("Content-type: application/json\r\n\r\n", stdout);
	
    CGI_varlist *varlist;
    int value;

    if ((varlist = CGI_get_all(0)) == 0) {
        printf("{\"reason\": \"No CGI data received\"}");
        return 0;
    }

	input = CGI_lookup(varlist, "id");
	value = parseInt(input);
    CGI_free_varlist(varlist);  //free variable list
	
	if(value < 0){
		printf("{\"reason\": \"Not correct number received - %s\"}", input);
		return 0;
	}
	
	cl = createClient();
	value = getArticleInformation(cl, value);
	clnt_destroy(cl);
	return value;
	
}
