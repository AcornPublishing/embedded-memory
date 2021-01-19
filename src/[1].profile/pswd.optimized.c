/*----------------------------------------------------------------------------
 *
 *				THE EXAMPLE DEMONSTRATING OPTIMIZATION TECHNIQUE
 *                  and profiling using Intel VTune profiler
 *              =================================================
 *
 * Build 0x001 12.08.2002
----------------------------------------------------------------------------*/
// STEP 1
//			Call printf to display progress after 6666 iterations

// STEP 2
//			Move strlen outside the loop body in the CalculateCRC function

//STEP 3
//			Align pswd

// STEP 4
//			Calculate the length within gen_pswd

// STEP 5
//			Remove progress calculation from gen_pswd

// STEP 6
//			Remove display of the current status

// STEP 7
//			Inline functions

// CONFIGURATION
#define ITER 40000000						// max number of iterations
#define MAX_CRYPT_LEN	200					// max. length of the cyphertext

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>

int length;
/*----------------------------------------------------------------------------
 *
 *				procedure of decrypting the cyphertext with the cracked password
 *				---------------------------------------------------
 *	ARG:
 *		pswd		:	passwrod
 *		crypteddata	:	cyphertext
 *
 *	RET:
 *		*crypreddata:	decrypted cyphertext
 *
 *	NOTE:
 *		none
-----------------------------------------------------------------------------*/
DeCrypt(char *pswd, char *crypteddata)
{

	unsigned int p = 0;		// pointer to the current position within the data to be decrypted

	// * * * MAIN DECRYPTION LOOP * * *
	do {
		// decrypting the current character
		crypteddata[p] ^= pswd[p % strlen(pswd)];
		// attention: taking the remainder ^^^ is  _very_ slow operation
		// nevertheless, we can safely use it here,
		// since this function is called very rarely (if it is called
		// at all)

		// proceeding with decryption of the next character
	} while(++p < strlen(crypteddata));
}

int gen_pswd(char *crypteddata, char *pswd, int max_iter, int validCRC)
{
	int a, b, x;
	int p = 0;
	char *buff;
	int y=0;
	int k;
	int length = strlen(pswd);

	x = -1;
	for (b = 0; b <= length;  b++)
		x += *(int *)((int)pswd + b);

	// generate passwords
	for(a = 0; a <  max_iter ; a++)
	{

		if (x==validCRC)
		{
		// copy the encrypted data into temporary buffer
		buff = (char *) malloc(strlen(crypteddata));
		strcpy(buff, crypteddata);

		// decrypt
		DeCrypt(pswd, buff);

			// display the results on screen
			printf("CRC %8X: try to decrypt: \"%s\"\n", validCRC,buff);
		}
		y = 1;
		k = 0x59;
		while((++pswd[p])>'z')
		{
			pswd[p] = '!';
			// next character
			y = y | y << 8;
			x -= k;
			k = k << 8;
			k += 0x59;

			p++;
			if (!pswd[p])
			{
					pswd[p]='!';
					pswd[p+1]=0;
					length++;
						x = -1;
						for (b = 0; b <= length;  b++)
						x += *(int *)((int)pswd + b);
					pswd[p]=' ';
						y = 0;
			}
			//printf("%x\n",y);
		} // end while(pswd)
		p = 0;
		x+=y;
	} // end for(a)

	return 0;
}

/*----------------------------------------------------------------------------
 *
 *				This function displays the number by separating positions with dots
 *				------------------------------------------------
 *	ARG:
 *		per			:	number for output
 *
 *	RET:
 *		none
 *
 *	NOTE:
 *		The function displays the number truncated to its integer part
 *
-----------------------------------------------------------------------------*/
print_dot(float per)
{
	// * * * CONFIGURATION * * *
	#define N			3		// separate by three digits
								// when displaying HEX separate by two digits

	#define DOT_SIZE	1		// dot size

	#define	DOT			"."		// separator

	int		a;
	char	buff[666];

	sprintf(buff,"%0.0f", per);
	/* ^^^^^^^^^^^^^^^^ output format */

	// * * * loop for parsing the number by digits * * *
	for(a = strlen(buff) - N; a > 0; a -= N)		// <-- displacement
	{ /* ^^^^^^^^^^^^^^^^ - this is a silly code - do not call this function frequently */

			memmove(buff + a + DOT_SIZE, buff + a, 66);
			/* attention!						^^^^^^^^^ */

			if(buff[a]==' ') break;	// blank character is reached - end of work
				else
			// copy the separator
			memcpy(buff + a, DOT, DOT_SIZE);
	}
	// output the result
	printf("%s\n",buff);
}


