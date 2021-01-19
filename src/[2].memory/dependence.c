/* ---------------------------------------------------------------------------
 * @
 *				UTILITY FOR MEASURING RAM THROUGHPUT
 *		when reading dependent and independent data
 *		============================================================
 *
 * Build 0x003	12.06.2002
 * Build 0x002	16.05.2002
--------------------------------------------------------------------------- */

//  ŒÕ‘»√”–¿÷»ﬂ
// ============
#define BLOCK_SIZE (16*M)					// size of the processed block


#include <DoCPU.h>

main()
{
	int a;
	int x=0;
	int *p1, *p2;

	// allocating memory blocks
	p1 = malloc(BLOCK_SIZE);
	p2 = malloc(BLOCK_SIZE);
	
	// TITLE
	PRINT("=== demonstrating efficiency of processing independent data ===\n");
	PRINT_TITLE;
	PRINT("------------------ Testing ------------------\n");

	/* -----------------------------------------------------------------------
	 *
	 *		Measuring throughput when reading dependent data
	 *
	----------------------------------------------------------------------- */

	// Filling memory with nulls, since it is necessary to use the read data
	// for indirect addressing.
	memset(p1, 0, BLOCK_SIZE);
	CLEAR_L2_CACHE();

	A_BEGIN(0)
		// Unrolling the loop to compensate for CPU latency
		for (a=0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			// Reading the cell
			x = *(int *)((char *)p1 + a + 0 * sizeof(int));

			// Address of the next cell is caluclated on the basis of the
			// previous one, therefore CPU cannot send request to the chipset until,
			// it receives this cell at its disposal
			a += x;

			// Proceeding the similar way...
			x = *(int *)((char *)p1 + a + 1 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 2 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 3 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 4 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 5 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 6 * sizeof(int));
			a += x;
			x = *(int *)((char *)p1 + a + 7 * sizeof(int));
			a += x;
		}
	A_END(0)
	printf("%s:\t %4.2f (Mbytes/s)\n",		/* displaying the results */
	_TEXT("◊Reading independent data"), ((BLOCK_SIZE)/cpu2time(Ax_GET(0))));

	/* -----------------------------------------------------------------------
	 *
	 *	Measuring thorughput when reading independent data
	 *
	----------------------------------------------------------------------- */
	CLEAR_L2_CACHE();

	A_BEGIN(1)
		for (a=0; a < BLOCK_SIZE; a += 8 * sizeof(int))
		{
			// Now the processor can send the next request for reading
			// without waiting for the previous one to accomplish, since
			// its address is in now way related to the processed data
			x += *(int *)((char *)p2 + a + 0 * sizeof(int));
			x += *(int *)((char *)p2 + a + 1 * sizeof(int));
			x += *(int *)((char *)p2 + a + 2 * sizeof(int));
			x += *(int *)((char *)p2 + a + 3 * sizeof(int));
			x += *(int *)((char *)p2 + a + 4 * sizeof(int));
			x += *(int *)((char *)p2 + a + 5 * sizeof(int));
			x += *(int *)((char *)p2 + a + 6 * sizeof(int));
			x += *(int *)((char *)p2 + a + 7 * sizeof(int));
		}
	A_END(1)

	// console output
	printf("%s:\t %4.2f (Mbytes/s)\n",		/* displaying the results */
	_TEXT("Reading independent data"),((BLOCK_SIZE)/cpu2time(Ax_GET(1))));


	PRINT(_TEXT("--------------------------------------------------\n"));
return x;

}