/* ---------------------------------------------------------------------
 * @
 *						MEMCMP optimization
 *						==================
 *
 * Build 0x001	09.06.2002
------------------------------------------------------------------------ */
// configuration
#define BLOCK_SIZE		(4*M)			// size of the processed block
#define DRAM_PG_SIZE	(4*K)			// DRAM page length
#define BRUST_LEN		(32)			// burst cycle length


#include <DoCPU.h>
main()
{
	char *p1, *p2;
	int a, b, x = 0;	
	unsigned int crc_1,crc_2;

	// TITLE
	PRINT("=== optimization of the memcmp function ===\n");
	PRINT_TITLE;

	// allocating memory
	p1 = (char *) _malloc32(BLOCK_SIZE);
	p2 = (char *) _malloc32(BLOCK_SIZE+DRAM_PG_SIZE);

	// fill both regions with zeros (to facilitate the comparison)
	memset(p1, 0, BLOCK_SIZE);
	memset(p2, 0, BLOCK_SIZE + DRAM_PG_SIZE);

	// make the last byte of the two blocks different
	p1[BLOCK_SIZE-1] = 66;
	p2[BLOCK_SIZE-1] = 99;

	/* ----------------------------------------------------------------------
	 *
	 *						BUILT-IN memcmp FUNCITON
	 *
	 * ------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(0)
		x += memcmp(p1, p2, BLOCK_SIZE);
	A_END(0)

	/* ----------------------------------------------------------------------
	 *
	 *						BUILT-IN memcmp FUNCTION
	 *					with  DRAM banks interleave
	 *
	 * ------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(1)
		x += memcmp(p1, p2 + DRAM_PG_SIZE, BLOCK_SIZE);
	A_END(1)

	/* ----------------------------------------------------------------------
	 *
	 *				COMPARING BY DOUBLE WORDS WITHOUT INTERLEAVE
	 *
	 * ------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(2)
		a = 0;
		while(a<BLOCK_SIZE)
		{
			if (*(int*)((int)p1+a)!= *(int*)((int)p2+a)) break;
														/* difference */
			a+=sizeof(int);
		}
	A_END(2)


	/* ----------------------------------------------------------------------
	 *
	 *					COMPARING BY DOUBLE WORDS
	 *			WITH OPTIMAL DRAM BANKS INTERLEAVE
	 *
	 * ------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(3)
		a=0;
		while(a < BLOCK_SIZE)
		{
			if (*(int*)((int)p1+a) != *(int*)((int)p2+a+DRAM_PG_SIZE)) break;
															/* difference */
			a += sizeof(int);
		}
	A_END(3)


	/* ----------------------------------------------------------------------
	 *
	 *							HACHE COMPARISON
	 *
	 * ------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(4)
		for(a = 0;a < BLOCK_SIZE; a += DRAM_PG_SIZE)
		{
			crc_1 = 0; crc_2 = 0;

			// calculating the CRC of the block by the p1 pointer
			for(b = 0; b < DRAM_PG_SIZE; b += sizeof(int))
				// attention! this is a weak algorithm for calculating CRC
				// and it can be used for _demonstration purposes only_
				crc_1 += *(int*)((int)p1+a+b);
				
			// calculating the CRC of the block by the p2 pointer
			for(b = 0; b < DRAM_PG_SIZE; b += sizeof(int))
				crc_2 += *(int*)((int)p2+a+b);
		
			if (crc_1 != crc_2)
				break;	// if CRC do not match,
						// memory blocks are different.
						// when necessary it is possible to call
						// memcmp(p1+a, p2+a, BLOCK_SIZE-a)
						// to clarify the result
		}
	A_END(4)


	/* ----------------------------------------------------------------------
	 *
	 *							HASH COMPARIOSON
	 *			WITH OPTIMAL DRAM BANKS INTERLEAVE
	 *
	 * ------------------------------------------------------------------- */
	CLEAR_L2_CACHE();VVV;
	A_BEGIN(5)
		for(a = 0; a < BLOCK_SIZE; a += BRUST_LEN)
		{
			crc_1 = 0; crc_2 = 0; b = 0;

			// caluclating the CRC of the block by the p1 pointer
			for(b = 0; b < BRUST_LEN; b += sizeof(int))
				crc_1 += *(int*)((int)p1+a+b);
		
			// calculating the CRC of the block by the p2 pointer
			for(b = 0; b < BRUST_LEN; b += sizeof(int))
				crc_1 += *(int*)((int)p2+a+b +DRAM_PG_SIZE /* <-- */);

			if (crc_1 != crc_2)
			{
				//printf("Opa!\n");
				break;	// If CRC do not match, 
						/// memory blocks are different.
						// when necessary it is possible to call
						// memcmp(p1+a, p2+a, BLOCK_SIZE-a)
						// to clarify the result
			}
		}
	A_END(5)

	// output of the results
	printf("memcmp......................... %d", Ax_GET(0)); L1_OUT("");
	printf("memcmp with DRAM-interleaving.. %d", Ax_GET(1)); Lx_OUT("",Ax_GET(0),Ax_GET(1));
	printf("DWORD compare.................. %d", Ax_GET(2)); Lx_OUT("",Ax_GET(0),Ax_GET(2));
	printf("DWORD compare with interleaving %d", Ax_GET(3)); Lx_OUT("",Ax_GET(0),Ax_GET(3));
	printf("Hash-compare................... %d", Ax_GET(4)); Lx_OUT("",Ax_GET(0),Ax_GET(4));
	printf("Hash-compare with interleaving. %d", Ax_GET(5)); Lx_OUT("",Ax_GET(0),Ax_GET(5));

	return x+crc_1+crc_2;
}


