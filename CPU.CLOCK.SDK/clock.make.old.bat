@ECHO OFF
REM ================================ Compiling utility =======================
REM 1) INSTALLATION NOTES
REM  - The CLOCKSDK environment variable must set the path to CLOCKSDK
REM    installation directory _without_ trailing slash
REM ===========================================================================
SETLOCAL

IF #%1#==#__class#    goto END
IF #%1#==#__typeinfo# goto USAGE
IF #%1#==#/?# GOTO HELP

ECHO * CLOCK PROJECT MAKER * by Kris Kaspersky Version 1.0 beta AT 23-07-2001
ECHO (utility for the "Code Optimization: Effective Memory Usage" book)


IF "%CLOCKSDK%"=="" SET CLOCKSDK=F:\.optimize\bin\clock
REM The default path to CLOCK SDK^^^^^^^^^^^^^^^^^^^^^^

IF "%CLOCKSDK%"=="" GOTO ERR_NO_CLOCKSDK

SET COMPIL=MSVC
SET COMPIL_NAME=Microsoft Visual C++
GOTO P1

:PARSE 
REM /*             Parsing command-line arguments                      */
REM =====================================================================
SHIFT

:P1
IF "%1"=="" GOTO USAGE

:P2
IF NOT "%1"=="-BC" GOTO P3
SET COMPIL=BC
SET COMPIL_NAME=Borland C++
GOTO PARSE

:P3
IF NOT "%1"=="-WPP" GOTO P4
SET COMPIL=WPP
SET COMPIL_NAME=WATCOM
GOTO PARSE

:P4
IF NOT "%1"=="-debug" GOTO P5
SET OPTIONS=DEBUG
GOTO PARSE

:P5
IF NOT "%1"=="-release" GOTO P6
SET OPTIONS=RELEASE
GOTO PARSE

:P6
IF NOT "%1"=="-nop" GOTO P7
GOTO PARSE

:P7
IF NOT "%1"=="-nocleartmp" GOTO P8
SET nocleartmp=YES
GOTO PARSE

:P8

REM /*                  Check for existence of the SDK files                 */
REM ===========================================================================
IF NOT EXIST %CLOCKSDK%\SOURCE\DoCPU.asm GOTO ERR_NO_DoCPU_ASM
IF NOT EXIST %CLOCKSDK%\INCLUDE\DoCPU.h GOTO ERR_NO_DoCPU_H


REM /*                   STARTING PROJECT BUILD                              */
REM ===========================================================================
ECHO =====  %~n1 project =====
IF NOT EXIST %~n1.c   GOTO ERR_C_NOT_EXIST

REM Displaying the list of project files
SET FILES_PRG=%~n1.c
IF EXIST %~n1.mod  SET FILES_PRG=%FILES_PRG%;%~n1.mod
IF EXIST %~n1.ini  SET FILES_PRG=%FILES_PRG%;%~n1.ini
SET FILES_PRG=%FILES_PRG%.
ECHO [0/3] Project files %FILES_PRG%

REM  /*                Preparing files for assembling                       */
REM ==========================================================================
COPY %CLOCKSDK%\SOURCE\DoCPU.asm %CLOCKSDK%\TEMP\ >NUL
IF EXIST %~n1.mod COPY %~n1.mod  %CLOCKSDK%\TEMP\ >NUL
IF EXIST %~n1.ini COPY %~n1.ini  %CLOCKSDK%\TEMP\ >NUL


IF EXIST %~n1.mod ECHO INCLUDE %~n1.mod > %CLOCKSDK%\TEMP\code.inc
IF NOT EXIST  %~n1.mod ECHO ; > %CLOCKSDK%\TEMP\code.inc

IF EXIST %~n1.ini ECHO INCLUDE %~n1.ini > %CLOCKSDK%\TEMP\code.ini
IF NOT EXIST  %~n1.ini ECHO ; > %CLOCKSDK%\TEMP\code.ini


REM /*                     Preparing the clock.clear.bat file                */
REM ===========================================================================
ECHO DEL %CLOCKSDK%\TEMP\code.inc   > %CLOCKSDK%\TEMP\clock.clear.bat
ECHO DEL %CLOCKSDK%\TEMP\code.ini  >> %CLOCKSDK%\TEMP\clock.clear.bat
ECHO DEL %CLOCKSDK%\TEMP\DoCPU.asm >> %CLOCKSDK%\TEMP\clock.clear.bat
IF EXIST %CLOCKSDK%\TEMP\%~n1.mod ECHO DEL %CLOCKSDK%\TEMP\%~n1.mod >> %CLOCKSDK%\TEMP\clock.clear.bat
IF EXIST %CLOCKSDK%\TEMP\%~n1.ini ECHO DEL %CLOCKSDK%\TEMP\%~n1.ini >> %CLOCKSDK%\TEMP\clock.clear.bat

