/*----------------------------------------------------------------------------
 *
 *							D	0		C	P	U
 *                      S               D              K
 *                      ================================
 *
 *				"DoCPU CLOCK" P r o f i l e r  V e r s i o n  1.1
 *              =================================================
 *
 *										for internal use
 *										with the "Code Optimization: Effective Memory Usage"
 *										book by Kris Kaspersky	kpnc@programme.ru
															kk@sendmail.ru
-------------------------------------------------------------------------- */


/*============================================================================
 *
 *						GLOBAL COMMAND-LINE KEYS
 *
 *	$Fcpu:xxx				-	Forcedly set CPU speed
 *	$NoSort					-	Do no sort
 *
 *	$DEBUG.print.Fcpu		-	Debug print of the CPU speed
 *
 *
 *
============================================================================*/


/*============================================================================
 *
 *
 *	A_NITER				-	number of times to run the profiled fragment

_NORDTSC
 * TITLE				-	Title displayed by the PRINT_TITLE macro
============================================================================*/

// built-in libraries
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// DoCPU libraries
//#define __DOS__
#include "CPU.ini"
#include "PC.ini"


// Global macros
#undef _TEXT
#ifdef __DOS__
    #define TITLE "Utility for the \"Code Optimization: Effective Memory Usage\"  "
#else
    #define TITLE "Utility for the \"Code Optimization: Effective Memory Usage\"  "
#endif

#define ERR_OEM	"-ERR translates a string into the OEM-defined character set"
#define TEXT_PIPE	_TEXT("* Program output can be redirected into the file\n")

#ifndef A_NITER					// how many times to run the profiled fragment
	#define A_NITER			8
#endif


#ifndef DoCPU_BUFF_N			// maximum number of measured buffers
	#define DoCPU_BUFF_N	0x10
#endif

// Non-configurable definitions
#define TIME_TO_WAIT		100
#define MAX_STR_LEN			256
#define DoCPU_BUFF_SIZE		A_NITER

#define	MAX_GRAPH_LEN	80
#define	GRAPH_CHAR		":"



#define A_BREAK __asm{int 0x3}

/*----------------------------------------------------------------------------
 *
 *						FUNCTION PROTOTYPES
 *
----------------------------------------------------------------------------*/

// Declaring kernel functions of the profiler from DoCPU.obj
// ------------------------------------------------------
#ifdef __cplusplus
	extern "C" int*	__cdecl	DoCPU(void* x);
	extern "C" void	__cdecl	A1(unsigned int *t);
	extern "C" int	__cdecl	A2(unsigned int *t);
#else
	extern int*		__cdecl	DoCPU(void* x);
	extern void		__cdecl	A1(unsigned int *t);
	extern int		__cdecl	A2(unsigned int *t);
#endif

// To avoid loading windows.h declare everything here
#ifndef _WINDOWS_
	extern int __stdcall Sleep(int a);
	extern int __stdcall GetTickCount(void);
	extern int __stdcall CharToOemA(char *src, char *dst);
#endif

// ERROR - there is such a macro
#ifdef ERROR
	#undef ERROR
#endif

// ???
extern void _null();


/*----------------------------------------------------------------------------
 *
 *						STATIC VARIABLES
 *						================
 *										for use in clock.h
 *
 * Attention:
 *			do not use these variables in your programs, since
 *			they might be changed in future releases!
----------------------------------------------------------------------------*/
static unsigned int	DoCPU_AL_Rool;						// AL runs loop
static unsigned int	DoCPU_tmp, DoCPU_tmp0, DoCPU_tmp1;	// temporary variables

static unsigned int	DoCPU_t[DoCPU_BUFF_N];
static unsigned int	DoCPU_buff[DoCPU_BUFF_N][DoCPU_BUFF_SIZE];

static char			DoCPU_v[]="|/-\\";
static char			DoCPU_s[MAX_STR_LEN];				// string buffer

float				DoCPU_float_tmp;
static int			CPU_CLOCK = 0;

