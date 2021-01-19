/* ---------------------------------------------------------------------------
 *
 *			DEMONSTRATION OF SPONTANEOUS FLUSHING OF STORE BUFFERS
 *			=====================================================
 *
 * Build 0x001 08.08.2002
--------------------------------------------------------------------------- */
#include <DoCPU.h>

#include "store_buff.h"			// h-file is generated automatically 
								// by the make-file and contains definition of N_ITER,
								// which stands for number of write iterations

main()
{
	int		*p;
	int		a, b;
	int		*buff;

	// allocating memory
	p = (int *) malloc(1*M);

	// sleep, in order to hit exact time quantum
	Sleep(100);
	
	// call store_buf.xm
	buff = DoCPU(&p);

	// display measurement results
	printf("%d",N_ITER);for(a = 0; a < 10; a++) printf("\t%d",buff[a]);
	printf("\n");

}
