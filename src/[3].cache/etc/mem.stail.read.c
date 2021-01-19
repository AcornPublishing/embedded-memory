/* ---------------------------------------------------------------------------
 *
 *				DEMONSTRATION OF MEMORY DELAYS
 *			WHEN READING/WRITING DATA BLOCKS OF DIFFERENT SIZES
 *			====================================================
 *
 * Build 0x001 01.09.2002
--------------------------------------------------------------------------- */
#define BLOCK_SIZE	(16*K)			// size of the processed block


#include <DoCPU.h>
main()
{
	int  a;
	int  *p;
	char tmp8  = 0;
	int  tmp32 = 0;

	PRINT("= = = Demonstration of delays when reading/writing data blocks of different size = = =\n");
	PRINT_TITLE;

	// alocating memory
	p = (int*)_malloc32(BLOCK_SIZE);

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *						OPTIMIZED VERSION
	 *					Long Read/Long Write (same addrr)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(0)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			tmp32 += *(int *)((int)p + a);
			*(int *)((int)p + a) = tmp32;
		}
	A_END(0)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Short Read/Long Write (same addrr)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(1)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			tmp8 += *(char *)((int)p + a);
			*(int *)((int)p + a) = tmp32;
			
		}
	A_END(1)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Short Read/Long Write (overlap space)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(2)
		for(a = sizeof(tmp32); a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			tmp8 += *(char *)((int)p + a + (sizeof(tmp32)/2)); 
			*(int *)((int)p + a ) = tmp32;
			
		}
	A_END(2)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Long Read/Short Write (same addrr)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(3)
		for(a = 0; a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			tmp32 += *(int *)((int)p + a);
			*(char *)((int)p + a) = tmp8;
			
		}
	A_END(3)

	VVV;
	/*------------------------------------------------------------------------
	 *
	 *					NON-OPTIMIZED VERSION
	 *				Long Read/Short Write (overlap space)
	 *
	----------------------------------------------------------------------- */
	A_BEGIN(4)
		for(a = sizeof(tmp32); a < BLOCK_SIZE; a += sizeof(tmp32))
		{
			tmp32 += *(int *)((int)p + a );
			*(char *)((int)p + a + (sizeof(tmp32)/2)) = tmp8;
			
		}
	A_END(4)


	Lx_OUT("Short Read/Long  Write (same addrr)   ", Ax_GET(0), Ax_GET(1));
	Lx_OUT("Short Read/Long  Write (overlap space)", Ax_GET(0), Ax_GET(2));
	Lx_OUT("Long  Read/Short Write (same addrr)   ", Ax_GET(0), Ax_GET(3));
	Lx_OUT("Long  Read/Short Write (overlap space)", Ax_GET(0), Ax_GET(4));
	
}