static unsigned int			DoCPU_vcp=0;
static int*			p_cache=0;



/* ---------------------------------------------------------------------------
 *
 *							"PHYSICAL" CONSTANTS
 *
----------------------------------------------------------------------------*/
#define	K			1024
#define	M			(1024*K)
#define G			(1024*M)
#define T			(1024*G)
#define	Mega		1000000
#define	Bit			8



/* ---------------------------------------------------------------------------
 *
 *							MEMORY BLOCK SIZES
 *
----------------------------------------------------------------------------*/
#define _HUGE		(512*M)
#define _LARGE		(MAX_AVIAL_MEM)
#define	_NORMAL		(L2_CACHE_SIZE*10)
#define _MEDIUM		(L2_CACHE_SIZE/2)
#define _SMALL		(L1_CACHE_SIZE/2)

#define MAX_CACHE_LINE_SIZE												\
					MAX(L1_CACHE_LINE_SIZE, L2_CACHE_LINE_SIZE)

#define MAX(a,b)	(((a)>(b))?a:b)
#define MIN(a,b)	((a>b)?b:a)


/*  -----------------------------------------------------------------------

                        DEFINING BLOCK SIZE

    ---------------------------------------------------------------------- */
//  Defining the size of the processed block
//  TERMINOLOGY:
//  _SMALL_BLOCKS_  (SMALL)  -   blocks that fit within L1 cache
//  _MEDIUM_BLOCKS_  (MEDIUM) -   blocks that fit within L2 cache
//  _LARGE_BLOCKS_    (LARGE)  -   blocks exceeding L2 cache
//  _HUGE_BLOCKS_ (HUGE)   -   blocks exceeding RAM size

// This macro creates the  _BLOCK_SIZE definition according to one
// of the defined keys [__LARGE|__MEDIUM|__SMALL] and cache size
// Large block is specified by default
// Block size definitions can be set via #define in the code of the program
// or by means of using the /D command-line key of the VC compiler
// For example: cl bla-bla-bla.c /D__MEDIUM


#ifdef __HUGE
	#define _BLOCK_SIZE _HUGE
#endif

#ifdef __LARGE
	#define _BLOCK_SIZE _LARGE
#endif

#ifdef __MEDIUM
	#define _BLOCK_SIZE _MEDIUM
#endif

#ifdef __SMALL
	#define _BLOCK_SIZE _SMALL
#endif

// Large blocks by default
#ifndef _BLOCK_SIZE
	#define _BLOCK_SIZE _LARGE
#endif



/*----------------------------------------------------------------------------
 *
 *							MEASUREMENTS OF EXECUTION TIME
 *
----------------------------------------------------------------------------*/

// BASE MACRO FOR MEASURING EXECUTION TIME
// --------------------------------------
//
//		Macros such as _[BEGIN | END]_[RDTSC|CLOCK]_  are wrappers for kernel 
// functions for measuring execution times of the profiled fragment of the program.
// These are low-level functions! Therefore do not call them from your program
//
//		Measurements are written into the DoCPU_buff buffer  specified
// by the t argument into the position defined by the value of the DoCPU_AL_Rool variable.
#define	_BEGIN_RDTSC(t)	A1(&DoCPU_t[t]);
#define	_END_RDTSC(t)	DoCPU_tmp = A2(&DoCPU_t[t]);\
						DoCPU_buff[t][DoCPU_AL_Rool] = DoCPU_tmp;

#define	_BEGIN_CLOCK(t)	DoCPU_t[t] = clock();
#define	_END_CLOCK(t)	DoCPU_tmp = clock() - DoCPU_t[t];\
						DoCPU_buff[t][DoCPU_AL_Rool] = DoCPU_tmp;


// RUN-TIME CHECKPOINTS
// ---------------------


