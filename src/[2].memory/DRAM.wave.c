/* ---------------------------------------------------------------------
 * @
 *					UTILITY FOR MEASURING THE DEPENDENCE
 *		OF THE CELL ACCESS TIME DEPENDING ON THE READ STEP
 *		=====================================================
 *
 * Build 0x004 24.05.2002	DRAM emulator removed
 * Build 0x003 29.05.2002	dram emulator added
 * Build 0x002 24.05.2002
------------------------------------------------------------------------ */
//CONFIGURATION
#ifdef DETAL_ANALYSE
	#define STEP_FACTOR	4				// step size increment
	#define MAX_PG_SIZE	(8*K)			// maximum step size
	#define BLOCK_SIZE	(1*M)			// size of the processed block
#else
	#define STEP_FACTOR	(64)			// step size increment
	#define MAX_PG_SIZE	(64*K)			// maximum step size
	#define BLOCK_SIZE	(4*M)			// size of the processed block
#endif

#include <DoCPU.h>
main()
{
	int a, b;							// loop counters
	int pg_size;						// current read step
	int x = 0;							// variable for reading memory cells
	int *p;								// pointer to the block being processed

	// TITLE
	PRINT("=== demonstrating memory hit, miss & organization ===\n");
	PRINT_TITLE;

	// allocating memory
	p = (int*) _malloc32(BLOCK_SIZE);

	// header
	printf("STEP");
	for(pg_size = STEP_FACTOR; pg_size <= MAX_PG_SIZE; pg_size += STEP_FACTOR)
		printf("\t%d",pg_size);printf("\nTime");

	// loop for testing steps
	for(pg_size = STEP_FACTOR; pg_size <= MAX_PG_SIZE; pg_size += STEP_FACTOR)
	{
		PRINT_PROGRESS(100*pg_size/MAX_PG_SIZE);VVV;
		A_BEGIN(0)

			// loop reading memory with the predefined step
			// since the amount of physcial memory is limited,
			// and at large read step not all cells bein read will fit within the cache
			// To workaround this problem it is necessary to represent the memory
			// as a ring array, as follows:
			for (b = 0; b < pg_size; b += STEP_FACTOR)
				for(a = b; a < BLOCK_SIZE; a += pg_size)
					x += *(int *) ((int)p + a );
		A_END(0)
		printf("\t%d",Ax_GET(0));
	}
	printf("\n");

	return x;
}

