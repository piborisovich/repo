@ECHO OFF
SETLOCAL EnableExtensions EnableDelayedExpansion
for /F %%a in ('echo prompt $E ^| cmd') do (
  set "ESC=%%a"
)

set BLACK_ESC=%ESC%[30m
set RED_ESC=%ESC%[31m
set GREEN_ESC=%ESC%[32m
set YELLOW_ESC=%ESC%[33m
set BLUE_ESC=%ESC%[34m
set MAGENTA_ESC=%ESC%[35m
set CYAN_ESC=%ESC%[36m
set WHITE_ESC=%ESC%[37m
set END_ESC=%ESC%[0m

CALL :get_build_version BUILD_VERSION

SET PATH=C:\windows\system32;

set ROOT_DIR=%cd%\..\..\..\..
set BUILD_TYPE=release

if "%1" EQU "debug" (
	set BUILD_TYPE=%1
)

set TARGET_NAME=HexSegmentationTest
set TEST_PROJECT_NAME=HexReaderLib
set APPLICATION_DIR=%cd%
set APPLICATION_FILE_NAME=%TARGET_NAME%.exe

set LIBS_TO_BUILD=%ROOT_DIR%\Lib\CoreLib ^
%ROOT_DIR%\Lib\HexReader

CALL :Display Blue "=============================BUILD_SCRIPT============================"
CALL :Display Magenta "Build type: %BUILD_TYPE%"

set WORK_DIR=%ROOT_DIR%\..
set BIN_DIR=%WORK_DIR%\BIN\tests\%TEST_PROJECT_NAME%\%TARGET_NAME%
set LIB_DIR=%WORK_DIR%\LIB

::READ QT DIRECTORY
set DEFAULT_QT_DIR=C:\Qt
set /p QT_DIR=Enter Qt dir and/or press enter (default=%DEFAULT_QT_DIR%):

IF [%QT_DIR%]==[] (
	set QT_DIR=%DEFAULT_QT_DIR%
) ELSE IF NOT exist %QT_DIR%\ (
	CALL :Display Red "Directory %QT_DIR% not exists. Using default"
	set QT_DIR=%DEFAULT_QT_DIR%
	) 
)

cd %WORK_DIR%

echo Work dir is %GREEN_ESC%%cd%       %END_ESC%
echo Qt dir is   %GREEN_ESC%%QT_DIR%   %END_ESC%

CALL :SearchAndSelectDir %QT_DIR% "qmake.exe"
if %errorlevel% NEQ 0 EXIT /B 1

CALL :SearchAndSelectDir %QT_DIR% "g++.exe"
if %errorlevel% NEQ 0 EXIT /B 1

CALL :Display Yellow "%PATH%"

set BUILD_DIR=%WORK_DIR%\build

echo BUILD_DIR: %GREEN_ESC%%BUILD_DIR%%END_ESC%
echo BIN_DIR: %GREEN_ESC%%BIN_DIR%%END_ESC%

@PAUSE

CALL :backupLocalPri

rmdir /S /Q %BIN_DIR%
rmdir /S /Q %BUILD_DIR%

cd %LIB_DIR%

del "*.dll"
del "*.a"

for %%x in (%LIBS_TO_BUILD%) do (
	cd %%x
	echo:
	echo Directory to build !MAGENTA_ESC!!cd! !END_ESC!
	echo:
	
	cmd /c mingw32-make clean -j8
	
	cmd /c qmake "CONFIG+=%BUILD_TYPE%" && mingw32-make -j8 qmake_all
	if !errorlevel! NEQ 0 (
		CALL :Display Red "Error: !errorlevel!"
		
		CALL :restoreLocalPri
		
		EXIT /B 1
	)
	
	cmd /c mingw32-make -j8
	if !errorlevel! NEQ 0 (
		CALL :Display Red "Error: !errorlevel!"
		
		CALL :restoreLocalPri
		
		EXIT /B 1
	)
)

cd %APPLICATION_DIR%

echo:
	echo Application build !MAGENTA_ESC!!cd! !END_ESC!
	echo:
	
	cmd /c mingw32-make clean -j8
	
	cmd /c qmake "CONFIG+=%BUILD_TYPE%" && mingw32-make -j8 qmake_all
	if !errorlevel! NEQ 0 (
		CALL :Display Red "Error: !errorlevel!"
		
		CALL :restoreLocalPri
		
		EXIT /B 1
	)
	
	cmd /c mingw32-make -j8
	if !errorlevel! NEQ 0 (
		CALL :Display Red "Error: !errorlevel!"
		
		CALL :restoreLocalPri
		
		EXIT /B 1
	)

move /Y %LIB_DIR%\*.dll %BIN_DIR%

cd %BIN_DIR%
cmd /c windeployqt %APPLICATION_FILE_NAME% --no-translations --no-quick-import --no-patchqt --no-system-d3d-compiler --no-webkit2 --no-opengl-sw --no-plugins

rmdir /S /Q %BUILD_DIR%

CALL :restoreLocalPri

@pause

EXIT /B 0

endlocal

:SearchAndSelectDir

CALL :Display Magenta "Search %~2. Please wait..."

SET counter=0
SET LIST_DIR=[]

for /F "tokens=* delims=" %%c in ('where /r %~1 %~2') do (
	SET /A counter+=1
	SET LIST_DIR[!counter!]=%%c
	CALL :Display Cyan "!counter! - %%c"
)

if %counter% EQU 0 (
	CALL :Display Red "%~2 not found"
	EXIT /B 1
)

SET ind=1

if %counter% NEQ 1 (
	if %counter% GTR 1 (
		set /p ind="Enter %~2 dir number and press enter (default:1) "
		if !ind! LSS 1 (
			CALL :Display Red "Number not correct"
			EXIT /B 1
		)
		
		if !ind! GTR !counter! (
			CALL :Display Red "Number not correct"
			EXIT /B 1
		)
	)
)

set CUR_DIR=!LIST_DIR[%ind%]!

for %%F in ("%CUR_DIR%") do set dirname=%%~dpF

SET PATH=%PATH%%dirname%;

EXIT /B 0


:Display 

if %~1==Black (
	echo !BLACK_ESC!%~2!END_ESC!
) else if %~1==Red ( 
	echo !RED_ESC!%~2!END_ESC!
) else if %~1==Green ( 
	echo !GREEN_ESC!%~2!END_ESC!
) else if %~1==Yellow ( 
	echo !YELLOW_ESC!%~2!END_ESC!
) else if %~1==Blue (
	echo !BLUE_ESC!%~2!END_ESC!
) else if %~1==Magenta (
	echo !MAGENTA_ESC!%~2!END_ESC!
) else if %~1==Cyan (
	echo !CYAN_ESC!%~2!END_ESC!
) else if %~1==White (
	echo !WHITE_ESC!%~2!END_ESC!
) else (
	echo %~2
)

EXIT /B 0

:get_build_version

set %~1=
set git_version=

for /F "delims=- tokens=1,2" %%a in ('git describe') do (
	set git_version=%%a.%%b
)

if [%git_version%] NEQ [] (
	set %~1=%git_version:~1%
)

EXIT /B 0

:backupLocalPri

set LOCAL_PATH=%ROOT_DIR%\local.pri

if exist %LOCAL_PATH% (
	move /Y %LOCAL_PATH% %LOCAL_PATH%_backup
	
	type NUL > %LOCAL_PATH%

	echo QT-=gui>>%LOCAL_PATH%
)
    
EXIT /B 0


:restoreLocalPri

set LOCAL_PATH="%ROOT_DIR%\local.pri"
set BACKUP_LOCAL_PATH="%ROOT_DIR%\local.pri_backup"

if exist %BACKUP_LOCAL_PATH% (
	del %LOCAL_PATH%
	move /Y %BACKUP_LOCAL_PATH% %LOCAL_PATH%
)