//		The   L_BEGIN   macro sets the checkpoint for staring measurment,
// while L_END sets the checkpoint for the end of the execution time measurment.
//
//		If the  __NORDTSC__  definition is specified, then system timer is 
// used for measurements, otherwise the RDTSC command will be used (default)
//
//		The L_BEGIN/L_END macros are recommended for use only within 
// the AL_BEGIN -- AL_END loop body! It makes sense to use these macros only
// in cases when it is necessary to perform specific actions before each
// of the A_NITER runs of the profiled fragment, and execution time of these
// action must not be taken into account. The A_BEGIN/A_END macros are used 
// more frequently
#ifdef _NORDTSC
	#define L_BEGIN(t)	_BEGIN_CLOCK(t)
	#define L_END(t)	_END_CLOCK(t)
#else
	#define L_BEGIN(t)	_BEGIN_RDTSC(t)
	#define L_END(t)	_END_RDTSC(t)
#endif

//		The UL_BEGIN/UL_END macros are the same as L_BEGIN/
// L_END, however the choice of the measurement strategy here can be done both
// at compile time and at run time.
//
//	ARG:
//		t	-	stream index for writing measurment results
//		tt	-	measurement strategy. 
//				  0 : RDTSC is used
//				!=0 : system timer is used
#define	UL_BEGIN(t,tt)	if (tt) {_BEGIN_CLOCK(t);}	else	{_BEGIN_RDTSC(t);}
#define	UL_END(t,tt)	if (tt) {_END_CLOCK(t);	}	else	{_END_RDTSC(t);  }



// CYCLIC PROFILING  RUNNING THE PROFILED FRAGMENT A_NITER times
// ---------------------------------------------------------------------

//		The  AL_BEGIN/A_END  macros run the marked fragment A_NITER times
// and write the number of the current iteration into the DoCPU_AL_Rool variable,
// defining the current position for writing the measurement results of the [U]L_BEGIN/
// [U]L_END macros
#define	AL_BEGIN		for(DoCPU_AL_Rool = 0;\
						DoCPU_AL_Rool < A_NITER; DoCPU_AL_Rool++ )\
						{
#define	AL_END			}


//		The  A_BEGIN/A_END   macros represent ready-to-use tool for
// creating the measurement checkpoints.  They run the profiled fragment
// A_NITER times, measuring execution time for each iteration
//
//		ATTENTION: MEASUREMENTS CANNOT BE NESTED!!!!
#define	A_BEGIN(t)		AL_BEGIN; L_BEGIN(t);
#define	A_END(t)		L_END(t); AL_END;


// READING AND PROCESSING MEASUREMENT RESULTS
// ---------------------------------------------

//		The Ax_GET macro returns average exeuction time of the fragment
// marked by the t checkpoint.
#define	Ax_GET(t)		cycle_mid(DoCPU_buff[t],0)

//		The  Lx_GET  macro returns the  _current_  value of the t checkpoint
// measurement. ATTENTION: do not use this macro if you are not absolutely,
// sure what actually are you doing!
#define	Lx_GET(t)		DoCPU_buff[t][DoCPU_AL_Rool]

//		The   Ax_GET_MIN   macro returns the minimum execution time 
// for the profiled fragment marked by the t checkpoint.
#define	Ax_GET_MIN(t)	cycle_min(DoCPU_buff[t],0)

//		The Ax_GET_MIN macro returns the execution time mode for the
// profiled fragment marked by the t checkpoint.
#define	Ax_GET_MOD(t)	cycle_mod(DoCPU_buff[t],0)


//		The ALx_GET_MIN macro returms the measurment time for the profiled 
// fragment specified by the t checkpoint in tt iteration
#define	ALx_GET(t,tt)	DoCPU_buff[t][DoCPU_AL_Rool*0 + tt]


/*----------------------------------------------------------------------------
 *
 *								SCREEN OUTPUT
 *
----------------------------------------------------------------------------*/

// WIN --> MS-DOS conversion
// ----------------------------
//
//	-	before version 1.1 this conversion had to be used for console output,
//		of the text containing national language characters
//      and typed in using andy Windows editor
//
//	-	starting with version 1.1 is transparent for programmers
//		ATTENTION: reverse conversion distorts the text!
#define _TEXT(a)		(CharToOemA(a,DoCPU_s)?&DoCPU_s[0]:ERR_OEM)


// MACROS FOR SCREEN OUTPUT
// ------------------------------

//		The  PRINT  macro translates the s string into DOS-encoding and
// outputs it to the terminal. The PRINT macro output cannot be redirected into a file.
//		To redirect output, use the built-in printf function
#define	PRINT(s)		fputs(_TEXT(s),stderr);

//		The ERROR macro displays the s string on the terminal, beeps and terminates execution
#define	ERROR(s)		{ PRINT(s); PRINT("\x7"); return 0; }

//		The PRINT_TITLE macro displays the service header and (c)
#define	PRINT_TITLE		PRINT("* "); PRINT(TITLE);PRINT("\n");

//		The  PRINT_PIPE  macro informs on the necessity to redirect the application output
// into a file. This is useful for applications that generate output tables that later can be 
// imported into MS Graph
#define	PRINT_PIPE		PRINT(TEXT_PIPE);



// MACROS FOR OUTPUT OF THE MEASUREMENT RESULTS ON THE SCREEN
// ----------------------------------------------------------

//		The L1_OUT macro displays the s string and the 100% label on screen
// Usually it is used in combination with Lx_OUT for output of the relative execution speed
// of profiled fragments of the program
#define	L1_OUT(s)			printf("%s : 100%%\n",_TEXT(s));

//		The Lx_OUT macro displays the s and val/base ration on the screen
#define	Lx_OUT(s,base,val)	printf("%s : %3.1f%%\n",\
								_TEXT(s),(float)val/(float)base*100);

