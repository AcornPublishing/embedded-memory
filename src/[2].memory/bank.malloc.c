/* ---------------------------------------------------------------------------
 * @
 *			UTILITY FOR DETERMINING DRAM BANKS
 *		FROM WHICH MEMORY BLOCKS ALLOCATED BY MALLOC START
 *		===================================================
 *
 * Build 0x001	5.06.2002
--------------------------------------------------------------------------- */
#define N_ITER			9				// number of iterations
#define STEP_FACTOR		(100*K)			// increment step
#define MAX_MEM_SIZE	(1*M)			// max. amount of requested memory

#define _one_bit		11				// these bits _usually_ are reponsible...
#define _two_bit		12				//				...for bank selection

// bit mask
#define MASK			((1<<_one_bit)+(1<<_two_bit))

// allocating memory and determining bank number that starts the block
#define zzz(a) (((int) malloc(a) & MASK) >> _one_bit)

#include <DoCPU.h>
main()
{
	int a, b;
	
	// TITLE
	PRINT("= = = determining numbers of DRAM-banks = = =\n");
	PRINT_TITLE;

	// subTITLE
	printf("   BLOCK\n SIZE (Kb)	-	-	-	n BANK	-	-	-\n");
	printf("----!----------------------------------------------------------------------\n");

	// cyclically allocate memory blocks of different size
	for(a = STEP_FACTOR; a < MAX_MEM_SIZE; a += STEP_FACTOR)
	{
		printf("%04d:",a/K);	/* amount of requested memory */
		for (b = 0; b < N_ITER; b++)
			printf("\t%x",zzz(a));	/* xx DRAM bank! */
		printf("\n");
	}
}