main(int argc, char **argv)
{
	// variables
	FILE *f;				// for reading the source file (if there is any)
	char *buff;				// for reading data from the source file
	char *pswd;				// password currently bein tested (needed by gen_pswd)
	int validCRC;			// for storing CRC of original password
	unsigned int t;			// for measuring cracking time
	int iter = ITER;		// max number of passwords to be tested
	char *crypteddata;		// for storing encrypted data

    //  built-in default crypt
	//	The one who manages to read what is encrypted here will learn the great secret
	//	Kris Kaspersky ;)
	char _DATA_[] = "\x4B\x72\x69\x73\x20\x4B\x61\x73\x70\x65\x72\x73\x6B"\
					"\x79\x20\x44\x65\x6D\x6F\x20\x43\x72\x79\x70\x74\x3A"\
					"\xB9\x50\xE7\x73\x20\x39\x3D\x30\x4B\x42\x53\x3E\x22"\
					"\x27\x32\x53\x56\x49\x3F\x3C\x3D\x2C\x73\x73\x0D\x0A";

	// TITLE
	printf("= = = VTune profiling demo = = =\n==================================\n");

	// HELP
	if (argc==2)
	{
			printf("USAGE:\n\tpswd.exe [StartPassword MAX_ITER]\n");
			return 0;
	}

	// allocating memory
	printf("memory malloc\t\t");
	buff = (char *) malloc(MAX_CRYPT_LEN);
	if (buff) printf("+OK\n"); else {printf("-ERR\n"); return -1;}

	// getting cyphertext for decryption
	printf("get source from\t\t");
	if ((f=fopen("crypted.dat","r"))!=0)
	{
		printf("crypted.dat\n");
		fgets(buff,MAX_CRYPT_LEN, f);
	}
	else
	{
		printf("build-in data\n");
		buff=_DATA_;
	}

	// calculating CRC
	validCRC=*(int *)((int) strstr(buff,":")+1);
	printf("calculate CRC\t\t%X\n",validCRC);
	if (!validCRC)
	{
		printf("-ERR: CRC is invalid\n");
		return -1;
	}

	// separating encrypted data
	crypteddata=strstr(buff,":") + 5;
	//printf("cryptodata\t\t%s\n",crypteddata);

	// allocating memory for password buffer
	printf("memory malloc\t\t");
	pswd = (char *) malloc(512*1024);
	//pswd+=62;

	memset(pswd,0,666);		// <-- initalization

	if (pswd) printf("+OK\n"); else {printf("-ERR\n"); return -1;}

	// parsing command line arguments
	// getting the starting password and max. number of iterations
	printf("get arg from\t\t");
	if (argc>2)
	{
		printf("command line\n");
		if(atol(argv[2])>0) iter=atol(argv[2]);
		strcpy(pswd,argv[1]);
	}
		else
	{
		printf("build-in default\n");
		strcpy(pswd,"!");
	}
	printf("start password\t\t%s\nmax iter\t\t%d\n",pswd,iter);


	// starting password cracking
	printf("==================================\ntry search... wait!\n");
	t=clock();
		gen_pswd(crypteddata,pswd,iter,validCRC);
	t=clock()-t;

	// displaying the number of passwords tested per second
	printf("                                       \rPassword per sec:\t");
	print_dot(iter/(float)t*CLOCKS_PER_SEC);
	//printf("%d\n",t);

	return 0;
}
