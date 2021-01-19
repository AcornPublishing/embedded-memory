/* ---------------------------------------------------------------------------
 * @
 *							UTILITY FOR DEMONSTRATION OF THE
 *			EFFICIENCY OF PARALLEL PROCESSING OF SEPARATED LISTS
 *			-----------------------------------------------------
 *
 * Bild 0x001  14.06.2002
--------------------------------------------------------------------------- */
#define N_ELEM	4000	// number of list elements

struct list{			// CLASSIC LIST
	struct list	*next;	// pointer to the next node
	int			val;	// value
};

struct mylist{			// OPTIMIZED SEPARATED LIST
	short int *next;	// array of pointers to the next node
	int *val;			// array of values
};

#include <DoCPU.h>
main()
{
	int a;
	int b = 0;
	struct list *classic_list,*tmp_list;
	struct mylist separated_list;

	// TITLE
	PRINT("= = = demonstration of the efficiency of parallel processing of several lists = = =\n");
	PRINT_TITLE;

	/* -----------------------------------------------------------------------
	 *
	 *						processing classic list
	 *
	------------------------------------------------------------------------ */
	// allocating memory
	classic_list = (struct list*) _malloc32(N_ELEM * sizeof(struct list));

	// list initialization
	for (a = 0; a < N_ELEM; a++)
	{
		classic_list[a].next= classic_list + a+1;
		classic_list[a].val = a;
	} classic_list[N_ELEM-1].next = 0;
	
	// tracing the list
	AL_BEGIN;
		CLEAR_L2_CACHE();
		L_BEGIN(0)
			tmp_list=classic_list;
			while(tmp_list = tmp_list[0].next);
		L_END(0)
	AL_END;
	L1_OUT("CLASSIC   ");


	/* ----------------------------------------------------------------------
	 *
	 *			processing optimized separated list
	 *
	----------------------------------------------------------------------- */
	// allocating memory
	separated_list.next = (short int *) _malloc32(N_ELEM*sizeof(short int));
	separated_list.val  = (int *)       _malloc32(N_ELEM*sizeof(int));
	
	// list initialization
	for (a=0;a<N_ELEM;a++)
	{
		separated_list.next[a] = a+1;
		/*                 ^^^ pay attention to the position of
		                       square brackets */
		separated_list.val[a] = a;
	} separated_list.next[N_ELEM-1] = 0;

	// tracing the list
	AL_BEGIN;
		CLEAR_L2_CACHE();
		L_BEGIN(1)
			while(b=separated_list.next[b]);
		L_END(1)
	AL_END;
	Lx_OUT("OPTIMIZED ",Ax_GET(0),Ax_GET(1));
}

