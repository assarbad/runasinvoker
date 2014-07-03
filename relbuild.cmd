@echo off
if "%~1" == "noinc" set NO_INCREASE_BUILDNO=1
call ddkbuild.cmd -WNETXP fre . -cZ
for %%i in (*.zip *.zip.asc bin\*.ilk bin\*.idb bin\*_dbg.*) do @del /f %%i
call ollisign.cmd "%~dp0\bin\*.exe" "http://assarbad.net" "RunAsInvoker launcher for Image Execution Options"
:: Find 7-Zip
set SEVENZIP=%ProgramFiles%\7-Zip\7z.exe
if not exist "%SEVENZIP%" set SEVENZIP=%ProgramFiles(x86)%\7-Zip\7z.exe
if not exist "%SEVENZIP%" ( echo ERROR: Could not find 7z.exe & goto :EOF )
set ARCHIVE=runasinvoker.zip
for %%i in (%ARCHIVE% %ARCHIVE%.asc) do @del /f %%i
"%SEVENZIP%" a -tzip %ARCHIVE% bin BUILD makefile sources *.cmd *.cpp *.h *.rc *.rst *.sln *.vcproj *.vcxproj *.vsprops
sha1sum %ARCHIVE%
md5sum %ARCHIVE%
gpg -a --detach-sign %ARCHIVE%
