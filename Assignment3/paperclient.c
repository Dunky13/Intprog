#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#include "rpcfunc.h"

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
	printf("Hmm: %s\n", argv);
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

char* readFile(char* file_path)
{
	char* buffer = 0;
	long length;
	FILE* f = fopen (file_path, "rb");

	if (f)
	{
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = malloc (length);
		if (buffer)
		{
			fread (buffer, 1, length, f);
		}
		fclose (f);
	}
	return buffer;
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
	paper_out *out;

	in = (int_in) article_id;

	out = info_paper_1(&in, cl);

	if (out==NULL)
	{
		printf("Error: %s\n",clnt_sperror(cl,"Get Article Information Error"));
		return 1;
	}
	printf("%s\t%s\n", out->author, out->title);
	return 0;
}

int getArticle(CLIENT *cl, int article_id)
{
	int_in in;
	paper_content_out *out;

	in = (int_in) article_id;

	out = fetch_paper_1(&in, cl);

	if (out==NULL)
	{
		printf("Error: %s\n",clnt_sperror(cl,"Get Article Error"));
		return 1;
	}
	printf("%s\n", *out);
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
	add_paper_in 	in;
	int_out 		*out;

	in.author 		= author;
	in.title 		= title;
	in.paper 		= readFile(file_path);

	out = add_paper_1(&in, cl);

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
			if(argc != 6 || access(argv[optind + 2 ], R_OK) == -1)
			{
				output = printUsage();
			}
			else
			{
				output = addArticle(cl, argv[optind], argv[optind + 1], argv[optind + 2]);
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
				printf("Hmm2: %s\n", optarg);
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
