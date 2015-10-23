#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "paperserver.h"

struct paper_list_out* head;
struct paper_list_out* tail;
//  [0,........,x]
// [tail,.....,head]
typedef enum { false, true } bool;

/***
* Free the whole list after out, if out is not the tail, previous entries are not freed
* Used only once where out is always the tail
*/
void freePreviousListOut(struct paper_list_out* out)
{
	struct paper_list_out* tmp;
	while(out != NULL)
	{
		free(out->paper_info);
		out->paper_info = NULL;
		tmp = out->next;
		free(out);
		out = tmp;
	}
}

bool isPaper(struct paper_list_out* curr, int id)
{
	return curr->id == id;
}

/***
* Find the paper closest to the ID, this can improve the search a bit, but worst case is O(n)
* Best case it is a part of O(n)
*/
struct paper_list_out* closer(int id, struct paper_list_out* a, struct paper_list_out* b)
{
	int toA = a == NULL ? INT_MAX : abs(a->id - id);
	int toB = b == NULL ? INT_MAX : abs(b->id - id);
	return toA < toB ? a : b;
}

bool hasPapers(){
	return head != NULL || tail != NULL;
}

/***
* Find the paper in the list
* First look whether the head or tail is proximately closer.
* There are cases where it will not work, but it is a small improvement for a large server
* Small use cases don't use this improvement
*/
struct paper_list_out* findPaper(int id){
	struct paper_list_out* curr;
	bool forward;

	if(!hasPapers())
	{
		return NULL;
	}
	curr = closer(id, head, tail); //Not necessarily best option:
	// [0,1,2,30,100] <= 30 is closer to 100 in this case. But worst O is O(n)
	forward = curr == head ? false : true;

	while(!isPaper(curr, id)){
		curr = forward ? curr->next : curr->prev;
		if(curr == NULL)
		{
			return NULL;
		}
	}
	return curr;
}

/***
* Look whether or not the paper is already in the list, the string compare function is case sensitive
* Paper {Auth: "Spyros", Title: "Assignment 3"} is not the same as Paper {Auth: "spyros", Title: "assignment 3"}
* It starts looking through from the tail, there is no best case here since the paper may or may not exist.
*/

struct paper_list_out* paperExists(struct paper_information *in){
	struct paper_list_out* curr;
	curr = tail;

	if(curr == NULL || curr->paper_info == NULL ||
		curr->paper_info->title == NULL || curr->paper_info->title == NULL)
	{
		return NULL;
	}
	while(!(strcmp(curr->paper_info->author, in->author) == 0 &&
		strcmp(curr->paper_info->title, in->title) == 0))
	{
		if(curr->next == NULL)
		{
			return NULL;
		}
		curr = curr->next;
	}
	return curr;
}


int_out *remove_paper_1_svc(int_in *in, struct svc_req *req)
{
	static int_out out = 1;
	int id = (int) *in;
	struct paper_list_out* curr;
	struct paper_list_out* tmp;

	curr = findPaper(id);

	if(curr == NULL)
	{
		return &out;
	}

	else if(curr == head && curr == tail) //If there is only one paper in the server
	{
		head 		= NULL;
		tail 		= NULL;
	}
	else if(curr == head) //If the selected paper is in the head, move the head back one position
						  //There will be one position at least since the previous else if catches 
						  //one paper
	{
		head 		= curr->prev;
		head->next 	= NULL;
	}
	else if(curr == tail) //Same as head but at the back of the list
	{
		tail 		= curr->next;
		tail->prev 	= NULL;
	}
	else //If it is neither the head, or the tail it is somewhere in between, and the usecase of 
		 //1 and 2 papers is caught in the previous else ifs
	{
		tmp = curr;
		curr->next->prev = tmp->prev;
		curr->prev->next = tmp->next;
	}
	free(curr->paper_info->author);
	free(curr->paper_info->title);
	free(curr->paper_info);
	free(curr);
	return &out;
}

/***
* Retrieve the paper, if it exists otherwise return an empty object which the client will handle.
*/
paper_data *fetch_paper_1_svc(int_in *in, struct svc_req *req)
{
	static paper_data* out;

	int id = (int) *in;
	struct paper_list_out* curr;

	if(out != NULL)
	{
		free(out);
	}

	out = (paper_data*) malloc(sizeof(paper_data));
	if(out == NULL)
	{
		perror("Error allocating memory");
		exit(1);
	}

	curr = findPaper(id);

	if(curr == NULL)
	{
		out->paper_data_val = malloc(sizeof(char));
		out->paper_data_len = 0;
		return out;
	}

	out->paper_data_val = malloc(curr->paper_info->paper.paper_data_len * sizeof(char));
	memcpy(out->paper_data_val, (&curr->paper_info->paper)->paper_data_val, curr->paper_info->paper.paper_data_len);
	out->paper_data_len = curr->paper_info->paper.paper_data_len;

	return out;
}

