@echo off
setlocal

set BASE=%~dp0

REM ���� ����(Debug/Release) ��������
set CONFIG=%1
if "%CONFIG%"=="" (
    set CONFIG=Debug
)

REM ������ ������ ��ٸ��� �Լ�
call :WaitForFile "%BASE%Engine\Bin\Engine.dll" 10
call :WaitForFile "%BASE%Engine\Bin\Engine.lib" 10

REM ���� ����
xcopy /y "%BASE%Engine\Header\*.*"         "%BASE%Reference\Header\"
xcopy /y "%BASE%Engine\Bin\Engine.dll"     "%BASE%Client\Bin\"
xcopy /y "%BASE%Engine\Bin\Engine.lib"     "%BASE%Reference\Library\"

REM FMOD DLL ���� (Debug / Release ����)
if /I "%CONFIG%"=="Debug" (
    xcopy /y "%BASE%ThirdParty\FMOD\fmodL.dll" "%BASE%Client\Bin\" >nul
) else (
    xcopy /y "%BASE%ThirdParty\FMOD\fmod.dll"  "%BASE%Client\Bin\" >nul
)

goto :eof

:WaitForFile
set file=%~1
set retry=%~2
set count=0

:loop
if exist %file% (
    echo Found %file%
    goto :eof
)

set /a count+=1
if %count% GEQ %retry% (
    echo [ERROR] %file% not found after %retry% seconds.
    exit /b 1
)

echo Waiting for %file% ...
timeout /t 1 > nul
goto loop
