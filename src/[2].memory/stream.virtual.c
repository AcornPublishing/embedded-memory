/* ---------------------------------------------------------------------------
 * @
 *					UTILITY FOR DEMONSTRATING
 *			EFFICIENCY OF DATA FLOWS VIRTUALIZATION
 *			==========================================
 *
 * Build 0x001	07.06.2002
--------------------------------------------------------------------------- */

// CONFIGURATION
#define BLOCK_SIZE			(2*M)	// max. amount of virtual data flow
#define MAX_N_DST			16		// max. number of virtual data flows


#define MAIL_ROOL(a)		for(a = 2; a <= MAX_N_DST; a++)
/*                            ^^^^^^^ start with two virtual data flows */

#include <DoCPU.h>
main()
{
	int a, b, r, x = 0;
	int *p, *px[MAX_N_DST];

	// TITLE
	PRINT("=== demonstration of the efficiency of data flow virtualization  ===\n");
	PRINT_TITLE;
	
	// header
	printf("N DATA STREAM"); MAIL_ROOL(a) printf("\t%d",a);printf("\n");

	/* -----------------------------------------------------------------------
	 *
	 *		flow processing using classic (non-optimized) method
	 *
	------------------------------------------------------------------------ */
	// allocating memory for all flows
	for (a = 0; a < MAX_N_DST; a++) px[a] = (int *) _malloc32(BLOCK_SIZE);

	printf("CLASSIC");
	MAIL_ROOL(r)
	{
		VVV;
		A_BEGIN(0)						/* starting point of execution time measurement */
			for(a = 0; a < BLOCK_SIZE; a += sizeof(int))
					for(b=0; b < r; b++)
						x += *(int *)((int)px[b] + a );
						// Testing all flows one by one.
						// It is easily seen that cells
						// of all flows are on different
						// DRAM pages, because of this
						// DRAM pages will be constantly 
						// opened/closed when processing more than
						// four flows, thus dropping the performance
						// ATTENTION! in this case a and b loops
						// can be exchanged by places.
						// This will increase the performance.
						// However, we want to process
						// flows _in parallel_
		A_END(0)						/* end of the execution time measurement */

		printf("\t%d",Ax_GET(0));		// output of the flow processing time
	} printf("\n");	/* end for */



	/* -----------------------------------------------------------------------
	 *
	 *				optimized processing of virtual flows
	 *
	------------------------------------------------------------------------ */
	// allocating memory for physical flow
	p = (int*) _malloc32(BLOCK_SIZE*MAX_N_DST);

	printf("OPTIMIZED");
	MAIL_ROOL(r)
	{
		VVV;
		A_BEGIN(1)						/* starting point of execution time measurement */
			for(a = 0; a < BLOCK_SIZE * r; a += (sizeof(int)*r))
			// what has changed? Look,^^ - increment step ^^^
			// is now equal to the number of virtual flows
				for(b = 0; b < r; b++)
					x += *(int *)((int)p + a + b*sizeof(int));
					// now cells of all flows are located _near_
					// because of this, the processing time is minimized
		A_END(1)						/* end of the execution time measurement */
		printf("\t%d",Ax_GET(1));		// output of the flow processing time
	} printf("\n");	/* end for */

	return 0;
}


