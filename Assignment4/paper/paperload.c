#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rpcfunc.h"
#include "ccgi.h"

#define HOST "localhost"
/*
struct fileType{
	char* contentType;
	char* contentDisposition;
};

void printReason(char* reason){
	printf("Content-type: application/json\r\n\r\n");
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

int checkFile(char* file, int TEST[])
{
	int i;
	int buffVal;
	for(i = 0; i < sizeof(TEST)/sizeof(TEST[0]); i++)
	{
		if(TEST[i] == 0x100)
			continue;
		buffVal = file[i] < 0 ? 256 + file[i]: file[i];
		if(buffVal != TEST[i])
		{
			return 0;
		}
	}
	return 1;
	
}

int addArticle(CLIENT *cl, char* author, char* title, char* file_path)
{
	paper_information 	*in;
	int_out 	*out;
	struct fileParams *file;

	in 			= (struct paper_information*) malloc(sizeof(struct paper_information));
	in->author 	= (author);
	in->title 	= (title);

	file		= readFile(file_path);
	//Set all the values properly
	in->paper.paper_data_val = file->buffer;
	in->paper.paper_data_len = file->length;

	out = add_paper_1(in, cl);

	if (out==NULL)
	{
		printf("Error: %s\n",clnt_sperror(cl,"Add Article Error"));
		return 1;
	}
	printf("%d\n", *out);
	return 0;
}
*/

int main(int argc, char **argv) {
	printf("Location: ../");
	/*
	CLIENT *cl;
	const char* authorVal;
	const char* titleVal;
	const char* fileVal;
    CGI_varlist *varlist;
    int value;
	const char *author = "author";
	const char *title = "title";
	const char *file = "file";
	
    if ((varlist = CGI_get_all(0)) == NULL || 
		varlist == 0 || 
		(varlist = CGI_get_post(varlist)) == NULL || 
		varlist == 0
	{
        printReason("No CGI post data received");
        return 0;
    }
	
	if(((authorVal = CGI_lookup(varlist, author)) == NULL) ||
		((titleVal = CGI_lookup(varlist, title)) == NULL) ||
		((fileVal = CGI_lookup(varlist, file)) == NULL))
		{
			printReason("No CGI post data received");
			return 0;
		}
	
    CGI_free_varlist(varlist);	

	if(value < 0)
	{
		printReason("Not correct number received");
		return 0;
	}
	cl = createClient();
	value = getArticle(cl, value);
	clnt_destroy(cl);
	
	return value;
	*/
}
