@ECHO OFF
SETLOCAL
IF "%1"=="__class"    goto END


REM === debug
SET CLOCKSDK=F:\.optimize\bin\clock
REM === end_debug


CALL %CLOCKSDK%\TEMP\clock.clear.bat
DEL  %CLOCKSDK%\TEMP\clock.clear.bat
:END
ENDLOCAL
set errorlevel=1
