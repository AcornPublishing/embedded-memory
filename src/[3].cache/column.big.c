/*----------------------------------------------------------------------------
 *
 *						THE EXAMPLE DEMONSTRATING EFFICIENCY 
 *					OF PROCESSING LARGE TWO-DIMENSIONAL ARRAYS
 *					============================================
 *
 * Build 0x001 07.08.2002
----------------------------------------------------------------------------*/
//#define __OPTIMIZE__			// uncomment this line for optimized
								// processing of the array row by row
#ifdef __OPTIMIZE__
	#define N_COL				(512+16)	// optimal number of matrix rows
											// since it is not a multiple of the
											// cache bank size and cache is used
											// entirely
#else
	#define N_COL				(512)		//  non-optimal number of matrix rows
											// since it is a multiple of the
											// cache bank size and cache is not used
											// entirely
#endif

#define N_ROW					(512)

#include <DoCPU.h>

/*----------------------------------------------------------------------------
 *
 *				SEQUENTIAL PROCESSING OF THE ARRAY BY COLUMNS
 *
----------------------------------------------------------------------------*/
int FOR_COL(int (*foo)[N_COL])
{
	int	x, y;
	int	z = 0;

	for (x = 0; x < N_ROW; x++)
	{
		for (y = 0; y < N_COL; y++)
			z += foo[x][y];
	}
	return z;
}


/*----------------------------------------------------------------------------
 *
 *				SEQUENTIAL PROCESSING OF THE ARRAY BY ROWS
 *
----------------------------------------------------------------------------*/
int FOR_ROW(int (*foo)[N_COL])
{
	int	x, y;
	int	z = 0;

	for (x = 0; x < N_COL; x++)
	{
		// attention: if the column height is a multiple of the cache bank size
		// the cache capacity will sighificantly decrease due to its
		// limited associativity. The amount of cache memory might become
		// insufficient, which would result in constant cache misses!
		for (y = 0; y < N_ROW; y++)
			z += foo[y][x];
	}
	return z;
}

#define _CACHE_SIZE	(64*K)
#define _LINE_SIZE	(32)


/*----------------------------------------------------------------------------
 *
 *				PARALLEL PROCESSING OF THE ARRAY BY COLUMNS
 *
----------------------------------------------------------------------------*/
int FOR_32(int (*foo)[N_COL])
{
	int x, y;
	int k, w;
	int z = 0;

	// testing rows one by one
	for (x = 0; x < N_ROW; x++)
	{
		// processing the next column...
		for (k = 0; k < N_COL; k += (_CACHE_SIZE/sizeof(int)))
		{	//... by portions that entirely fit
			// within L1 cache (on P-III and especially P-4 - L2 cache)

			// initiating parallel loading of data into the cache
			for (y = k; (y < k + (_CACHE_SIZE/sizeof(int))) && (y < N_COL);  y+=(_LINE_SIZE/sizeof(int)));
			{
				z += foo[x][y];
			}
			
			// reading the data that are already loaded into the cache
			for (y = k; y < k + (_CACHE_SIZE/sizeof(int)) && (y < N_COL);  y+=(_LINE_SIZE/sizeof(int)))
			{
				for (w = y + 1; w < (y + 8); w++)
					z += foo[x][w];
			}
			
		}
	}

	return z;
}



main()
{

	int	z = 0;

	// allocating memory for 2D-array
	int (*foo)[N_COL] = (int (*)[N_COL]) malloc(N_COL*N_ROW*sizeof(int));
	
	// TITLE
	PRINT("= = = Specific features of processing large two-dimensional arrays = = =\n");
	PRINT_TITLE;

	// processing by columns
	A_BEGIN(0)
		FOR_COL(foo);
	A_END(0)

	// processing by rows
	A_BEGIN(1)
		FOR_ROW(foo);
	A_END(1)

	A_BEGIN(2)
		FOR_32(foo);
	A_END(2)

	// console output of the measurement results
	Lx_OUT("SERAL	 COL vs SERIAL ROW",	Ax_GET(0), Ax_GET(1));
	Lx_OUT("PARALLEL COL vs SERIAL COL",	Ax_GET(0), Ax_GET(2));

	return z;
}
