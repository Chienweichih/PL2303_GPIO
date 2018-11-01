@echo off

set COMPORT=14
set DISKNUM=2

for /f "delims=:. tokens=1-4" %%t in ("%TIME: =0%") do (
   set OUTPUT=%TEMP%\QSI-LOG-%%t%%u%%v%%w
)

"PL-2303 4 GPIOTest".exe %COMPORT% %DISKNUM% > %OUTPUT%

type %OUTPUT%

setlocal EnableDelayedExpansion
for /F "delims=" %%a in (%OUTPUT%) do (
   set "lastline=%%a"
   for %%b in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do (
      set "lastline=!lastline:%%b=%%b!"
   )
)

del %OUTPUT%

echo %lastline%|find "FAIL" >nul
if errorlevel 1 (EXIT /B 0) else (EXIT /B 1)