/***
* Retrieve the information, copy the data and set the paper content to empty, this command does not
* need to send the whole paper data, only author and title. The next and previous are also not filled
*/
paper_information *info_paper_1_svc(int_in *in, struct svc_req *req)
{
	static paper_information* out;

	int id = (int) *in;
	struct paper_list_out* curr;

	if(out != NULL)
	{
		free(out->paper.paper_data_val);
		free(out);
	}
	out = (paper_information*) malloc(sizeof(struct paper_information));
	if(out == NULL)
	{
		perror("Error allocating memory");
		exit(1);
	}
	out->author = "\0";
	out->title = "\0";
	out->paper.paper_data_len =  0;
	out->paper.paper_data_val =  malloc(sizeof(char));
	curr = findPaper(id);

	if(curr == NULL)
	{
		return out;
	}

	out->author = strdup(curr->paper_info->author);
	out->title 	= strdup(curr->paper_info->title);

	return out;
}

/***
* Same as the info function, but here the previous and next are filled if there is more than 1 paper
*/
paper_list_out *list_paper_1_svc(list_in *in, struct svc_req *req)
{
	static paper_list_out* out;
	struct paper_list_out* curr_out;
	struct paper_list_out* curr;

	curr = tail;

	freePreviousListOut(out);
	out = (struct paper_list_out*) malloc(sizeof(struct paper_list_out));
	if(out == NULL)
	{
		perror("Error allocating memory");
		exit(1);
	}

	if(curr == NULL){
		out->next				= NULL;
		out->prev				= NULL;
		out->id 				= -1;
		out->paper_info 		= (struct paper_information*) malloc(sizeof(struct paper_information));
		out->paper_info->author = "\0";
		out->paper_info->title 	= "\0";
		out->paper_info->paper.paper_data_len =  0;
		out->paper_info->paper.paper_data_val =  malloc(sizeof(char));
		return out;
	}

	curr_out = out;
	while(curr != NULL){
		curr_out->paper_info	= (struct paper_information*) malloc(sizeof(struct paper_information));
		if(curr_out->paper_info == NULL)
		{
			perror("Error allocating memory");
			exit(1);
		}

		curr_out->id 					= curr->id;
		curr_out->paper_info->author 	= strdup(curr->paper_info->author);
		curr_out->paper_info->title 	= strdup(curr->paper_info->title);
		curr_out->paper_info->paper.paper_data_len = 0;
		curr_out->paper_info->paper.paper_data_val = malloc(sizeof(char));
		curr_out->prev 					= NULL;
		if(curr->next != NULL){
			curr_out->next 			= (struct paper_list_out*) malloc(sizeof(struct paper_list_out));
			curr_out 				= curr_out->next;
			curr 					= curr->next;
		}
		else{
			curr_out->next			= NULL;
			curr 					= NULL;
		}
	}
	return out;
}

int_out *add_paper_1_svc(paper_information *in, struct svc_req *req)
{
	struct paper_list_out* newHead;

	
	if((newHead = paperExists(in)) == NULL){
		newHead 						= (struct paper_list_out*) malloc(sizeof(struct paper_list_out));
		newHead->next					= NULL;
		newHead->prev					= NULL;
		newHead->id 					= head == NULL ? 0 : head->id + 1;
		newHead->paper_info 			= (struct paper_information*) malloc(sizeof(struct paper_information));
		(newHead->paper_info)->author 	= strdup(in->author);
		(newHead->paper_info)->title	= strdup(in->title);

		if(head != NULL){ //If a head exist we need to set the head to this previous paper
			newHead->prev = head;
			head->next = newHead;
		}//Otherwise there is no previous paper

		head = newHead; //This paper becomes the new head
		if(tail == NULL) //If it is the first paper, it becomes the tail as well
		{
			tail = newHead;
		}
	}

	(newHead->paper_info)->paper.paper_data_val = (char *)malloc(in->paper.paper_data_len * sizeof(char));
	memcpy((newHead->paper_info)->paper.paper_data_val,
		in->paper.paper_data_val,
		in->paper.paper_data_len
	);
	(newHead->paper_info)->paper.paper_data_len = in->paper.paper_data_len;
	return (int_out*) &newHead->id;
}
