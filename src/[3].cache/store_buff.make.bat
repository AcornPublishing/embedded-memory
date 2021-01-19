@ECHO OFF
IF #%1#==#MAKE_FOR# GOTO make_it

REM MAKE ALL
ECHO = = = СБОРКА ПРМЕРА, ДЕМОНСТРУЮЩЕГО ВЫГРУЗКУ БУФЕРОВ ЗАПИСИ = = =
ECHO Утилита к книге \"Техника  оптимизации  програм\"  /* название рабочее */
ECHO @ECHO OFF 																		> store_buff.run.bat
ECHO ECHO = = = демонстрация выгрузки буферов записи = = = 							>> store_buff.run.bat
ECHO ECHO Утилита к книге \"Техника  оптимизации  програм\"  /* название рабочее */	>> store_buff.run.bat
ECHO ECHO N STORE                       ...CLOCK...                                 >> store_buff.run.bat
ECHO ECHO ------------------------------------------------------------------------- >> store_buff.run.bat

FOR %%A IN (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20) DO CALL  %0 MAKE_FOR %%A
ECHO DEL %%0 																		>> store_buff.run.bat

GOTO end

:make_it
ECHO /%0/%1/%2 *
SHIFT
ECHO	N_ITER EQU %1 					> store_buff.mod
ECHO	#define N_ITER %1 				> store_buff.h
TYPE	store_buff.xm 					>> store_buff.mod
CALL	clock.make.bat    store_buff.c	> NUL
DEL		store_buff.mod
DEL		store_buff.h
IF 		NOT EXIST store_buff.exe GOTO err
IF 		EXIST store_buff.%1.exe DEL store_buff.%1.exe
REN  	store_buff.exe store_buff.%1.exe
ECHO 	store_buff.%1.exe 				>> store_buff.run.bat
ECHO 	DEL store_buff.%1.exe 			>> store_buff.run.bat
GOTO 	end

:err
ECHO -ERR ошибка компиляции! подробнее см. store_buff.err 
TYPE store_buff.err
EXIT
:end


