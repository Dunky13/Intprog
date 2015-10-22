#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rpcfunc.h"
#include "ccgi.h"

#define HOST "localhost"

CLIENT* createClient(){
	CLIENT *cl;
	cl = clnt_create(HOST, RPC_FUNCTIONS, RPC_FUNC_VERS, "tcp");
	if(cl == NULL)
	{
		perror("Error creating RPC Client");
		exit(1);
	}
	return cl;
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
	printf("[\n");
	do
	{
		if(out->id < 0 || out->paper_info == NULL ||
			(strlen(out->paper_info->author) == 0 && strlen(out->paper_info->title) == 0)
			)
		{
			continue;
		}
		printf("\t{");
		printf("\t\t\"id:\" %d,\n", out->id);
		printf("\t\t\"title:\" %s,\n", out->paper_info->title);
		printf("\t\t\"author:\" %s\n", out->paper_info->author);
		printf("\t}%s", out->next == NULL ? "\n" : ",\n");
	} while((out = out->next) != NULL);
	printf("]");
	return 0;
}


int main(int argc, char **argv) {
	CLIENT *cl;
	
	/*
    CGI_varlist *varlist;
    const char *name;
    CGI_value  *value;
    int i;

    fputs("Content-type: text/plain\r\n\r\n", stdout);
    if ((varlist = CGI_get_all(0)) == 0) {
        printf("No CGI data received\r\n");
        return 0;
    }

    //output all values of all variables and cookies

    for (name = CGI_first_name(varlist); name != 0;
        name = CGI_next_name(varlist))
    {
        value = CGI_lookup_all(varlist, 0);

        // CGI_lookup_all(varlist, name) could also be used

        for (i = 0; value[i] != 0; i++) {
            printf("%s [%d] = %s\r\n", name, i, value[i]);
        }
    }
    CGI_free_varlist(varlist);  //free variable list
	*/
	
	cl = createClient();
	return getAllArticles(cl);
	
}
