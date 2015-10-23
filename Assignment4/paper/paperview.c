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
	int firstChar = file[0];
	struct fileType *out = (struct fileType*) malloc(sizeof(struct fileType));
	
	int PDF[4] 	= {0x25, 0x50, 0x44, 0x46};
	int DOCX[8] = {0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x06, 0x00};
	int JAR[7] 	= {0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x08};
	int ZIP[7] 	= {0x50, 0x4B, 0x03, 0x04, 0x0A, 0x00, 0x00};
	int DOC1[8] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
	int DOC2[8] = {0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1, 0x00};
	int DOC3[4] = {0x0D, 0x44, 0x4F, 0x43};
	int RTF[6] 	= {0x7B, 0x5C, 0x72, 0x74, 0x66, 0x31};
	int JPG[3] 	= {0xFF, 0xD8, 0xFF};
	
	if(firstChar == 0x25)
	{
		if(checkFile(file, PDF)){
			out->contentType = "application/pdf";
			out->contentDisposition = ".pdf";
			return out;
		}
	}
	else if(firstChar == 0x50)
	{
		if(checkFile(file, DOCX)){
			out->contentType = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
			out->contentDisposition = ".docx";
			return out;
		}
		if(checkFile(file, JAR)){
			out->contentType = "application/java-archive";
			out->contentDisposition = ".jar";
			return out;
		}
		if(checkFile(file, ZIP)){
			out->contentType = "application/zip";
			out->contentDisposition = ".zip";
			return out;
		}
	}
	else if(firstChar == 0x0D || firstChar == 0xD0 || firstChar == 0xCF)
	{
		if(checkFile(file, DOC1) ||
			checkFile(file, DOC2) ||
			checkFile(file, DOC3)){
			out->contentType = "application/msword";
			out->contentDisposition = ".doc";
			return out;
		}
	}
	else if(firstChar == 0x7B)
	{
		if(checkFile(file, RTF)){
			out->contentType = "text/richtext";
			out->contentDisposition = ".rtf";
			return out;
		}
	}
	else if(firstChar == 0xFF)
	{
		if(checkFile(file, JGP)){
			out->contentType = "image/jpeg";
			out->contentDisposition = ".jpg";
			return out;
		}
	}
	else
	{
		out->contentType = "text/plain";
		out->contentDisposition = "";
		return out;
	}
}
int getArticle(CLIENT *cl, int article_id)
{
	int_in in;
	paper_data *out;
	int i;
	struct fileType* contentInfo;
	in = (int_in) article_id;

	out = fetch_paper_1(&in, cl);

	if (out==NULL)
	{
		printReason("Get Article Information Error");
		return 1;
	}
	contentInfo = getContentType(out->paper_data_val);
	printf("Content-type: %s\r\n\r\n", contentInfo->contentType);
	printf("Content-disposition: paper%02d%s\r\n\r\n", article_id, contentInfo->contentDisposition);
	if(out->paper_data_len > 0)
	{
		for(i = 0; i < out->paper_data_len; i++)
		{
			printf("%c", out->paper_data_val[i]);
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	CLIENT *cl;
	const char* input;
    CGI_varlist *varlist;
    int value;
	const char *name = "id";
	
    if ((varlist = CGI_get_all(0)) == NULL || 
		varlist == 0 || 
		(input = CGI_lookup(varlist, name)) == NULL)
	{
        printReason("No CGI data received");
        return 0;
    }
	
	value = parseInt(input);
    CGI_free_varlist(varlist);	
	if(value < 0)
	{
		printReason("Not correct number received");
		return 0;
	}
	value = 0;
	cl = createClient();
	value = getArticle(cl, value);
	clnt_destroy(cl);
	
	return value;
	
}
