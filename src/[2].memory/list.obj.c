/* ---------------------------------------------------------------------------
 * @
 *						UTILITY FOR DEMONSTRATING
 *			NEGATIVE INFLUENCE OF PAGED MEMORY ORGANIZATION
 *			-----------------------------------------------
 *
 * Bild 0x001  14.06.2002
--------------------------------------------------------------------------- */
#define N_ELEM	1000					// number of list elements

// uncomment the following line to enable "pessimized" variant of the structure
//#define PESSIMIZE						// overseparated
										// obj_attr[ATTR_SIZE] --> obj_attr*

#define ATTR_SIZE	14
#define	BODY_SIZE	8000

struct LIST_OF_OBJ {					// NON-OPTIMIZED LIST
	struct   LIST_OF_OBJ *next;			// pointer to the next object
	int		 obj_attr[ATTR_SIZE];		// object attributes /something compact
	int		 obj_body[BODY_SIZE];		// object body		/somtehing monstrous
};

struct LIST_OF_OBJ_OPTIMIZED {			// OPTIMIZED LIST
	struct LIST_OF_OBJ_OPTIMIZED *next;	// pointer to the next object

	#ifdef PESSIMIZE
		int		*obj_attr;				// pointer to attributes (that's too bad!)
	#else			
		int		 obj_attr[ATTR_SIZE];	// object attributes (this is good!)
	#endif

	int			 *obj_body;				// pointer to the object body
};

#include <DoCPU.h>
main()
{

	int a, attr, x = 0;
	struct LIST_OF_OBJ *list_of_obj, *tmp_list_of_obj;
	struct LIST_OF_OBJ_OPTIMIZED	 *list_of_obj_optimized,
									 *tmp_list_of_obj_optimized;

	//TITLE
	PRINT("= = = demonstration of the negative influence of the page memory organization = = =\n");
	PRINT_TITLE;

	/* -----------------------------------------------------------------------
	 *
	 *						processing classical list
	 *
	------------------------------------------------------------------------ */
	// allocating memory
	list_of_obj = (struct LIST_OF_OBJ*)
		_malloc32(N_ELEM*sizeof(struct LIST_OF_OBJ));
	
	// list initialization
	for (a = 0; a < N_ELEM; a++)
		list_of_obj[a].next = list_of_obj + a + 1;
		list_of_obj[N_ELEM-1].next = 0;
	
	// tracing the list
	AL_BEGIN;
		CLEAR_L2_CACHE();
		L_BEGIN(0)
			tmp_list_of_obj = list_of_obj;
			do{
				for(attr = 0; attr < ATTR_SIZE; attr++)
					x += tmp_list_of_obj[0].obj_attr[attr];
			} while(tmp_list_of_obj = tmp_list_of_obj[0].next);
		L_END(0)
	AL_END;
	L1_OUT("CLASSIC   ");


	/* ----------------------------------------------------------------------
	 *
	 *			processing optimized list
	 *
	----------------------------------------------------------------------- */
	// allocating memory
	list_of_obj_optimized = (struct LIST_OF_OBJ_OPTIMIZED*)
					_malloc32(N_ELEM*sizeof(struct LIST_OF_OBJ_OPTIMIZED));

	// list initialization
	for (a = 0; a < N_ELEM ;a++)
	{
		list_of_obj_optimized[a].next = list_of_obj_optimized + a + 1;
		
		#ifdef PESSIMIZE
			list_of_obj_optimized[a].obj_attr = malloc(sizeof(int)*ATTR_SIZE);
			// printf("*%X\n",list_of_obj_optimized[a].obj_attr);
		#endif
		
		list_of_obj_optimized[a].obj_body = malloc(sizeof(int)*BODY_SIZE);
	}	list_of_obj_optimized[N_ELEM-1].next = 0;

	// tracing the list
	AL_BEGIN;
		CLEAR_L2_CACHE();
		L_BEGIN(1)
			tmp_list_of_obj_optimized=list_of_obj_optimized;
			do{
				for(attr=0;attr<ATTR_SIZE;attr++)
					x+=tmp_list_of_obj_optimized[0].obj_attr[attr];

			} while(tmp_list_of_obj_optimized
								= tmp_list_of_obj_optimized[0].next);
		L_END(1)
	AL_END;
	Lx_OUT("OPTIMIZED ",Ax_GET(0),Ax_GET(1));
}