//		The A1_OUT macro does the same thing as L1_OUT
#define	A1_OUT(s)			L1_OUT(s)

//		The A1_OUT macro outputs the s string and execution time of the program fragment
// labeled by the val  checkpoint in relation to the exeution time of the fragment labeled
// by the base checkpoint.
#define	Ax_OUT(s,base,val)	Lx_OUT(s, Ax_GET(base), Ax_GET(val))


//		The A_LIST_ITER prints all results for measurement of the 
// t checkpoint
#define A_LIST_ITER(t)		printf("LIST_ITER {\n");						\
							for(DoCPU_tmp=0;DoCPU_tmp<A_NITER;DoCPU_tmp++)	\
								printf("\t%d\n",DoCPU_buff[t][DoCPU_tmp]);	\
							printf("} LIST_ITER\n");


//		The  A_LIST_ITER_AS_TABLE  prints all measurment results for the
// program fragment labeled by the t checkpoint and generates
// a table suitable for importing into MS Graph
#define A_LIST_ITER_AS_TABLE(s,buf)											\
							printf(s);										\
							for(DoCPU_tmp=0;DoCPU_tmp<A_NITER;DoCPU_tmp++)	\
								printf("\t%d",buf[DoCPU_tmp]);				\
								printf("\n");
/*
// #define	Lx_OUTx(s,buf_base,buf_val,x_factor)\
// printf("%s : %d%%\n",s,100*(cycle_mid(DoCPU_buff[buf_val],0)/x_factor)/
// (cycle_mid(DoCPU_buff[buf_base],0)/x_factor));
// #define	Lx_OUT_DEBUG(s,buf_base,buf_val) printf("%s : %d%%, %d%%\n",s,cycle_mid(DoCPU_buff[buf_val],0),cycle_mid(DoCPU_buff[buf_base],0));
*/


// MACROS FOR DISPLAYING THE PROGRESS
// ======================================

//		Macro displaying the rotating "propeller"
#define VVV	if (DoCPU_vcp >= strlen(DoCPU_v))	DoCPU_vcp = 0;				\
			sprintf(DoCPU_s,"\rDoCPU:%c\r",DoCPU_v[DoCPU_vcp]);				\
			PRINT(DoCPU_s); DoCPU_vcp++;