ECHO IF EXIST DoCPU.obj DEL DoCPU.obj >> %CLOCKSDK%\TEMP\clock.clear.bat
ECHO IF EXIST %~n1.obj  DEL %~n1.obj  >> %CLOCKSDK%\TEMP\clock.clear.bat

rem ECHO IF EXIST %~n1.pdb DEL %~n1.pdb >> %CLOCKSDK%\TEMP\clock.clear.bat
rem ECHO IF EXIST vc??.pdb DEL vc??.pdb >> %CLOCKSDK%\TEMP\clock.clear.bat

REM  **********************   Assembling DoCPU ********************************
REM  ==========================================================================
ECHO [1/3] Assembling DoCPU.asm
IF  %COMPIL%==MSVC SET OOPS=/coff
IF "%OPTIONS%"=="DEBUG" SET OOPS=/Zi
ml.exe /WX %oops% /X /c /nologo %CLOCKSDK%\TEMP\DoCPU.asm > %~n1.err

IF ERRORLEVEL==1 GOTO err_link
IF NOT EXIST DoCPU.obj GOTO ERR_LINK
DEL %~n1.err

REM *************************   Compiling  **********************************
REM =========================================================================
ECHO [2/3] Compiling project "%~n1" by %compil_name% %2 %3 %4 %5 %6 %7
IF EXIST %~n1.exe DEL %~n1.exe

REM Determining the compiler to use
IF %COMPIL%==MSVC GOTO msvc
IF %COMPIL%==BC GOTO bc
IF %COMPIL%==WPP GOTO wpp
GOTO ERR_UNK_COMPIL

:MSVC
SET OOPS=
IF "%OPTIONS%"=="DEBUG" SET OOPS=/Zi
IF "%OPTIONS%"=="RELEASE" SET OOPS=/Ox
cl.exe %~n1.c %2 %3 %4 %5 %6 %oops% /WX /nologo /I%CLOCKSDK%\INCLUDE\   DoCPU.obj >%~n1.err
GOTO after_compil

:BC
call bcc32.bat -I%CLOCKSDK%\INCLUDE\ %~n1.c DoCPU.obj %2 %3 %4 %5 %6 -w!  >%~n1.err
GOTO after_compil

:WPP
call wpp.bat %~n1.c  %2 %3 %4 %5 %6 -I%CLOCKSDK%\INCLUDE; DoCPU.obj >%~n1.err
rem call wpp.bat %2 %3 %4 %5 %6 -IC:\SYS\; %~n1.c DoCPU.obj >%~n1.err
GOTO after_compil

:AFTER_COMPIL

REM /*                     Removing temporary files                    */
REM =====================================================================
IF NOT "%NOCLEARTMP%"=="YES" GOTO DEL_TEMP
ECHO [3/3] Temporary files were not removed because of -nocleartmp option


GOTO CHK_ERR

:DEL_TEMP
ECHO [3/3] Removing temporary files
CALL %CLOCKSDK%\TEMP\clock.clear.bat


:CHK_ERR
IF ERRORLEVEL==1 GOTO err_compil
IF NOT EXIST %~n1.exe GOTO ERR_COMPIL
DEL %~n1.err


ECHO [+OK] Compile successfully completed
ECHO ::The %~n1.exe file was created
IF "%oops%"=="/Zi" ECHO ::with debug information
IF "%opt%"=="/Ox" ECHO ::compiled with maximum optimization
GOTO END

:HELP
ECHO USAGE:
ECHO       clock.make.bat [-debug\-release\-nop\-BC\-WPP] modname [arg1,arg2,ar3]
ECHO       -debug      : generate debug info
ECHO       -release    : maximum optimize   
ECHO       -nocleartmp : do not remove temporary files
        
GOTO END

:USAGE
ECHO CLOCK PROJECT MAKER     [clock.make.bat $opt $filename $user_opt] 
GOTO END

:ERR_LINK
ECHO -ERR: Assembling error. See %~n1.err
GOTO END

:ERR_COMPIL
ECHO -ERR: Compile error. See %~n1.err
GOTO END


:ERR_C_NOT_EXIST
ECHO -ERR: The %1 main module does not exist
GOTO END

:ERR_NO_DoCPU_ASM
ECHO.
ECHO -ERR: The %CLOCKSDK%\SOURCE\DoCPU.ASM file is missing
ECHo Restore the file and try again!
GOTO END

:ERR_NO_DoCPU_H
ECHO.
ECHO -ERR: The %CLOCKSDK%\INCLUDE\DoCPU.h file is missing
ECHo Restore the file and try again!
GOTO END


:ERR_NO_CLOCKSDK
ECHO.
ECHO -ERR: The CLOCKSDK v1 environment variable not specified,
ECHO which sets the path to the CLOCKSDK installation directory
ECHO without trailing slash. Set this variable and try again...
GOTO END

:ERR_UNK_COMPIL
ECHO -ERR: Unknown compiler %compil%
GOTO END


ENDLOCAL
:END
set errorlevel=1
