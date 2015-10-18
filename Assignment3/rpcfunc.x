struct paper_out{
	string author<>;
	string title<>;
};

struct paper_list_out
{
	struct paper_list_out* next;
	int id;
	paper_out* paper_info;
};

struct add_paper_in
{
	string author<>;
	string title<>;
	string paper<>;
};

typedef int int_in;
typedef string paper_content_out<>;
typedef int int_out;
typedef int list_in;

program RPC_FUNCTIONS
{
  version RPC_FUNC_VERS
  {
		int_out 			REMOVE_PAPER(int_in) 	= 1;	/* Procedure nb */
		paper_content_out 	FETCH_PAPER(int_in) 	= 2;	/* Procedure nb */
		paper_out 			INFO_PAPER(int_in) 		= 3;	/* Procedure nb */
		paper_list_out 		LIST_PAPER(list_in) 	= 4;	/* Procedure nb */
		int_out 			ADD_PAPER(add_paper_in) = 5;	/* Procedure nb */
  } = 1;												/* Version nb */
} = 0x00000042;											/* Program number */