//		Macro displaying the current progress in percents
#define PRINT_PROGRESS(x)	sprintf(DoCPU_s,"\rProgress : %3d%%\r",(x));	\
							PRINT(DoCPU_s);


// FUNCTION FOR DISPLAYING DIAGRAMS "ON THE fLY"
// ---------------------------------------------
// ARG:
//		size	-	size of the diagram bar
//
//		n_sym	-	number of characters within the string (NULL == 80)
//
//		max		-	maximum value of size.
//					==  0 : max is equal to the doubled value of the first size
//					== -1 : resetting the previous value of form_factor and returning
//
//		nl		-	==  0 : proportional scale
//					==  1 : logarithmic scale
void online_graph(int size, int n_sym, int max, int nl)
{
	int		a;
	int		form_factor;
	static	graph_size = 0;

	// resetting graph_size to zero
	if (max == -1) { graph_size = 0; return; }

	// calculating graph_size by the first size value
	if ((!max) && (!graph_size)) graph_size = size * 2;

	// calculating the diagram length
	if (!n_sym) n_sym = MAX_GRAPH_LEN;

	// calculating the form_factor
	if (!max) form_factor = graph_size / n_sym;
		else
	form_factor = max / n_sym;

	// special processing for logarithmic string
	if (nl)
	{
		size = 10*log(size);
		form_factor=1;
	}

	if (!form_factor) form_factor = 1;

	size /= form_factor;
	// displaying the diagram
	for (a = 0; a < size; a++)
	{
		if (a >= (n_sym-3))
		{									// diagram limits are exceeded
			if (size > (n_sym-3)*3) printf(">");
			if (size > (n_sym-3)*2)	printf(">");
			if (size > (n_sym-3)*1)	printf(">");
			
			
			break;
		}
		printf(GRAPH_CHAR);
	}
	printf("\n");
}

//		Macro for quick call to online_graph with default arguments
#define GRAPH(a) online_graph(a,0,0,0);

//		Macros for initializing online_graph
#define RE_GRAPH online_graph(0,0,-1,0);


/*----------------------------------------------------------------------------
 *
 *										INPUT
 *
----------------------------------------------------------------------------*/

// reading memory area. Is it needed?
#define A_READ(p, size_p)	for(DoCPU_tmp=0; DoCPU_tmp < size_p; DoCPU_tmp += sizeof(int))\
													DoCPU_tmp1+=((int *)(p+DoCPU_tmp))[0];




/*----------------------------------------------------------------------------

						FUNCTIONS FOR WORKING WITH MEMORY AND CACHE

----------------------------------------------------------------------------*/

//		The _malloc32 macro allocates x bytes of memory and guarantees that the allocated block
// will start from the address that is a multiple of 32
#define	_malloc32(x)	(( (((int) malloc((x)+0x20)) + 0x20) & 0xFFFFFFE0  ));


//		The _align32 macro aligns the p pointer by address that is a multiple of 32 bytes
#define	_align32(p)		(( (((int) (p)) + 0x20) & 0xFFFFFFE0  ));


//		The CLEAR_L2_CACHE  macro fill L2 cache with garbage, thus
// flushing the data that was written into it earlier.
int CLEAR_L2_CACHE()
{
	int a;
	int x = 0;
	
	if (!p_cache) p_cache = (int *) malloc(_NORMAL);

	for (a = 0; a<_NORMAL; a++)
		x += *(int *)((int)p_cache+a);
	
	return x;
}



int CLEAR_ALL()
{
		// !!!UNDER CONSTRUCTION!!!
		return 0;
}


//		This macro flushes store buffers on P-III 
void _DoCPU_a_fflush()
{
#ifdef __FLUSH__
	_DoCPU_a_flush();
#endif
}



#define RM 	for(DoCPU_tmp = 0; DoCPU_tmp < 10; DoCPU_tmp++) DoCPU_float_tmp+=0.99;



