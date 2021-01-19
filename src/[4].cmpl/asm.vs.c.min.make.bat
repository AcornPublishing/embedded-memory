@ECHO OFF
SET		prj_name=asm.vs.c.min
ECHO	= = = assembling %prj_name% project = = =

IF		EXIST %prj_name%.asm.obj 		DEL %prj_name%.asm.obj 
CALL	ml /c %prj_name%.asm.asm	 >	%prj_name%.asm.err

IF  	NOT EXIST %prj_name%.asm.obj	GOTO err_asm

CALL	clock.make %1 -release %prj_name%.c %prj_name%.asm.obj 

IF		EXIST %prj_name%.asm.err		DEL %prj_name%.asm.err
IF		EXIST %prj_name%.asm.obj		DEL %prj_name%.asm.obj
IF		NOT	EXIST %prj_name%.exe		GOTO end

ECHO ::%prj_name% project created successfully
ECHO ::run %prj_name%.exe to start it
GOTO	end

:err_asm
ECHO -ERR: assembling error! (see %prj_name%.asm.err)

:end
