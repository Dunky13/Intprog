#include <stdio.h>
#include <stdlib.h>

#include "rpcfunc.h"

struct papers
{
	struct papers *next;
	struct papers *prev;
	int id;

	char* author;
	char* title;
	char* paper;
};
struct papers* head;
struct papers* tail;
typedef enum { false, true } bool;
//  [0,........,x]
// [tail,.....,head]

void freePreviousInfoOut(struct paper_out* out)
{
	if(out == NULL) //Nothing to free if it is NULL
	{
		return;
	}
	free(out->author);
	free(out->title);
	free(out);
}
void freePreviousListOut(struct paper_list_out* out)
{
	struct paper_list_out* tmp;
	while(out != NULL)
	{

		freePreviousInfoOut(out->paper_info);
		tmp = out->next;
		free(out);
		out = tmp;
	}
}

void remove_paper(struct papers* curr)
{
	if(curr->next != NULL && curr->prev != NULL)
	{
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
	}
	else if(curr->next != NULL && curr->prev == NULL)
	{
		curr->next->prev = NULL;
	}
	else if(curr->next == NULL && curr->prev != NULL){
		curr->prev->next = NULL;
	}
	free(curr->author);
	free(curr->title);
	free(curr->paper);
	free(curr);
}
bool isPaper(struct papers* curr, int id)
{
	return curr->id == id;
}

struct papers* closer(int id, struct papers* a, struct papers* b)
{
	int toA = a == NULL ? INT_MAX : abs(a->id - id);
	int toB = b == NULL ? INT_MAX : abs(b->id - id);
	return toA < toB ? a : b;
}

bool hasPapers(){
	return head != NULL || tail != NULL;
}

int_out *remove_paper_1_svc(int_in *in, struct svc_req *req)
{
	static int_out out = 1;
	int id = (int) *in;
	struct papers* curr;
	struct papers* tmp;

	bool forward;

	if(!hasPapers())
	{
		out = -1;
		return &out;
	}

	curr = closer(id, head, tail); //Not necessarily best option:
	// [0,1,2,30,100] <= 30 is closer to 100 in this case. But worst O is O(n)

	forward = curr == head ? false : true;

	while(!isPaper(curr, id)){
		curr = forward ? curr->next : curr->prev;
		if(curr == NULL)
		{
			return &out;
		}
	}

	if(curr == head)
	{
		if(head->next != NULL) //SHOULDN'T Happen
		{
			tmp = head->next;
			tmp->prev = head->prev;
		}
		else if(head->prev != NULL)
		{
			tmp = head->prev;
			tmp->next = NULL;
		}
		remove_paper(head);
		head = tmp;
	}
	else if(curr == tail)
	{
		if(tail->prev != NULL) //SHOULDN'T Happen
		{
			tmp = tail->prev;
			tmp->next = tail->next;
		}
		else if(tail->next != NULL)
		{
			tmp = tail->next;
			tmp->prev = NULL;
		}
		remove_paper(tail);
		tail = tmp;
	}
	else
	{
		remove_paper(curr);
	}

	return &out;
}

paper_content_out *fetch_paper_1_svc(int_in *in, struct svc_req *req)
{
	static paper_content_out* out;

	int id = (int) *in;
	struct papers* curr;
	bool forward;

	if(out != NULL)
	{
		free(out);
	}

	if(!hasPapers())
	{
		out = (paper_content_out*) malloc(sizeof(char) * (1));
		return out;
	}

	curr = closer(id, head, tail); //Not necessarily best option:
	// [0,1,2,30,100] <= 30 is closer to 100 in this case. But worst O is O(n)
	forward = curr == head ? false : true;

	while(!isPaper(curr, id)){
		curr = forward ? curr->next : curr->prev;
		if(curr == NULL)
		{
			return out;
		}
	}

	out = (paper_content_out*) curr->paper;

	return out;
}


paper_out *info_paper_1_svc(int_in *in, struct svc_req *req)
{
	static paper_out* out;

	int id = (int) *in;
	struct papers* curr;
	bool forward;

	freePreviousInfoOut(out);

	out = malloc(sizeof(struct paper_out));
	if(out == NULL)
	{
		perror("Error allocating memory");
		exit(1);
	}

	if(!hasPapers())
	{
		return out;
	}
	curr = closer(id, head, tail); //Not necessarily best option:
	// [0,1,2,30,100] <= 30 is closer to 100 in this case. But worst O is O(n)
	forward = curr == head ? false : true;

	while(!isPaper(curr, id)){
		curr = forward ? curr->next : curr->prev;
		if(curr == NULL)
		{
			return out;
		}
	}

	out->author = strdup(curr->author);
	out->title 	= strdup(curr->title);

	return out;
}

paper_list_out *list_paper_1_svc(list_in *in, struct svc_req *req)
{
	static paper_list_out* out;
	struct paper_list_out* curr_out;
	struct papers* curr;

	curr = tail;

	freePreviousListOut(out);

	out = malloc(sizeof(struct paper_list_out));
	if(out == NULL)
	{
		perror("Error allocating memory");
		exit(1);
	}

	curr_out = out;

	while(curr != NULL){

		curr_out->id = curr->id;

		curr_out->paper_info	= malloc(sizeof(struct paper_out));
		curr_out->next 			= malloc(sizeof(struct paper_list_out));
		if(curr_out->next == NULL || curr_out->paper_info == NULL)
		{
			perror("Error allocating memory");
			exit(1);
		}

		curr_out->paper_info->author 	= strdup(curr->author);
		curr_out->paper_info->title 	= strdup(curr->title);

		curr_out 	= curr_out->next;
		curr 		= curr->next;
	}

	return out;
}

int_out *add_paper_1_svc(add_paper_in *in, struct svc_req *req)
{
	static int_out out;

	struct papers* newHead;

	newHead = malloc(sizeof(struct papers));
	newHead->id 	= head->id + 1;
	newHead->author = strdup(in->author);
	newHead->title	= strdup(in->title);
	newHead->paper	= in->paper;

	if(head != NULL){
		newHead->prev = head;
		head->next = newHead;
	}

	head = newHead;
	if(tail == NULL)
	{
		tail = head;
	}
	out = (int_out) newHead->id;
	return &out;
}
