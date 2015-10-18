#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#include "rpcfunc.h"

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
		if(out->paper_info == NULL)
		{
			break;
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
		for(i = 0; i < out->paper_data_len; i++)
		{
			printf("%c", out->paper_data_val[i]);
		}
		printf("\n");
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

	in->paper_data_val = file->buffer;
	in->paper_data_len = file->length;

	//printf("Article: %s - %s, %d\n", in->author, in->title, (int)strlen(in->paper));

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
			{
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
		return output;
	}
	printf("WTF happened? %d args provided", argc);
	return -1;
}
