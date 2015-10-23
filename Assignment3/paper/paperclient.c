#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#include "paperserver.h"

struct fileParams{
	char* buffer;
	long length;
};

CLIENT* createClient(char* host){
	CLIENT *cl;
	cl = clnt_create(host, RPC_FUNCTIONS, RPC_FUNC_VERS, "tcp");
	if(cl == NULL)
	{
		perror("Error creating RPC Client");
		exit(1);
	}
	return cl;
}

//Parse the integer and catch too long numbers or faulty numbers
int parseInt(char* argv)
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

//Read the file from the path
struct fileParams *readFile(char* file_path)
{
	char* buffer;
	long length;
	struct fileParams *out = (struct fileParams*) malloc(sizeof(struct fileParams));
	FILE* f = fopen (file_path, "rb");
	//if the file exists, fill the buffer and length
	if (f)
	{
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = (char *)malloc (length+1);
		if (buffer)
		{
			fread (buffer, length + 1, 1, f); //Read to the buffer of file length + \0 character
		}
		fclose (f);
	}
	//Save it to the struct, struct is used instead of opaque for pointer reasons
	out->length = length;
	out->buffer = buffer;
	return out;
}
int getAllArticles(CLIENT *cl)
{
	list_in in = 0;
	struct paper_list_out *out;

	out = list_paper_1(&in, cl);

	if (out == NULL)
	{
		printf("Error: %s\n",clnt_sperror(cl,"Get All Articles Error"));
		return 1;
	}
	do
	{
		if(out->id < 0 || out->paper_info == NULL ||
			(strlen(out->paper_info->author) == 0 && strlen(out->paper_info->title) == 0)
			) //Don't print if the paper doesn't exist, or the author AND title are empty
		{
			continue;
		}
		printf("%d\t%s\t%s\n", out->id, (out->paper_info)->author, (out->paper_info)->title);
	} while((out = out->next) != NULL);
	return 0;
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
	//Don't print if author AND title are empty
	if(!(strlen(out->author) == 0 && strlen(out->title) == 0))
	{
		printf("%s\t%s\n", out->author, out->title);
	}
	return 0;
}

int getArticle(CLIENT *cl, int article_id)
{
	int_in in;
	paper_data *out;
	int i;

	in = (int_in) article_id;

	out = fetch_paper_1(&in, cl);

	if (out==NULL)
	{
		printf("Error: %s\n",clnt_sperror(cl,"Get Article Error"));
		return 1;
	}
	if(out->paper_data_len > 0)
	{
		//Cannot print as string since it would stop at the \0 character, which for binary files must be printed
		for(i = 0; i < out->paper_data_len; i++)
		{
			printf("%c", out->paper_data_val[i]);
		}
	}
	return 0;
}

int removeArticle(CLIENT *cl, int article_id)
{
	int_in in;
	int_out *out;

	in = (int_in) article_id;

	out = remove_paper_1(&in, cl);

	if (out==NULL)
	{
		printf("Error: %s\n",clnt_sperror(cl,"Remove Article Error"));
		return 1;
	}
	return 0;
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

int printUsage()
{
	printf("Usage:\n");
	printf("\tHelp: paperclient -h\n");											//2 args
	printf("\tList articles: paperclient <host> -l\n");							//3 args
	printf("\tArticle Information: paperclient <host> -i <ID>\n");				//4 args
	printf("\tFetch article: paperclient <host> -f <ID>\n");					//4 args
	printf("\tRemove article: paperclient <host> -r <ID>\n");					//4 args
	printf("\tAdd Paper: paperclient <host> -a <Author> <Title> <File Path>\n");//5 args
	return 1;
}

int main(int argc, char** argv)
{
	CLIENT *cl;
	int id;

	int c;
	int output;


	if(argc == 1){
		return printUsage();
	}
	//else if(argc == 2) -> should only be -h, and either -h or any other command will show printUsage
	else if(argc > 2)
	{			
		cl = createClient(argv[1]);
	}
	
	if(cl == NULL) //If the code is executed but the client could not be created and returned a NULL, print the usage
	{
		return printUsage();
	}
	while((c = getopt(argc, argv, "a:f:i:r:hl")) != -1)
	{
		switch (c) {
			case 'a':
			if(argc != 6)
			{
				output = printUsage();
			}
			else if(access(argv[optind + 1 ], R_OK) == -1){
				output = printUsage();
				printf("Could not access file %d - %s - %s %s\n", optind, optarg, argv[optind], argv[optind + 1]);
			}
			else
			{ //get the three arguments, if there are not exactly 6 arguments the usage is printed
				output = addArticle(cl, optarg, argv[optind], argv[optind + 1]);
			}
			break;
			case 'f':
			if(argc != 4)
			{
				output = printUsage();
			}
			else
			{
				id = parseInt(optarg);
				if(id < 0)
				{
					output = 1;
				}
				else
				{
					output = getArticle(cl, id);
				}
			}
			break;
			case 'i':
			if(argc != 4)
			{
				output = printUsage();
			}
			else
			{
				id = parseInt(optarg);
				if(id < 0)
				{
					output = 1;
				}
				else
				{
					output = getArticleInformation(cl, id);
				}
			}
			break;
			case 'l':
			output = getAllArticles(cl);
			break;
			case 'r':
			if(argc != 4)
			{
				output = printUsage();
			}
			else
			{
				id = parseInt(optarg);
				if(id < 0)
				{
					output = 1;
				}
				else
				{
					output = removeArticle(cl, id);
				}
			}
			break;
			case 'h':
			default:
			output = printUsage();
			break;
		}
		clnt_destroy(cl);
		return output;
	}
	//This should not happen, since there is a default case defined. But just in case the client needs to be destroyed
	clnt_destroy(cl);
	printf("WTF happened? %d args provided", argc);
	return -1;
}