/*----------------------------------------------------------------------------

								SERVICE FUNCTIONS

----------------------------------------------------------------------------*/

// * FUNC: int getargv(char *arg_name, char *arg_val)
// * Function for checking if the argument keys are present and received
//
//		If the arg_name argument is specified in the command line,
// the function returns non-negative value;
//
//		If the arg_name argument is missing from the command line
// (or, if its length exceeds MAX_STR_LEN), the function returns -1;
//
//		If the argument has a key,   separated from it by a colon
// (something like "/XXX:666"), its string value is returned in arg_val
// provided that arg_val is not equal to zero;
//
//		If  arg_val==NULL this means that the argument's key is not needed;
//
//		If the key is present, the function also attempts to return its numeric
// value (received from atol);
//
//		NOTE: If the function returns 0, the situation is ambiguous: either
// the argument has no key at all, or the key is not a numeric value, or the key
// is actually equal to zero. If this is the case, check the contents of 
// arg_val to clarify the situation
int getargv(char *arg_name, char *arg_val)
{
	int				a;
	int*			p;
	unsigned int	c;
	
	char			buf_arg[MAX_STR_LEN];
	char			buf_val[MAX_STR_LEN];

	#ifndef __argv		
		char** __argv;
		int __argc=0;
	#endif

	if (!arg_name) return -1;

	if (arg_val) arg_val[0] = 0;	// Initialization

	// Testing all arguments
	for (a = 1; a < __argc; a++)
	{
		if (strlen(__argv[a]) >= MAX_STR_LEN) return -1;

		// Parsing the argument syntax and copying its name into buf_arg
		for (c=0;c!=(1+strlen(__argv[a]));c++)
		{
			if (__argv[a][c] == ':') { buf_arg[c] = 0; break; }
				buf_arg[c] = __argv[a][c];
		}
		
		// Copying the key (if there is any) into buf_val
		if (__argv[a][c] == ':') strcpy(buf_val, &__argv[a][c +1]); else buf_val[0] = 0;

		// If this is the argument that is needed, then copy its value into arg_val
		if (!strcmp(arg_name,buf_arg))
		{
			if (arg_val) strcpy(arg_val,buf_val);
			return atol(buf_val);
		}
	}
	return -1;
}

//		The IFHELP macro displays the s string provided that the "/?" 
// command-line option is specified, and terminates the program execution
#define	IFHELP(s)	if (getargv("/?",0)!=-1){ PRINT(_TEXT(s)); return 0; }

//		The  GETARGV   is the "wrapper" for the getargv function.
// If the s  key is present in the command line, it stores the key value into the
// val variable. If the s key is not present in the command line, the val variable
// does not change.
#define	GETARGV(s,val)	DoCPU_tmp=getargv(s,NULL);							\
						if (DoCPU_tmp!=-1) val=DoCPU_tmp;

//#define A_FLUSH _DoCPU_a_flush();
//#define A_FFLUSH _DoCPU_a_fflush();
//#define A_WAIT  Sleep(TIME_TO_WAIT);


/*----------------------------------------------------------------------------
 *
 *						PROFILER KERNEL FUNCTIONS
 *
----------------------------------------------------------------------------*/
int*	A0(void* x)
{
		Sleep(100);
		return DoCPU(x);
}


/*----------------------------------------------------------------------------
 *
 *						SERVICE FUNCTIONS OF THE PROFILER KERNEL
 *
----------------------------------------------------------------------------*/
// This function returns the clock frequency of the processor in MHz
int GetCPUclock(void)
{
	#define to_time 1

	int				tmp;
	unsigned int	t;
	volatile		time_t timer;

	tmp = getargv("$Fcpu",0);

	if (tmp <= 0)
	{
		if (!(tmp=CPU_CLOCK))
		{
			timer = time(NULL);
			while(timer == time(NULL));
			timer = time(NULL);

			A1(&t);
				while( timer+to_time > time(NULL));
			t=A2(&t);

			tmp=((double)t) / ((double)1000000) / ((double)to_time);

			CPU_CLOCK=tmp;
		}
	}
	
	if (getargv("$DEBUG.print.Fcpu",0)!=-1) printf(">DEBUG:Fcpu=%d\n",tmp);
	return tmp;
}

