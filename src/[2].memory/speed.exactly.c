/* ---------------------------------------------------------------------------
 * @
 *		UTILITY FOR MEASURING ACTUAL MEMORY THROUGHPUT
 *					WITH ACCOUNT OF CHIPSET AND CPU LATENCY
 *		============================================================
 *
 * Build 0x001	03.04.2002
 * Build 0x002	16.05.2002
--------------------------------------------------------------------------- */

#include <DoCPU.h>

// converting float to int
#define _f2i(a) ((int) a)

// calculating the remainder of the division of larger argument by a smaller one
#define remainder(a,b) (MAX(_f2i(a),_f2i(b)) % MIN(_f2i(a),_f2i(b)))

// data input
#define _INPUT(s, var) printf(_TEXT(s));							\
	fgets(buff, MAX_STR_LEN - 1, stdin);							\
	if (buff[0]=='y' || buff[0]=='Y') var=1; else					\
	if (buff[0]=='n' || buff[0]=='N') var=0; else var=atof(buff);	\
/* end define _INPUT */

main()
{
	float Fcpu;					// CPU clock frequency
	float FSB;					// system bus frequency
	float Ftransf;				// effective system bus frequency
	float Fm;					// memory clock frequency	

	float RAS_latency;			// RAS to CAS Delay
	float CAS_latency;			// CAS Delay
	float RAS_precharge;		// RAS Precharge Delay
	float LEN_page;				// DRAM page length
	float N;					// memory width

	float BRST_LEN;				// length of the FSB packet
	float BRST_NUM;				// number of DRAM packets in one FSB packet

	float CPU_latency;			// CPU latency
	float X_CACHE;				/* relation of the processor frequency 
								^^ to L2 cache frequency */
	
	float C;					// throughput

	int fInterleaving;			// are banks interleaving?

	int fSrl;					/* 0: parallel data transfer from MCT to BIU
								^^ 1: data transfer via intermediate buffer */

	int fMCT2BIUparallel;		/* 0: sequential data transfer from buffer to BIU
								^^ 1: parallel data transfer */

	int fImmediately;			/* 0: cell is available after completing the transaction
								^^ 1: cell is available immediately */

	int Chipset_penalty = 0;	// penalty for coordinating FSB and memory frequencies
	int CPU_penalty 	= 0;	// penalty for coordinating CPU and FSB frequencies

	char buff[MAX_STR_LEN];		// buffer for strings

	// TITLE
	PRINT("=== Calculation of memory throughput with account of chipset latency ===\n");
	PRINT_TITLE; printf(_TEXT("\n-------------- enter --------------\n"));

	// data input
	_INPUT("CPU frequency     ..... (MHz)?\t", Fcpu);
	_INPUT("System bus frequency... (MHz)?\t", FSB);
	_INPUT("Data transfer rate..... (MHz)?\t", Ftransf);
	_INPUT("Memory frequency....... (MHz)?\t", Fm);
	_INPUT("RAS to CAS latency..... (CLK)?\t", RAS_latency);
	_INPUT("CAS latency............ (CLK)?\t", CAS_latency);
	_INPUT("RAS precharge.......... (CLK)?\t", RAS_precharge);
	_INPUT("DRAM page length....... (KB )?\t", LEN_page);
	_INPUT("Memory width........... (bit)?\t", N);
	_INPUT("Lenth of bus packet.... (num)?\t", BRST_LEN);
	_INPUT("Length of DRAM packet.. (num)?\t", BRST_NUM);
	_INPUT("CPU latency............ (CLK)?\t", CPU_latency);
	_INPUT("CPU/L2 CACHE............(X/x)?\t", X_CACHE);
	_INPUT("ÏSequential buffer......[y,n]?\t", fSrl);
	_INPUT("Bank interleave..â..... [y,n]?\t", fInterleaving);
	_INPUT("MCT2BIU parallel  mode  [y,n]?\t", fMCT2BIUparallel);
	_INPUT("Immediately-cache mode  [y,n]?\t", fImmediately);
	
	// checking input for correctness
	if (!BRST_NUM || !BRST_LEN) ERROR("packet length is specified incorrectly\x7")
		else BRST_NUM = BRST_LEN / BRST_NUM;

	if (!FSB || !Fm || !Fcpu)
			ERROR("-ERR: frequency cannot be equal to zero!\x7\n")

	if (!LEN_page)
			ERROR("-ERR: page length cannot be equal to zero!\x7\n")

	/* end of check for input correctness */

	// calculating penalty
	if (remainder(FSB,Fm)	!=0)	Chipset_penalty = 1;
	if (remainder(FSB,Fcpu)	!=0)	CPU_penalty 	= 1;


	// converting MHz to millions of Hz
	FSB*=Mega; Fm*=Mega; Fcpu*=Mega; Ftransf*=Mega; N/=Bit;


	//						CALCULATING THE THROUGHPUT
	// ------------------------------------------------------------------------
	C = (N /* memory width, bytes */ * BRST_LEN /* packet length, iter */)/
		(
		  2/FSB						/* àarbitration 							*/
		+ 1/FSB						/* cell address transfer				*/
		+ 1/FSB						/* passing transaction ID	*/

		+ 1/FSB						/* BIU latency						*/

		+ 1/FSB						/* decoding MCT address of the cell		*/

		+ 1/FSB						/* MCT latency						*/

		+ Chipset_penalty/Fm		/* penalty for coordination of Mem/FSB frequencies	*/

		+ BRST_NUM*CAS_latency/Fm	/* CAS Delay							*/

		+ (fSrl?BRST_LEN/Fm:1/Fm)	/* data transfer from DRAM to BUFF/BIU	*/

		+ Chipset_penalty/FSB		/* penatly for frequency coordination		*/

		+ (fMCT2BIUparallel?BRST_LEN/FSB:1/FSB)
									/* data transfer from BUFF to BIU		*/

		+ 1/FSB						/* BIU latence						*/

		+ (fImmediately?1/FSB:BRST_LEN/Ftransf)
									/* data transfer from BIU to CPU			*/

		+ CPU_latency/Fcpu			/* ÑPU latency						*/

		+ X_CACHE*BRST_LEN/Fcpu		/* data transfer from L2 to L1			*/

		+ CPU_penalty/Fcpu			/* penalty for CPU/FSB	frequencies coordination */

		+ RAS_latency/((LEN_page*K/((N*BRST_LEN)))*Fm)
									/* delay for opening memory page	*/

		+ (fInterleaving?0:RAS_precharge/Fm)
									/* bank precharge delay			*/
		);
		
	C /= M;							// Convert millions of bytes to Megabytes

	// displaying the results
	printf("-------------------------------------\n%s =\t%4.2f\n",
		_TEXT("Throughput (MB/sec)"),C);

	return 0;
}

