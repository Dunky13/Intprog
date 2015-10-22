#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rpcfunc.h"
#include "ccgi.h"

#define HOST "localhost"

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

struct fileType* getContentType(char* file){
	int firstChar = file[0] < 0 ? 256 + file[0]: file[0];
	struct fileType *out = (struct fileType*) malloc(sizeof(struct fileType));
	
	int PDF[4] 	= {0x25, 0x50, 0x44, 0x46};
	int DOCX[8] = {0x50, 0x4B, 0x03, 0x04, 0x14, 0x00};
	//int JAR[7] 	= {0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x08};
	int ZIP[7] 	= {0x50, 0x4B, 0x03, 0x04, 0x0A, 0x00, 0x00};
	int DOC1[8] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
	int DOC2[8] = {0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1, 0x00};
	int DOC3[4] = {0x0D, 0x44, 0x4F, 0x43};
	int RTF[6] 	= {0x7B, 0x5C, 0x72, 0x74, 0x66, 0x31};
	int JPG[3] 	= {0xFF, 0xD8, 0xFF};
	int PNG[8] 	= {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
	
	int found = 0;
	
	if(firstChar == 0x25)
	{
		if(checkFile(file, PDF)){
			out->contentType = "application/pdf";
			out->contentDisposition = ".pdf";
			found = 1;
		}
	}
	else if(firstChar == 0x50)
	{
		if(checkFile(file, DOCX)){
			out->contentType = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
			out->contentDisposition = ".docx";
			found = 1;
		}
		/*else if(checkFile(file, JAR)){
			out->contentType = "application/java-archive";
			out->contentDisposition = ".jar";
			found = 1;
		}*/
		else if(checkFile(file, ZIP)){
			out->contentType = "application/zip";
			out->contentDisposition = ".zip";
			found = 1;
		}
	}
	else if(firstChar == 0x0D || firstChar == 0xD0 || firstChar == 0xCF)
	{
		if(checkFile(file, DOC1) ||
			checkFile(file, DOC2) ||
			checkFile(file, DOC3)){
			out->contentType = "application/msword";
			out->contentDisposition = ".doc";
			found = 1;
		}
	}
	else if(firstChar == 0x7B)
	{
		if(checkFile(file, RTF)){
			out->contentType = "text/richtext";
			out->contentDisposition = ".rtf";
			found = 1;
		}
	}
	else if(firstChar == 0x89)
	{
		if(checkFile(file, PNG)){
			out->contentType = "image/png";
			out->contentDisposition = ".png";
			found = 1;
		}
	}
	else if(firstChar == 0xFF)
	{
		if(checkFile(file, JPG)){
			out->contentType = "image/jpeg";
			out->contentDisposition = ".jpg";
			found = 1;
		}
	}
	if(!found){
		out->contentType = "text/plain";
		out->contentDisposition = ".txt";
	}
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
		printf("Error: %s\n",clnt_sperror(cl,"Add Article Error"));
		return 1;
	}
	printf("%d\n", *out);
	return 0;
}

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
