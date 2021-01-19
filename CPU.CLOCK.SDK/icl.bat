@ECHO OFF

SETLOCAL

REM Initial parsing of command-line arguments
REM =========================================
	IF #%1#==##						GOTO usage
	IF #%1#==#__typeinfo#			GOTO usage
	IF #%1#==#__class#				GOTO end
	IF #%1#==#/?# 					GOTO help

REM	Initializing variables
REM ==========================
	SET OBJ=
	SET OOPS=
	SET LOGO=
	SET	WL=
	SET WX=
	SET MYLIBS=
	SET MACRO=
	SET C_AS_CPP=
	SET	VER_USE=


REM Parsing command-line arguments
REM ==============================
GOTO first_parse

:PARSE
	SHIFT

:first_parse
	IF #%1#==#--X#				GOTO set_ver_use
	IF #%1#==#--I# 				GOTO add_include_path
	IF #%1#==#--L# 				GOTO add_lib_path
	IF #%1#==#--o# 				GOTO add_obj
	IF #%1#==#--l#				GOTO add_lib

	IF #%1#==#-release# 		GOTO release
	IF #%1#==#-debug# 			GOTO debug

	IF #%1#==#-nologo# 			GOTO nologo

	IF #%1#==#--w!#				GOTO w_as_err
	IF #%1#==#--cpp#			GOTO c_as_cpp
	IF #%1#==#-macro#			GOTO define_macro

	IF NOT EXIST %1				GOTO err_arg

	GOTO						compil


:set_ver_use
	SHIF
	SET VER_USE=%1
GOTO PARSE	

:add_include_path
	SHIFT
	SET INCLUDE=%INCLUDE%;%1
GOTO PARSE

:add_lib_path
	SHIFT
	SET LIB=%LIB%;%1
GOTO PARSE

:add_obj
	SHIFT
	SET OBJ=%OBJ% %1
GOTO PARSE

:add_lib
	SHIFT
	SET MYLIBS=%MYLIBS% %1
GOTO PARSE

:release
	SET OOPS=/Ox
GOTO PARSE

:debug
	SET OOPS=/Zi
GOTO PARSE

:nologo
	SET LOGO=/nologo
GOTO PARSE

:w_as_err
	SET WL=/W4
	SET WX=/WX
GOTO PARSE

:c_as_cpp
	SET C_AS_CPP=/Tp
GOTO PARSE

:define_macro
	SHIFT
	SET	MACRO=%MACRO%%1
GOTO PARSE


:compil
	IF NOT #%MACRO%#==## SET MACRO=/D%MACRO%
	SET PATH="C:\Program Files\Intel\Compiler70\IA32\Bin";%PATH%
	SET LIB="C:\Program Files\Intel\Compiler70\IA32\Lib";%LIB%
	SET INCLUDE="C:\Program Files\Intel\Compiler70\IA32\INCLUDE";%INCLUDE%
	icl.exe %OOPS% %LOGO% %WL% %WX% %MACRO% %C_AS_CPP%%1 %OBJ% %2 %3 %4 %5 %6 %7 %8 %9 %MYLIBS%

	rem cl.exe 
GOTO end

:help
	cl.exe -?
GOTO END

:usage
	ECHO USAGE icl.bat [-key] file_name.c [options]
	ECHO.
	ECHO = = = KEY = = =
	ECHO --I path            - path to include files
	ECHO --L path            - path to libraries
	ECHO --l library.lib     - link library
	ECHO --o file.obj        - link obj-файл
	ECHO -release            - compiling with max. optimization
	ECHO -debug              - include debug info
	ECHO -nologo             - suppress logo
	ECHO --w!                - interpret warnings as errors
	ECHO --cpp               - interpret .c as .cpp
	ECHO.
	ECHO NOTE:
	ECHO	1) When it is necessary to link several libraries
	ECHO (or specify several paths) it is allowed to specify appropriate
	ECHO keys several times, for example: 
        ECHO cl.bat --l my.lib --l user.lib myfile.c
	ECHO	2) When specifying mutually exclusive options
        ECHO (for example,  -release & -debug),
	ECHO their cumulative effect is undefined
	ECHO.
	ECHO = = = file_name.c = = =
	ECHO 	In the current version only one c-file can be specified.
	ECHO If it is necessary to compile several files, they must be
	ECHO compiled separately and then linked as obj-files
	ECHO.
	ECHO = = = OPTIONS = = =
	ECHO	All options specified before the filename (to the left from it)
	ECHO it passed to the compiler AS IS.

GOTO end

:err_arg
	ECHO -ERR: Unknown command-line argument: "%1"
GOTO end

:end
ENDLOCAL
SET __class__=compil


