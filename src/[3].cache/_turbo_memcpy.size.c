/*----------------------------------------------------------------------------
 *
 *						EXAMPLE DEMONSTRATING DEPENDENCE
 *			of the performance on the size of the prefetched memory block
 *			========================================================
 *
---------------------------------------------------------------------------- */
// CONFIGURATION
#define BLOCK_SIZE	(4*M)						// size of the processed block

#include <DoCPU.h>
#include <DoCPU.cmd.h >

// Turbo-copying function
__turbo_memcpy(char *dst, char *src, int len, int _page_size)
{
	int a, b, temp;
	for (a=0; a < (len - _page_size); a += _page_size)
	{
		// prefetching 
		temp = *(int *)((int) src + a + _page_size);
		for (b = a; b < a + _page_size; b += L1_PREFETCH_SIZE)
			__prefetchnta(src + b);
		
		for (b = a; b < a + _page_size; b += 16 * 8)
		{
			__stream_cpy(dst + b + 16*0, src + b + 16*0);
			__stream_cpy(dst + b + 16*1, src + b + 16*1);
			__stream_cpy(dst + b + 16*2, src + b + 16*2);
			__stream_cpy(dst + b + 16*3, src + b + 16*3);

			__stream_cpy(dst + b + 16*4, src + b + 16*4);
			__stream_cpy(dst + b + 16*5, src + b + 16*5);
			__stream_cpy(dst + b + 16*6, src + b + 16*6);
			__stream_cpy(dst + b + 16*7, src + b + 16*7);
		}
	}
}

main()
{
	int a;
	char *src, *dst;
	src= (char *) _malloc32(BLOCK_SIZE);
	dst=(char *) _malloc32(BLOCK_SIZE);

	// header
	printf("size");for (a = 0*K; a < 1*M; a += 16*K) printf("\t%d",a/K);
	printf("\n%dM READ",BLOCK_SIZE/(M));

	for (a = 0*K; a < 1*M; a += 16*K)
	{
		A_BEGIN(0);
			__turbo_memcpy(dst, src,  a, 4096);
		A_END(0);

		A_BEGIN(1);
			memcpy(dst, src,  a);
		A_END(1);

		printf("\t%4.0f",(float)Ax_GET(0)*100/Ax_GET(1));
	}
	printf("\n");
}

