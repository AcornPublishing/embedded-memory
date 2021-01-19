; N_ITER EQU	?	;// <-- !auto gen!

; /*--------------------------------------------------------------------------
;  *
;  *			 macro that automatically duplicates its body N times
;  *
; ---------------------------------------------------------------------------*/
STORE_BUFF MACRO N
	_N = N			
	_A = 0			 
	WHILE _A NE _N	 
		NOP				; 			<-- MACRO BODY		 
		_A = _A + 1  
	ENDM
ENDM


; /*--------------------------------------------------------------------------
;  *
;  *		DEMONSTRATION OF BUFFER UNLOADING DURING BUS IDLE TIME
;  *
; ---------------------------------------------------------------------------*/
	MOV	[ESI],	ECX		; *p = a;	<-	here we write some value into *p
						;			<-	first of all, this value
						;			<- 	goes to the store buffer

	STORE_BUFF	N_ITER	; ...       <-	one or more NOPs
						; ...		 	in parallel with NOP execution
						; ...   		buffer contents is unloaded into L1 (AMD)
						; ...			or L2 (Intel) cache

	MOV	EDX,	[ESI]	; b=*p;		<-	reading the contents of the *p cell
						;				if by that moment the buffer corresponding to it
						;				us not unloaded, it will be read as fast as
						;				possible;  otherwise the delay would occur	
						
	ADD	ESI,	32*20	; (int)p+32 <-	move the pointer to the next buffer	

