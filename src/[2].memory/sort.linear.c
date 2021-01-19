/* ---------------------------------------------------------------------------
 * @
 *					DEMONSTRATION OF THE EFFICIENCY OF THE
 *				LINEAR SORTING ALGORITHM
 *				==================================
 *
 * Build 0x001 04.05.2002
-------------------------------------------------------------------------- */
// configuration
#define N_MAX	10000000

#define VAL_MAX	1000		// max. allowed value of the sorted elements
#define _NORDTSC			// for measuring large time intervals

#include <DoCPU.h>


/* ---------------------------------------------------------------------------
 *
 *				LINEAR SORTING OF n ELEMENTS OF THE p ARRAY
 *
--------------------------------------------------------------------------- */
int* linear_sort(int *p, int n)
{
	int a, b;
	int count = 0;
	int *virtual_a;					// array for sorting

	// allocating memory
	virtual_a = malloc(VAL_MAX*sizeof(int));
	if (!virtual_a) /* insufficient memory */ return 0;

	// init
	memset(virtual_a, 0, VAL_MAX*sizeof(int));

	// sorting
	for(a = 0; a < n; a++)
		virtual_a[p[a]]++;

	// forming an answer
	for(a = 0; a < VAL_MAX; a++)
		for(b = 0; b < virtual_a[a]; b++)
			p[count++]=a;

	// releasing memory
	free(virtual_a);

	return p;
}


/*----------------------------------------------------------------------------
 *
 *							FUNCTION FOR COMPARING QSORT
 *
--------------------------------------------------------------------------- */
int compare( const void *arg1, const void *arg2 )
{
	return *(int *)arg2 - *(int *)arg1;
}


/*----------------------------------------------------------------------------
 * 
 *				SORTING N NUMBERS USING VARIOUS METHODS AND COMPARING EFFICIENCY
 *
--------------------------------------------------------------------------- */
vs(int *p_array, int *p_src, int N)
{
	int a;
	char buf[MAX_STR_LEN];
	#define FORCE ((N>500000)?1:0)

	VVV
	// qsort
	AL_BEGIN
		memcpy(p_array,p_src,N*sizeof(int));
		UL_BEGIN(0, FORCE)
			qsort( (void *)p_array, N, sizeof(int), compare);
		UL_END(0, FORCE)
		VVV;
	AL_END

	// linear sort
	AL_BEGIN
		memcpy(p_array,p_src, N*sizeof(int));
		UL_BEGIN(1, FORCE)
			linear_sort(p_array, N);
		UL_END(1, FORCE)
		VVV
	AL_END
	sprintf(buf,"N %8d %4.5f %4.5f",N,cpu2timeu(Ax_GET(0),FORCE), cpu2timeu(Ax_GET(1),FORCE));
	PRINT(buf);	Lx_OUT("",Ax_GET(0),Ax_GET(1));
}

main()
{
	int a;
	int *p_src, *p_array;

	// TITLE
	PRINT("= = = Demonstration of linear_sort superiority over qsort = = =\n");
	PRINT_TITLE;

	// allocating memory
	p_src	= malloc(N_MAX*sizeof(int));
	p_array	= malloc(N_MAX*sizeof(int));

	// preparing array of numbers to be sorted
	for (a = 0; a < N_MAX; a++)
		p_src[a]=rand() % VAL_MAX;

	#define VS(N) if(N>N_MAX) {PRINT("-ERR:too big\n");return -1;} else vs(p_array, p_src, N);

	printf(_TEXT("numbers sort tmime qsort/linear(сек.) superiority\n"));

	// SORTING 100 NUMBERS
	VS(100)

	// SORTING 1.000 NUMBERS
	VS(1000)

	// SORTING 10.000 NUMBERS
	VS(10000)

	// SORTING 100.000 NUMBERS
	VS(100000)

	// SORTING 1.000.000 NUMBERS
	VS(1000000)

	// SORTING 10.000.000 SORTING
	VS(10000000)

	return 0;
}




