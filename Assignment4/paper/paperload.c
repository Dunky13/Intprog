#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rpcfunc.h"
#include "ccgi.h"

#define HOST "localhost"
#define SUCCESS "../paperinfo.php"
#define ERROR "../papererror.php"
struct fileParams{
	char* buffer;
	long length;
};
void redirectError(char* reason){
	printf("Location: %s?reason=\"%s\"\r\n\r\n", ERROR, reason);
}

void redirectSuccess(int id){
	printf("Location: %s?id=%d&new=1\r\n\r\n", SUCCESS, id);
}

CLIENT* createClient(){
	CLIENT *cl;
	cl = clnt_create(HOST, RPC_FUNCTIONS, RPC_FUNC_VERS, "tcp");
	if(cl == NULL)
	{
		redirectError("Error creating RPC Client");
		exit(1);
	}
	return cl;
}
struct fileParams *readFile(char* file_path)
{
	char* buffer;
	long length;
	struct fileParams *out = (struct fileParams*) malloc(sizeof(struct fileParams));
	FILE* f = fopen (file_path, "rb");

	if (f)
	{
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = (char *)malloc (length+1);
		if (buffer)
		{
			fread (buffer, length + 1, 1, f);
		}
		fclose (f);
	}
	out->length = length;
	out->buffer = buffer;
	return out;
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
		redirectError("Add Article Error");
		return -1;
	}
	return (int) out;
}

int main(int argc, char **argv) {

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
        redirectError("No CGI post data received");
        return 0;
    }
	
	if(((authorVal = CGI_lookup(varlist, author)) == NULL) ||
		((titleVal = CGI_lookup(varlist, title)) == NULL) ||
		((fileVal = CGI_lookup(varlist, file)) == NULL))
		{
			redirectError("No CGI post data received");
			return 0;
		}
	
    CGI_free_varlist(varlist);	

	if(value < 0)
	{
		redirectError("Not correct number received");
		return 0;
	}
	cl = createClient();
	value = getArticle(cl, value);
	clnt_destroy(cl);
	if(value < 0){
		redirectError("Add Article Error");
	}
	redirectSuccess(value);
	return value;
}
