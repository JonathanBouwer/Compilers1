@echo off
set FLAGS=/nologo /EHsc /Febin\main
if "%~1"=="run" goto run
if "%~1"=="debug" goto debug
if "%~1"=="clean" goto clean
if "%~1"=="output" goto output

cl src\main.cpp %FLAGS% %LIBS% 
goto :EOF

:run
bin\main %2 %3
goto :EOF

:debug
cl src\main.cpp %FLAGS% /Zi
goto :EOF

:output
chcp 65001
type output.txt
goto :EOF

:clean
del bin\*.exe bin\*.pdb bin\*.ilk *.pdb *.obj 2>nul