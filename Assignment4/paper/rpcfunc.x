typedef opaque paper_data<>;
typedef int int_in;
typedef int int_out;
typedef int list_in;

struct paper_information{
	string author<>;
	string title<>;
	paper_data paper;
};

struct paper_list_out
{
	struct paper_list_out* next;
	struct paper_list_out* prev;
	int id;
	struct paper_information* paper_info;
};


program RPC_FUNCTIONS
{
  version RPC_FUNC_VERS
  {
		int_out 			REMOVE_PAPER(int_in)	 	= 1;	/* Procedure nb */
		paper_data		 	FETCH_PAPER(int_in) 		= 2;	/* Procedure nb */
		paper_information	INFO_PAPER(int_in) 			= 3;	/* Procedure nb */
		paper_list_out 		LIST_PAPER(list_in) 		= 4;	/* Procedure nb */
		int_out 			ADD_PAPER(paper_information)= 5;	/* Procedure nb */
  } = 1;												/* Version nb */
} = 0x00000042;	
