;	// TEST PROGRAM FOR MEASURING EXECUTION TIME
;	// OF INDIVIDUAL MACHINE COMMANDS (OR GROUPS OF COMMANDS)
;	// place here machine code for which execution time 
;	// will be measured using the  RDTSC instruction
;	// all serialization is already accomplished in DoCPU.asm,
;	// therefore you do not need to worry about it here

	MOV		ECX,1000					; _ecx = 1000;
										; // loop counter

@rool:								; do {
	; MUL	EAX							; // multiplication command 
									; // uncomment the previous line
									; // if you need to find out how many CPU clocks
									; // are required for its execution

	REPT	4							; // macro for cyclic repetition
										; // shows how many times to spawn
		INC	EAX							; // the INC EAX command
	ENDM								; // you can experiment with this 
										; // coefficient

	; REPT 4
	; 		FDIV						; // floating-point division
	; ENDM


DEC	ECX								; _ecx--;
JNZ @rool							; } while(_ecx>0);

;	//
;	// Q: HOW TO CALCULATE THE EXECUTION TIME OF AN INDIVIDUAL MACHINE COMMAND?
;	// A: Divide averaged execution time of this loop by the number of
;	// 	  loop iterations and number of commands
;	//