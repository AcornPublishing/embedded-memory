/* ---------------------------------------------------------------------------
 * @
 *							UTILITY FOR EVALUATION OF THE 
 *		INFLUENCE OF THE READ/WRITE TRANSACTIONS OVERLAPPING ON THE PERFORMANCE
 *		==================================================================
 *
 * Build 0x001 30.06.2002
--------------------------------------------------------------------------- */

// configuration
#define BLOCK_SIZE	(8*M)

#include <DoCPU.h>
main()
{
	int a;
	int x=0, y=0;
	int *p1, *p2;

	// TITLE
	PRINT("= = = influence of the read/write transaction overlapping on the performance = = =\n");
	PRINT_TITLE;

	// allocating memory
	p1 = malloc(BLOCK_SIZE);
	p2 = malloc(BLOCK_SIZE);

	/* -----------------------------------------------------------------------
	 *
	 *			read and write transactions are _not_ overlapped
	 *				influence of loop unrolling is not taken into account
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(0)
		// read
		for (a = 0; a < BLOCK_SIZE; a += 4)
				x += *(int *)((char *)p1 + a);

		// write
		for (a = 4; a < BLOCK_SIZE; a += 4)
			*(int *)((char *)p2 + a) = y;
	A_END(0)


	/* -----------------------------------------------------------------------
	 *
	 *			read and write transactions _do_ overlap
	 *				influence of loop unrolling is not taken into account
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(1)
		for (a = 0; a < BLOCK_SIZE; a += 4)
		{
			// read
			x += *(int *)((char *)p1 + a);

			// write
			*(int *)((char *)p2 + a + 4) = y;
		}
	A_END(1)

	/* -----------------------------------------------------------------------
	 *
	 *			read and write transaction do _not_ overlap
	 *					influence of loop unrolling is taken into account
	 *
	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(2)
		// read
		for (a = 0; a < BLOCK_SIZE; a += 32)
		{
			x += *(int *)((char *)p1 + a);
			x += *(int *)((char *)p1 + a + 4);
			x += *(int *)((char *)p1 + a + 8);
			x += *(int *)((char *)p1 + a + 12);
			x += *(int *)((char *)p1 + a + 16);
			x += *(int *)((char *)p1 + a + 20);
			x += *(int *)((char *)p1 + a + 24);
			x += *(int *)((char *)p1 + a + 28);
		}

		// write
		for (a = 4; a < BLOCK_SIZE; a += 32)
		{
			*(int *)((char *)p2 + a) = y;
			*(int *)((char *)p2 + a + 4) = y;
			*(int *)((char *)p2 + a + 8) = y;
			*(int *)((char *)p2 + a + 12) = y;
			*(int *)((char *)p2 + a + 16) = y;
			*(int *)((char *)p2 + a + 20) = y;
			*(int *)((char *)p2 + a + 24) = y;
			*(int *)((char *)p2 + a + 28) = y;
		}
	A_END(2)

	/* -----------------------------------------------------------------------

				read and write transactions _do_ overlap
						influence of loop unrolling is taken into account

	----------------------------------------------------------------------- */
	VVV;
	A_BEGIN(3)
		for (a = 0; a < BLOCK_SIZE; a += 16)
		{
			// read
			x += *(int *)((char *)p1 + a);
			x += *(int *)((char *)p1 + a + 4);
			x += *(int *)((char *)p1 + a + 8);
			x += *(int *)((char *)p1 + a + 12);

			// write
			*(int *)((char *)p2 + a) = y;
			*(int *)((char *)p2 + a + 4) = y;
			*(int *)((char *)p2 + a + 8) = y;
			*(int *)((char *)p2 + a + 12) = y;
		}
	A_END(3)

	// console output of the results
	Lx_OUT("without loop unrolling",Ax_GET(0),Ax_GET(1));
	Lx_OUT("with loop unrolling",Ax_GET(2),Ax_GET(3));
}

