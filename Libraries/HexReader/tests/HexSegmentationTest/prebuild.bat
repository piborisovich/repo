@ECHO OFF

echo "=============================PREBUILD============================"

set APP_DIR=%cd%
set ROOT_DIR=%cd%\..\..\..\..
set WORK_DIR=%ROOT_DIR%\..
set RESOURCES_DIR=%APP_DIR%\resources
set BIN_DIR=%WORK_DIR%\BIN\tests\HexReaderLib\HexSegmentationTest

cd %RESOURCES_DIR%
xcopy * %BIN_DIR% /E/Y

EXIT /B 0