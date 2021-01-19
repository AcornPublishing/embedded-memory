CODE_SIZE EQU 1024 					
; CODE_SIZE EQU	?		; // The CODE_SIZE macro is automaticall generated
						; // by batch translator file

; /*--------------------------------------------------------------------------
;  *
;  * 				MACRO GENERATING N NOP machine commands
;  *				-----------------------------------------
;  *
;  *						(the NOP command actually means "no operation"
;  * 						 and takes exactly one byte, i.e. N NOP commands
;  *						 produce N bytes of executable code)
;  *
;  -------------------------------------------------------------------------*/
NOPING MACRO N			; // MACRO START //
	_N = N				; _N := N (getting the argument passed to the macro)
	_A = 0				; _A -- loop counter variable

	WHILE _A NE _N		; while(_A <= _N){
		NOP				; inserting NOPs into the source code
		_A = _A + 1		; _A++;
	ENDM				; }
ENDM					; // MACRO END //

; /*--------------------------------------------------------------------------
;  * 
;  * 		CALLING THE MACRO FOR CREATING A BLOCK WITH THE SIZE EQUAL TO CODE_SIZE KB of NOP commands
;  *		-----------------------------------------------------------
;  *
; --------------------------------------------------------------------------*/
NOPING CODE_SIZE*1024