// This function converts clocks into seconds
float cpu2timeu(unsigned int ticks, int tt)
{
	if (tt)
		return ((float )ticks)/CLOCKS_PER_SEC;

	if (!CPU_CLOCK) CPU_CLOCK=GetCPUclock();
		return ((float )ticks)/CPU_CLOCK/1000000;
}


#ifdef _NORDTSC
	float cpu2time(int ticks)
	{
		return cpu2timeu(ticks, 0);
		
	}
#else
	float cpu2time(int ticks)
	{
		return cpu2timeu(ticks, 1);
	}
#endif


/*----------------------------------------------------------------------------
 *
 *						KERNEL FUNCTIONS FOR PROCESSING THE RESULTS
 *
----------------------------------------------------------------------------*/
int cycle_min(int *buff, int n_buff)
{
	int a, tmp;
	if (!n_buff)	n_buff = DoCPU_BUFF_SIZE;

	tmp = buff[0];
	for(a = 0; a < n_buff; a++)
		if (buff[a] < tmp) tmp = buff[a];
	return tmp;
}

int cycle_max(int *buff, int n_buff)
{
	int a, tmp;
	if (!n_buff)	n_buff = DoCPU_BUFF_SIZE;
	
	tmp = buff[0];
	for(a = 0; a < n_buff; a++)
		if (buff[a] > tmp) tmp = buff[a];

	return tmp;
}


	int _compare(unsigned int *arg1, unsigned int *arg2 )
	{
		if (*arg2 >= *arg1) return -1;
		// else 
		return +1;
	}

unsigned int cycle_mid(unsigned int *buff, int nbuff)
{
	int a, xa = 0;
	if (!nbuff) nbuff = A_NITER;

	// Исключаем первый элемент
	buff = buff + 1; nbuff--;

	if (getargv("$NoSort",0) == -1)
		qsort(buff, nbuff, sizeof(int),\
				(int (*)(const void *,const void*))(_compare));

	for (a = nbuff/3; a < (2*nbuff/3); a++)
		xa += buff[a];

	xa /= (nbuff/3);

	return xa;
}


	int _Search(int *array, int array_size, int val)
	{
		int a;

		for (a = 0; a < array_size; a++)
		{
			if (array[a] == val) return a;
		}
	return -1;
	}

int cycle_mod(int *buff, int n_buff)
{
	int a,b,tmp;
	int *val_array;
	int *cnt_array;
	int array_size = 0;

	if (!n_buff) n_buff=DoCPU_BUFF_SIZE;

	val_array = (int*) malloc(n_buff*sizeof(int));
	cnt_array = (int*) malloc(n_buff*sizeof(int));

	// Accumulating statistics 
	for (a=0;a<n_buff;a++)
	{
		tmp=_Search(val_array, array_size, buff[a]);
		if (tmp==-1) {array_size++;val_array[array_size-1]=buff[a];cnt_array[array_size-1]=1;}
			else
		{ cnt_array[tmp]+=1; }
	}

	// Searching for the most popular element
	tmp=cnt_array[0];b=0;
	for (a=1;a<array_size;a++)
	{
		if (cnt_array[a]>tmp) {tmp=cnt_array[a]; b=a;}
	}

	tmp = val_array[b];
	free(val_array);
	free(cnt_array);
	return tmp;
}


float cycle_average(int* buff, int n_buff)
{
	int a;
	float avrg = 0;

	if (!n_buff) n_buff=DoCPU_BUFF_SIZE;

	for (a = 0; a < n_buff; a++)
		avrg += buff[a];

	return avrg/n_buff;
}


